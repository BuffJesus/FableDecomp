import unittest
from tools.script_recovery.teddy_girl_owner_prepare import prepare

class TeddyGirlOwnerTests(unittest.TestCase):
    def test_full_candidate_merged_owner_real_lua_x86(self):
        report=prepare()
        self.assertEqual(report['peMachine'],'0x014c (x86)')
        self.assertEqual([row['exitCode'] for row in report['commands']],[0,0])
        self.assertIn('14 full-candidate',report['compiledTest'])
        self.assertIn('unapplied',report['status'])

if __name__=='__main__':unittest.main()
