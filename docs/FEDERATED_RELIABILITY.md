# Federated reliability model

NUNLauncher owns the producer-side boot preparation contract. Its current implementation is a host-side metadata validator and specification scaffold, not a functional executable loader.

## Producer boundary

- The launcher side emits a versioned `nun.boot-handoff.v1` description.
- The handoff names an image, its declared size and content digest, an entry point, a bounded memory map and explicit capabilities.
- The contract rejects address overflow and overlapping regions before the consumer interprets metadata.
- The producer and NUN consumer carry mirrored schemas and validators so either side can test compatibility independently.

## Required launcher gates

A real launcher still needs image discovery, architecture-aware loading, memory reservation, signature verification, boot-service interaction and failure-safe transfer of control. None of those functions is claimed by the metadata validator.

## Current verification

```bash
python tools/validate_boot_handoff.py --self-test
python tools/validate_boot_handoff.py spec/boot-handoff.example.json
```

The example is synthetic and does not declare actual CPU support or create a bootable binary.
