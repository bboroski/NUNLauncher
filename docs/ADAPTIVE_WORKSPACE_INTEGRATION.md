# Adaptive Workspace Integration

> Repository: **NUNLauncher**  
> Status: design/integration contract. Described capabilities are not automatically implemented features.

## Project responsibility

NUNLauncher owns the pre-kernel validation and handoff boundary. A future version may validate a versioned workspace/session manifest only as non-privileged metadata and must reject unknown fields that affect security. Kernel image integrity, memory-map bounds, architecture entry points and capability checks take precedence over restoring a visual session. No bootloader or supported CPU target is claimed by this specification.

## Shared system contract

The repository adopts the canonical **Duck Black / Yellow** system. Graphite is the structural base; yellow remains the primary action and focus signal. Project colors may mark context but cannot replace semantic labels, icons or status text.

Living glass, metal and paper are progressive enhancements with opaque fallbacks, GPU budgets and contrast checks. CSS blur is not true physical refraction. Typography uses fluid sizing and a readable long-form measure. Comfortable, calm and compact density modes remain user-overridable. Reduced-motion and high-contrast settings always take precedence.

Window memory, context clipboard, session capsules, semantic search, timeline snapshots and global undo require versioned state and explicit user consent. A restore must show a preview, validate source versions and permissions, then commit transactionally. Motion must not imply success before the underlying operation completes.

Camera, gaze, presence, audio sensing and cross-device transfer are off by default. Do not infer emotion or health from clicks, typing or biometrics. Any optional sensor must be local-first, short-lived, revocable and covered by a visible permission state. Critical security and system-failure notices must never be suppressed by an attention filter.

Autonomous agents receive named capabilities, a resource/time budget, cancellation, a local audit log and human confirmation for destructive or external actions. Preserve the original content before a voice/intent rewrite; show a diff before applying it.

## Versioned workspace event envelope

```json
{
  "schema_version": "1.0",
  "event_id": "stable-event-id",
  "source": "NUNLauncher",
  "sequence": 12,
  "monotonic_ms": 482201,
  "type": "workspace.focus.changed",
  "payload": {
    "workspace_id": "workspace-01",
    "focus_id": "main-surface",
    "density": "comfortable"
  }
}
```

The event contract carries references and state, not credentials, hidden form values, raw biometric samples or privileged tokens. Consumers distinguish `confirmed`, `observed`, `unknown` and `failed`.

## Capability map

| Feature group | Required behavior |
|---|---|
| Materials, adaptive typography, context color, cursor and motion | Use `design/duck-ui.tokens.json` and `assets/duck-ui/duck-ui.css`; preserve static fallback |
| Window memory, clipboard context, session transfer and timeline | Explicit opt-in, versioned manifests, preview and rollback |
| Semantic file mesh and micro-agents | Permission-scoped inputs, provenance, resource budget, cancellation and approval |
| Attention scheduler and adaptive density | Foreground focus and explicit priority first; optional sensors remain off by default |
| Sandbox, attention firewall and privacy shutter | Least privilege, explainable rules, manual bypass and no hidden permission escalation |
| Circadian scenes and audio feedback | User schedule/override, low-power fallback, quiet hours and audio-session-aware ducking |

## Release gates

1. **Accessibility:** keyboard/touch parity, visible focus, 44–48 px targets where appropriate, high contrast and reduced motion.
2. **State integrity:** schema compatibility, bounded storage, stale/corrupt checkpoint handling and rollback fault injection.
3. **Privacy:** deny/revoke permissions, redact secrets, inspect audit events, verify no sensor frames or biometric samples are stored.
4. **Platform evidence:** hardware-dependent support requires target-specific compiler, emulator or device tests.
5. **Truthful maturity:** label every capability `implemented`, `experimental`, `specified`, `hardware_dependent` or `unavailable`.

## Русский / Російський опис

NUNLauncher отвечает за проверку до ядра и границу передачи управления. Будущая версия может проверять версионированный workspace/session manifest только как непривилегированные метаданные и обязана отклонять неизвестные поля, влияющие на безопасность. Целостность образа ядра, границы карты памяти, точки входа архитектуры и проверки возможностей важнее восстановления визуальной сессии. Эта спецификация не означает наличия загрузчика или подтверждённой поддержки CPU.

## Український опис

NUNLauncher відповідає за перевірку до ядра та межу передавання керування. Майбутня версія може перевіряти версіонований workspace/session manifest лише як непривілейовані метадані й має відхиляти невідомі поля, що впливають на безпеку. Цілісність образу ядра, межі карти пам'яті, точки входу архітектури й перевірки можливостей важливіші за відновлення візуальної сесії. Ця специфікація не означає наявності завантажувача або підтвердженої підтримки CPU.

## English summary

NUNLauncher owns the pre-kernel validation and handoff boundary. A future version may validate a versioned workspace/session manifest only as non-privileged metadata and must reject unknown fields that affect security. Kernel image integrity, memory-map bounds, architecture entry points and capability checks take precedence over restoring a visual session. No bootloader or supported CPU target is claimed by this specification.

The repository keeps the shared black/yellow identity, accessible fallbacks, explicit permissions and reversible state transitions. Hardware-dependent behavior is not marked supported until target-specific evidence exists.
