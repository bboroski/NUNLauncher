<p align="center"><img src="assets/duck-ui/duck-mark.svg" alt="Duck mark" width="104" /></p>
<h1 align="center">NUNLauncher</h1>
<p align="center"><strong>Kernel Bootstrap Supervisor</strong></p>
<p align="center"><img src="https://img.shields.io/badge/STATUS-C11%20CONTRACT-FFD600?style=for-the-badge&labelColor=111111" /><img src="https://img.shields.io/badge/IMPLEMENTATION-HOST%20METADATA%20SLICE-FFD600?style=for-the-badge&labelColor=111111" /></p>
<p align="center"><a href="README.md">🇺🇦 Українська</a> · <a href="README.en.md">🇬🇧 English</a> · <a href="README.ru.md">🇷🇺 Русский</a></p>
<hr />

**A specification scaffold for controlled kernel startup and transfer of a validated execution context.**

## Product position

NUNLauncher defines the future narrow boundary between firmware/bootloader and the NUN minimal kernel. Its role is to validate incoming data and transfer control predictably without duplicating kernel subsystems.

## Intended responsibilities

- Validate the kernel image format and version.
- Validate the memory map, alignment, and permitted address ranges.
- Check required platform capabilities.
- Build a versioned boot-information block.
- Transfer control through a defined architecture entry point.
- Record failures before handoff without silently falling back to an unknown mode.

## Contract with the kernel

NUNLauncher must not duplicate the kernel memory manager, scheduler, or device drivers. It is responsible for image validation, handoff-data construction, and transition control. [NUN](https://github.com/bboroski/NUN) must independently validate the received context before initialization.

## Current status

**Current status: host-side image and handoff metadata contract added; no actual boot transfer yet.**

The C11 API validates magic/version, structure size, selected architecture, image range, entry offset, memory map, and consistency between the image descriptor and handoff block. CMake and host tests are provided (python one.py check). nunlauncher_handoff_prepare prepares a validated metadata copy; it does not perform a CPU jump, switch privilege levels, or create a bootable binary. The Android home-screen profile is separate and remains specification-only.

## Project management and readiness gates

| Stage | Required evidence |
|---|---|
| Image contract | Format, version, checksum, and compatibility rules are defined |
| Memory validation | Boundary, alignment, and address-range tests pass |
| Capability gate | An incompatible platform is rejected before handoff |
| Boot information | Handoff structure is versioned and validated by the kernel |
| Handoff | Architecture entry point is verified in an emulator and on target hardware |

A metadata architecture ID is not a support claim. A real handoff still needs an architecture-specific assembly entry, platform startup code, linker integration, emulator smoke tests, and target qualification. Pre-transfer failures remain explicit.

## Responsibility boundaries

NUNLauncher does not manage kernel memory after the transfer, schedule tasks, or implement drivers. Responsibility passes to NUN after handoff.

## Ecosystem

[SAMBU](https://github.com/bboroski/SAMBU) · [ToolNG](https://github.com/bboroski/ToolNG) · [ReaperCripple](https://github.com/bboroski/ReaperCripple) · [Kloz](https://github.com/bboroski/Kloz) · [CO](https://github.com/bboroski/CO) · [SaintSI](https://github.com/bboroski/SaintSI) · [Klipot](https://github.com/bboroski/Klipot) · [NUN](https://github.com/bboroski/NUN).
