# NUNLauncher — Kernel Bootstrap Supervisor

NUNLauncher is the pre-handoff admission layer for NUN boot images. It validates
the image manifest, exact file size and SHA-256, x86_64 target contract, BIOS
and UEFI El Torito entries, the ELF64 kernel entry point and the AMD64 PE/COFF
UEFI fallback loader.

## Implemented

- Python CLI and fail-closed manifest validation.
- Read-only ISO inspection with xorriso.
- ELF64/LE/EM_X86_64/ET_DYN and executable PT_LOAD entry checks.
- EFI/BOOT/BOOTX64.EFI PE/COFF AMD64 checks.
- Host-side positive and negative unit tests.
- CI that builds NUN, verifies the image, and boots it in QEMU through BIOS and
  x86_64 UEFI (OVMF).

Run:

    make test
    make verify IMAGE=../NUN/build/NUN.iso MANIFEST=../NUN/build/NUN.image.json

Firmware launches Limine from the hybrid image; Limine loads the NUN kernel and
performs the protocol handoff. NUNLauncher is the image admission supervisor,
not a replacement firmware or a second-stage kernel loader.

The currently qualified profile is **x86_64 with BIOS and x86_64 UEFI only**.
No AArch64, RISC-V, E2K/Elbrus, Apple Silicon or other architecture support is
claimed.

NUN images are currently unsigned. SHA-256 detects file/manifest mismatch but
does not authenticate the publisher. Secure Boot trust provisioning, signed
manifests, key rotation and TPM attestation are not implemented. The
`--require-signature` option therefore fails closed.

See [BOOT_SUPERVISOR.md](docs/BOOT_SUPERVISOR.md) and
[the accepted image manifest schema](spec/nun-image-manifest.schema.json).
