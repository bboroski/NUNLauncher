<p align="center"><img src="assets/duck-ui/duck-mark.svg" alt="Duck mark" width="104" /></p>
<h1 align="center">NUNLauncher</h1>
<p align="center"><strong>Kernel Bootstrap Supervisor</strong></p>
<p align="center"><img src="https://img.shields.io/badge/STATUS-SPECIFICATION%20ONLY-FFD600?style=for-the-badge&labelColor=111111" /><img src="https://img.shields.io/badge/IMPLEMENTATION-DOCUMENTATION%20SCAFFOLD-FFD600?style=for-the-badge&labelColor=111111" /></p>
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

<badge>SPECIFICATION ONLY</badge>

The repository does not yet contain launcher code, a bootloader, image configuration, a build pipeline, or tests. There is no bootable binary or verified architecture support. This is a specification scaffold, not a working loader.

## Project management and readiness gates

| Stage | Required evidence |
|---|---|
| Image contract | Format, version, checksum, and compatibility rules are defined |
| Memory validation | Boundary, alignment, and address-range tests pass |
| Capability gate | An incompatible platform is rejected before handoff |
| Boot information | Handoff structure is versioned and validated by the kernel |
| Handoff | Architecture entry point is verified in an emulator and on target hardware |

No architecture is claimed as supported until implementation and validation exist. Pre-handoff failures must be explicit; an unknown mode must never start silently.

## Responsibility boundaries

NUNLauncher does not manage kernel memory after the transfer, schedule tasks, or implement drivers. Responsibility passes to NUN after handoff.

## Ecosystem

[SAMBU](https://github.com/bboroski/SAMBU) · [ToolNG](https://github.com/bboroski/ToolNG) · [ReaperCripple](https://github.com/bboroski/ReaperCripple) · [Kloz](https://github.com/bboroski/Kloz) · [CO](https://github.com/bboroski/CO) · [SaintSI](https://github.com/bboroski/SaintSI) · [Klipot](https://github.com/bboroski/Klipot) · [NUN](https://github.com/bboroski/NUN).
