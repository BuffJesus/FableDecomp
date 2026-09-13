import unittest
from pathlib import Path

from tools.script_recovery.audit_guard_lecture_distance import audit


class GuardLectureDistanceAuditTests(unittest.TestCase):
    def test_all_three_distance_checks_use_same_retail_constant(self):
        root = Path(__file__).resolve().parents[2]
        result = audit(
            root / "ghidra_out/script_recovery/guard_main_disasm.txt",
            root / "refs/script_recovery/reconstructed/NewOakValeIntro/FSE/NewOakValeIntro/Entities/NOVI_Guard.lua",
            root / "refs/script_recovery/new_oakvale_intro/entities/NOVI_Guard.json")
        self.assertTrue(result["ok"], result)


if __name__ == "__main__":
    unittest.main()
