import json
import unittest
from pathlib import Path

from tools.script_recovery.lift_native_lua import RData
from tools.script_recovery.test_native_singing_stone_play import play
from tools.script_recovery.native_reviewed_singing_temp_cleanup import (
    BLOCK, DIAGNOSTICS, restore,
)

ROOT = Path(__file__).resolve().parents[2]
W = ROOT / 'work/codex_lua_singing_temp_cleanup_20261004'
BEFORE = W / 'readable_comparison/before/singing_stones/FSE/V_SingingStones/Entities/SingingStone.lua'
AFTER = W / 'readable_comparison/after/singing_stones/FSE/V_SingingStones/Entities/SingingStone.lua'


class TempCleanupReview(unittest.TestCase):
    @classmethod
    def setUpClass(cls):
        unit = json.loads((W / 'candidate_unit.json').read_text())
        cls.row = unit['reviewedSingingTempCleanup']['0x00ed3370']
        typed = json.loads((ROOT / 'refs/script_recovery/singing_stones/translation_unit_typed.json').read_text())
        cls.fn = next(f for f in typed['functions'] if f['address'].lower() == '0x00ed3370')
        cls.rdata = RData()

    def test_evidence_and_shape_gates(self):
        source = (W / 'baseline_draft.lua').read_text()
        report = json.loads((W / 'baseline_report.json').read_text())
        todo = next(f['todo'] for f in report['units'][0]['functions'] if f['address'].lower() == '0x00ed3370')
        result, remaining, calls = restore(source, todo[:], [], self.fn, self.row, self.rdata)
        self.assertNotIn(BLOCK, result)
        self.assertIn('::LAB_00ed3906::', result)
        self.assertEqual(len(remaining), len(todo)-len(DIAGNOSTICS))
        self.assertEqual(calls, [])
        with self.assertRaisesRegex(ValueError, 'native body changed'):
            restore(source, todo[:], [], self.fn, {**self.row, 'nativeSha256': '0'*64}, self.rdata)
        with self.assertRaisesRegex(ValueError, 'lifted release shape changed'):
            restore(source.replace('r1:GetName()', 'r1:OtherCall()'), todo[:], [], self.fn, self.row, self.rdata)

    def test_full_main_traces_unchanged(self):
        for stone, initial, hero, special, heal, prior in (
                (2, 0, True, False, False, None),
                (2, 0, False, True, False, None),
                (2, 0, False, True, True, None),
                (2, 0, False, False, False, None),
                (2, 1, True, False, False, 2),
                (2, 1, True, False, False, 1)):
            with self.subTest(hero=hero, special=special, heal=heal, prior=prior):
                args = (stone, initial, hero, special, heal, prior)
                self.assertEqual(play(BEFORE, *args), play(AFTER, *args))


if __name__ == '__main__':
    unittest.main()
