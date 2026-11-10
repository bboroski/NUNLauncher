# Boot handoff contract

This repository owns the producer-side handoff contract for NUN. The schema is
versioned as `nun.boot-handoff.v1` and is shared with the NUN consumer so both
sides can reject malformed or ambiguous boot metadata before the handoff boundary.

## Contract checks

- Version and required fields are explicit; unknown fields and duplicate JSON keys are rejected.
- The kernel image path is normalized and relative, with a bounded size and SHA-256 content digest.
- The memory map is checked for unsigned 64-bit overflow and overlapping regions.
- Exactly one kernel memory region is required; the entry point must fall inside it.
- Capabilities are explicit and unique.
- A canonical record digest detects content changes when compared with a trusted digest.

## Run

```bash
python tools/validate_boot_handoff.py --self-test
python tools/validate_boot_handoff.py spec/boot-handoff.example.json
python tools/validate_boot_handoff.py --digest-only spec/boot-handoff.example.json
```

The sample architecture and image metadata are fixtures, not a supported target
declaration. A SHA-256 digest is not a signature: it does not authenticate the
producer, prove image origin, authorize execution, or validate physical hardware.
A real loader still needs architecture-specific image loading, signature policy,
memory reservation, exception-safe handoff, and emulator/target qualification.
This utility validates metadata only; it is not a bootloader and produces no
bootable binary.
