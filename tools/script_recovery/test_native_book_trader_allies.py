import unittest
from dataclasses import replace
from unittest.mock import patch
from lupa.lua54 import LuaRuntime

from tools.script_recovery import test_native_book_trader_health as fixtures
from tools.script_recovery.native_book_trader_health import recover_book_trader_health
from tools.script_recovery.native_book_trader_allies import recover_book_trader_allies
from tools.script_recovery.native_call_setup_ir import read_call_window
from tools.script_recovery.lift_native_lua import Lifter


class BookTraderAlliesTests(unittest.TestCase):
    def inputs(self):
        fn, source, data, manifest = fixtures.BookTraderHealthTests().inputs()
        source, _ = recover_book_trader_health(fn, source, data, manifest)
        return fn, source, data, manifest

    def test_directed_calls_keep_both_getters_and_second_result(self):
        fn, source, data, manifest = self.inputs()
        result, evidence = recover_book_trader_allies(fn, source, data, manifest)
        self.assertEqual(evidence[0]['status'], 'recovered')
        snippet = '{\n' + evidence[0]['new'] + '\nreturn uVar9;\n}'
        lifter = Lifter(manifest, {}, 'quest', True, '', data)
        body = '\n'.join(lifter.lift('Allies', snippet))
        self.assertEqual(lifter.todo, [])
        lua, events, count = LuaRuntime(), [], [0]
        def hero(q):
            count[0] += 1
            value = 'hero' + str(count[0])
            events.append(('get', value))
            return value
        quest = lua.table_from({'GetHero': hero,
            'EntitySetThingAsAllyOfThing': lambda q, first, second: events.append(('ally', first, second))})
        value = lua.execute('return function(quest,me)\n' + body + '\nend')(quest, 'trader')
        self.assertEqual(value, 'hero2')
        self.assertEqual(events, [('get', 'hero1'), ('ally', 'trader', 'hero1'),
                                  ('get', 'hero2'), ('ally', 'hero2', 'trader')])
        self.assertNotIn('GSI->GetHero(pCVar1)', result)

    def test_reversed_native_direction_rejects_both_calls(self):
        fn, source, data, manifest = self.inputs()
        def changed(*args, **kwargs):
            setup = read_call_window(*args, **kwargs)
            return replace(setup, stack_arguments=setup.stack_arguments[::-1])
        with patch('tools.script_recovery.native_book_trader_allies.read_call_window', side_effect=changed):
            result, evidence = recover_book_trader_allies(fn, source, data, manifest)
        self.assertEqual(result, source)
        self.assertEqual(evidence[0]['status'], 'rejected')
