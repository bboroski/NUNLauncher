import json
import sys
import unittest
from pathlib import Path

ROOT = Path(__file__).resolve().parents[1]
sys.path.insert(0, str(ROOT / "tools"))
from validate_boot_profile import validate_profile  # noqa: E402


class BootProfileTests(unittest.TestCase):
    def setUp(self):
        self.profile = json.loads(
            (ROOT / "spec" / "domain-boot-profile.json").read_text(encoding="utf-8")
        )

    def test_reference_profile_is_valid(self):
        self.assertEqual(validate_profile(self.profile), [])

    def test_overlapping_regions_rejected(self):
        self.profile["memory_regions"][1]["start"] = 1500000
        self.assertTrue(any("overlap" in item for item in validate_profile(self.profile)))

    def test_entry_point_must_be_executable(self):
        self.profile["entry_point"] = 2097152
        self.assertTrue(any("entry_point" in item for item in validate_profile(self.profile)))

    def test_bad_digest_rejected(self):
        self.profile["artifact"]["sha256"] = "not-a-digest"
        self.assertTrue(any("sha256" in item for item in validate_profile(self.profile)))

    def test_retry_count_is_bounded(self):
        self.profile["recovery"]["max_attempts"] = 99
        self.assertTrue(any("max_attempts" in item for item in validate_profile(self.profile)))


if __name__ == "__main__":
    unittest.main()
