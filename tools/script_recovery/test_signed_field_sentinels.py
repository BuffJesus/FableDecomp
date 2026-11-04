"""Native signed DWORD reads must recognize unsigned-spelled sentinels."""
import unittest
from lupa.lua54 import LuaRuntime
from tools.script_recovery.test_lift_native_lua import make


class SignedFieldSentinels(unittest.TestCase):
    def test_signed_comparisons_execute_for_both_representations(self):
        for ctype in ('int', 'long'):
            for operator in ('==', '!=', '<', '<=', '>', '>='):
                for reverse in (False, True):
                    for stored in (0, 1, 17, 0x7fffffff, 0x80000000, 0xffffffff, -1, -2):
                        with self.subTest(ctype=ctype,operator=operator,reverse=reverse,stored=stored):
                            read=f'*({ctype} *)(this + 0x1c)'
                            expression=f'-1 {operator} {read}' if reverse else f'{read} {operator} -1'
                            lifter=make(entity=True,state={'0x1c':('saved','Int')})
                            lowered=lifter.expr(expression)
                            lua=LuaRuntime();lua.globals().quest=lua.table_from({'GetStateInt':lambda _,name:stored})
                            signed=(stored+0x80000000)%0x100000000-0x80000000
                            expected=eval(f'-1 {operator} signed' if reverse else f'signed {operator} -1')
                            self.assertEqual(lua.eval(lowered),expected)

    def test_unsigned_ambiguous_and_nonnegative_reads_are_unchanged(self):
        lifter=make(entity=True,state={'0x1c':('saved','Int')})
        for expression in ('*(uint *)(this + 0x1c) == -1',
                           '*(undefined4 *)(this + 0x1c) == -1',
                           '*(int *)(this + 0x1c) == 1',
                           '*(int *)(this + 0x1c) + 1'):
            self.assertNotIn('4294967296',lifter.expr(expression))

    def test_single_evaluation(self):
        calls=[];lifter=make(entity=True,state={'0x1c':('saved','Int')})
        lua=LuaRuntime();lua.globals().quest=lua.table_from({'GetStateInt':lambda _,name:calls.append(name) or 0xffffffff})
        self.assertTrue(lua.eval(lifter.expr('*(int *)(this + 0x1c) == -1')))
        self.assertEqual(calls,['saved'])


if __name__=='__main__':unittest.main()
