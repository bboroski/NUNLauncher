# NUNLauncher — preflight contract for simulation-capable images

## Scope

NUNLauncher remains a bootstrap supervisor, not a runtime for the ReaperCripple simulator. Its responsibility is to validate a boot artifact and construct a versioned handoff block. The 6666-iteration simulation may be included only as an optional user-mode or diagnostic payload after NUN is ready.

## Required preflight checks

- verify image magic, format version, declared architecture, and ABI version;
- reject an architecture mismatch instead of guessing a fallback;
- validate entry-point alignment and that the entry lies inside an executable image range;
- validate memory-map ranges, overlap, integer overflow, and required reservations;
- verify artifact digest and signature using a separately specified cryptographic implementation;
- pass only validated metadata and explicitly granted capabilities in the handoff block.

## CPU matrix

A 64-bit payload does not mean every target is x86-64. x86-64, AArch64, RISC-V, and Elbrus/E2K must have distinct validated image targets. A simulation library's uint64_t state values do not qualify an architecture backend.

## Status

NUNLauncher currently contains specifications, not a bootloader. These checks describe acceptance criteria and must not be read as implemented validation or a bootable binary.

## NUN image-contract dependency

NUNLauncher consumes the proposed NUN capability ID `kernel.image.contract.specification.v1`, defined in [NUN Kernel Image Contract v1](https://github.com/bboroski/NUN/blob/main/docs/architecture/kernel-image-contract-v1.md). This is a documentation-level dependency only: neither repository currently implements an image parser, loader, signature verifier, or boot handoff runtime.
