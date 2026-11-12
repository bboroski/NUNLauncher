# NUN integration — boot image validation and handoff

## Assigned responsibility

NUNLauncher owns the host-side validation contract for kernel image metadata and the future boundary between firmware/boot infrastructure and the NUN kernel. It does not own the kernel, HAL or device drivers.

## Current verified state

- The project contains a portable C11 validator for its 48-byte `NUNIMG1` image header and a caller-supplied memory map.
- The validator checks header/version/architecture fields, bounds, alignment and CRC-32.
- CRC-32 detects accidental corruption only; it is not a cryptographic signature or proof of publisher identity.
- Firmware entry, UEFI/BIOS loading, architecture handoff assembly, Secure Boot and a real emulator bootloader are not implemented.

Phase A in [NUN](https://github.com/bboroski/NUN) currently uses GRUB's Multiboot2 loader to boot the kernel ELF in QEMU. That test path does not imply that NUNLauncher is already the firmware loader.

## Integration contract

1. Keep the `NUNIMG1` header distinct from the Multiboot2 protocol. A future adapter must explicitly translate validated image metadata into a versioned boot-information structure.
2. Validate every size, offset, address range and arithmetic operation before a caller performs a copy or control transfer.
3. Keep memory-map acquisition and firmware trust decisions outside the pure host validator.
4. Add cryptographic signature verification only with a reviewed trust-root, key rotation, revocation and rollback policy. CRC must never be relabeled as authentication.
5. Preserve architecture-specific handoff code behind a separate target adapter; x86_64 rules must not be reused for ARM64 or E2K.

## Acceptance gates

- Existing GCC/Clang host tests remain green.
- Golden test vectors cover the serialized header.
- A future integration test verifies the adapter against NUN's versioned boot-info structure.
- QEMU image boot and physical firmware qualification remain separate gates.

## ABI status

No NUN kernel ABI is frozen by this change. NUN is currently experimental 0.x; no syscall, module or userspace ABI is being introduced here. Record future NUN-facing format/API changes in the root `CHANGELOG.NUN`.
