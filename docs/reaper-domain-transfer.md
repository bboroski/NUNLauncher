# Reaper Domain Transfer Profile — NUNLauncher

**Status:** bootstrap trust and handoff specification  
**Role:** image admission, platform compatibility, memory-map validation and controlled transfer

## Objective

Translate the source list's relay, core, signal, memory and cycle concepts into a reviewable boot sequence. NUNLauncher must validate a declared kernel image and handoff context; it must not assume that an artifact is trustworthy merely because it has a valid filename or a matching display name.

## Proposed bootstrap stages

`DISCOVERED → METADATA_PARSED → DIGEST_CHECKED → SIGNATURE_POLICY_CHECKED → ABI_VALIDATED → MEMORY_MAP_VALIDATED → HANDOFF_READY → HANDED_OFF`

Any failed or unknown required check stops the transition. There is no silent fallback to an unknown entry point, memory map or ABI. Recovery must be an explicit policy.

## Boot manifest

The proposed `spec/boot-manifest.schema.json` describes:

- manifest format version and stable image identity;
- image format, size, digest, entry point and target ABI;
- memory-map regions with base, length and declared purpose;
- required platform capabilities;
- build provenance and optional signature metadata;
- verification policy and a traceable result for each gate.

The schema describes declarations; a future implementation must still perform actual byte-level checks. A SHA-256 digest detects content mismatch but does not authenticate a publisher. Signature verification requires a target-specific root of trust, key policy, revocation behavior and reviewed implementation.

## Handoff invariants

- Reject integer overflow, overlapping or out-of-range regions, invalid alignment and entry points outside an executable region.
- Preserve the firmware-provided memory map; do not invent RAM, MMIO, DMA or cache properties.
- Validate manifest version and ABI before constructing handoff data.
- The kernel independently validates the handoff block.
- Record the result of each verification gate without claiming secure boot unless the entire trust chain is implemented and tested.
- Never copy an unvalidated native structure as a wire format; define widths, byte order and version explicitly.

## Validation plan

Host tests should cover malformed manifests, duplicate regions, overflow, overlap, invalid entry points, missing requirements, digest mismatch and incompatible ABI. Emulator/target tests must separately verify firmware interaction, memory ownership and the real architecture entry point.

NUNLauncher remains a specification scaffold until loader code and target evidence exist. This profile does not claim a bootable binary or completed secure-boot support.
