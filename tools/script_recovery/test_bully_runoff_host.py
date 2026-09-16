import unittest
from tools.script_recovery.bully_runoff_prepare import prepare

class BullyRunoffHostTests(unittest.TestCase):
    def test_actual_fse_x86_lua_map_and_retained_thing_adapter(self):
        report=prepare()
        self.assertEqual(report['peMachine'],'0x014c (x86)')
        self.assertEqual([x['exitCode'] for x in report['commands']],[0,0,0])
        self.assertIn('4 string-map/empty-Thing/movie/error Lua policies passed',report['compiledTest'])
