import itertools
import unittest
from types import SimpleNamespace

from lupa.lua54 import LuaRuntime
from tools.script_recovery import test_native_barrel_pause as fixtures
from tools.script_recovery.native_affair_pause import recover_barrel_pause
from tools.script_recovery.native_book_trader_hits import recover_barrel_hits
from tools.script_recovery.lift_native_lua import RData, Lifter, load_slots, load_thing_tables, thing_signatures


class BarrelHitTests(unittest.TestCase):
    def inputs(self):
        fn, source, manifest = fixtures.BarrelPauseTests().inputs()
        source, _ = recover_barrel_pause(fn, source, RData(), manifest)
        return fn, source, manifest

    def test_all_hit_combinations_preserve_result_and_short_circuit_order(self):
        fn, source, manifest = self.inputs()
        result, evidence = recover_barrel_hits(fn, source, RData(), manifest)
        self.assertEqual(evidence[0]['status'], 'recovered')
        self.assertNotIn('LAB_00db5e85', result)
        things, _ = load_thing_tables(manifest, load_slots())
        lifter = Lifter(manifest, {}, 'quest', True, '', RData(), thing_sigs=thing_signatures(things))
        body = '\n'.join(lifter.lift('Hit', '{\n' + evidence[0]['new'] + '\nreturn bVar3;\n}',
            parameters={'ppuStack_1c8': 'number', 'ppuVar18': 'number'}))
        self.assertEqual(lifter.todo, [])
        lua = LuaRuntime()
        run = lua.execute('return function(me,ppuStack_1c8,ppuVar18)\n' + body + '\nend')
        for hit, any_special, excluded in itertools.product((False, True), repeat=3):
            events = []
            me = lua.table_from({
                'MsgIsHitByHero': lambda _: events.append('hit') or hit,
                'MsgIsHitByAnySpecialAbilityFromHero': lambda _: events.append('any') or any_special,
                'MsgIsHitByHeroSpecialAbility': lambda _, ability: events.append(('excluded', ability)) or excluded})
            self.assertEqual(run(me, 0, 0), hit or (any_special and not excluded))
            expected = ['hit']
            if not hit:
                expected.append('any')
                if any_special:
                    expected.append(('excluded', 14))
            self.assertEqual(events, expected)

    def test_changed_native_ability_rejects_recovery(self):
        fn, source, manifest = self.inputs()
        data = RData()
        def changed(address, size):
            raw = data.bytes_at(address, size)
            if address == 0xDB5330:
                raw = bytearray(raw)
                raw[0xDB5E6D - address] = 13
                return bytes(raw)
            return raw
        result, evidence = recover_barrel_hits(fn, source,
            SimpleNamespace(bytes_at=changed, string_at=data.string_at), manifest)
        self.assertEqual(result, source)
        self.assertEqual(evidence[0]['status'], 'rejected')


if __name__ == '__main__':
    unittest.main()
