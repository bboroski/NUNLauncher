# Auto-Improvement Workbench

## Purpose

The NUN Workbench is the native desktop control plane for continuously improving the NUN operating-system project and its companion applications. It coordinates repository adapters; it does not become a second kernel, silently rewrite source trees, or claim that an untested proposal is an improvement.

The existing NUNLauncher desktop-workbench implementation is the UI foundation. This specification extends it with a governed improvement loop and a three-stage ternary matrix.

## Three-stage ternary improvement matrix

Every candidate improvement is represented as a versioned matrix with three ordered stages:

1. **Observe and baseline** — inventory repository state, identify current behavior, collect reproducible build/test/security/performance evidence, and record unknowns.
2. **Propose and validate** — generate bounded alternatives, calculate dependencies and risk, run static checks and isolated tests, compare against the baseline, and produce a rollback plan.
3. **Approve and apply** — present a reviewable change set, require explicit approval for mutations, apply through a branch/PR, then verify the resulting state and preserve evidence.

Each cell uses a ternary result:

- `-1`: regression, failed gate, unsafe action, or evidence contradicting the proposal.
- `0`: unknown, not measured, not applicable, or no verified change.
- `1`: measured improvement with reproducible supporting evidence.

A stage cannot be promoted when required evidence is missing. A matrix is not a numerical score that can hide a failed safety gate: any critical `-1` blocks promotion. Unknown (`0`) is never silently treated as success.

## Matrix lifecycle

`discover → baseline → generate → validate → rank → review → approve → apply → verify → learn`

- Matrix generation may be automatic and repeatable.
- Matrix refinement may generate a new immutable version; it must not rewrite prior evidence.
- The engine may automatically improve its own proposals, tests, and prioritization rules in a sandbox.
- The engine must not autonomously grant itself permissions, disable policy gates, merge PRs, install packages, alter boot configuration, flash firmware, or change kernel/system files.
- Every applied change is branch-based, diff-visible, attributable, and reversible.

## Native desktop modules

- **Repository Observatory** — discovers configured repositories and their current branch, working tree, CI and open PR state.
- **Baseline Lab** — runs only declared, bounded, repository-specific checks and records evidence provenance.
- **Ternary Matrix Studio** — displays stage × subsystem × evidence matrices, dependencies, confidence and blocked gates.
- **Proposal Foundry** — generates small, scoped proposals from repository contracts and verified gaps.
- **Validation Orchestrator** — schedules static checks, tests, build jobs and isolated experiments.
- **Policy & Approval Center** — enforces per-action approvals and repository-specific guardrails.
- **Change Composer** — creates branches and PRs; never writes directly to `main`.
- **Regression Sentinel** — compares post-change evidence to the baseline and recommends rollback when a regression is detected.
- **Audit Ledger** — stores immutable matrix versions, evidence references, decisions, approvals and resulting commit SHAs.

## Ecosystem adapter responsibilities

The integration contract at `ecosystem/auto-improvement.json` is present in each companion repository. Adapters remain domain-specific:

| Repository | Adapter responsibility |
| --- | --- |
| NUN | OS capability profiles, kernel/build evidence; no Python in kernel |
| SAMBU | host-side matrix generation, static analysis and reproducible planning |
| NUNLauncher | native desktop UI, orchestration, approval and audit |
| Klipot | repository registry and adapter discovery |
| Luci | policy, risk and fail-closed validation |
| ToolNG | read-only Windows host diagnostics |
| SaintSI | integrity and health evidence |
| CO | deterministic component lifecycle evidence |
| Kloz | ABI, memory and portable substrate contracts |
| Hellduck | workspace and cross-module validation |
| ReaperCripple | firmware workflow and target-specific evidence |
| Fragile-Baby | E2K/toolchain/platform qualification evidence |

## Safety and correctness invariants

1. Read-only discovery is the default.
2. No arbitrary shell command or generated code runs with elevated privileges.
3. Every action has a declared scope, timeout, resource budget and expected outputs.
4. Destructive or externally visible operations require explicit approval.
5. No repository's identity, Git history, license or ownership is replaced by the integration.
6. Host tests, emulation and hardware qualification are distinct evidence classes.
7. Secrets are never copied into matrix records or logs.
8. A failed validation blocks promotion; rollback is a first-class plan.
9. The app reports unavailable evidence as unknown, not as success.
10. NUN's freestanding C kernel contract remains separate from host-side desktop and Python tooling.

## Delivery sequence

- **Foundation:** repository contracts, matrix schema, read-only discovery, baseline snapshots.
- **Planner:** deterministic matrix generation, validation and ranking.
- **Desktop:** native matrix studio, approval center and audit ledger.
- **Adapters:** domain-specific integration and checks for all repositories.
- **Continuous loop:** scheduled observation, proposal refresh, regression detection and user-approved application.

## Current implementation boundary

The repository integration manifests define the protocol and safety contract. They do not, by themselves, prove that every adapter, native UI screen, system optimizer, or autonomous improvement loop is implemented. Those capabilities must be delivered and tested in incremental PRs with evidence.
