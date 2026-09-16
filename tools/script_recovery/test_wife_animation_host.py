import unittest
from tools.script_recovery.wife_animation_prepare import prepare
class WifeAnimationHostTests(unittest.TestCase):
    def test_actual_fse_types_real_lua_binding_and_owner_cleanup(self):
        r=prepare();self.assertIn('32 policies passed',r['compiledTest']);self.assertTrue(all(c['exitCode']==0 for c in r['commands']))
if __name__=='__main__':unittest.main()
