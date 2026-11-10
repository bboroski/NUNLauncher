# NUNLauncher architecture

## Application boundary

NUNLauncher is a standard Android application assigned the HOME role by Android's intent resolution. It does not replace the Linux kernel used by Android devices, modify boot partitions, access firmware variables or install NUN as a bare-metal operating system.

## Current component map

~~~text
Android system
  |-- Role / HOME resolver
  |-- LauncherApps API
  |-- Package visibility policy
  |
  v
NUNLauncher
  |-- MainActivity
  |-- LauncherHome (Compose)
  |-- LauncherApps discovery
  |-- Search/filter
  |-- App launch adapter
  |-- Duck design tokens
  |
  +--> Android Settings intent
  +--> Future NUN bridge client (not implemented)
~~~

## Current behavior

1. Android chooses NUNLauncher as HOME after user selection.
2. The application requests the list of launchable activities for the current user profile.
3. It presents the results in a responsive adaptive grid.
4. Search filters labels locally.
5. Selecting an item delegates launch to LauncherApps.startMainActivity.
6. Errors are visible as an in-app toast; they are not silently swallowed.

The catalogue is refreshed on first render or explicit refresh. Automatic package-change observation and work-profile management are future work.

## Privacy boundary

- The catalogue is metadata about launchable activities.
- No private application databases, documents, messages or credentials are read.
- No telemetry or account is required.
- The current app has no internet permission.
- Work-profile and private-profile access is left to Android policy and user authorization.

## System integration roadmap

- NUN bridge connection discovery with explicit user opt-in.
- Mutual authentication and versioned message envelopes.
- Read-only system information before administrative operations.
- Capability-scoped commands, per-action confirmation and audit record.
- Explicit disconnected/degraded states.
- No arbitrary command shell, root escalation or remote code execution.

## Build verification

The project uses a Gradle Android application module, Kotlin with built-in AGP Kotlin support, the Compose compiler plugin and the stable Compose BOM. A build must be validated with the selected Android SDK and a real emulator/device before publishing an APK.
