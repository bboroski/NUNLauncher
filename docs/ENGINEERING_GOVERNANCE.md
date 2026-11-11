# Engineering Governance — NUNLauncher

This document defines the repository-level change-control baseline. It does not claim that a control is enforced until the corresponding GitHub setting or required workflow check is enabled and verified.

## Pull request lifecycle
1. Work on topic branches; never push directly to `main`.
2. Use **Squash and merge** by default with a meaningful commit title.
3. Update the branch against `main`, resolve conflicts, and rerun checks.
4. Require at least one independent approval (two for high-risk, security-sensitive, ABI, kernel, boot, release, or cross-repository changes).
5. Require applicable CI checks and resolve every review conversation before merge.
6. Delete the head branch after merge.

## Required validation gates
Adapt commands to actual languages/build systems. Never add placeholder commands or claim unrun checks.
- Formatting, linting, static analysis, and schema/config validation.
- Unit and integration tests, including boundary, failure-path, and regression cases.
- Build/package validation for supported targets and platforms.
- Smoke/E2E tests for deployable or user-visible surfaces where applicable.
- Dependency and secret scanning; documentation/link checks where relevant.

Coverage target: **85% where meaningful and measurable**, using the established project baseline. Do not invent coverage for generated, hardware-only, unavailable-platform, or specification-only code. Report total and changed-line coverage; block regressions below the agreed project threshold.

## Merge and post-merge safety
- Protect `main` against force-push and deletion.
- Auto-merge only low-risk dependency patch/minor updates after required checks and review policy pass. Major/security-sensitive updates need human review.
- After merge, run main-branch build and integration/smoke checks. Deploy only through configured environments with approvals and health checks.
- Roll back only after a regression is confirmed. Do not auto-revert transient external-service failures or flaky tests; prevent rollback loops and preserve incident evidence.

## Documentation contract
- Update README and relevant `docs/` pages in the same PR as behavior, interface, build, or deployment changes.
- Keep Quick Start reproducible and limited to three commands where accurate.
- Tie badges to real workflows and coverage/version sources.
- Maintain Mermaid diagrams where they explain current architecture/data flow.
- Generate OpenAPI/GraphQL specs only if those interfaces actually exist.

## Dependency and security automation
Dependabot is configured for GitHub Actions updates. Add package-manager ecosystems only after confirming actual manifests/lockfiles. Review updates in isolated PRs. Enable Dependabot alerts, secret scanning, and push protection where plan and permissions allow.

## GitHub settings checklist (administrator action)
- [ ] Set `main` as default branch.
- [ ] Require PRs and agreed approval count; use CODEOWNERS for owned paths.
- [ ] Require applicable status checks and latest-push approval.
- [ ] Require conversation resolution.
- [ ] Block force pushes and deletion on `main`.
- [ ] Allow squash merging; disable unused merge methods if compatible.
- [ ] Enable automatic deletion of merged head branches.
- [ ] Enable Dependabot alerts, secret scanning, and push protection where available.

**Enforcement status:** checklist items are policy targets, not proof of active GitHub settings. Verify in repository Rulesets/Branches settings and record the result.
