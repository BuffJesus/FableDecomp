import unittest
from lupa.lua54 import LuaRuntime

from tools.script_recovery.native_flat_control import flatten_control
from tools.script_recovery.lift_native_lua import Lifter, RData, strip_declarations


class FlatControlTests(unittest.TestCase):
    def run_body(self, body, parameters=None):
        parameters = parameters or {}
        lifter = Lifter({}, {}, 'quest', False, '', RData(), flat_control=True)
        lines = lifter.lift('Control', '{\n' + body + '\n}', parameters=parameters)
        self.assertEqual(lifter.todo, [])
        lua = LuaRuntime()
        function = lua.execute('return function(' + ','.join(parameters) + ')\n' + '\n'.join(lines) + '\nend')
        return function, lifter

    def test_switch_fallthrough_break_and_continue_target_the_correct_structure(self):
        fn, lifter = self.run_body('''
iVar1 = 0;
for (iVar2 = 0; iVar2 < 4; iVar2 = iVar2 + 1) {
  switch ((iVar2 + selector) % 3) {
    case 0:
      iVar1 = iVar1 + 1;
      continue;
    case 1:
      iVar1 = iVar1 + 10;
    case 2:
      iVar1 = iVar1 + 100;
      break;
    default:
      iVar1 = -1000;
  }
  iVar1 = iVar1 + 1000;
}
return iVar1;''', {'selector': 'number'})
        for selector in range(5):
            expected = 0
            for i in range(4):
                case = (i + selector) % 3
                if case == 0:
                    expected += 1
                    continue
                if case == 1:
                    expected += 10
                expected += 1100
            self.assertEqual(fn(selector), expected)
        self.assertEqual(len(lifter.flat_control_evidence['switchLocals']), 1)

    def test_jump_into_loop_body_and_shared_cleanup(self):
        fn, _ = self.run_body('''
iVar1 = 0;
goto LAB_00000010;
while (iVar1 < 3) {
  iVar1 = iVar1 + 100;
LAB_00000010:
  iVar1 = iVar1 + 1;
  if (iVar1 == 1) goto LAB_00000020;
  iVar1 = iVar1 + 10;
LAB_00000020:
}
if (cancelled) goto LAB_00000030;
return iVar1;
LAB_00000030:
return -1;''', {'cancelled': 'bool'})
        self.assertEqual(fn(False), 112)
        self.assertEqual(fn(True), -1)

    def test_do_while_else_if_and_named_nonstandard_labels(self):
        fn, lifter = self.run_body('''
iVar1 = 0;
iVar2 = 0;
do {
  iVar1 = iVar1 + 1;
  if (iVar1 == 1) {
    continue;
  } else if (iVar1 == 3) {
    goto joined_r0x00000040;
  } else {
    iVar2 = iVar2 + 7;
  }
} while (iVar1 < 5);
joined_r0x00000040:
return iVar2;
''')
        self.assertEqual(fn(), 7)
        self.assertIn('joined_r0x00000040', lifter.flat_control_evidence['nativeLabels'])

    def test_bad_structures_and_missing_targets_reject(self):
        for body in ('if (x) {\nreturn;', 'else {\n}', 'break;', 'continue;',
                     'goto LAB_1234;', 'case 1:', 'for (;;) nonsense;',
                     'switch (x) {\ncase 1:\ncase 1:\n}', 'LAB_1234:\nLAB_1234:'):
            with self.subTest(body=body), self.assertRaises(ValueError):
                flatten_control(body.splitlines())

    def test_nested_call_parentheses_and_literals_do_not_split_conditions(self):
        lines, _ = flatten_control(['if (test("()", nested(1))) call(2);'])
        self.assertEqual(lines[0].split(' goto ')[0], 'if (test("()", nested(1)))')
        self.assertEqual(lines[3], 'call(2);')
        namespace, _ = flatten_control(['CCreatureAction_TrollWhackGroundBase::Initialise(actor);'])
        self.assertEqual(namespace, ['CCreatureAction_TrollWhackGroundBase::Initialise(actor);'])

    def test_loop_comma_assignments_and_short_circuit_integer_truth(self):
        fn, _ = self.run_body('''
iVar1 = 0;
iVar2 = 0;
while (iVar1 = iVar1 + 1, iVar1 < 4) {
  if (selector && (iVar2 = iVar2 + 10, iVar2 < 25)) {
    iVar2 = iVar2 + 1;
  }
}
if (!selector) {
  iVar2 = iVar2 + 100;
}
return iVar2;''', {'selector': 'number'})
        self.assertEqual(fn(0), 100)
        self.assertEqual(fn(1), 32)
        constant, _ = self.run_body('''
iVar1 = 0;
if (0) {
  iVar1 = 99;
}
if (!0x0) {
  iVar1 = iVar1 + 1;
}
if (-2) {
  iVar1 = iVar1 + 10;
}
return iVar1;''')
        self.assertEqual(constant(), 11)
