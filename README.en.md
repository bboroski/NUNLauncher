# NUNLauncher
**Kernel Bootstrap Supervisor**

[Українська](README.md) · [English](README.en.md) · [Русский](README.ru.md)

A host-side supervisor that validates kernel-image metadata and boot-information contracts before a possible handoff.

## Features
- Host validator for a 48-byte kernel-image header.
- Architecture, entry offset and caller-provided memory-map bounds validation.
- Portable CMake build, host tests and GCC/Clang CI.
- Versioned boot-information contract and capability-gate design.

## Run and test
```bash
cmake -S . -B build
cmake --build build --config Release
ctest --test-dir build --output-on-failure
python one.py check
make test
```

## Scope
The supervisor does not write an image to disk or transfer control to a kernel. QEMU CI covers only tested emulator profiles; E2K ELF validation is not a bare-metal E2K boot path.

## Project documentation
This host-side utility has no separate GUI. Test output and CI logs are the relevant visual evidence.

## Related
[NUN](https://github.com/bboroski/NUN) · [Fragile-Baby](https://github.com/bboroski/Fragile-Baby)

## License
No root license file is declared; review the repository terms before reuse.

---
**两心相印，白首不离。**
*Two hearts understand each other and stay together until old age.*
— Traditional Chinese expression
