# NUNLauncher — Android Home Application Architecture

**Status:** first Android application baseline; emulator/device verification is still required.  
**Platform:** Android API 26+; Java 17; Android Gradle Plugin 9.4.1.  
**Product role:** third-party Android Home application and NUN user-facing shell.

## 1. Product boundary

NUNLauncher is an Android application implementing the HOME/DEFAULT activity contract. When a user selects it as the default Home app, Android starts it as the device's Home surface. It can discover and launch activities exposed with the MAIN/LAUNCHER intent filter, subject to Android package-visibility rules.

NUNLauncher is not:
- an EFI application or firmware bootloader;
- a fastboot/recovery replacement;
- a root manager or partition flasher;
- an Android system process with signature permissions;
- a way to replace the boot chain on a locked stock device.

Installing NUN as a bare-metal OS requires a separate, device-specific boot port, unlocked/authorized boot chain and verified firmware images. NUNLauncher remains a normal app unless a future NUN Android distribution explicitly installs it as a privileged/system component.

## 2. Current application baseline

The first app slice provides:
- HOME/DEFAULT intent registration and a normal LAUNCHER entry;
- scoped package visibility for activities declaring MAIN/LAUNCHER;
- a local app grid with icons and readable labels;
- client-side search by application label and package name;
- explicit app launch through the resolved activity component;
- explicit navigation to Android Settings;
- no account, telemetry, cloud dependency, runtime download, hidden device mutation or broad package inventory permission.

No `QUERY_ALL_PACKAGES`, root permission, bootloader command, firmware write or background device-management permission is requested.

## 3. User experience

Use the shared Duck Black/Yellow token set:

- graphite canvas `#0D0D0D`;
- panel surface `#1A1A1A`;
- yellow primary accent `#FFD600`;
- primary text `#E6E6E6`;
- muted text `#999999`;
- danger `#FF4444`.

Yellow is reserved for identity, focus, selected states and high-priority actions. It must not be used as a full-screen background. The app grid supports keyboard/D-pad focus where present, screen readers, large text and reduced-motion preferences.

The first implementation is a native Android Activity using platform widgets and does not require a third-party UI framework. A later adaptive layout may use a tablet/foldable grid, configurable rows/columns, widgets, folders, dock, gestures and workspace pages after interaction and accessibility tests exist.

## 4. Application lifecycle

- `onCreate`: build the Home surface and restore non-sensitive local preferences.
- `onResume`: refresh visible launchable activities and preserve the current search query.
- Search: filter the in-memory list; it must not trigger network requests.
- Launch: resolve a concrete package/activity component and handle missing or disabled activities.
- Settings: open Android Settings through an explicit platform intent.
- Process recreation: reconstruct UI from safe local preferences; never persist stale Activity or Drawable references.

## 5. Package visibility and privacy

Android package visibility is deliberately scoped to launcher activities using a manifest `<queries>` intent declaration. The app must not enumerate every installed package or collect installed-app inventories for analytics.

The launcher stores no app usage history by default. Any future recents, usage statistics or recommendation feature must use the least-privileged Android API, disclose the data, and remain optional.

## 6. NUN service integration

A future NUN Android distribution may expose system services through a permissioned Binder service. The bridge must have:
- a stable interface version and capability negotiation;
- caller identity checks and scoped permissions;
- bounded request/response sizes and timeouts;
- explicit error and unavailable states;
- no arbitrary command execution;
- audit events that exclude private app data;
- graceful absence on ordinary Android devices.

The Android Home UI must remain fully usable when the NUN service is not installed.

## 7. Validation

The build pipeline compiles a debug APK and runs JVM unit tests. Device acceptance must additionally test:
- setting NUNLauncher as the default Home app;
- Home key and device restart behavior;
- app enumeration on API 26 and the current target API;
- package visibility on API 30+;
- launching disabled/uninstalled activities safely;
- search, empty states and large app catalog rendering;
- screen reader labels, contrast, focus, font scaling and landscape/foldable layout;
- Android 12+ exported component requirements;
- no unexpected permissions or network requests.

A CI build is not proof of compatibility with every Android skin, OEM or device.
