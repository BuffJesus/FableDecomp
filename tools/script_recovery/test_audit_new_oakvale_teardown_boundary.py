import unittest
from pathlib import Path

from tools.script_recovery.audit_new_oakvale_teardown_boundary import audit


class NewOakValeTeardownBoundaryAuditTests(unittest.TestCase):
    def test_completion_teardown_and_termination_guards_are_proven(self):
        result = audit(Path(__file__).resolve().parents[2])
        self.assertTrue(result["ok"], result)
        self.assertEqual(result["remainingRuntimeBoundary"], "repeated mid-childhood region unload/reload re-entry timing")
        self.assertGreaterEqual(len(result["runtimeExitedEntityClasses"]), 10)


if __name__ == "__main__":
    unittest.main()
