import unittest
from pathlib import Path

from tools.script_recovery.audit_forgefse_entity_setup_abi import audit


class ForgeFSEEntitySetupAbiAuditTests(unittest.TestCase):
    def test_victim_setup_preserves_all_retail_flags(self):
        root = Path(__file__).resolve().parents[2]
        forge = Path(r"D:\Code\ForgeFSE-retail-shadow\FableScriptExtender")
        result = audit(
            forge / "GameInterface.h", forge / "LuaQuestState.cpp",
            root / "refs/script_recovery/reconstructed/NewOakValeIntro/FSE/NewOakValeIntro/Entities/NOVI_Victim.lua",
            root / "refs/script_recovery/new_oakvale_intro/entities/NOVI_Victim.json")
        self.assertTrue(result["ok"], result)


if __name__ == "__main__":
    unittest.main()
