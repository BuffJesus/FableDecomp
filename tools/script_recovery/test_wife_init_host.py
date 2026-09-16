import unittest
from tools.script_recovery.wife_init_prepare import prepare
class WifeInitHostTests(unittest.TestCase):
    def test_actual_fse_owner_and_lua_binding(self):
        report=prepare()
        self.assertIn('92 policies passed',report['compiledTest'])
        self.assertTrue(all(c['exitCode']==0 for c in report['commands']))
if __name__=='__main__':unittest.main()
