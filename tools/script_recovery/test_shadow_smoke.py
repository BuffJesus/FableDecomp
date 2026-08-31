import json
import tempfile
import unittest
from pathlib import Path

from tools.script_recovery.verify_shadow_smoke_log import verify
from tools.script_recovery.verify_shadow_smoke_package import verify as verify_package


class ShadowSmokeLogTests(unittest.TestCase):
    def test_complete_log(self):
        with tempfile.TemporaryDirectory() as temp:
            root = Path(temp)
            (root / "manifest.json").write_text(json.dumps({"expectedScripts": 2}), encoding="utf-8")
            (root / "fse.log").write_text(
                "--- Fable Custom Quest DLL Attached ---\n"
                "--- Lua systems initialized successfully. ---\n"
                "    [RetailShadow] completed mutation-free preflight for Q_A\n"
                "    [RetailShadow] completed mutation-free preflight for Q_A::Entity\n"
                "--- Retail shadow preflight complete: passed=2 failed=0 (engine bindings unavailable by design). ---\n",
                encoding="utf-8")
            self.assertTrue(verify(root / "fse.log", root / "manifest.json")["complete"])

    def test_failure_is_not_complete(self):
        with tempfile.TemporaryDirectory() as temp:
            root = Path(temp)
            (root / "manifest.json").write_text(json.dumps({"expectedScripts": 1}), encoding="utf-8")
            (root / "fse.log").write_text("[RetailShadow] missing candidate: nope\n", encoding="utf-8")
            self.assertFalse(verify(root / "fse.log", root / "manifest.json")["complete"])

    def test_package_rejects_unsafe_policy(self):
        with tempfile.TemporaryDirectory() as temp:
            root = Path(temp)
            (root / "FSE").mkdir()
            (root / "FSE" / "retail_shadow.lua").write_text("enabled = true\n", encoding="utf-8")
            (root / "shadow_smoke_manifest.json").write_text(json.dumps({
                "safety": {"mode": "override", "mutations": True, "saveWrites": True},
                "expectedScripts": 0, "files": []}), encoding="utf-8")
            result = verify_package(root)
            self.assertFalse(result["complete"])
            self.assertIn("unsafe manifest policy", result["failures"])


if __name__ == "__main__":
    unittest.main()
