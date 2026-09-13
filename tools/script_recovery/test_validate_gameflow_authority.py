import tempfile
import unittest
from pathlib import Path

from tools.script_recovery.validate_gameflow_authority import validate


class ValidateGameflowAuthorityTests(unittest.TestCase):
    def _profile(self, root: Path, enabled: str = "false", custom: str = "FSE_Master"):
        profile = root / "profile"
        source = root / "source"
        profile.mkdir()
        (source / "LUAGameflow").mkdir(parents=True)
        (source / "LUAGameflow" / "LUAGameflow.lua").write_text("", encoding="utf-8")
        (profile / "retail_override.lua").write_text(
            "RetailOverrides={enabled=" + enabled + ",entries={{nativeName='Gameflow',file='LUAGameflow/LUAGameflow',mode='override',mutatingCallsAllowed=true,saveWritesAllowed=true}}}",
            encoding="utf-8",
        )
        (profile / "quests.lua").write_text(
            "Quests={x={name='" + custom + "',file='Master/FSE_Master',id=1,entity_scripts={}}}",
            encoding="utf-8",
        )
        return profile, source

    def test_safe_profile(self):
        with tempfile.TemporaryDirectory() as directory:
            profile, source = self._profile(Path(directory))
            self.assertTrue(validate(profile, source)["ok"])

    def test_enabled_repository_default_fails(self):
        with tempfile.TemporaryDirectory() as directory:
            profile, source = self._profile(Path(directory), enabled="true")
            self.assertFalse(validate(profile, source)["ok"])

    def test_named_lua_gameflow_competitor_fails(self):
        with tempfile.TemporaryDirectory() as directory:
            profile, source = self._profile(Path(directory), custom="LUAGameflow")
            self.assertFalse(validate(profile, source)["ok"])


if __name__ == "__main__":
    unittest.main()
