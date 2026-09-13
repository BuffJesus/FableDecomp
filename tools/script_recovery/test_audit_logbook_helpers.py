import unittest
from pathlib import Path

from tools.script_recovery.audit_logbook_helpers import audit


class LogbookHelperAuditTests(unittest.TestCase):
    def test_native_semantics_forge_wiring_and_new_oakvale_call_agree(self):
        root = Path(__file__).resolve().parents[2]
        forge = Path(r"D:\Code\ForgeFSE-retail-shadow\FableScriptExtender")
        result = audit(
            root / "ghidra_out/script_recovery/logbook_helpers_disasm.txt",
            forge / "FableAPI.h", forge / "FableAPI.cpp",
            forge / "LuaQuestState.cpp",
            root / "refs/script_recovery/reconstructed/NewOakValeIntro/FSE/NewOakValeIntro/deeds.lua",
        )
        self.assertTrue(result["ok"], result)


if __name__ == "__main__":
    unittest.main()
