import unittest
from pathlib import Path

from tools.script_recovery.audit_dead_father_movie_bracket import audit


class DeadFatherMovieBracketAuditTests(unittest.TestCase):
    def test_native_order_host_ownership_and_runtime_agree(self):
        root = Path(__file__).resolve().parents[2]
        result = audit(
            root / "refs/script_recovery/reconstructed/NewOakValeIntro/FSE/NewOakValeIntro/NewOakValeIntro.lua",
            Path(r"D:\Code\ForgeFSE-retail-shadow\FableScriptExtender\LuaQuestState.cpp"),
            root / "refs/script_recovery/new_oakvale_intro/runtime_evidence/father-cutscene-skip-success-20260910.log",
        )
        self.assertTrue(result["ok"], result)


if __name__ == "__main__":
    unittest.main()
