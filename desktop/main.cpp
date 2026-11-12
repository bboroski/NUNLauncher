#include <QApplication>
#include <QCoreApplication>
#include <QDir>
#include <QFileDialog>
#include <QFileInfo>
#include <QFormLayout>
#include <QFont>
#include <QGroupBox>
#include <QHBoxLayout>
#include <QLabel>
#include <QLineEdit>
#include <QMainWindow>
#include <QMessageBox>
#include <QPlainTextEdit>
#include <QProcess>
#include <QProcessEnvironment>
#include <QPushButton>
#include <QSettings>
#include <QStandardPaths>
#include <QTabWidget>
#include <QTextCursor>
#include <QTextStream>
#include <QVBoxLayout>

#ifndef NUNLAUNCHER_VERSION
#define NUNLAUNCHER_VERSION "0.2.0"
#endif

namespace {
QString theme()
{
    return QStringLiteral(R"(
      * { font-family: "Segoe UI", "Inter", sans-serif; }
      QMainWindow, QWidget { background:#0D0D0D; color:#F2F2F2; }
      QTabWidget::pane { border:1px solid #303030; border-radius:10px; }
      QTabBar::tab { background:#171717; color:#C4C4C4; padding:12px 18px; margin-right:4px; }
      QTabBar::tab:selected { color:#FFD600; background:#242424; border-bottom:2px solid #FFD600; }
      QGroupBox { border:1px solid #303030; border-radius:12px; margin-top:14px; padding:14px; background:#171717; }
      QGroupBox::title { subcontrol-origin:margin; left:12px; padding:0 5px; color:#FFD600; }
      QLabel[muted="true"] { color:#A3A3A3; }
      QLineEdit, QPlainTextEdit { background:#101010; color:#F2F2F2; border:1px solid #484848; border-radius:8px; padding:9px; }
      QLineEdit:focus, QPlainTextEdit:focus { border:2px solid #FFD600; }
      QPushButton { background:#242424; color:#F2F2F2; border:1px solid #484848; border-radius:8px; padding:10px 14px; min-height:24px; font-weight:600; }
      QPushButton:hover { background:#2B2B2B; border-color:#FFD600; }
      QPushButton:focus { border:2px solid #FFD600; }
      QPushButton[primary="true"] { background:#FFD600; color:#0D0D0D; border-color:#FFD600; }
      QPushButton:disabled { color:#777777; background:#1A1A1A; }
      QPlainTextEdit { font-family:"Cascadia Code","Consolas",monospace; font-size:12px; }
    )");
}

QPushButton *button(const QString &text, bool primary = false)
{
    auto *result = new QPushButton(text);
    result->setProperty("primary", primary);
    result->setMinimumHeight(44);
    return result;
}

QLabel *muted(const QString &text)
{
    auto *label = new QLabel(text);
    label->setProperty("muted", true);
    label->setWordWrap(true);
    return label;
}

class Workbench final : public QMainWindow
{
public:
    Workbench() : process_(new QProcess(this))
    {
        setWindowTitle(QStringLiteral("NUN Workbench"));
        resize(1120, 780);
        setMinimumSize(860, 620);
        setStyleSheet(theme());
        buildUi();
        restoreSettings();
        connectProcess();
        updateWorkspace();
        log(QStringLiteral("NUN Workbench %1 — host-side engineering console").arg(QStringLiteral(NUNLAUNCHER_VERSION)));
        log(QStringLiteral("The host application does not load or execute a kernel image."));
    }

    ~Workbench() override
    {
        if (process_->state() != QProcess::NotRunning) {
            process_->terminate();
            if (!process_->waitForFinished(1200)) process_->kill();
        }
        QSettings settings(QStringLiteral("NUN"), QStringLiteral("NUNWorkbench"));
        settings.setValue(QStringLiteral("nunRoot"), nunRoot_->text());
        settings.setValue(QStringLiteral("image"), image_->text());
        settings.setValue(QStringLiteral("manifest"), manifest_->text());
        settings.setValue(QStringLiteral("qemuImage"), qemuImage_->text());
    }

private:
    QProcess *process_;
    QLineEdit *nunRoot_ = nullptr;
    QLineEdit *image_ = nullptr;
    QLineEdit *manifest_ = nullptr;
    QLineEdit *qemuImage_ = nullptr;
    QLabel *workspaceStatus_ = nullptr;
    QLabel *runStatus_ = nullptr;
    QPlainTextEdit *output_ = nullptr;
    QPushButton *stop_ = nullptr;
    QString launcherRoot_;
    QString task_;

    QString root() const { return QDir::cleanPath(nunRoot_->text().trimmed()); }

    QString findLauncherRoot() const
    {
        QStringList candidates;
#ifdef NUNLAUNCHER_SOURCE_DIR
        candidates << QStringLiteral(NUNLAUNCHER_SOURCE_DIR);
#endif
        candidates << QCoreApplication::applicationDirPath() << QDir::currentPath();
        for (const QString &candidate : candidates) {
            QDir directory(candidate);
            for (int i = 0; i < 6; ++i) {
                if (QFileInfo::exists(directory.filePath(QStringLiteral("nunlauncher_boot/cli.py"))))
                    return directory.absolutePath();
                if (!directory.cdUp()) break;
            }
        }
        return {};
    }

    QString python() const
    {
        QString value = QStandardPaths::findExecutable(QStringLiteral("python3"));
        if (value.isEmpty()) value = QStandardPaths::findExecutable(QStringLiteral("python"));
        return value;
    }

    void buildUi()
    {
        auto *central = new QWidget();
        auto *layout = new QVBoxLayout(central);
        layout->setContentsMargins(22, 18, 22, 18);
        layout->setSpacing(12);

        auto *header = new QHBoxLayout();
        auto *titles = new QVBoxLayout();
        auto *title = new QLabel(QStringLiteral("NUN Workbench"));
        QFont font = title->font(); font.setPointSize(22); font.setBold(true); title->setFont(font);
        titles->addWidget(title);
        titles->addWidget(muted(QStringLiteral("Build · image admission · QEMU · diagnostics")));
        header->addLayout(titles, 1);
        runStatus_ = new QLabel(QStringLiteral("IDLE"));
        runStatus_->setAlignment(Qt::AlignCenter);
        runStatus_->setMinimumWidth(100);
        runStatus_->setStyleSheet(QStringLiteral("color:#FFD600;background:#332B00;border-radius:8px;padding:8px;"));
        header->addWidget(runStatus_, 0, Qt::AlignTop);
        layout->addLayout(header);

        auto *workspace = new QGroupBox(QStringLiteral("NUN workspace"));
        auto *workspaceRow = new QHBoxLayout(workspace);
        nunRoot_ = new QLineEdit();
        nunRoot_->setPlaceholderText(QStringLiteral("Select local NUN source directory"));
        nunRoot_->setAccessibleName(QStringLiteral("NUN source directory"));
        auto *browseRoot = button(QStringLiteral("Browse…"));
        workspaceStatus_ = muted(QStringLiteral("Workspace not selected"));
        connect(browseRoot, &QPushButton::clicked, this, [this] {
            const QString chosen = QFileDialog::getExistingDirectory(this, QStringLiteral("Select NUN repository"), root());
            if (!chosen.isEmpty()) { nunRoot_->setText(chosen); updateWorkspace(); }
        });
        connect(nunRoot_, &QLineEdit::editingFinished, this, [this] { updateWorkspace(); });
        workspaceRow->addWidget(nunRoot_, 1);
        workspaceRow->addWidget(browseRoot);
        workspaceRow->addWidget(workspaceStatus_);
        layout->addWidget(workspace);

        auto *tabs = new QTabWidget();
        tabs->addTab(buildOverview(), QStringLiteral("Overview"));
        tabs->addTab(buildImageGate(), QStringLiteral("Image Gate"));
        tabs->addTab(buildBuildTab(), QStringLiteral("Build & Test"));
        tabs->addTab(buildQemuTab(), QStringLiteral("QEMU Lab"));
        layout->addWidget(tabs, 1);

        auto *console = new QGroupBox(QStringLiteral("Execution output"));
        auto *consoleLayout = new QVBoxLayout(console);
        output_ = new QPlainTextEdit();
        output_->setReadOnly(true);
        output_->setMaximumBlockCount(2500);
        output_->setMinimumHeight(145);
        output_->setAccessibleName(QStringLiteral("Build and diagnostic output"));
        consoleLayout->addWidget(output_);
        auto *consoleActions = new QHBoxLayout();
        auto *clear = button(QStringLiteral("Clear output"));
        stop_ = button(QStringLiteral("Stop process"));
        stop_->setEnabled(false);
        connect(clear, &QPushButton::clicked, output_, &QPlainTextEdit::clear);
        connect(stop_, &QPushButton::clicked, this, [this] { stopProcess(); });
        consoleActions->addWidget(clear);
        consoleActions->addStretch(1);
        consoleActions->addWidget(stop_);
        consoleLayout->addLayout(consoleActions);
        layout->addWidget(console);
        setCentralWidget(central);
    }

    QWidget *buildOverview()
    {
        auto *page = new QWidget();
        auto *layout = new QVBoxLayout(page);
        layout->setContentsMargins(16, 16, 16, 16);
        auto *scope = new QGroupBox(QStringLiteral("Platform status"));
        auto *scopeLayout = new QVBoxLayout(scope);
        scopeLayout->addWidget(new QLabel(QStringLiteral(
            "NUN is an early x86_64 kernel bring-up project. This workbench runs on Windows/Linux host environments "
            "and coordinates existing build, validation and emulator tools.")));
        scopeLayout->addWidget(muted(QStringLiteral(
            "This is not the NUN operating-system desktop. Kernel GUI, scheduler, VFS, networking, graphics drivers "
            "and userspace are not claimed as implemented.")));
        layout->addWidget(scope);

        auto *actions = new QGroupBox(QStringLiteral("Quick actions"));
        auto *grid = new QGridLayout(actions);
        auto *iso = button(QStringLiteral("Build Limine ISO"), true);
        auto *qemu = button(QStringLiteral("Build QEMU image"));
        auto *tests = button(QStringLiteral("Test NUNLauncher"));
        connect(iso, &QPushButton::clicked, this, [this] { buildIso(); });
        connect(qemu, &QPushButton::clicked, this, [this] { buildQemu(); });
        connect(tests, &QPushButton::clicked, this, [this] { runLauncherTests(); });
        grid->addWidget(iso, 0, 0); grid->addWidget(qemu, 0, 1); grid->addWidget(tests, 0, 2);
        layout->addWidget(actions);
        layout->addStretch(1);
        return page;
    }

    QWidget *buildImageGate()
    {
        auto *page = new QWidget();
        auto *layout = new QVBoxLayout(page);
        layout->setContentsMargins(16, 16, 16, 16);
        auto *formBox = new QGroupBox(QStringLiteral("Read-only pre-handoff verification"));
        auto *form = new QFormLayout(formBox);
        image_ = new QLineEdit(); image_->setPlaceholderText(QStringLiteral("Path to NUN.iso"));
        auto *imageRow = new QHBoxLayout();
        auto *imageBrowse = button(QStringLiteral("Browse…"));
        connect(imageBrowse, &QPushButton::clicked, this, [this] { chooseFile(image_, QStringLiteral("ISO images (*.iso);;All files (*)")); });
        imageRow->addWidget(image_, 1); imageRow->addWidget(imageBrowse);
        form->addRow(QStringLiteral("Boot image"), imageRow);

        manifest_ = new QLineEdit(); manifest_->setPlaceholderText(QStringLiteral("Path to NUN.image.json"));
        auto *manifestRow = new QHBoxLayout();
        auto *manifestBrowse = button(QStringLiteral("Browse…"));
        connect(manifestBrowse, &QPushButton::clicked, this, [this] { chooseFile(manifest_, QStringLiteral("JSON manifests (*.json);;All files (*)")); });
        manifestRow->addWidget(manifest_, 1); manifestRow->addWidget(manifestBrowse);
        form->addRow(QStringLiteral("Manifest"), manifestRow);
        layout->addWidget(formBox);

        auto *meaning = new QGroupBox(QStringLiteral("Validation boundary"));
        auto *meaningLayout = new QVBoxLayout(meaning);
        meaningLayout->addWidget(muted(QStringLiteral(
            "Delegates to the existing NUNLauncher verifier. It checks the selected image and manifest but does not "
            "load or execute a kernel, write to a disk, authenticate the publisher or qualify physical hardware.")));
        layout->addWidget(meaning);
        auto *verify = button(QStringLiteral("Verify image"), true);
        connect(verify, &QPushButton::clicked, this, [this] { verifyImage(); });
        layout->addWidget(verify, 0, Qt::AlignLeft);
        layout->addStretch(1);
        return page;
    }

    QWidget *buildBuildTab()
    {
        auto *page = new QWidget();
        auto *layout = new QVBoxLayout(page);
        layout->setContentsMargins(16, 16, 16, 16);
        auto *buildBox = new QGroupBox(QStringLiteral("Build targets"));
        auto *buildLayout = new QVBoxLayout(buildBox);
        auto *limine = button(QStringLiteral("Build hybrid BIOS / UEFI ISO"));
        auto *multiboot = button(QStringLiteral("Build Multiboot2 QEMU image"), true);
        connect(limine, &QPushButton::clicked, this, [this] { buildIso(); });
        connect(multiboot, &QPushButton::clicked, this, [this] { buildQemu(); });
        buildLayout->addWidget(limine);
        buildLayout->addWidget(muted(QStringLiteral("Runs make iso in the selected NUN repository.")));
        buildLayout->addWidget(multiboot);
        buildLayout->addWidget(muted(QStringLiteral("Runs the existing scripts/build-qemu.sh target.")));
        layout->addWidget(buildBox);

        auto *testsBox = new QGroupBox(QStringLiteral("Host tests"));
        auto *testsLayout = new QVBoxLayout(testsBox);
        auto *nunTests = button(QStringLiteral("Run NUN host tests"));
        auto *launcherTests = button(QStringLiteral("Run NUNLauncher tests"));
        connect(nunTests, &QPushButton::clicked, this, [this] { runNUNTests(); });
        connect(launcherTests, &QPushButton::clicked, this, [this] { runLauncherTests(); });
        testsLayout->addWidget(nunTests);
        testsLayout->addWidget(launcherTests);
        layout->addWidget(testsBox);
        layout->addStretch(1);
        return page;
    }

    QWidget *buildQemuTab()
    {
        auto *page = new QWidget();
        auto *layout = new QVBoxLayout(page);
        layout->setContentsMargins(16, 16, 16, 16);
        auto *box = new QGroupBox(QStringLiteral("Multiboot2 QEMU target"));
        auto *form = new QFormLayout(box);
        qemuImage_ = new QLineEdit(); qemuImage_->setPlaceholderText(QStringLiteral("build/qemu/nun-qemu.iso"));
        auto *row = new QHBoxLayout();
        auto *browse = button(QStringLiteral("Browse…"));
        connect(browse, &QPushButton::clicked, this, [this] { chooseFile(qemuImage_, QStringLiteral("ISO images (*.iso);;All files (*)")); });
        row->addWidget(qemuImage_, 1); row->addWidget(browse);
        form->addRow(QStringLiteral("QEMU image"), row);
        layout->addWidget(box);
        layout->addWidget(muted(QStringLiteral(
            "Launch invokes NUN scripts/run-qemu.sh and requires bash plus qemu-system-x86_64 on PATH. "
            "The emulator is a separate process and can be stopped from this window.")));
        auto *build = button(QStringLiteral("Build QEMU image"));
        auto *launch = button(QStringLiteral("Launch QEMU"), true);
        connect(build, &QPushButton::clicked, this, [this] { buildQemu(); });
        connect(launch, &QPushButton::clicked, this, [this] { launchQemu(); });
        layout->addWidget(build, 0, Qt::AlignLeft);
        layout->addWidget(launch, 0, Qt::AlignLeft);
        layout->addStretch(1);
        return page;
    }

    void restoreSettings()
    {
        launcherRoot_ = findLauncherRoot();
        QSettings settings(QStringLiteral("NUN"), QStringLiteral("NUNWorkbench"));
        nunRoot_->setText(settings.value(QStringLiteral("nunRoot")).toString());
        image_->setText(settings.value(QStringLiteral("image")).toString());
        manifest_->setText(settings.value(QStringLiteral("manifest")).toString());
        qemuImage_->setText(settings.value(QStringLiteral("qemuImage")).toString());
        if (nunRoot_->text().isEmpty() && !launcherRoot_.isEmpty()) {
            QDir sibling(launcherRoot_);
            if (sibling.cdUp() && sibling.exists(QStringLiteral("NUN/CMakeLists.txt"))) {
                sibling.cd(QStringLiteral("NUN"));
                nunRoot_->setText(sibling.absolutePath());
            }
        }
        if (qemuImage_->text().isEmpty() && !nunRoot_->text().isEmpty())
            qemuImage_->setText(QDir(root()).filePath(QStringLiteral("build/qemu/nun-qemu.iso")));
    }

    void updateWorkspace()
    {
        const QString path = root();
        const bool valid = QFileInfo::exists(QDir(path).filePath(QStringLiteral("CMakeLists.txt")))
            && QFileInfo::exists(QDir(path).filePath(QStringLiteral("scripts/run-qemu.sh")));
        workspaceStatus_->setText(valid ? QStringLiteral("NUN workspace found") : QStringLiteral("Invalid workspace"));
        workspaceStatus_->setStyleSheet(valid ? QStringLiteral("color:#8FD694;") : QStringLiteral("color:#FF8585;"));
        if (valid && qemuImage_->text().isEmpty())
            qemuImage_->setText(QDir(path).filePath(QStringLiteral("build/qemu/nun-qemu.iso")));
    }

    bool requireWorkspace()
    {
        updateWorkspace();
        if (!QFileInfo::exists(QDir(root()).filePath(QStringLiteral("CMakeLists.txt")))) {
            QMessageBox::warning(this, QStringLiteral("NUN workspace required"), QStringLiteral("Select the local NUN repository first."));
            return false;
        }
        return true;
    }

    void chooseFile(QLineEdit *field, const QString &filter)
    {
        const QString selected = QFileDialog::getOpenFileName(this, QStringLiteral("Select file"), field->text(), filter);
        if (!selected.isEmpty()) field->setText(selected);
    }

    void log(const QString &line) { output_->appendPlainText(line); }

    void start(const QString &program, const QStringList &args, const QString &cwd,
               const QString &label, QProcessEnvironment env = QProcessEnvironment::systemEnvironment())
    {
        if (process_->state() != QProcess::NotRunning) {
            QMessageBox::information(this, QStringLiteral("Task running"), QStringLiteral("Stop the active task first."));
            return;
        }
        if (!QFileInfo(cwd).isDir()) {
            QMessageBox::warning(this, QStringLiteral("Directory missing"), cwd);
            return;
        }
        QString executable = program;
        if (QFileInfo(program).isAbsolute() || program.contains(QLatin1Char('/')) || program.contains(QLatin1Char('\\'))) {
            if (!QFileInfo(program).isFile()) {
                QMessageBox::warning(this, QStringLiteral("Executable missing"), program);
                return;
            }
        } else {
            executable = QStandardPaths::findExecutable(program);
            if (executable.isEmpty()) {
                QMessageBox::warning(this, QStringLiteral("Tool missing"), program + QStringLiteral(" is not on PATH."));
                return;
            }
        }

        task_ = label;
        runStatus_->setText(QStringLiteral("RUNNING"));
        runStatus_->setStyleSheet(QStringLiteral("color:#FFD600;background:#332B00;border-radius:8px;padding:8px;"));
        stop_->setEnabled(true);
        log(QStringLiteral("\n> %1").arg(label));
        log(QStringLiteral("Working directory: %1").arg(QDir::toNativeSeparators(cwd)));
        process_->setWorkingDirectory(cwd);
        process_->setProcessEnvironment(std::move(env));
        process_->setProcessChannelMode(QProcess::MergedChannels);
        process_->setProgram(executable);
        process_->setArguments(args);
        process_->start();
    }

    QProcessEnvironment launcherEnvironment() const
    {
        QProcessEnvironment env = QProcessEnvironment::systemEnvironment();
        const QString previous = env.value(QStringLiteral("PYTHONPATH"));
        env.insert(QStringLiteral("PYTHONPATH"), launcherRoot_ +
                   (previous.isEmpty() ? QString() : QString(QDir::listSeparator()) + previous));
        return env;
    }

    void connectProcess()
    {
        connect(process_, &QProcess::readyReadStandardOutput, this, [this] {
            const QString text = QString::fromLocal8Bit(process_->readAllStandardOutput());
            output_->moveCursor(QTextCursor::End);
            output_->insertPlainText(text);
            output_->moveCursor(QTextCursor::End);
        });
        connect(process_, &QProcess::finished, this, [this](int code, QProcess::ExitStatus status) {
            const bool ok = status == QProcess::NormalExit && code == 0;
            log(ok ? QStringLiteral("\n[OK] %1 completed").arg(task_)
                   : QStringLiteral("\n[FAILED] %1 (exit %2)").arg(task_).arg(code));
            runStatus_->setText(ok ? QStringLiteral("COMPLETE") : QStringLiteral("FAILED"));
            runStatus_->setStyleSheet(ok
                ? QStringLiteral("color:#8FD694;background:#17311D;border-radius:8px;padding:8px;")
                : QStringLiteral("color:#FF8585;background:#351717;border-radius:8px;padding:8px;"));
            stop_->setEnabled(false);
            task_.clear();
        });
        connect(process_, &QProcess::errorOccurred, this, [this](QProcess::ProcessError error) {
            if (error == QProcess::FailedToStart) {
                log(QStringLiteral("[ERROR] %1").arg(process_->errorString()));
                runStatus_->setText(QStringLiteral("ERROR"));
                stop_->setEnabled(false);
                task_.clear();
            }
        });
    }

    void stopProcess()
    {
        if (process_->state() == QProcess::NotRunning) return;
        log(QStringLiteral("\n[STOP] Terminating %1").arg(task_));
        process_->terminate();
        if (!process_->waitForFinished(1200)) process_->kill();
    }

    void buildIso()
    {
        if (!requireWorkspace()) return;
        start(QStringLiteral("make"), {QStringLiteral("iso")}, root(), QStringLiteral("Build NUN hybrid BIOS / UEFI ISO"));
    }

    void buildQemu()
    {
        if (!requireWorkspace()) return;
        const QString bash = QStandardPaths::findExecutable(QStringLiteral("bash"));
        if (bash.isEmpty()) {
            QMessageBox::warning(this, QStringLiteral("Bash missing"), QStringLiteral("Install bash (or Git Bash on Windows) and add it to PATH."));
            return;
        }
        start(bash, {QStringLiteral("scripts/build-qemu.sh")}, root(), QStringLiteral("Build NUN Multiboot2 QEMU image"));
    }

    void launchQemu()
    {
        if (!requireWorkspace()) return;
        const QString bash = QStandardPaths::findExecutable(QStringLiteral("bash"));
        const QString path = QDir::cleanPath(qemuImage_->text().trimmed());
        if (bash.isEmpty() || !QFileInfo(path).isFile()) {
            QMessageBox::warning(this, QStringLiteral("QEMU unavailable"), QStringLiteral("Ensure bash is installed and select an existing QEMU ISO."));
            return;
        }
        start(bash, {QStringLiteral("scripts/run-qemu.sh"), path}, root(), QStringLiteral("Launch NUN in QEMU"));
    }

    void runNUNTests()
    {
        if (!requireWorkspace()) return;
        start(QStringLiteral("make"), {QStringLiteral("test")}, root(), QStringLiteral("Run NUN host tests"));
    }

    void runLauncherTests()
    {
        const QString py = python();
        if (launcherRoot_.isEmpty() || py.isEmpty()) {
            QMessageBox::warning(this, QStringLiteral("Launcher test prerequisites"), QStringLiteral("Run from a NUNLauncher source checkout with Python 3 available."));
            return;
        }
        start(py, {QStringLiteral("-m"), QStringLiteral("unittest"), QStringLiteral("discover"),
                   QStringLiteral("-s"), QStringLiteral("tests"), QStringLiteral("-v")},
              launcherRoot_, QStringLiteral("Run NUNLauncher tests"), launcherEnvironment());
    }

    void verifyImage()
    {
        const QString py = python();
        const QString img = QDir::cleanPath(image_->text().trimmed());
        const QString manifest = QDir::cleanPath(manifest_->text().trimmed());
        if (launcherRoot_.isEmpty() || py.isEmpty() || !QFileInfo(img).isFile() || !QFileInfo(manifest).isFile()) {
            QMessageBox::warning(this, QStringLiteral("Verification prerequisites"),
                                 QStringLiteral("Select an existing image and manifest, and ensure Python 3 and the NUNLauncher source are available."));
            return;
        }
        start(py, {QStringLiteral("-m"), QStringLiteral("nunlauncher_boot.cli"), QStringLiteral("verify"),
                   QStringLiteral("--image"), img, QStringLiteral("--manifest"), manifest},
              launcherRoot_, QStringLiteral("Verify NUN boot image"), launcherEnvironment());
    }
};
} // namespace

int main(int argc, char *argv[])
{
    QApplication application(argc, argv);
    if (application.arguments().contains(QStringLiteral("--version"))) {
        QTextStream(stdout) << "NUN Workbench " << NUNLAUNCHER_VERSION << Qt::endl;
        return 0;
    }
    Workbench window;
    window.show();
    return application.exec();
}
