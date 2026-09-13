import unittest
from pathlib import Path

from tools.script_recovery.audit_new_oakvale_quest_teardown import audit


class NewOakValeQuestTeardownAuditTests(unittest.TestCase):
    def test_complete_teardown_chain_matches_layout_and_helper_ir(self):
        result = audit(Path(__file__).resolve().parents[2])
        self.assertTrue(result["ok"], result)
        self.assertEqual(result["timersDeregistered"], 2)
        self.assertEqual(result["speechVectorsDestroyed"], 8)
        self.assertEqual(result["baseCleanup"], "0x00CBD510")
        self.assertEqual(result["baseClass"], "CScriptBase")
        self.assertEqual(result["baseDestructorConsumers"], 125)


if __name__ == "__main__":
    unittest.main()
