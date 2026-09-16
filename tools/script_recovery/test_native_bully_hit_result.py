import itertools
import unittest
from types import SimpleNamespace

from lupa.lua54 import LuaRuntime
from tools.script_recovery import test_native_bully_presented_item as fixtures
from tools.script_recovery.native_bully_presented_item import recover_bully_presented_item
from tools.script_recovery.native_bully_teddy_offer import recover_bully_hit_result
from tools.script_recovery.lift_native_lua import RData, Lifter, load_slots, load_thing_tables, thing_signatures


class BullyHitResultTests(unittest.TestCase):
    def inputs(self):
        fn, source, manifest = fixtures.BullyPresentedItemTests().inputs()
        source, _ = recover_bully_presented_item(fn, source, RData(), manifest)
        return fn, source, manifest

    def test_boolean_result_and_query_order_for_all_hit_combinations(self):
        fn, source, manifest = self.inputs()
        result, evidence = recover_bully_hit_result(fn, source, RData(), manifest)
        self.assertEqual(evidence[0]['status'], 'recovered')
        self.assertNotIn('LAB_00dbc338', result)
        self.assertNotIn('uStack_134._3_1_', result)
        things, _ = load_thing_tables(manifest, load_slots())
        lifter = Lifter(manifest, {}, 'quest', True, '', RData(), thing_sigs=thing_signatures(things))
        native = ('{\nCCharString::CCharString(aCStack_a4,"SCRIPT_NAME_HERO",-1);\n' +
                  evidence[0]['edits'][0]['new'] + '\nreturn native_arg_bully_hit;\n}')
        body = '\n'.join(lifter.lift('HitResult', native, parameters={'uVar12': 'number'}))
        self.assertEqual(lifter.todo, [])
        lua = LuaRuntime()
        run = lua.execute('return function(me,uVar12)\n' + body + '\nend')
        for hit, any_special, excluded in itertools.product((False, True), repeat=3):
            events = []
            me = lua.table_from({
                'MsgIsHitByHero': lambda _: events.append('hit') or hit,
                'MsgIsHitByAnySpecialAbilityFromHero': lambda _: events.append('any') or any_special,
                'MsgIsHitByHeroSpecialAbility': lambda _, ability: events.append(('excluded', ability)) or excluded})
            self.assertEqual(run(me, 0), hit or (any_special and not excluded))
            self.assertEqual(events, ['hit'] if hit else ['hit', 'any', ('excluded', 14)] if any_special else ['hit', 'any'])

    def test_changed_native_branch_rejects_result_lowering(self):
        fn, source, manifest = self.inputs()
        data = RData()
        def changed(address, size):
            raw = data.bytes_at(address, size)
            if address == 0xDBC2A0:
                raw = bytearray(raw)
                raw[0xDBC32F - address] ^= 1
                return bytes(raw)
            return raw
        result, evidence = recover_bully_hit_result(fn, source,
            SimpleNamespace(bytes_at=changed, string_at=data.string_at), manifest)
        self.assertEqual(result, source)
        self.assertEqual(evidence[0]['status'], 'rejected')


if __name__ == '__main__':
    unittest.main()
