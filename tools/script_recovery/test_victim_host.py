import unittest
from tools.script_recovery.victim_prepare import prepare
class VictimHostTests(unittest.TestCase):
    def test_actual_fse_lua_retained_and_copied_thing_policies(self):
        report=prepare();self.assertEqual(report['peMachine'],'0x014c (x86)');self.assertEqual([c['exitCode'] for c in report['commands']],[0,0,0]);self.assertIn('56 Init/release',report['compiledTest'])
if __name__=='__main__':unittest.main()
