<p align="center"><img src="assets/duck-ui/duck-mark.svg" alt="Duck mark" width="104" /></p>
<h1 align="center">NUNLauncher</h1>
<p align="center"><strong>Duck Black/Yellow · Android Home Application</strong></p>
<p align="center"><img src="https://img.shields.io/badge/STATUS-ANDROID%20APP%20FOUNDATION-FFD600?style=for-the-badge&labelColor=111111" /><img src="https://img.shields.io/badge/PLATFORM-ANDROID%2026%2B-FFD600?style=for-the-badge&labelColor=111111" /></p>
<p align="center"><a href="README.md">🇺🇦 Українська</a> · <a href="README.en.md">🇬🇧 English</a> · <a href="README.ru.md">🇷🇺 Русский</a></p>
<hr />

**NUNLauncher is the Android Home application and user-facing shell for the NUN ecosystem.**

## Purpose

NUNLauncher is a normal Android application that users can select as their default Home app. It displays visible Android launcher activities, supports local search and app launching, and provides a base for future system panels and NUN OS integration.

It is **not** a UEFI bootloader, fastboot/recovery replacement, root manager or firmware flasher. An ordinary Android app cannot replace the device boot chain or write protected partitions.

## Current application foundation

- Android Gradle project: Java 17, SDK 26+, Gradle 9.9.0 and Android Gradle Plugin 9.4.1.
- HOME/DEFAULT and LAUNCHER intent filters.
- Local discovery of activities visible through the LAUNCHER category.
- Search by application label or package name.
- Launch through an explicit ComponentName.
- Explicit navigation to Android Settings.
- Duck Black/Yellow palette with graphite panels and semantic yellow accents.
- Unit tests for local filtering and a GitHub Actions APK build workflow.

Package visibility is scoped to launcher activities. The app does not request `QUERY_ALL_PACKAGES`, root, firmware-write or network permissions.

## Build

Requirements: JDK 17, Android SDK Platform 36 and Gradle 9.9.0.

```bash
gradle :app:testDebugUnitTest :app:assembleDebug
```

The debug APK is written to:

```text
app/build/outputs/apk/debug/app-debug.apk
```

A successful build and runtime behavior on physical devices still need CI and device smoke-test evidence. The workflow runs JVM unit tests and assembles the debug APK.

## Architecture boundaries

- **NUN OS** is the independent bare-metal OS, kernel and system ABI.
- **NUNLauncher** is the Android Home application.
- **NUN System Bridge** is a future restricted Binder client for a NUN Android distribution; the current repository contains its contract, not a working system service.

NUNLauncher must remain usable as an Android Home app without a NUN service. When the bridge is absent, NUN-specific controls must display an unavailable state instead of pretending to have system privileges.

## Roadmap

Adaptive workspaces, dock, folders, widgets, quick settings, accessibility improvements, keyboard/D-pad navigation and a permissioned NUN service integration will be added in separate tested stages.

## Documentation

- [Android Home architecture](docs/ANDROID_HOME_ARCHITECTURE.md)
- [NUN system bridge contract](docs/NUN_SYSTEM_BRIDGE.md)
- [Duck UI system](docs/DUCK_UI_SYSTEM.md)
- [NUN OS repository](https://github.com/bboroski/NUN)
