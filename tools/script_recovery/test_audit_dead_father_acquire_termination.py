import unittest
from pathlib import Path

from tools.script_recovery.audit_dead_father_acquire_termination import audit


class DeadFatherAcquireTerminationAuditTests(unittest.TestCase):
    def test_retail_forge_lua_and_fixture_agree(self):
        root = Path(__file__).resolve().parents[2]
        result = audit(
            root / "refs/script_recovery/new_oakvale_intro/translation_unit.json",
            Path(r"D:\Code\ForgeFSE-retail-shadow\FableScriptExtender\LuaEntityAPI.cpp"),
            root / "refs/script_recovery/reconstructed/NewOakValeIntro/FSE/NewOakValeIntro/common.lua",
            root / "refs/script_recovery/reconstructed/NewOakValeIntro/FSE/NewOakValeIntro/Entities/OVI_DeadFather.lua",
            root / "refs/script_recovery/new_oakvale_intro/fixtures/entity_dead_father_acquire_terminated.json",
        )
        self.assertTrue(result["ok"], result)


if __name__ == "__main__":
    unittest.main()
