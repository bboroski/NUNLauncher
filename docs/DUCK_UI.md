# NUNLauncher Duck UI specification

## Color tokens

| Token | Value | Meaning |
|---|---|---|
| Graphite | #0D0D0D | app background |
| Panel | #1A1A1A | cards and input surfaces |
| Signal yellow | #FFD600 | primary actions, focus and active states |
| Secondary yellow | #BFA100 | secondary emphasis |
| Main text | #E6E6E6 | primary copy |
| Secondary text | #999999 | metadata |
| Divider | #262626 | separators |
| Error | #FF4444 | failure |
| Success | #4CAF50 | positive state |

Yellow is reserved for active or actionable information. Do not flood the screen with yellow panels.

## Interaction

- Touch targets should be at least 48dp where practical.
- App icons retain their original aspect ratio and should not be recolored.
- Search and action buttons expose accessible labels.
- Empty, loading and error states must be distinct.
- State changes must not depend only on color.
- Motion is limited to short, non-blocking transitions and never delays app launch.
- The launcher must remain usable with large font sizes, display scaling and TalkBack.

## Layout

Use adaptive grid cells with a minimum width instead of a fixed column count. Use safe drawing insets and scrollable content. Keep the status header compact and maintain readable labels at tablet and landscape sizes.

## Brand mark

The duck emblem is a small geometric vector asset. It must remain legible at launcher icon sizes, with no photographic texture or tiny decorative details.
