# NUNLauncher — Kernel Bootstrap Supervisor

**A specification scaffold for controlled system-kernel startup.**

Current status: **documentation scaffold only**. The repository does not yet contain launcher code, a bootloader, image configuration, a build pipeline, or tests. NUNLauncher defines the intended narrow contract between firmware/a bootloader and the minimal kernel substrate MKS.

[Українська](README.md) · [English](README.en.md) · [Русский](README.ru.md) · [Repository](https://github.com/bboroski/NUNLauncher)

## Purpose

A future bootstrap supervisor should:
- validate the kernel-image format and version;
- validate the memory map, alignment, and permitted address ranges;
- verify required platform capabilities;
- construct a versioned boot-information block;
- transfer control through a defined architecture entry point;
- record failures before handoff instead of silently falling back to an unknown mode.

## Kernel contract

NUNLauncher should not duplicate the kernel memory manager, scheduler, or device drivers. Its scope is image validation, boot-information construction, and controlled handoff. The kernel must validate the received context before initialization.

## Current verification status

There is no implementation, build, or test suite yet. No bootable binary or verified architecture support exists.

## Technical identity

**NUNLauncher** is the canonical name of the planned bootstrap supervisor. The GitHub address is retained for link compatibility.

## Status

Do not use this repository as a bootloader: no launcher or bootloader implementation exists yet.

## Academic biology glossary

- [500 academic terms EN–RU](data/biology/academic_terms_500.tsv)
- [Data contract and usage boundaries](docs/BIOLOGY_DOMAIN_PACK.md)

An offline reference pack for education and metadata; it does not control systems, devices or diagnostics.
