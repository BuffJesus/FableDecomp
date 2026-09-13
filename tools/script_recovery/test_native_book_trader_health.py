import unittest
from dataclasses import replace
from unittest.mock import patch
from lupa.lua54 import LuaRuntime

from tools.script_recovery import test_native_book_trader_line as fixtures
from tools.script_recovery.native_book_trader_line import recover_book_trader_line
from tools.script_recovery.native_book_trader_health import recover_book_trader_health
from tools.script_recovery.native_call_setup_ir import read_call_window
from tools.script_recovery.lift_native_lua import Lifter


class BookTraderHealthTests(unittest.TestCase):
    def inputs(self):
        fn, source, data, manifest = fixtures.BookTraderLineTests().inputs()
        source, _ = recover_book_trader_line(fn, source, data, manifest)
        return fn, source, data, manifest

    def test_all_seven_health_queries_use_self(self):
        fn, source, data, manifest = self.inputs()
        result, evidence = recover_book_trader_health(fn, source, data, manifest)
        self.assertEqual(evidence[0]['status'], 'recovered')
        self.assertEqual(result.count('GSI->GetHealth(me)'), 7)
        self.assertEqual(evidence[0]['cleanupStatus'], 'unresolved')
        lifter = Lifter(manifest, {}, 'quest', True, '', data)
        body = '\n'.join(lifter.lift('Health', '{\nfVar18 = (float10)GSI->GetHealth(me);\nreturn fVar18;\n}'))
        self.assertEqual(lifter.todo, [])
        lua, actors = LuaRuntime(), []
        for health in (0, 0.25, 1, 100):
            quest = lua.table_from({'GetHealth': lambda q, actor: actors.append(actor) or health})
            value = lua.execute('return function(quest,me)\n' + body + '\nend')(quest, 'trader')
            self.assertEqual(value, health)
        self.assertEqual(actors, ['trader'] * 4)

    def test_wrong_resource_or_consumer_rejects_all_edits(self):
        fn, source, data, manifest = self.inputs()
        for site in (0xDB4426, 0xDB442F):
            def changed(*args, **kwargs):
                setup = read_call_window(*args, **kwargs)
                return replace(setup, stack_arguments=(('constant', 0),)) if args[3] == site else setup
            with patch('tools.script_recovery.native_book_trader_health.read_call_window', side_effect=changed):
                result, evidence = recover_book_trader_health(fn, source, data, manifest)
            self.assertEqual(result, source)
            self.assertEqual(evidence[0]['status'], 'rejected')
