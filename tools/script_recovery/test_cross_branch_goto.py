"""A jump into a sibling branch's nested block reaches its target in the converted Lua.

The 2026-09-19 in-game defect: retail `TheRealGuildmaster::Main` (PreMelee, 0x00D52E90) shares the stick-counter
block (`PreMeleeMode = 2`, `AddQuestInfoCounter("HUD_QUEST_ICON_TARGET_DUMMY", 7, 1.0)`, the DummyHits loop)
between the Xbox and PC halves of the tutorial: the PC half (`TEXT_QST_028_PREMELEE_INSTRUCTIONS_STICK_PC`) ends
in `goto LAB_00d53c7e`, a label Ghidra prints inside the Xbox half's nested ifs. The converter used to drop that
jump (`-- TODO(native): goto LAB_00d53c7e`) and the PC script fell out of the block: no counter, no dummy stage.
`native_goto_scopes.hoist_shared_tails` now moves the shared tail out of the branch that hides it.

This runs the draft and the readable `Main` through a mock quest with `IsXbox() == false` and `DummyHits`
already at 7 (both counter loops exit at once) and asserts that both tails run: the punch stage
(`PreMeleeMode = 1`) and the stick stage (`PreMeleeMode = 2`), each with its `AddQuestInfoCounter`.

The evening's second defect (`WoodsStageTests`): the woods loop `do { ... } while (cStack_169 != 0)` lost its
exit. Retail clears that flag on the YES answer to `TEXT_QST_028_GUILDMASTER_PREMELEE_END_QUESTION`
(`mov byte ptr [esp+0x33], 0` at 0x00D54DF0, ESP = entry-412 there, so -0x169) and the walk-back flag
`cStack_161` on NO (0x00D54F17), but Ghidra's stack model had drifted 4 bytes at the untyped
`(**(code **)(*piVar1 + 0x5ec))()` (the `push 1` it dropped), so both stores were printed against the wrong
slot (`uStack_170 = uStack_170 & 0xffffff;`, `CStack_168._3_1_ = 0;`), the lifter dropped them and the loop
became `until false`: YES never reached `HeroSleeps`, the quest never ended. `convert_quest_unit.
_drifted_byte_slices` re-slots such byte stores by the drift of the temporaries constructed beside them.
"""
import re
import unittest
from pathlib import Path

from lupa.lua54 import LuaRuntime

from tools.script_recovery.native_goto_scopes import hoist_shared_tails, _parse, _unexpressible

ROOT = Path(__file__).resolve().parents[2]
UNIT = ROOT / 'refs/script_recovery/lifted/GuildTraining'
STAGES = {
    'draft': UNIT / 'draft/FSE/GuildTrainingPreMelee/Entities/TheRealGuildmaster.lua',
    'readable_converter': UNIT / 'readable_converter/FSE/GuildTrainingPreMelee/Entities/TheRealGuildmaster.lua',
}

MOCK = r'''
local events, state = {}, {DummyHits = 7, GuildmasterTeleport = true}
local terminating = false
local counters, frames = 0, 0
local function record(...) events[#events + 1] = {...} end
local function thing(name)
    return setmetatable({name = name}, {__index = function(_, method)
        if method == 'IsTalkedToByHero' then return function() return true end end
        if method == 'IsPerformingScriptTask' then return function() return false end end
        if method == 'IsAlive' then return function() return true end end
        if method == 'GetPos' then return function() return {x = 0, y = 0, z = 0} end end
        return function() return nil end
    end})
end
local hero = thing('hero')
local numbers = {GetTimer = 0, GetHealth = 100, GetMasterGameState = 0, RegisterTimer = 1, AddNewConversation = 1,
                 GiveHeroYesNoQuestion = 1, AddQuestInfoTickByText = 1}
local booleans = {IsXbox = false, MsgIsGameInfoClickedPast = true, MsgIsQuestionAnsweredYesOrNo = true,
                  IsQuestActive = false, IsDistanceBetweenThingsUnder = false, IsDistanceBetweenThingsOver = true,
                  IsConversationActive = false, MsgIsTutorialClickedPast = true}
local things = {GetHero = hero, GetThingWithScriptName = thing('marker'), CreateExperienceOrb = thing('orb')}
local resources = setmetatable({}, {__index = function(_, method)
    if method == 'TryAcquire' then return function() return true end end
    if method == 'IsAcquired' then return function() return true end end
    return function() return {} end
end})
local quest = setmetatable({}, {__index = function(_, method)
    if method == 'RetailResources' then return function() return resources end end
    if method == 'NewScriptFrame' then
        return function()
            frames = frames + 1
            if frames > 500 then terminating = true end       -- a stage the mock cannot satisfy: wind down
            return not terminating
        end
    end
    if method == 'IsActiveThreadTerminating' then return function() return terminating end end
    if method == 'GetStateBool' then return function(_, name) return state[name] end end
    if method == 'GetStateInt' then
        return function(_, name) if name == 'DummyHits' then return 7 end return state[name] end   -- the dummy is already down
    end
    if method == 'SetStateInt' or method == 'SetStateBool' then
        return function(_, name, value) state[name] = value; record(method, name, value) end
    end
    if method == 'AddQuestInfoCounter' then
        return function(_, key, count, scale)
            record(method, key, count, scale)
            counters = counters + 1
            if counters == 2 then terminating = true end   -- both stages reached: wind the script down
            return counters
        end
    end
    if numbers[method] ~= nil then return function() return numbers[method] end end
    if booleans[method] ~= nil then return function() return booleans[method] end end
    if things[method] ~= nil then return function() return things[method] end end
    return function(_, ...) record(method, ...) end
end})
return quest, thing('guildmaster'), events, state
'''


def run_main(stage, mock=None):
    lua = LuaRuntime()
    lua.execute(STAGES[stage].read_text(encoding='utf-8'))
    quest, me, events, state = lua.execute(mock or MOCK)
    lua.execute('debug.sethook(function() error("instruction budget exhausted") end, "", 5000000)')
    lua.globals().Main(quest, me)
    return [tuple(e.values()) for e in events.values()], dict(state)


# Past the woods: `ScorpionsDestroyedCutscenePlayed` true, `Q_GuildTrainingWoodsMelee` no longer active, the
# dummy already down, the hero talks once at the start (the first talk loop) and again after every NO answer
# (retail re-asks from the `IsTalkedToByHero` branch). `answers` are the successive replies to
# `MsgIsQuestionAnsweredYesOrNo` (1 = yes, 0 = no); an unanswered question winds the script down by frame budget.
WOODS_MOCK = r'''
local answers = {%s}
local events, state = {}, {GuildmasterTeleport = true}
local terminating, frames, asked, talked, orbPolls = false, 0, 0, true, 0
local function record(...) events[#events + 1] = {...} end
local function thing(name)
    return setmetatable({name = name}, {__index = function(_, method)
        if method == 'IsTalkedToByHero' then return function() local t = talked; talked = false; return t end end
        if method == 'IsPerformingScriptTask' then return function() return false end end
        -- the XP orb is alive for one poll, then collected (retail waits `while orb:IsAlive()`; live since the
        -- 2026-09-21 counted-pointer fold landed the orb copy -- before, the polled thing was nil)
        if method == 'IsAlive' then return function() if name == 'orb' then orbPolls = orbPolls + 1; return orbPolls <= 1 end return true end end
        if method == 'GetPos' then return function() return {x = 1, y = 2, z = 3} end end
        if method == 'Speak' then return function(_, _, key) record(name .. ':Speak', key); return true end end
        return function(_, ...) record(name .. ':' .. method, ...) end
    end})
end
local hero = thing('hero')
local numbers = {GetTimer = 0, GetHealth = 100, RegisterTimer = 1, AddNewConversation = 1, AddQuestInfoCounter = 1,
                 AddQuestInfoTickByText = 1}
local booleans = {IsXbox = false, MsgIsGameInfoClickedPast = true, IsQuestActive = false,
                  IsDistanceBetweenThingsUnder = false, IsDistanceBetweenThingsOver = true, IsConversationActive = false,
                  MsgIsTutorialClickedPast = true, DisplayTutorial = false}
local master = {ScorpionsDestroyedCutscenePlayed = true}
local things = {GetHero = hero, GetThingWithScriptName = thing('marker'), CreateExperienceOrb = thing('orb')}
local resources = setmetatable({}, {__index = function(_, method)
    if method == 'TryAcquire' or method == 'IsAcquired' then return function() return true end end
    if method == 'RunMacro' then return function(_, name) record('RunMacro', name) end end
    if method == 'ScriptThing' then return function() return thing('guildmaster') end end
    return function() return {} end
end})
local quest = setmetatable({}, {__index = function(_, method)
    if method == 'RetailResources' then return function() return resources end end
    if method == 'NewScriptFrame' then
        return function() frames = frames + 1; if frames > 400 then terminating = true end; return not terminating end
    end
    if method == 'IsActiveThreadTerminating' then return function() return terminating end end
    if method == 'GetStateBool' then return function(_, name) return state[name] end end
    if method == 'GetStateInt' then return function(_, name) if name == 'DummyHits' then return 7 end return state[name] end end
    if method == 'GetMasterGameState' then return function(_, name) return master[name] end end
    if method == 'SetStateInt' or method == 'SetStateBool' then
        return function(_, name, value) state[name] = value; record(method, name, value) end
    end
    if method == 'GiveHeroYesNoQuestion' then
        return function(_, key) asked = asked + 1; record(method, key); if answers[asked] == 0 then talked = true end end
    end
    if method == 'MsgIsQuestionAnsweredYesOrNo' then return function() return answers[asked] or -1 end end
    if numbers[method] ~= nil then return function() return numbers[method] end end
    if booleans[method] ~= nil then return function() return booleans[method] end end
    if things[method] ~= nil then return function() return things[method] end end
    return function(_, ...) record(method, ...) end
end})
return quest, thing('guildmaster'), events, state
'''


class CrossBranchGotoTests(unittest.TestCase):
    def test_pc_stick_path_reaches_the_shared_counter_block(self):
        for stage, path in STAGES.items():
            with self.subTest(stage=stage):
                source = path.read_text(encoding='utf-8')
                self.assertIn('INSTRUCTIONS_STICK_PC', source)
                self.assertNotIn('TODO(native): goto', source)
                events, state = run_main(stage)
                modes = [e for e in events if e[:2] == ('SetStateInt', 'PreMeleeMode')]
                self.assertEqual([m[2] for m in modes], [1, 2], events[-20:])
                counters = [e for e in events if e[0] == 'AddQuestInfoCounter']
                self.assertEqual(counters, [('AddQuestInfoCounter', 'HUD_QUEST_ICON_TARGET_DUMMY', 7, 1.0)] * 2)
                # the PC logbook entry precedes the stick counter: the jump came from the PC half
                pc_log = next(i for i, e in enumerate(events) if e[:2] == ('AddLogbookTutorialEntryPC', 'TEXT_QST_LOG_COMBAT_MELEE'))
                stick = next(i for i, e in enumerate(events) if e[:3] == ('SetStateInt', 'PreMeleeMode', 2))
                self.assertLess(pc_log, stick)

    def test_hoist_moves_the_shared_tail_after_the_chain(self):
        statements = '''{
if (X) {
pre();
if (Y) {
LAB_00000010:
tail1();
tail2();
}
}
else {
pc();
goto LAB_00000010;
}
after();
}'''.split('\n')
        before = _parse(statements)
        self.assertEqual(len(_unexpressible(before)), 1)
        out = hoist_shared_tails(statements)
        self.assertEqual(_unexpressible(_parse(out)), [])
        text = '\n'.join(s.strip() for s in out)
        self.assertIn('if (Y) {\ngoto LAB_00000010;\n}', text)
        self.assertIn('pc();\ngoto LAB_00000010;\n}\ngoto FLOW_past_lab_00000010;\nLAB_00000010:\ntail1();\ntail2();\nFLOW_past_lab_00000010:\nafter();', text)
        self.assertEqual(text.count('tail1();'), 1, 'a move, not a copy')

    def test_hoist_leaves_loops_and_epilogues_alone(self):
        into_loop = '''{
while (c) {
LAB_00000020:
body();
}
goto LAB_00000020;
}'''.split('\n')
        self.assertEqual(hoist_shared_tails(into_loop), into_loop)
        epilogue = '''{
if (X) {
work();
if (bad) {
LAB_00000030:
release();
return;
}
more();
}
else {
if (bad) goto LAB_00000030;
}
}'''.split('\n')
        self.assertEqual(hoist_shared_tails(epilogue), epilogue, 'straight-line cleanup belongs to native_cleanup_regions')

    def test_no_residue_in_the_guild_unit(self):
        for stage in ('draft', 'readable_converter'):
            hits = [p for p in (UNIT / stage).rglob('*.lua') if re.search(r'TODO\(native\): goto', p.read_text(encoding='utf-8'))]
            self.assertEqual(hits, [], stage)


class WoodsStageTests(unittest.TestCase):
    """The woods loop after the beetles: YES ends the stage, NO walks back and re-asks, PUNCH never replays."""

    def woods(self, stage, answers):
        events, state = run_main(stage, WOODS_MOCK % answers)
        self.assertEqual([e[1] for e in events if e[0] == 'RunMacro' and e[1] == 'CS_GUILD_PREMELEE_PUNCH'],
                         ['CS_GUILD_PREMELEE_PUNCH'], 'the punch stage runs exactly once')
        self.assertIn(('RunMacro', 'CS_GUILD_MELEE_WOODSWON'), events)
        # retail marks him green on the walk to the woods door, then with the quest orb once the beetles are dead
        self.assertEqual([e[2] for e in events if e[0] == 'MiniMapAddMarker'], ['HUD_ORB_GREEN_SMALL', 'HUD_ORB_QUEST_CORE'])
        return events, state

    def test_yes_plays_the_avi_and_ends_the_stage(self):
        for stage in STAGES:
            with self.subTest(stage=stage):
                events, state = self.woods(stage, '1')
                questions = [e for e in events if e[0] == 'GiveHeroYesNoQuestion']
                self.assertEqual(questions, [('GiveHeroYesNoQuestion', 'TEXT_QST_028_GUILDMASTER_PREMELEE_END_QUESTION')])
                avi = next(i for i, e in enumerate(events) if e[0] == 'PlayAVIMovie')
                self.assertIn('2_guild_split_1_comp.xmv', events[avi][1])
                sleeps = next(i for i, e in enumerate(events) if e[:3] == ('SetStateBool', 'HeroSleeps', True))
                self.assertLess(avi, sleeps, 'the loop exits on YES: HeroSleeps ends the PreMelee quest')
                self.assertNotIn('guildmaster:MoveToPosition', [e[0] for e in events[avi:]])

    def test_no_walks_back_to_the_woods_guard_marker_and_waits(self):
        for stage in STAGES:
            with self.subTest(stage=stage):
                events, state = self.woods(stage, '0')
                self.assertNotIn('PlayAVIMovie', [e[0] for e in events])
                self.assertNotIn('HeroSleeps', state)
                no = next(i for i, e in enumerate(events) if e[:2] == ('guildmaster:Speak', 'TEXT_QST_028_GUILDMASTER_PREMELEE_END_NO'))
                # (the first MoveToPosition is the alarm stage's walk to the woods door)
                walk = next(i for i, e in enumerate(events) if e[0] == 'guildmaster:MoveToPosition' and i > no)
                self.assertEqual(events[walk][2:], (1.0, 0, False, True), 'MK_GTM_WD_GUARD, walk, radius 1.0')
                # talked to again: the retail re-ask (`TEXT_QST_028_GUILDMASTER_PREMELEE_END` then the question)
                self.assertEqual([e[1] for e in events if e[0] == 'GiveHeroYesNoQuestion'],
                                 ['TEXT_QST_028_GUILDMASTER_PREMELEE_END_QUESTION'] * 2)

    def test_no_then_yes_on_the_second_talk(self):
        for stage in STAGES:
            with self.subTest(stage=stage):
                events, state = self.woods(stage, '0, 1')
                self.assertEqual(sum(1 for e in events if e[0] == 'PlayAVIMovie'), 1)
                self.assertTrue(state.get('HeroSleeps'))


class DriftedByteSliceTests(unittest.TestCase):
    def test_store_and_read_follow_the_constructed_temporaries_drift(self):
        from tools.script_recovery.convert_quest_unit import _drifted_byte_slices
        text = ('  uint uStack_170;\n  char cStack_169;\n  CCharString CStack_168;\n  char cStack_161;\n'
                '  CCharString::CCharString(&CStack_78,"x",-1);\n'
                '  uStack_170 = uStack_170 & 0xffffff;\n'
                '  CStack_168._3_1_ = 0;\n'
                "  if (CStack_168._3_1_ != '\\0') {\n")
        pos = text.index('CCharString::CCharString(&CStack_78')
        uses = [(pos, 0, 'CStack_78', -116, 0, True)]            # export slot -116 = -0x74: drift +4
        edits = sorted(_drifted_byte_slices(text, uses))
        self.assertEqual([text[a:b] + ' -> ' + r for a, b, r in edits],
                         ['  uStack_170 = uStack_170 & 0xffffff; ->   cStack_169 = 0;',
                          '  CStack_168._3_1_ = 0; ->   cStack_161 = 0;',
                          'CStack_168._3_1_ -> cStack_161'])
        # no drift: the slice below `uStack_170` has no declared byte local, nothing changes
        uses = [(pos, 0, 'CStack_78', -120, 0, True)]
        self.assertEqual(list(_drifted_byte_slices(text, uses)), [])


if __name__ == '__main__':
    unittest.main()
