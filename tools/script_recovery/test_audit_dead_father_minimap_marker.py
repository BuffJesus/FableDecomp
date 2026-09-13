import unittest
from pathlib import Path

from tools.script_recovery.audit_dead_father_minimap_marker import audit


class DeadFatherMinimapMarkerAuditTests(unittest.TestCase):
    def test_native_forge_lua_and_lifecycle_agree(self):
        root = Path(__file__).resolve().parents[2]
        result = audit(
            root / "refs/script_recovery/new_oakvale_intro/translation_unit.json",
            root / "ghidra_out/egor_pdb_names.tsv",
            Path(r"D:\Code\ForgeFSE-retail-shadow\FableScriptExtender\LuaQuestState.cpp"),
            root / "refs/script_recovery/reconstructed/NewOakValeIntro/FSE/NewOakValeIntro/Entities/OVI_DeadFather.lua",
            root / "refs/script_recovery/new_oakvale_intro/entities/OVI_DeadFather.json",
        )
        self.assertTrue(result["ok"], result)


if __name__ == "__main__":
    unittest.main()
