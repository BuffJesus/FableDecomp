import unittest
from pathlib import Path

from tools.script_recovery.audit_guard_repeat_lecture import audit


class GuardRepeatLectureAuditTests(unittest.TestCase):
    def test_native_lua_and_runtime_repeat_flow(self):
        root = Path(__file__).resolve().parents[2]
        result = audit(
            root / "ghidra_out/script_recovery/guard_main_disasm.txt",
            root / "refs/script_recovery/reconstructed/NewOakValeIntro/FSE/NewOakValeIntro/Entities/NOVI_Guard.lua",
            root / "refs/script_recovery/new_oakvale_intro/runtime_evidence/single-authority-childhood-complete-20260910.log",
        )
        self.assertTrue(result["ok"], result)


if __name__ == "__main__":
    unittest.main()
