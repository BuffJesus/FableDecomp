import json
import unittest
from dataclasses import replace
from unittest.mock import patch

from tools.script_recovery.lift_native_lua import (ROOT, RData, annotate, load_manifest,
    load_slots, load_thing_tables, fold_self_wrapper_arguments)
from tools.script_recovery.native_barrel_position import map_barrel_position, recover_first_barrel_actors
from tools.script_recovery.native_function_parameters import function_parameters, rename_parameters
from tools.script_recovery.native_call_setup_ir import read_call_window


class BarrelPositionTests(unittest.TestCase):
    def function(self):
        unit = json.loads((ROOT / 'refs/script_recovery/new_oakvale_intro/translation_unit.json').read_text())
        return next(f for f in unit['functions'] if f['address'] == '0x00DB5330')

    def test_actual_first_lookup_owns_implementation_but_fallback_is_unresolved(self):
        evidence = map_barrel_position(self.function(), RData())
        self.assertEqual(len(evidence), 1)
        mapping = evidence[0]
        self.assertEqual((mapping['status'], mapping['loweringStatus']), ('mapped', 'unresolved'))
        self.assertEqual(mapping['scriptName'], 'M_BarrelManWalkOff')
        self.assertEqual(mapping['lookupSetup']['stack_arguments'][0], ('stack', 172))
        self.assertEqual(mapping['implementationOffset'], 176)
        self.assertEqual(mapping['fallbackAddress'], '0x143e8e0')
        self.assertTrue(mapping['limitations'])
        self.assertEqual(mapping['repeatActorSetup']['ecx'], ('stack', 20))
        self.assertEqual(mapping['repeatDistanceSetup']['edx'], ('stack', 184))
        self.assertEqual(mapping['repeatDistanceSetup']['stack_arguments'], (('constant', 0x40000000),))
        self.assertEqual(mapping['vectorConsumers']['threshold'], 2.0)
        self.assertEqual(len(mapping['acquisitionSetups']), 2)

    def test_first_two_actor_queries_use_proven_self_resource_only(self):
        fn = self.function()
        manifest, slots = load_manifest(), load_slots()
        things, returning = load_thing_tables(manifest, slots)
        source = annotate(rename_parameters(fn['decompile'], function_parameters(fn['decompile'], member=True)),
                          slots, things, returning, entity=True)
        source, _ = fold_self_wrapper_arguments(source)
        evidence = map_barrel_position(fn, RData())
        result = recover_first_barrel_actors(source, evidence)
        self.assertEqual(result.count('pCVar7 = me;'), 2)
        self.assertEqual(source.count('_GetScriptThing_') - result.count('_GetScriptThing_'), 2)
        self.assertIn('recovered', evidence[0]['actorLoweringStatus'])
        changed = source + '\n'
        self.assertEqual(recover_first_barrel_actors(changed, evidence), changed)
        self.assertIn('rejected', evidence[0]['actorLoweringStatus'])
        self.assertEqual(recover_first_barrel_actors(source, [{'status': 'rejected'}]), source)

    def test_changed_consumer_relationships_are_rejected(self):
        for site, changes in (
                (0xDB5577, {'stack_arguments': (('register', 'edi'), ('stack', 24), ('constant', 4))}),
                (0xDB55A4, {'stack_arguments': (('register', 'eax'), ('stack', 20), ('constant', 4))}),
                (0xDB56E4, {'ecx': ('stack', 24)}),
                (0xDB56E4, {'stack_arguments': (('stack', 236),)}),
                (0xDB56F2, {'ecx': ('register', 'eax')}),
                (0xDB56F2, {'edx': ('stack', 188)}),
                (0xDB56F2, {'stack_arguments': (('constant', 0x40400000),)})):
            with self.subTest(site=hex(site), changes=changes):
                def decode(*args, **kwargs):
                    setup = read_call_window(*args, **kwargs)
                    return replace(setup, **changes) if args[3] == site else setup
                with patch('tools.script_recovery.native_barrel_position.read_call_window', side_effect=decode):
                    self.assertEqual(map_barrel_position(self.function(), RData())[0]['status'], 'rejected')

    def test_changed_lookup_slot_or_source_cannot_invent_actor_binding(self):
        fn = self.function()
        def decode(*args, **kwargs):
            setup = read_call_window(*args, **kwargs)
            return replace(setup, stack_arguments=(('stack', 208), ('stack', 160)))
        with patch('tools.script_recovery.native_barrel_position.read_call_window', side_effect=decode):
            self.assertEqual(map_barrel_position(fn, RData())[0]['status'], 'rejected')
        changed = dict(fn, decompile=fn['decompile'] + '\n')
        self.assertEqual(map_barrel_position(changed, RData())[0]['status'], 'rejected')


if __name__ == '__main__':
    unittest.main()
