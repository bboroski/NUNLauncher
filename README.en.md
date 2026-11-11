<p align="center"><img src="assets/duck-ui/duck-mark.svg" alt="NUNLauncher mark" width="92"></p>
<h1 align="center">NUNLauncher</h1>
<p align="center"><strong>Kernel Bootstrap Supervisor</strong></p>
<p align="center"><img src="https://img.shields.io/badge/STATUS-HOST%20VALIDATOR-FFD600?style=for-the-badge&labelColor=111111"><img src="https://img.shields.io/badge/IMPLEMENTATION-NO%20FIRMWARE%20HANDOFF-FFD600?style=for-the-badge&labelColor=111111"><img src="https://img.shields.io/badge/LICENSE-NOT%20DECLARED-FFD600?style=for-the-badge&labelColor=111111"></p>
<p align="center"><a href="README.md">🇺🇦 Українська</a> · <a href="README.en.md">🇬🇧 English</a> · <a href="README.ru.md">🇷🇺 Русский</a></p>

---

**NUNLauncher defines the future narrow boundary between firmware/bootloader and the NUN minimal kernel.** Its responsibility is to validate an image, prepare boot context, and transfer control through a defined entry point.

## Intended functions

- Validate kernel-image format and version.
- Validate memory map, alignment, and permitted address ranges.
- Check required platform capabilities.
- Build a versioned boot-information block.
- Transfer control through an architecture entry point.
- Report pre-handoff failures explicitly instead of silently entering an unknown mode.

## Current status

The repository now contains a portable C11 host-side validator for the 48-byte kernel-image header, architecture match, entry offset, and load-span containment in a caller-supplied memory map. A CMake build, unit tests, and GCC/Clang CI are included. This is **not yet a bootloader or bootable binary**: firmware entry code, linker scripts, architecture handoff, a secure signature chain, and emulator/hardware boot tests remain unimplemented. The manual E2K workflow is prepared but requires a self-hosted runner with LCC; it is not evidence of qualified support.

## NUN contract

[NUN](https://github.com/bboroski/NUN) must independently validate the handoff context before initialization. NUNLauncher is responsible only for image validation, boot-information construction, and controlled transfer. Memory management, scheduling, and drivers belong to the kernel.

## Implementation plan

| Stage | Readiness evidence |
|---|---|
| Image contract | Format, version, checksum, and compatibility are defined |
| Memory validation | Boundary, alignment, and address-range tests pass |
| Capability gate | An incompatible platform is rejected before handoff |
| Boot information | Handoff structure is versioned and checked by the kernel |
| Handoff | Entry point is verified in an emulator and on target hardware |

Pre-handoff failures must be explicit. An unknown mode must never be entered silently. Host tests do not prove a real boot on a CPU or board.

## Ecosystem and license

Primary kernel project: [NUN](https://github.com/bboroski/NUN). Related components: [Kloz](https://github.com/bboroski/Kloz), [CO](https://github.com/bboroski/CO), [SaintSI](https://github.com/bboroski/SaintSI), and [Klipot](https://github.com/bboroski/Klipot).

No <code>LICENSE</code> file is present; no license is declared. Define the terms before public distribution.

---

**Built with love for systems engineering and people.**