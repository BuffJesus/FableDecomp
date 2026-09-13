import unittest
from dataclasses import replace
from unittest.mock import patch
from capstone import Cs,CS_ARCH_X86,CS_MODE_32
from tools.script_recovery import test_native_affair_man_talk_continuation as fixtures
from tools.script_recovery.native_post_attack_resources import map_affair_man_resources
from tools.script_recovery.native_resource_lifetime import check_single_resource_lifetime
from tools.script_recovery.native_call_setup_ir import read_call_window


class AffairManResourceTests(unittest.TestCase):
    def test_ten_temporary_things_close_after_their_query(self):
        fn,data,evidence=self.mapped()
        self.assertEqual(evidence['temporaryThingStatus'],'ten getter/query/destructor lifetimes verified')
        things=evidence['temporaryThings']
        self.assertEqual(len(things),10)
        for thing in things:
            self.assertLess(thing['create'],thing['query'])
            self.assertLess(thing['query'],thing['destroy'])
            self.assertEqual(thing['destroySetup']['ecx'],thing['output'])
        self.assertEqual([t['query'] for t in things[-2:]],[0xdb1729,0xdb17d3])
        decoder=Cs(CS_ARCH_X86,CS_MODE_32)
        decoder.detail=True
        ins=list(decoder.disasm(data.bytes_at(0xdb09e0,5058),0xdb09e0))
        events={t[key]:(operation,tuple(t['output'])) for t in things
                for key,operation in (('create','start'),('query','use'),('destroy','end'))}
        self.assertTrue(check_single_resource_lifetime(ins,events))
        del events[things[-1]['destroy']]
        self.assertFalse(check_single_resource_lifetime(ins,events))

    def test_query_using_a_different_actor_rejects(self):
        fn,data,evidence=self.mapped()
        def changed(*args,**kwargs):
            setup=read_call_window(*args,**kwargs)
            return replace(setup,ecx=('register','ebp')) if args[3]==0xdb17d3 else setup
        with patch('tools.script_recovery.native_post_attack_resources.read_call_window',side_effect=changed):
            self.assertEqual(map_affair_man_resources(fn,data)[0]['status'],'rejected')

    def mapped(self):
        fn,source,data,manifest=fixtures.AffairManTalkContinuationTests().inputs()
        evidence=map_affair_man_resources(fn,data)[0]
        self.assertEqual(evidence['status'],'mapped')
        return fn,data,evidence

    def test_one_local_with_three_preparations_and_six_attempts(self):
        fn,data,evidence=self.mapped()
        all_events=evidence['events']
        self.assertEqual(len(all_events),57)
        events=[e for e in all_events if e['name'] in ('construct','has_resource','reset','acquire','destroy')]
        self.assertEqual([e['name'] for e in events],['construct']+['has_resource','reset','acquire','acquire']*3+['destroy'])
        for event in events:
            if event['name']=='acquire':
                self.assertEqual(event['setup']['stack_arguments'],[['register','ebp'],['stack',16],['constant',4]])
            else: self.assertEqual(event['setup']['ecx'],['stack',16])
        self.assertIn('paths verified',evidence['lifetimeStatus'])
        self.assertEqual(evidence['loweringStatus'],'unresolved')

    def test_actions_and_queries_share_resource_and_dynamic_animation_flag(self):
        fn,data,evidence=self.mapped()
        counts={}
        for event in evidence['events']:
            counts[event['name']]=counts.get(event['name'],0)+1
            if event['name']!='acquire': self.assertEqual(event['setup']['ecx'],['stack',16])
            if event['name']=='animation':
                self.assertEqual(event['setup']['stack_arguments'][5],['unsigned_byte',['memory',['constant',0x1375748]]])
        self.assertEqual({name:counts[name] for name in ('speak','task_query','animation','move','clear_actions','clear_commands','get_thing')},
            {'speak':8,'task_query':19,'animation':3,'move':1,'clear_actions':1,'clear_commands':1,'get_thing':10})

    def test_use_requires_constructed_matching_resource_on_every_path(self):
        fn,data,evidence=self.mapped()
        decoder=Cs(CS_ARCH_X86,CS_MODE_32)
        decoder.detail=True
        ins=list(decoder.disasm(data.bytes_at(0xdb09e0,5058),0xdb09e0))
        events={int(e['site'],16):('start' if e['name']=='construct' else 'end' if e['name']=='destroy' else 'use',('stack',16)) for e in evidence['events']}
        self.assertTrue(check_single_resource_lifetime(ins,events))
        missing_constructor=dict(events)
        del missing_constructor[0xdb0a3c]
        self.assertFalse(check_single_resource_lifetime(ins,missing_constructor))
        wrong_resource=dict(events)
        wrong_resource[0xdb0eb4]=('use',('stack',32))
        self.assertFalse(check_single_resource_lifetime(ins,wrong_resource))
        missing_destructor=dict(events)
        del missing_destructor[0xdb1d92]
        self.assertFalse(check_single_resource_lifetime(ins,missing_destructor))

    def test_changed_acquisition_receiver_rejects(self):
        fn,data,evidence=self.mapped()
        self.assertEqual(data.bytes_at(0xdb0a75,1),b'\x55')
        class Changed:
            def bytes_at(self,address,size):
                raw=data.bytes_at(address,size)
                if address==0xdb09e0:
                    raw=bytearray(raw)
                    raw[0xdb0a75-address]=0x56
                    return bytes(raw)
                return raw
            def string_at(self,address): return data.string_at(address)
        self.assertEqual(map_affair_man_resources(fn,Changed())[0]['status'],'rejected')
