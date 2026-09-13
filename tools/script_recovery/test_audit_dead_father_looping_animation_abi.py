import unittest
from pathlib import Path

from tools.script_recovery.audit_dead_father_looping_animation_abi import audit


class DeadFatherLoopingAnimationAbiAuditTests(unittest.TestCase):
    def test_all_seven_retail_flags_reach_native_vtable(self):
        root = Path(__file__).resolve().parents[2]
        forge = Path(r"D:\Code\ForgeFSE-retail-shadow\FableScriptExtender")
        result = audit(
            root / "refs/script_recovery/reconstructed/NewOakValeIntro/FSE/NewOakValeIntro/Entities/OVI_DeadFather.lua",
            root / "refs/script_recovery/new_oakvale_intro/entities/OVI_DeadFather.json",
            forge / "LuaManager.cpp", forge / "EntityScriptingAPI.h", forge / "LuaEntityAPI.cpp")
        self.assertTrue(result["ok"], result)


if __name__ == "__main__":
    unittest.main()
