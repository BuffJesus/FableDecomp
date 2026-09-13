import unittest
from pathlib import Path

from tools.script_recovery.audit_new_oakvale_quest_initialization import audit
from tools.script_recovery.export_new_oakvale_init_writes import export


class NewOakValeQuestInitializationAuditTests(unittest.TestCase):
    def test_lua_resets_exactly_the_native_init_fields(self):
        result = audit(Path(__file__).resolve().parents[2])
        self.assertTrue(result["ok"], result)
        self.assertEqual(result["directRetailScalarWrites"], 27)
        self.assertEqual(result["fieldFamilies"], 37)
        self.assertEqual(result["explicitInitStores"], 27)
        self.assertEqual(result["luaScalarResets"], 25)
        self.assertEqual(result["producerInitializedFields"], 10)

    def test_checked_in_init_snapshot_reproduces_from_installed_exe(self):
        root = Path(__file__).resolve().parents[2]
        exe = Path(r"C:\Programs\Steam\steamapps\common\Fable The Lost Chapters\Fable.exe")
        if not exe.exists():
            self.skipTest("installed retail executable is unavailable")
        actual = export(exe, root / "refs/script_recovery/native_clusters/Q_NewOakValeIntro.json")
        expected = __import__("json").loads(
            (root / "ghidra_out/script_recovery/new_oakvale_init_retail_bytes.json").read_text(encoding="utf-8"))
        self.assertEqual(actual, expected)


if __name__ == "__main__":
    unittest.main()
