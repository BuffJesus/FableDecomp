import json, unittest
from pathlib import Path
from tools.script_recovery.audit_affair_man_full_bytes import audit
from tools.script_recovery.export_affair_man_full_bytes import export

class AffairManFullByteAuditTests(unittest.TestCase):
    def test_snapshot_lua_inventory_agree(self):
        result=audit(Path(__file__).resolve().parents[2]); self.assertTrue(result['ok'],result); self.assertEqual(result['retailBytes'],5202)
    def test_snapshot_reproduces_from_installed_exe(self):
        root=Path(__file__).resolve().parents[2]; exe=Path(r'C:\Programs\Steam\steamapps\common\Fable The Lost Chapters\Fable.exe')
        if not exe.exists(): self.skipTest('installed retail executable unavailable')
        expected=json.loads((root/'ghidra_out/script_recovery/affair_man_full_retail_bytes.json').read_text(encoding='utf-8'))
        self.assertEqual(export(exe),expected)
if __name__=='__main__': unittest.main()
