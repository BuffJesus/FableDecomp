"""A loop entered in the middle by a goto is rotated, not copied past (2026-09-26).

GTDI_Maze Main (0x00E27E90) retries the hero's TryAcquire in a loop only reachable through
`if (!bVar5) goto LAB_00e281e0;`, where the label sits after the loop's own `TryAcquire ... break`:

    goto LAB_00e28c8a;
    while( true ) {
      bVar5 = RESOURCE_TryAcquire(res, hero, 4);
      if (bVar5) break;
    LAB_00e281e0:
      GSI->NewScriptFrame();
      ...
    }

duplicate_sibling_tails copied the label's tail to the jump site and then jumped PAST the loop, so a failed
first acquire went on without the hero resource (Q_BanditCampBossBattle's alarm loop over the defensive
guards was skipped the same way). Rotating the loop puts the label in front of it: the jump lands on a
visible label and every retry iteration is kept.
"""
import unittest

from tools.script_recovery.native_goto_scopes import rotate_mid_entered_loops


class RotateMidEnteredLoopTests(unittest.TestCase):
    RETRY = ['  if (!ok) goto LAB_00e281e0;',
             '  goto LAB_00e28c8a;',
             '  while( true ) {',
             '    ok = RESOURCE_TryAcquire(res, hero, 4);',
             '    if (ok) break;',
             'LAB_00e281e0:',
             '    GSI->NewScriptFrame();',
             '  }',
             'LAB_00e28c8a:']

    def test_loop_is_rotated_behind_its_label(self):
        out = rotate_mid_entered_loops(self.RETRY)
        self.assertEqual(out, ['  if (!ok) goto LAB_00e281e0;',
                               '  goto LAB_00e28c8a;',
                               '  LAB_00e281e0:',
                               '  while( true ) {',
                               '    GSI->NewScriptFrame();',
                               '    ok = RESOURCE_TryAcquire(res, hero, 4);',
                               '    if (ok) break;',
                               '  }',
                               'LAB_00e28c8a:'])

    def test_loop_reachable_by_fall_through_is_left_alone(self):
        text = [l for l in self.RETRY if l != '  goto LAB_00e28c8a;']
        self.assertEqual(rotate_mid_entered_loops(text), text)

    def test_continue_in_body_is_left_alone(self):
        text = self.RETRY[:6] + ['    continue;'] + self.RETRY[6:]
        self.assertEqual(rotate_mid_entered_loops(text), text)

    def test_jump_from_inside_the_loop_is_left_alone(self):
        text = self.RETRY[:4] + ['    if (x) goto LAB_00e281e0;'] + self.RETRY[4:]
        self.assertEqual(rotate_mid_entered_loops(text), text)


if __name__ == '__main__':
    unittest.main()
