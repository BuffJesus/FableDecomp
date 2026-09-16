import unittest
from tools.script_recovery.rock_reward_prepare import prepare


class RewardAdapterTests(unittest.TestCase):
    def test_actual_fse_types_lua_dynamic_values_and_cleanup(self):
        report=prepare()
        self.assertEqual(report['peMachine'],'0x014c (x86)')
        self.assertFalse(report['runtimeChanged'])
        self.assertIn('36 real-Lua lifetime policies and 2 input guards passed',report['compiledTest'])
        self.assertTrue(all(command['exitCode']==0 for command in report['commands']))


if __name__=='__main__':unittest.main()
