# NUN Launcher — Android home surface for NUN

NUN Launcher is a real Android HOME application with a black-and-gold interface, animated app tiles, application search, system overview and a native bridge to the NUN runtime.

## What it uses

The full NUN source tree is linked at core/NUN. That repository pins all seven ecosystem projects:
- SAMBU — language, interpreter, CLI, IDE and developer tooling.
- ToolNG — native policy and bounded workflow core.
- ReaperCripple — bootstrap, rollback, BSP and telemetry.
- Kloz — platform ABI, memory arena and resource budgets.
- CO — deterministic lifecycle runtime.
- SaintSI — health and integrity observation.
- Klipot — registry and integration planner.

The Android NDK build compiles NUN's native runtime into libnun_android_bridge.so. SAMBU and Klipot remain host-side tools in the linked source tree.

## Build

Requirements: JDK 17, Gradle 9.6.0+, Android SDK Platform 36, Android NDK 28.2.13676358 and CMake 3.22.1.

Clone recursively and run:
- Windows: one.cmd
- Linux/macOS: python3 one.py

The debug APK is written to app/build/outputs/apk/debug/app-debug.apk.

## Use

Install the APK, then choose NUN Launcher in Android's Home app settings. The launcher lists apps that expose the standard launcher intent and opens them without requesting broad package visibility or privileged device access.

## Status

This is an Android launcher and host runtime integration, not a replacement Android OS image or bare-metal bootloader. NUN's kernel substrate still needs an architecture-specific boot path, memory manager, interrupt layer, drivers and hardware/emulator validation.
