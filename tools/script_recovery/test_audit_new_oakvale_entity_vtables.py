import unittest
from pathlib import Path

from tools.script_recovery.audit_new_oakvale_entity_vtables import audit


class NewOakValeEntityVtableAuditTests(unittest.TestCase):
    def test_retail_slots_prove_entity_function_ownership(self):
        root = Path(__file__).resolve().parents[2]
        result = audit(
            root / "ghidra_out/script_recovery/new_oakvale_entity_vtables.json",
            root / "refs/script_recovery/new_oakvale_intro/entities", root,
            root / "refs/script_recovery/new_oakvale_intro/translation_unit.json")
        self.assertTrue(result["ok"], result)
        self.assertEqual(result["entities"], 16)
        self.assertEqual(result["functionSlots"], 32)
        self.assertEqual(result["nativeDecompiles"], 32)


if __name__ == "__main__":
    unittest.main()
