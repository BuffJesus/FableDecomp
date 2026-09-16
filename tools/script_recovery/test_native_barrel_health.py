import unittest
from dataclasses import replace
from unittest.mock import patch

from lupa.lua54 import LuaRuntime
from tools.script_recovery import test_native_barrel_hits as fixtures
from tools.script_recovery.native_book_trader_hits import recover_barrel_hits
from tools.script_recovery.native_dispatch_scaffolding import recover_barrel_dispatch
from tools.script_recovery.native_book_trader_health import recover_barrel_health, recover_barrel_health_boolean
from tools.script_recovery.native_call_setup_ir import read_call_window
from tools.script_recovery.lift_native_lua import RData, Lifter, strip_declarations


class BarrelHealthTests(unittest.TestCase):
    def inputs(self):
        fn, source, manifest = fixtures.BarrelHitTests().inputs()
        source, _ = recover_barrel_hits(fn, source, RData(), manifest)
        source, _ = recover_barrel_dispatch(fn, source, RData())
        return fn, source, manifest

    def test_seven_health_queries_read_the_controlled_actor(self):
        fn, source, manifest = self.inputs()
        result, evidence = recover_barrel_health(fn, source, RData(), manifest)
        self.assertEqual(evidence[0]['status'], 'recovered')
        statements = strip_declarations(result)
        queries = [s for s in statements if 'GSI->GetHealth(' in s]
        self.assertEqual(len(queries), 7)
        self.assertNotIn('_DAT_0122dedc', result)
        lifter = Lifter(manifest, {}, 'quest', True, '', RData())
        body = '\n'.join(lifter.lift('Health', '{\nuVar4 = me;\nuVar6 = me;\n' + '\n'.join(queries) + '\n}'))
        self.assertEqual(lifter.todo, [])
        lua, calls = LuaRuntime(), []
        me = lua.table_from({'marker': 'controlled_actor'})
        quest = lua.table_from({'GetHealth': lambda _, actor: calls.append(actor.marker) or 12.5})
        lua.execute('return function(quest,me)\n' + body + '\nend')(quest, me)
        self.assertEqual(calls, ['controlled_actor'] * 7)

    def test_packed_boolean_matches_x87_including_unordered(self):
        fn, source, manifest = self.inputs()
        source, _ = recover_barrel_health(fn, source, RData(), manifest)
        result, evidence = recover_barrel_health_boolean(fn, source, RData())
        self.assertEqual(evidence[0]['status'], 'recovered')
        self.assertNotIn('CONCAT13', result)
        self.assertEqual(result.count('native_arg_barrel_has_health = fVar20 > 0.0;'), 3)
        self.assertEqual(result.count('if (native_arg_barrel_has_health)'), 3)
        self.assertEqual(result.count('C3DClothPrimitive::~C3DClothPrimitive'),
                         source.count('C3DClothPrimitive::~C3DClothPrimitive'))
        lifter = Lifter(manifest, {}, 'quest', True, '', RData())
        body = '\n'.join(lifter.lift('PositiveHealth', '{\n' + evidence[0]['edits'][0]['new'] +
            '\nreturn native_arg_barrel_has_health;\n}', parameters={'fVar20': 'number'}))
        self.assertEqual(lifter.todo, [])
        run = LuaRuntime().execute('return function(fVar20)\n' + body + '\nend')
        for value, expected in [(1.0, True), (0.0, False), (-0.0, False), (-2, False),
                                (float('inf'), True), (-float('inf'), False), (float('nan'), False)]:
            self.assertEqual(run(value), expected)

    def test_wrong_resource_or_health_actor_rejects_recovery(self):
        fn, source, manifest = self.inputs()
        for address, change in [(0xDB6156, {'stack_arguments': (('register', 'edi'), ('stack', 24), ('constant', 4))}),
                                (0xDB632E, {'ecx': ('stack', 24)}),
                                (0xDB6337, {'stack_arguments': (('register', 'eax'),)})]:
            def decode(*args, **kwargs):
                setup = read_call_window(*args, **kwargs)
                return replace(setup, **change) if args[3] == address else setup
            with patch('tools.script_recovery.native_book_trader_health.read_call_window', side_effect=decode):
                result, evidence = recover_barrel_health(fn, source, RData(), manifest)
            self.assertEqual(result, source)
            self.assertEqual(evidence[0]['status'], 'rejected')


if __name__ == '__main__':
    unittest.main()
