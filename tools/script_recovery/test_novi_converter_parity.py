"""Offline behavioral comparisons between generated New Oakvale Lua and the reviewed port.

The real port modules are loaded unchanged. The host is mocked; this does not claim in-game parity.
"""
import copy
import unittest

from lupa.lua54 import LuaRuntime
from tools.script_recovery.lift_native_lua import (
    ROOT, Lifter, RData, converter_signatures, lift_entity, load_manifest,
)


PORT = ROOT / 'refs/script_recovery/reconstructed/NewOakValeIntro/FSE'
TU = ROOT / 'refs/script_recovery/new_oakvale_intro/translation_unit.json'


class RemovalSignatureTests(unittest.TestCase):
    def test_legacy_signature_is_corrected_without_mutating_input(self):
        original = {'RemoveThing': {'scope': 'Quest', 'returnType': 'void', 'parameters': [
            {'name': 'pThing', 'type': 'CScriptThing*'}]}}
        saved = copy.deepcopy(original)
        corrected = converter_signatures(original)
        self.assertEqual(original, saved)
        self.assertEqual([p['name'] for p in corrected['RemoveThing']['parameters']],
                         ['pThing', 'immediate', 'removeFromWorld'])
        self.assertEqual(converter_signatures(corrected), corrected)
        for change in ({'scope': 'Entity'}, {'returnType': 'bool'}, {'parameters': []}):
            altered = copy.deepcopy(original)
            altered['RemoveThing'].update(change)
            self.assertEqual(converter_signatures(altered)['RemoveThing'], altered['RemoveThing'])

    def test_explicit_flags_survive_and_absent_flags_remain_absent(self):
        for operands, expected in (
            ('', ()), (',0', (False,)), (',1,1', (True, True)),
            (',0,1', (False, True)), (',1,0', (True, False)),
            (',0x0,0x1', (False, True)),
        ):
            with self.subTest(operands=operands):
                lifter = Lifter(load_manifest(), {}, 'quest', True, '', RData())
                # Stale native scalar staging must not populate omitted optional flags.
                native = '{\nuStack_20 = 1;\nGSI->RemoveThing((CScriptThing *)(this + 8)' + operands + ');\n}'
                body = '\n'.join(lifter.lift('Main', native))
                lua, events, me = LuaRuntime(), [], object()
                quest = lua.table_from({'RemoveThing': lambda _q, thing, *flags: events.append((thing, flags))})
                lua.execute('return function(quest, me)\n' + body + '\nend')(quest, me)
                self.assertEqual(events, [(me, expected)])
                self.assertTrue(all(type(flag) is bool for flag in events[0][1]))
                self.assertEqual(lifter.todo, [])


    def test_optional_boolean_converts_numeric_results_by_zero_comparison(self):
        lifter = Lifter(load_manifest(), {}, 'quest', True, '', RData())
        body = '\n'.join(lifter.lift('Main', '{\niVar1 = GSI->GetTimer(42);\n'
                                     'GSI->RemoveThing((CScriptThing *)(this + 8),iVar1,1);\n}'))
        for value in (0, 2, -1):
            with self.subTest(value=value):
                lua, events, me = LuaRuntime(), [], object()
                quest = lua.table_from({'GetTimer': lambda _q, _timer: value,
                    'RemoveThing': lambda _q, _thing, *flags: events.append(flags)})
                lua.execute('return function(quest, me)\n' + body + '\nend')(quest, me)
                self.assertEqual(events, [(value != 0, True)])
                self.assertTrue(all(type(flag) is bool for flag in events[0]))


class CreatedBeetleParityTests(unittest.TestCase):
    @classmethod
    def setUpClass(cls):
        cls.generated = lift_entity(TU, 'NOVI_CreatedBeetle', None, '0x00DB80C0')['lua']
        cls.port = (PORT / 'NewOakValeIntro/Entities/NOVI_CreatedBeetle.lua').read_text(encoding='utf-8')

    def run_script(self, source, ticks, cancel_frame):
        lua = LuaRuntime()
        lua.globals().package.path = PORT.as_posix() + '/?.lua;' + lua.globals().package.path
        lua.execute(source)
        me, events, values, frames = object(), [], iter(ticks), [0]

        def frame(_quest, entity):
            self.assertIs(entity, me)
            frames[0] += 1
            self.assertLess(frames[0], 10)
            events.append(('frame',))
            return frames[0] != cancel_frame

        def timer(_quest, timer_id):
            self.assertEqual(timer_id, 42)
            value = next(values)
            events.append(('GetTimer', value))
            return value

        def remove(_quest, thing, immediate=False, remove_from_world=True):
            self.assertIs(thing, me)
            events.append(('RemoveThing', immediate, remove_from_world))

        def register(_quest):
            events.append(('RegisterTimer',))
            return 42

        quest = lua.table_from({'NewScriptFrame': frame, 'RegisterTimer': register,
            'SetTimer': lambda _q, timer_id, seconds: events.append(('SetTimer', timer_id, seconds)),
            'GetTimer': timer, 'RemoveThing': remove,
            'DeregisterTimer': lambda _q, timer_id: events.append(('DeregisterTimer', timer_id))})
        lua.globals().Main(quest, me)
        return events

    def test_complete_generated_main_matches_port_across_lifetime_and_cancellation(self):
        for ticks, cancel in (([0], None), ([5, 3, 1, 0], None), ([], 1), ([5], 2), ([5, 3], 3)):
            with self.subTest(ticks=ticks, cancel=cancel):
                generated = self.run_script(self.generated, ticks, cancel)
                self.assertEqual(generated, self.run_script(self.port, ticks, cancel))
                if cancel == 1:
                    self.assertEqual(generated, [('frame',)])
                else:
                    self.assertIn(('SetTimer', 42, 5), generated)
                    self.assertEqual(generated[-1], ('DeregisterTimer', 42))
                    self.assertEqual(generated.count(('DeregisterTimer', 42)), 1)
                    removal = [e for e in generated if e[0] == 'RemoveThing']
                    self.assertEqual(removal, [('RemoveThing', True, True)] if cancel is None else [])

    def test_comparison_detects_the_previous_dropped_flag_bug(self):
        old_draft = self.generated.replace('RemoveThing(me, true, true)', 'RemoveThing(me)')
        self.assertNotEqual(old_draft, self.generated)
        old_trace = self.run_script(old_draft, [0], None)
        self.assertIn(('RemoveThing', False, True), old_trace)
        self.assertNotEqual(old_trace, self.run_script(self.port, [0], None))


if __name__ == '__main__':
    unittest.main()
