import itertools
import tempfile
import unittest
from pathlib import Path
from tools.script_recovery.build_readable_new_oakvale import RAW,build
from tools.script_recovery.readable_oakvale_deeds import lower
from tools.script_recovery.test_oakvale_deed_helpers import run
from tools.script_recovery.native_oakvale_deed_helpers import execute


class ReadableOakvaleDeedsTests(unittest.TestCase):
    def test_both_emitted_entry_shapes_match_original_helper(self):
        with tempfile.TemporaryDirectory() as directory:
            report=build(Path(directory));root=Path(directory)/'FSE/NewOakValeIntro'
            quest=(root/'NewOakValeIntro.lua').read_text()
            shared=(root/'native_quest_helpers.lua').read_text()
            self.assertTrue(report['syntax']['ok']);self.assertEqual(set(report['deedHelpers']),{'quest','shared'})
            self.assertNotIn('GiveHeroMorality(0.001',shared)
            quest+='\nreturn {good=AddGoodDeed,bad=function(q,me,deed) AddBadDeed(q,deed) end}'
            for source,kind,cancel,counts in itertools.product((quest,shared),('good','bad'),(1,2,4,99),((0,0),(2,1),(2147483647,2147483647))):
                options=dict(cancel=cancel,good=counts[0],bad=counts[1],delay=1,amount=.25)
                self.assertEqual(run(kind,source=source,**options),execute(kind,**options),(kind,options))

    def test_changed_deed_source_rejected(self):
        for shared,filename in ((True,'native_quest_helpers.lua'),(False,'NewOakValeIntro.lua')):
            source=(RAW/'FSE/NewOakValeIntro'/filename).read_text()
            with self.assertRaisesRegex(ValueError,'deed draft changed'):
                lower(source.replace('GiveHeroMorality(-0.0010000000474974513)','GiveHeroMorality(0)'),shared)


if __name__=='__main__':unittest.main()
