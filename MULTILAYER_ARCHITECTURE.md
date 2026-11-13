# Multi-Axis Layer Architecture

**Architecture:** NUN Mesh — Multi-Axis Layer Fabric (MALF)  
**Repository:** `bboroski/NUNLauncher`  
**Role:** Host-side kernel image admission and pre-handoff validation  
**Maturity:** Host-side C11/ Python validation; image remains explicitly unsigned and handoff is not performed by the validator.  
**Status:** Target boundary contract, not a claim of complete OS/boot functionality.

## Purpose

MALF gives the system independent structural, vertical-slice, trust, lifecycle and evidence axes. Layers describe ownership; they are not mandatory directory names. Preserve separate bring-up paths unless an explicitly tested unification is designed.

## Structural layers

| ID | Layer | Responsibility |
|---|---|---|
| L0 | **Human and operator surface** | Operator tools, launch UI and diagnostics; not kernel privilege logic. |
| L1 | **Boot and subsystem contracts** | Boot handoff, syscall, ABI, image and subsystem data contracts. |
| L2 | **System orchestration** | Boot stages, subsystem initialization, admission and coordinated state transitions. |
| L3 | **Kernel domain** | Memory, execution, process, synchronization, fault and device models. |
| L4 | **Privilege and capability policy** | Address-space, privilege, resource and device-access authorization. |
| L5 | **Architecture-independent ports** | CPU, timer, interrupt controller, physical memory, console and device interfaces. |
| L6 | **Architecture/platform adapters** | x86_64/E2K or firmware-specific implementations, assembly entry, platform drivers. |
| L7 | **Memory and persistent state** | Memory maps, boot metadata, module tables and filesystem/storage state. |
| L8 | **Boot/runtime and delivery** | Build/link, image composition, bootloader integration, emulator and deployment. |
| L9 | **Verification and target evidence** | Host tests, boot tests, emulation, ABI checks, hardware qualification and logs. |

## Dependency and privilege law

- L0/L1 request well-defined operations; they do not bypass L4 privilege or capability checks.
- L2 owns initialization order and state transitions.
- L3 contains architecture-neutral invariants wherever practical.
- L5 is the abstraction boundary; L6 owns the machine-specific implementation.
- L7 memory ownership and persistence are explicit; no subsystem assumes a mapping or allocator that is not initialized.
- L8 composes/link-loads the image; L9 proves only the target/path actually exercised.

**Allowed:** boot/entry → validate handoff → establish minimal memory/CPU context → initialize bounded kernel subsystems → expose capability.  
**Forbidden:** user/driver path bypassing checks; unvalidated boot metadata controlling memory; unqualified target features inferred from host tests.

## Orthogonal architecture axes

1. **Structural:** L0–L9 subsystem ownership.
2. **Vertical slices:** boot, exception, memory, timer, device or image-admission path traced end-to-end.
3. **Trust:** firmware/bootloader input → validate → establish trusted execution state → capability gate → controlled operation → evidence.
4. **Lifecycle:** configure → compile/link → inspect image → boot in emulator → test → qualify hardware → release/recover.
5. **Assurance:** assertions, serial logs, test records and platform qualification remain separated by target and confidence.

## Repository-specific layer allocation

### L0 — Operator CLI and diagnostics
Commands for image validation, manifest selection and readable findings; no execution of submitted images.

### L1 — Image and handoff contracts
48-byte image header, JSON manifest, architecture identity, entry offset, memory ranges and boot-information contract.

### L2 — Admission workflow
Parse manifest → verify paths/size/hash → validate image format and architecture → check entry/memory bounds → produce admission result.

### L3 — Image admission domain
Image identity, supported target, entry-point semantics, memory-map bounds and validation outcome.

### L4 — Fail-closed admission policy
Reject unknown architecture, malformed metadata, unsupported entry offsets and out-of-range memory claims. Unsigned status is explicit.

### L5 — File and parser ports
Abstract file access, hash provider, manifest parser, image-format inspection and diagnostic reporting.

### L6 — Host validators and firmware boundary
Python/C11 validation tools, ELF/ISO inspection and external bootloader boundary. Validator never substitutes for firmware or Limine.

### L7 — Manifest and artifact state
Kernel image bytes, JSON manifest, digest, toolchain identity and reproducible validation evidence.

### L8 — Host build and emulator composition
CMake/Make, test executable, integration with NUN image build and separate QEMU BIOS/UEFI test jobs.

### L9 — Admission assurance
Schema tests, hash mismatch tests, path safety, malformed headers, image-boundary tests and separate emulator results.

## Vertical slices

1. **Kernel image validation:** Image+manifest → schema validation → path/size/hash → format/architecture → entry/memory bounds → explicit pass/fail report.
2. **NUN integration:** NUN build output → known manifest contract → NUNLauncher preflight → separate BIOS/UEFI QEMU evidence.
3. **Future signed admission:** Trusted key policy → signature verification → revocation/rotation policy → admission decision; keep this planned until implemented.

## Cross-cutting planes

- **Privilege/security:** explicit privilege transitions, memory isolation, validated pointers/ranges and least authority.
- **Fault/recovery:** defined fault states, diagnosable panic path and safe recovery policy; no silent continuation after invalid critical state.
- **Portability:** separate architecture-neutral logic from ISA/ABI/firmware-specific code.
- **Observability:** serial/diagnostic output is bounded and must not mutate state beyond the diagnostic contract.
- **Reproducibility:** pin toolchain and boot artifacts; record hashes and exact emulator/target parameters.
- **Performance:** use target-aware measurements, not extrapolation from host timings.

## NUN Mesh federation

Kernel and launcher remain separate buildable products. The launcher validates an image and handoff contract; it is not a replacement for firmware/bootloader and does not execute the kernel merely by validating it. Cross-repository contract versions are explicit; no implicit private ABI coupling.

## Incremental adoption

1. Inventory boot paths, architecture-specific code and current subsystem status.
2. Separate architecture-neutral kernel models from CPU/firmware adapters.
3. Freeze and validate boot, memory-map and image metadata contracts.
4. Gate privileged operations and every untrusted handoff field.
5. Add one vertical slice at a time with host tests, emulator evidence and target evidence kept distinct.
6. Preserve separate boot targets until there is repeatable compatibility proof.
7. Publish capability/maturity matrices with exact test paths.

## Definition of done

The entry/handoff contract is versioned; privilege and memory boundaries are explicit; architecture ports are isolated; boot and target claims have reproducible evidence; missing OS capabilities are labelled honestly.

## Repository guardrails

NUNLauncher does not boot, transfer control to or execute the kernel. SHA-256 equality proves byte identity against a manifest, not publisher identity. Secure Boot, signatures, key rotation and TPM attestation remain unimplemented unless supported by source and verification evidence.
