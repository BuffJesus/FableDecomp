import unittest
from tools.script_recovery.dead_father_prepare import prepare
class DeadFatherHostTests(unittest.TestCase):
    def test_actual_fse_lua_scoped_markers_and_unnormalized_animation_byte(self):
        report=prepare();self.assertEqual(report['peMachine'],'0x014c (x86)');self.assertEqual([c['exitCode'] for c in report['commands']],[0,0,0]);self.assertIn('91 policies passed',report['compiledTest']);self.assertIn('30 emitted Main scenarios',report['compiledTest'])
if __name__=='__main__':unittest.main()
