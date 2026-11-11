from __future__ import annotations

import hashlib
import json
import sys
import tempfile
import unittest
from pathlib import Path

sys.path.insert(0, str(Path(__file__).resolve().parents[1] / "tools"))
from validate_boot_image import ManifestError, validate_manifest


class BootManifestTests(unittest.TestCase):
    def setUp(self) -> None:
        self.temp = tempfile.TemporaryDirectory()
        self.root = Path(self.temp.name)
        self.payload = b"NUN test kernel image"
        (self.root / "kernel.bin").write_bytes(self.payload)
        self.manifest = self.root / "kernel.manifest.json"
        self.data = {
            "format": "nun.kernel-image",
            "version": 1,
            "architecture": "x86_64",
            "image": "kernel.bin",
            "image_size": len(self.payload),
            "sha256": hashlib.sha256(self.payload).hexdigest(),
            "load_address": 0x100000,
            "memory_size": 0x2000,
            "entry_address": 0x100000,
            "entry_alignment": 16,
        }
        self.write_manifest()

    def tearDown(self) -> None:
        self.temp.cleanup()

    def write_manifest(self) -> None:
        self.manifest.write_text(json.dumps(self.data), encoding="utf-8")

    def test_valid_manifest(self) -> None:
        result = validate_manifest(self.manifest, expected_architecture="x86_64")
        self.assertEqual(result["architecture"], "x86_64")
        self.assertEqual(result["image_size"], len(self.payload))
        self.assertFalse(result["experimental"])

    def test_digest_mismatch_is_rejected(self) -> None:
        self.data["sha256"] = "0" * 64
        self.write_manifest()
        with self.assertRaisesRegex(ManifestError, "digest mismatch"):
            validate_manifest(self.manifest)

    def test_path_traversal_is_rejected(self) -> None:
        self.data["image"] = "../outside.bin"
        self.write_manifest()
        with self.assertRaisesRegex(ManifestError, "relative path"):
            validate_manifest(self.manifest)

    def test_entry_outside_memory_region_is_rejected(self) -> None:
        self.data["entry_address"] = 0x900000
        self.write_manifest()
        with self.assertRaisesRegex(ManifestError, "inside the declared load region"):
            validate_manifest(self.manifest)

    def test_non_power_of_two_alignment_is_rejected(self) -> None:
        self.data["entry_alignment"] = 3
        self.write_manifest()
        with self.assertRaisesRegex(ManifestError, "power of two"):
            validate_manifest(self.manifest)

    def test_architecture_mismatch_is_rejected(self) -> None:
        with self.assertRaisesRegex(ManifestError, "does not match"):
            validate_manifest(self.manifest, expected_architecture="aarch64")

    def test_e2k_requires_explicit_experimental_opt_in(self) -> None:
        self.data["architecture"] = "e2k"
        self.write_manifest()
        with self.assertRaisesRegex(ManifestError, "experimental"):
            validate_manifest(self.manifest)
        result = validate_manifest(self.manifest, allow_experimental_target=True)
        self.assertTrue(result["experimental"])


if __name__ == "__main__":
    unittest.main()
