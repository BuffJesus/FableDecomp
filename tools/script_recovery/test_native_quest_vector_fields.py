import copy
import json
import re
import unittest
from types import SimpleNamespace

from lupa.lua54 import LuaRuntime
from tools.script_recovery.lift_native_lua import (
    ROOT, RData, Lifter, annotate, load_manifest, load_slots, load_thing_tables,
    thing_signatures, strip_declarations,
)
from tools.script_recovery.native_function_parameters import function_parameters, rename_parameters
from tools.script_recovery.native_quest_vector_fields import new_oakvale_vectors, recover_barrel_spawn_health


class QuestVectorTests(unittest.TestCase):
    def inputs(self, address, entity=False):
        unit = json.loads((ROOT / 'refs/script_recovery/new_oakvale_intro/translation_unit.json').read_text())
        fn = next(f for f in unit['functions'] if f['address'].lower() == address.lower())
        manifest, slots = load_manifest(), load_slots()
        things, returning = load_thing_tables(manifest, slots)
        source = annotate(rename_parameters(fn['decompile'], function_parameters(fn['decompile'], member=True)),
                          slots, things, returning, entity=entity)
        return fn, source, manifest, thing_signatures(things)

    def test_actual_death_callback_then_beetle_spawn_uses_copied_position_and_two_health(self):
        data = RData()
        vectors, components, _ = new_oakvale_vectors(data)
        fn, source, manifest, sigs = self.inputs('0x00DB7DB0', True)
        parent = dict(components, **{'0x74': ('BarrelBrokenInstantaneous', 'Bool'),
                                     '0x75': ('BarrelBrokenPersistent', 'Bool')})
        lifter = Lifter(manifest, {}, 'quest', True, '', data, parent_state=parent, thing_sigs=sigs)
        callback = '\n'.join(lifter.lift('OnPredicateFail', source))
        self.assertEqual(lifter.todo, [])
        fn, source, manifest, _ = self.inputs('0x00DBE890')
        recovered, evidence = recover_barrel_spawn_health(fn, source, data, manifest)
        self.assertEqual(evidence[0]['status'], 'recovered')
        statements = strip_declarations(recovered)
        start = next(i for i, s in enumerate(statements) if '"NOVI_CreatedBeetle"' in s)
        end = next(i for i, s in enumerate(statements) if 'GSI->EntitySetMaxHealth' in s)
        lifter = Lifter(manifest, components, 'quest', False, '', data, state_vectors=vectors)
        spawn = '\n'.join(lifter.lift('Spawn', '{\n' + '\n'.join(statements[start:end + 1]) + '\n}'))
        self.assertIn('2.0, true)', spawn)
        self.assertNotIn('nil --[[missing]]', spawn)
        for coords in ((1.25, -9.5, 37.0), (0.0, 0.0, 0.0)):
            lua, state, events = LuaRuntime(), {}, []
            def put(_q, key, value):
                state[key] = value
            def create(_q, definition, position, name):
                events.append(('create', definition, tuple(position[a] for a in 'xyz'), name))
                return 'beetle'
            quest = lua.table_from({'SetStateBool': put, 'SetStateFloat': put,
                'GetStateFloat': lambda _q, key: state[key], 'CreateCreature': create,
                'EntitySetMaxHealth': lambda _q, *args: events.append(('health', *args))})
            position = lua.table_from(dict(zip('xyz', coords)))
            me = lua.table_from({'GetPos': lambda _me: position})
            lua.execute('return function(quest,me)\n' + callback + '\nend')(quest, me)
            position.x = 999  # Native callback copies; it does not retain the position pointer.
            lua.execute('return function(quest)\n' + spawn + '\nend')(quest)
            self.assertEqual(events, [('create', 'CREATURE_OAKVALE_STAG_BEETLE', coords, 'NOVI_CreatedBeetle'),
                                      ('health', 'beetle', 2.0, True)])
            self.assertTrue(state['BarrelBrokenInstantaneous'])
            self.assertTrue(state['BarrelBrokenPersistent'])

    def test_changed_evidence_rejects(self):
        missing = SimpleNamespace(bytes_at=lambda *_: None)
        with self.assertRaisesRegex(ValueError, 'position copy changed'):
            new_oakvale_vectors(missing)
        fn, source, manifest, _ = self.inputs('0x00DBE890')
        for f, s, data, spec in ((fn, source, missing, manifest),
                (dict(fn, decompile=fn['decompile'] + ' '), source, RData(), manifest),
                (fn, source.replace('0x40000000', '0x40400000'), RData(), manifest)):
            result, evidence = recover_barrel_spawn_health(f, s, data, spec)
            self.assertEqual(result, s)
            self.assertEqual(evidence[0]['status'], 'rejected')
        changed = copy.deepcopy(manifest)
        changed['EntitySetMaxHealth']['parameters'][1]['type'] = 'int'
        self.assertEqual(recover_barrel_spawn_health(fn, source, RData(), changed)[1][0]['status'], 'rejected')

    def test_vector_and_parent_aliases_require_known_provenance(self):
        manifest, data = load_manifest(), RData()
        vectors, components, _ = new_oakvale_vectors(data)
        lifter = Lifter(manifest, {}, 'quest', True, '', data, parent_state=components, state_vectors=vectors)
        self.assertNotIn('GetStateFloat', lifter.expr('param_1 + 0x76'))
        source = '{\niVar1 = 7;\n*(undefined4 *)(iVar1 + 0x76) = 4;\n}'
        body = '\n'.join(lifter.lift('Unknown', source))
        self.assertNotIn('SetStateFloat', body)
        self.assertNotEqual(lifter.todo, [])
        lifter = Lifter(manifest, {}, 'quest', False, '', data, state_vectors=vectors)
        self.assertEqual(lifter.expr('other + 0x76'), 'other + 0x76')
        lifter.kinds['puVar2'] = 'vector'
        self.assertEqual(lifter.expr('puVar2[1]'), 'puVar2.y')
        lifter.forget_value('puVar2')
        self.assertEqual(lifter.expr('puVar2[1]'), 'puVar2[1]')
