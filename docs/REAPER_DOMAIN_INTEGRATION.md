# Reaper-Domain Integration — NUNLauncher

**Role:** specification scaffold for the NUN bootstrap supervisor; no launcher implementation is currently present  
**Canonical catalogue:** [SAMBU — 50 curated engineering patterns](https://github.com/bboroski/SAMBU/blob/main/data/reaper/engineering_patterns_50.tsv)

## Boot-time engineering transfer

- **Relay/network integrity:** verify that the selected image, platform profile, and ABI belong to the same declared boot contract.
- **Kinetic barrier / hull integrity:** validate memory-map bounds, alignment, reserved regions, and image extents before handoff.
- **Core reconstruction:** define a last-known-good boot profile and an explicit failure record; do not silently jump to an unknown image.
- **Signal relay / provenance:** preserve image digest, source, version, and validation result in the boot-information block.
- **Cycle/reset model:** make retry count, fallback order, and recovery policy explicit and bounded.

## Required gates

Validate the image format and size, target architecture, ABI version, entry-point range, memory map, required platform capabilities, and integrity metadata before transferring control. Cryptographic authenticity requires a real signature-verification design and protected trust anchors; a plain checksum is not authentication.

## Status and acceptance

NUNLauncher remains documentation-only. The reference vocabulary does not create a bootloader, certify architectures, or provide real firmware security. The future implementation must have host tests for malformed images and target-specific boot/handoff evidence.
