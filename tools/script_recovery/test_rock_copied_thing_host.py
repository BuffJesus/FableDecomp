import unittest
from tools.script_recovery.rock_copied_thing_prepare import prepare
from tools.script_recovery.rock_actor_dispatch_native import execute

class CopiedThingHostTests(unittest.TestCase):
    def test_actual_fse_lua_copy_lifetime_error_and_escape(self):
        for empty in (False,True):
            for info in (False,True):
                self.assertEqual(execute(empty,info),[
                    ('call.by.value',empty,info),('dispatch.complete',),
                    ('stored.argument.destroy',),('thread.base.destroy',)])
        report=prepare()
        self.assertEqual(report['peMachine'],'0x014c (x86)')
        self.assertIn('24 ownership Lua policies passed',report['compiledTest'])
        self.assertFalse(report['runtimeChanged'])
        self.assertTrue(all(c['exitCode']==0 for c in report['commands']))
