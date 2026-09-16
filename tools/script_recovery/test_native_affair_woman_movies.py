import json
import unittest
from pathlib import Path
from unittest.mock import patch

from tools.script_recovery.lift_native_lua import ROOT,RData
from tools.script_recovery.native_affair_woman_movies import verify


class AffairWomanMoviesTests(unittest.TestCase):
    @classmethod
    def setUpClass(cls):
        unit=json.loads((ROOT/'refs/script_recovery/new_oakvale_intro/translation_unit.json').read_text())
        cls.function=next(f for f in unit['functions'] if int(f['address'],16)==0xDB1F00)

    def test_two_movies_and_all_normal_and_cancel_exits(self):
        w=verify(self.function,RData())
        self.assertEqual(len(w['events']),10)
        self.assertEqual(len(w['pauses']),8)
        self.assertEqual(sum(e['setup']['stack_arguments']==[['constant',1]] for e in w['pauses']),2)

    def test_omitted_cleanup_or_pause_rejects(self):
        path=Path(__file__).with_name('native_affair_woman_movies_witness.json')
        original=json.loads(path.read_text());read_text=Path.read_text
        for key in ('events','pauses'):
            for index in range(len(original[key])):
                changed=dict(original);changed[key]=original[key][:index]+original[key][index+1:]
                def read(p,*args,**kwargs):
                    return json.dumps(changed) if p.name==path.name else read_text(p,*args,**kwargs)
                with patch.object(Path,'read_text',read),self.assertRaisesRegex(ValueError,'coverage'):
                    verify(self.function,RData())
