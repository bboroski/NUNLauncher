from __future__ import annotations

import hashlib
import json
import struct
import tempfile
import unittest
from pathlib import Path

from nunlauncher_boot.e2k import verify_e2k_runtime
from nunlauncher_boot.manifest import VerificationError


def make_e2k_elf(machine: int = 175, entry: int = 0x400080) -> bytes:
    data = bytearray(120)
    data[:4] = b"\x7fELF"
    data[4:7] = bytes((2, 1, 1))
    struct.pack_into("<HHIQQQIHHHHHH", data, 16, 2, machine, 1, entry, 64, 0, 0, 64, 56, 1, 0, 0, 0)
    struct.pack_into("<IIQQQQQQ", data, 64, 1, 5, 0, 0x400000, 0, len(data), 0x1000, 0x1000)
    return bytes(data)


class E2KRuntimeVerifierTests(unittest.TestCase):
    def setUp(self):
        self.temp = tempfile.TemporaryDirectory()
        self.root = Path(self.temp.name)
        self.binary = self.root / "NUN-E2K-host"
        self.binary.write_bytes(make_e2k_elf())
        self.manifest = self.root / "NUN-E2K-host.json"
        self.write_manifest()

    def tearDown(self):
        self.temp.cleanup()

    def write_manifest(self):
        self.manifest.write_text(json.dumps({
            "schema_version": "nun.e2k.runtime.v1",
            "product": "NUN",
            "artifact": {
                "file": self.binary.name,
                "size_bytes": self.binary.stat().st_size,
                "sha256": hashlib.sha256(self.binary.read_bytes()).hexdigest(),
            },
            "target": {
                "architecture": "e2k",
                "os": "linux",
                "word_bits": 64,
                "endianness": "little",
                "elf_machine": 175,
                "execution_mode": "native-linux-userspace",
                "deployment_chain": "MCST-PNS-to-Elbrus-Linux-to-NUN-userspace",
            },
            "binary": {
                "format": "ELF64",
                "endianness": "little",
                "machine": "EM_MCST_ELBRUS",
                "e_machine": 175,
                "type": "ET_EXEC",
                "entry_point": "0x400080",
            },
            "compiler": {"family": "LCC", "version": "1.31.05"},
            "security": {"digest_algorithm": "sha256", "signature_status": "unsigned"},
            "build": {"source_commit": "unit-test"},
        }), encoding="utf-8")

    def test_accepts_matching_e2k_elf(self):
        result = verify_e2k_runtime(self.binary, self.manifest)
        self.assertEqual(result["status"], "verified-e2k-elf-structure")
        self.assertEqual(result["architecture"], "e2k")
        self.assertEqual(result["elf_machine"], 175)
        self.assertEqual(result["boot_status"], "not-tested-by-preflight")

    def refresh_manifest_digest(self):
        data = json.loads(self.manifest.read_text(encoding="utf-8"))
        payload = self.binary.read_bytes()
        data["artifact"]["size_bytes"] = len(payload)
        data["artifact"]["sha256"] = hashlib.sha256(payload).hexdigest()
        self.manifest.write_text(json.dumps(data), encoding="utf-8")

    def test_rejects_x86_or_wrong_machine(self):
        self.binary.write_bytes(make_e2k_elf(machine=62))
        self.refresh_manifest_digest()
        with self.assertRaisesRegex(VerificationError, "175"):
            verify_e2k_runtime(self.binary, self.manifest)

    def test_rejects_tampered_binary(self):
        self.binary.write_bytes(b"tampered binary")
        with self.assertRaisesRegex(VerificationError, "size mismatch|SHA-256"):
            verify_e2k_runtime(self.binary, self.manifest)

    def test_rejects_entry_outside_executable_segment(self):
        self.binary.write_bytes(make_e2k_elf(entry=0x500000))
        self.refresh_manifest_digest()
        with self.assertRaisesRegex(VerificationError, "outside an executable"):
            verify_e2k_runtime(self.binary, self.manifest)

    def test_rejects_signature_required(self):
        with self.assertRaisesRegex(VerificationError, "not configured"):
            verify_e2k_runtime(self.binary, self.manifest, require_signature=True)

    def test_rejects_wrong_target_os(self):
        data = json.loads(self.manifest.read_text(encoding="utf-8"))
        data["target"]["os"] = "windows"
        self.manifest.write_text(json.dumps(data), encoding="utf-8")
        with self.assertRaisesRegex(VerificationError, "target profile"):
            verify_e2k_runtime(self.binary, self.manifest)


if __name__ == "__main__":
    unittest.main()
