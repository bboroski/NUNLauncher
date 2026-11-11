from __future__ import annotations

import hashlib
import json
import struct
import tempfile
import unittest
from pathlib import Path

from nunlauncher_boot.binary import inspect_elf64_x86_64, inspect_pe_x86_64
from nunlauncher_boot.manifest import VerificationError
from nunlauncher_boot.verify import verify_image


def make_elf(entry: int = 0x400080, machine: int = 62) -> bytes:
    data = bytearray(120)
    data[:4] = b"\x7fELF"
    data[4:7] = bytes((2, 1, 1))
    struct.pack_into("<HHIQQQIHHHHHH", data, 16, 3, machine, 1, entry, 64, 0, 0, 64, 56, 1, 0, 0, 0)
    struct.pack_into("<IIQQQQQQ", data, 64, 1, 5, 0, 0x400000, 0, len(data), 0x1000, 0x1000)
    return bytes(data)


def make_pe(machine: int = 0x8664) -> bytes:
    data = bytearray(0x90)
    data[:2] = b"MZ"
    struct.pack_into("<I", data, 0x3C, 0x80)
    data[0x80:0x84] = b"PE\0\0"
    struct.pack_into("<H", data, 0x84, machine)
    return bytes(data)


class FakeInspector:
    def __init__(self, kernel: bytes | None = None, efi: bytes | None = None, modes=None):
        self.kernel = kernel if kernel is not None else make_elf()
        self.efi = efi if efi is not None else make_pe()
        self.modes = set(modes if modes is not None else ["bios", "uefi"])

    def boot_modes(self, image: Path) -> set[str]:
        return self.modes

    def extract(self, image: Path, iso_path: str) -> bytes:
        if iso_path == "boot/NUN/kernel.elf":
            return self.kernel
        if iso_path == "EFI/BOOT/BOOTX64.EFI":
            return self.efi
        raise AssertionError(f"unexpected path: {iso_path}")


class BootSupervisorTests(unittest.TestCase):
    def setUp(self):
        self.tmp = tempfile.TemporaryDirectory()
        self.root = Path(self.tmp.name)
        self.image = self.root / "NUN.iso"
        self.image.write_bytes(b"ISO9660-hybrid test fixture")
        self.kernel = make_elf()
        self.manifest = {
            "schema_version": 1,
            "product": "NUN",
            "artifact": {
                "file": self.image.name,
                "format": "ISO9660-hybrid",
                "size_bytes": self.image.stat().st_size,
                "sha256": hashlib.sha256(self.image.read_bytes()).hexdigest(),
            },
            "target": {
                "architecture": "x86_64",
                "firmware": ["bios", "uefi"],
                "boot_protocol": "limine",
                "limine_base_revision": 6,
            },
            "bootloader": {
                "name": "Limine",
                "version": "12.9.6",
                "archive_sha256": "f00bf1e994e969905b3a8259e2414809174491a8e8ae87e7129095745dfc6857",
            },
            "kernel": {
                "path": "boot/NUN/kernel.elf",
                "format": "ELF64",
                "endianness": "little",
                "machine": "x86_64",
                "type": "ET_DYN",
                "entry_point": "0x400080",
            },
            "security": {"digest_algorithm": "sha256", "signature_status": "unsigned"},
            "build": {"source_commit": "test-commit"},
        }
        self.manifest_path = self.root / "NUN.image.json"
        self.write_manifest()

    def tearDown(self):
        self.tmp.cleanup()

    def write_manifest(self):
        self.manifest_path.write_text(json.dumps(self.manifest), encoding="utf-8")

    def test_accepts_matching_image_and_boot_contract(self):
        result = verify_image(self.image, self.manifest_path, inspector=FakeInspector())
        self.assertEqual(result["status"], "verified-structure")
        self.assertEqual(result["architecture"], "x86_64")
        self.assertEqual(result["firmware"], ["bios", "uefi"])
        self.assertEqual(result["signature_status"], "unsigned")

    def test_rejects_tampered_image(self):
        self.image.write_bytes(b"changed image")
        with self.assertRaisesRegex(VerificationError, "size mismatch|SHA-256 mismatch"):
            verify_image(self.image, self.manifest_path, inspector=FakeInspector())

    def test_rejects_wrong_architecture_request(self):
        with self.assertRaisesRegex(VerificationError, "not supported"):
            verify_image(self.image, self.manifest_path, architecture="aarch64", inspector=FakeInspector())

    def test_rejects_missing_uefi_boot_entry(self):
        with self.assertRaisesRegex(VerificationError, "UEFI"):
            verify_image(self.image, self.manifest_path, inspector=FakeInspector(modes=["bios"]))

    def test_rejects_non_string_firmware_entry(self):
        self.manifest["target"]["firmware"] = ["bios", {"mode": "uefi"}]
        self.write_manifest()
        with self.assertRaisesRegex(VerificationError, "list of string"):
            verify_image(self.image, self.manifest_path, inspector=FakeInspector())


    def test_rejects_kernel_entry_mismatch(self):
        bad = FakeInspector(kernel=make_elf(entry=0x400090))
        with self.assertRaisesRegex(VerificationError, "entry point"):
            verify_image(self.image, self.manifest_path, inspector=bad)

    def test_rejects_non_amd64_efi_loader(self):
        with self.assertRaisesRegex(VerificationError, "AMD64"):
            verify_image(self.image, self.manifest_path, inspector=FakeInspector(efi=make_pe(0xAA64)))

    def test_signature_requirement_fails_closed(self):
        with self.assertRaisesRegex(VerificationError, "not configured"):
            verify_image(self.image, self.manifest_path, require_signature=True, inspector=FakeInspector())

    def test_elf_parser_checks_entry_in_executable_segment(self):
        with self.assertRaisesRegex(VerificationError, "not inside"):
            inspect_elf64_x86_64(make_elf(entry=0x500000))

    def test_elf_parser_rejects_non_x86_machine(self):
        with self.assertRaisesRegex(VerificationError, "x86_64"):
            inspect_elf64_x86_64(make_elf(machine=183))

    def test_pe_parser_rejects_wrong_machine(self):
        with self.assertRaisesRegex(VerificationError, "AMD64"):
            inspect_pe_x86_64(make_pe(0xAA64))


if __name__ == "__main__":
    unittest.main()
