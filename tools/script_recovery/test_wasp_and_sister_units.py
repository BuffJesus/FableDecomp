"""The first conversion of Q_WaspBoss and QS_GuardianSisterInfo (2026-09-22), and the generic converter
gaps it exposed.

* A thing vcall spelled through the object's first dword (`(**(code **)(X._0_4_ + 0x12c))()`) is the same
  call as `*(int *)X + 0x12c`: WaspAttacker 0x00E11480 loops while the WaspVictim is alive.
* The RAW hidden return makes its out slot a thing: `(**(code **)(**(int **)(this + 4) + 0x120))(recv,
  &slot, &name)`. `annotate` runs on the raw decompile (it is what produces the `GSI->` names) and the
  hidden return is the SECOND argument, after the repeated receiver.
* `AreAllThingsInVectorDead` takes the VECTOR. WaspBoss Main 0x00E0EA40 polls it on its own
  `GetAllThingsWithScriptName` lists, and the element-indexing rewrite must not turn the argument into
  `list[0 + 1]` (the first drone).
* A staged bool literal must survive a block boundary: helper_E12F20 0x00E12F20 stages `isGold = false`
  between a termination check and its test, and the `if .. return end` in between dropped it, leaving
  `KickOffQuestStartScreen(pQuestName, bVar4, isGold)` reading a free global (nil, not false).
"""
import re
import unittest
from pathlib import Path

ROOT = Path(__file__).resolve().parents[2]
WASP = ROOT / 'refs/script_recovery/lifted/WaspBoss/draft/FSE/WaspBoss'
SISTER = ROOT / 'refs/script_recovery/lifted/GuardianSisterInfo/draft/FSE'


def read(path):
    return path.read_text(encoding='utf-8')


class WaspBossTests(unittest.TestCase):
    def test_no_unresolved_thing_vcalls(self):
        for lua in sorted(WASP.rglob('*.lua')):
            with self.subTest(file=lua.name):
                self.assertNotRegex(read(lua), r'TODO\(native\).*\+ 0x12c\)\)\(\)')

    def test_victim_liveness_loop_lifts(self):
        body = read(WASP / 'Entities/WaspAttacker.lua')
        self.assertRegex(body, r'(\w+) = quest:GetThingWithScriptName\("WaspVictim"\)')
        self.assertRegex(body, r':IsAlive\(\)')

    def test_all_dead_takes_the_whole_list(self):
        body = read(WASP / 'WaspBoss.lua')
        self.assertIn('local function __native_all_dead(list)', body)
        calls = re.findall(r'__native_all_dead\(([^)]*)\)', body)
        self.assertTrue(calls)
        for arg in calls:
            if arg == 'list':
                continue                      # the helper's own definition
            self.assertNotIn('[', arg, f'an element, not the vector: {arg!r}')

    def test_quest_start_screen_keeps_its_staged_bool(self):
        self.assertRegex(read(WASP / 'WaspBoss.lua'),
                         r'quest:KickOffQuestStartScreen\([^)]*, (?:true|false)\)')

    def test_every_wasp_entity_converted(self):
        names = {p.stem for p in (WASP / 'Entities').glob('*.lua')}
        self.assertLessEqual({'WaspHelper', 'QueenHornet', 'HornetDrone', 'WaspChaser', 'WaspChaseWoman',
                              'WaspAttacker', 'WaspVictim', 'FleeingWoman', 'GratefulVillagerSpawn'}, names)


class GuardianSisterInfoTests(unittest.TestCase):
    def test_main_waits_for_the_slums_then_sets_the_time(self):
        body = read(SISTER / 'QS_GuardianSisterInfo/QS_GuardianSisterInfo.lua')
        self.assertIn('quest:SetQuestCardObjective(', body)
        self.assertIn('"BowerstoneSlums"', body)
        self.assertRegex(body, r'quest:IsRegionLoaded\("BowerstoneSlums"\)')
        self.assertRegex(body, r'quest:SetTimeOfDay\(10(?:\.0)?\)')

    def test_both_sister_quests_are_in_the_unit(self):
        packages = {p.name for p in SISTER.iterdir() if p.is_dir()}
        self.assertIn('QS_GuardianSisterInfo', packages)
        self.assertIn('QS_GuardianSisterInfo2_SisterInBanditCamp', packages)

    def test_maze_entity_is_bound(self):
        self.assertIn('quest:AddEntityBinding("MazeAtTavern"',
                      read(SISTER / 'QS_GuardianSisterInfo/QS_GuardianSisterInfo.lua'))


if __name__ == '__main__':
    unittest.main()
