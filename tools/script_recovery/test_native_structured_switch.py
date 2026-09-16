import unittest
from lupa.lua54 import LuaRuntime
from tools.script_recovery.native_structured_switch import lower_nonfallthrough_switches
from tools.script_recovery.lift_native_lua import Lifter, RData, strip_declarations


class StructuredSwitchTests(unittest.TestCase):
    def test_selector_is_evaluated_once_and_only_selected_case_runs(self):
        manifest = {'NextPhase': {'scope': 'Quest', 'returnType': 'int', 'parameters': []}}
        lifter = Lifter(manifest, {}, 'quest', False, '', RData(), native_gotos=True)
        native = '''{
switch (GSI->NextPhase()) {
case 1:
  return 11;
case 2:
case 3:
  return 23;
default:
  return 99;
}
return -1;
}'''
        body = '\n'.join(lifter.lift('Switch', native))
        self.assertEqual(lifter.todo, [])
        self.assertEqual(lifter.structured_switch_evidence[0]['status'], 'lowered')
        for selected, expected in ((0, 99), (1, 11), (2, 23), (3, 23), (4, 99)):
            lua, calls = LuaRuntime(), []
            def select(_q):
                calls.append('select')
                return selected
            quest = lua.table_from({'NextPhase': select})
            run = lua.execute('return function(quest)\n' + body + '\nend')
            self.assertEqual(run(quest), expected)
            self.assertEqual(calls, ['select'])

    def test_case_break_does_not_exit_outer_loop(self):
        lifter = Lifter({}, {}, 'quest', False, '', RData(), native_gotos=True)
        body = '\n'.join(lifter.lift('Loop', '''{
iVar1 = 0;
iVar2 = 0;
while (iVar1 < 3) {
  switch (selector) {
  case 1:
    iVar2 = iVar2 + 1;
    break;
  case 2:
    iVar2 = iVar2 + 2;
    break;
  default:
    iVar2 = iVar2 + 7;
    break;
  }
  iVar1 = iVar1 + 1;
}
return iVar2;
}''', parameters={'selector': 'number'}))
        self.assertEqual(lifter.todo, [])
        lua = LuaRuntime()
        run = lua.execute('return function(selector)\n' + body + '\nend')
        self.assertEqual([run(n) for n in (1, 2, 3)], [3, 6, 21])

    def test_fallthrough_outer_continue_and_unknown_case_reject_without_partial_output(self):
        for source in ('''switch (x) {
case 1:
  y = 1;
case 2:
  break;
}''', '''switch (x) {
case 1:
  continue;
}''', '''switch (x) {
case f():
  break;
}'''):
            lines = source.splitlines()
            output, evidence = lower_nonfallthrough_switches(lines)
            self.assertEqual(output, lines)
            self.assertEqual(evidence[0]['status'], 'rejected')

    def test_native_default_jump_label_is_preserved(self):
        lifter = Lifter({}, {}, 'quest', False, '', RData(), native_gotos=True)
        body = '\n'.join(lifter.lift('DefaultJoin', '''{
iVar1 = 0;
if (skip) goto switchD_00123456_default;
switch (phase) {
case 1:
  iVar1 = 11;
  break;
}
switchD_00123456_default:
return iVar1;
}''', parameters={'skip': 'bool', 'phase': 'number'}))
        self.assertEqual(lifter.todo, [])
        run = LuaRuntime().execute('return function(skip,phase)\n' + body + '\nend')
        self.assertEqual((run(False, 1), run(True, 1), run(False, 9)), (11, 0, 0))
