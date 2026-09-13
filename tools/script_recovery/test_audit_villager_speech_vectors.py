import unittest
from pathlib import Path

from tools.script_recovery.audit_villager_speech_vectors import audit


class VillagerSpeechVectorAuditTests(unittest.TestCase):
    def test_native_inventory_and_lua_vectors_are_identical(self):
        root = Path(__file__).resolve().parents[2]
        result = audit(
            root / "refs/script_recovery/new_oakvale_intro/entities/NOVI_Villager.json",
            root / "refs/script_recovery/reconstructed/NewOakValeIntro/FSE/NewOakValeIntro/villager_speech.lua",
            root / "ghidra_out/script_recovery/native_decompiles/0x00DAADD0.c")
        self.assertTrue(result["ok"], result)
        self.assertEqual(result["vectors"], 8)
        self.assertEqual(result["keys"], 42)


if __name__ == "__main__":
    unittest.main()
