"""A stack slot reused as a scalar after an object life keeps its scalar stores (2026-09-26).

Q_BanditCampBossBattle CampHostageGuard::Main 0x00D09010 constructs the "CampHostage" lookup string in
`CStack_9c`, destroys it, then reuses the same byte as the guard's patrol leg (1 = GuardFirstMarker,
2 = GuardSecondMarker, flipped every 10 s). The export keeps the slot typed as a string:

    CCharString::CCharString(&xStack_9c,"CampHostage",-1);
    ... GetNearestWithScriptName(..., &xStack_9c);
    xStack_9c = (CCharString)0x1;
    if (xStack_9c == (CCharString)0x1) {          -- walk to GuardFirstMarker, else GuardSecondMarker
    xStack_9c = (CCharString)0x2;

Left as an object name the lifter dropped both leg stores and folded the test to `0x1 == 0x1`, so the
guard never walked away from the hostage door (the retail opening for the hero).
"""
import unittest
from pathlib import Path

import sys

ROOT = Path(__file__).resolve().parents[2]
sys.path.insert(0, str(ROOT))

from tools.script_recovery.native_evidence_lowering import split_scalar_slot_tail  # noqa: E402


class ScalarSlotTailTests(unittest.TestCase):
    GUARD = ("  CCharString xStack_9c;\n"
             "  CCharString::CCharString(&xStack_9c,\"CampHostage\",-1);\n"
             "  GSI_GetNearest(this,&xStack_c0,p0,&xStack_9c);\n"
             "  xStack_9c = (CCharString)0x1;\n"
             "  while (true) {\n"
             "    if (xStack_9c == (CCharString)0x1) {\n"
             "      xStack_9c = (CCharString)0x2;\n"
             "    }\n"
             "    else {\n"
             "      xStack_9c = (CCharString)0x1;\n"
             "    }\n"
             "  }\n")

    def test_scalar_tail_is_renamed(self):
        out = split_scalar_slot_tail(self.GUARD)
        self.assertIn('x_stk_9c = 1;', out)
        self.assertIn('if (x_stk_9c == 1) {', out)
        self.assertIn('x_stk_9c = 2;', out)
        self.assertNotIn('(CCharString)0x', out)
        # the object life keeps its spelling
        self.assertIn('CCharString::CCharString(&xStack_9c,"CampHostage",-1);', out)
        self.assertIn('&xStack_9c);', out)

    def test_mixed_tail_is_left_alone(self):
        # a later non-scalar use (the slot handed to a call) means the tail is not a pure scalar life
        text = self.GUARD.replace("    }\n  }\n", "    }\n    Use(xStack_9c);\n  }\n")
        self.assertEqual(split_scalar_slot_tail(text), text)

    def test_address_after_store_is_left_alone(self):
        # a store BEFORE the last address use belongs to the object life
        text = ("  xStack_9c = (CCharString)0x0;\n"
                "  CCharString::CCharString(&xStack_9c,\"A\",-1);\n")
        self.assertEqual(split_scalar_slot_tail(text), text)


if __name__ == '__main__':
    unittest.main()
