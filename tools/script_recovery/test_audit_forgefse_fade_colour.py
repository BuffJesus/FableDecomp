import unittest
from pathlib import Path

from tools.script_recovery.audit_forgefse_fade_colour import audit


class ForgeFSEFadeColourAuditTests(unittest.TestCase):
    def test_implicit_colour_matches_both_retail_calls(self):
        root = Path(__file__).resolve().parents[2]
        evidence = root / "refs/script_recovery/new_oakvale_intro/entities"
        result = audit(
            Path(r"D:\Code\ForgeFSE-retail-shadow\FableScriptExtender\LuaQuestState.cpp"),
            root / "refs/script_recovery/reconstructed/NewOakValeIntro/FSE/NewOakValeIntro/Entities/NOVI_BarrelMan.lua",
            evidence / "NOVI_BarrelMan.json", evidence / "NOVI_Theresa.json")
        self.assertTrue(result["ok"], result)


if __name__ == "__main__":
    unittest.main()
