# NUNLauncher boot image contract — implementation stage 0

## Scope and maturity

NUNLauncher now contains a portable C11 **host-side validator** for a versioned image header and a caller-supplied physical-memory map. This is the first executable contract layer, not a firmware bootloader.

The repository still has no firmware entry point, architecture handoff assembly, linker script, device drivers, signed-image trust chain, emulator boot test or bootable binary. No hardware target is marked qualified by the host tests.

## Version 1 image header

The header is exactly 48 bytes, little-endian, with these fixed offsets:

| Offset | Width | Field | Rule |
|---:|---:|---|---|
| 0 | 8 | Magic | `NUNIMG1\0` |
| 8 | 4 | Version | 1 |
| 12 | 4 | Header size | 48 |
| 16 | 8 | Image size | At least 48 and no larger than the supplied buffer |
| 24 | 8 | Entry offset | Inside the image and after the header |
| 32 | 4 | Architecture | Exact match with the selected target |
| 36 | 4 | Flags | Zero until a versioned flag is specified |
| 40 | 4 | Header CRC-32 | CRC-32/ISO-HDLC with this field treated as zero |
| 44 | 4 | Reserved | Zero |

The CRC detects accidental header corruption only. It is not a cryptographic signature, does not authenticate the author and does not protect image payload bytes.

## Validation rules

- Reject unknown magic, version, header size, architecture, flags and reserved bits.
- Reject short buffers, impossible sizes, entry offsets outside the image and integer overflow.
- Require a page-aligned load address.
- Accept the image only when its complete load span is contained in a caller-declared usable-RAM range.
- Bound the memory map to 32 entries and reject zero-length or overflowing ranges.
- Return the computed entry address only after every check passes.

The validator allocates no memory and performs no I/O, mapping, copying, privilege changes or control transfer. The caller remains responsible for obtaining trustworthy firmware memory-map data and for preserving reserved regions.

## Build and test

```sh
cmake -S . -B build -DBUILD_TESTING=ON
cmake --build build --parallel
ctest --test-dir build --output-on-failure
```

The host suite covers valid headers, architecture mismatch, unsupported version, checksum mismatch, invalid entry, alignment and memory-range containment. CI builds with GCC and Clang. The manual E2K job requires a self-hosted Linux E2K runner with LCC installed.

## Next gates

1. Freeze the serialized image format and publish golden test vectors.
2. Add cryptographic signature verification with a separately reviewed trust-root and key-rotation design.
3. Define architecture-specific handoff structures and linker scripts.
4. Add emulator boot smoke tests for each target separately.
5. Add hardware-in-the-loop evidence before claiming a platform is supported.
