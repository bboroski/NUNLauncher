# NUN Workbench

## Purpose

NUN Workbench is a host-side desktop engineering console for the NUN project.
It is an optional Qt 6 Widgets application in NUNLauncher. It does not run in
the NUN kernel, replace firmware or the bootloader, or claim to be the future
NUN operating-system desktop.

## First implementation

| Area | Action | Existing implementation |
|---|---|---|
| Overview | Current scope and workspace selection | Repository documentation |
| Image Gate | Verify ISO and JSON manifest | Python module nunlauncher_boot.cli verify |
| Build & Test | Build Limine ISO | NUN make iso |
| Build & Test | Build Multiboot2 image | NUN scripts/build-qemu.sh |
| Build & Test | Run NUN host tests | NUN make test |
| Build & Test | Run NUNLauncher tests | Python unittest |
| QEMU Lab | Launch Multiboot2 image | NUN scripts/run-qemu.sh |

The workbench runs one child process at a time, streams its output to a bounded
log view, offers a stop control and remembers local paths. It passes executable
arguments separately rather than composing shell command strings. It does not
offer USB flashing or any destructive disk operation.

## Build

The desktop target is optional. NUNLauncher C11 libraries and tests remain
buildable when a C++ compiler or Qt 6 is not installed.

On Debian/Ubuntu:

~~~sh
sudo apt-get install -y cmake ninja-build g++ qt6-base-dev
cmake -S . -B build -DNUNLAUNCHER_BUILD_DESKTOP=ON
cmake --build build --parallel
ctest --test-dir build --output-on-failure
~~~

For Windows, install Qt 6 Widgets and a C++17 compiler, then configure with the
Qt CMake package path. NUN shell scripts additionally require bash (for example,
Git Bash or WSL) and the corresponding toolchain on PATH. The app does not
silently install dependencies or rewrite PATH.

For a core-only build:

~~~sh
cmake -S . -B build-core -DNUNLAUNCHER_BUILD_DESKTOP=OFF
cmake --build build-core --parallel
ctest --test-dir build-core --output-on-failure
~~~

## Trust and safety

- Image verification is read-only and delegates to the existing NUNLauncher
  verifier.
- SHA-256 establishes digest equality with the manifest, not publisher identity.
- A successful preflight check does not mean the image was loaded, executed,
  booted on physical hardware or security-qualified.
- QEMU launch invokes only the existing NUN emulator script.
- The application does not mount images, flash drives, elevate privileges,
  change firmware settings or bypass Secure Boot.
- Build operations write only to locations defined by existing NUN build scripts.

## UI and limitations

The interface follows the Duck Black / Yellow system in
design/DUCK_UI_SYSTEM.md: graphite surfaces, yellow primary actions, visible
focus, readable status labels and native Qt widgets.

This first increment is a host development tool, not a NUN-native application.
Qt 6 is optional at configure time. The initial QEMU action is the existing
x86_64 Multiboot2 path and is not represented as NUNLauncher UEFI handoff.
The workbench does not claim that NUN scheduler, VFS, network stack, compositor,
graphics drivers or userland are implemented.
