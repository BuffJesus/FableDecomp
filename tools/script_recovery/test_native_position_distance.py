import unittest

from lupa.lua54 import LuaRuntime

from tools.script_recovery import test_lift_native_lua as fixtures
from tools.script_recovery.lift_native_lua import converter_signatures


class PositionDistanceTests(unittest.TestCase):
    def lift(self, operands='actor, position, distance', assigned=True, parameters=None):
        lifter = fixtures.make()
        prefix = 'answer = ' if assigned else ''
        source = '{\n' + prefix + 'IsDistanceFromThingToPositionOver(' + operands + ');\n}'
        body = '\n'.join(lifter.lift('Query', source, parameters=parameters or {
            'actor': 'thing', 'position': 'vector', 'distance': 'number'}))
        return lifter, body

    def test_native_binding_forwards_vector_and_signed_threshold_without_math(self):
        lifter, body = self.lift()
        self.assertEqual(lifter.todo, [])
        lua = LuaRuntime()
        run = lua.execute('return function(actor, position, distance)\n' + body + '\nreturn answer end')
        seen = []
        actor = lua.table_from({'IsDistanceFromPositionOver':
                                lambda actor, position, distance: seen.append((position['x'], distance)) or True})
        position = lua.table_from({'x': 13, 'y': -2, 'z': 7})
        self.assertTrue(run(actor, position, -2.0))
        self.assertEqual(seen, [(13, -2.0)])
        self.assertFalse(run(None, position, 2.0))
        self.assertEqual(len(seen), 1)
        self.assertEqual(lifter.kinds['answer'], 'bool')

    def test_discarded_result_is_valid_lua_and_nil_safe(self):
        lifter, body = self.lift(assigned=False)
        self.assertEqual(lifter.todo, [])
        lua = LuaRuntime()
        lua.execute('return function(actor, position, distance)\n' + body + '\nend')(None, None, 2)

    def test_unproven_native_operands_are_rejected(self):
        for operands in ('actor, (C3DVector *)&uStack_150, 2.0',
                         'unknown, position, 2.0', 'me, position, 2.0',
                         'actor, position, unknownDistance', 'actor, position'):
            with self.subTest(operands=operands):
                lifter, body = self.lift(operands)
                self.assertTrue(lifter.todo)
                self.assertNotIn('IsDistanceFromPositionOver', lifter.calls)
                self.assertIn('answer = nil --[[unresolved native result]]', body)

    def test_literal_float_bits_in_the_distance_slot_are_the_float(self):
        # the helper's third operand is a float stack argument: V_ChickenKicking ChickenMaster pushes it as an
        # immediate (`push 0x40c00000; lea edx,[esp+0x230]; mov ecx,ebp; call 0xcbe4b7`), which Ghidra prints raw
        lifter, body = self.lift('(actor), position, 0x40000000')
        self.assertEqual(lifter.todo, [])
        self.assertIn('actor:IsDistanceFromPositionOver(position, 2.0)', body)

    def test_reviewed_signature_overlay_preserves_input(self):
        original = {}
        overlay = converter_signatures(original)
        self.assertEqual(original, {})
        self.assertEqual(overlay['IsDistanceFromPositionOver']['parameters'][1]['nativeKind'], 'vector')
        newer = {'IsDistanceFromPositionOver': {'review': 'newer'}}
        self.assertEqual(converter_signatures(newer)['IsDistanceFromPositionOver'], {'review': 'newer'})


if __name__ == '__main__':
    unittest.main()
