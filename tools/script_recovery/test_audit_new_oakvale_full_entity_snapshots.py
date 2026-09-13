import unittest
from pathlib import Path
from tools.script_recovery.audit_new_oakvale_full_entity_snapshots import audit

class FullEntitySnapshotAuditTests(unittest.TestCase):
    def test_all_new_oakvale_entities_have_complete_snapshots(self):
        result = audit(Path(__file__).resolve().parents[2])
        self.assertTrue(result["ok"], result)
        self.assertEqual(result["entities"], 15)
        self.assertEqual(result["complete"], 15)

if __name__ == "__main__":
    unittest.main()
