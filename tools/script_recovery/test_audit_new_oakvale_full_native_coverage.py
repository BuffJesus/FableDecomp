import unittest
from pathlib import Path
from tools.script_recovery.audit_new_oakvale_full_native_coverage import audit

class FullNativeCoverageAuditTests(unittest.TestCase):
 def test_quest_and_all_entities_are_covered_from_one_retail_executable(self):
  result=audit(Path(__file__).resolve().parents[2]);self.assertTrue(result['ok'],result);self.assertEqual(result['inventories'],17);self.assertEqual(result['retailBytes'],78891);self.assertEqual(result['instructions'],23388)

if __name__=='__main__':unittest.main()
