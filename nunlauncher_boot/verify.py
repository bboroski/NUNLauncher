"""End-to-end pre-handoff admission for a NUN hybrid ISO image."""

from __future__ import annotations

from pathlib import Path
from typing import Any

from .binary import inspect_elf64_x86_64, inspect_pe_x86_64
from .manifest import VerificationError, load_manifest, verify_artifact_file
from .media import XorrisoInspector


def verify_image(
    image: Path,
    manifest_path: Path,
    architecture: str = "x86_64",
    require_signature: bool = False,
    inspector: Any | None = None,
) -> dict:
    if architecture != "x86_64":
        raise VerificationError(
            f"architecture {architecture!r} is not supported by this NUN boot profile"
        )
    if require_signature:
        raise VerificationError(
            "signature verification is not configured; unsigned images are rejected when --require-signature is set"
        )

    manifest = load_manifest(manifest_path)
    if manifest["target"]["architecture"] != architecture:
        raise VerificationError("manifest architecture does not match the selected target")

    image = image.resolve()
    digest = verify_artifact_file(image, manifest)
    media = inspector if inspector is not None else XorrisoInspector()

    modes = media.boot_modes(image)
    missing_modes = {"bios", "uefi"} - modes
    if missing_modes:
        raise VerificationError(
            "image is missing required El Torito boot entries: " + ", ".join(mode.upper() for mode in sorted(missing_modes))
        )

    kernel = media.extract(image, manifest["kernel"]["path"])
    entry = inspect_elf64_x86_64(kernel)
    if f"0x{entry:x}" != manifest["kernel"]["entry_point"]:
        raise VerificationError("kernel entry point does not match the image manifest")

    efi_loader = media.extract(image, "EFI/BOOT/BOOTX64.EFI")
    inspect_pe_x86_64(efi_loader)

    return {
        "status": "verified-structure",
        "product": "NUN",
        "image": image.name,
        "sha256": digest,
        "architecture": architecture,
        "firmware": ["bios", "uefi"],
        "boot_protocol": "limine",
        "limine_version": manifest["bootloader"]["version"],
        "kernel_entry_point": f"0x{entry:x}",
        "signature_status": "unsigned",
        "trust_note": "Digest and structural checks passed; publisher authenticity is not established.",
    }
