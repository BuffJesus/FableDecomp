"""Q_GuildTraining's `CheckFriendlyAttacks` thread (0x00D45060) survives the return to the Guild.

Two regressions this pins (2026-09-20, v6 run `work/ab_runs/v6-20260920-080006`): after the second
`GetAllCreaturesExcludingHero` fill the vector's end slot (Ghidra `puStack_90`, drifted 0x10 from its true
slot) sits on the PreMeleeMaze CScriptThing's base and `canonicalise_stack_objects` folded it onto the thing,
so the count lifted to `preMeleeMaze - creatures2 >> 31` (arithmetic on userdata: the thread died the moment
the hero re-entered HeroGuildComplex); and the four `GetDefName` vcalls on `((int)V + i)` elements stayed
native (`at_vcall_local` only accepted a bare index), so the def-name compares lifted to `nil == "CREATURE_..."`
and the first loop set EVERY creature friendly + unkillable instead of skipping the sparrows / apprentices / Maze.
"""
import re
import unittest
from pathlib import Path

from lupa.lua54 import LuaRuntime

ROOT = Path(__file__).resolve().parents[2]
UNIT = ROOT / 'refs/script_recovery/lifted/GuildTraining'
STAGES = {
    'draft': UNIT / 'draft/FSE/GuildTraining/GuildTraining.lua',
    'readable_converter': UNIT / 'readable_converter/FSE/GuildTraining/GuildTraining.lua',
}
NAMES = ['CREATURE_BIRD_GUILD_SPARROW', 'CREATURE_GUILD_GUARD_01', 'CREATURE_RIVAL_HERO_MAZE',
         'CREATURE_RIVAL_HERO_WHISPER_APPRENTICE', 'CREATURE_APPRENTICE_02']


def function_source(path):
    text = path.read_text(encoding='utf-8')
    m = re.search(r'^function CheckFriendlyAttacks\(quest\)\n.*?^end$', text, re.M | re.S)
    assert m, path
    return m.group(0)


class CheckFriendlyAttacksTests(unittest.TestCase):
    def run_stage(self, stage):
        src = function_source(STAGES[stage])
        lua = LuaRuntime(unpack_returned_tuples=True)
        events = []
        loaded = [True, False, True]     # in the Guild, then away (the woods), then back

        def creature(name):
            t = lua.table()
            t.GetDefName = lambda _: name
            t.SetFriendsWithEverythingFlag = lambda _, v: events.append(('friends', name))
            t.name = name
            return t
        creatures = lua.table(*[creature(n) for n in NAMES])
        q = lua.table()
        hero = lua.table()
        for name in ('MsgHitFriendWithBareHands', 'MsgHitFriendWithMeleeWeapon', 'MsgHitFriendWithRangedWeapon'):
            hero[name] = lambda _: False
        q.GetHero = lambda _: hero
        q.RetailResources = lambda _: lua.table()
        q.GetThingWithScriptName = lambda _, name: lua.table(scriptName=name)
        q.GetAllCreaturesExcludingHero = lambda _: creatures
        frames = [0]

        def frame(*_):
            frames[0] += 1
            return frames[0] < 12
        q.IsActiveThreadTerminating = lambda _: frames[0] >= 12
        q.IsLevelLoaded = lambda _, name: loaded.pop(0) if loaded else True
        q.NewScriptFrame = frame
        q.EntitySetAsKillable = lambda _, thing, a, b: events.append(('killable', thing.name, a, b))
        lua.execute(src + '\nreturn CheckFriendlyAttacks')
        fn = lua.eval('CheckFriendlyAttacks')
        fn(q)
        return events

    def test_source_has_no_residue(self):
        for stage, path in STAGES.items():
            # the two creature loops only (the friend-hit checks after them carry a separate, older residue)
            src = function_source(path).split('MsgHitFriendWithBareHands')[0]
            self.assertNotRegex(src, r'nil == "CREATURE_|nil ~= "CREATURE_', stage)
            self.assertNotRegex(src, r'>> 31|>> 0x1f', stage)
            self.assertNotRegex(src, r'\w+ - creatures\d* ?\)? ?/ ?12', stage)
            self.assertNotIn('unresolved native value', src, stage)

    def test_only_unlisted_creatures_are_made_friendly(self):
        for stage in STAGES:
            with self.subTest(stage=stage):
                events = self.run_stage(stage)
                friendly = [e[1] for e in events if e[0] == 'friends']
                killable = [e[1] for e in events if e[0] == 'killable']
                # native: the sparrow, the two Whisper apprentices and Maze are skipped; everything else is
                # set friends-with-everything and not killable -- on the first pass AND after the return
                self.assertEqual(friendly[:2], ['CREATURE_GUILD_GUARD_01', 'CREATURE_APPRENTICE_02'], stage)
                self.assertEqual(killable[:2], ['CREATURE_GUILD_GUARD_01', 'CREATURE_APPRENTICE_02'], stage)
                self.assertGreaterEqual(len(friendly), 4, f'{stage}: no second pass after the level reload')
                self.assertNotIn('CREATURE_RIVAL_HERO_MAZE', friendly, stage)
                self.assertNotIn('CREATURE_BIRD_GUILD_SPARROW', friendly, stage)


if __name__ == '__main__':
    unittest.main()
