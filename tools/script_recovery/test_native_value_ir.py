import unittest

from tools.script_recovery.native_flow_ir import build_flow_graph
from tools.script_recovery.native_value_ir import NativeValues, ScalarValue


class NativeValueTests(unittest.TestCase):
    def test_else_assignment_does_not_overwrite_true_arm_fact(self):
        self.assertIsNone(self.value(['if (choose) {', 'iVar1 = 1;', '}', 'else {',
                                      'iVar1 = 2;', '}', 'use(iVar1);'], 'iVar1'))

    def value(self, lines, name):
        return NativeValues(build_flow_graph(lines)).at(len(lines) - 1, name)

    def test_same_value_branches_agree_but_different_values_do_not(self):
        for alternative, expected in (('7', ScalarValue('integer', 7)), ('8', None)):
            self.assertEqual(self.value(['iVar1 = 7;', 'if (choose) {', 'iVar1 = '+alternative+';', '}',
                                         'use(iVar1);'], 'iVar1'), expected)

    def test_copy_captures_value_before_source_changes(self):
        self.assertEqual(self.value(['iVar1 = 7;', 'iVar2 = iVar1;', 'iVar1 = 8;', 'use(iVar2);'], 'iVar2'),
                         ScalarValue('integer', 7))

    def test_types_remain_distinct(self):
        for literal, expected in (('false', ScalarValue('bool', False)), ('0', ScalarValue('integer', 0)),
                                  ('"Bully"', ScalarValue('string', 'Bully'))):
            self.assertEqual(self.value(['local_x = '+literal+';', 'use(local_x);'], 'local_x'), expected)

    def test_unknown_calls_subregisters_and_loop_mutation_do_not_prove_constants(self):
        for lines in (['iVar1 = 1;', 'Unknown(iVar1);', 'use(iVar1);'],
                      ['iVar1 = 1;', 'iVar1._1_1_ = 2;', 'use(iVar1);'],
                      ['iVar1 = 1;', 'while (running) {', 'iVar1 = iVar1 + 1;', '}', 'use(iVar1);'],
                      ['iVar1 = 1;', 'store(&iVar1);', 'use(iVar1);']):
            self.assertIsNone(self.value(lines, 'iVar1'))
