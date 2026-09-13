import unittest
from pathlib import Path

from tools.script_recovery.audit_new_oakvale_cleanup_paths import audit


class NewOakValeCleanupPathAuditTests(unittest.TestCase):
    def test_cleanup_evidence_references_real_inventory_operations(self):
        root = Path(__file__).resolve().parents[2]
        result = audit(root / "refs/script_recovery/new_oakvale_intro/entities")
        self.assertTrue(result["ok"], result)
        self.assertEqual(result["filesWithCleanupEvidence"], 17)
        self.assertEqual(result["cleanupRecords"], 73)
        self.assertGreaterEqual(result["acquisitionSequenceReferences"], 130)


if __name__ == "__main__":
    unittest.main()
