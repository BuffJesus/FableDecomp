import unittest
from pathlib import Path

from tools.script_recovery.audit_new_oakvale_inventory_structure import audit


class NewOakvaleInventoryStructureAuditTests(unittest.TestCase):
    def test_every_operation_has_an_ordered_declared_native_owner(self):
        root = Path(__file__).resolve().parents[2]
        result = audit(root / "refs/script_recovery/new_oakvale_intro/entities")
        self.assertTrue(result["ok"], result)
        self.assertEqual(result["inventoriedOperations"], 747)
        self.assertEqual(len(result["nestedLuaHelpers"]), 9)


if __name__ == "__main__":
    unittest.main()
