#include <QApplication>
#include <QCryptographicHash>
#include <QDateTime>
#include <QDir>
#include <QDirIterator>
#include <QFile>
#include <QFileDialog>
#include <QFileInfo>
#include <QFont>
#include <QAbstractItemView>
#include <QHBoxLayout>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QLabel>
#include <QLineEdit>
#include <QMainWindow>
#include <QMessageBox>
#include <QProcess>
#include <QPushButton>
#include <QSysInfo>
#include <QTableWidget>
#include <QHeaderView>
#include <QVBoxLayout>
#include <QUuid>
#include <QWidget>

namespace {
QString hashText(const QByteArray &value)
{
    return QString::fromLatin1(QCryptographicHash::hash(value, QCryptographicHash::Sha256).toHex());
}

class AutoImproveStudio final : public QMainWindow
{
public:
    AutoImproveStudio()
    {
        setWindowTitle(QStringLiteral("NUN AutoImprove Studio"));
        resize(1040, 700);
        setMinimumSize(820, 560);
        setStyleSheet(QStringLiteral(
            "QMainWindow,QWidget{background:#0d0d0d;color:#f2f2f2;font-family:'Segoe UI',sans-serif;}"
            "QLineEdit,QTableWidget{background:#151515;color:#f2f2f2;border:1px solid #454545;border-radius:6px;padding:7px;}"
            "QHeaderView::section{background:#242424;color:#ffd600;padding:8px;border:0;}"
            "QPushButton{background:#242424;color:#f2f2f2;border:1px solid #555;border-radius:7px;padding:10px 14px;}"
            "QPushButton:hover{border-color:#ffd600;}"));
        buildUi();
    }

private:
    QLineEdit *repository_ = nullptr;
    QLabel *status_ = nullptr;
    QLabel *host_ = nullptr;
    QTableWidget *matrixTable_ = nullptr;
    QPushButton *refineButton_ = nullptr;
    QPushButton *exportButton_ = nullptr;
    QJsonObject matrix_;
    int version_ = 0;

    void buildUi()
    {
        auto *central = new QWidget();
        auto *layout = new QVBoxLayout(central);
        layout->setContentsMargins(22, 20, 22, 20);
        auto *title = new QLabel(QStringLiteral("AutoImprove Studio"));
        QFont titleFont = title->font();
        titleFont.setPointSize(22);
        titleFont.setBold(true);
        title->setFont(titleFont);
        layout->addWidget(title);
        auto *subtitle = new QLabel(QStringLiteral(
            "Three-stage ternary optimization · local-first · evidence-backed · proposal-only"));
        subtitle->setStyleSheet(QStringLiteral("color:#b0b0b0;"));
        layout->addWidget(subtitle);

        auto *repoRow = new QHBoxLayout();
        repository_ = new QLineEdit();
        repository_->setPlaceholderText(QStringLiteral("Select a local repository/workspace"));
        auto *browse = new QPushButton(QStringLiteral("Browse…"));
        connect(browse, &QPushButton::clicked, this, [this] {
            const QString selected = QFileDialog::getExistingDirectory(this, QStringLiteral("Select repository"));
            if (!selected.isEmpty()) repository_->setText(selected);
        });
        repoRow->addWidget(repository_, 1);
        repoRow->addWidget(browse);
        layout->addLayout(repoRow);

        host_ = new QLabel(QStringLiteral("Host: %1 · %2")
            .arg(QSysInfo::prettyProductName(), QSysInfo::currentCpuArchitecture()));
        host_->setStyleSheet(QStringLiteral("color:#b0b0b0;"));
        layout->addWidget(host_);

        matrixTable_ = new QTableWidget(0, 4);
        matrixTable_->setHorizontalHeaderLabels({QStringLiteral("Stage"), QStringLiteral("Subsystem"),
            QStringLiteral("Ternary"), QStringLiteral("Evidence / claim")});
        matrixTable_->horizontalHeader()->setSectionResizeMode(0, QHeaderView::ResizeToContents);
        matrixTable_->horizontalHeader()->setSectionResizeMode(1, QHeaderView::ResizeToContents);
        matrixTable_->horizontalHeader()->setSectionResizeMode(2, QHeaderView::ResizeToContents);
        matrixTable_->horizontalHeader()->setSectionResizeMode(3, QHeaderView::Stretch);
        matrixTable_->setEditTriggers(QAbstractItemView::NoEditTriggers);
        matrixTable_->setSelectionBehavior(QAbstractItemView::SelectRows);
        layout->addWidget(matrixTable_, 1);

        auto *actions = new QHBoxLayout();
        auto *scan = new QPushButton(QStringLiteral("Observe & generate matrix"));
        refineButton_ = new QPushButton(QStringLiteral("Create child iteration"));
        exportButton_ = new QPushButton(QStringLiteral("Export matrix JSON"));
        scan->setStyleSheet(QStringLiteral("background:#ffd600;color:#111;font-weight:700;"));
        refineButton_->setEnabled(false);
        exportButton_->setEnabled(false);
        connect(scan, &QPushButton::clicked, this, [this] { observe(); });
        connect(refineButton_, &QPushButton::clicked, this, [this] { refineMatrix(); });
        connect(exportButton_, &QPushButton::clicked, this, [this] { exportMatrix(); });
        actions->addWidget(scan);
        actions->addWidget(refineButton_);
        actions->addWidget(exportButton_);
        layout->addLayout(actions);
        status_ = new QLabel(QStringLiteral(
            "Idle. Observation is read-only; no code, OS settings, drivers or packages will be changed."));
        status_->setWordWrap(true);
        status_->setStyleSheet(QStringLiteral("color:#ffd600;padding:8px;"));
        layout->addWidget(status_);
        setCentralWidget(central);
    }

    QJsonObject makeCell(const QString &stage, const QString &subsystem, int result,
                         const QString &claim, const QStringList &evidenceRefs,
                         bool blocking = false) const
    {
        QJsonArray refs;
        for (const QString &ref : evidenceRefs) refs.append(ref);
        return QJsonObject{{QStringLiteral("stage"), stage},
            {QStringLiteral("subsystem"), subsystem},
            {QStringLiteral("result"), result},
            {QStringLiteral("claim"), claim},
            {QStringLiteral("evidence_refs"), refs},
            {QStringLiteral("confidence"), result == 1 ? 1.0 : 0.0},
            {QStringLiteral("blocking"), blocking}};
    }

    void observe()
    {
        const QString path = QDir::cleanPath(repository_->text().trimmed());
        if (!QFileInfo(path).isDir()) {
            QMessageBox::warning(this, QStringLiteral("Repository required"),
                QStringLiteral("Choose an existing local repository directory."));
            return;
        }

        QStringList evidenceLines;
        evidenceLines << QStringLiteral("path=%1").arg(QDir(path).absolutePath());
        evidenceLines << QStringLiteral("host=%1").arg(QSysInfo::prettyProductName());
        evidenceLines << QStringLiteral("architecture=%1").arg(QSysInfo::currentCpuArchitecture());

        int fileCount = 0;
        QDirIterator iterator(path, QDir::Files | QDir::NoDotAndDotDot,
                              QDirIterator::Subdirectories);
        while (iterator.hasNext()) {
            iterator.next();
            ++fileCount;
            if (fileCount >= 100000) break;
        }
        evidenceLines << QStringLiteral("file_count_lower_bound=%1").arg(fileCount);

        QProcess git;
        git.setWorkingDirectory(path);
        git.start(QStringLiteral("git"), {QStringLiteral("status"), QStringLiteral("--porcelain=v1"),
            QStringLiteral("--untracked-files=no")});
        QString gitStatus = QStringLiteral("git_status=unknown (git unavailable or not a repository)");
        if (git.waitForStarted(1000) && git.waitForFinished(2500)
            && git.exitStatus() == QProcess::NormalExit && git.exitCode() == 0) {
            gitStatus = QStringLiteral("git_status=available; tracked_changes=%1")
                .arg(QString::fromUtf8(git.readAllStandardOutput()).trimmed().count(QLatin1Char('\n')));
        }
        evidenceLines << gitStatus;

        const QByteArray evidenceBytes = evidenceLines.join(QLatin1Char('\n')).toUtf8();
        const QString evidenceId = QStringLiteral("obs-%1").arg(hashText(evidenceBytes).left(16));
        const QString matrixId = QStringLiteral("NUN-%1")
            .arg(QUuid::createUuid().toString(QUuid::WithoutBraces));
        const QJsonArray evidence{QJsonObject{
            {QStringLiteral("id"), evidenceId},
            {QStringLiteral("kind"), QStringLiteral("manual_review")},
            {QStringLiteral("source"), QStringLiteral("local read-only inventory: filesystem + git status")},
            {QStringLiteral("sha256"), hashText(evidenceBytes)}}};

        QJsonArray cells;
        cells.append(makeCell(QStringLiteral("observe"), QStringLiteral("repository_inventory"), 1,
            QStringLiteral("Repository directory is readable; %1 files observed (capped at 100000).").arg(fileCount),
            {evidenceId}));
        cells.append(makeCell(QStringLiteral("observe"), QStringLiteral("working_tree"), 0,
            gitStatus, {evidenceId}));
        cells.append(makeCell(QStringLiteral("observe"), QStringLiteral("host_environment"), 1,
            QStringLiteral("Host OS and CPU architecture were read from Qt system metadata."),
            {evidenceId}));
        cells.append(makeCell(QStringLiteral("propose_validate"), QStringLiteral("performance_candidates"), 0,
            QStringLiteral("No performance claim is made until project-specific benchmarks are declared and run."),
            {evidenceId}));
        cells.append(makeCell(QStringLiteral("propose_validate"), QStringLiteral("build_and_test"), 0,
            QStringLiteral("Build and test status is unknown; this observer does not execute repository scripts."),
            {evidenceId}, true));
        cells.append(makeCell(QStringLiteral("propose_validate"), QStringLiteral("security_and_dependencies"), 0,
            QStringLiteral("Security and dependency posture requires a repository-specific, reviewed adapter."),
            {evidenceId}));
        cells.append(makeCell(QStringLiteral("approve_apply"), QStringLiteral("change_approval"), 0,
            QStringLiteral("No changes are applied; explicit human approval and a reviewable diff are required."),
            {evidenceId}, true));
        cells.append(makeCell(QStringLiteral("approve_apply"), QStringLiteral("post_change_verification"), 0,
            QStringLiteral("Not applicable until an approved change exists and its checks have completed."),
            {evidenceId}));
        cells.append(makeCell(QStringLiteral("approve_apply"), QStringLiteral("rollback_readiness"), 0,
            QStringLiteral("Rollback plan must be supplied by the repository adapter before promotion."),
            {evidenceId}, true));

        ++version_;
        matrix_ = QJsonObject{
            {QStringLiteral("schema"), QStringLiteral("nun.auto-improvement.matrix.v1")},
            {QStringLiteral("matrix_id"), matrixId},
            {QStringLiteral("repository"), QStringLiteral("local/%1").arg(QFileInfo(path).fileName())},
            {QStringLiteral("version"), version_},
            {QStringLiteral("parent_matrix_id"), QJsonValue::Null},
            {QStringLiteral("created_at"), QDateTime::currentDateTimeUtc().toString(Qt::ISODate)},
            {QStringLiteral("stages"), QJsonArray{QStringLiteral("observe"),
                QStringLiteral("propose_validate"), QStringLiteral("approve_apply")}},
            {QStringLiteral("cells"), cells},
            {QStringLiteral("evidence"), evidence},
            {QStringLiteral("rollback"), QJsonObject{
                {QStringLiteral("required"), true},
                {QStringLiteral("plan"), QStringLiteral("No mutation performed. For any later approved change, revert the dedicated branch/commit and rerun the baseline checks.")}}}
        };
        renderMatrix();
        status_->setText(QStringLiteral("Matrix %1 generated. Observation is complete; proposal and apply stages remain UNKNOWN (0) pending project-specific evidence and approval.")
            .arg(matrixId));
        refineButton_->setEnabled(true);
        exportButton_->setEnabled(true);
    }

    void refineMatrix()
    {
        if (matrix_.isEmpty()) return;
        const QString previousId = matrix_.value(QStringLiteral("matrix_id")).toString();
        const int nextVersion = matrix_.value(QStringLiteral("version")).toInt() + 1;
        QJsonArray cells = matrix_.value(QStringLiteral("cells")).toArray();
        cells.append(makeCell(QStringLiteral("propose_validate"), QStringLiteral("meta_optimizer"),
            0, QStringLiteral("Child iteration created. It cannot promote itself; a human must compare evidence and approve policy changes."),
            {matrix_.value(QStringLiteral("evidence")).toArray().first().toObject()
                .value(QStringLiteral("id")).toString()}, true));
        matrix_.insert(QStringLiteral("parent_matrix_id"), previousId);
        matrix_.insert(QStringLiteral("matrix_id"), QStringLiteral("NUN-%1")
            .arg(QUuid::createUuid().toString(QUuid::WithoutBraces)));
        matrix_.insert(QStringLiteral("version"), nextVersion);
        matrix_.insert(QStringLiteral("created_at"), QDateTime::currentDateTimeUtc().toString(Qt::ISODate));
        matrix_.insert(QStringLiteral("cells"), cells);
        renderMatrix();
        status_->setText(QStringLiteral("Child matrix v%1 created from %2. Previous matrix evidence remains unchanged; new meta-optimization cell is UNKNOWN (0).")
            .arg(nextVersion).arg(previousId));
    }

    void renderMatrix()
    {
        const QJsonArray cells = matrix_.value(QStringLiteral("cells")).toArray();
        matrixTable_->setRowCount(cells.size());
        for (int row = 0; row < cells.size(); ++row) {
            const QJsonObject cell = cells.at(row).toObject();
            const QStringList values{
                cell.value(QStringLiteral("stage")).toString(),
                cell.value(QStringLiteral("subsystem")).toString(),
                QString::number(cell.value(QStringLiteral("result")).toInt()),
                cell.value(QStringLiteral("claim")).toString()};
            for (int column = 0; column < values.size(); ++column)
                matrixTable_->setItem(row, column, new QTableWidgetItem(values.at(column)));
        }
    }

    void exportMatrix()
    {
        if (matrix_.isEmpty()) return;
        const QString target = QFileDialog::getSaveFileName(this, QStringLiteral("Export ternary matrix"),
            QStringLiteral("auto-improvement-matrix.json"), QStringLiteral("JSON files (*.json)"));
        if (target.isEmpty()) return;
        QFile file(target);
        if (!file.open(QIODevice::WriteOnly | QIODevice::Truncate)) {
            QMessageBox::critical(this, QStringLiteral("Export failed"), file.errorString());
            return;
        }
        file.write(QJsonDocument(matrix_).toJson(QJsonDocument::Indented));
        file.close();
        status_->setText(QStringLiteral("Matrix exported to %1").arg(target));
    }

signals:
    void matrixReady();
};
} // namespace

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    AutoImproveStudio window;
    window.show();
    return app.exec();
}
