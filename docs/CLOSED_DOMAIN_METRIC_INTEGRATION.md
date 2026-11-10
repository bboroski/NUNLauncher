# Closed-Domain Boundary Profile — NUNLauncher

**Role:** specification scaffold for image validation and controlled handoff to NUN.

## Transfer into the boot path

The launcher's bounded domain is the validated image and handoff region. The goal is to prove that every byte range and capability handed to the kernel satisfies the declared boot contract.

| CDM-1 concept | NUNLauncher feature |
|---|---|
| Bounded region | Kernel image, init payload, memory map, stack, and boot-info ranges |
| Boundary | Address limits, alignment, image length, checksums, ABI version, and capability allowlist |
| Fixed snapshot | Immutable validated handoff record |
| Boundary stress | Overlap, overflow, truncation, unsupported architecture, bad checksum, missing capability |
| Invariant ledger | Count of validated ranges and explicit rejection reason |
| Isolation | Fail-closed pre-handoff validation; no undocumented fallback |

## Required invariants

1. Validate all half-open ranges `[start, end)` with overflow-safe arithmetic.
2. Reject overlapping image, boot-info, stack, reserved, and MMIO ranges unless the ABI explicitly permits that exact overlap.
3. Validate image length, entry-point containment, alignment, architecture ID, ABI version, and checksum before handoff.
4. Freeze the validated boot-info record; do not mutate it between validation and transfer.
5. Reject unknown required capabilities and unsupported architecture profiles.
6. Any failure before handoff stops the launch and emits a structured reason; it never jumps to an unknown entry point.

## Acceptance gates

- Add adversarial tests for integer overflow, truncated images, invalid entry points, overlap, alignment, and checksum mismatch.
- Verify validation occurs before any irreversible platform side effect.
- Verify NUN can independently re-check the handed-off memory map and ABI version.
- Keep emulator, cross-toolchain, and physical-target evidence separate.

## Scope

NUNLauncher remains specification-only until its boot code, image pipeline, and tests exist. A passing static contract document does not constitute a bootable loader or verified architecture support.
