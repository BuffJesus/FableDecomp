import unittest
from pathlib import Path

from tools.script_recovery.audit_dead_father_cutscene_execution import audit


class DeadFatherCutsceneExecutionAuditTests(unittest.TestCase):
    def test_execution_and_state_restoration_are_closed(self):
        root = Path(__file__).resolve().parents[2]
        result = audit(root)
        self.assertTrue(result["ok"], result)
        self.assertEqual(result["remaining"], "visual camera composition parity")


if __name__ == "__main__":
    unittest.main()
