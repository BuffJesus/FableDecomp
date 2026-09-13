import itertools
import unittest
from types import SimpleNamespace
from lupa.lua54 import LuaRuntime

from tools.script_recovery import test_native_affair_wife_argument_hits as fixtures
from tools.script_recovery.native_book_trader_hits import recover_affair_wife_argument_hits
from tools.script_recovery.native_affair_wife_mask import recover_affair_wife_mask
from tools.script_recovery.lift_native_lua import Lifter, load_slots, load_thing_tables, thing_signatures


class AffairWifeMaskTests(unittest.TestCase):
    def inputs(self):
        fn, source, data, manifest = fixtures.AffairWifeArgumentHitTests().inputs()
        source, _ = recover_affair_wife_argument_hits(fn, source, data, manifest)
        return fn, source, data, manifest

    def test_mask_clears_temporaries_and_preserves_upper_bits_on_repeated_checks(self):
        fn, source, data, manifest = self.inputs()
        result, evidence = recover_affair_wife_mask(fn, source, data)
        self.assertEqual(evidence[0]['status'], 'recovered')
        start = result.index('          CCharString::CCharString((CCharString *)&ppuStack_74,"SCRIPT_NAME_HERO",-1);')
        stop = result.index('          if (bVar4) {', start)
        snippet = '{\n' + result[start:stop] + '\nreturn uVar21;\n}'
        things, _ = load_thing_tables(manifest, load_slots())
        lifter = Lifter(manifest, {}, 'quest', True, '', data, thing_sigs=thing_signatures(things))
        body = '\n'.join(lifter.lift('Mask', snippet, parameters={'uVar21': 'number'}))
        self.assertEqual(lifter.todo, [])
        lua = LuaRuntime()
        run = lua.execute('return function(me,uVar21)\n' + body + '\nend')
        for seed in (0, 8, 0x38, 0xA8, 0xFFFF_FFF8):
            mask = seed
            for hit, any_special, special in itertools.product((False, True), repeat=3):
                me = lua.table_from({'MsgIsHitByHero': lambda me: hit,
                    'MsgIsHitByAnySpecialAbilityFromHero': lambda me: any_special,
                    'MsgIsHitByHeroSpecialAbility': lambda me, ability: special})
                mask = run(me, mask)
                self.assertEqual(mask, seed)

    def test_changed_native_clear_rejects(self):
        fn, source, data, _ = self.inputs()
        def changed(address, size):
            raw = bytearray(data.bytes_at(address, size))
            if address == 0xDB2B10:
                raw[0xDB3798-address] ^= 1
            return bytes(raw)
        result, evidence = recover_affair_wife_mask(fn, source, SimpleNamespace(bytes_at=changed))
        self.assertEqual(result, source)
        self.assertEqual(evidence[0]['status'], 'rejected')
