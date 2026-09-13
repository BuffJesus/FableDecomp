import json
import unittest
from pathlib import Path

from tools.script_recovery.audit_new_oakvale_watch_barrels import audit
from tools.script_recovery.export_new_oakvale_watch_barrels import export


class NewOakValeWatchBarrelsAuditTests(unittest.TestCase):
    def test_retail_lua_and_interface_catalog_agree(self):
        root = Path(__file__).resolve().parents[2]
        result = audit(root)
        self.assertTrue(result["ok"], result)
        self.assertEqual(result["retailBytes"], 647)

    def test_checked_in_snapshot_reproduces_from_installed_exe(self):
        root = Path(__file__).resolve().parents[2]
        exe = Path(r"C:\Programs\Steam\steamapps\common\Fable The Lost Chapters\Fable.exe")
        if not exe.exists(): self.skipTest("installed retail executable is unavailable")
        actual = export(exe, root / "refs/script_recovery/new_oakvale_intro/entities/Q_NewOakValeIntro.json")
        expected = json.loads((root / "ghidra_out/script_recovery/new_oakvale_watch_barrels_retail_bytes.json").read_text(encoding="utf-8"))
        self.assertEqual(actual, expected)

    def test_final_gold_trace_brackets_native_insertion(self):
        root = Path(__file__).resolve().parents[2]
        trace = json.loads((root / "refs/script_recovery/new_oakvale_intro/traces/quest_thread_watch_barrels.json").read_text())
        messages = [event["arguments"][0] for event in trace["events"] if event.get("name") == "Log"]
        armed = next(i for i, message in enumerate(messages) if "GOLD_ARM broken=5 total=6" in message)
        returned = next(i for i, message in enumerate(messages) if "GOLD_INSERT_RETURNED item=OBJECT_GOLD_1" in message)
        self.assertLess(armed, returned)
        self.assertIn("container_present=true", messages[armed])
        self.assertIn("container_present=true", messages[returned])


if __name__ == "__main__": unittest.main()
