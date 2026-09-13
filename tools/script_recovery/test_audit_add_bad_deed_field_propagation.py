import unittest
from pathlib import Path

from tools.script_recovery.audit_add_bad_deed_field_propagation import audit


class AddBadDeedFieldPropagationAuditTests(unittest.TestCase):
    def test_helper_field_writes_propagate_without_inference_labels(self):
        root = Path(__file__).resolve().parents[2]
        result = audit(
            root / "ghidra_out/script_recovery/add_bad_deed_disasm.txt",
            root / "refs/script_recovery/new_oakvale_intro/entities")
        self.assertTrue(result["ok"], result)


if __name__ == "__main__":
    unittest.main()
