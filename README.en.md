<p align="center"><img src="assets/duck-ui/duck-mark.svg" alt="NUNLauncher mark" width="92"></p>
<h1 align="center">NUNLauncher</h1>
<p align="center"><strong>Kernel Bootstrap Supervisor</strong></p>
<p align="center"><img src="https://img.shields.io/badge/STATUS-HOST%20VALIDATOR-FFD600?style=for-the-badge&labelColor=111111"><img src="https://img.shields.io/badge/IMPLEMENTATION-IMAGE%20CHECKER-FFD600?style=for-the-badge&labelColor=111111"><img src="https://img.shields.io/badge/LICENSE-NOT%20DECLARED-FFD600?style=for-the-badge&labelColor=111111"></p>
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

The repository now contains a host-side validator (`tools/validate_boot_image.py`) and unit tests for the JSON manifest, SHA-256 digest, safe image path, memory bounds, and entry alignment. This is **not a bootloader**: architecture-specific startup/loader code, a linker script, a complete boot pipeline, and a bootable binary are still absent. No architecture is qualified; E2K is accepted only as an experimental metadata-validation target.

## Host-side image validation

```sh
python tools/validate_boot_image.py path/to/kernel.manifest.json --target x86_64
python -m unittest discover -s tests -p 'test_*.py' -v
```

The validator does not load or execute code. Manifest/digest validation does not replace a bootloader, image signature verification, emulator testing, or physical-target qualification.

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