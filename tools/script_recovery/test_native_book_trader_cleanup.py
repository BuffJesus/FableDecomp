import unittest
from lupa.lua54 import LuaRuntime

from tools.script_recovery import test_native_book_trader_movies as fixtures
from tools.script_recovery.native_affair_movies import recover_book_trader_movies
from tools.script_recovery.native_book_trader_cleanup import recover_book_trader_cleanup, JUMP
from tools.script_recovery.lift_native_lua import Lifter


class BookTraderCleanupTests(unittest.TestCase):
    def inputs(self):
        fn, source, data, manifest = fixtures.BookTraderMoviesTests().inputs()
        source, _ = recover_book_trader_movies(fn, source, data, manifest)
        return fn, source, data, manifest

    def test_all_terminal_jump_expansions_preserve_condition_and_effect_order(self):
        fn, source, data, manifest = self.inputs()
        result, evidence = recover_book_trader_cleanup(fn, source, data)
        self.assertEqual(evidence[0]['status'], 'recovered')
        self.assertEqual(sum(evidence[0]['counts'].values()), 24)
        self.assertIsNone(JUMP.search(result))
        self.assertEqual(evidence[0]['destinations']['LAB_00db4ce1'],
                         {'native': '0xDB4CE6', 'source': 'LAB_00db4ce6'})
        self.assertEqual(evidence[0]['destinations']['LAB_00db4bc5'],
                         {'native': '0xDB4F56', 'source': 'LAB_00db4f5a'})
        # Extract each actual expansion using unchanged text around each match.
        previous = 0
        result_pos = 0
        for match in JUMP.finditer(source):
            prefix = source[previous:match.start()]
            self.assertEqual(result[result_pos:result_pos + len(prefix)], prefix)
            result_pos += len(prefix)
            destination = evidence[0]['destinations'][match[3]]['source']
            jump = 'goto ' + destination + ';'
            tail = result.index(jump, result_pos) + len(jump)
            if match[2] is not None:
                tail = result.index('}', tail) + 1
            expanded = result[result_pos:tail]
            condition = match[2]
            if condition is not None:
                expanded = expanded.replace(condition, 'cVar1', 1)
            snippet = '{\n' + expanded + '\nreturn false;\n' + destination + ':\nreturn true;\n}'
            lifter = Lifter(manifest, {}, 'quest', True, '', data, native_gotos=True)
            body = '\n'.join(lifter.lift('Cleanup', snippet, parameters={'cVar1': 'bool'}))
            self.assertEqual(lifter.todo, [])
            lua, events = LuaRuntime(), []
            quest = lua.table_from({'PauseAllNonScriptedEntities': lambda q, value: events.append(('unpause', value)),
                                    'EndMovieSequence': lambda q: events.append(('movie_end',))})
            run = lua.execute('return function(quest,cVar1)\n' + body + '\nend')
            if condition is not None:
                self.assertFalse(run(quest, False))
                self.assertEqual(events, [])
            self.assertTrue(run(quest, True))
            self.assertEqual(events, [('unpause', False) if e == 'unpause' else ('movie_end',)
                                      for e in evidence[0]['paths'][match[3]]])
            result_pos, previous = tail, match.end()

    def test_changed_source_keeps_unrecovered_jumps(self):
        fn, source, data, manifest = self.inputs()
        source += '\n// changed'
        result, evidence = recover_book_trader_cleanup(fn, source, data)
        self.assertEqual(result, source)
        self.assertEqual(evidence[0]['status'], 'rejected')
