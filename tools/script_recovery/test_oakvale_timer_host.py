import unittest
from tools.script_recovery.oakvale_timer_host_checks import run
class OakvaleTimerHostTests(unittest.TestCase):
    def test_compiled_registry_allocator_and_transient_state(self):
        report=run();self.assertIn('28 actual-FSE timer policy',report['compiledTest'])
        self.assertEqual(report['peMachine'],'0x014c (x86)')
        self.assertTrue(all(c['exitCode']==0 for c in report['commands']))
if __name__=='__main__':unittest.main()
