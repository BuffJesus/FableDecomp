import unittest
from pathlib import Path

from tools.script_recovery.verify_foundation import verify


class FoundationAuditTests(unittest.TestCase):
    def test_checked_in_foundation_is_complete(self):
        result = verify(Path("refs/script_recovery"))
        failures = [row for row in result["checks"] if not row["passed"]]
        self.assertTrue(result["summary"]["complete"], failures)


if __name__ == "__main__":
    unittest.main()
