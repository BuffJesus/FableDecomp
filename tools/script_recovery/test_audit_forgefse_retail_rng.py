import tempfile
import unittest
from pathlib import Path

from tools.script_recovery.audit_forgefse_retail_rng import audit


class ForgeFSERetailRngAuditTests(unittest.TestCase):
    def setUp(self):
        self.root = Path(__file__).resolve().parents[2]
        self.forge = Path(r"D:\Code\ForgeFSE-retail-shadow\FableScriptExtender\LuaQuestState.cpp")
        self.lua = self.root / "refs/script_recovery/reconstructed/NewOakValeIntro/FSE/NewOakValeIntro"

    def test_all_random_sites_use_retail_stream(self):
        result = audit(self.forge, self.lua)
        self.assertTrue(result["ok"], result)

    def test_lua_rng_is_rejected(self):
        with tempfile.TemporaryDirectory() as temporary:
            test_root = Path(temporary)
            (test_root / "bad.lua").write_text("return math.random()\n", encoding="utf-8")
            result = audit(self.forge, test_root)
        self.assertFalse(result["ok"])
        self.assertFalse(result["checks"]["noLuaMathRandom"])


if __name__ == "__main__":
    unittest.main()
