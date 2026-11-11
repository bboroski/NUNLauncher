# NUN OS integration contract — NUNLauncher

## Ownership
NUNLauncher owns boot-image metadata, image staging, pre-boot validation and the future UEFI/firmware handoff. NUN owns kernel entry, HAL and kernel initialization. The current QEMU smoke path uses GRUB Multiboot2 and does not yet execute NUNLauncher.

## Integration direction
1. Define a versioned boot manifest containing architecture, image digest, ABI version, boot protocol, minimum firmware/loader version and required memory map fields.
2. Validate image bounds and cryptographic signatures before handing control to an image.
3. Implement UEFI first, with Legacy BIOS compatibility treated as a separately tested backend.
4. Pass a bounded, typed boot-information structure to NUN; document ownership and lifetime for every pointer and memory region.
5. Preserve a recovery entry and never disable Secure Boot checks to make a development image load.

## Acceptance gates
- Invalid digest, signature, architecture and ABI are rejected.
- Boot information is range-checked and reserved memory is not returned to the kernel allocator.
- Firmware and QEMU tests identify the exact boot protocol used.
- GRUB-based tests do not get reported as UEFI validation.

## Current status
NUNLauncher is not yet part of the current GRUB Multiboot2 QEMU execution path. UEFI, Secure Boot and rollback handoff remain future milestones.
