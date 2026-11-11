"""Strict validation for NUN machine-readable boot-image manifests."""

from __future__ import annotations

import json
import re
from pathlib import Path
from typing import Any


class VerificationError(ValueError):
    """Raised when an image or its metadata must be rejected."""


_SHA256 = re.compile(r"^[a-f0-9]{64}$")
_ENTRY = re.compile(r"^0x[0-9a-f]+$")
_KERNEL_PATH = "boot/NUN/kernel.elf"


def _object(value: Any, label: str) -> dict:
    if not isinstance(value, dict):
        raise VerificationError(f"{label} must be a JSON object")
    return value


def _string(value: Any, label: str) -> str:
    if not isinstance(value, str) or not value:
        raise VerificationError(f"{label} must be a non-empty string")
    return value


def load_manifest(path: Path) -> dict:
    try:
        data = json.loads(path.read_text(encoding="utf-8"))
    except (OSError, UnicodeError, json.JSONDecodeError) as exc:
        raise VerificationError(f"cannot read manifest: {exc}") from exc
    return validate_manifest(data)


def validate_manifest(data: Any) -> dict:
    root = _object(data, "manifest")
    required_root = {
        "schema_version", "product", "artifact", "target",
        "bootloader", "kernel", "security", "build",
    }
    if set(root) != required_root:
        raise VerificationError("manifest fields do not match schema version 1")
    if root["schema_version"] != 1 or root["product"] != "NUN":
        raise VerificationError("unsupported manifest schema or product")

    artifact = _object(root["artifact"], "artifact")
    if set(artifact) != {"file", "format", "size_bytes", "sha256"}:
        raise VerificationError("invalid artifact fields")
    _string(artifact["file"], "artifact.file")
    if artifact["format"] != "ISO9660-hybrid":
        raise VerificationError("unsupported image format")
    if not isinstance(artifact["size_bytes"], int) or isinstance(artifact["size_bytes"], bool) or artifact["size_bytes"] <= 0:
        raise VerificationError("artifact.size_bytes must be a positive integer")
    if not isinstance(artifact["sha256"], str) or not _SHA256.fullmatch(artifact["sha256"]):
        raise VerificationError("artifact.sha256 must be a lowercase SHA-256 digest")

    target = _object(root["target"], "target")
    if set(target) != {"architecture", "firmware", "boot_protocol", "limine_base_revision"}:
        raise VerificationError("invalid target fields")
    if target["architecture"] != "x86_64":
        raise VerificationError(f"unsupported architecture: {target['architecture']!r}")
    if target["boot_protocol"] != "limine" or target["limine_base_revision"] != 6:
        raise VerificationError("unsupported boot protocol or Limine base revision")
    if not isinstance(target["firmware"], list) or not {"bios", "uefi"}.issubset(target["firmware"]):
        raise VerificationError("image must declare both BIOS and UEFI firmware paths")
    if len(target["firmware"]) != len(set(target["firmware"])):
        raise VerificationError("duplicate firmware declarations")

    bootloader = _object(root["bootloader"], "bootloader")
    if set(bootloader) != {"name", "version", "archive_sha256"}:
        raise VerificationError("invalid bootloader fields")
    if bootloader["name"] != "Limine":
        raise VerificationError("unsupported bootloader")
    _string(bootloader["version"], "bootloader.version")
    if not isinstance(bootloader["archive_sha256"], str) or not _SHA256.fullmatch(bootloader["archive_sha256"]):
        raise VerificationError("bootloader.archive_sha256 must be a SHA-256 digest")

    kernel = _object(root["kernel"], "kernel")
    if set(kernel) != {"path", "format", "endianness", "machine", "type", "entry_point"}:
        raise VerificationError("invalid kernel fields")
    if kernel["path"] != _KERNEL_PATH:
        raise VerificationError("unexpected kernel path")
    if (kernel["format"], kernel["endianness"], kernel["machine"], kernel["type"]) != (
        "ELF64", "little", "x86_64", "ET_DYN"
    ):
        raise VerificationError("unsupported kernel binary contract")
    if not isinstance(kernel["entry_point"], str) or not _ENTRY.fullmatch(kernel["entry_point"]):
        raise VerificationError("kernel.entry_point must be a hexadecimal address")

    security = _object(root["security"], "security")
    if set(security) != {"digest_algorithm", "signature_status"}:
        raise VerificationError("invalid security fields")
    if security["digest_algorithm"] != "sha256":
        raise VerificationError("unsupported image digest algorithm")
    if security["signature_status"] != "unsigned":
        raise VerificationError("signature state is not supported by the configured trust backend")

    build = _object(root["build"], "build")
    if set(build) != {"source_commit"}:
        raise VerificationError("invalid build provenance fields")
    _string(build["source_commit"], "build.source_commit")
    return root


def verify_artifact_file(image: Path, manifest: dict) -> str:
    import hashlib

    image = image.resolve()
    if not image.is_file():
        raise VerificationError(f"image does not exist or is not a regular file: {image}")
    artifact = manifest["artifact"]
    if image.name != artifact["file"]:
        raise VerificationError("image filename does not match the manifest")
    size = image.stat().st_size
    if size != artifact["size_bytes"]:
        raise VerificationError(f"image size mismatch: manifest={artifact['size_bytes']} actual={size}")
    digest = hashlib.sha256()
    with image.open("rb") as stream:
        for chunk in iter(lambda: stream.read(1024 * 1024), b""):
            digest.update(chunk)
    actual = digest.hexdigest()
    if actual != artifact["sha256"]:
        raise VerificationError("image SHA-256 mismatch")
    return actual
