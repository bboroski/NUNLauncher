#!/usr/bin/env python3
"""Validate image metadata and boot handoff constraints without loading an image."""
from __future__ import annotations

import argparse
import json
import re
from pathlib import Path
from typing import Any


def _integer(value: Any) -> bool:
    return isinstance(value, int) and not isinstance(value, bool)


def validate_profile(document: Any) -> list[str]:
    errors: list[str] = []
    if not isinstance(document, dict):
        return ["profile root must be an object"]
    if document.get("schema_version") != "nun.boot-domain-profile.v1":
        errors.append("unsupported schema_version")
    for field in ("profile_id", "architecture"):
        if not isinstance(document.get(field), str) or not document[field].strip():
            errors.append(f"{field} must be a non-empty string")

    abi = document.get("abi")
    if not isinstance(abi, dict) or not isinstance(abi.get("family"), str):
        errors.append("abi.family must be declared")
    elif not _integer(abi.get("major")) or not _integer(abi.get("minor")):
        errors.append("abi major/minor must be non-negative integers")
    elif abi["major"] < 0 or abi["minor"] < 0:
        errors.append("abi major/minor must be non-negative integers")

    artifact = document.get("artifact")
    if not isinstance(artifact, dict):
        errors.append("artifact must be an object")
    else:
        if not isinstance(artifact.get("format"), str) or not artifact["format"].strip():
            errors.append("artifact.format must be non-empty")
        if not _integer(artifact.get("size_bytes")) or artifact["size_bytes"] <= 0:
            errors.append("artifact.size_bytes must be a positive integer")
        digest = artifact.get("sha256")
        if not isinstance(digest, str) or re.fullmatch(r"[0-9a-f]{64}", digest) is None:
            errors.append("artifact.sha256 must be 64 lowercase hexadecimal characters")

    regions = document.get("memory_regions")
    entry_point = document.get("entry_point")
    if not _integer(entry_point) or entry_point < 0:
        errors.append("entry_point must be a non-negative integer")
    if not isinstance(regions, list) or not regions:
        errors.append("memory_regions must be a non-empty array")
        regions = []
    parsed: list[tuple[int, int, dict[str, Any]]] = []
    for index, region in enumerate(regions):
        prefix = f"memory_regions[{index}]"
        if not isinstance(region, dict):
            errors.append(f"{prefix} must be an object")
            continue
        start, size, alignment = region.get("start"), region.get("size"), region.get("alignment")
        if not _integer(start) or start < 0 or not _integer(size) or size <= 0:
            errors.append(f"{prefix} start/size are invalid")
            continue
        if not _integer(alignment) or alignment <= 0 or alignment & (alignment - 1):
            errors.append(f"{prefix}.alignment must be a positive power of two")
        elif start % alignment:
            errors.append(f"{prefix}.start is not aligned")
        permissions = region.get("permissions")
        if not isinstance(permissions, list) or any(
            not isinstance(item, str) or item not in ("read", "write", "execute")
            for item in permissions
        ):
            errors.append(f"{prefix}.permissions is invalid")
        if not isinstance(region.get("name"), str) or not region["name"].strip():
            errors.append(f"{prefix}.name must be non-empty")
        end = start + size
        if end <= start:
            errors.append(f"{prefix} address range overflows")
        else:
            parsed.append((start, end, region))

    parsed.sort(key=lambda item: item[0])
    for previous, current in zip(parsed, parsed[1:]):
        if current[0] < previous[1]:
            errors.append(
                f"memory regions overlap: {previous[2].get('name')} and {current[2].get('name')}"
            )

    if _integer(entry_point) and entry_point >= 0:
        executable = any(
            start <= entry_point < end and "execute" in region.get("permissions", [])
            for start, end, region in parsed
        )
        if not executable:
            errors.append("entry_point must lie inside an executable memory region")

    capabilities = document.get("required_capabilities")
    if not isinstance(capabilities, list) or any(
        not isinstance(item, str) or not item.strip() for item in capabilities
    ):
        errors.append("required_capabilities must be a string array")

    recovery = document.get("recovery")
    if not isinstance(recovery, dict):
        errors.append("recovery must be an object")
    else:
        attempts = recovery.get("max_attempts")
        delay = recovery.get("retry_delay_ms")
        if not _integer(attempts) or not 1 <= attempts <= 5:
            errors.append("recovery.max_attempts must be between 1 and 5")
        if not _integer(delay) or not 0 <= delay <= 120000:
            errors.append("recovery.retry_delay_ms must be between 0 and 120000")
        if recovery.get("rollback_enabled") is not True:
            errors.append("recovery.rollback_enabled must be true")
        fallback = recovery.get("fallback_order")
        if not isinstance(fallback, list) or any(
            not isinstance(item, str) or not item.strip() for item in fallback
        ):
            errors.append("recovery.fallback_order must contain non-empty strings")
        elif len(set(fallback)) != len(fallback):
            errors.append("recovery.fallback_order must contain unique strings")
    return errors


def main() -> int:
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument(
        "profile", nargs="?",
        default=str(Path(__file__).resolve().parents[1] / "spec" / "domain-boot-profile.json"),
    )
    args = parser.parse_args()
    try:
        document = json.loads(Path(args.profile).read_text(encoding="utf-8"))
    except (OSError, json.JSONDecodeError) as exc:
        print(f"ERROR: {exc}")
        return 2
    errors = validate_profile(document)
    if errors:
        for error in errors:
            print(f"ERROR: {error}")
        return 1
    print("NUNLauncher boot profile: VALID (example metadata only)")
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
