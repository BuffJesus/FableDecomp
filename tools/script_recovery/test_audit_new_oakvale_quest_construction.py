import unittest
from pathlib import Path

from tools.script_recovery.audit_new_oakvale_quest_construction import audit
from tools.script_recovery.export_new_oakvale_quest_vtable import export
from tools.script_recovery.export_new_oakvale_quest_ownership import export as export_ownership


class NewOakValeQuestConstructionAuditTests(unittest.TestCase):
    def test_quest_construction_and_lifecycle_are_exported(self):
        root = Path(__file__).resolve().parents[2]
        result = audit(root, root / "refs/script_recovery/native_clusters/Q_NewOakValeIntro.json")
        self.assertTrue(result["ok"], result)
        self.assertEqual(result["objectSize"], 0x10C)
        self.assertEqual(result["lifecycleSlots"], 5)
        self.assertEqual(result["exportedFunctions"], 10)

    def test_export_wrapper_is_cluster_driven_and_read_only(self):
        root = Path(__file__).resolve().parents[2]
        source = (root / "tools/script_recovery/export_new_oakvale_quest_construction.ps1").read_text()
        self.assertIn("$cluster.allocatorAddress", source)
        self.assertIn("$cluster.constructorAddress", source)
        self.assertIn("$cluster.baseConstructorAddress", source)
        self.assertIn("$cluster.destructorImplementationAddress", source)
        self.assertIn("$cluster.baseDestructorAddress", source)
        self.assertIn("$cluster.lifecycle.address", source)
        self.assertIn('"-readOnly", "-noanalysis"', source)
        self.assertNotIn("0x00", source)

    def test_checked_in_vtable_snapshot_reproduces_from_installed_retail_exe(self):
        root = Path(__file__).resolve().parents[2]
        exe = Path(r"C:\Programs\Steam\steamapps\common\Fable The Lost Chapters\Fable.exe")
        if not exe.exists():
            self.skipTest("installed retail executable is unavailable")
        actual = export(exe, root / "refs/script_recovery/native_clusters/Q_NewOakValeIntro.json")
        expected = __import__("json").loads(
            (root / "ghidra_out/script_recovery/new_oakvale_quest_vtable.json").read_text(encoding="utf-8"))
        self.assertEqual(actual, expected)

    def test_checked_in_ownership_snapshot_reproduces_from_installed_retail_exe(self):
        root = Path(__file__).resolve().parents[2]
        exe = Path(r"C:\Programs\Steam\steamapps\common\Fable The Lost Chapters\Fable.exe")
        if not exe.exists():
            self.skipTest("installed retail executable is unavailable")
        actual = export_ownership(exe, root / "refs/script_recovery/native_clusters/Q_NewOakValeIntro.json")
        expected = __import__("json").loads(
            (root / "ghidra_out/script_recovery/new_oakvale_quest_ownership.json").read_text(encoding="utf-8"))
        self.assertEqual(actual, expected)


if __name__ == "__main__":
    unittest.main()
