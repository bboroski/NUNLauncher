# NUNLauncher Domain Capabilities

The Reaper infrastructure vocabulary becomes a boot-time validation and recovery
contract. The Python tool validates metadata only; it does not load or execute a
kernel image.

## Implemented validation functions

- **Citadel / relay-network integrity:** validate artifact format, size, digest syntax,
  architecture label and ABI declaration.
- **Kinetic barrier / hull integrity:** reject overlapping or misaligned memory ranges.
- **Core reconstruction:** require a bounded retry budget and an explicit last-known-good
  fallback policy.
- **Signal provenance:** carry image digest and profile identity in the boot metadata contract.
- **Cycle control:** make retry count and delay explicit rather than allowing unbounded loops.

## Verification

```sh
python tools/validate_boot_profile.py
python -m unittest tests.test_boot_profile
```

## Status boundary

NUNLauncher remains specification and validation tooling, not a bootloader.
The example profile uses an intentionally placeholder digest and an unverified
architecture. SHA-256 syntax is not signature verification. A real boot path still
requires a concrete image format, toolchain, trust anchor, architecture entry code,
linker map, emulator tests and target-hardware evidence.
