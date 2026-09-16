import unittest
from tools.script_recovery.teddy_girl_prepare import prepare

class TeddyGirlHostTests(unittest.TestCase):
    def test_actual_fse_types_real_lua_and_x86_ownership(self):
        report=prepare()
        self.assertEqual(report['peMachine'],'0x014c (x86)')
        self.assertEqual([row['exitCode'] for row in report['commands']],[0,0,0])
        self.assertIn('96 policies passed',report['compiledTest'])
        self.assertIn('unapplied',report['status'])

if __name__=='__main__':unittest.main()
