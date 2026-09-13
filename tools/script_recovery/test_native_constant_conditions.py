import unittest

from lupa.lua54 import LuaRuntime
from tools.script_recovery.native_constant_conditions import fold_decisive_condition
from tools.script_recovery.test_lift_native_lua import make


class ConstantConditionTests(unittest.TestCase):
    def test_decisive_literal_test_does_not_evaluate_rhs(self):
        for source, expected in (('(nil == nil) or (*nil = *nil + -1, *nil ~= 0)', 'true'),
                                 ('(7 ~= 7) and (touch())', 'false'),
                                 ('(0x10 == 16) or (touch())', 'true')):
            folded = fold_decisive_condition(source)
            self.assertEqual(folded, expected)
            self.assertEqual(LuaRuntime().execute('return ' + folded), expected == 'true')

    def test_nondecisive_unknown_or_outer_operators_are_unchanged(self):
        for source in ('(nil ~= nil) or (touch())', '(1 == 1) and (touch())',
                       '(pointer == nil) or (*pointer = 1)', '(nil == nil) or (x) and (y)',
                       '(nil == nil) or (x) or (y)', '(nil == nil) or (x'):
            self.assertEqual(fold_decisive_condition(source), source)

    def test_native_null_cleanup_guard_folds_after_literal_staging(self):
        lifter = make()
        lifter.push_temp('piStack_10', '0')
        expression = lifter.expr('(piStack_10 == 0) || (*piStack_10 = *piStack_10 + -1, *piStack_10 != 0)')
        self.assertEqual(expression, 'true')


if __name__ == '__main__':
    unittest.main()
