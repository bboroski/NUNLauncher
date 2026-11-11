<p align="center"><img src="assets/duck-ui/duck-mark.svg" alt="NUNLauncher mark" width="92"></p>
<h1 align="center">NUNLauncher</h1>
<p align="center"><strong>Kernel Bootstrap Supervisor</strong></p>
<p align="center"><strong>Portable C11 host validator · no firmware handoff</strong></p>
<p align="center"><a href="README.md">🇺🇦 Українська</a> · <a href="README.en.md">🇬🇧 English</a> · <a href="README.ru.md">🇷🇺 Русский</a></p>

> **NUNLauncher** — NUNLauncher defines the boundary between firmware/bootloader and the NUN kernel, validating kernel-image metadata ahead of a future handoff.

## Current capabilities
- Host-side validator for a 48-byte kernel-image header.
- Architecture, entry offset and caller-provided memory-map bounds validation.
- Portable CMake build, host tests and GCC/Clang CI.
- Versioned boot-information contract and capability-gate design.

## Quick start and verification
```bash
cmake -S . -B build
cmake --build build --config Release
ctest --test-dir build --output-on-failure
python one.py check
```

## Architecture and verified scope
This is not yet a bootloader or bootable binary. Firmware entry, linker scripts, architecture handoff, secure signature chain and real emulator/hardware boot are not implemented. Host validation does not prove OS startup.

## Engineering quality
- Run the checks appropriate to the target platform; host CI is not hardware qualification.
- Mark capabilities as implemented, partial or planned according to code, tests and evidence.
- A mockup, specification or open pull request is not proof of a production-ready feature.
- External integrations must be explicit, versioned, isolated and testable.

## NUN Mesh and related repositories
[SAMBU](https://github.com/bboroski/SAMBU) · [ToolNG](https://github.com/bboroski/ToolNG) · [ReaperCripple](https://github.com/bboroski/ReaperCripple) · [Kloz](https://github.com/bboroski/Kloz) · [CO](https://github.com/bboroski/CO) · [SaintSI](https://github.com/bboroski/SaintSI) · [Klipot](https://github.com/bboroski/Klipot) · [NUN](https://github.com/bboroski/NUN) · [NUNLauncher](https://github.com/bboroski/NUNLauncher) · [Fragile-Baby](https://github.com/bboroski/Fragile-Baby) · [Luci](https://github.com/bboroski/Luci)

Each repository retains its canonical name, code, APIs, release lifecycle and ownership boundaries. Integration means versioned contracts, not automatic merging of independent runtimes.

## Design and documentation
The shared interface direction is **Duck Black / Yellow**: dark surfaces, yellow accents for actions and state, readable typography, accessible contrast, keyboard navigation and reduced-motion support. See [design/DUCK_UI_SYSTEM.md](design/DUCK_UI_SYSTEM.md) when present.

## License
License not declared; no LICENSE file is present.

---
**Built with care for engineering, clarity and people.**


## GUI and contribution standard

The shared visual identity uses deep black `#111111` and Duck Yellow `#FFD600`. Repository-specific GUI boundaries, real-versus-concept status, accessibility and evidence requirements are defined in [GUI Design Standard](docs/gui/GUI_DESIGN_STANDARD.md). Use the [Pull Request template](.github/PULL_REQUEST_TEMPLATE.md) for changes; merge policy requires two independent approvals and passing required CI checks, subject to branch protection.
