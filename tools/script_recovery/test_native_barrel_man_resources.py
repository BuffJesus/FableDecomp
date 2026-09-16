import copy
import json
import unittest
from pathlib import Path
from types import SimpleNamespace
from capstone import Cs,CS_ARCH_X86,CS_MODE_32
from tools.script_recovery.lift_native_lua import RData,ROOT
from tools.script_recovery.native_barrel_man_resources import verify
from tools.script_recovery.native_bounded_switch import resolve
from tools.script_recovery.native_resource_lifetime import check_single_resource_lifetime


class BarrelResourceTests(unittest.TestCase):
    @classmethod
    def setUpClass(cls):
        cls.data=RData()
        cls.function=next(f for f in json.loads((ROOT/'refs/script_recovery/new_oakvale_intro/translation_unit.json').read_text())['functions'] if int(f['address'],16)==0xDB5330)
        cls.witness=json.loads(Path(__file__).with_name('native_barrel_man_resources_witness.json').read_text())

    def test_complete_lifetime_and_each_omitted_event_rejects(self):
        self.assertEqual(len(verify(self.function,self.data)['events']),66)
        for index in range(66):
            changed=copy.deepcopy(self.witness);changed['events'].pop(index)
            with self.subTest(index=index),self.assertRaises(ValueError):verify(self.function,self.data,changed)

    def test_missing_cleanup_and_unknown_switch_paths_reject(self):
        w=self.witness;decoder=Cs(CS_ARCH_X86,CS_MODE_32);decoder.detail=True
        instructions=list(decoder.disasm(self.data.bytes_at(w['address'],w['size']),w['address']))
        events={e['site']:(e['operation'],('stack',20)) for e in w['events']}
        branches=resolve(instructions,self.data)
        self.assertFalse(check_single_resource_lifetime(instructions,events))
        for site in (0xDB69FE,0xDB6B13):
            changed=dict(events);del changed[site]
            self.assertFalse(check_single_resource_lifetime(instructions,changed,indirect_branches=branches))
        for target in (0xDB6B18,0xDB6B19,0xDB5331):
            self.assertFalse(check_single_resource_lifetime(instructions,events,indirect_branches={0xDB5526:(target,)}))

    def test_mutated_table_guard_and_resource_bytes_reject(self):
        for site in (0xDB6B24,0xDB551F,0xDB538B,0xDB69F6,0xDB5FA0):
            def read(address,size):
                raw=self.data.bytes_at(address,size)
                if raw and address<=site<address+size:
                    raw=bytearray(raw);raw[site-address]^=1;return bytes(raw)
                return raw
            with self.subTest(site=hex(site)),self.assertRaises(ValueError):
                verify(self.function,SimpleNamespace(bytes_at=read))
