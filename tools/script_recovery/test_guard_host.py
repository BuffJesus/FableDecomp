import unittest
from tools.script_recovery.guard_prepare import prepare

class GuardHostTests(unittest.TestCase):
    def test_actual_fse_types_real_lua_arguments_and_lifetimes(self):
        report=prepare()
        self.assertEqual(report['peMachine'],'0x014c (x86)')
        self.assertEqual([r['exitCode'] for r in report['commands']],[0,0,0])
        self.assertIn('20 Init copy',report['compiledTest'])
        self.assertIn('12 line lifetime/error',report['compiledTest'])
        self.assertIn('4 retained follow',report['compiledTest'])
        self.assertIn('unapplied',report['status'])

if __name__=='__main__':unittest.main()
