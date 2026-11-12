# Inter-substrate runtime profile — NUNLauncher

**Contract:** [SAMBU runtime API v1](https://github.com/bboroski/SAMBU/blob/feat/substrate-runtime-contract-v1/docs/INTERSUBSTRATE_RUNTIME_API_V1.md)  
**Status:** architecture/integration profile. Target-specific functionality remains gated by evidence.

## Ownership
Host-side kernel image admission and pre-handoff validation.

## Existing evidence
48-byte image header and manifest checks, architecture/entry/load bounds, SHA-256 verification, BIOS/UEFI image inspection and QEMU-linked NUN image verification. It does not execute the image or pass control to the kernel.

## Integration slice
Use a versioned image resource descriptor: architecture/ABI, image type, digest algorithm/value, length, alignment, entry offset, load ranges, manifest version and evidence provenance. Validate signature metadata only after a defined publisher trust store/key lifecycle exists. Emit an admission decision with stable status, message ID and correlation ID; keep validation read-only.

## Explicit non-goals
SHA-256 matching does not authenticate a publisher. No Secure Boot, key rotation, TPM attestation, driver loading, process containerization or firmware-to-kernel handoff should be claimed until code and target evidence exist.

## Acceptance tests
Malformed/oversized manifests; path traversal; digest mismatch; unsupported architecture; overflow and overlap in load ranges; alignment; unknown schema; unsigned images; same test image through BIOS and UEFI QEMU workflows.

## Maturity rule
Host validation, a manifest, a simulator, or a successful unrelated target build cannot be promoted into kernel, firmware, ABI, or physical-hardware qualification. Every supported target requires its own reproducible evidence.
