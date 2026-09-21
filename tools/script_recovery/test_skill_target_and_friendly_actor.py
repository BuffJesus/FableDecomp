"""Two in-game Lua runtime errors from the 2026-09-20 teen play-through (user report: archery target dead,
Whisper fight unfinishable after the Guildmaster's third warning), pinned at the generated source.

* `GuildTraining/Entities/SkillTarget.lua`: retail 0x00D41D00 waits on
  `MsgIsHitByWithProjectileWeapon("SCRIPT_NAME_HERO", &damage)` and then tests `0.0 < damage`. The Forge binding
  returns the damage (number) or nil, so the lifter must bind the out slot to the call's result
  (`OUT_AS_RESULT`) instead of leaving it unassigned (`attempt to compare number with nil` on the first arrow).
* `GuildTraining.lua` CheckFriendlyAttacks (0x00D45060): the PreMeleeMaze lookup at the thread's start and the
  TryAcquire actor operand are ONE Ghidra slot (`auStack_a0`) that the typed export spreads over -0x90/-0x80/-0xa0;
  `restore_stack_operands` must keep them one object ("Retail TryAcquire requires an actor" otherwise).
"""
import re
import unittest
from pathlib import Path

ROOT = Path(__file__).resolve().parents[2]
UNIT = ROOT / 'refs/script_recovery/lifted/GuildTraining'


def read(rel):
    return (UNIT / rel).read_text(encoding='utf-8')


class SkillTargetProjectileTests(unittest.TestCase):
    def test_damage_is_the_result_and_compared(self):
        for stage in ('draft', 'readable_converter'):
            src = read(f'{stage}/FSE/GuildTraining/Entities/SkillTarget.lua')
            m = re.search(r'(\w+) = me:MsgIsHitByHeroWithProjectileWeapon\(\)', src)
            self.assertIsNotNone(m, stage)
            var = m.group(1)
            self.assertRegex(src, rf'0\.0 < {re.escape(var)}\b', stage)
            self.assertNotIn('until me:MsgIsHitByHeroWithProjectileWeapon()', src, stage)


class FriendlyAttackActorTests(unittest.TestCase):
    def test_try_acquire_actor_is_the_maze_lookup(self):
        for stage in ('draft', 'readable_converter'):
            src = read(f'{stage}/FSE/GuildTraining/GuildTraining.lua')
            body = re.search(r'^function CheckFriendlyAttacks\(quest\)\n.*?^end$', src, re.M | re.S).group(0)
            m = re.search(r'(\w+) = quest:GetThingWithScriptName\("PreMeleeMaze"\)', body)
            self.assertIsNotNone(m, stage)
            actor = m.group(1)
            acquires = re.findall(r'resources:TryAcquire\((\w+), (\w+), 4\)', body)
            self.assertTrue(acquires, stage)
            self.assertEqual(acquires[0][1], actor, (stage, acquires))     # the Maze acquire (the hero's follows)


if __name__ == '__main__':
    unittest.main()
