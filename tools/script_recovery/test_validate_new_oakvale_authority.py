import tempfile
import unittest
from pathlib import Path

from tools.script_recovery.validate_new_oakvale_authority import validate


ROOT = Path(__file__).resolve().parents[2]
PROFILE = ROOT / "refs/script_recovery/new_oakvale_intro/runtime_playtest"
SOURCE = ROOT / "refs/script_recovery/reconstructed/NewOakValeIntro/FSE"


class NewOakvaleAuthorityValidationTests(unittest.TestCase):
    def test_checked_in_profile_is_single_authority(self):
        report = validate(PROFILE, SOURCE)
        self.assertTrue(report["ok"], report["errors"])
        self.assertEqual(report["nativeOverrideCount"], 1)
        self.assertEqual(report["competingAuthorities"], [])
        self.assertEqual(len(report["entityNames"]), 16)

    def test_rejects_legacy_custom_authority(self):
        with tempfile.TemporaryDirectory() as directory:
            profile = Path(directory)
            (profile / "retail_override.lua").write_text(
                (PROFILE / "retail_override.lua").read_text(encoding="utf-8"), encoding="utf-8")
            (profile / "quests.lua").write_text(
                'Quests={ bad={name="NewOakValeIntro",file="x",id=1,entity_scripts={}} }',
                encoding="utf-8")
            report = validate(profile, SOURCE)
            self.assertFalse(report["ok"])
            self.assertEqual(report["competingAuthorities"], ["NewOakValeIntro"])

    def test_rejects_enabled_repository_default(self):
        with tempfile.TemporaryDirectory() as directory:
            profile = Path(directory)
            text = (PROFILE / "retail_override.lua").read_text(encoding="utf-8")
            (profile / "retail_override.lua").write_text(
                text.replace("enabled = false", "enabled = true"), encoding="utf-8")
            (profile / "quests.lua").write_text(
                (PROFILE / "quests.lua").read_text(encoding="utf-8"), encoding="utf-8")
            report = validate(profile, SOURCE)
            self.assertFalse(report["ok"])
            self.assertIn("authority switch must be disabled in the repository profile", report["errors"])


if __name__ == "__main__":
    unittest.main()
