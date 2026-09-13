import unittest
import json
from pathlib import Path

class TranslationUnitBodyRangeExporterTests(unittest.TestCase):
 def test_exporter_distinguishes_body_count_from_extent(self):
  root=Path(__file__).resolve().parents[2]
  source=(root/'tools/ghidra_scripts/ExportScriptTranslationUnit.java').read_text(encoding='utf-8')
  for token in ('bodyAddressCount','bodyMin','bodyMaxInclusive','bodyEndExclusive','bodyExtent','bodyRanges','getAddressRanges()','fable-script-translation-unit/0.2'):
   self.assertIn(token,source)
  self.assertIn('\\"size\\":',source)  # retained as the legacy body-address-count alias

 def test_checked_in_translation_unit_uses_range_aware_schema(self):
  root=Path(__file__).resolve().parents[2]
  artifact=json.loads((root/'refs/script_recovery/new_oakvale_intro/translation_unit.json').read_text(encoding='utf-8-sig'))
  self.assertEqual(artifact['schema'],'fable-script-translation-unit/0.2')
  self.assertEqual(artifact['functionCount'],100)
  bully=next(row for row in artifact['functions'] if row['address']=='0x00DBB310')
  self.assertEqual(bully['size'],bully['bodyAddressCount'])
  self.assertEqual((bully['bodyAddressCount'],bully['bodyExtent'],len(bully['bodyRanges'])),(6630,6639,4))
  self.assertEqual(bully['bodyEndExclusive'],'0x00DBCCFF')

if __name__=='__main__':unittest.main()
