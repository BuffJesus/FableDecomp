import unittest
from tools.script_recovery.live_father_prepare import prepare

class LiveFatherHostTests(unittest.TestCase):
    def test_actual_fse_types_lua_and_scoped_output_lifetimes(self):
        report=prepare();self.assertEqual(report['peMachine'],'0x014c (x86)');self.assertEqual([c['exitCode'] for c in report['commands']],[0,0,0]);self.assertIn('37 Init/amount',report['compiledTest'])

if __name__=='__main__':unittest.main()
