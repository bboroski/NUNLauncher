#!/usr/bin/env python3
"""Validate a NUN kernel-image manifest and its referenced binary.

This host-side checker does not load, relocate, authenticate, or execute a kernel.
It is a pre-handoff integrity and bounds check only.
"""

from __future__ import annotations

import argparse
import hashlib
import json
import re
import sys
from pathlib import Path
from typing import Any

FORMAT = "nun.kernel-image"
MANIFEST_VERSION = 1
SUPPORTED_FORMAT_TARGETS = {"x86_64", "aarch64", "armv7", "riscv32", "riscv64", "e2k"}
EXPERIMENTAL_TARGETS = {"e2k"}
UINT64_MAX = (1 << 64) - 1
SHA256_RE = re.compile(r"^[0-9a-fA-F]{64}$")


class ManifestError(ValueError):
    """Raised when a boot-image manifest or its payload is invalid."""


def _integer(value: Any, field: str, *, minimum: int = 0) -> int:
    if isinstance(value, bool) or not isinstance(value, int):
        raise ManifestError(f"{field} must be an integer")
    if value < minimum or value > UINT64_MAX:
        raise ManifestError(f"{field} is outside the permitted unsigned 64-bit range")
    return value


def validate_manifest(
    manifest_path: Path,
    *,
    expected_architecture: str | None = None,
    allow_experimental_target: bool = False,
) -> dict[str, Any]:
    try:
        manifest_file = manifest_path.resolve(strict=True)
        raw = json.loads(manifest_file.read_text(encoding="utf-8"))
    except FileNotFoundError as exc:
        raise ManifestError(f"manifest not found: {manifest_path}") from exc
    except (OSError, UnicodeError, json.JSONDecodeError) as exc:
        raise ManifestError(f"cannot read manifest: {exc}") from exc

    if not isinstance(raw, dict):
        raise ManifestError("manifest root must be a JSON object")
    if raw.get("format") != FORMAT:
        raise ManifestError(f"format must be {FORMAT!r}")
    if raw.get("version") != MANIFEST_VERSION:
        raise ManifestError(f"version must be {MANIFEST_VERSION}")

    architecture = raw.get("architecture")
    if not isinstance(architecture, str) or architecture not in SUPPORTED_FORMAT_TARGETS:
        raise ManifestError("architecture must be one of: " + ", ".join(sorted(SUPPORTED_FORMAT_TARGETS)))
    if architecture in EXPERIMENTAL_TARGETS and not allow_experimental_target:
        raise ManifestError(
            f"{architecture} is an experimental manifest target; "
            "pass --allow-experimental-target to validate its metadata"
        )
    if expected_architecture and architecture != expected_architecture:
        raise ManifestError(
            f"manifest architecture {architecture!r} does not match "
            f"requested target {expected_architecture!r}"
        )

    image_name = raw.get("image")
    if not isinstance(image_name, str) or not image_name.strip():
        raise ManifestError("image must be a non-empty relative path")
    image_relative = Path(image_name)
    if image_relative.is_absolute() or ".." in image_relative.parts:
        raise ManifestError("image path must stay within the manifest directory")

    base_dir = manifest_file.parent.resolve()
    try:
        image_path = (base_dir / image_relative).resolve(strict=True)
    except FileNotFoundError as exc:
        raise ManifestError(f"image file not found: {image_name}") from exc
    if not image_path.is_relative_to(base_dir):
        raise ManifestError("image path resolves outside the manifest directory")
    if not image_path.is_file():
        raise ManifestError("image must refer to a regular file")

    expected_size = _integer(raw.get("image_size"), "image_size", minimum=1)
    actual_size = image_path.stat().st_size
    if actual_size != expected_size:
        raise ManifestError(f"image_size mismatch: manifest={expected_size}, actual={actual_size}")

    expected_digest = raw.get("sha256")
    if not isinstance(expected_digest, str) or not SHA256_RE.fullmatch(expected_digest):
        raise ManifestError("sha256 must contain exactly 64 hexadecimal characters")
    digest = hashlib.sha256()
    try:
        with image_path.open("rb") as handle:
            for chunk in iter(lambda: handle.read(1024 * 1024), b""):
                digest.update(chunk)
    except OSError as exc:
        raise ManifestError(f"cannot read image: {exc}") from exc
    actual_digest = digest.hexdigest()
    if actual_digest.lower() != expected_digest.lower():
        raise ManifestError("sha256 digest mismatch")

    load_address = _integer(raw.get("load_address"), "load_address")
    entry_address = _integer(raw.get("entry_address"), "entry_address")
    memory_size = _integer(raw.get("memory_size"), "memory_size", minimum=1)
    if memory_size < actual_size:
        raise ManifestError("memory_size cannot be smaller than the image")
    if load_address > UINT64_MAX - memory_size:
        raise ManifestError("load_address + memory_size overflows unsigned 64-bit address space")
    end_address = load_address + memory_size
    if not load_address <= entry_address < end_address:
        raise ManifestError("entry_address must fall inside the declared load region")

    alignment = _integer(raw.get("entry_alignment", 1), "entry_alignment", minimum=1)
    if alignment & (alignment - 1):
        raise ManifestError("entry_alignment must be a power of two")
    if entry_address % alignment:
        raise ManifestError("entry_address does not satisfy entry_alignment")

    return {
        "architecture": architecture,
        "image": image_path,
        "image_size": actual_size,
        "sha256": actual_digest,
        "load_address": load_address,
        "entry_address": entry_address,
        "memory_size": memory_size,
        "experimental": architecture in EXPERIMENTAL_TARGETS,
    }


def main(argv: list[str] | None = None) -> int:
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("manifest", type=Path, help="path to a nun.kernel-image JSON manifest")
    parser.add_argument("--target", choices=sorted(SUPPORTED_FORMAT_TARGETS))
    parser.add_argument(
        "--allow-experimental-target",
        action="store_true",
        help="permit metadata validation for an experimental target; does not qualify hardware",
    )
    args = parser.parse_args(argv)

    try:
        result = validate_manifest(
            args.manifest,
            expected_architecture=args.target,
            allow_experimental_target=args.allow_experimental_target,
        )
    except ManifestError as exc:
        print(f"INVALID: {exc}", file=sys.stderr)
        return 2

    state = "EXPERIMENTAL METADATA ONLY" if result["experimental"] else "MANIFEST VALID"
    print(
        f"{state}: {result['architecture']} | {result['image_size']} bytes | "
        f"sha256={result['sha256']} | entry=0x{result['entry_address']:X}"
    )
    print("No code was loaded or executed; bootloader and target qualification remain separate.")
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
