import unittest
from pathlib import Path

from tools.script_recovery.audit_new_oakvale_timer_render_closure import audit


class NewOakValeTimerRenderClosureAuditTests(unittest.TestCase):
    def test_static_and_runtime_timer_render_evidence_is_closed(self):
        root = Path(__file__).resolve().parents[2]
        result = audit(root)
        self.assertTrue(result["ok"], result)
        self.assertEqual(result["observedCountdown"], list(range(45, -1, -1)))


if __name__ == "__main__":
    unittest.main()
