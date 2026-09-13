import unittest

from lupa.lua54 import LuaRuntime
from tools.script_recovery.test_lift_native_lua import make


class CommaConditionTests(unittest.TestCase):
    def test_counted_loop_updates_and_break_have_c_order(self):
        for stop, expected, result in ((2, [0, 1, 2], 2), (9, [0, 1, 2, 3], 4)):
            native = '''{
for (iVar1 = 0; iVar1 < 4; iVar1 = iVar1 + 1) {
GSI->SetTimer(17,iVar1);
if (iVar1 == native_arg_stop) break;
}
return iVar1;
}'''
            lifter = make()
            body = '\n'.join(lifter.lift('Main', native, parameters={'native_arg_stop': 'number'}))
            lua, events = LuaRuntime(), []
            q = lua.table_from({'SetTimer': lambda _q, timer, value: events.append(value)})
            self.assertEqual(lua.execute('return function(Quest,native_arg_stop)\n' + body + '\nend')(q, stop), result)
            self.assertEqual(events, expected)
            self.assertEqual(lifter.todo, [])

    def test_nested_counted_loops_keep_separate_updates(self):
        source = '''{
for (iVar1 = 0; iVar1 < 2; iVar1 = iVar1 + 1) {
for (iVar2 = 0; iVar2 < 3; iVar2 = iVar2 + 1) {
GSI->SetTimer(iVar1,iVar2);
}
}
}'''
        lifter = make()
        body = '\n'.join(lifter.lift('Main', source))
        lua, events = LuaRuntime(), []
        q = lua.table_from({'SetTimer': lambda _q, *args: events.append(args)})
        lua.execute('return function(Quest)\n' + body + '\nend')(q)
        self.assertEqual(events, [(i, j) for i in range(2) for j in range(3)])
        self.assertEqual(lifter.todo, [])

    def test_assignment_runs_on_every_check_and_final_value_survives(self):
        source = '''{
iVar1 = 0;
while (iVar1 = iVar1 + 1, iVar1 < 3) {
GSI->SetTimer(17,iVar1);
}
return iVar1;
}'''
        lifter = make()
        body = '\n'.join(lifter.lift('Main', source))
        lua, events = LuaRuntime(), []
        q = lua.table_from({'SetTimer': lambda _q, timer, value: events.append((timer, value))})
        result = lua.execute('return function(Quest)\n' + body + '\nend')(q)
        self.assertEqual(result, 3)
        self.assertEqual(events, [(17, 1), (17, 2)])
        self.assertIsNone(lua.globals().iVar1)
        self.assertEqual(lifter.todo, [])

    def test_false_first_check_still_executes_ordered_prefixes(self):
        source = '{\nwhile (iVar1 = 7, iVar2 = iVar1 + 2, false) {\nreturn 99;\n}\nreturn iVar2;\n}'
        lifter = make()
        body = '\n'.join(lifter.lift('Main', source))
        self.assertEqual(LuaRuntime().execute('return function()\n' + body + '\nend')(), 9)
        self.assertEqual(lifter.todo, [])

    def test_nested_call_comma_is_not_a_sequence_separator(self):
        source = '{\nwhile (iVar1 = GSI->GetTimer(17), iVar1 != 0) {\nGSI->SetTimer(17,0);\n}\nreturn iVar1;\n}'
        lifter = make()
        body = '\n'.join(lifter.lift('Main', source))
        lua, state, events = LuaRuntime(), [2], []
        def read(_q, timer):
            events.append(('read', timer, state[0]))
            return state[0]
        def write(_q, timer, value):
            state[0] = value
            events.append(('write', timer, value))
        q = lua.table_from({'GetTimer': read, 'SetTimer': write})
        self.assertEqual(lua.execute('return function(Quest)\n' + body + '\nend')(q), 0)
        self.assertEqual(events, [('read', 17, 2), ('write', 17, 0), ('read', 17, 0)])

    def test_pointer_assignment_is_not_silently_discarded(self):
        lifter = make()
        body = '\n'.join(lifter.lift('Main', '{\nwhile (*piVar1 = 7, false) {\nbreak;\n}\n}'))
        self.assertIn('*piVar1', body)


if __name__ == '__main__':
    unittest.main()
