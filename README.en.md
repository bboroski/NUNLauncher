# NUN Launcher — Android home surface for NUN

NUN Launcher is a native Android HOME application with a black-and-gold interface, animated app tiles, application search, a system overview and a JNI bridge to the NUN runtime.

## Source integration

The complete NUN repository is linked at core/NUN. NUN pins all seven ecosystem projects as nested submodules:
- SAMBU — language, interpreter, CLI, IDE and developer tooling.
- ToolNG — native policy and bounded workflow core.
- ReaperCripple — bootstrap, rollback, BSP and telemetry.
- Kloz — platform ABI, memory arena and resource budgets.
- CO — deterministic lifecycle runtime.
- SaintSI — health and integrity observation.
- Klipot — ecosystem registry and integration planner.

The Android NDK compiles NUN's native runtime into libnun_android_bridge.so. SAMBU and Klipot remain host-side tools in the linked source tree.

## Build

Requirements: JDK 17, Gradle 9.6.0+, Android SDK Platform 36, Android NDK 28.2.13676358 and CMake 3.22.1.

Clone recursively and run one.cmd on Windows or python3 one.py on Linux/macOS. The debug APK is generated at app/build/outputs/apk/debug/app-debug.apk.

## Use

Install the APK, then select NUN Launcher in Android's Home app settings. The launcher discovers apps that expose the standard launcher intent and opens them without requesting broad package visibility or privileged device access.

## Status

This is an Android launcher and host-runtime integration, not a replacement Android OS image or bare-metal bootloader. A bootable NUN kernel still needs an architecture-specific boot path, memory manager, interrupt layer, device drivers and emulator/hardware validation.
