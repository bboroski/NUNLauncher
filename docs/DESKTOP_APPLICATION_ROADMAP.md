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

## Native NUN desktop applications (future OS userland)

These applications are intended to run inside NUN after the operating system
has the required user-space services and stable APIs. They are not host tools
and are not part of the first Workbench increment.

| Product | Responsibility | Required OS capability gate |
|---|---|---|
| **NUN Shell** | Native terminal, command execution, jobs and interactive sessions | Init, process lifecycle, syscall ABI, console and input |
| **NUN Files** | File manager, search, permissions, mount view and removable media | VFS, file descriptors, filesystem drivers and permission model |
| **NUN System Monitor** | Process/thread view, memory, CPU, interrupt and device telemetry | Scheduler, process introspection, memory statistics and trace APIs |
| **NUN Network Desk** | Network interfaces, addressing, routes, DNS and connection diagnostics | Network stack, socket API and network device drivers |
| **NUN Control Center** | Display, keyboard, pointer, audio, device and user preferences | Device services, input stack, display/compositor and user/session APIs |
| **NUN Software Center** | Package discovery, signature checks, installation, updates and rollback | Signed package format, publisher trust store and transactional updater |

Native applications must consume versioned public OS APIs. They must not access
kernel internals, physical addresses or device registers directly. Until these
gates exist, the corresponding products remain architecture and interface
targets only; host-side simulators or mock data must be labelled explicitly.

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
