# NUNLauncher ↔ NUN System Bridge

**Status:** contract proposal only; no Binder service is implemented.

## 1. Deployment modes

| Mode | Launcher behavior | NUN service |
|---|---|---|
| Ordinary Android | Full Home app and Android app catalog | Absent; NUN-specific controls show unavailable |
| NUN Android distribution | Home app plus user-visible system dashboard | System service installed by the OS image |
| Bare-metal NUN OS | Not an Android app process | Native NUN shell/session owns system control |

The same NUNLauncher APK must not infer privileged access from the device model or from an installed package name.

## 2. Binder contract

The proposed Android bridge uses a versioned Binder interface, installed and permissioned by a NUN Android system image. The client performs a handshake before exposing any system controls.

Handshake fields:
- bridge interface major/minor version;
- NUN OS build identifier;
- service feature identifiers;
- caller permission result;
- supported request size and timeout limits.

The client handles absent service, permission denial, version mismatch, service death and timeout as separate visible states.

## 3. Initial read-only methods

The first bridge revision should be read-only:
- obtain NUN build and architecture information;
- obtain service readiness and degraded-state summaries;
- inspect supported features and installed update metadata;
- open a NUN system-settings page using an explicit in-app route;
- receive bounded service events with sequence numbers.

Firmware updates, reboot-to-bootloader, partition writes, arbitrary shell execution, raw device access and policy changes are excluded from the initial bridge.

## 4. Security requirements

- Require an explicit signature-level permission or OS-managed caller allow-list.
- Check Binder calling UID at every privileged method boundary.
- Do not trust package names alone.
- Bound message sizes, callback counts, event history and execution time.
- Do not expose a generic command endpoint.
- Do not return raw secrets, private app data or full filesystem paths.
- Reject unsupported interface versions.
- Record sensitive administrative actions in the system's local audit service.
- Make privileged operations unavailable to the third-party APK build.

The Android-side service is an adapter to NUN; it is not the kernel ABI. The native kernel syscall/service ABI remains a separate versioned contract.
