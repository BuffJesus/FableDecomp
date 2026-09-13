import json,unittest
from pathlib import Path
from tools.script_recovery.audit_barrel_man_text_assets import audit
from tools.script_recovery.export_barrel_man_text_assets import export

class BarrelManTextAssetTests(unittest.TestCase):
 def test_lua_selects_exact_installed_scold_group(self):
  result=audit(Path(__file__).resolve().parents[2]);self.assertTrue(result['ok'],result)
 def test_snapshot_reproduces_from_installed_bank(self):
  root=Path(__file__).resolve().parents[2]
  bank=Path(r'C:\Programs\Steam\steamapps\common\Fable The Lost Chapters\data\lang\English\text.big')
  if not bank.exists():self.skipTest('installed English text.big unavailable')
  expected=json.loads((root/'ghidra_out/script_recovery/barrel_man_text_assets.json').read_text(encoding='utf-8-sig'))
  self.assertEqual(export(bank),expected)

if __name__=='__main__':unittest.main()
