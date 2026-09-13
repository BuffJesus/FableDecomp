import json
import unittest
from types import SimpleNamespace

from lupa.lua54 import LuaRuntime

from tools.script_recovery.lift_native_lua import (
    ROOT, Lifter, RData, annotate, load_manifest, load_slots, load_thing_tables, thing_signatures,
)
from tools.script_recovery.native_post_attack_cleanup import recover_post_attack_cleanup, OWNERSHIP_LIMITATION


class PostAttackCleanupTests(unittest.TestCase):
    def inputs(self):
        unit = json.loads((ROOT / 'refs/script_recovery/new_oakvale_intro/translation_unit.json').read_text())
        fn = next(f for f in unit['functions'] if f['address'] == '0x00DBEB20')
        manifest, slots = load_manifest(), load_slots()
        things, returning = load_thing_tables(manifest, slots)
        return fn, annotate(fn['decompile'], slots, things, returning, entity=False), manifest, things

    def test_actual_availability_loop_retries_missing_and_dead_and_obeys_cancellation(self):
        fn, source, manifest, things = self.inputs()
        recovered, evidence = recover_post_attack_cleanup(fn, source, RData())
        self.assertEqual(evidence[0]['status'], 'recovered')
        self.assertEqual(evidence[0]['ownershipLimitation'], OWNERSHIP_LIMITATION)
        start = recovered.index('  while( true )')
        end = recovered.index('  bVar3 = CScriptBase::IsActiveThreadTerminating', recovered.index('\n  }', start))
        loop = '{\n' + recovered[start:end] + '\nreturn true;\n}'
        lifter = Lifter(manifest, {}, 'quest', False, '', RData(),
                        thing_sigs=thing_signatures(things), live_termination=True)
        body = '\n'.join(lifter.lift('AvailabilityWait', loop))
        self.assertEqual(lifter.todo, [])
        for states, cancel in (((True,), None), ((None, False, True), None), ((None,), 1), ((False,), 1)):
            with self.subTest(states=states, cancel=cancel):
                lua, events, frames = LuaRuntime(), [], [0]
                def lookup(_q, name):
                    self.assertEqual(name, 'M_PostAttackStart')
                    self.assertLess(frames[0], len(states), 'availability wait failed to stop')
                    state = states[frames[0]]
                    events.append(('lookup', state))
                    def alive(_thing):
                        events.append(('alive', state))
                        return state
                    return None if state is None else lua.table_from({'IsAlive': alive})
                def frame(_q):
                    frames[0] += 1
                    events.append(('frame',))
                    return cancel != frames[0]
                quest = lua.table_from({'GetThingWithScriptName': lookup, 'NewScriptFrame': frame,
                                        'IsActiveThreadTerminating': lambda _q: cancel == frames[0]})
                result = lua.execute('return function(quest)\n' + body + '\nend')(quest)
                self.assertIs(result, None if cancel else True)
                expected = []
                for state in states:
                    expected.append(('lookup', state))
                    if state is not None:
                        expected.append(('alive', state))
                    if not state:
                        expected.append(('frame',))
                self.assertEqual(events, expected)

    def test_changed_native_or_source_evidence_preserves_original(self):
        fn, source, _, _ = self.inputs()
        data = RData()
        for changed_address in (0xDBEB20, 0x99A2E0):
            def read(address, size):
                raw = data.bytes_at(address, size)
                return bytes([raw[0] ^ 1]) + raw[1:] if address == changed_address else raw
            result, evidence = recover_post_attack_cleanup(fn, source, SimpleNamespace(bytes_at=read))
            self.assertEqual(result, source)
            self.assertEqual(evidence[0]['status'], 'rejected')
        changed = source.replace('operator_delete(local_38[0]);', 'GSI->NewScriptFrame();', 1)
        self.assertNotEqual(changed, source)
        self.assertEqual(recover_post_attack_cleanup(fn, changed, data)[0], changed)
        altered_fn = dict(fn, decompile=fn['decompile'] + '\n')
        self.assertEqual(recover_post_attack_cleanup(altered_fn, source, data)[1][0]['status'], 'rejected')
        self.assertEqual(recover_post_attack_cleanup(dict(fn, address='0x00000000'), source, data), (source, []))

    def test_full_lifter_records_ownership_limitation(self):
        fn, source, manifest, things = self.inputs()
        lifter = Lifter(manifest, {}, 'quest', False, '', RData(),
                        thing_sigs=thing_signatures(things), live_termination=True, native_gotos=True)
        lifter.lift('PostAttackStuff', source, native_function=fn)
        self.assertIn(OWNERSHIP_LIMITATION, lifter.todo)
        self.assertEqual(lifter.post_attack_cleanup_evidence[0]['status'], 'recovered')


if __name__ == '__main__':
    unittest.main()
