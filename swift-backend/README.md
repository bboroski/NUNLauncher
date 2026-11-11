# NUNLauncher management plane — Server-Side Swift

Swift 6 / Vapor management service kept separate from the existing system implementation. Run: `cd swift-backend && swift test && swift run nunlauncher-server`.

Configuration: `HOST`, `PORT`, `SERVICE_NAME`, `SERVICE_VERSION`. Credentials and signing keys must come from an external secret provider, never source code.

Endpoints: `GET /health`, `GET /ready`, `GET /v1/health`. Readiness checks process state only; dependency probes remain a production gate. Responses propagate `X-Trace-ID`.

**Critical boundary:** this service manages launcher operations only. It must not run in the pre-boot environment or be a dependency of the bootloader. Boot-time validation remains local and fail-safe; remote management is advisory and cannot bypass signature verification.

Core contracts include immutable Sendable request context, validated Codable configuration, API error DTO, async repository boundary and trace/security-header middleware. Never block SwiftNIO event loops; use parameterized queries and bounded pools.

PostgreSQL/Redis, JWT/RBAC, TLS, OpenTelemetry, Prometheus, migrations, rate limiting, OpenAPI, load testing and Kubernetes are not yet claimed as production-ready.
