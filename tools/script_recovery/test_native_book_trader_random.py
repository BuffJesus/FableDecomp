import unittest
from lupa.lua54 import LuaRuntime

from tools.script_recovery import test_native_book_trader_pause as fixtures
from tools.script_recovery.native_affair_pause import recover_book_trader_pause
from tools.script_recovery.native_random import fold_book_trader_random
from tools.script_recovery.lift_native_lua import RData, Lifter, strip_declarations


class BookTraderRandomTests(unittest.TestCase):
    def test_native_gate_skips_random_until_timer_zero(self):
        fn, source, manifest = fixtures.BookTraderPauseTests().inputs()
        data = RData()
        source, _ = recover_book_trader_pause(fn, source, data, manifest)
        recovered, evidence = fold_book_trader_random(source, fn, data.bytes_at)
        self.assertEqual(evidence[0]['status'], 'recovered')
        self.assertEqual(evidence[0]['modulus'], 200)
        self.assertNotIn('rand()', recovered)
        gate = next(line for line in strip_declarations(recovered) if 'RetailRandModulo(200)' in line)
        lifter = Lifter(manifest, {}, 'quest', True, '', data)
        body = '\n'.join(lifter.lift('Gate', '{\n' + gate + '\nreturn true;\n}\nreturn false;\n}',
                                    parameters={'iVar11': 'number'}))
        self.assertEqual(lifter.todo, [])
        lua, events, next_value = LuaRuntime(), [], [0]
        def random(_q, modulus):
            events.append(modulus)
            return next_value[0]
        quest = lua.table_from({'RetailRandModulo': random})
        run = lua.execute('return function(quest,iVar11)\n' + body + '\nend')
        self.assertFalse(run(quest, 7))
        self.assertEqual(events, [])
        for remainder in range(200):
            events.clear()
            next_value[0] = remainder
            self.assertEqual(run(quest, 0), remainder == 0)
            self.assertEqual(events, [200])
        changed = source.replace('iVar11 % 200', 'iVar11 % 199')
        result, evidence = fold_book_trader_random(changed, fn, data.bytes_at)
        self.assertEqual(result, changed)
        self.assertEqual(evidence[0]['status'], 'rejected')


if __name__ == '__main__':
    unittest.main()
