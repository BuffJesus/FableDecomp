import tempfile
import unittest
from pathlib import Path
from tools.script_recovery.build_readable_new_oakvale import RAW,build
from tools.script_recovery.readable_oakvale_lifecycle import lower


class ReadableOakvaleLifecycleTests(unittest.TestCase):
    def test_native_inventory_points_to_host_destructor(self):
        with tempfile.TemporaryDirectory() as directory:
            report=build(Path(directory))
            source=(Path(directory)/'FSE/NewOakValeIntro/NewOakValeIntro.lua').read_text()
            self.assertNotIn('function destructor(',source)
            self.assertNotIn('return this',source)
            self.assertNotIn('CParticleEmitter_Dtor_7',source)
            row=next(r for r in report['functions'] if r['function']=='destructor' and r['path'].endswith('/NewOakValeIntro.lua'))
            self.assertEqual(row['implementationFunction'],'LuaQuestHost::Destructor')
            self.assertEqual(row['implementationLanguage'],'C++')
            self.assertEqual(report['hostLifecycle']['requiredNativeLifetime'],'NewOakValeIntro')
            self.assertTrue(report['syntax']['ok'])
            self.assertIn('Quests = {}',(Path(directory)/'FSE/quests.lua').read_text())

    def test_changed_raw_destructor_rejected(self):
        source=(RAW/'FSE/NewOakValeIntro/NewOakValeIntro.lua').read_text()
        with self.assertRaisesRegex(ValueError,'destructor draft changed'):
            lower(source.replace('return this','return anotherOwner'))


if __name__=='__main__':unittest.main()
