import copy
import json
import unittest
from pathlib import Path
from unittest.mock import patch

from tools.script_recovery.lift_native_lua import ROOT,RData
from tools.script_recovery.native_affair_wife_actor_scope import verify,USES,DESTROYS


class WifeActorScopeTests(unittest.TestCase):
    @classmethod
    def setUpClass(cls):
        unit=json.loads((ROOT/'refs/script_recovery/new_oakvale_intro/translation_unit.json').read_text())
        cls.function=next(f for f in unit['functions'] if int(f['address'],16)==0xDB2B10)

    def test_retained_husband_has_eight_uses_and_five_cleanup_joins(self):
        result=verify(self.function,RData())
        self.assertEqual(result['uses'],USES)
        self.assertEqual(result['destroys'],DESTROYS)
        self.assertEqual(result['output'],['stack',48])

    def test_missing_use_cleanup_and_wrong_actor_reject(self):
        path=Path(__file__).with_name('native_affair_wife_actor_scope_witness.json')
        original=json.loads(path.read_text());read_text=Path.read_text
        variants=[]
        for key in ('uses','destroys'):
            for site in original[key]:
                changed=copy.deepcopy(original);changed[key].remove(site);variants.append(changed)
        changed=copy.deepcopy(original);changed['name']='NOVI_AffairWoman';variants.append(changed)
        changed=copy.deepcopy(original);changed['calls'].pop();variants.append(changed)
        for altered in variants:
            def read(p,*args,**kwargs):
                return json.dumps(altered) if p==path else read_text(p,*args,**kwargs)
            with patch.object(Path,'read_text',read),self.assertRaises(ValueError):
                verify(self.function,RData())
