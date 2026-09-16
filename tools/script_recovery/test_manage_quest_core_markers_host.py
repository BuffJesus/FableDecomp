import unittest
from tools.script_recovery.manage_quest_core_markers_prepare import prepare
class ManageQuestCoreMarkersHostTests(unittest.TestCase):
    def test_actual_fse_types_real_lua_and_owner_cleanup(self):
        r=prepare();self.assertIn('128 full-helper policies passed',r['compiledTest'])
        self.assertEqual(r['peMachine'],'0x014c (x86)');self.assertTrue(all(c['exitCode']==0 for c in r['commands']))
if __name__=='__main__':unittest.main()
