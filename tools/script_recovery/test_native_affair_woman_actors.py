import copy
import json
import unittest
from pathlib import Path
from types import SimpleNamespace
from unittest.mock import patch

from tools.script_recovery.lift_native_lua import ROOT,RData
from tools.script_recovery.native_affair_woman_actors import verify


class AffairWomanActorsTests(unittest.TestCase):
    @classmethod
    def setUpClass(cls):
        unit=json.loads((ROOT/'refs/script_recovery/new_oakvale_intro/translation_unit.json').read_text())
        cls.function=next(f for f in unit['functions'] if int(f['address'],16)==0xDB1F00)

    def test_retained_partner_and_marker_lifetimes(self):
        w=verify(self.function,RData())
        self.assertEqual([a['output'] for a in w['actors']],[['stack',48],['stack',36],['stack',72]])
        self.assertEqual(w['hitScope']['predicate'],'directHeroHit or (anyHeroAbility and not excludedHeroAbility14)')

    def test_wrong_partner_omitted_use_or_destructor_reject(self):
        path=Path(__file__).with_name('native_affair_woman_actors_witness.json')
        original=json.loads(path.read_text());read_text=Path.read_text
        variants=[]
        changed=copy.deepcopy(original);changed['actors'][0]['output']=['stack',36];variants.append(changed)
        changed=copy.deepcopy(original);changed['actors'][1]['uses'].pop();variants.append(changed)
        changed=copy.deepcopy(original);changed['actors'][2]['destroy']=0xDB2981;variants.append(changed)
        for changed in variants:
            def read(p,*args,**kwargs):
                return json.dumps(changed) if p.name==path.name else read_text(p,*args,**kwargs)
            with patch.object(Path,'read_text',read),self.assertRaises(ValueError):verify(self.function,RData())
        data=RData()
        with self.assertRaisesRegex(ValueError,'name|key'):
            verify(self.function,SimpleNamespace(bytes_at=data.bytes_at,string_at=lambda address:'WRONG'))
