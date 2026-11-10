# NUNLauncher — Amber–Obsidian interface specification

## Product boundary

**NUNLauncher** — Kernel image validation and bootstrap handoff supervisor.

NUNLauncher is a specification-only repository. This design illustrates the future validation flow; it is not a working bootloader, firmware UI or proof of a bootable binary.

This repository's current implementation status is preserved. The preview is a static design concept, not a functioning application or evidence of platform support.

## Visual system

- Canvas: `#0B0B0D`; graphite: `#141416`, `#1C1C1E`, `#2C2C30`.
- Border: `#34343A`; text: `#F5F3EE`; muted metadata: `#A6A3AB`.
- Project accent: `#FF9F0A`; pressed: `#C76F13`; secondary: `#A78BFA`.
- State colours: passed `#64C787`; caution `#FFBF47`; fault `#F16D65`; unknown `#A78BFA`.

## Proposed information hierarchy

1. **IMAGE VALIDATION** — high-level context.
2. **MEMORY MAP** — inspectable project contract.
3. **HANDOFF** — qualification and verification gate.

Use a fixed identity rail, compact navigation, a primary work surface and a docked diagnostics panel. Keep statuses textual and explicit. Numeric values use monospaced tabular figures. Provide keyboard focus, clear target areas and reduced-motion support when interactive components are implemented.

## Readiness policy

Never display a successful state without test evidence. Use `NOT IMPLEMENTED`, `UNKNOWN`, `UNVERIFIED` and `SPECIFICATION ONLY` where appropriate. Design artwork is not an implementation claim.

## Artwork

- [Project mark](../assets/amber-obsidian-mark.svg)
- [Interface concept preview](../assets/amber-obsidian-preview.svg)

## Tokens

```css
:root {
  color-scheme: dark;
  --ao-canvas: #0B0B0D;
  --ao-surface-1: #141416;
  --ao-surface-2: #1C1C1E;
  --ao-surface-3: #2C2C30;
  --ao-border: #34343A;
  --ao-text: #F5F3EE;
  --ao-muted: #A6A3AB;
  --ao-accent: #FF9F0A;
  --ao-accent-pressed: #C76F13;
  --ao-secondary: #A78BFA;
  --ao-success: #64C787;
  --ao-warning: #FFBF47;
  --ao-danger: #F16D65;
}
```
