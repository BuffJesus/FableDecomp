import unittest
from dataclasses import replace
from unittest.mock import patch
from lupa.lua54 import LuaRuntime

from tools.script_recovery import test_native_book_trader_pause as fixtures
from tools.script_recovery.native_affair_pause import recover_book_trader_pause
from tools.script_recovery.native_random import fold_book_trader_random
from tools.script_recovery.native_book_trader_timer import recover_book_trader_timer
from tools.script_recovery.native_call_setup_ir import read_call_window
from tools.script_recovery.lift_native_lua import RData, Lifter, load_entity_parent_state


class BookTraderTimerTests(unittest.TestCase):
    def inputs(self):
        fn, source, manifest = fixtures.BookTraderPauseTests().inputs()
        data = RData()
        source, _ = recover_book_trader_pause(fn, source, data, manifest)
        source, _ = fold_book_trader_random(source, fn, data.bytes_at)
        return fn, source, data, manifest

    def test_native_read_and_reset_use_shared_parent_timer(self):
        fn, source, data, manifest = self.inputs()
        result, evidence = recover_book_trader_timer(fn, source, data, manifest)
        self.assertEqual(evidence[0]['status'], 'recovered')
        snippet = '\n'.join(edit['new'] for edit in evidence[0]['edits'])
        lifter = Lifter(manifest, {}, 'quest', True, '', data,
                        parent_state=load_entity_parent_state('NOVI_BookTrader'))
        body = '\n'.join(lifter.lift('Timer', '{\n' + snippet + '\n}'))
        self.assertEqual(lifter.todo, [])
        lua, events = LuaRuntime(), []
        quest = lua.table_from({
            'GetStateInt': lambda q, key: 73 if key == 'TalkIntermittentTimer' else -1,
            'GetTimer': lambda q, timer: events.append(('read', timer)) or 0,
            'SetTimer': lambda q, timer, delay: events.append(('reset', timer, delay))})
        lua.execute('return function(quest)\n' + body + '\nend')(quest)
        self.assertEqual(events, [('read', 73), ('reset', 73, 3)])
        self.assertNotIn('GSI->GetTimer();', result)

    def test_changed_native_timer_id_rejects_both_edits(self):
        fn, source, data, manifest = self.inputs()
        def changed(*args, **kwargs):
            setup = read_call_window(*args, **kwargs)
            return replace(setup, stack_arguments=(('constant', 0),) + setup.stack_arguments[1:])
        with patch('tools.script_recovery.native_book_trader_timer.read_call_window', side_effect=changed):
            result, evidence = recover_book_trader_timer(fn, source, data, manifest)
        self.assertEqual(result, source)
        self.assertEqual(evidence[0]['status'], 'rejected')

    def test_changed_source_rejects_recovery(self):
        fn, source, data, manifest = self.inputs()
        source += '\n// changed'
        result, evidence = recover_book_trader_timer(fn, source, data, manifest)
        self.assertEqual(result, source)
        self.assertEqual(evidence[0]['status'], 'rejected')
