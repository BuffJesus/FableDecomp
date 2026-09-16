import unittest
from tools.script_recovery.bully_presented_prepare import prepare

class BullyPresentedHostTests(unittest.TestCase):
    def test_actual_resource_scope_retained_output_movie_and_error_lifetimes(self):
        report=prepare()
        self.assertEqual(report['peMachine'],'0x014c (x86)')
        self.assertIn('4 retained-output/movie/error Lua policies passed',report['compiledTest'])
        self.assertFalse(report['runtimeChanged'])
        self.assertTrue(all(c['exitCode']==0 for c in report['commands']))
