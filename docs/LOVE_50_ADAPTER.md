# LOVE-50 adapter — NUNLauncher

**Repository:** `bboroski/NUNLauncher`  
**Role:** Kernel-image admission and pre-handoff validation boundary  
**Status:** SPECIFIED — mapping only; runtime conformance is not implied.  
**Canonical doctrine:** [Klipot LOVE-50](https://github.com/bboroski/Klipot/blob/feat/love-50-chinese-proverbs-20261113/docs/LOVE_50_CHINESE_DOCTRINE.md)

## Why this profile exists

NUNLauncher is an admission gate, not a full bootloader. 'Better no match than a bad match' means invalid or unsupported images are rejected explicitly.

## Repository-specific application

### Fate / compatibility
Match image architecture, header version, entry offset, memory map and required boot capabilities before admission.

### Fidelity / trust
Keep metadata validation separate from image authentication. A correct header or digest does not prove trusted authorship.

### Responsiveness / care
Return stable, actionable diagnostic keys with bounded input handling; do not execute or map an untrusted image during validation.

### Harmony / integration
Use a versioned handoff contract with NUN and report exactly which later boot stages remain unverified.

### Wisdom / restraint
Fail closed on malformed, overlapping, truncated or unsupported metadata. Do not silently downgrade to weaker validation.

## Local verification

- `cmake -S . -B build`
- `cmake --build build --config Release`
- `ctest --test-dir build --output-on-failure`
- `python one.py check`
- `make test`
- `Add malformed-header, truncated-buffer, overflow, entry-range and overlapping-map cases.`

## Non-goals and safety boundaries

- No execution of the image by the validator.
- No secure-boot or signature-verification claim unless separately implemented and evidenced.
- No statement that successful preflight equals firmware handoff or hardware boot.

## Adoption state

This document establishes an interpretation only. A control is IMPLEMENTED only when code/configuration exists, and VERIFIED only when repository-native checks and target-specific evidence pass. Keep unknown, planned, partial and failed states distinct. Existing security, privacy, ABI, platform, licensing and release requirements remain authoritative.

---
*The Chinese expressions are engineering prompts, not claims that software has human emotions.*
