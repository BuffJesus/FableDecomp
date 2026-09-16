import unittest

from lupa.lua54 import LuaRuntime
from tools.script_recovery.lua_literal_values import inline_literal_locals


class LiteralValueTests(unittest.TestCase):
    def run_lua(self, source, *args):
        lua = LuaRuntime(unpack_returned_tuples=True)
        lua.execute(source)
        return lua.globals().Main(*args)

    def test_dominating_literals_inline_without_moving_calls(self):
        source = '''function Main(flag)
    local pVar1, fVar2
    pVar1 = "dialogue key" -- retained note
    fVar2 = -0.25
    local calls = {}
    if flag then
        calls[#calls+1] = pVar1
    end
    calls[#calls+1] = fVar2
    return table.concat(calls, ":")
end
'''
        output, evidence = inline_literal_locals(source)
        self.assertEqual(set(evidence), {'pVar1', 'fVar2'})
        self.assertIn('-- retained note', output)
        for flag in (False, True):
            self.assertEqual(self.run_lua(output, flag), self.run_lua(source, flag))

    def test_conditional_assignment_cannot_replace_nil_read(self):
        source = '''function Main(flag)
    local uVar1
    if flag then
        uVar1 = 42
    end
    return uVar1
end
'''
        output, evidence = inline_literal_locals(source)
        self.assertEqual(output, source)
        self.assertEqual(evidence, {})
        self.assertIsNone(self.run_lua(output, False))

    def test_zero_iteration_and_backedge_reads_keep_original_values(self):
        source = '''function Main(count)
    local uVar1
    while count > 0 do
        count = count - 1
        uVar1 = 3
    end
    return uVar1
end
'''
        self.assertEqual(inline_literal_locals(source), (source, {}))
        source = '''function Main(count)
    local uVar1
    while count > 0 do
        if uVar1 then return uVar1 end
        uVar1 = 3
        count = count - 1
    end
    return uVar1
end
'''
        self.assertEqual(inline_literal_locals(source), (source, {}))

    def test_effectful_or_reassigned_values_and_closures_are_retained(self):
        for body in ('uVar1 = math.random()', 'uVar1 = 1\n    uVar1 = 2',
                     'uVar1 = 1\n    local function read() return uVar1 end'):
            source = 'function Main()\n    local uVar1\n    ' + body + '\n    return uVar1\nend\n'
            self.assertEqual(inline_literal_locals(source), (source, {}))

    def test_unconditional_jump_skipping_definition_is_not_dominance(self):
        source = '''function Main()
    local pVar1
    goto after
    pVar1 = "unreached"
    ::after::
    return pVar1
end
'''
        self.assertEqual(inline_literal_locals(source), (source, {}))

    def test_member_keys_and_literals_are_not_rewritten(self):
        source = '''function Main()
    local uVar1
    uVar1 = true
    local row = {uVar1 = "uVar1"}
    return uVar1, row.uVar1, "uVar1"
end
'''
        output, evidence = inline_literal_locals(source)
        self.assertEqual(set(evidence), {'uVar1'})
        self.assertEqual(self.run_lua(source), self.run_lua(output))

    def test_inline_break_keeps_both_values_reaching_loop_exit(self):
        from tools.script_recovery.readable_lua import readable_source
        source = '''function Main(stopEarly)
    local uVar1
    uVar1 = 7
    while true do
        if stopEarly then break end
        uVar1 = 9
        break
    end
    return uVar1
end
'''
        output, _ = readable_source(source, inline_literals=True)
        self.assertEqual(self.run_lua(output, True), 7)
        self.assertEqual(self.run_lua(output, False), 9)


if __name__ == '__main__':
    unittest.main()
