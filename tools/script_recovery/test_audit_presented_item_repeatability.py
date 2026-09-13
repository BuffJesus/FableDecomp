import unittest
from pathlib import Path

from tools.script_recovery.audit_presented_item_repeatability import audit


class PresentedItemRepeatabilityAuditTests(unittest.TestCase):
    def test_retail_forge_and_lua_repeatability(self):
        root = Path(__file__).resolve().parents[2]
        result = audit(
            root / "ghidra_out/fablewin_pdb_names.tsv",
            root / "ghidra_out/script_recovery/presented_item_native.c",
            root / "ghidra_out/script_recovery/presented_item_native_disasm.txt",
            root / "ghidra_out/script_recovery/theresa_presented_item_disasm.txt",
            root / "refs/script_recovery/reconstructed/NewOakValeIntro/FSE/NewOakValeIntro/Entities/NOVI_Theresa.lua",
            Path(r"D:\Code\ForgeFSE-retail-shadow\FableScriptExtender\LuaEntityAPI.cpp"),
        )
        self.assertTrue(result["ok"], result)


if __name__ == "__main__":
    unittest.main()
