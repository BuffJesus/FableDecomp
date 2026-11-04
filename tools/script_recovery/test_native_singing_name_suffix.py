import itertools
import json
import unittest
from pathlib import Path

from tools.script_recovery.lift_native_lua import RData
from tools.script_recovery.test_native_singing_playlist_compare import run
try:
    from tools.script_recovery.native_reviewed_singing_name_suffix import ARG, APPEND, ARG_DIAG, APPEND_DIAG, restore
except ImportError:
    from work.codex_lua_singing_name_suffix_20261004.native_reviewed_singing_name_suffix import ARG, APPEND, ARG_DIAG, APPEND_DIAG, restore

ROOT = Path(__file__).resolve().parents[2]
W = ROOT / 'work/codex_lua_singing_name_suffix_20261004'
AFTER = ROOT / 'refs/script_recovery/lifted/SingingStones/readable/FSE/V_SingingStones/V_SingingStones.lua'
if not (ROOT / 'tools/script_recovery/native_reviewed_singing_name_suffix.py').exists():
    AFTER = W / 'readable_comparison/after/singing_stones/FSE/V_SingingStones/V_SingingStones.lua'


class SingingNameSuffixReview(unittest.TestCase):
    @classmethod
    def setUpClass(cls):
        unit_path = ROOT / 'refs/script_recovery/singing_stones/units/V_SingingStones.json'
        if not (ROOT / 'tools/script_recovery/native_reviewed_singing_name_suffix.py').exists():
            unit_path = W / 'candidate_unit.json'
        unit = json.loads(unit_path.read_text())
        cls.row = unit['reviewedSingingNameSuffix']['0x00ed2bb0']
        cls.fields = unit['quest']['fields']
        typed = json.loads((ROOT / 'refs/script_recovery/singing_stones/translation_unit_typed.json').read_text())
        cls.fn = next(f for f in typed['functions'] if f['address'].lower() == '0x00ed2bb0')
        cls.rdata = RData()
        cls.source = (W / 'baseline_draft.lua').read_text()
        cls.todo = [ARG_DIAG, APPEND_DIAG]

    def test_native_and_lifted_gates(self):
        source, todo, calls = restore(self.source, self.todo[:], [], self.fn,
                                      self.row, self.rdata, self.fields)
        self.assertEqual(todo, [])
        self.assertNotIn(ARG, source)
        self.assertNotIn(APPEND, source)
        self.assertIn('tuneName = tuneName .. p0', source)
        self.assertIn('GetStateInt', calls)
        with self.assertRaisesRegex(ValueError, 'native body changed'):
            restore(self.source, self.todo[:], [], self.fn,
                    {**self.row, 'nativeSha256': '0'*64}, self.rdata, self.fields)
        with self.assertRaisesRegex(ValueError, 'personal playlist layout changed'):
            restore(self.source, self.todo[:], [], self.fn,
                    self.row, self.rdata, {**self.fields, '0x6c': ['Wrong', 'Int']})

    def test_name_suffix_for_personal_tunes(self):
        normal, rude = (0, 1, 2, 3), (3, 2, 1, 0)
        choices = [x for x in itertools.permutations(range(4)) if x not in (normal, rude)]
        choices += [(4, 1, 2, 3), (0, -1, 2, 3)]
        for chosen in choices:
            with self.subTest(chosen=chosen):
                expected = 'TEXT_QST_060_NAME_' + ''.join('ABCD'[x] for x in chosen if 0 <= x < 4)
                before = run(W / 'baseline_readable.lua', normal, rude, chosen)
                after = run(AFTER, normal, rude, chosen)
                self.assertIn(('line', 'TEXT_QST_060_NAME_'), before)
                self.assertIn(('line', expected), after)
                self.assertIn(('clear', 'EffectList'), after)


if __name__ == '__main__':
    unittest.main()
