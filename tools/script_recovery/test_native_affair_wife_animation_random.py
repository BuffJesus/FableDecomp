import unittest
from lupa.lua54 import LuaRuntime
from tools.script_recovery import test_native_affair_wife_health as fixtures
from tools.script_recovery.native_book_trader_health import recover_affair_wife_health
from tools.script_recovery.native_affair_wife_animation_random import recover_affair_wife_animation_random
from tools.script_recovery.lift_native_lua import Lifter


class AffairWifeAnimationRandomTests(unittest.TestCase):
    def inputs(self):
        fn, source, data, manifest = fixtures.AffairWifeHealthTests().inputs()
        source, _ = recover_affair_wife_health(fn, source, data, manifest)
        return fn, source, data, manifest

    def test_selector_matches_native_signed_remainder_branch(self):
        fn, source, data, manifest = self.inputs()
        result, evidence = recover_affair_wife_animation_random(fn, source, data)
        self.assertEqual(evidence[0]['status'], 'recovered')
        self.assertNotIn('rand();', result)
        old = evidence[0]['old']
        new = evidence[0]['new']
        # Preserve the complete original suffix, including both branch-local
        # termination checks and animation arguments. This test proves selection,
        # not the still-unresolved animation flags or actor cleanup.
        self.assertEqual(result.split(new, 1)[1], source.split(old, 1)[1])
        lifter = Lifter(manifest, {}, 'quest', True, '', data)
        body = '\n'.join(lifter.lift('SelectAnimation', '{\n' + new + '\nreturn bVar4;\n}'))
        self.assertEqual(lifter.todo, [])
        lua = LuaRuntime()
        run = lua.execute('return function(quest,me)\n' + body + '\nend')
        for raw in (*range(-100, 101), -2147483648, 2147483647):
            # Independently evaluate the native AND/sign-fix instruction sequence.
            eax = (raw & 0xffffffff) & 0x80000001
            if eax & 0x80000000:
                eax = (((eax - 1) | 0xfffffffe) + 1) & 0xffffffff
            remainder = (abs(raw) % 2) * (-1 if raw < 0 else 1)
            calls = []
            quest = lua.table_from({'RetailRandModulo': lambda q, n: calls.append(n) or remainder})
            self.assertEqual(run(quest, 'wife'), eax == 0)
            self.assertEqual(calls, [2])

    def test_changed_native_selector_rejects(self):
        fn, source, data, manifest = self.inputs()
        class Changed:
            def bytes_at(self, address, size):
                raw = data.bytes_at(address, size)
                if address <= 0xdb3613 < address + size:
                    raw = bytearray(raw)
                    raw[0xdb3613-address] ^= 1
                    return bytes(raw)
                return raw
        result, evidence = recover_affair_wife_animation_random(fn, source, Changed())
        self.assertEqual(result, source)
        self.assertEqual(evidence[0]['status'], 'rejected')
