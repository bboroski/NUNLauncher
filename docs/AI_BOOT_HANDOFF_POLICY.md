# Boot Handoff and AI-Originated Artifacts

**Status:** design requirement  
**Implementation status:** not implemented

## Authority boundary

Gemini or another model may draft documentation, test fixtures or a proposed boot manifest. It must never provide authoritative entry-point addresses, memory-map ranges, page-table values, signature decisions, boot flags or executable image bytes to a live boot path.

A generated manifest is untrusted input. It is not a bootable image, a signed release or proof of target compatibility.

## Required validation before handoff

A future NUNLauncher implementation must validate, in a deterministic order:

1. image format, version, declared size and architecture;
2. cryptographic digest and signature under a configured trust store;
3. ABI compatibility with the target NUN kernel;
4. entry-point alignment and containment within an approved executable image range;
5. memory-map bounds, alignment, overlap and reserved-region exclusions;
6. required platform capabilities and boot protocol version;
7. handoff-block structure, lengths, offsets and version fields.

On failure, stop before handoff and return a typed error. Never guess an address, silently downgrade validation or jump to a model-supplied fallback entry point.

## Separation of responsibilities

NUNLauncher validates the image and constructs the handoff block. NUN independently validates the received handoff context before initializing its own memory manager or drivers. A launcher success status does not prove that the kernel booted successfully.

## Verification

The future implementation requires malformed-image tests, truncated metadata, integer overflow, misalignment, overlapping memory ranges, wrong architecture, stale digest, bad signature, unknown ABI version and invalid entry-point fixtures. Host tests must be supplemented by emulator and target-hardware qualification.

The repository currently contains a specification scaffold only. This policy does not claim the presence of a loader, binary image or supported CPU port.
