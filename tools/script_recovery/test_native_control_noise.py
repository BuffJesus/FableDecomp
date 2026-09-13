import unittest

from lupa.lua54 import LuaRuntime

from tools.script_recovery.test_lift_native_lua import make


class ControlNoiseTests(unittest.TestCase):
    def test_register_named_branch_keeps_its_condition_and_block(self):
        lifter = make()
        source = '''{
do {
if (unaff_EDI != (undefined4 *)0x0) {
GSI->SetTimer(42,1);
}
else {
GSI->SetTimer(42,2);
}
} while (false);
}'''
        body = '\n'.join(lifter.lift('Main', source, parameters={'unaff_EDI': 'unknown'}))
        lua, events = LuaRuntime(), []
        quest = lua.table_from({'SetTimer': lambda _q, timer, value: events.append(value)})
        run = lua.execute('return function(Quest,unaff_EDI)\n' + body + '\nend')
        run(quest, None)
        run(quest, lua.table())
        self.assertEqual(events, [2, 1])
        self.assertTrue(any('unresolved native control operand' in t for t in lifter.todo))

    def test_loop_header_with_noise_operand_is_not_dropped(self):
        lifter = make()
        body = '\n'.join(lifter.lift('Main', '''{
while (unaff_EDI != (undefined4 *)0x0) {
GSI->SetTimer(42,1);
break;
}
GSI->SetTimer(42,2);
}''', parameters={'unaff_EDI': 'unknown'}))
        lua, events = LuaRuntime(), []
        quest = lua.table_from({'SetTimer': lambda _q, timer, value: events.append(value)})
        run = lua.execute('return function(Quest,unaff_EDI)\n' + body + '\nend')
        run(quest, None)
        self.assertEqual(events, [2])
        events.clear()
        run(quest, lua.table())
        self.assertEqual(events, [1, 2])

    def test_unknown_native_call_condition_remains_a_visible_diagnostic(self):
        lifter = make()
        body = '\n'.join(lifter.lift('Main', '{\nif (FUN_00123456()) {\nGSI->SetTimer(42,1);\n}\n}'))
        self.assertIn('if FUN_00123456() then', body)
        self.assertTrue(any('FUN_00123456' in t for t in lifter.todo))


if __name__ == '__main__':
    unittest.main()
