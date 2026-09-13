import json
import unittest
from pathlib import Path

from tools.script_recovery.audit_new_oakvale_attack_stuff import audit
from tools.script_recovery.export_new_oakvale_attack_stuff import export


class NewOakValeAttackStuffAuditTests(unittest.TestCase):
    def test_retail_lua_and_interface_catalog_agree(self):
        root = Path(__file__).resolve().parents[2]
        result = audit(root)
        self.assertTrue(result["ok"], result)
        self.assertEqual(result["retailBytes"], 285)
        self.assertEqual(result["interfaceSlots"], 6)

    def test_checked_in_snapshot_reproduces_from_installed_exe(self):
        root = Path(__file__).resolve().parents[2]
        exe = Path(r"C:\Programs\Steam\steamapps\common\Fable The Lost Chapters\Fable.exe")
        if not exe.exists(): self.skipTest("installed retail executable is unavailable")
        actual = export(exe, root / "refs/script_recovery/new_oakvale_intro/entities/Q_NewOakValeIntro.json")
        expected = json.loads((root / "ghidra_out/script_recovery/new_oakvale_attack_stuff_retail_bytes.json").read_text(encoding="utf-8"))
        self.assertEqual(actual, expected)


if __name__ == "__main__": unittest.main()
