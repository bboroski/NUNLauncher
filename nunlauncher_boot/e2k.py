"""Admission checks for NUN's E2K Linux userspace runtime."""

from __future__ import annotations

import hashlib
import json
import struct
from pathlib import Path
from typing import Any

from .manifest import VerificationError


def _read_document(path: Path) -> dict:
    try:
        data = json.loads(path.read_text(encoding="utf-8"))
    except (OSError, UnicodeError, json.JSONDecodeError) as exc:
        raise VerificationError(f"cannot read E2K runtime manifest: {exc}") from exc
    if not isinstance(data, dict):
        raise VerificationError("E2K runtime manifest must be a JSON object")
    return data


def _sha256(path: Path) -> str:
    digest = hashlib.sha256()
    with path.open("rb") as stream:
        for chunk in iter(lambda: stream.read(1024 * 1024), b""):
            digest.update(chunk)
    return digest.hexdigest()


def _inspect_e2k_elf(path: Path) -> dict[str, Any]:
    try:
        data = path.read_bytes()
    except OSError as exc:
        raise VerificationError(f"cannot read E2K binary: {exc}") from exc

    if len(data) < 64 or data[:4] != b"\x7fELF":
        raise VerificationError("E2K runtime is not a complete ELF image")
    if data[4] != 2 or data[5] != 1 or data[6] != 1:
        raise VerificationError("E2K runtime must be ELF64 little-endian, version 1")

    e_type, machine, version, entry, phoff, _shoff, _flags, ehsize, phentsize, phnum, *_ = struct.unpack_from(
        "<HHIQQQIHHHHHH", data, 16
    )
    if machine != 175:
        raise VerificationError(
            f"ELF e_machine is {machine}; expected EM_MCST_ELBRUS (175)"
        )
    if version != 1 or e_type not in (2, 3):
        raise VerificationError("E2K ELF must be ET_EXEC or ET_DYN, version 1")
    if ehsize < 64 or phentsize < 56 or phnum == 0:
        raise VerificationError("E2K ELF program-header table is invalid")
    if phoff > len(data) or phnum > (len(data) - phoff) // phentsize:
        raise VerificationError("E2K ELF program-header table is out of bounds")
    if entry == 0:
        raise VerificationError("E2K ELF entry point must not be zero")

    executable_entry = False
    for index in range(phnum):
        offset = phoff + index * phentsize
        p_type, p_flags, _p_offset, p_vaddr, _p_paddr, _p_filesz, p_memsz, _p_align = struct.unpack_from(
            "<IIQQQQQQ", data, offset
        )
        if p_type != 1 or not (p_flags & 1) or p_memsz == 0:
            continue
        end = p_vaddr + p_memsz
        if end > (1 << 64):
            raise VerificationError("E2K ELF load segment overflows the address space")
        if p_vaddr <= entry < end:
            executable_entry = True
            break
    if not executable_entry:
        raise VerificationError("E2K entry point is outside an executable PT_LOAD segment")

    return {
        "format": "ELF64",
        "endianness": "little",
        "machine": "EM_MCST_ELBRUS",
        "e_machine": machine,
        "type": "ET_EXEC" if e_type == 2 else "ET_DYN",
        "entry_point": f"0x{entry:x}",
    }


def verify_e2k_runtime(binary: Path, manifest_path: Path, *, require_signature: bool = False) -> dict:
    """Verify NUN's native E2K Linux userspace binary without claiming it booted."""
    if require_signature:
        raise VerificationError(
            "E2K publisher signature verification is not configured; refusing the signature-required request"
        )

    binary = binary.resolve()
    if not binary.is_file():
        raise VerificationError(f"E2K runtime binary is missing: {binary}")
    manifest = _read_document(manifest_path)

    expected_root = {
        "schema_version", "product", "artifact", "target",
        "binary", "compiler", "security", "build",
    }
    if set(manifest) != expected_root:
        raise VerificationError("E2K manifest fields do not match schema nun.e2k.runtime.v1")
    if manifest["schema_version"] != "nun.e2k.runtime.v1" or manifest["product"] != "NUN":
        raise VerificationError("unsupported E2K runtime manifest version or product")

    artifact = manifest["artifact"]
    if not isinstance(artifact, dict) or set(artifact) != {"file", "size_bytes", "sha256"}:
        raise VerificationError("invalid E2K artifact fields")
    if artifact["file"] != binary.name:
        raise VerificationError("E2K binary name does not match the manifest")
    if not isinstance(artifact["size_bytes"], int) or isinstance(artifact["size_bytes"], bool):
        raise VerificationError("E2K artifact size must be an integer")
    if artifact["size_bytes"] <= 0 or artifact["size_bytes"] != binary.stat().st_size:
        raise VerificationError("E2K artifact size mismatch")
    if not isinstance(artifact["sha256"], str) or len(artifact["sha256"]) != 64:
        raise VerificationError("E2K artifact SHA-256 is invalid")
    actual_digest = _sha256(binary)
    if actual_digest != artifact["sha256"]:
        raise VerificationError("E2K binary SHA-256 mismatch")

    target = manifest["target"]
    expected_target = {
        "architecture": "e2k",
        "os": "linux",
        "word_bits": 64,
        "endianness": "little",
        "elf_machine": 175,
        "execution_mode": "native-linux-userspace",
        "deployment_chain": "MCST-PNS-to-Elbrus-Linux-to-NUN-userspace",
    }
    if target != expected_target:
        raise VerificationError("E2K target profile does not match the supported NUN userspace contract")

    binary_info = _inspect_e2k_elf(binary)
    if manifest["binary"] != binary_info:
        raise VerificationError("E2K ELF metadata does not match the manifest")

    compiler = manifest["compiler"]
    if not isinstance(compiler, dict) or set(compiler) != {"family", "version"}:
        raise VerificationError("invalid E2K compiler metadata")
    if compiler["family"] != "LCC" or not isinstance(compiler["version"], str) or not compiler["version"]:
        raise VerificationError("E2K runtime must declare the MCST LCC compiler and version")

    security = manifest["security"]
    if security != {"digest_algorithm": "sha256", "signature_status": "unsigned"}:
        raise VerificationError("unsupported E2K signature or digest policy")
    build = manifest["build"]
    if not isinstance(build, dict) or set(build) != {"source_commit"} or not isinstance(build["source_commit"], str):
        raise VerificationError("invalid E2K build provenance")

    return {
        "status": "verified-e2k-elf-structure",
        "product": "NUN",
        "binary": binary.name,
        "sha256": actual_digest,
        "architecture": "e2k",
        "elf_machine": 175,
        "operating_system": "linux",
        "execution_mode": "native-linux-userspace",
        "compiler": "LCC",
        "compiler_version": compiler["version"],
        "entry_point": binary_info["entry_point"],
        "signature_status": "unsigned",
        "boot_status": "not-tested-by-preflight",
        "trust_note": "ELF structure and digest passed; publisher authenticity and physical hardware execution are not established.",
    }
