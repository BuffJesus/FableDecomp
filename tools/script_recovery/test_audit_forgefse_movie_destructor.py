import unittest
from pathlib import Path

from tools.script_recovery.audit_forgefse_movie_destructor import audit


class ForgeFSEMovieDestructorAuditTests(unittest.TestCase):
    def test_end_movie_uses_exact_retail_destructor(self):
        root = Path(__file__).resolve().parents[2]
        forge = Path(r"D:\Code\ForgeFSE-retail-shadow\FableScriptExtender")
        result = audit(
            forge / "FableAPI.cpp", forge / "LuaQuestState.cpp",
            root / "refs/script_recovery/new_oakvale_intro/entities",
            root / "refs/script_recovery/new_oakvale_intro/FORGEFSE_API_GAPS.md")
        self.assertTrue(result["ok"], result)


if __name__ == "__main__":
    unittest.main()
