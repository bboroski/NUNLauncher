# GUI / Visual System Standard

> Design contract for future host tooling. NUNLauncher currently contains documentation/specification materials, not a completed bootloader GUI or verified boot implementation.

## Visual identity

| Token | Value | Use |
|---|---|---|
| Primary yellow | `#FFCC00` | Main action and selected item |
| Highlight yellow | `#FFE600` | Focus and progress |
| Black / graphite | `#000000` / `#1A1A1A` | Structural surfaces and outlines |
| Error red | `#E53935` | Validation and boot errors |
| Signal cyan | `#29B6F6` | Live validation/data flow |
| White | `#FFFFFF` | Text and labels |

Existing Duck UI tokens/assets remain canonical where present. Palette changes must be synchronized across source tokens, styles, previews and tests.

## Future workspace

A future host utility may present a header with app mark, real validation status/progress and local time; an image/manifest validation canvas; contextual handoff diagnostics; and a pager for multi-stage workflows. `55 / ~~~ / 21:33` are conceptual placeholders only.

## Launcher-specific boundaries

- A mockup is not a bootloader, executable image, verified handoff or hardware test.
- The intended flow separates image format/version checks, checksum, address-range/alignment validation, capability gating, boot-information generation and handoff.
- Unknown platform modes fail closed and display a specific reason.
- Never claim an OS image was started unless an actual target/emulator run produced evidence.
- Long-running build/emulator steps display real progress and artifacts, not fabricated status.

## Accessibility and release gate

Host tools provide keyboard-visible focus, semantic labels, text/icon states, reduced motion, scalable layout and WCAG AA contrast (4.5:1 normal text; 3:1 large text). Conceptual screens are labelled **concept**. A future runnable tool requires tests, screenshots from the build and evidence for every supported platform.

## Українська

NUNLauncher поки описує bootstrap/handoff-контракти; цей документ не означає наявність bootloader GUI чи готової реалізації. Майбутній інструмент має показувати справжні перевірки образу, checksum, межі адрес, alignment, capability gate і handoff. `55 / ~~~ / 21:33` — умовні значення макета. Непідтримуваний режим відхиляється з причиною.

## Русский

NUNLauncher пока описывает bootstrap/handoff-контракты; этот документ не означает наличие bootloader GUI или готовой реализации. Будущий инструмент должен показывать реальные проверки образа, checksum, границы адресов, alignment, capability gate и handoff. `55 / ~~~ / 21:33` — условные значения макета. Неподдерживаемый режим отклоняется с причиной.
