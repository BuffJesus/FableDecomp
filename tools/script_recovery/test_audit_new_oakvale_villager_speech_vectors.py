import unittest
from pathlib import Path

from tools.script_recovery.audit_new_oakvale_villager_speech_vectors import audit


class NewOakValeVillagerSpeechVectorAuditTests(unittest.TestCase):
    def test_native_metadata_lua_layout_and_teardown_agree(self):
        result = audit(Path(__file__).resolve().parents[2])
        self.assertTrue(result["ok"], result)
        self.assertEqual(result["vectors"], 8)
        self.assertEqual(result["textKeys"], 42)
        self.assertEqual(result["layoutBytes"], 96)
        self.assertEqual(result["nativeInitOrder"], [
            "FemaleBad", "MaleBad", "FemaleBoth", "MaleBoth",
            "FemaleGood", "MaleGood", "FemaleNeither", "MaleNeither",
        ])


if __name__ == "__main__":
    unittest.main()
