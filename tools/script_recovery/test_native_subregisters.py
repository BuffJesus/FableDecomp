import unittest

from lupa.lua54 import LuaRuntime
from tools.script_recovery.native_subregisters import fold_literal_slices, fold_unsigned_three_byte_casts
from tools.script_recovery.test_lift_native_lua import make


class SubregisterTests(unittest.TestCase):
    def test_unsigned_three_byte_narrowing_preserves_high_byte_test(self):
        lifter = make()
        expression = lifter.expr("(char)((uint3)uVar30 >> 0x10) == '\\0'")
        run = LuaRuntime().execute('return function(uVar30) return ' + expression + ' end')
        for value in (0, 0x10000, 0xff000000, 0x01010000, -1, 0xffffffff):
            self.assertEqual(run(value), ((value & 0xffffff) >> 16) == 0)

    def test_unsigned_narrowing_rejects_non_atomic_inputs_and_protects_literals(self):
        for source in ('(uint3)call()', '(uint3)(a + b)', '(uint3)0.5',
                       '(uint3)value[0]', '(uint3)value->field', '(uint3)value++',
                       '"(uint3)value"', '/* (uint3)value */', '(uint3)value.field'):
            self.assertEqual(fold_unsigned_three_byte_casts(source), source)

    def test_conditional_controlled_resource_call_uses_entity_binding(self):
        from tools.script_recovery.lift_native_lua import Lifter, load_manifest, RData
        source = '''{
if ((left) && (bVar5 = CScriptThing::_IsPerformingScriptTask_CScriptGameResourceObjectScriptedThingBase__UBE_NXZ(&stack0xffffff48), bVar5 == '\\0')) {
return 1;
}
return 0;
}'''
        lifter = Lifter(load_manifest(), {}, 'quest', True, '', RData())
        body = '\n'.join(lifter.lift('Main', source))
        lua, calls = LuaRuntime(), []
        def performing(_me):
            calls.append('query')
            return False
        me = lua.table_from({'IsPerformingScriptTask': performing})
        run = lua.execute('return function(quest,me,left)\n' + body + '\nend')
        self.assertEqual(run(None, me, False), 0)
        self.assertEqual(calls, [])
        self.assertEqual(run(None, me, True), 1)
        self.assertEqual(calls, ['query'])
        self.assertEqual(lifter.todo, [])

    def test_literal_slices_preserve_byte_positions(self):
        for source, expected in (('0._3_1_', '0'), ('0x0._3_1_', '0'), ('0x12345678._1_2_', '13398'),
                                 ('-1._3_1_', '255'), ('0xff000000._3_1_', '255')):
            self.assertEqual(fold_literal_slices(source), expected)
        for source in ('value._3_1_', '0.5._3_1_', '1._8_1_', '1._0_0_', '"0._3_1_"'):
            self.assertEqual(fold_literal_slices(source), source)

    def test_known_staged_integer_slice_controls_branch(self):
        for word, expected in (('0x0', 7), ('0x01000000', 9)):
            lifter = make()
            source = '{\nuStack_c = ' + word + ';\nif (uStack_c._3_1_ != 0) {\nreturn 9;\n}\nreturn 7;\n}'
            body = '\n'.join(lifter.lift('Main', source))
            self.assertEqual(LuaRuntime().execute('return function()\n' + body + '\nend')(), expected)
            self.assertEqual(lifter.todo, [])

    def test_hoisted_conditional_call_has_no_bare_variable_statement(self):
        source = '''{
if (true) {
if ((left) && (iVar2 = GSI->GetTimer(17), iVar2 > 0)) {
GSI->SetTimer(17,0);
}
}
return iVar2;
}'''
        lifter = make()
        body = '\n'.join(lifter.lift('Main', source))
        lua, events = LuaRuntime(), []
        q = lua.table_from({'GetTimer': lambda _q, timer: 7,
                            'SetTimer': lambda _q, *args: events.append(args)})
        run = lua.execute('return function(Quest,left)\n' + body + '\nend')
        self.assertEqual(run(q, True), 7)
        self.assertEqual(events, [(17, 0)])
        self.assertIsNone(run(q, False))
        self.assertEqual(lifter.todo, [])


if __name__ == '__main__':
    unittest.main()
