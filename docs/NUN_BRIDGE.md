# NUNLauncher ↔ NUN system bridge contract

## Status

**Not implemented.** This document reserves a safe interface boundary. The current Android app remains a local HOME launcher and does not claim a live NUN connection.

## Transport decision

A transport must be chosen only after NUN has a real user-space service and Android-side endpoint. Candidate transports include authenticated local network IPC for a remote NUN host, or a platform-provided companion service when running an NUN Android system image. A plain exported Android Activity, implicit broadcast or unauthenticated local socket is not a privileged control channel.

## Message envelope

A future bridge uses a versioned envelope with request identity, sequence, producer, capability scope, issued-at monotonic context, expiry and bounded payload. It must reject unknown major versions, oversized messages, duplicate request IDs and invalid capability scopes.

## Authorization

- Pairing is explicit and initiated by the user.
- Authentication binds the device identity to an enrolled trust key.
- Read-only status is the first capability.
- Administrative actions are granted individually and expire.
- Destructive operations require a second confirmation describing the target and effect.
- Firmware, boot, reset, power, package and storage operations remain blocked until their dedicated policy and recovery path are implemented.
- No command string or arbitrary script is accepted.

## Failure behavior

The UI must distinguish disconnected, pairing, authenticated, degraded and unavailable states. A timeout or missing heartbeat is not proof that the remote system is healthy or powered off. The bridge never represents an observation as a completed operation.

## Data minimization

Export only the fields needed for the requested action. Never transmit installed-app private data, account credentials, raw storage contents, or unredacted diagnostic paths by default.
