import json
import re
import unittest
from dataclasses import replace
from unittest.mock import patch

from lupa.lua54 import LuaRuntime
from tools.script_recovery.lift_native_lua import (ROOT, RData, Lifter, annotate,
    load_manifest, load_slots, load_thing_tables, fold_self_wrapper_arguments, strip_declarations)
from tools.script_recovery.native_function_parameters import function_parameters, rename_parameters
from tools.script_recovery.native_teddy_distance import recover_affair_man_distance, recover_affair_man_woman_distance
from tools.script_recovery.native_random import fold_retail_random
from tools.script_recovery.native_call_setup_ir import read_call_window


class AffairManDistanceTests(unittest.TestCase):
    def test_remaining_random_gates_use_retail_modulus_and_correct_branches(self):
        fn, source, manifest = self.inputs()
        data = RData()
        source, _ = recover_affair_man_distance(fn, source, data)
        source, _ = recover_affair_man_woman_distance(fn, source, data)
        recovered, evidence = fold_retail_random(source, fn, data.bytes_at)
        self.assertEqual([item['modulus'] for item in evidence], [100, 50, 2])
        self.assertNotIn('rand()', recovered)
        fifty = re.search(r'iVar10 = GSI->RetailRandModulo\(50\);\s*if \(iVar10 != 0\) goto LAB_00db1c71;', recovered)[0]
        parity = re.search(r'uVar14 = GSI->RetailRandModulo\(2\);\s*bVar4 = uVar14 == 0;', recovered)[0]
        for modulus, fragment in ((50, fifty + '\nreturn true;\nLAB_00db1c71:\nreturn false;'),
                                  (2, parity + '\nreturn bVar4;')):
            lifter = Lifter(manifest, {}, 'quest', True, '', data, native_gotos=True)
            body = '\n'.join(lifter.lift('RandomGate', '{\n' + fragment + '\n}'))
            self.assertEqual(lifter.todo, [])
            for remainder in range(modulus):
                lua, events = LuaRuntime(), []
                def random(_q, requested):
                    events.append(requested)
                    return remainder
                quest = lua.table_from({'RetailRandModulo': random})
                self.assertEqual(lua.execute('return function(quest)\n' + body + '\nend')(quest), remainder == 0)
                self.assertEqual(events, [modulus])
        changed = source.replace('iVar10 % 0x32', 'iVar10 % 0x31')
        result, rejected = fold_retail_random(changed, fn, data.bytes_at)
        self.assertEqual(result, changed)
        self.assertEqual(rejected[0]['status'], 'rejected')

    def test_actual_random_and_woman_gate_preserves_short_circuit_calls(self):
        fn, source, manifest = self.inputs()
        data = RData()
        source, _ = recover_affair_man_distance(fn, source, data)
        source, evidence = recover_affair_man_woman_distance(fn, source, data)
        self.assertEqual(evidence[0]['status'], 'recovered')
        source, random_evidence = fold_retail_random(source, fn, data.bytes_at)
        self.assertEqual(random_evidence[0]['status'], 'recovered')
        self.assertNotIn('rand()', source)
        lookup = re.search(r'GSI->GetThingWithScriptName\(&local_native_cached_woman[^;]+;', source)[0]
        gate = next(line for line in strip_declarations(source) if 'RetailRandModulo(100)' in line)
        gate = gate.replace("cVar5 != '\\0'", 'native_arg_conversation').replace(
            'param_1[0x1d] != (CWorldMap)0x0', 'native_arg_said')
        self.assertNotIn('param_1', gate)
        lifter = Lifter(manifest, {}, 'quest', True, '', data, native_gotos=True)
        body = '\n'.join(lifter.lift('Gate', '{\n' + lookup +
            '\npCVar1 = (CScriptThing *)(param_1 + 8);\n' + gate +
            '\nreturn true;\nLAB_00db1c71:\nreturn false;\n}',
            parameters={'native_arg_conversation': 'bool', 'native_arg_said': 'bool'}))
        self.assertEqual(lifter.todo, [])
        for conversation, said, remainder, near, expected, checks in (
                (True, True, 0, True, False, []),
                (False, False, 99, True, True, ['distance']),
                (False, True, 1, True, False, ['random']),
                (False, True, 0, True, True, ['random', 'distance']),
                (False, True, 0, False, False, ['random', 'distance'])):
            lua, events = LuaRuntime(), []
            def random(_q, modulus):
                self.assertEqual(modulus, 100)
                events.append('random')
                return remainder
            def distance(_q, actor, target, threshold):
                self.assertEqual((actor, target, threshold), ('man', 'woman', 2.0))
                events.append('distance')
                return near
            quest = lua.table_from({'GetThingWithScriptName': lambda _q, name: 'woman' if name == 'NOVI_AffairWoman' else 'wrong',
                                    'RetailRandModulo': random, 'IsDistanceBetweenThingsUnder': distance})
            run = lua.execute('return function(quest,me,native_arg_conversation,native_arg_said)\n' + body + '\nend')
            self.assertEqual(run(quest, 'man', conversation, said), expected)
            self.assertEqual(events, checks)

    def inputs(self):
        unit = json.loads((ROOT / 'refs/script_recovery/new_oakvale_intro/translation_unit.json').read_text())
        fn = next(f for f in unit['functions'] if f['address'] == '0x00DB09E0')
        manifest, slots = load_manifest(), load_slots()
        things, returning = load_thing_tables(manifest, slots)
        source = annotate(rename_parameters(fn['decompile'], function_parameters(fn['decompile'], member=True)),
                          slots, things, returning, entity=True)
        return fn, fold_self_wrapper_arguments(source)[0], manifest

    def test_two_native_stack_names_refer_to_same_cached_wife(self):
        fn, source, manifest = self.inputs()
        recovered, evidence = recover_affair_man_distance(fn, source, RData())
        self.assertEqual(evidence[0]['status'], 'recovered')
        lookup = re.search(r'GSI->GetThingWithScriptName\(&local_native_cached_wife[^;]+;', recovered)[0]
        self.assertLess(recovered.index('GSI->GetThingWithScriptName();'), recovered.index(lookup))
        calls = re.findall(r'bVar4 = IsDistanceBetweenThingsUnder\(pCVar1,local_native_cached_wife,5.0\);', recovered)
        self.assertEqual(len(calls), 2)
        lifter = Lifter(manifest, {}, 'quest', True, '', RData())
        body = '\n'.join(lifter.lift('Checks', '{\n' + lookup +
            '\npCVar1 = (CScriptThing *)(param_1 + 8);\n' + '\n'.join(calls) + '\nreturn bVar4;\n}'))
        self.assertEqual(lifter.todo, [])
        for answer in (False, True):
            lua, events = LuaRuntime(), []
            def lookup_fn(_q, name):
                events.append(('lookup', name))
                return 'cached_wife'
            def distance(_q, actor, target, threshold):
                events.append(('distance', actor, target, threshold))
                return answer
            quest = lua.table_from({'GetThingWithScriptName': lookup_fn, 'IsDistanceBetweenThingsUnder': distance})
            self.assertEqual(lua.execute('return function(quest,me)\n' + body + '\nend')(quest, 'affair_man'), answer)
            self.assertEqual(events, [('lookup', 'NOVI_AffairWife')] +
                             [('distance', 'affair_man', 'cached_wife', 5.0)] * 2)

    def test_changed_native_receiver_or_stack_slot_rejects_rebinding(self):
        fn, source, _ = self.inputs()
        for site, change in ((0xDB0B07, {'stack_arguments': (('stack', 32), ('stack', 64))}),
                             (0xDB162D, {'ecx': ('register', 'edi')})):
            def decode(*args, **kwargs):
                setup = read_call_window(*args, **kwargs)
                return replace(setup, **change) if args[3] == site else setup
            with patch('tools.script_recovery.native_teddy_distance.read_call_window', side_effect=decode):
                result, evidence = recover_affair_man_distance(fn, source, RData())
                self.assertEqual(result, source)
                self.assertEqual(evidence[0]['status'], 'rejected')


if __name__ == '__main__':
    unittest.main()
