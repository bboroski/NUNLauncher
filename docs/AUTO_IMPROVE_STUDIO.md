# AutoImprove Studio

AutoImprove Studio is an optional native Qt 6 Widgets host application delivered beside NUN Workbench. It is deliberately outside the freestanding NUN kernel.

## Build

The target is part of the optional desktop build:

```sh
cmake -S . -B build -DNUNLAUNCHER_BUILD_DESKTOP=ON
cmake --build build --target NUNLauncher.AutoImprove
```

Qt 6.4 Widgets and a C++17 compiler are required. If Qt is unavailable, the existing C11 library and tests remain buildable and the optional desktop targets are skipped.

## First iteration

1. Select a local repository/workspace.
2. Choose **Observe & generate matrix**.
3. Review the three-stage ternary matrix.
4. Export the versioned JSON evidence record.
5. Optionally create a child iteration. The parent matrix ID is retained and prior evidence is not rewritten.

The observer reads host OS/CPU and root-storage metadata, counts files (with a 100,000-file cap), invokes only `git status --porcelain=v1 --untracked-files=no`, and inspects `ecosystem/auto-improvement.json` when present (maximum 1 MiB). A valid adapter supplies that repository's own domain, role and capabilities to the matrix; a missing adapter remains UNKNOWN and an invalid adapter blocks promotion. Its bytes are represented by a SHA-256 evidence digest. It does not execute repository build scripts, tests, generated code, package managers, or arbitrary shell commands.

## Ternary semantics

- `1`: observed fact supported by recorded evidence.
- `0`: unknown, not measured, pending validation, or pending approval.
- `-1`: reserved for a demonstrated regression or failed safety gate.

The first iteration records observation evidence and intentionally leaves performance, security, build/test, approval, post-change verification, and rollback readiness as unknown until repository-specific evidence exists.

## Safety boundary

This is an evidence collector and matrix planner, not an autonomous OS tuner. It does not edit source files, change OS settings, install software, load drivers, alter boot configuration, flash firmware, elevate privileges, merge pull requests, or write to `main`. Any future mutating adapter must use a dedicated branch, a reviewable diff, explicit human approval, project-specific tests, and a rollback plan.

NUN's kernel remains freestanding C. Qt, host tooling, and this application are never linked into the kernel.
