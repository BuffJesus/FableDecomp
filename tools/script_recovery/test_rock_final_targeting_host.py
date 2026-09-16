import unittest
from tools.script_recovery.rock_final_targeting_prepare import prepare


class FinalTargetingHostTests(unittest.TestCase):
    def test_actual_fse_lua_raw_targets_and_error_unwind(self):
        report=prepare()
        self.assertEqual(report['peMachine'],'0x014c (x86)')
        self.assertIn('12 raw borrowed-target Lua policies passed',report['compiledTest'])
        self.assertFalse(report['runtimeChanged'])
        self.assertTrue(all(command['exitCode']==0 for command in report['commands']))


if __name__=='__main__':unittest.main()
