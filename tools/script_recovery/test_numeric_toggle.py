"""C's `flag = flag == 0` toggle must stay numeric (2026-09-22, found in-game).

The Skill stage's moving dummies wrapped back to the start of a leg instead of bouncing side to side.
SkillTarget::Main 0x00D41D00 keeps the direction in a char:

    char cStack_16d;
    cStack_16d = '\\0';
    if (cStack_16d == '\\0') { legA } else { legB }
    cStack_16d = cStack_16d == '\\0';

In C that assignment is 1 or 0. Lifted literally it assigns a Lua BOOLEAN, and Lua does not coerce:
`true == 0` is false, so the next flip yields `false`, `false == 0` is false, and the flag freezes on one
leg -- each time the step counter wraps, the dummy teleports back to that leg's start.

The two legs are mirrored in the same function, which is the evidence that retail ping-pongs: one leg
interpolates from the far marker toward the near one, the other from the near back to the far.
"""
import re
import unittest
from pathlib import Path

ROOT = Path(__file__).resolve().parents[2]
SKILL_TARGET = ROOT / 'refs/script_recovery/lifted/GuildTraining/draft/FSE/GuildTraining/Entities/SkillTarget.lua'


class MovingDummyToggleTests(unittest.TestCase):
    def setUp(self):
        self.src = SKILL_TARGET.read_text(encoding='utf-8')

    def test_direction_flag_toggles_numerically(self):
        flips = re.findall(r'^\s*(\w+) = \(\1 == 0\) and 1 or 0\s*$', self.src, re.M)
        self.assertTrue(flips, 'the direction flag is not lifted as a numeric toggle')

    def test_no_comparison_assigned_to_a_numeric_flag(self):
        """`X = X == 0` (bare) would freeze any flag the script also compares against a number."""
        for m in re.finditer(r'^\s*(\w+) = \1 == 0\s*$', self.src, re.M):
            self.fail(f'{m.group(1)} takes a boolean from a self-comparison and is compared to 0 elsewhere')

if __name__ == '__main__':
    unittest.main()
