# Swift 6+ Modular-Monolith Adoption Contract

**Status:** architecture baseline; implementation is incremental and must be proven by repository CI. **Language mode:** Swift 6; pin the exact toolchain.

## Objective and safety rules

Use an in-process Swift core only where the target and deployment model support it. Use SwiftPM targets with acyclic dependencies and typed APIs; preserve hardware, privilege, network, and fault-containment boundaries.

- Enable Swift 6 language mode and complete strict-concurrency checking per target; fix diagnostics rather than blanket-suppressing them.
- Protect shared mutable state with actors or narrow synchronization. Audit every `@unchecked Sendable`, unsafe pointer, native bridge, and mutable global.
- `Sendable` is a checked concurrency contract, not a module-access restriction. Keep non-Sendable values in their owning isolation domain.
- Prefer value semantics; use `~Copyable`, `borrowing`, and `consuming` only when supported by the pinned compiler and justified by measurements.
- Partition SwiftPM targets into Domain, Application, Infrastructure, Adapters, and Tests; avoid cyclic dependencies.
- Replace only unnecessary in-process HTTP/JSON hops with typed calls. Retain network/process/device/privilege boundaries that provide independent lifecycle or containment.
- Preserve APIs, ABI, schemas, file formats, boot contracts, and cross-language interfaces. Version incompatible changes and test compatibility.
- Select embedded or external persistence from durability and multi-writer needs; test transactions, WAL/checkpointing, crash recovery, backup, and restore.
- Shutdown by stopping ingress, cancelling owned task groups, draining owned transports, flushing durable state, and reporting incomplete cleanup.
- Benchmark real workloads (toolchain, hardware, p50/p95/p99, throughput, allocations, memory) before optimization.
- Enable CMO/LTO only in supported release configurations after correctness and reproducibility checks.
- CI must pin toolchains, build with strict concurrency, test native boundaries, and validate supported targets.

## Adoption gates

A. Inventory languages, deployables, module graph, public interfaces, persistence, unsafe code, and target constraints.  
B. Define the smallest stable Swift core API and versioned contracts at genuine process boundaries.  
C. Migrate target-by-target to Swift 6 complete concurrency checking.  
D. Isolate C/C++ through Clang modules or a narrow C ABI when direct interop is unsuitable; test ownership and error propagation.  
E. Select storage from durability/multi-writer needs and implement orderly lifecycle.  
F. Profile before selectively enabling LTO/CMO.  
G. Merge only after required CI, ABI checks, and review pass.

## Repository-specific scope

NUNLauncher owns boot orchestration and validation. Keep bootloader stages, boot handoff structures, firmware interfaces, and kernel ABI stable; use Swift only for supported host-side orchestration/validation unless target-specific Embedded Swift is demonstrated.



This is an adoption contract, not a claim that Swift code or a SwiftPM package already exists. Preserve this repository's native target and integration contracts; Swift components must be additive, versioned, and validated against existing consumers.

## Acceptance criteria

- Pinned toolchain and reproducible SwiftPM build for each Swift target.
- Complete strict-concurrency checks with no unexplained suppressions.
- Tests for isolation, cancellation, persistence recovery, and native-boundary ownership where applicable.
- Documented module/deployment graph and versioned external contracts.
- Benchmarks and CI evidence attached to implementation PRs.
