import unittest

from lupa.lua54 import LuaRuntime
from tools.script_recovery.native_function_parameters import function_parameters, rename_parameters
from tools.script_recovery.test_lift_native_lua import make


class NativeParameterTests(unittest.TestCase):
    def test_loop_result_survives_break_without_leaking_global(self):
        body = '\n'.join(make().lift('Pick', '{\nwhile (true) {\nuVar3 = 7;\nbreak;\n}\nreturn uVar3;\n}'))
        lua = LuaRuntime()
        self.assertEqual(lua.execute('return function()\n' + body + '\nend')(), 7)
        self.assertIsNone(lua.globals().uVar3)

    def test_comparison_mask_returns_value_or_zero(self):
        body = '\n'.join(make().lift('Pick', "{\nreturn ~-(uint)(native_arg_param_1 != 0) & uVar3;\n}",
                                    parameters={'native_arg_param_1': 'number', 'uVar3': 'number'}))
        run = LuaRuntime().execute('return function(native_arg_param_1, uVar3)\n' + body + '\nend')
        self.assertEqual(run(0, 7), 7)
        self.assertEqual(run(1, 7), 0)
        self.assertEqual(run(0, 0), 0)
        self.assertNotIn('and', make().expr('~-(uint)(uVar1) & uVar3'))

    def test_inventory_proven_member_overrides_missing_decompiler_convention(self):
        source = 'void FUN_00dbb310(CWorldMap *param_1)\n{\nreturn;\n}'
        self.assertEqual(function_parameters(source, member=True)['parameters'], [])
        self.assertEqual(len(function_parameters(source)['parameters']), 1)

    def test_scalar_param_one_is_not_an_entity_receiver(self):
        source = 'long __thiscall Owner::Pick(Owner *this,long param_1)\n{\nreturn param_1 + 8;\n}'
        signature = function_parameters(source)
        renamed = rename_parameters(source, signature)
        self.assertEqual(signature['returnKind'], 'number')
        self.assertEqual(signature['parameters'], [{'native': 'param_1', 'lua': 'native_arg_param_1', 'type': 'long'}])
        lifter = make(entity=True)
        body = '\n'.join(lifter.lift('Pick', renamed, parameters={'native_arg_param_1': 'number'}))
        self.assertNotIn('me', body)
        run = LuaRuntime().execute('return function(native_arg_param_1)\n' + body + '\nend')
        self.assertEqual(run(4), 12)

    def test_parameter_reassignment_in_branch_retains_incoming_value(self):
        source = '{\nif (change) {\nnative_arg_param_1 = 7;\n}\nreturn native_arg_param_1;\n}'
        body = '\n'.join(make().lift('Helper', source, parameters={'native_arg_param_1': 'number'}))
        run = LuaRuntime().execute('return function(native_arg_param_1, change)\n' + body + '\nend')
        self.assertEqual(run(42, False), 42)
        self.assertEqual(run(42, True), 7)

    def test_parameter_renaming_preserves_comments_and_strings(self):
        source = 'void __thiscall Owner::F(Owner *this,int param_1) { /* param_1 */\nfoo("param_1");\nreturn param_1; }'
        output = rename_parameters(source, function_parameters(source))
        self.assertIn('/* param_1 */', output)
        self.assertIn('foo("param_1")', output)
        self.assertIn('return native_arg_param_1;', output)

    def test_entity_helper_receives_context_and_returns_assigned_value(self):
        lifter = make(entity=True)
        lifter.helper_names = {'Pick'}
        lifter.helper_parameters = {'Pick': ['native_arg_param_1']}
        lifter.helper_return_kinds = {'Pick': 'number'}
        body = '\n'.join(lifter.lift('Main', '{\niVar1 = Pick(this,42);\nreturn iVar1;\n}'))
        lua = LuaRuntime()
        lua.execute('function Pick(quest, me, value) assert(quest == "quest"); assert(me == "entity"); return value+1 end')
        result = lua.execute('return function(quest, me)\n' + body + '\nend')('quest', 'entity')
        self.assertEqual(result, 43)
        self.assertEqual(lifter.todo, [])


if __name__ == '__main__':
    unittest.main()
