import unittest

from tools.script_recovery.native_switch_tree import flatten_switch_tree
from tools.script_recovery.native_structured_switch import lower_nonfallthrough_switches


# The shape VC7.1 + Ghidra give a sparse fall-through switch: a binary search over the selector,
# jump tables in the leaves, the case bodies scattered across the branches and linked by gotos into
# out-of-line labels, the last body entered at a loop test, an early-return ladder nested by Ghidra.
TREE = '''
sel = GSI->GetStage();
if (sel < 400) {
  if (sel != 300) {
    if (sel != 200) {
      switch(sel) {
      case 0:
        GSI->Run(0);
      case 100:
        GSI->Run(100);
        break;
      default:
        return;
      }
    }
    GSI->Run(200);
  }
  goto LAB_300;
}
switch(sel) {
case 400:
  goto LAB_400;
default:
  flag = sel == 500;
  goto LAB_loop;
}
LAB_400:
GSI->Run(400);
flag = GSI->Ready();
if (!flag) {
  GSI->Prepare();
  do {
    GSI->Frame();
    flag = GSI->Alive();
LAB_loop:
  } while (flag);
}
switchD_caseD_1:
return;
LAB_300:
GSI->Run(300);
if (GSI->Skip()) goto switchD_caseD_1;
goto LAB_300_tail;
LAB_300_tail:
GSI->Run(301);
goto LAB_400;
'''


class SwitchTreeTests(unittest.TestCase):
    def test_dispatch_tree_becomes_one_fall_through_switch(self):
        out, evidence = flatten_switch_tree(TREE.strip().split('\n'))
        self.assertEqual(evidence['status'], 'flattened', evidence)
        self.assertEqual(evidence['selector'], 'sel')
        self.assertEqual(evidence['chain'], [['0'], ['100'], ['200'], ['300'], ['400'], ['500']])
        self.assertEqual(evidence['loopConditionEntries'], 1)
        self.assertEqual(evidence['trailingIfsInverted'], 1)
        text = '\n'.join(out)
        self.assertEqual(text.count('switch(sel) {'), 1)
        self.assertNotIn('if (sel', text)                       # the dispatch ladder is gone
        self.assertIn('if (flag) {\n    return;\n  }', text)     # the early return is a return again
        cases = [line.strip() for line in out if line.startswith('case ') or line.startswith('default')]
        self.assertEqual(cases, ['case 0:', 'case 100:', 'case 200:', 'case 300:', 'case 400:', 'case 500:', 'default:'])
        # every body sits under its own case, in chain order, and the loop is the 500 body
        order = [line.strip() for line in out if 'GSI->Run(' in line or line.strip() in ('do {', 'case 500:')]
        self.assertEqual(order, ['GSI->Run(0);', 'GSI->Run(100);', 'GSI->Run(200);', 'GSI->Run(300);', 'GSI->Run(301);',
                                 'GSI->Run(400);', 'case 500:', 'do {'])
        # and the structured lowering accepts the result as a fall-through chain
        lowered, chain_evidence = lower_nonfallthrough_switches(out, allow_fallthrough=True)
        self.assertEqual(chain_evidence[0]['status'], 'lowered', chain_evidence)
        self.assertIn('fall-through chain', chain_evidence[0]['policy'])

    def test_untouched_without_two_switches(self):
        statements = ['x = 1;', 'switch(x) {', 'case 1:', 'GSI->Run(1);', 'break;', '}', 'return;']
        out, evidence = flatten_switch_tree(statements)
        self.assertEqual(out, statements)
        self.assertEqual(evidence['status'], 'skipped')

    def test_body_test_on_the_reused_register_is_not_dispatch(self):
        statements = TREE.strip().split('\n')
        # the 400 body re-uses the selector register for something else and tests it
        at = statements.index('GSI->Run(400);')
        statements[at + 1:at + 1] = ['sel = GSI->Result();', 'if (sel == 1) {', 'GSI->Run(401);', '}']
        out, evidence = flatten_switch_tree(statements)
        self.assertEqual(evidence['status'], 'flattened', evidence)
        self.assertNotIn('1', [v for group in evidence['chain'] for v in group])
        self.assertIn('if (sel == 1) {', '\n'.join(out))

    def test_nested_jump_to_a_later_case_hops_along_the_chain(self):
        out, _ = flatten_switch_tree(TREE.strip().split('\n'))
        lowered, _ = lower_nonfallthrough_switches(out, allow_fallthrough=True)
        text = '\n'.join(lowered)
        # `if (GSI->Skip()) goto <default: return>` stays a plain return; a hop to a later block is
        # `selector = value; goto FLOW_chain_next_N;` with the label right after the current block
        self.assertNotIn('FLOW_chain_next', text)      # nothing here jumps to a later block's head label
        statements = TREE.strip().split('\n')
        statements[statements.index('if (GSI->Skip()) goto switchD_caseD_1;')] = 'if (GSI->Skip()) goto LAB_400;'
        out, evidence = flatten_switch_tree(statements)
        self.assertEqual(evidence['status'], 'flattened', evidence)
        lowered, chain_evidence = lower_nonfallthrough_switches(out, allow_fallthrough=True)
        text = '\n'.join(lowered)
        self.assertEqual(chain_evidence[0]['status'], 'lowered', chain_evidence)
        hop = [line for line in lowered if 'goto FLOW_chain_next_' in line]
        self.assertEqual(len(hop), 1)
        self.assertRegex(text, r'native_arg_switch_\d+ = 400;\ngoto FLOW_chain_next_\d+;')
        self.assertRegex(text, r'\}\nFLOW_chain_next_\d+:\nif \(native_arg_switch_\d+ == 400\) \{')


if __name__ == '__main__':
    unittest.main()
