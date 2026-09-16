import json
import unittest
from dataclasses import replace
from unittest.mock import patch

from tools.script_recovery.lift_native_lua import (ROOT, RData, annotate, load_manifest,
    load_slots, load_thing_tables, fold_self_wrapper_arguments)
from tools.script_recovery.native_barrel_position import (map_barrel_position, recover_first_barrel_actors,
    recover_first_barrel_vector, recover_barrel_teleport_choice, recover_barrel_return_position)
from tools.script_recovery.native_function_parameters import function_parameters, rename_parameters
from tools.script_recovery.native_call_setup_ir import read_call_window


class BarrelPositionTests(unittest.TestCase):
    def return_position_inputs(self):
        fn = self.function()
        manifest, slots = load_manifest(), load_slots()
        things, returning = load_thing_tables(manifest, slots)
        source = fold_self_wrapper_arguments(annotate(rename_parameters(fn['decompile'],
            function_parameters(fn['decompile'], member=True)), slots, things, returning, entity=True))[0]
        evidence = map_barrel_position(fn, RData())
        source = recover_first_barrel_vector(recover_first_barrel_actors(source, evidence), evidence, RData())
        source = recover_barrel_teleport_choice(source, evidence, RData(), manifest)
        return source, evidence, manifest

    def test_return_position_uses_cached_man_start_and_shared_snapshot(self):
        from lupa.lua54 import LuaRuntime
        from tools.script_recovery.lift_native_lua import Lifter, strip_declarations
        source, evidence, manifest = self.return_position_inputs()
        recovered = recover_barrel_return_position(source, evidence, RData())
        self.assertTrue(evidence[0]['returnPositionStatus'].startswith('recovered'))
        statements = strip_declarations(recovered)
        lookup = next(s for s in statements if 'GSI->GetThingWithScriptName(&local_native_man_start' in s)
        position = next(s for s in statements if 'native_arg_man_start_position = ' in s)
        distances = [s for s in statements if 'IsDistanceFromThingToPositionOver' in s and 'native_arg_man_start_position' in s]
        self.assertEqual(len(distances), 2)
        move = next(s for s in statements if '_MoveToPosition_' in s and 'native_arg_man_start_position' in s)
        native = '{\n' + '\n'.join([lookup, position, 'pCVar7 = me;', 'fVar25 = 2.0;',
                                   distances[0], move, distances[1]]) + '\n}'
        lifter = Lifter(manifest, {}, 'quest', True, '', RData())
        body = '\n'.join(lifter.lift('ReturnToStart', native))
        self.assertEqual(lifter.todo, [])
        lua, events = LuaRuntime(), []
        def lookup_point(_q, name):
            self.assertEqual(name, 'M_WHouse_ManStart')
            events.append('lookup')
            return 'start_marker'
        def position_query(actor):
            self.assertEqual(actor, 'start_marker')
            events.append('position')
            return lua.table_from({'x': -1, 'y': 3.25, 'z': 17})
        lua.globals().RetailThingPosition = position_query
        def distance(_me, pos, threshold):
            events.append(('distance', pos.x, pos.y, pos.z, threshold))
            return True
        me = lua.table_from({'IsDistanceFromPositionOver': distance,
            'MoveToPosition': lambda _me, pos, *args: events.append(('move', pos.x, pos.y, pos.z, *args))})
        quest = lua.table_from({'GetThingWithScriptName': lookup_point})
        lua.execute('return function(quest,me)\n' + body + '\nend')(quest, me)
        self.assertEqual(events, ['lookup', 'position', ('distance', -1, 3.25, 17, 2.0),
            ('move', -1, 3.25, 17, 0.0, 1, False, False), ('distance', -1, 3.25, 17, 2.0)])

    def test_return_position_rejects_changed_acquisition_or_consumer(self):
        source, evidence, _ = self.return_position_inputs()
        for site, changes in ((0xDB58B2, {'stack_arguments': (('register', 'eax'), ('stack', 20), ('constant', 4))}),
                              (0xDB5A04, {'ecx': ('stack', 24)}), (0xDB5A12, {'edx': ('stack', 200)})):
            def decode(*args, **kwargs):
                setup = read_call_window(*args, **kwargs)
                return replace(setup, **changes) if args[3] == site else setup
            with patch('tools.script_recovery.native_barrel_position.read_call_window', side_effect=decode):
                self.assertEqual(recover_barrel_return_position(source, evidence, RData()), source)
                self.assertTrue(evidence[0]['returnPositionStatus'].startswith('rejected'))

    def test_camera_selects_primary_offscreen_and_alternate_onscreen(self):
        from lupa.lua54 import LuaRuntime
        from tools.script_recovery.lift_native_lua import Lifter, strip_declarations, known_callee_aliases
        fn = self.function()
        manifest, slots = load_manifest(), load_slots()
        things, returning = load_thing_tables(manifest, slots)
        source = fold_self_wrapper_arguments(annotate(rename_parameters(fn['decompile'],
            function_parameters(fn['decompile'], member=True)), slots, things, returning, entity=True))[0]
        evidence = map_barrel_position(fn, RData())
        source = recover_first_barrel_vector(recover_first_barrel_actors(source, evidence), evidence, RData())
        recovered = recover_barrel_teleport_choice(source, evidence, RData(), manifest)
        self.assertTrue(evidence[0]['teleportChoiceStatus'].startswith('recovered'))
        statements = strip_declarations(recovered)
        start = next(i for i, s in enumerate(statements) if '"M_BarrelManWalkOff"' in s and 'aaStack_170' in s)
        end = next(i for i, s in enumerate(statements) if 'GSI->EntityTeleportToThing(me,native_arg_teleport_point,0)' in s)
        native = '{\n' + '\n'.join(statements[start:end + 1]) + '\n}'
        lifter = Lifter(manifest, {}, 'quest', True, '', RData(), live_termination=True,
                        callee_names=known_callee_aliases(fn))
        body = '\n'.join(lifter.lift('ChoosePoint', native))
        for visible in (False, True):
            lua, events = LuaRuntime(), []
            lua.globals().RetailThingPosition = lambda actor: lua.table_from({'x': 12, 'y': 34, 'z': 56})
            def camera(_q, pos):
                self.assertEqual((pos.x, pos.y, pos.z), (12, 34, 56))
                events.append('camera')
                return visible
            def terminating(_q):
                events.append('termination')
                return False
            def teleport(_q, actor, point, flag):
                events.append(('teleport', actor, point, flag))
            quest = lua.table_from({'GetThingWithScriptName': lambda _q, name: name,
                'IsCameraPosOnScreen': camera, 'IsActiveThreadTerminating': terminating,
                'EntityTeleportToThing': teleport})
            lua.execute('return function(quest,me)\n' + body + '\nend')(quest, 'barrel_man')
            self.assertEqual(events, ['camera', 'termination', ('teleport', 'barrel_man',
                'M_BarrelManWalkOffAlt' if visible else 'M_BarrelManWalkOff', False)])
        changed = source + '\n'
        self.assertEqual(recover_barrel_teleport_choice(changed, evidence, RData(), manifest), changed)
        self.assertTrue(evidence[0]['teleportChoiceStatus'].startswith('rejected'))

    def test_first_position_snapshot_drives_both_distance_queries_and_movement(self):
        from lupa.lua54 import LuaRuntime
        from tools.script_recovery.lift_native_lua import Lifter, strip_declarations
        fn = self.function()
        manifest, slots = load_manifest(), load_slots()
        things, returning = load_thing_tables(manifest, slots)
        source = annotate(rename_parameters(fn['decompile'], function_parameters(fn['decompile'], member=True)),
                          slots, things, returning, entity=True)
        source = fold_self_wrapper_arguments(source)[0]
        evidence = map_barrel_position(fn, RData())
        source = recover_first_barrel_actors(source, evidence)
        recovered = recover_first_barrel_vector(source, evidence, RData())
        self.assertTrue(evidence[0]['vectorLoweringStatus'].startswith('recovered'))
        lines = strip_declarations(recovered)
        start = next(i for i, s in enumerate(lines) if '"M_BarrelManWalkOff"' in s)
        first = next(i for i, s in enumerate(lines) if 'IsDistanceFromThingToPositionOver' in s)
        move = next(s for s in lines if '_MoveToPosition_' in s and 'native_arg_walkoff_position' in s)
        lifter = Lifter(manifest, {}, 'quest', True, '', RData())
        body = '\n'.join(lifter.lift('WalkOff', '{\n' + '\n'.join(lines[start:first + 1] + [move, lines[first]]) + '\n}'))
        self.assertEqual(lifter.todo, [])
        lua, events = LuaRuntime(), []
        lua.globals().RetailThingPosition = lambda actor: lua.table_from({'x': 1, 'y': 2, 'z': 3})
        quest = lua.table_from({'GetThingWithScriptName': lambda _q, name: 'point'})
        def distance(_me, pos, threshold):
            events.append(('distance', pos.x, pos.y, pos.z, threshold))
            return True
        def move_call(_me, pos, *args):
            events.append(('move', pos.x, pos.y, pos.z, *args))
        me = lua.table_from({'IsDistanceFromPositionOver': distance, 'MoveToPosition': move_call})
        lua.execute('return function(quest,me)\n' + body + '\nend')(quest, me)
        self.assertEqual(events, [('distance', 1, 2, 3, 2.0), ('move', 1, 2, 3, 0.0, 1, False, False),
                                  ('distance', 1, 2, 3, 2.0)])
        changed = source + '\n'
        self.assertEqual(recover_first_barrel_vector(changed, evidence, RData()), changed)
        self.assertTrue(evidence[0]['vectorLoweringStatus'].startswith('rejected'))

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
