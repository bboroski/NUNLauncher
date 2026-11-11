# NUNLauncher — Android HOME application

**Status:** starter Android application source added; device/build qualification pending  
**Application ID:** org.nun.launcher  
**Design system:** Duck Black / Yellow  
**Languages:** English, Ukrainian and Russian

## 1. Product boundary

NUNLauncher is an Android HOME activity. Android starts it through the normal HOME intent resolution after the user selects it as the preferred home application.

It runs as an ordinary Android application. It does not replace the Android kernel, device firmware, recovery partition, bootloader, system image, or vendor services. It cannot start a different operating system kernel from an APK.

The repository keeps firmware boot-supervisor work as a separate future build target and contract. The Android APK must not inherit firmware privileges or pretend to have them.

## 2. Starter application capabilities

The initial source slice provides:

- HOME and DEFAULT intent filters for Android home resolution.
- A separate LAUNCHER entry so NUNLauncher can also be opened from an existing home screen.
- LauncherApps-based discovery of launchable activities in profiles visible to the current user.
- Local search over application labels.
- Launch through LauncherApps.startMainActivity.
- Long-press favorite pinning stored in app-private SharedPreferences.
- Reordering that places favorites before the alphabetical app list.
- English, Ukrainian and Russian string resources.
- Native adaptive icon resources and the Duck graphite/yellow palette.
- Explicit disconnected status: no NUN kernel service is assumed or simulated.

No QUERY_ALL_PACKAGES permission is requested. The manifest declares only the MAIN/LAUNCHER intent query used for app discovery.

## 3. Build

Requirements:

- JDK 17 or newer compatible with the selected Android Gradle Plugin.
- Android SDK Platform 36.
- Gradle 9.6.0.
- Android Gradle Plugin 9.5.1 and Kotlin Android plugin 2.4.10.

Build from the repository root with an installed Gradle 9.6.0 distribution:

~~~sh
gradle :app:assembleDebug
~~~

The expected debug APK path is app/build/outputs/apk/debug/app-debug.apk. This path is a build expectation, not evidence that an APK has already been generated or installed.

## 4. Select as the default home application

1. Install the debug or signed release APK.
2. Open Android Settings.
3. Open Apps, then Default apps, then Home app (wording varies by device vendor).
4. Select NUNLauncher.
5. Press Home to verify that Android resolves the new HOME activity.

The application does not change the default HOME role silently. Android and the user remain in control of that selection.

## 5. Package visibility and profiles

Android filters package visibility for applications targeting Android 11 and later. NUNLauncher declares the narrow launcher-activity intent query and uses the LauncherApps API, which is intended for launcher use cases and profile-aware activity discovery.

The visible profile set is controlled by Android and device policy. A launcher must not claim that every installed package is visible or use broad package enumeration without a legitimate, policy-compliant reason.

## 6. Data and privacy

- Favorites are stored locally in app-private preferences.
- The starter app does not upload application lists or search terms.
- No contacts, location, microphone, camera, accessibility-service or notification-listener permissions are requested.
- Search filters labels already visible to the launcher; it is not a cloud search service.
- The status line clearly says that the NUN kernel is not connected.

## 7. Planned desktop-shell capabilities

Subsequent releases may add a home grid, configurable dock, folders, app shortcuts, widgets, notification dots, gestures, wallpaper integration, accessibility settings and a NUN device-service dashboard.

Each feature must use public Android APIs, request only the permissions it needs, persist state with schema versions and remain usable when optional NUN services are offline. Widgets, notification access and cross-device services require additional explicit user setup and must not be implied by this starter.

## 8. Acceptance checklist

- [ ] Gradle project sync succeeds with the pinned toolchain.
- [ ] Debug APK builds.
- [ ] APK installs on an Android 8.0+ emulator/device.
- [ ] Android lists NUNLauncher among available HOME applications.
- [ ] User can select and later replace the default HOME application.
- [ ] App discovery respects package visibility and user profiles.
- [ ] Search updates the grid without blocking the UI thread.
- [ ] Tap launches the intended app; stale activities produce a visible error.
- [ ] Favorites survive process restart and ignore removed applications.
- [ ] English, Ukrainian and Russian resources render without missing strings.
- [ ] Dark theme, contrast, touch targets and screen-size adaptation are reviewed.

Until these checks are run on an emulator and physical devices, this repository contains an Android launcher starter, not a qualified production launcher.

## 9. Official platform references

- [Android Intent — CATEGORY_HOME and CATEGORY_LAUNCHER](https://developer.android.com/reference/android/content/Intent)
- [LauncherApps API](https://developer.android.com/reference/android/content/pm/LauncherApps)
- [Package visibility filtering](https://developer.android.com/training/package-visibility)
- [Declare package visibility needs](https://developer.android.com/training/package-visibility/declaring)

---

**NUNLauncher — створено з любов'ю до простих, швидких і зрозумілих інтерфейсів.**
