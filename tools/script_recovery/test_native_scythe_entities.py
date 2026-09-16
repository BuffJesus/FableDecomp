import json
import re
import unittest
from dataclasses import replace
from unittest.mock import patch

from lupa.lua54 import LuaRuntime

from tools.script_recovery.lift_native_lua import (ROOT, RData, Lifter, annotate,
    load_manifest, load_slots, load_thing_tables, thing_signatures)
from tools.script_recovery.native_scythe_entities import recover_scythe_entity
from tools.script_recovery.native_call_setup_ir import read_call_window


class ScytheEntityTests(unittest.TestCase):
    def inputs(self, address):
        raw = (ROOT / 'work/scythe_converter/evidence/decompiles' / (address + '.c')).read_text()
        manifest, slots, data = load_manifest(), load_slots(), RData()
        things, returning = load_thing_tables(manifest, slots)
        return {'address': address, 'decompile': raw}, annotate(raw, slots, things, returning, entity=True), data, manifest

    def lowered(self, address):
        fn, source, data, manifest = self.inputs(address)
        recovered, evidence = recover_scythe_entity(fn, source, data, manifest)
        self.assertEqual(evidence[0]['status'], 'recovered')
        things = load_thing_tables(manifest, {})[0]
        lifter = Lifter(manifest, {}, 'quest', True, 'ScytheInfo', data,
                        thing_sigs=thing_signatures(things), readable_locals=True)
        body = '\n'.join(lifter.lift('Main', recovered))
        self.assertEqual(lifter.todo, [])
        return body

    def test_marker_spawns_scythe_at_its_position_then_removes_marker(self):
        body = self.lowered('0x00E29F50')
        for created in ('created-scythe', None):
            lua, events = LuaRuntime(), []
            def position(me):
                events.append('position')
                return lua.table_from({'x': -127.5, 'y': 200, 'z': 3.25})
            me = lua.table_from({'GetPos': position})
            def create(_quest, definition, pos, name):
                events.append(('create', definition, pos.x, pos.y, pos.z, name))
                return created
            def remove(_quest, marker, immediate, world):
                self.assertTrue(lua.eval('function(a,b) return a == b end')(marker, me))
                events.append(('remove', immediate, world))
            quest = lua.table_from({'CreateCreature': create, 'RemoveThing': remove})
            lua.execute('return function(quest,me)\n' + body + '\nend')(quest, me)
            self.assertEqual(events, ['position', ('create', 'CREATURE_RIVAL_HERO_SCYTHE',
                -127.5, 200, 3.25, 'ScytheNearOracle'), ('remove', False, True)])

    def test_near_oracle_init_uses_self_for_all_five_calls(self):
        body = self.lowered('0x00E2A0F0')
        lua, events = LuaRuntime(), []
        names = ('SetIsThingForcePushable', 'SetIsPushableByHero', 'EntitySetAsKillable',
                 'EntitySetAsToAddToComboMultiplierWhenHit', 'SetThingHasInformation')
        quest = lua.table_from({name: (lambda _q, *args, name=name: events.append((name, *args))) for name in names})
        lua.execute('return function(quest,me)\n' + body + '\nend')(quest, 'scythe')
        self.assertEqual(events, [(names[0], 'scythe', False), (names[1], 'scythe', False),
            (names[2], 'scythe', False, False), (names[3], 'scythe', False),
            (names[4], 'scythe', False, False, False)])

    def test_near_oracle_facing_and_timer_share_proven_actor_and_handle(self):
        fn, source, data, manifest = self.inputs('0x00E2A320')
        recovered, evidence = recover_scythe_entity(fn, source, data, manifest)
        self.assertEqual(evidence[0]['status'], 'recovered')
        self.assertIn('GSI->GetTimer(TurningTimer)', recovered)
        self.assertEqual(recovered.count('GSI->DeregisterTimer(TurningTimer);'), 5)
        self.assertNotIn('cVar6 = extraout_AL_', recovered)
        statements = []
        for needle in ('native_arg_scythe_initial_target =', 'TurningTimer =',
                       'native_arg_scythe_turn_target ='):
            statements.append(re.search(re.escape(needle) + r'[^;]+;', recovered).group(0))
            if 'target' in needle:
                name = needle.split()[0]
                statements.append(re.search(r'GSI->EntitySetFacingAngleTowardsThing\(me,' + name + r',0\);', recovered).group(0))
        statements += ['GSI->SetTimer(TurningTimer,2);', 'iVar9 = GSI->GetTimer(TurningTimer);',
                       'GSI->DeregisterTimer(TurningTimer);', 'return iVar9;']
        lifter = Lifter(manifest, {}, 'quest', True, 'ScytheInfo', data)
        body = '\n'.join(lifter.lift('Turn', '{\n' + '\n'.join(statements) + '\n}'))
        self.assertEqual(lifter.todo, [])
        lua, events = LuaRuntime(), []
        callbacks = {
            'GetThingWithScriptName': lambda _q, name: events.append(('lookup', name)) or 'marker',
            'GetHero': lambda _q: 'hero', 'RegisterTimer': lambda _q: 643,
            'GetTimer': lambda _q, handle: events.append(('get', handle)) or 7,
        }
        for name in ('EntitySetFacingAngleTowardsThing', 'SetTimer', 'DeregisterTimer'):
            callbacks[name] = lambda _q, *args, name=name: events.append((name, *args))
        result = lua.execute('return function(quest,me)\n' + body + '\nend')(lua.table_from(callbacks), 'scythe')
        self.assertEqual(result, 7)
        self.assertEqual(events, [('lookup', 'MK_OW_SCYTHE3'),
            ('EntitySetFacingAngleTowardsThing', 'scythe', 'marker', False),
            ('EntitySetFacingAngleTowardsThing', 'scythe', 'hero', False),
            ('SetTimer', 643, 2), ('get', 643), ('DeregisterTimer', 643)])

    def test_changed_native_receivers_arguments_strings_and_by_value_payload_reject(self):
        for address in ('0x00E29F50', '0x00E2A0F0', '0x00E2A320'):
            fn, source, data, manifest = self.inputs(address)
            _, evidence = recover_scythe_entity(fn, source, data, manifest)
            for call in evidence[0]['calls']:
                for change in ({'target': ('constant', 0)}, {'ecx': ('constant', 0)},
                               {'stack_arguments': (('constant', 777),)}):
                    def decode(*args, **kwargs):
                        setup = read_call_window(*args, **kwargs)
                        return replace(setup, **change) if args[3] == call['site'] else setup
                    with patch('tools.script_recovery.native_scythe_entities.read_call_window', side_effect=decode):
                        result, status = recover_scythe_entity(fn, source, data, manifest)
                        self.assertEqual(result, source)
                        self.assertEqual(status[0]['status'], 'rejected')
            original = data.bytes_at
            site = {'0x00E29F50': 0xE29F9D, '0x00E2A0F0': 0xE2A119,
                    '0x00E2A320': 0xE2A470}[address]
            def changed(start, size):
                raw = original(start, size)
                if raw is not None and start <= site < start + size:
                    raw = bytearray(raw)
                    raw[site - start] ^= 1
                    return bytes(raw)
                return raw
            with patch.object(data, 'bytes_at', side_effect=changed):
                self.assertEqual(recover_scythe_entity(fn, source, data, manifest)[1][0]['status'], 'rejected')
            self.assertEqual(recover_scythe_entity(fn, source + '\n', data, manifest)[1][0]['status'], 'rejected')
        fn, source, data, manifest = self.inputs('0x00E29F50')
        with patch.object(data, 'string_at', return_value='wrong'):
            self.assertEqual(recover_scythe_entity(fn, source, data, manifest)[1][0]['status'], 'rejected')


if __name__ == '__main__':
    unittest.main()
