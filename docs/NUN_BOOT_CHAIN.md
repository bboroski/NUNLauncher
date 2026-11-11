# NUN boot chain and Android HOME separation

NUNLauncher has two explicitly separated product boundaries:

1. **Android HOME shell** — an APK running under the Android framework. It shows launchable applications and provides a user-selected home screen.
2. **Future firmware boot supervisor** — a separate non-Android target that may validate a NUN kernel image and hand off control to it on a qualified computer platform.

The two targets may share visual tokens and versioned declarative metadata. They must not share privileged code or claim equivalent system authority.

## Firmware loader target

The future firmware loader is responsible for image format/architecture checks, image digest and signature policy, memory-map validation, platform capability checks, handoff construction and controlled entry transfer.

For a UEFI path, the loader must obtain the current memory map and call ExitBootServices with the corresponding map key. If the key is stale, it must reacquire the memory map and retry as required by the UEFI specification. After successful exit, it must not call boot services as if they were still available.

The kernel must independently validate the received handoff. A loader-side check cannot replace kernel-side validation.

## Android HOME target

The Android target is an ordinary application. Its HOME activity is discoverable through MAIN/HOME/DEFAULT intent resolution. It can start visible launchable activities through Android's launcher APIs. The user selects it as the default home app through Android Settings or the system resolver.

No APK can claim to be a UEFI loader, unlock a locked bootloader, replace the Android kernel, or bypass device-vendor boot-chain policy.

## Current evidence

- Android project scaffolding and a native HOME starter activity are present in this branch.
- The UEFI boot supervisor remains a design contract only.
- No UEFI executable, kernel image, bootable ISO, device flash image or verified handoff binary is produced by this repository change.
- No architecture, including x86_64, AArch64, RISC-V or E2K, is certified by the Android build.

## References

- [UEFI 2.10 Boot Services](https://uefi.org/specs/UEFI/2.10/07_Services_Boot_Services.html)
- [Android Intent reference](https://developer.android.com/reference/android/content/Intent)
