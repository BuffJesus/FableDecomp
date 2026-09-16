import itertools
import tempfile
import unittest
from pathlib import Path
from tools.script_recovery.build_readable_new_oakvale import build,RAW
from tools.script_recovery.readable_post_attack_cutscene import lower as lower_movie
from tools.script_recovery.readable_post_attack_body import lower
from tools.script_recovery.test_post_attack_dispatcher import native,lua_trace


class PostAttackBodyReadableTests(unittest.TestCase):
    def test_emitted_dispatcher_matches_native_boundaries(self):
        with tempfile.TemporaryDirectory() as directory:
            report=build(Path(directory))
            source=(Path(directory)/'FSE/NewOakValeIntro/NewOakValeIntro.lua').read_text()
            self.assertTrue(report['syntax']['ok'])
            self.assertEqual(report['postAttackBody']['implementation'],'runPostAttack')
            self.assertIn('Quests = {}',(Path(directory)/'FSE/quests.lua').read_text())
            start=source.index('local function runPostAttack(')
            end=source.index('function ManageQuestCoreMarkers(',start)
            for token in ('TODO(native)','goto ','LAB_','pCVar','RegisterBoundConsciousCondition'):
                self.assertNotIn(token,source[start:end])
            for first,near,cancel in itertools.product((0,1,3),(0,1,3),range(1,13)):
                self.assertEqual(lua_trace(first,near,cancel,source),native(first,near,cancel),(first,near,cancel))

    def test_changed_dispatcher_draft_rejected(self):
        source=(RAW/'FSE/NewOakValeIntro/NewOakValeIntro.lua').read_text()
        source,_=lower_movie(source)
        with self.assertRaisesRegex(ValueError,'dispatcher draft changed'):
            lower(source.replace('function PostAttackStuff(quest)\n','function PostAttackStuff(quest)\n    quest:NewScriptFrame()\n'))


if __name__=='__main__':unittest.main()
