import unittest
from tools.script_recovery.rock_exhumation_prepare import prepare


class ExhumationHostTests(unittest.TestCase):
    def test_actual_fse_lua_resource_movie_and_error_policies(self):
        report=prepare()
        self.assertEqual(report['peMachine'],'0x014c (x86)')
        self.assertIn('32 real-Lua map/movie/resource policies passed',report['compiledTest'])
        self.assertFalse(report['runtimeChanged'])
        self.assertTrue(all(command['exitCode']==0 for command in report['commands']))


if __name__=='__main__':unittest.main()
