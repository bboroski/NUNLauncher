# NUN desktop application roadmap

## Direction

Build a coherent desktop suite in dependency order. First deliver host tools
that improve NUN kernel development, image admission and emulator testing.
NUN-native applications must wait for stable user-space, VFS, device and
graphics contracts.

The 666-item desktop concept register is an idea pool, not a commitment to
implement every entry. Select tools that directly advance the NUN roadmap.

## Portfolio

| Product | Responsibility | Delivery gate |
|---|---|---|
| **NUN Workbench** | Workspace, build, tests, image verification and QEMU in one host console | First implementation |
| **NUN Image Inspector** | Read-only manifest, digest, ELF/PE, boot-entry and compatibility inspection | Stable validator output contract |
| **NUN BootLab** | Named BIOS/UEFI/QEMU profiles, repeatable launch settings, serial capture and test evidence | Versioned boot profiles and evidence schema |
| **NUN Trace Studio** | Search and correlate serial traces, faults, boot stages and timing | Stable trace/event formats |
| **NUN Device Matrix** | Hardware inventory, driver capability coverage and qualification evidence | HAL/device capability descriptors |
| **NUN Package Desk** | Inspect, stage, verify, install and roll back signed packages | Package format, trust store and transactional updater |
| **NUN System Settings** | Native settings for devices, display, input, networking and users | Userland services and stable system APIs |

## Shared foundation

- Workspace registry for local repositories and target profiles.
- Argument-based command runner with cancellation and bounded logs.
- Artifact catalog containing hash, format, architecture, provenance and
  verification status.
- Evidence journal for timestamps, tool versions, build/test/boot results and
  artifact hashes.
- Versioned QEMU, firmware and hardware profiles.
- Shared Duck UI tokens, accessibility, keyboard operation and semantic status.

## Platform policy

- Host tools: Windows and Linux first; macOS after scripts and toolchains have a
  tested path.
- NUN-native tools: only after stable user ABI, process model, memory management,
  VFS, input, display/compositor and device services.
- Keep GUI optional and isolated from C11 kernel and contract libraries.
- Qt 6 Widgets is used for the first host application; review Qt's licensing
  and distribution obligations for every release package.
- External tools remain explicit dependencies. The UI never downloads and runs
  an unreviewed tool implicitly.

A feature becomes implemented only with source, automated tests and a
reproducible runtime demonstration. A mockup or host build is not hardware
qualification. Preserve the labels implemented, partial, planned and
hardware-qualified.
