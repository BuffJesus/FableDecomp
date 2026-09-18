import unittest

from tools.script_recovery.declare_free_locals import declare_free_locals


class DeclareFreeLocalsTests(unittest.TestCase):
    def test_a_read_only_slot_joins_the_local_line(self):
        source = '''function Main(quest, me)
    local iVar1
    iVar1 = 0
    quest:F(xStack_7c, iVar1)
end
'''
        out, added = declare_free_locals(source)
        self.assertEqual(added, {'Main': ['xStack_7c']})
        self.assertIn('    local iVar1, xStack_7c\n', out)

    def test_a_function_without_a_local_line_gets_one(self):
        source = '''function Main(quest, me)
    quest:F(xStack_7c)
end
'''
        out, added = declare_free_locals(source)
        self.assertEqual(added, {'Main': ['xStack_7c']})
        self.assertIn('function Main(quest, me)\n    local xStack_7c\n', out)

    def test_each_function_declares_its_own(self):
        source = '''function A(quest)
    quest:F(uVar3)
end

function B(quest)
    quest:F(uVar3)
end
'''
        out, added = declare_free_locals(source)
        self.assertEqual(added, {'A': ['uVar3'], 'B': ['uVar3']})
        self.assertEqual(out.count('local uVar3'), 2)

    def test_real_gaps_stay_visible(self):
        # `this`, `unaff_EBP` and `__unknown_push` are decompiler/lifter gaps: declaring them would hide
        # the evidence behind a silent nil
        source = '''function Main(quest)
    quest:F(this, unaff_EBP, __unknown_push)
end
'''
        out, added = declare_free_locals(source)
        self.assertEqual(added, {})
        self.assertEqual(out, source)

    def test_names_already_in_scope_are_not_redeclared(self):
        source = '''local pShared

function Main(quest, me)
    local pCVar1 = quest:GetHero()
    for iVar2 = 1, 3 do
        quest:F(pCVar1, iVar2, pShared)
    end
end
'''
        out, added = declare_free_locals(source)
        self.assertEqual(added, {})
        self.assertEqual(out, source)


if __name__ == '__main__':
    unittest.main()
