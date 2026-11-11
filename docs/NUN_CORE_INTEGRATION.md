# NUN Launcher and the NUN source tree

NUNLauncher is an Android HOME application. It is not a bare-metal bootloader, firmware monitor or kernel loader.

The repository links the NUN integration root at core/NUN. NUN in turn pins the seven source projects as nested Git submodules. A recursive clone therefore brings in the original source trees, tests, build files, assets, manifests and independent repository histories.

## Android native path

The Android NDK CMake build adds core/NUN as a subdirectory and links NUN.Runtime into libnun_android_bridge.so. NunCore.java is the JNI boundary used by the Android application.

The bridge supplies a monotonic Android host clock through the Kloz platform contract and binds a minimal component lifecycle to the CO runtime. NUN then composes its native resource, lifecycle, health, staged-bootstrap and policy layers. The app polls the host runtime while its Activity is alive and shuts it down when the Activity is destroyed.

SAMBU and Klipot remain available as host-side Python tools inside the nested NUN source tree. They are not loaded into Android's native process as Python interpreters.

## Permission boundary

The launcher only queries applications that expose the standard MAIN + LAUNCHER intent and starts their declared launch activities. It does not request QUERY_ALL_PACKAGES, accessibility, device-admin, VPN, root, overlay or usage-statistics privileges. The native bridge uses a monotonic clock callback and does not access MMIO, firmware, private device APIs or network services.

## One-knob build

Clone with recursive submodules and run one.cmd on Windows or python3 one.py on Linux/macOS. Requirements: JDK 17, Gradle 9.6.0+, Android SDK Platform 36, NDK 28.2.13676358 and CMake 3.22.1.

The build produces a debug APK. Installing it does not silently replace the current home app; choose NUN Launcher in Android's Home app settings.

## Verification boundary

The native bridge and Android UI must still be built and exercised on an Android SDK/NDK toolchain and a target device or emulator. A successful Android app build proves an Android host integration, not that NUN is a bootable standalone operating system.
