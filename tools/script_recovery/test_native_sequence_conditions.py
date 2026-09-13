import unittest

from lupa.lua54 import LuaRuntime
from tools.script_recovery import test_lift_native_lua as fixtures
from tools.script_recovery.native_sequence_conditions import expand_sequence_conditions


class SequenceConditionTests(unittest.TestCase):
    def test_ordinary_if_else_preserves_nested_short_circuit_and_result_scope(self):
        source = '''{
iVar1 = 7;
if (native_arg_skip || ((native_arg_enabled && (iVar1 = GSI->GetTimer(1), iVar1 != 0)) || false)) {
GSI->SetTimer(2,10);
}
else {
GSI->SetTimer(2,20);
}
return iVar1;
}'''
        for skip, enabled, timer, expected, read, branch in (
                (True, True, 0, 7, False, 10),
                (False, False, 1, 7, False, 20),
                (False, True, 1, 1, True, 10),
                (False, True, 0, 0, True, 20)):
            lifter = fixtures.make()
            body = '\n'.join(lifter.lift('Main', source, parameters={'native_arg_skip': 'bool', 'native_arg_enabled': 'bool'}))
            self.assertEqual(lifter.todo, [])
            lua, events = LuaRuntime(), []
            quest = lua.table_from({'GetTimer': lambda *_: events.append('read') or timer,
                                   'SetTimer': lambda _q, ident, value: events.append(value)})
            run = lua.execute('return function(Quest,native_arg_skip,native_arg_enabled)\n' + body + '\nend')
            self.assertEqual(run(quest, skip, enabled), expected)
            self.assertEqual(events, (['read'] if read else []) + [branch])

    def test_nested_short_circuit_order_and_skipped_assignment_values(self):
        source = '''{
iVar1 = 7;
iVar2 = 8;
if (early || ((enabled && (iVar1 = GSI->GetTimer(1), iVar1 != 0)) || (iVar2 = GSI->GetTimer(2), iVar2 != 0))) goto LAB_00abcd;
GSI->SetTimer(3,99);
LAB_00abcd:
GSI->SetTimer(4,iVar1);
return iVar2;
}'''
        for early, enabled, first, second, reads, write, result in (
                (True, True, 1, 1, [], 7, 8),
                (False, False, 1, 1, [2], 7, 1),
                (False, True, 1, 1, [1], 1, 8),
                (False, True, 0, 1, [1, 2], 0, 1),
                (False, True, 0, 0, [1, 2], 0, 0)):
            with self.subTest(early=early, enabled=enabled, first=first, second=second):
                lifter = fixtures.make()
                lifter.native_gotos = True
                body = '\n'.join(lifter.lift('Main', source, parameters={'early': 'bool', 'enabled': 'bool'}))
                lua, events = LuaRuntime(), []
                def read(_q, timer):
                    events.append(('read', timer))
                    return first if timer == 1 else second
                quest = lua.table_from({'GetTimer': read, 'SetTimer': lambda _q, timer, val: events.append(('write', timer, val))})
                actual = lua.execute('return function(Quest,early,enabled)\n' + body + '\nend')(quest, early, enabled)
                self.assertEqual(actual, result)
                expected = [('read', timer) for timer in reads]
                if not early and (not enabled or first == 0) and second == 0:
                    expected.append(('write', 3, 99))
                expected.append(('write', 4, write))
                self.assertEqual(events, expected)
                self.assertEqual(lifter.todo, [])
                self.assertTrue(lifter.sequence_temporaries)
                self.assertTrue(lifter.sequence_temporaries.isdisjoint(lifter.order))

    def test_numeric_zero_leaf_is_false(self):
        lifter = fixtures.make()
        lifter.native_gotos = True
        source = '{\nif (n && (iVar1 = GSI->GetTimer(1), iVar1 != 0)) goto LAB_00abc;\nreturn 0;\nLAB_00abc:\nreturn 1;\n}'
        body = '\n'.join(lifter.lift('Main', source, parameters={'n': 'number'}))
        lua, calls = LuaRuntime(), []
        quest = lua.table_from({'GetTimer': lambda *_: calls.append(1) or 1})
        run = lua.execute('return function(Quest,n)\n' + body + '\nend')
        self.assertEqual(run(quest, 0), 0)
        self.assertEqual(calls, [])
        self.assertEqual(run(quest, 2), 1)
        self.assertEqual(calls, [1])

    def test_unsupported_effects_and_ordinary_call_commas_stay_unchanged(self):
        for source in ('if (a || (*p = 1, true)) goto LAB_00abc;',
                       'if (Call(a,b)) goto LAB_00abc;',
                       'if (a || (iVar1 = 1, b ? c : d)) goto LAB_00abc;'):
            self.assertEqual(expand_sequence_conditions([source], set()), ([source], set()))


if __name__ == '__main__':
    unittest.main()
