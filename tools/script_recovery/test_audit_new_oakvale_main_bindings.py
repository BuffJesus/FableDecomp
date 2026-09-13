import json
import unittest
from pathlib import Path

from tools.script_recovery.audit_new_oakvale_main_bindings import audit
from tools.script_recovery.export_new_oakvale_main_bindings import export


class NewOakValeMainBindingsAuditTests(unittest.TestCase):
    def test_retail_layout_lua_and_forge_bindings_agree(self):
        root = Path(__file__).resolve().parents[2]
        result = audit(root, Path(r"D:\Code\ForgeFSE-retail-shadow"))
        self.assertTrue(result["ok"], result)
        self.assertEqual(result["retailBindings"], 16)
        self.assertEqual(result["allocatorMatches"], 16)

    def test_checked_in_snapshot_reproduces_from_installed_exe(self):
        root = Path(__file__).resolve().parents[2]
        exe = Path(r"C:\Programs\Steam\steamapps\common\Fable The Lost Chapters\Fable.exe")
        if not exe.exists():
            self.skipTest("installed retail executable is unavailable")
        actual = export(exe, root / "refs/script_recovery/native_clusters/Q_NewOakValeIntro.json")
        expected = json.loads((root / "ghidra_out/script_recovery/new_oakvale_main_bindings_retail_bytes.json").read_text(encoding="utf-8"))
        self.assertEqual(actual, expected)


if __name__ == "__main__":
    unittest.main()
