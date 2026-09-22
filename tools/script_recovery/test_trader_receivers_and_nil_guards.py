"""TraderConflictGood TraderToRescue 0x00DFE0F0 (2026-09-22) and the receiver evidence behind it.

* Two `IsAlive()` tests print NO receiver (`(**(code **)(NAME + 0x12c))()`); Ghidra's NAME there is a drifted
  spelling four slots off, so the object fold read them as calls on the entity's control *resource*. The typed
  export's `ecxStack` names the true slot (-0x134 / -0x130, the second drifted into the thing's +4), which is
  the hostage keeper the trader waits on.
* `GetHeroTargetedThing()` returns a CScriptThing per the manifest, so its result is a thing receiver
  (`IsEqualTo`); the lowering folds the Data field back onto the handle (`*(int *)(recv + 0x0)`).
* Forge represents an empty CScriptThing as nil, so every CONST BOOL query on a looked-up thing needs the nil
  guard retail gets for free (`GetHeroTargetedThing()` is nil on every frame the hero targets nothing).
* The resource constructor's member zero-stores belong to the constructor, including the `X[0] = 0` spelling
  (plain `0`, not `0x0`) -- it used to survive as `TODO(native): xStack_148[0] = 0;`.
"""
import re
import unittest
from pathlib import Path

ROOT = Path(__file__).resolve().parents[2]
MAIN = ROOT / 'refs/script_recovery/lifted/TraderConflict/draft/FSE/TraderConflictGood/Entities/TraderToRescue.lua'


class TraderToRescueMainTests(unittest.TestCase):
    def setUp(self):
        self.src = MAIN.read_text(encoding='utf-8')
        self.main = re.search(r'^function Main\(quest, me\)\n.*?^end$', self.src, re.M | re.S).group(0)

    def test_liveness_tests_are_the_hostage_keeper(self):
        m = re.search(r'(\w+) = quest:GetNearestWithScriptName\(me, "TC_BanditHostageKeeper"\)', self.main)
        self.assertIsNotNone(m)
        keeper = m.group(1)
        alive = re.findall(r'\((\w+) ~= nil and \1:IsAlive\(\)\)', self.main)
        self.assertEqual(alive, [keeper, keeper], alive)
        self.assertNotIn('+ 0x12c))()', self.main)          # no unresolved vtable head is left

    def test_hero_targeting_check_is_a_guarded_thing_call(self):
        m = re.search(r'(\w+) = quest:GetHeroTargetedThing\(\)\n\s*\w+ = \((\w+) ~= nil and \2:IsEqualTo\(me\)\)',
                      self.main)
        self.assertIsNotNone(m, self.main[self.main.find('GetHeroTargetedThing') - 200:][:400])
        self.assertEqual(m.group(1), m.group(2))

    def test_outro_helper_keeps_its_cutscene_name(self):
        # 0x00E00589: `CCharString("CS_TRADERCON_GOOD_OUTRO"); mov ecx,[ebp+0x14]; call 0xdfded0` -- Ghidra printed
        # the receiver and dropped the by-value string, so the outro cutscene never reached the Lua
        self.assertRegex(self.main, r'helper_DFDED0\(quest, me, "CS_TRADERCON_GOOD_OUTRO"\)')

    def test_resource_constructor_leaves_no_member_store(self):
        self.assertNotRegex(self.main, r'TODO\(native\): \w+\[0\] = 0;')
        self.assertRegex(self.main, r'\w+ = resources:NewResource\(\)\n\s*resources:PrepareResource\(')


if __name__ == '__main__':
    unittest.main()
