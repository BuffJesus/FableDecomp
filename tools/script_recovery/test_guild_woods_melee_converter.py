"""The converter's Q_GuildTrainingWoodsMelee (draft and readable) survives the Guild Woods entry seam.

Two regressions this pins (2026-09-19): `Main` lifted VC7.1's exception-state flag as `local CVar4 ...
if (CVar4 & 2) ~= 0` -- `nil & 2`, a Lua 5.4 runtime error right after FinalizeEntityBindings, i.e. on
entering Guild Woods; and `DoMission` re-polled `IsLevelLoaded("")` because the callOrder pairing fell
back to address order over a namespace-stripped `CQ_CinemaTestScript::EndMission(` label.
"""
import re
import unittest
from pathlib import Path

from lupa.lua54 import LuaRuntime

ROOT = Path(__file__).resolve().parents[2]
UNIT = ROOT / 'refs/script_recovery/lifted/GuildTraining'
STAGES = {
    'draft': UNIT / 'draft/FSE/GuildTrainingWoodsMelee/GuildTrainingWoodsMelee.lua',
    'readable_converter': UNIT / 'readable_converter/FSE/GuildTrainingWoodsMelee/GuildTrainingWoodsMelee.lua',
}


def quest_stub(lua, loaded, state, events, frames_until_done=2):
    q = lua.table()
    frames = 0
    q.SetStateBool = lambda _, name, value: (state.__setitem__(name, value), events.append((name, value)))
    q.GetStateBool = lambda _, name: state[name]
    q.IsLevelLoaded = lambda _, name: (events.append(('level', name)), loaded.pop(0) if loaded else True)[1]

    def frame(*_):
        nonlocal frames
        frames += 1
        if frames == frames_until_done:
            state['ScorpionsAlive'] = False
        return True
    q.NewScriptFrame = frame
    q.IsActiveThreadTerminating = lambda _: False
    q.AddEntityBinding = lambda _, name, path, *flags: events.append(('bind', name, path))
    q.FinalizeEntityBindings = lambda _: events.append('finalize')
    q.CreateThread = lambda _, name: events.append(('thread', name))
    q.GiveHeroNewQuestObjective = lambda _, *a: events.append(('objective',) + a)
    return q


class GuildWoodsMeleeConverterTests(unittest.TestCase):
    def run_main(self, stage):
        source = STAGES[stage].read_text(encoding='utf-8')
        lua = LuaRuntime()
        lua.execute(source)
        events, state = [], {'ScorpionsAlive': True, 'MissionSucceeded': False, 'MissionFailed': False, 'MissionOver': False}
        q = quest_stub(lua, [False, True], state, events)
        lua.globals().Main(q)          # raises LuaError on `nil & 2`
        return source, events, state

    def test_main_passes_the_level_gate_and_starts_both_threads(self):
        for stage in STAGES:
            with self.subTest(stage=stage):
                source, events, state = self.run_main(stage)
                self.assertIn(('bind', 'ScorpionHome', 'GuildTrainingWoodsMelee/Entities/ScorpionHome'), events)
                self.assertIn(('thread', 'WatchForTermination'), events)
                self.assertIn(('thread', 'DoMission'), events)
                self.assertTrue(state['MissionSucceeded'])
                self.assertNotRegex(source, r'\b\w+ & 0x[0-9a-f]+ ~= 0|\(\w+ & \d+\) ~= 0',
                                    'exception-state flag bookkeeping survived into the Lua')

    def test_do_mission_repolls_guild_woods_not_the_empty_string(self):
        for stage in STAGES:
            with self.subTest(stage=stage):
                source = STAGES[stage].read_text(encoding='utf-8')
                body = re.search(r'^function DoMission\(quest\)(.*?)^end', source, re.S | re.M).group(1)
                polls = re.findall(r'IsLevelLoaded\("([^"]*)"\)', body)
                self.assertEqual(polls, ['GuildWoods', 'GuildWoods'], body)


if __name__ == '__main__':
    unittest.main()
