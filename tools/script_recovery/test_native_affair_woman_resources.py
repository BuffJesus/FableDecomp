import json
import copy
import unittest
from pathlib import Path
from types import SimpleNamespace
from unittest.mock import patch

from tools.script_recovery.lift_native_lua import ROOT,RData
from tools.script_recovery.native_affair_woman_resources import verify


class AffairWomanResourcesTests(unittest.TestCase):
    @classmethod
    def setUpClass(cls):
        unit=json.loads((ROOT/'refs/script_recovery/new_oakvale_intro/translation_unit.json').read_text())
        cls.function=next(f for f in unit['functions'] if int(f['address'],16)==0xDB1F00)

    def test_complete_single_resource_lifetime(self):
        result=verify(self.function,RData())
        self.assertEqual(len(result['events']),39)
        self.assertEqual([e['site'] for e in result['events'] if e['operation']=='start'],[0xDB1F5C])
        self.assertEqual([e['site'] for e in result['events'] if e['operation']=='end'],[0xDB298A])
        self.assertEqual(sum(e['name']=='get_thing' for e in result['events']),5)

    def test_omitted_event_and_changed_native_receiver_or_exit_reject(self):
        path=Path(__file__).with_name('native_affair_woman_resources_witness.json')
        original=json.loads(path.read_text()); read_text=Path.read_text
        for site in (0xDB2376,0xDB280B,0xDB298A):
            altered=dict(original,events=[e for e in original['events'] if e['site']!=site])
            def read(p,*args,**kwargs):
                return json.dumps(altered) if p.name==path.name else read_text(p,*args,**kwargs)
            with patch.object(Path,'read_text',read),self.assertRaisesRegex(ValueError,'coverage'):
                verify(self.function,RData())
        data=RData()
        for site in (0xDB1F94,0xDB2800,0xDB2986):
            def read(address,size):
                raw=data.bytes_at(address,size)
                if raw is not None and address<=site<address+size:
                    raw=bytearray(raw);raw[site-address]^=1;return bytes(raw)
                return raw
            with self.assertRaises(ValueError):
                verify(self.function,SimpleNamespace(bytes_at=read))

    def test_missing_temporary_wrong_output_and_early_destruction_reject(self):
        path=Path(__file__).with_name('native_affair_woman_resources_witness.json')
        original=json.loads(path.read_text()); read_text=Path.read_text
        variants=[]
        changed=copy.deepcopy(original);changed['temporaryThings'].pop();variants.append(changed)
        changed=copy.deepcopy(original);changed['temporaryThings'][0]['output']=['stack',136];variants.append(changed)
        changed=copy.deepcopy(original);changed['temporaryThings'][0]['destroy']=0xDB20D2;variants.append(changed)
        for changed in variants:
            def read(p,*args,**kwargs):
                return json.dumps(changed) if p.name==path.name else read_text(p,*args,**kwargs)
            with patch.object(Path,'read_text',read),self.assertRaises(ValueError):
                verify(self.function,RData())
