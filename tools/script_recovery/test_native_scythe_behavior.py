import itertools
import unittest
from dataclasses import replace
from unittest.mock import patch

from lupa.lua54 import LuaRuntime

from tools.script_recovery import test_native_scythe_entities as fixtures
from tools.script_recovery.lift_native_lua import ROOT, Lifter, load_thing_tables, thing_signatures
from tools.script_recovery.native_scythe_entities import recover_scythe_entity, recover_scythe_behavior
from tools.script_recovery.native_call_setup_ir import read_call_window
from tools.script_recovery.scythe_converter import convert


class ScytheBehaviorTests(unittest.TestCase):
    def inputs(self):
        fn, source, data, manifest = fixtures.ScytheEntityTests().inputs('0x00E2A320')
        source, _ = recover_scythe_entity(fn, source, data, manifest)
        return fn, source, data, manifest

    def test_hit_gate_short_circuits_and_excludes_ability_fourteen(self):
        fn, source, data, manifest = self.inputs()
        recovered, evidence = recover_scythe_behavior(fn, source, data, manifest)
        self.assertEqual(evidence[0]['status'], 'recovered')
        start = recovered.index('native_arg_scythe_hit =')
        end = recovered.index('if (native_arg_scythe_hit)', start)
        snippet = recovered[start:end] + 'return native_arg_scythe_hit;'
        lifter = Lifter(manifest, {}, 'quest', True, 'ScytheInfo', data,
            thing_sigs=thing_signatures(load_thing_tables(manifest, {})[0]))
        body = '\n'.join(lifter.lift('Hit', '{\n' + snippet + '\n}'))
        self.assertEqual(lifter.todo, [])
        for normal, special, excluded in itertools.product((False, True), repeat=3):
            lua, events = LuaRuntime(), []
            def ability(_me, value):
                self.assertEqual(value, 14)
                events.append('ability')
                return excluded
            me = lua.table_from({'MsgIsHitByHero': lambda _me: events.append('normal') or normal,
                'MsgIsHitByAnySpecialAbilityFromHero': lambda _me: events.append('special') or special,
                'MsgIsHitByHeroSpecialAbility': ability})
            result = lua.execute('return function(quest,me)\n' + body + '\nend')(lua.table(), me)
            self.assertEqual(result, normal or (special and not excluded))
            self.assertEqual(events, ['normal'] + ([] if normal else ['special'] + (['ability'] if special else [])))

    def test_hit_movie_health_speech_allies_and_termination_cleanup(self):
        report = convert(ROOT / 'work/scythe_converter/behavior-test-draft')
        self.assertEqual(report['entities']['ScytheNearOracle']['syntax'], 'Lua 5.4 passed')
        source = (ROOT / 'work/scythe_converter/behavior-test-draft/Entities/ScytheNearOracle.lua').read_text()
        self.assertNotIn('quest:PlayCutscene(', source)
        cases = [(1.0, 8), (1.0, 6), (1.0, 7), (0.0, 6), (-1.0, 6), (float('nan'), 6)]
        for health, stop_at in cases:
            with self.subTest(health=health, stop_at=stop_at):
                lua, events = LuaRuntime(), []
                counts = {'termination': 0, 'task': 0}
                def terminating(_q):
                    counts['termination'] += 1
                    self.assertLessEqual(counts['termination'], stop_at)
                    return counts['termination'] == stop_at
                def task(_me):
                    counts['task'] += 1
                    return counts['task'] == 1
                me = lua.table_from({'AcquireControl': lambda _me, priority: events.append(('acquire', priority)) or True,
                    'MsgIsHitByHero': lambda _me: True, 'IsPerformingScriptTask': task,
                    'Speak': lambda _me, target, key, *flags: events.append(('speak', target, key, *flags))})
                def distance(_q, actor, target, threshold):
                    self.assertTrue(lua.eval('function(a,b) return a==b end')(actor, me))
                    self.assertEqual(target, 'hero')
                    return threshold == 20.0
                callbacks = {'NewScriptFrame': lambda _q, *args: True,
                    'IsActiveThreadTerminating': terminating, 'GetHero': lambda _q: 'hero',
                    'GetThingWithScriptName': lambda _q, name: 'marker',
                    'RegisterTimer': lambda _q: 912, 'GetTimer': lambda _q, timer: 0,
                    'IsDistanceBetweenThingsUnder': distance,
                    'GetHealth': lambda _q, actor: events.append(('health', health)) or health,
                    'MiniMapAddMarker': lambda _q, *args: None,
                    'EntitySetFacingAngleTowardsThing': lambda _q, *args: None,
                    'SetTimer': lambda _q, handle, seconds: events.append(('set', handle, seconds))}
                for name in ('StartMovieSequence', 'EndMovieSequence', 'PauseAllNonScriptedEntities',
                             'DeregisterTimer', 'EntitySetThingAsAllyOfThing'):
                    callbacks[name] = lambda _q, *args, name=name: events.append((name, *args))
                lua.execute(source)
                lua.globals().Main(lua.table_from(callbacks), me)
                self.assertEqual(events.count(('StartMovieSequence',)), 1)
                self.assertEqual(events.count(('EndMovieSequence',)), 1)
                self.assertEqual(events.count(('DeregisterTimer', 912)), 1)
                self.assertEqual([e for e in events if e[0] == 'PauseAllNonScriptedEntities'],
                    [('PauseAllNonScriptedEntities', True), ('PauseAllNonScriptedEntities', False)])
                speeches = [e for e in events if e[0] == 'speak']
                self.assertEqual(speeches, [('speak', 'hero', 'TEXT_QST_B03_SCYTHE_ON_HIT_RETURNED',
                    2, False, True, False)] if health > 0.0 else [])
                allies = [e for e in events if e[0] == 'EntitySetThingAsAllyOfThing']
                early = health > 0.0 and stop_at in (6, 7)
                self.assertEqual(len(allies), 0 if early else 2)
                if allies:
                    self.assertEqual(allies[0][2], 'hero')
                    self.assertEqual(allies[1][1], 'hero')
                    self.assertTrue(lua.eval('function(a,b) return a==b end')(allies[0][1], me))
                    self.assertTrue(lua.eval('function(a,b) return a==b end')(allies[1][2], me))
                self.assertEqual(events[-1], ('DeregisterTimer', 912))

    def test_changed_health_movie_allies_or_native_hit_evidence_rejects(self):
        fn, source, data, manifest = self.inputs()
        _, evidence = recover_scythe_behavior(fn, source, data, manifest)
        for call in evidence[0]['calls']:
            for changes in ({'target': ('constant', 0)}, {'stack_arguments': (('constant', 987),)}):
                def decode(*args, **kwargs):
                    setup = read_call_window(*args, **kwargs)
                    return replace(setup, **changes) if args[3] == call['site'] else setup
                with patch('tools.script_recovery.native_scythe_entities.read_call_window', side_effect=decode):
                    result, status = recover_scythe_behavior(fn, source, data, manifest)
                    self.assertEqual(result, source)
                    self.assertEqual(status[0]['status'], 'rejected')
        original = data.bytes_at
        for site in (0xE2A67D, 0xE2A76B, 0xE2A773, 0x122DEDC, 0xE2A5BB, 0xE2A5D9):
            def changed(start, size):
                raw = original(start, size)
                if raw is not None and start <= site < start + size:
                    raw = bytearray(raw)
                    raw[site - start] ^= 1
                    return bytes(raw)
                return raw
            with patch.object(data, 'bytes_at', side_effect=changed):
                self.assertEqual(recover_scythe_behavior(fn, source, data, manifest)[1][0]['status'], 'rejected')
        with patch('tools.script_recovery.native_scythe_entities.check_single_resource_lifetime', return_value=False):
            self.assertEqual(recover_scythe_behavior(fn, source, data, manifest)[1][0]['status'], 'rejected')


if __name__ == '__main__':
    unittest.main()
