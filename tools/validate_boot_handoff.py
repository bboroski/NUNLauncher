#!/usr/bin/env python3
"""Validate NUN's versioned boot-handoff contract using only the Python standard library.

The digest detects accidental or unauthorized content changes only when the
expected digest is trusted separately. It is not a digital signature and does
not authenticate a bootloader, kernel image, firmware, or physical memory map.
"""
from __future__ import annotations

import argparse
import copy
import hashlib
import hmac
import json
from pathlib import Path, PurePosixPath
import re
import sys
from typing import Any

MAX_U64 = (1 << 64) - 1
MAX_DOCUMENT_BYTES = 1_048_576
_HEX_256 = re.compile(r"^[0-9a-fA-F]{64}$")
_KINDS = {"available", "reserved", "firmware", "kernel", "initrd", "mmio"}


class HandoffError(ValueError):
    """Raised when a handoff record violates the contract."""


def _no_duplicate_keys(pairs: list[tuple[str, Any]]) -> dict[str, Any]:
    result: dict[str, Any] = {}
    for key, value in pairs:
        if key in result:
            raise HandoffError(f"duplicate JSON object key: {key}")
        result[key] = value
    return result


def _canonical_bytes(document: dict[str, Any]) -> bytes:
    payload = {key: value for key, value in document.items() if key != "digest_sha256"}
    try:
        return json.dumps(
            payload, ensure_ascii=False, sort_keys=True,
            separators=(",", ":"), allow_nan=False,
        ).encode("utf-8")
    except (TypeError, ValueError, RecursionError) as exc:
        raise HandoffError("handoff must contain finite JSON-compatible values") from exc


def calculate_digest(document: dict[str, Any]) -> str:
    """Calculate the canonical SHA-256 digest without the digest field itself."""
    if not isinstance(document, dict):
        raise HandoffError("handoff root must be an object")
    return hashlib.sha256(_canonical_bytes(document)).hexdigest()


def _uint64(value: Any, field: str, *, positive: bool = False) -> int:
    if type(value) is not int:
        raise HandoffError(f"{field} must be an integer (boolean values are not accepted)")
    lower = 1 if positive else 0
    if not lower <= value <= MAX_U64:
        raise HandoffError(f"{field} must be in the range {lower}..2^64-1")
    return value


def _non_empty(value: Any, field: str, limit: int) -> str:
    if not isinstance(value, str) or not value.strip() or len(value) > limit:
        raise HandoffError(f"{field} must be a non-empty string of at most {limit} characters")
    return value


def _validate_image_path(value: Any) -> str:
    path = _non_empty(value, "kernel_image.path", 1024)
    parsed = PurePosixPath(path)
    if (
        parsed.is_absolute()
        or "\\" in path
        or any(part in {"", ".", ".."} for part in path.split("/"))
        or path.startswith("//")
    ):
        raise HandoffError("kernel_image.path must be a normalized relative POSIX path")
    return path


def validate_handoff(document: Any, *, verify_digest: bool = True) -> dict[str, Any]:
    """Validate shape, arithmetic, map overlap, entry point and content digest."""
    if not isinstance(document, dict):
        raise HandoffError("handoff root must be an object")

    allowed = {
        "schema_version", "boot_id", "architecture", "kernel_image",
        "entry_point", "memory_map", "capabilities", "digest_sha256",
    }
    extra = sorted(set(document) - allowed)
    missing = sorted(allowed - set(document))
    if extra:
        raise HandoffError(f"unknown top-level fields: {', '.join(extra)}")
    if missing:
        raise HandoffError(f"missing top-level fields: {', '.join(missing)}")
    if document["schema_version"] != "nun.boot-handoff.v1":
        raise HandoffError("schema_version must be 'nun.boot-handoff.v1'")

    _non_empty(document["boot_id"], "boot_id", 128)
    _non_empty(document["architecture"], "architecture", 64)

    image = document["kernel_image"]
    if not isinstance(image, dict) or set(image) != {"path", "size_bytes", "sha256"}:
        raise HandoffError("kernel_image must contain exactly path, size_bytes, and sha256")
    image_path = _validate_image_path(image["path"])
    image_size = _uint64(image["size_bytes"], "kernel_image.size_bytes", positive=True)
    image_hash = image["sha256"]
    if not isinstance(image_hash, str) or not _HEX_256.fullmatch(image_hash):
        raise HandoffError("kernel_image.sha256 must be a 64-character hexadecimal digest")

    entry_point = _uint64(document["entry_point"], "entry_point")
    memory_map = document["memory_map"]
    if not isinstance(memory_map, list) or not 1 <= len(memory_map) <= 256:
        raise HandoffError("memory_map must contain between 1 and 256 regions")

    regions: list[tuple[int, int, str]] = []
    for index, raw in enumerate(memory_map):
        field = f"memory_map[{index}]"
        if not isinstance(raw, dict) or set(raw) != {"base", "length", "kind"}:
            raise HandoffError(f"{field} must contain exactly base, length, and kind")
        base = _uint64(raw["base"], f"{field}.base")
        length = _uint64(raw["length"], f"{field}.length", positive=True)
        if base + length > MAX_U64 + 1:
            raise HandoffError(f"{field} address range overflows 64-bit address space")
        kind = raw["kind"]
        if kind not in _KINDS:
            raise HandoffError(f"{field}.kind is not a supported memory-map category")
        regions.append((base, base + length, kind))

    regions.sort(key=lambda item: (item[0], item[1], item[2]))
    for previous, current in zip(regions, regions[1:]):
        if current[0] < previous[1]:
            raise HandoffError("memory_map regions overlap")

    kernel_regions = [item for item in regions if item[2] == "kernel"]
    if len(kernel_regions) != 1:
        raise HandoffError("memory_map must contain exactly one kernel region")
    kernel_start, kernel_end, _ = kernel_regions[0]
    if not kernel_start <= entry_point < kernel_end:
        raise HandoffError("entry_point must fall inside the kernel memory region")
    if image_size > kernel_end - kernel_start:
        raise HandoffError("kernel_image.size_bytes exceeds the declared kernel region")

    capabilities = document["capabilities"]
    if not isinstance(capabilities, list) or len(capabilities) > 64:
        raise HandoffError("capabilities must be an array with at most 64 entries")
    for index, capability in enumerate(capabilities):
        _non_empty(capability, f"capabilities[{index}]", 128)
    if len(set(capabilities)) != len(capabilities):
        raise HandoffError("capabilities contains duplicates")

    supplied_digest = document["digest_sha256"]
    if not isinstance(supplied_digest, str) or not re.fullmatch(r"[0-9a-f]{64}", supplied_digest):
        raise HandoffError("digest_sha256 must be a lowercase 64-character SHA-256 digest")
    calculated = calculate_digest(document)
    if verify_digest and not hmac.compare_digest(supplied_digest, calculated):
        raise HandoffError("digest_sha256 does not match the canonical handoff content")

    return {
        "boot_id": document["boot_id"],
        "architecture": document["architecture"],
        "kernel_image": image_path,
        "kernel_image_size_bytes": image_size,
        "entry_point": entry_point,
        "memory_region_count": len(regions),
        "capability_count": len(capabilities),
        "digest_sha256": calculated,
        "digest_verified": bool(verify_digest),
        "authentication": "not_provided",
    }


def _fixture() -> dict[str, Any]:
    value: dict[str, Any] = {
        "schema_version": "nun.boot-handoff.v1",
        "boot_id": "example-boot-001",
        "architecture": "x86_64",
        "kernel_image": {
            "path": "artifacts/kernel.bin",
            "size_bytes": 4096,
            "sha256": "a" * 64,
        },
        "entry_point": 1048576,
        "memory_map": [
            {"base": 0, "length": 1048576, "kind": "firmware"},
            {"base": 1048576, "length": 2097152, "kind": "kernel"},
            {"base": 3145728, "length": 4194304, "kind": "available"},
        ],
        "capabilities": ["memory-map.v1", "serial-console.v1"],
    }
    value["digest_sha256"] = calculate_digest(value)
    return value


def self_test() -> int:
    good = _fixture()
    validate_handoff(good)
    bad_overlap = copy.deepcopy(good)
    bad_overlap["memory_map"][2]["base"] = 2_000_000
    bad_overlap["digest_sha256"] = calculate_digest(bad_overlap)
    try:
        validate_handoff(bad_overlap)
    except HandoffError:
        pass
    else:
        raise AssertionError("overlapping memory regions were accepted")

    bad_entry = copy.deepcopy(good)
    bad_entry["entry_point"] = 9_999_999
    bad_entry["digest_sha256"] = calculate_digest(bad_entry)
    try:
        validate_handoff(bad_entry)
    except HandoffError:
        pass
    else:
        raise AssertionError("entry point outside kernel region was accepted")

    bad_digest = copy.deepcopy(good)
    bad_digest["boot_id"] = "tampered"
    try:
        validate_handoff(bad_digest)
    except HandoffError:
        pass
    else:
        raise AssertionError("digest mismatch was accepted")

    bad_path = copy.deepcopy(good)
    bad_path["kernel_image"]["path"] = "../kernel.bin"
    bad_path["digest_sha256"] = calculate_digest(bad_path)
    try:
        validate_handoff(bad_path)
    except HandoffError:
        pass
    else:
        raise AssertionError("path traversal was accepted")

    print("boot handoff self-test: PASS (4 rejection cases + valid fixture)")
    return 0


def main(argv: list[str] | None = None) -> int:
    parser = argparse.ArgumentParser(description="Validate the NUN boot-handoff contract")
    parser.add_argument("document", nargs="?", help="path to a JSON boot-handoff record")
    parser.add_argument("--digest-only", action="store_true",
                        help="print the canonical SHA-256 digest without claiming verification")
    parser.add_argument("--self-test", action="store_true",
                        help="run offline contract validation checks")
    args = parser.parse_args(argv)

    if args.self_test:
        return self_test()
    if not args.document:
        parser.error("document is required unless --self-test is used")

    try:
        path = Path(args.document)
        with path.open("rb") as handle:
            raw = handle.read(MAX_DOCUMENT_BYTES + 1)
        if len(raw) > MAX_DOCUMENT_BYTES:
            raise HandoffError(f"document exceeds the {MAX_DOCUMENT_BYTES}-byte limit")
        try:
            document = json.loads(raw.decode("utf-8"), object_pairs_hook=_no_duplicate_keys)
        except UnicodeDecodeError as exc:
            raise HandoffError("document must be UTF-8") from exc
        except json.JSONDecodeError as exc:
            raise HandoffError(
                f"invalid JSON at line {exc.lineno}, column {exc.colno}: {exc.msg}"
            ) from exc

        if args.digest_only:
            print(calculate_digest(document))
            return 0
        report = validate_handoff(document)
        print(json.dumps(report, ensure_ascii=False, indent=2, sort_keys=True))
        print("Note: digest integrity is not a digital signature or boot authorization.")
        return 0
    except (OSError, HandoffError) as exc:
        print(f"boot-handoff: error: {exc}", file=sys.stderr)
        return 2


if __name__ == "__main__":
    raise SystemExit(main())
