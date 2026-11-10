# NUNLauncher staged boot and handoff contract

NUNLauncher remains a specification-only repository. This is a machine-checkable design target, not an implemented bootloader.

## Ordered boot gates

1. **Image discovery:** locate the candidate image and record its source.
2. **Format validation:** check magic, header size, schema version and total image bounds.
3. **Integrity check:** compute the declared checksum over the exact image region; distinguish checksum from authenticity.
4. **Architecture gate:** compare the image target with CPU and firmware capabilities.
5. **Memory planning:** validate alignment, address-width, reserved ranges and overlap before copying.
6. **Handoff construction:** populate a versioned boot-information block with memory map, framebuffer/serial data and feature bits.
7. **Final marker:** record validated image ID, stage sequence and handoff schema.
8. **Transfer:** invoke the architecture entry point only after all required markers pass.

A failed gate stops the sequence and emits a stable error code. There is no silent fallback to an unknown address, image format or architecture.

## Automation and visual feedback

Automation may validate headers, calculate checksums, compare capability sets and produce a boot report. Any boot animation is a presentation of completed validation stages only; it must not claim a handoff occurred until control transfer is observed or confirmed by the next stage.

## Boundary with NUN

NUNLauncher owns image validation and handoff construction. NUN validates the received block again before initialization. Kernel memory management, scheduling, physics libraries and device drivers remain outside the launcher.

## Promotion gates

No architecture is considered supported until a reproducible artifact, linker map, emulator boot trace and target-device result are attached to the relevant release evidence.
