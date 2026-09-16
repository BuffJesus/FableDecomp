import unittest
from tools.script_recovery.rock_lifecycle_prepare import prepare

class LifecycleHostTests(unittest.TestCase):
    def test_actual_fse_lua_empty_actor_scope_and_error_cleanup(self):
        report=prepare()
        self.assertEqual(report['peMachine'],'0x014c (x86)')
        self.assertIn('6 bound-Thing Lua policies passed',report['compiledTest'])
        self.assertFalse(report['runtimeChanged'])
        self.assertTrue(all(c['exitCode']==0 for c in report['commands']))
