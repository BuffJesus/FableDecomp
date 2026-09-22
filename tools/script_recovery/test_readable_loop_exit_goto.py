"""A `goto` that leaves a LOOP is not the tail of the enclosing block (2026-09-22).

`fold_goto_else` turns `if C then goto L end; Y; ::L::` into `if not C then Y end`, skipping any `end` lines
between the rest of the branch and the label. Skipping a *loop's* `end` changes what the jump does: the script
returned, the fold leaves it looping. TraderConflictGood `WatchForKilledPeople` 0x00DFC290 came out as

    while quest:GetStateInt("TradersReachedTeleporter") < 3 do
        if quest:NewScriptFrame() then ... end          -- no exit: a frame-less spin once terminating
    end

which `smoke_run_unit.py --unit trader_conflict --stage readable` reports as
`call trace overflow (loop without frames?)`.
"""
import re
import sys
import unittest
from pathlib import Path

ROOT = Path(__file__).resolve().parents[2]
sys.path.insert(0, str(ROOT / 'tools/script_recovery'))

import readable_style  # noqa: E402

DRAFT = ROOT / 'refs/script_recovery/lifted/TraderConflict/draft/FSE/TraderConflictGood/TraderConflictGood.lua'
READABLE = ROOT / 'refs/script_recovery/lifted/TraderConflict/readable/FSE/TraderConflictGood/TraderConflictGood.lua'


def body(path, name):
    src = path.read_text(encoding='utf-8')
    return re.search(rf'^function {name}\(quest\)\n.*?^end$', src, re.M | re.S).group(0)


class LoopExitGotoTests(unittest.TestCase):
    def test_frame_check_keeps_its_exit(self):
        styled, _ = readable_style.style_function(body(DRAFT, 'WatchForKilledPeople') + '\n', set())
        loop = styled[styled.index('while '):]
        self.assertRegex(loop, r'if not quest:NewScriptFrame\(\) then (?:goto \w+|return) end')
        self.assertNotRegex(loop, r'if quest:NewScriptFrame\(\) then')

    def test_generated_readable_matches(self):
        self.assertRegex(body(READABLE, 'WatchForKilledPeople'),
                         r'if not quest:NewScriptFrame\(\) then (?:goto \w+|return) end')

    def test_openers_name_the_block_each_end_closes(self):
        lines = ['function F()\n', '    while c do\n', '        if x then\n', '        end\n', '    end\n', 'end\n']
        openers = readable_style._block_openers(lines)
        self.assertEqual(openers[3], 'if x then')
        self.assertEqual(openers[4], 'while c do')
        self.assertEqual(openers[5], 'function F()')


if __name__ == '__main__':
    unittest.main()
