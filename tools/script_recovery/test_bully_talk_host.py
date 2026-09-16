import unittest
from tools.script_recovery.bully_talk_prepare import prepare

class BullyTalkHostTests(unittest.TestCase):
    def test_actual_types_lua_string_overlap_null_hero_and_error_unwind(self):
        report=prepare()
        self.assertEqual(report['peMachine'],'0x014c (x86)')
        self.assertIn('24 CString/raw-Hero Lua policies passed',report['compiledTest'])
        self.assertFalse(report['runtimeChanged'])
        self.assertTrue(all(c['exitCode']==0 for c in report['commands']))
