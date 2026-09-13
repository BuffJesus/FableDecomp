import itertools
import unittest
from types import SimpleNamespace
from lupa.lua54 import LuaRuntime

from tools.script_recovery import test_native_affair_wife_hero_distance as fixtures
from tools.script_recovery.native_hero_distance import recover_affair_wife_hero_distances
from tools.script_recovery.native_book_trader_hits import recover_affair_wife_hits
from tools.script_recovery.lift_native_lua import Lifter, load_slots, load_thing_tables, thing_signatures


class AffairWifeHitTests(unittest.TestCase):
    def inputs(self):
        fn, source, data, manifest = fixtures.AffairWifeHeroDistanceTests().inputs()
        source, _ = recover_affair_wife_hero_distances(fn, source, data)
        return fn, source, data, manifest

    def test_hit_classification_truth_table_and_query_order(self):
        fn, source, data, manifest = self.inputs()
        result, evidence = recover_affair_wife_hits(fn, source, data, manifest)
        self.assertEqual(evidence[0]['status'], 'recovered')
        self.assertNotIn('LAB_00db2c99', result)
        things, _ = load_thing_tables(manifest, load_slots())
        lifter = Lifter(manifest, {}, 'quest', True, '', data, thing_sigs=thing_signatures(things))
        body = '\n'.join(lifter.lift('Hit', '{\n' + evidence[0]['new'].replace('cStack_9d', 'native_arg_wife_hit') + '\nreturn native_arg_wife_hit;\n}',
                                    parameters={'uVar21': 'number'}))
        self.assertEqual(lifter.todo, [])
        lua = LuaRuntime()
        run = lua.execute('return function(me,uVar21)\n' + body + '\nend')
        inspect = lua.execute('return function(me,uVar21)\n' + body.replace('return native_arg_wife_hit',
                              'return native_arg_wife_hit, piVar20, cVar5') + '\nend')
        for hit, any_special, special in itertools.product((False, True), repeat=3):
            events = []
            def ability(me, value):
                events.append(('special', value))
                return special
            me = lua.table_from({'MsgIsHitByHero': lambda me: events.append('hit') or hit,
                'MsgIsHitByAnySpecialAbilityFromHero': lambda me: events.append('any') or any_special,
                'MsgIsHitByHeroSpecialAbility': ability})
            self.assertEqual(run(me, 0), hit or (any_special and not special))
            expected = ['hit']
            if not hit:
                expected.append('any')
                if any_special:
                    expected.append(('special', 14))
            self.assertEqual(events, expected)
            events.clear()
            inspected = inspect(me, 0)
            self.assertEqual(inspected[0], hit or (any_special and not special))
            self.assertEqual(inspected[1], 8 if hit else 56 if any_special else 24)
            self.assertEqual(inspected[2], hit if hit else special if any_special else any_special)
            self.assertEqual(events, expected)

    def test_changed_native_ability_rejects_recovery(self):
        fn, source, data, manifest = self.inputs()
        def changed(address, size):
            raw = data.bytes_at(address, size)
            if address == 0xDB2B10:
                raw = bytearray(raw)
                raw[0xDB2C81 - address] = 13
                return bytes(raw)
            return raw
        result, evidence = recover_affair_wife_hits(fn, source,
            SimpleNamespace(bytes_at=changed, string_at=data.string_at), manifest)
        self.assertEqual(result, source)
        self.assertEqual(evidence[0]['status'], 'rejected')
