import unittest
from pathlib import Path

from tools.script_recovery.audit_new_oakvale_native_sources import audit


class NewOakvaleNativeSourceAuditTests(unittest.TestCase):
    def test_every_inventory_function_has_a_local_reproducible_decompile(self):
        root = Path(__file__).resolve().parents[2]
        result = audit(root, root / "refs/script_recovery/new_oakvale_intro/entities")
        self.assertTrue(result["ok"], result)
        self.assertEqual(result["inventoryFunctions"], 52)
        self.assertEqual(result["uniqueNativeEntries"], 50)
        self.assertEqual(result["exportedDecompiles"], 50)
        self.assertEqual(result["validatedCoverageAliases"], 1)

    def test_export_wrapper_is_inventory_driven_and_read_only(self):
        root = Path(__file__).resolve().parents[2]
        source = (root / "tools/script_recovery/export_new_oakvale_native_sources.ps1").read_text()
        self.assertIn("$inventory.functions.PSObject.Properties.Value.address", source)
        self.assertIn('"-readOnly", "-noanalysis"', source)
        self.assertIn("audit_new_oakvale_native_sources.py", source)
        self.assertNotIn("0x00", source)


if __name__ == "__main__":
    unittest.main()
