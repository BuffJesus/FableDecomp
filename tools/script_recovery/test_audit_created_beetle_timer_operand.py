import unittest
from pathlib import Path

from tools.script_recovery.audit_created_beetle_timer_operand import audit


class CreatedBeetleTimerOperandAuditTests(unittest.TestCase):
    def test_both_get_timer_calls_use_registered_timer(self):
        root = Path(__file__).resolve().parents[2]
        result = audit(
            root / "ghidra_out/script_recovery/created_beetle_main_disasm.txt",
            root / "refs/script_recovery/reconstructed/NewOakValeIntro/FSE/NewOakValeIntro/Entities/NOVI_CreatedBeetle.lua",
            root / "refs/script_recovery/new_oakvale_intro/entities/NOVI_CreatedBeetle.json")
        self.assertTrue(result["ok"], result)


if __name__ == "__main__":
    unittest.main()
