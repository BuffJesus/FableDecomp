import unittest
from pathlib import Path

from tools.script_recovery.audit_new_oakvale_no_gap_scaffolding import audit


class NewOakvaleNoGapScaffoldingAuditTests(unittest.TestCase):
    def test_zero_blocker_package_has_no_placeholder_framework(self):
        root = Path(__file__).resolve().parents[2]
        evidence = root / "refs/script_recovery/new_oakvale_intro"
        result = audit(
            root / "refs/script_recovery/reconstructed/NewOakValeIntro/FSE",
            evidence / "api_requirements.json",
            evidence / "coverage.json")
        self.assertTrue(result["ok"], result)


if __name__ == "__main__":
    unittest.main()
