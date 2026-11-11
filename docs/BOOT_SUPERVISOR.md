# NUNLauncher boot supervisor

## Responsibility boundary

NUNLauncher is the host-side image admission and pre-handoff supervisor. The
firmware launches Limine from the hybrid image; Limine loads the NUN ELF64 PIE
kernel and enters its Limine protocol entry point. NUNLauncher does not replace
firmware, implement a second BIOS stage, or jump directly into a kernel from an
Android HOME activity.

The current implementation validates the image before it is selected for boot.
The kernel independently validates the Limine boot context after entry.

## Supported target profile

| Field | Current target |
|---|---|
| CPU / ELF machine | x86_64 / EM_X86_64 |
| Firmware paths | Legacy BIOS + x86_64 UEFI |
| Boot protocol | Limine, base revision 6 |
| Kernel | ELF64, little-endian, ET_DYN |
| UEFI fallback | EFI/BOOT/BOOTX64.EFI, PE/COFF AMD64 |
| Bootloader | Limine 12.9.6 |

This is one qualified target profile. No AArch64, RISC-V, E2K/Elbrus, 32-bit
x86 or Apple Silicon boot support is claimed.

## Verification sequence

1. Parse manifest version and reject unknown fields.
2. Match image basename, exact byte length and SHA-256 digest.
3. Enforce the selected architecture and require BIOS and UEFI boot entries.
4. Inspect El Torito entries using xorriso.
5. Extract the kernel and check ELF64 class, byte order, machine, ET_DYN type,
   program-header bounds and that the entry point is inside an executable
   PT_LOAD segment.
6. Extract EFI/BOOT/BOOTX64.EFI and check the PE/COFF AMD64 machine type.
7. Return a typed rejection on any failure; do not silently downgrade or guess.

The verifier never writes to the image or a disk device. It is a preflight gate,
not a bootloader binary and not proof that the target machine has compatible
firmware settings.

## Commands

On Debian/Ubuntu, install Python 3 and xorriso:

    sudo apt-get install -y python3 xorriso

Run unit tests:

    make test

Verify a NUN image and its companion manifest:

    make verify IMAGE=../NUN/build/NUN.iso MANIFEST=../NUN/build/NUN.image.json

Equivalent direct command:

    PYTHONPATH=. python3 -m nunlauncher_boot.cli verify \
      --image ../NUN/build/NUN.iso \
      --manifest ../NUN/build/NUN.image.json

Use `--json` for machine-readable output. Use of `--require-signature`
fails closed because no publisher trust store or signature-verification backend
is configured yet.

## Trust model

SHA-256 verifies that the file matches the manifest. It does **not** authenticate
the publisher if an attacker can replace both. Current NUN images are marked
unsigned and the CLI states that explicitly. Secure Boot, signed manifests,
key rotation, revocation, measured boot and TPM attestation are not implemented.

## CI

The boot-supervisor workflow runs the NUNLauncher host tests, checks out the
pinned NUN commit `ae314deb44f334625c62623eb812d194ce92271c`, builds the same ISO through NUN's build pipeline, then
runs the image verifier and QEMU firmware smoke tests. Only x86_64 BIOS and
x86_64 UEFI are qualified by that pipeline.
