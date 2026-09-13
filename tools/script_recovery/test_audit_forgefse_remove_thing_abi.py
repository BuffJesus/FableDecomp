import unittest
from pathlib import Path

from tools.script_recovery.audit_forgefse_remove_thing_abi import audit


class ForgeFSERemoveThingAbiAuditTests(unittest.TestCase):
    def test_native_effects_and_forge_forwarding(self):
        root = Path(__file__).resolve().parents[2]
        result = audit(
            root / "ghidra_out/scriptvm_decomp.c",
            Path(r"D:\Code\ForgeFSE-retail-shadow\FableScriptExtender\GameInterface.h"),
            Path(r"D:\Code\ForgeFSE-retail-shadow\FableScriptExtender\LuaQuestState.cpp"),
            root / "refs/script_recovery/new_oakvale_intro/entities/NOVI_CreatedBeetle.json",
        )
        self.assertTrue(result["ok"], result)


if __name__ == "__main__":
    unittest.main()
