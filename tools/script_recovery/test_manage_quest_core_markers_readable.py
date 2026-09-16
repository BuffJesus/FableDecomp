import tempfile
import unittest
from pathlib import Path
from tools.script_recovery.build_readable_new_oakvale import build,RAW
from tools.script_recovery.readable_manage_quest_core_markers import lower
from tools.script_recovery.test_manage_quest_core_markers import run
from tools.script_recovery.manage_quest_core_markers_native import execute


class ManageQuestCoreMarkersReadableTests(unittest.TestCase):
    def test_emitted_function_matches_native_wait_cleanup_and_marker_policy(self):
        with tempfile.TemporaryDirectory() as directory:
            report=build(Path(directory))
            source=(Path(directory)/'FSE/NewOakValeIntro/NewOakValeIntro.lua').read_text()
            self.assertTrue(report['syntax']['ok'])
            start=source.index('\nfunction ManageQuestCoreMarkers(')
            end=source.index('\nfunction StartBarrelTimer(',start)
            helper=source[start:end]
            for token in ('TODO(native)','goto ','LAB_','RegisterBoundConsciousCondition','pCVar'):
                self.assertNotIn(token,helper)
            for cancel in (1,2,4,8,12,20,99):
                for tutorial in (False,True):
                    for counts in ((0,0,0),(1,2,1)):
                        options=dict(cancel=cancel,tutorial=tutorial,counts=counts,
                                     populated=(False,True,False),delays=(2,1,2,1,2))
                        self.assertEqual(run(source=source,**options),execute(**options),options)

    def test_changed_source_rejected(self):
        source=(RAW/'FSE/NewOakValeIntro/NewOakValeIntro.lua').read_text()
        start=source.index('\nfunction ManageQuestCoreMarkers(')
        source=source[:start]+source[start:].replace('GetHeroGold()', 'GetHeroGold(1)',1)
        with self.assertRaises(ValueError):lower(source)


if __name__=='__main__':unittest.main()
