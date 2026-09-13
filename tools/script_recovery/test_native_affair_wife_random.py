import unittest
from lupa.lua54 import LuaRuntime
from tools.script_recovery import test_native_affair_wife_timer as fixtures
from tools.script_recovery.native_book_trader_timer import recover_affair_wife_timer
from tools.script_recovery.native_random import fold_affair_wife_random
from tools.script_recovery.lift_native_lua import Lifter, load_entity_parent_state


class AffairWifeRandomTests(unittest.TestCase):
    def inputs(self):
        fn, source, data, manifest = fixtures.AffairWifeTimerTests().inputs()
        source, _ = recover_affair_wife_timer(fn, source, data, manifest)
        return fn, source, data, manifest

    def test_timer_and_forced_speech_short_circuit_rng(self):
        fn, source, data, manifest = self.inputs()
        result, evidence = fold_affair_wife_random(source, fn, data.bytes_at)
        self.assertEqual(evidence[0]['status'], 'recovered')
        start = result.index('iVar10 = GSI->GetTimer(')
        stop = result.index('fVar33 = 5.0;', start)
        snippet = '{\n' + result[start:stop] + '\nreturn true;\n}\nreturn false;\n}'
        lifter = Lifter(manifest, {'0x1d': ('ForceFirstTimeSpeak', 'Bool')}, 'quest', True, '', data,
                        parent_state=load_entity_parent_state('NOVI_AffairWife'), state_receiver='me')
        body = '\n'.join(lifter.lift('Gate', snippet))
        self.assertEqual(lifter.todo, [])
        lua = LuaRuntime()
        run = lua.execute('return function(quest,me)\n' + body + '\nend')
        for timer, forced in ((1, False), (1, True), (0, True), (0, False)):
            for residue in range(500):
                events = []
                quest = lua.table_from({'GetStateInt': lambda q, key: 73,
                    'GetTimer': lambda q, value: events.append(('timer', value)) or timer,
                    'RetailRandModulo': lambda q, modulus: events.append(('rand', modulus)) or residue})
                me = lua.table_from({'GetStateBool': lambda m, key: events.append(('forced', key)) or forced})
                self.assertEqual(run(quest, me), timer == 0 and (forced or residue == 0))
                expected = [('timer', 73)]
                if timer == 0:
                    expected.append(('forced', 'ForceFirstTimeSpeak'))
                    if not forced:
                        expected.append(('rand', 500))
                self.assertEqual(events, expected)

    def test_changed_source_rejects(self):
        fn, source, data, _ = self.inputs()
        source += '\n// changed'
        result, evidence = fold_affair_wife_random(source, fn, data.bytes_at)
        self.assertEqual(result, source)
        self.assertEqual(evidence[0]['status'], 'rejected')
