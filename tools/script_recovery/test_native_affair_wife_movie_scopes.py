import copy
import json
import unittest
from pathlib import Path
from unittest.mock import patch

from tools.script_recovery.lift_native_lua import ROOT,RData
from tools.script_recovery.native_affair_wife_movie_scopes import verify


class WifeMovieScopeTests(unittest.TestCase):
    @classmethod
    def setUpClass(cls):
        unit=json.loads((ROOT/'refs/script_recovery/new_oakvale_intro/translation_unit.json').read_text())
        cls.function=next(f for f in unit['functions'] if int(f['address'],16)==0xDB2B10)

    def test_four_movies_seventeen_pauses_and_shared_destructors(self):
        result=verify(self.function,RData())
        self.assertEqual(len(result['events']),15)
        self.assertEqual(len(result['pauses']),17)
        self.assertEqual([e['identity'] for e in result['events'] if e['name']=='construct'],
                         [['stack',164],['stack',64],['stack',64],['stack',116]])
        self.assertEqual(len(result['selections']),5)
        self.assertEqual(len(result['classStrings']),4)

    def test_every_omitted_movie_or_pause_rejects(self):
        path=Path(__file__).with_name('native_affair_wife_movie_scopes_witness.json')
        original=json.loads(path.read_text());read_text=Path.read_text
        for key in ('events','pauses'):
            for event in original[key]:
                altered=copy.deepcopy(original)
                altered[key]=[e for e in altered[key] if e['site']!=event['site']]
                def read(p,*args,**kwargs):
                    return json.dumps(altered) if p==path else read_text(p,*args,**kwargs)
                with self.subTest(site=hex(event['site'])),patch.object(Path,'read_text',read),self.assertRaisesRegex(ValueError,'coverage'):
                    verify(self.function,RData())

    def test_missing_or_wrong_shared_destructor_selection_rejects(self):
        path=Path(__file__).with_name('native_affair_wife_movie_scopes_witness.json')
        original=json.loads(path.read_text());read_text=Path.read_text
        variants=[]
        for site in original['selections']:
            changed=copy.deepcopy(original);del changed['selections'][site];variants.append(changed)
        changed=copy.deepcopy(original);changed['selections'][str(0xDB2E90)]=['stack',64];variants.append(changed)
        changed=copy.deepcopy(original);changed['classStrings'].pop();variants.append(changed)
        changed=copy.deepcopy(original);changed['classStrings'][0]['destroy']=0xDB2DA9;variants.append(changed)
        for altered in variants:
            def read(p,*args,**kwargs):
                return json.dumps(altered) if p==path else read_text(p,*args,**kwargs)
            with patch.object(Path,'read_text',read),self.assertRaises(ValueError):
                verify(self.function,RData())
