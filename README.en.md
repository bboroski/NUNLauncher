<p align="center"><img src="assets/duck-ui/duck-mark.svg" alt="NUNLauncher mark" width="92"></p>
<h1 align="center">NUNLauncher</h1>
<p align="center"><strong>Android HOME Launcher · NUN Boot-Chain Design</strong></p>
<p align="center"><img src="https://img.shields.io/badge/STATUS-ANDROID%20STARTER-FFD600?style=for-the-badge&labelColor=111111"><img src="https://img.shields.io/badge/BUILD-VALIDATION%20PENDING-FFB000?style=for-the-badge&labelColor=111111"><img src="https://img.shields.io/badge/LANG-UK%20%7C%20EN%20%7C%20RU-FFD600?style=for-the-badge&labelColor=111111"></p>
<p align="center"><a href="README.md">🇺🇦 Українська</a> · <a href="README.en.md">🇬🇧 English</a> · <a href="README.ru.md">🇷🇺 Русский</a></p>
<p align="center"><sub><a href="design/DUCK_UI_SYSTEM.md">Duck Black / Yellow UI standard</a> · <a href="assets/duck-ui/duck-ui-preview.svg">Vector UI preview</a></sub></p>

**NUNLauncher is the Android HOME application for the NUN ecosystem.** It provides a home screen, discovers visible applications, launches them and stores favorites. The future firmware boot supervisor is a separate build target, not an Android activity.

## Implemented starter layer

- Kotlin Android application module with application ID org.nun.launcher.
- MAIN + HOME + DEFAULT intent filter for the Android home role.
- Separate MAIN + LAUNCHER filter so NUNLauncher can be opened as a regular app.
- Discovery of launchable activities through LauncherApps and Android-visible profiles.
- Local search, application grid, app launch and local favorites (long press).
- Duck black/yellow palette, vector adaptive icon and Ukrainian, English and Russian strings.
- CI workflow for assembling a debug APK.

## Responsibility boundary

Android HOME is an ordinary application under Android. It does not replace the Android kernel, firmware, recovery, system image or bootloader. The user selects NUNLauncher as HOME through Android Settings. NUN core connectivity is not simulated; the UI explicitly reports that the kernel is disconnected.

## Build

Requires JDK 17, Android SDK Platform 36 and Gradle 9.6.0. Run <code>gradle :app:assembleDebug</code>.

Expected APK path: <code>app/build/outputs/apk/debug/app-debug.apk</code>. The existence of this file is not claimed until a build has run and the artifact has been verified.

## First run

1. Build and install the debug APK on Android 8.0 or later.
2. Open Settings → Apps → Default apps → Home app (wording varies by vendor).
3. Select NUNLauncher.
4. Press Home and verify the system resolver result.

## Planned capabilities

A dock, folders, widgets, shortcuts, gestures, notification dots, wallpaper controls, accessibility settings, a local NUN device dashboard and session synchronization are separate future work. Each feature must use public Android APIs, request minimum permissions, use versioned storage and expose an explicit offline state.

## Separate firmware boot chain

The future NUN loader specification is kept separately in [NUN_BOOT_CHAIN.md](docs/NUN_BOOT_CHAIN.md). Android HOME code does not imply a UEFI loader, bootable image or verified architecture. The NUN kernel will independently validate the handoff context.

## Documentation

- [Android HOME Launcher](docs/ANDROID_HOME_LAUNCHER.md)
- [NUN boot chain separation](docs/NUN_BOOT_CHAIN.md)
- [OS foundation in NUN](https://github.com/bboroski/NUN/blob/main/docs/OS_FOUNDATION.md)

## License

NUNLauncher has no LICENSE file and no declared license. Usage terms must be defined before public redistribution.

---

**NUNLauncher — built with love for simple, fast and understandable interfaces.**
