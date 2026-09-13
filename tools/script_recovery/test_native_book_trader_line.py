import unittest
from dataclasses import replace
from unittest.mock import patch
from lupa.lua54 import LuaRuntime

from tools.script_recovery import test_native_book_trader_timer as fixtures
from tools.script_recovery.native_book_trader_timer import recover_book_trader_timer
from tools.script_recovery.native_book_trader_line import recover_book_trader_line
from tools.script_recovery.native_call_setup_ir import read_call_window
from tools.script_recovery.lift_native_lua import Lifter


class BookTraderLineTests(unittest.TestCase):
    def inputs(self):
        fn, source, data, manifest = fixtures.BookTraderTimerTests().inputs()
        source, _ = recover_book_trader_timer(fn, source, data, manifest)
        return fn, source, data, manifest

    def test_generated_line_preserves_speaker_listener_and_flag(self):
        fn, source, data, manifest = self.inputs()
        recovered, evidence = recover_book_trader_line(fn, source, data, manifest)
        self.assertEqual(evidence[0]['status'], 'recovered')
        snippet = ('{\nCCharString::CCharString(aCStack_d4,"TEXT_QST_048_TRADER_ROLL_UP",-1);\n'
                   + evidence[0]['new'] + '\n}')
        lifter = Lifter(manifest, {}, 'quest', True, '', data)
        body = '\n'.join(lifter.lift('Line', snippet, parameters={'uVar9': 'number'}))
        self.assertEqual(lifter.todo, [])
        lua, events = LuaRuntime(), []
        def hero(q):
            events.append('hero')
            return 'hero'
        quest = lua.table_from({'GetHero': hero,
            'AddLineToConversation': lambda q, *args: events.append(args)})
        lua.execute('return function(quest,me,uVar9)\n' + body + '\nend')(quest, 'trader', 81)
        self.assertEqual(events, ['hero', (81, 'TEXT_QST_048_TRADER_ROLL_UP', 'trader', 'hero', False)])
        self.assertNotIn(evidence[0]['old'], recovered)

    def test_changed_native_speaker_rejects_recovery(self):
        fn, source, data, manifest = self.inputs()
        def changed(*args, **kwargs):
            setup = read_call_window(*args, **kwargs)
            return replace(setup, stack_arguments=setup.stack_arguments[:3] + (('register', 'edi'),) + setup.stack_arguments[4:])
        with patch('tools.script_recovery.native_book_trader_line.read_call_window', side_effect=changed):
            result, evidence = recover_book_trader_line(fn, source, data, manifest)
        self.assertEqual(result, source)
        self.assertEqual(evidence[0]['status'], 'rejected')

    def test_changed_contract_rejects_recovery(self):
        fn, source, data, manifest = self.inputs()
        manifest['AddLineToConversation'] = {}
        result, evidence = recover_book_trader_line(fn, source, data, manifest)
        self.assertEqual(result, source)
        self.assertEqual(evidence[0]['status'], 'rejected')
