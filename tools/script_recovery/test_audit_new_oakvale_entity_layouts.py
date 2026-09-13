import unittest
from pathlib import Path

from tools.script_recovery.audit_new_oakvale_entity_layouts import audit


class NewOakValeEntityLayoutAuditTests(unittest.TestCase):
    def test_allocator_sizes_contain_all_class_local_fields(self):
        root = Path(__file__).resolve().parents[2]
        result = audit(
            root / "ghidra_out/script_recovery/new_oakvale_entity_layouts.json",
            root / "ghidra_out/script_recovery/new_oakvale_entity_vtables.json",
            root / "refs/script_recovery/new_oakvale_intro/translation_unit.json",
            root / "refs/script_recovery/new_oakvale_intro/entities")
        self.assertTrue(result["ok"], result)
        self.assertEqual(result["entityAllocators"], 16)
        self.assertEqual(result["classLocalFields"], 31)


if __name__ == "__main__":
    unittest.main()
