import unittest
from tools.script_recovery.affair_man_complete_prepare import prepare
class AffairManCompleteHostTests(unittest.TestCase):
    def test_actual_fse_owner_and_real_lua(self):
        r=prepare();self.assertIn('76 policies passed',r['compiledTest']);self.assertTrue(all(c['exitCode']==0 for c in r['commands']))
if __name__=='__main__':unittest.main()
