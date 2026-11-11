## Summary
- [ ] Describe the specification or implementation change.
- [ ] Preserve canonical NUNLauncher and boot-contract names.

## Constraint → verified capability
- Existing evidence:
- Implemented behavior:
- Required emulator/target proof:

## Documentation and UI
- [ ] README and affected UA/EN/RU variants updated.
- [ ] Future host UI follows `docs/gui/GUI_DESIGN_STANDARD.md`.
- [ ] Mockups are labelled; no bootloader/boot success claim without evidence.
- [ ] Image format, checksum, address bounds, alignment and handoff fields documented.

## Verification
- [ ] Documentation/schema checks and CI pass.
- [ ] Unknown platform/mode fails closed.
- [ ] Changed code meets 80% coverage where configured or exception justified.
- [ ] Host tests, emulator validation and physical target results are distinguished.
- [ ] Compatibility and failure paths reviewed.

## Review policy
- Branches: `feature/<ticket>-<slug>`, `fix/<slug>`, `docs/<slug>`.
- No direct commits to protected default/release branches.
- Merge requires two independent approvals and all required CI checks, enforced through branch protection.
- [ ] Two independent approvals.
- [ ] Required checks green.

## Українська / Русский
- [ ] README та локалізації оновлено.
- [ ] Специфікацію не подано як готовий bootloader.
- [ ] Два незалежні схвалення та успішний CI перед злиттям.
