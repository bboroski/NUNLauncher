# NUNLauncher — Android home environment

**NUNLauncher is the Android home-screen application for the NUN ecosystem.** The canonical repository name remains NUNLauncher. It is not a PC bootloader and cannot replace the Android kernel.

<sub>Implemented foundation: HOME activity, launcher-app discovery/search/launch and a black/yellow Compose UI. A privileged NUN management bridge remains a future contract.</sub>

## Foundation features

- Android HOME role through MAIN + HOME + DEFAULT intent categories.
- App catalogue using Android LauncherApps.
- Local search across installed launchable activities.
- Refresh and launch actions.
- Android settings shortcut.
- RU/UK/EN UI resources.
- Duck black/yellow design tokens and vector app mark.
- Local-only; no account, telemetry, remote execution or background network access.

## Build

Requirements: Android Studio/JDK 17+, Android SDK Platform 37 and Gradle 9.6.0.

~~~bash
gradle :app:assembleDebug
~~~

APK output: app/build/outputs/apk/debug/app-debug.apk.

Install on a test device and select NUNLauncher as the default Home app in Android settings. The previous Home app can be restored through system settings.

## Android API model

The app catalogue uses LauncherApps and launches items through startMainActivity. It does not inspect private app data, alter packages or silently change system settings.

QUERY_ALL_PACKAGES is declared because global launcher-app discovery is a core launcher function. Google Play distribution remains subject to package-visibility policy review.

## NUN connection

The future NUN system bridge is a separate versioned interface. This Android application does not gain root, firmware or kernel privileges by being named NUNLauncher. No NUN RPC implementation is present in this foundation; the UI must not fake a connected state.

Read [architecture](docs/ARCHITECTURE.md), [bridge contract](docs/NUN_BRIDGE.md) and [Duck UI](docs/DUCK_UI.md).
