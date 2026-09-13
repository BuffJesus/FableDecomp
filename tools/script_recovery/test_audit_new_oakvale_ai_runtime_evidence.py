import unittest
from pathlib import Path

from tools.script_recovery.audit_new_oakvale_ai_runtime_evidence import audit


class NewOakValeAIRuntimeEvidenceAuditTests(unittest.TestCase):
    def test_success_and_intermittent_shortfall_are_both_preserved(self):
        result = audit(Path(__file__).resolve().parents[2])
        self.assertTrue(result["ok"], result)
        self.assertGreater(result["blockingSpeechCompletions"], 0)
        self.assertEqual(result["classification"], "intermittent path-completion variance")


if __name__ == "__main__":
    unittest.main()
