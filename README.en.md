
> **Brand palette:** signal yellow `#FFD600`; deep black `#111111`.
<p align="center"><img src="assets/duck-ui/duck-mark.svg" alt="NUNLauncher mark" width="92"></p>
<h1 align="center">NUNLauncher</h1>
<p align="center"><strong>Kernel Bootstrap Supervisor</strong></p>
<p align="center"><strong>Pre-handoff image admission · unsigned</strong></p>
<p align="center"><a href="README.md">🇺🇦 Українська</a> · <a href="README.en.md">🇬🇧 English</a> · <a href="README.ru.md">🇷🇺 Русский</a></p>
<sub>Updated: 2026-11-11</sub>


## Contents

- [Current capabilities](#current-capabilities)
- [Quick start and verification](#quick-start-and-verification)
- [Architecture and verified scope](#architecture-and-verified-scope)
- [Interface and visual materials](#interface-and-visual-materials)
- [Engineering quality](#engineering-quality)
- [Documentation map](#documentation-map)
- [NUN Mesh and related repositories](#nun-mesh-and-related-repositories)
- [Design and documentation](#design-and-documentation)
- [License](#license)
- [Duck Black / Yellow palette](#duck-black-yellow-palette)

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
make test
```

Verify a built NUN ISO with `make verify IMAGE=../NUN/build/NUN.iso MANIFEST=../NUN/build/NUN.image.json`.

## Architecture and verified scope
NUNLauncher is a host-side image-admission layer: it verifies a built NUN ISO before boot, but does not replace firmware or Limine, transfer control to the kernel, or execute image code. CI separately boots the same image in QEMU through BIOS and x86_64 UEFI. Host preflight alone does not establish compatibility with arbitrary physical hardware.

Secure Boot, publisher signatures, key rotation and TPM attestation are not implemented. The current manifest explicitly marks the image unsigned; SHA-256 confirms a match with the manifest, not publisher identity.

## Interface and visual materials

NUNLauncher is a host-side C11 validator for boot metadata, not a graphical application. No GUI screenshots are available; the image below shows only the shared Duck Black / Yellow documentation style.

## Host-side image validation

`tools/validate_boot_image.py` checks the JSON manifest, SHA-256 digest, safe image path, memory bounds, and entry-point alignment. This is metadata validation, not a bootloader and not evidence that an architecture is supported.

```sh
python tools/validate_boot_image.py path/to/kernel.manifest.json --target x86_64
python -m unittest discover -s tests -p 'test_*.py' -v
```

The validator does not load or execute code. Signature verification and physical-target qualification remain separate stages.

## NUN boot-image supervisor

Run `make verify IMAGE=../NUN/build/NUN.iso MANIFEST=../NUN/build/NUN.image.json` for the full pre-handoff sequence: manifest/schema, ISO filename and size, SHA-256, BIOS/UEFI El Torito entries, the x86_64 ELF64 kernel and the AMD64 EFI fallback loader. Verification is read-only; it does not write to a disk or transfer control to the kernel.

`make test` runs the supervisor host tests. CI builds NUN from a pinned commit and boots the same ISO in QEMU through BIOS and x86_64 UEFI. This confirms the initial boot path in those emulated profiles, not a complete OS or physical-hardware qualification.

The separate `verify-e2k` command checks an E2K Linux userspace ELF (`EM_MCST_ELBRUS=175`) and its manifest. It is not a bare-metal E2K bootloader or kernel. Native execution is confined to a separate manual workflow on a self-hosted E2K runner.

<p align="center"><img src="assets/duck-ui/duck-ui-preview.svg" alt="Shared Duck Black Yellow design system; not a NUNLauncher interface" width="960"></p>

## Engineering quality
- Run the checks appropriate to the target platform; host CI is not hardware qualification.
- Mark capabilities as implemented, partial or planned according to code, tests and evidence.
- A mockup, specification or open pull request is not proof of a production-ready feature.
- External integrations must be explicit, versioned, isolated and testable.

## Documentation map

- [Kernel image contract](docs/BOOT_IMAGE_CONTRACT.md)
- [Boot-image supervisor and trust model](docs/BOOT_SUPERVISOR.md)
- [E2K Linux userspace admission](docs/E2K_RUNTIME.md)
- [Ecosystem integration](docs/ECOSYSTEM_INTEGRATION.md)
- [Engineering governance](docs/ENGINEERING_GOVERNANCE.md)

## NUN Mesh and related repositories
[SAMBU](https://github.com/bboroski/SAMBU) · [ToolNG](https://github.com/bboroski/ToolNG) · [ReaperCripple](https://github.com/bboroski/ReaperCripple) · [Kloz](https://github.com/bboroski/Kloz) · [CO](https://github.com/bboroski/CO) · [SaintSI](https://github.com/bboroski/SaintSI) · [Klipot](https://github.com/bboroski/Klipot) · [NUN](https://github.com/bboroski/NUN) · [NUNLauncher](https://github.com/bboroski/NUNLauncher)

Each repository retains its canonical name, code, APIs, release lifecycle and ownership boundaries. Integration means versioned contracts, not automatic merging of independent runtimes.

## Design and documentation
The shared interface direction is **Duck Black / Yellow**: dark surfaces, yellow accents for actions and state, readable typography, accessible contrast, keyboard navigation and reduced-motion support. See [design/DUCK_UI_SYSTEM.md](design/DUCK_UI_SYSTEM.md) when present.

## License
License not declared; no LICENSE file is present.

---
**Built with care for engineering, clarity and people.**


## Duck Black / Yellow palette
The legacy badge and mark compatibility colors remain `#FFD600` (yellow) and `#111111` (near-black). The current interface canvas is `#0D0D0D`. The complete six-color palette, fourteen functional shades and RGB/HSL values are maintained in `design/duck-ui.tokens.json` (schema 3.0.0). Yellow is reserved for primary actions, focus and selection; red and cyan identify semantic error and information states.
