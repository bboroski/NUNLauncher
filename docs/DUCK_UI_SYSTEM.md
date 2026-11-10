# NUNLauncher Duck Black/Yellow UI System

The launcher inherits the canonical Duck UI tokens from SAMBU. This file defines Android-specific application rules.

## Palette

| Token | Value | Use |
|---|---|---|
| Canvas | `#0D0D0D` | Main Home surface |
| Surface | `#1A1A1A` | Search and app tiles |
| Alternate surface | `#141414` | Section backgrounds |
| Yellow accent | `#FFD600` | Brand, focus, selection and primary action |
| Muted yellow | `#BFA100` | Secondary accent |
| Main text | `#E6E6E6` | Labels and headings |
| Secondary text | `#999999` | Metadata and hints |
| Divider | `#262626` | Tile boundaries |

## Interaction rules

- App tiles are focusable, clickable and expose a screen-reader label.
- An icon is never the only source of application identity; show its label.
- Search has a visible label and a useful empty state.
- The Home surface must reflow at phone, tablet and foldable widths.
- Respect Android font scaling, system insets, landscape and reduced motion.
- Keep touch targets large enough for comfortable use.
- Do not hide system status/navigation controls or interfere with emergency/system UI.
- No ads, analytics or network requests in the base launcher.

## Visual restraint

Yellow is a semantic accent, not a fill color for every card. App tiles use graphite surfaces, separators and clear focus indication. Avoid decorative animation that delays application launching.
