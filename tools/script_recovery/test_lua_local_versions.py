import itertools
import unittest

from lupa.lua54 import LuaRuntime

from tools.script_recovery.lua_local_versions import split_hoisted_locals, prune_unused_literal_locals


class LuaLocalVersionTests(unittest.TestCase):
    def assert_equivalent(self, source, cases):
        output, mapping = split_hoisted_locals(source)
        for args in cases:
            def run(chunk):
                lua = LuaRuntime(unpack_returned_tuples=True)
                lua.execute(chunk)
                return lua.globals().Main(*args)
            with self.subTest(args=args):
                self.assertEqual(run(output), run(source))
        return output, mapping

    def test_reused_loop_predicate_splits_without_changing_loop_carried_values(self):
        source = '''function Main(limit)
    local cVar1, iVar2
    iVar2 = 0
    cVar1 = iVar2 < limit
    while cVar1 do
        cVar1 = iVar2 % 2 == 0
        if cVar1 then
            iVar2 = iVar2 + 1
        else
            iVar2 = iVar2 + 2
        end
        cVar1 = iVar2 < limit
    end
    cVar1 = iVar2 == 0
    return iVar2, cVar1
end
'''
        output, mapping = self.assert_equivalent(source, [(n,) for n in range(12)])
        self.assertEqual(len(mapping['cVar1']['versions']), 3)
        self.assertIn('while cVar1_', output)

    def test_branch_merge_is_kept_and_later_use_is_separated(self):
        source = '''function Main(a, b)
    local iVar1
    iVar1 = 10
    if a then
        iVar1 = 20
    elseif b then
        iVar1 = 30
    else
        iVar1 = 40
    end
    local first = iVar1
    iVar1 = 100
    return first, iVar1
end
'''
        _, mapping = self.assert_equivalent(source, itertools.product((False, True), repeat=2))
        groups = list(mapping['iVar1']['versions'].values())
        self.assertTrue(any(len(group) == 3 for group in groups))

    def test_goto_repeat_break_and_uninitialized_read_preserve_values(self):
        source = '''function Main(skip)
    local iVar1, iVar2
    if skip then goto finish end
    iVar1 = 0
    repeat
        iVar1 = iVar1 + 1
        if iVar1 == 3 then break end
    until iVar1 > 9
    ::finish::
    iVar2 = iVar1
    iVar1 = 7
    return iVar2, iVar1
end
'''
        # Both spellings must preserve the break edge and the skipped nil value.
        self.assert_equivalent(source, [(False,), (True,)])
        source = source.replace('if iVar1 == 3 then break end', 'if iVar1 == 3 then\n            break\n        end')
        self.assert_equivalent(source, [(False,), (True,)])

    def test_unknown_structure_closures_and_parallel_assignment_fail_closed(self):
        for source in ['''function Main()
    local iVar1
    iVar1 = 1
    local f = function() return iVar1 end
    iVar1 = 2
    return f()
end
''', '''function Main()
    local iVar1, iVar2
    iVar1 = 1
    iVar1, iVar2 = 2, 3
    return iVar1
end
''']:
            self.assertEqual(split_hoisted_locals(source), (source, {}))

    def test_unused_literal_staging_is_removed_but_unused_calls_are_preserved(self):
        source = '''function Main(quest)
    local pCVar1, iVar2
    pCVar1 = (("TEXT_KEY"))
    iVar2 = (quest:SideEffect())
    return 9
end
'''
        output, removed = prune_unused_literal_locals(source)
        self.assertIn('pCVar1', removed)
        self.assertNotIn('pCVar1', output)
        self.assertIn('iVar2 = (quest:SideEffect())', output)
        lua, calls = LuaRuntime(), []
        lua.execute(output)
        q = lua.table_from({'SideEffect': lambda _: calls.append('called')})
        self.assertEqual(lua.globals().Main(q), 9)
        self.assertEqual(calls, ['called'])

    def test_local_limit_is_respected(self):
        source = 'function Main(sink)\n    local ' + ', '.join('iVar' + str(n) for n in range(1, 161)) + '\n'
        for variable in ('iVar1', 'iVar2'):
            for n in range(18):
                source += f'    {variable} = {n}\n    sink({variable})\n'
        source += 'end\n'
        output, mapping = split_hoisted_locals(source)
        LuaRuntime().execute(output)
        self.assertEqual(len(mapping), 1)
