import copy
import json
import unittest
from pathlib import Path
from types import SimpleNamespace
from unittest.mock import patch

from tools.script_recovery.lift_native_lua import ROOT, RData
from tools.script_recovery.native_affair_wife_resource_scope import verify


class WifeResourceScopeTests(unittest.TestCase):
    @classmethod
    def setUpClass(cls):
        unit=json.loads((ROOT/'refs/script_recovery/new_oakvale_intro/translation_unit.json').read_text())
        cls.function=next(f for f in unit['functions'] if int(f['address'],16)==0xDB2B10)

    def test_all_resource_calls_and_constructed_exits(self):
        result=verify(self.function,RData())
        self.assertEqual(len(result['events']),60)
        self.assertEqual([e['site'] for e in result['events'] if e['operation']=='start'],[0xDB2B6C])
        self.assertEqual(sum(e['operation']=='end' for e in result['events']),7)
        self.assertEqual(sum(e['name']=='get_thing' for e in result['events']),6)
        priorities=[e['setup']['stack_arguments'][2][1] for e in result['events'] if e['name']=='acquire']
        self.assertEqual(priorities,[3,3]+[4]*8)
        self.assertEqual(len(result['temporaryThings']),6)

    def test_omitted_temporary_and_early_destroy_reject(self):
        path=Path(__file__).with_name('native_affair_wife_resource_scope_witness.json')
        original=json.loads(path.read_text());read_text=Path.read_text
        variants=[]
        changed=copy.deepcopy(original);changed['temporaryThings'].pop();variants.append(changed)
        changed=copy.deepcopy(original);changed['temporaryThings'][0]['destroy']=0xDB2DED;variants.append(changed)
        changed=copy.deepcopy(original);changed['temporaryThings'][0]['output']=['stack',16];variants.append(changed)
        for altered in variants:
            def read(p,*args,**kwargs):
                return json.dumps(altered) if p==path else read_text(p,*args,**kwargs)
            with patch.object(Path,'read_text',read),self.assertRaises(ValueError):
                verify(self.function,RData())

    def test_every_omitted_event_rejects(self):
        path=Path(__file__).with_name('native_affair_wife_resource_scope_witness.json')
        original=json.loads(path.read_text());read_text=Path.read_text
        for event in original['events']:
            altered=copy.deepcopy(original)
            altered['events']=[e for e in altered['events'] if e['site']!=event['site']]
            def read(p,*args,**kwargs):
                return json.dumps(altered) if p==path else read_text(p,*args,**kwargs)
            with self.subTest(site=hex(event['site'])),patch.object(Path,'read_text',read),self.assertRaisesRegex(ValueError,'coverage'):
                verify(self.function,RData())

    def test_changed_resource_receiver_inline_cleanup_and_base_destructor_reject(self):
        data=RData()
        for site in (0xDB2B68,0xDB2BC1,0xDB33B5,0xDB33F7,0xDB3E1A,0x99A430):
            def read(address,size):
                raw=data.bytes_at(address,size)
                if raw is not None and address<=site<address+size:
                    raw=bytearray(raw);raw[site-address]^=1;return bytes(raw)
                return raw
            with self.subTest(site=hex(site)),self.assertRaises(ValueError):
                verify(self.function,SimpleNamespace(bytes_at=read))
