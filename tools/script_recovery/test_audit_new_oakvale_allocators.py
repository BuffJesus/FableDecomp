import unittest
from pathlib import Path

from tools.script_recovery.audit_new_oakvale_allocators import audit


class NewOakValeAllocatorExportAuditTests(unittest.TestCase):
    def test_all_entity_allocators_have_exact_native_exports_and_wiring(self):
        root = Path(__file__).resolve().parents[2]
        result = audit(
            root, root / "ghidra_out/script_recovery/new_oakvale_entity_layouts.json",
            root / "ghidra_out/script_recovery/new_oakvale_entity_vtables.json")
        self.assertTrue(result["ok"], result)
        self.assertEqual(result["entityAllocators"], 16)
        self.assertEqual(result["exportedDecompiles"], 16)
        self.assertEqual(result["fullyWiredAllocators"], 16)

    def test_export_wrapper_is_layout_snapshot_driven_and_read_only(self):
        root = Path(__file__).resolve().parents[2]
        source = (root / "tools/script_recovery/export_new_oakvale_allocators.ps1").read_text()
        self.assertIn("$snapshot.entities.PSObject.Properties.Value.allocator", source)
        self.assertIn('"-readOnly", "-noanalysis"', source)
        self.assertNotIn("0x00", source)


if __name__ == "__main__":
    unittest.main()
