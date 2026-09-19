"""Focused tests for the readable-stage folds. The style pass has no end-to-end fixture by design (its
output is the reviewed artefact in refs/), so each fold is exercised on the shape it exists for."""
import unittest

from lupa.lua54 import LuaRuntime

from tools.script_recovery.readable_style import (
    alias_entity_state, fold_guard_wrappers, inline_entity_fields, name_cleanup_closures,
    name_enum_operands, simplify_conditions, sink_hoisted_locals)

SHIM = '''local __native_entity_state = {}
do
    local fields = {}
    for _, kind in ipairs({"Bool", "Int", "Float", "String", "Thing"}) do
        __native_entity_state["GetState" .. kind] = function(_, name) return fields[name] end
        __native_entity_state["SetState" .. kind] = function(_, name, value) fields[name] = value end
    end
end
'''


def compiles(source):
    LuaRuntime().execute(source)
    return True


class EntityStateShimTests(unittest.TestCase):
    def test_a_shim_no_entity_touches_is_dropped(self):
        source = SHIM + '\nfunction Main(quest, me)\n    quest:Log("hi")\nend\n'
        out, _ = alias_entity_state(source)
        out, count = inline_entity_fields(out)
        self.assertEqual(count, 1)
        self.assertNotIn('state', out.split('function Main')[0])
        self.assertIn('function Main(quest, me)', out)

    def test_fields_become_file_level_locals(self):
        source = SHIM + '''
function Main(quest, me)
    __native_entity_state:SetStateInt("AppleMode", 3)
    if __native_entity_state:GetStateInt("AppleMode") == 3 then quest:Log("x") end
end
'''
        out, _ = alias_entity_state(source)
        out, count = inline_entity_fields(out)
        self.assertEqual(count, 1)
        self.assertIn('local appleMode', out)
        self.assertIn('appleMode = 3', out)
        self.assertNotIn('__native_entity_state', out)

    def test_a_non_literal_key_keeps_the_shim(self):
        # the fields cannot become locals when the key is computed
        source = SHIM + '\nfunction Main(quest, me, k)\n    __native_entity_state:SetStateInt(k, 1)\nend\n'
        out, _ = alias_entity_state(source)
        out, count = inline_entity_fields(out)
        self.assertEqual(count, 0)
        self.assertIn('state', out)


class CleanupClosureNameTests(unittest.TestCase):
    def test_closures_are_named_for_what_they_do_and_identical_ones_merge(self):
        source = '''function Main(quest, me, resources, a, b, c, timerId, resource)
    local function __region_LAB_00d1_c1()
        quest:PauseAllNonScriptedEntities(false)
    end
    local function __region_LAB_00d2_c2()
        quest:PauseAllNonScriptedEntities(false)
    end
    local function __cleanup_LAB_00d3()
        quest:DeregisterTimer(timerId)
        resources:ReleaseResource(resource)
    end
    if a then __region_LAB_00d1_c1(); return end
    if b then __region_LAB_00d2_c2(); return end
    if c then __cleanup_LAB_00d3(); return end
end
'''
        out, count = name_cleanup_closures(source.splitlines(keepends=True))
        text = ''.join(out)
        self.assertEqual(count, 3)                       # three names chosen
        self.assertEqual(text.count('local function ResumeEntities()'), 1)   # ...two of them merged
        self.assertEqual(text.count('ResumeEntities()'), 3)                  # definition + both call sites
        self.assertIn('local function ReleaseEverything()', text)
        self.assertNotIn('__region_', text)
        self.assertNotIn('__cleanup_', text)
        self.assertTrue(compiles(text))

    def test_a_timer_only_epilogue_says_so(self):
        source = '''function Main(quest, a, timerId)
    local function __cleanup_LAB_00d4()
        quest:DeregisterTimer(timerId)
    end
    if a then __cleanup_LAB_00d4(); return end
end
'''
        out, _ = name_cleanup_closures(source.splitlines(keepends=True))
        self.assertIn('local function DeregisterTimers()', ''.join(out))

    def test_nothing_to_do_is_left_alone(self):
        source = 'function Main(quest)\n    quest:Log("x")\nend\n'
        out, count = name_cleanup_closures(source.splitlines(keepends=True))
        self.assertEqual((''.join(out), count), (source, 0))


class GuardWrapperTests(unittest.TestCase):
    def test_a_comment_orphaned_at_the_guard_indent_does_not_abort_the_file(self):
        # an earlier fold can leave a bare comment at the guard's own indent; it carries no semantics,
        # and raising on it used to make build_readable_unit ship the whole file as the raw draft
        source = '''function Main(quest, me)
    if not quest:IsActiveThreadTerminating() then
        quest:Log("work")
    --[[unresolved native value]]
        quest:Log("more")
    end
end
'''
        out, count = fold_guard_wrappers(source.splitlines(keepends=True))
        text = ''.join(out)
        self.assertEqual(count, 1)
        self.assertIn('if quest:IsActiveThreadTerminating() then return end', text)
        self.assertTrue(compiles(text))


class SinkHoistedLocalTests(unittest.TestCase):
    def sink(self, source):
        out, count = name_cleanup_closures(source.splitlines(keepends=True))
        out, count = sink_hoisted_locals(out if count else source.splitlines(keepends=True))
        return ''.join(out), count

    def test_a_single_assignment_whose_block_holds_every_read_is_sunk(self):
        source = '''function Main(quest, me)
    local birdKiller, other
    other = 1
    if quest:GetStateBool("X") then
        birdKiller = quest:GetThingWithScriptName("BirdKiller")
        if birdKiller ~= nil and birdKiller:IsAlive() then
            quest:Log(birdKiller)
        end
    end
    quest:Log(other)
end
'''
        out, count = self.sink(source)
        self.assertEqual(count, 1)
        self.assertIn('    local other\n', out)
        self.assertIn('        local birdKiller = quest:GetThingWithScriptName("BirdKiller")', out)
        self.assertTrue(compiles(out))

    def test_a_goto_from_before_the_sink_point_blocks_it(self):
        # Lua 5.2+ rejects a jump into the scope of a local, so this would not even load
        source = '''function Main(quest, me, a)
    local meleeOpponent
    if a then goto LAB_1 end
    meleeOpponent = quest:GetThingWithScriptName("X")
    ::LAB_1::
    quest:Log(meleeOpponent)
end
'''
        out, count = self.sink(source)
        self.assertEqual(count, 0)
        self.assertTrue(compiles(out))

    def test_a_closure_that_captured_the_name_blocks_it(self):
        source = '''function Main(quest, me, resources, a)
    local movie
    local function ReleaseEverything()
        resources:DestroyMovie(movie)
    end
    if a then
        movie = resources:StartMovie("")
        quest:Log(movie)
    end
end
'''
        self.assertEqual(self.sink(source)[1], 0)

    def test_a_read_outside_the_block_blocks_it(self):
        source = '''function Main(quest, me, a)
    local thing
    if a then
        thing = quest:GetThingWithScriptName("X")
    end
    quest:Log(thing)
end
'''
        self.assertEqual(self.sink(source)[1], 0)

    def test_two_assignments_block_it(self):
        source = '''function Main(quest, me, a)
    local thing
    if a then
        thing = quest:GetThingWithScriptName("X")
        thing = quest:GetThingWithScriptName("Y")
        quest:Log(thing)
    end
end
'''
        self.assertEqual(self.sink(source)[1], 0)


class EnumOperandTests(unittest.TestCase):
    SOURCE = '''-- Readable native conversion: X.
-- Registration remains disabled.

function Main(quest, me, p0, someVar)
    me:MoveToPosition(p0, 3.0, 1, false, true)
    quest:GiveHeroAbility(11, false)
    quest:DisplayTutorial(26)
    quest:EntitySetCutsceneBehaviour(nil, 2)
end
'''

    def test_retail_typed_operands_are_named_from_the_pdb(self):
        out, changed = name_enum_operands(self.SOURCE)
        self.assertEqual(changed, 4)
        self.assertIn('me:MoveToPosition(p0, 3.0, ENTITY_MOVE_RUN, false, true)', out)
        self.assertIn('quest:GiveHeroAbility(HERO_ABILITY_LIGHTNING_SPELL, false)', out)
        self.assertIn('quest:DisplayTutorial(TUTORIAL_CATEGORY_MOVEMENT)', out)
        self.assertIn('quest:EntitySetCutsceneBehaviour(nil, CUTSCENE_BEHAVIOUR_NOT_PAUSED)', out)
        self.assertIn('local ENTITY_MOVE_RUN = 1  -- EScriptEntityMoveType (Ego_r.pdb)', out)
        self.assertTrue(compiles(out))

    def test_a_variable_or_an_out_of_range_number_is_left_alone(self):
        source = '''-- x

function Main(quest, me, p0, someVar)
    me:MoveToPosition(p0, 3.0, someVar, false, true)
    quest:DisplayTutorial(999)
end
'''
        out, changed = name_enum_operands(source)
        self.assertEqual((out, changed), (source, 0))

    def test_the_values_match_the_second_source(self):
        # the FSE headers are the independent check on the PDB dump
        from tools.script_recovery.retail_enums import members
        self.assertEqual(members('EScriptEntityMoveType')[1], 'ENTITY_MOVE_RUN')
        self.assertEqual(members('EHeroAbility')[14], 'HERO_ABILITY_HEAL_LIFE_SPELL')
        self.assertEqual(members('ECutsceneBehaviour')[2], 'CUTSCENE_BEHAVIOUR_NOT_PAUSED')
        self.assertEqual(members('ETutorialCategory')[28], 'TUTORIAL_CATEGORY_QUEST_CARD')


class IntBoolStagingTests(unittest.TestCase):
    """`((C) and 1 or 0) == 0` is the decompiler staging a condition as an int. Both spellings, both
    comparisons, and a condition carrying several separate paren groups."""

    LINE = ('    if ((q:At("L", i):Name() ~= "X") {staged}) {op} 0 '
            'then return "T" else return "F" end\n')
    PREAMBLE = ('local q = {{At=function(_, ...) return {{Name=function() return {value} end}} end}}\n'
                'local i = 1\n')

    def test_every_combination_agrees_with_the_original_under_lua(self):
        for staged in ('and 1 or 0', 'and 0 or 1'):
            for op in ('==', '~='):
                line = self.LINE.format(staged=staged, op=op)
                folded = ''.join(simplify_conditions([line])[0])
                self.assertNotIn(staged, folded)
                for value in ('"X"', '"Y"'):
                    preamble = self.PREAMBLE.format(value=value)

                    def run(text):
                        body = text.replace('if', 'do if', 1).replace('end\n', 'end end\n')
                        return LuaRuntime().execute(preamble + body)

                    with self.subTest(staged=staged, op=op, value=value):
                        self.assertEqual(run(folded), run(line))


if __name__ == '__main__':
    unittest.main()
