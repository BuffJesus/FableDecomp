import unittest
from lupa.lua54 import LuaRuntime

from tools.script_recovery import test_native_book_trader_hits as fixtures
from tools.script_recovery.native_book_trader_hits import recover_book_trader_hits
from tools.script_recovery.native_book_trader_termination import recover_book_trader_termination
from tools.script_recovery.lift_native_lua import Lifter


class BookTraderTerminationTests(unittest.TestCase):
    def inputs(self):
        fn, source, data, manifest = fixtures.BookTraderHitTests().inputs()
        source, _ = recover_book_trader_hits(fn, source, data, manifest)
        return fn, source, data, manifest

    def test_loop_enters_when_live_and_stops_at_next_saved_result(self):
        fn, source, data, manifest = self.inputs()
        result, evidence = recover_book_trader_termination(fn, source, data)
        self.assertEqual(evidence[0]['status'], 'recovered')
        self.assertNotIn('cVar5 = extraout_AL_00;', result)
        self.assertNotIn('cVar5 = extraout_AL_40;', result)
        first, repeat = [e['new'] for e in evidence[0]['edits']]
        snippet = ('{\n' + first + '\nwhile (true) {\nif (cVar5 != \'\\0\') {\nreturn;\n}\n'
                   'GSI->NewScriptFrame();\n' + repeat + '\n}\n}')
        lifter = Lifter(manifest, {}, 'quest', True, '', data, live_termination=True)
        body = '\n'.join(lifter.lift('Loop', snippet))
        self.assertEqual(lifter.todo, [])
        lua = LuaRuntime()
        run = lua.execute('return function(quest)\n' + body + '\nend')
        for answers in ([True], [False, True], [False, False, True]):
            pending, events = list(answers), []
            def terminating(q):
                value = pending.pop(0)
                events.append(('check', value))
                return value
            quest = lua.table_from({'IsActiveThreadTerminating': terminating,
                'NewScriptFrame': lambda q, *args: events.append(('frame',)) or True})
            run(quest)
            expected = []
            for value in answers:
                expected.append(('check', value))
                if not value:
                    expected.append(('frame',))
            self.assertEqual(events, expected)

    def test_changed_source_rejects_both_results(self):
        fn, source, data, manifest = self.inputs()
        source += '\n// changed'
        result, evidence = recover_book_trader_termination(fn, source, data)
        self.assertEqual(result, source)
        self.assertEqual(evidence[0]['status'], 'rejected')
