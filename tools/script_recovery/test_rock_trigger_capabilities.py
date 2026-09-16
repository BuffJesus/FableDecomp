import unittest
from tools.script_recovery.rock_trigger_prepare_proposal import prepare


class RockTriggerCapabilities(unittest.TestCase):
    def test_actual_runtime_types_and_scoped_lua_consumers(self):
        report=prepare()
        self.assertFalse(report['runtimeChanged'])
        self.assertTrue(report['compiledTest'].startswith('PASS: live proximity'))
        self.assertTrue(all(item['exitCode']==0 for item in report['commands']))


if __name__=='__main__':unittest.main()
