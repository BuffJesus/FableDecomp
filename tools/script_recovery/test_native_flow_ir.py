import unittest

from tools.script_recovery.native_flow_ir import build_flow_graph
from tools.script_recovery.native_reaching_definitions import reaching_definitions


class NativeFlowIRTests(unittest.TestCase):
    def test_true_arm_skips_else_and_join_retains_both_assignments(self):
        graph, incoming = self.analyze(['if (choose) {', 'iVar1 = 1;', '}',
                                       'else {', 'iVar1 = 2;', '}', 'use(iVar1);'])
        self.assertEqual(graph.statements[2].successors, (6,))
        self.assertEqual(incoming[6]['iVar1'], {1, 4})

    def test_else_if_chain_and_nested_alternatives_keep_all_paths(self):
        _, incoming = self.analyze(['if (a) {', 'iVar1 = 1;', '}', 'else if (b) {',
                                   'if (c) {', 'iVar1 = 2;', '}', 'else {', 'iVar1 = 3;', '}',
                                   '}', 'else {', 'iVar1 = 4;', '}', 'use(iVar1);'])
        self.assertEqual(incoming[14]['iVar1'], {1, 5, 8, 12})

    def test_unmatched_alternative_is_rejected(self):
        self.assertIsNone(build_flow_graph(['else {', '}']))

    def analyze(self, lines):
        graph = build_flow_graph(lines)
        self.assertIsNotNone(graph)
        return graph, reaching_definitions(graph)

    def test_branch_join_retains_both_possible_definitions(self):
        graph, incoming = self.analyze(['iVar1 = 1;', 'if (choose) {', 'iVar1 = 2;', '}', 'use(iVar1);'])
        self.assertEqual(incoming[4]['iVar1'], {0, 2})
        self.assertEqual(graph.statements[1].successors, (2, 4))
        self.assertEqual(graph.statements[2].source, 'iVar1 = 2;')
        self.assertFalse(graph.exact)

    def test_loop_back_edge_reaches_a_fixed_point(self):
        _, incoming = self.analyze(['iVar1 = 1;', 'while (running) {', 'use(iVar1);', 'iVar1 = 2;', '}', 'use(iVar1);'])
        self.assertEqual(incoming[2]['iVar1'], {0, 3})
        self.assertEqual(incoming[5]['iVar1'], {0, 3})

    def test_goto_skips_unreachable_assignment(self):
        _, incoming = self.analyze(['iVar1 = 1;', 'goto LAB_ab;', 'iVar1 = 2;', 'LAB_ab:', 'use(iVar1);'])
        self.assertIsNone(incoming[2])
        self.assertEqual(incoming[4]['iVar1'], {0})

    def test_conditional_assignment_and_escape_do_not_prove_uniqueness(self):
        _, incoming = self.analyze(['iVar1 = 1;', 'if (left && (iVar1 = call(), iVar1)) {', '}', 'use(iVar1);'])
        self.assertEqual(incoming[3]['iVar1'], {0, 1})
        _, incoming = self.analyze(['iVar1 = 1;', 'save(&iVar1);', 'call();', 'use(iVar1);'])
        self.assertEqual(incoming[3]['iVar1'], {None})

    def test_unknown_control_flow_is_rejected(self):
        self.assertIsNone(build_flow_graph(['goto LAB_missing;']))
        self.assertIsNone(build_flow_graph(['switch (x) {', '}']))

    def test_compound_and_subregister_writes_invalidate_earlier_uniqueness(self):
        for write in ('iVar1 += 2;', 'iVar1 <<= 1;', 'iVar1._1_1_ = 0;'):
            _, incoming = self.analyze(['iVar1 = 1;', write, 'use(iVar1);'])
            self.assertEqual(incoming[2]['iVar1'], {0, 1})


if __name__ == '__main__':
    unittest.main()
