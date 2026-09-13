import unittest
from pathlib import Path

from tools.script_recovery.audit_new_oakvale_section_swap_runtime import audit


class NewOakValeSectionSwapRuntimeAuditTests(unittest.TestCase):
    def test_scripted_father_replacement_is_observed_across_three_runs(self):
        result = audit(Path(__file__).resolve().parents[2])
        self.assertTrue(result["ok"], result)
        self.assertTrue(all(result["observedRuns"].values()))


if __name__ == "__main__":
    unittest.main()
