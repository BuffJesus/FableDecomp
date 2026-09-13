import unittest
from pathlib import Path

from tools.script_recovery.audit_new_oakvale_destructors import audit


class NewOakValeDestructorAuditTests(unittest.TestCase):
    def test_all_entity_destructors_have_the_shared_retail_cleanup_body(self):
        root = Path(__file__).resolve().parents[2]
        result = audit(root, root / "ghidra_out/script_recovery/new_oakvale_entity_vtables.json")
        self.assertTrue(result["ok"], result)
        self.assertEqual(result["entityDestructors"], 16)
        self.assertEqual(result["exportedDecompiles"], 16)
        self.assertEqual(result["uniqueCleanupBodies"], 1)
        self.assertEqual(result["sharedCleanupTarget"], "0x00F35B40")
        self.assertEqual(result["sharedCleanupCallers"], 16)
        self.assertEqual(result["supportDecompiles"], 1)

    def test_export_wrapper_is_snapshot_driven_and_read_only(self):
        root = Path(__file__).resolve().parents[2]
        source = (root / "tools/script_recovery/export_new_oakvale_destructors.ps1").read_text()
        self.assertIn("$snapshot.entities.PSObject.Properties.Value.destructor", source)
        self.assertIn("$snapshot.sharedEntityCleanup.address", source)
        self.assertIn('"-readOnly", "-noanalysis"', source)
        self.assertNotIn("0x00", source)


if __name__ == "__main__":
    unittest.main()
