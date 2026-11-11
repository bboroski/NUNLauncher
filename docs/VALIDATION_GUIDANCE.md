# Boot contract diagnostics

`nunl_guidance_for_result()` turns a validator result into stable summary and next-action keys suitable for CLI, installer or operator UI localization. Invalid metadata remains blocking. A successful metadata check is deliberately phrased as permission to continue only to a separately verified boot-chain stage; it does not claim that firmware, image authentication, kernel handoff or hardware boot succeeded.

The function is deterministic and performs no file, memory-map or device I/O.
