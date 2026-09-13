import json,unittest
from pathlib import Path
from tools.script_recovery.audit_affair_woman_full_bytes import audit
from tools.script_recovery.export_affair_woman_full_bytes import export
class AffairWomanFullByteAuditTests(unittest.TestCase):
 def test_snapshot_lua_inventory_agree(self):
  r=audit(Path(__file__).resolve().parents[2]);self.assertTrue(r['ok'],r);self.assertEqual(r['retailBytes'],2830)
 def test_snapshot_reproduces(self):
  root=Path(__file__).resolve().parents[2];exe=Path(r'C:\Programs\Steam\steamapps\common\Fable The Lost Chapters\Fable.exe')
  if not exe.exists():self.skipTest('installed retail executable unavailable')
  self.assertEqual(export(exe),json.loads((root/'ghidra_out/script_recovery/affair_woman_full_retail_bytes.json').read_text()))
if __name__=='__main__':unittest.main()
