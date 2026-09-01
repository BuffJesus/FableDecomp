import json
import tempfile
import unittest
from unittest.mock import patch
from pathlib import Path

from tools.script_recovery.verify_shadow_smoke_log import verify
from tools.script_recovery.verify_shadow_smoke_package import verify as verify_package
from tools.script_recovery.deploy_shadow_smoke_package import deploy, rollback, verify_deployment


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

    @patch("tools.script_recovery.deploy_shadow_smoke_package.fable_running", return_value=False)
    def test_deploy_and_rollback_are_reversible(self, _running):
        with tempfile.TemporaryDirectory() as temp:
            root = Path(temp)
            package = root / "package"
            game = root / "game"
            (package / "FSE").mkdir(parents=True)
            game.mkdir()
            (game / "Fable.exe").write_bytes(b"exe")
            (game / "FableScriptExtender.dll").write_bytes(b"old")
            (package / "FableScriptExtender.dll").write_bytes(b"new")
            config = "enabled = true\nmutatingCallsAllowed = false\nsaveWritesAllowed = false\n"
            (package / "FSE" / "retail_shadow.lua").write_text(config, encoding="utf-8")
            files = []
            for relative in (Path("FableScriptExtender.dll"), Path("FSE/retail_shadow.lua")):
                path = package / relative
                from tools.script_recovery.verify_shadow_smoke_package import sha256
                files.append({"path": relative.as_posix(), "sha256": sha256(path), "size": path.stat().st_size})
            (package / "shadow_smoke_manifest.json").write_text(json.dumps({
                "safety": {"mode": "shadow", "mutations": False, "saveWrites": False},
                "expectedScripts": 1, "files": files}), encoding="utf-8")
            deployed = deploy(package, game)
            verified = verify_deployment(Path(deployed["transaction"]))
            self.assertTrue(verified["complete"])
            self.assertEqual(verified["intact"], deployed["deployed"])
            self.assertEqual((game / "FableScriptExtender.dll").read_bytes(), b"new")
            rolled_back = rollback(Path(deployed["transaction"]))
            self.assertEqual((game / "FableScriptExtender.dll").read_bytes(), b"old")
            self.assertFalse((game / "FSE" / "retail_shadow.lua").exists())
            self.assertEqual(rolled_back["restored"], 1)


if __name__ == "__main__":
    unittest.main()
