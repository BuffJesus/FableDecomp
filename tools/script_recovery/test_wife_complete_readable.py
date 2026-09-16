"""Exercise the shipped readable Wife after all shared builder transformations."""
import tempfile
import unittest
from pathlib import Path

from tools.script_recovery.build_readable_new_oakvale import build
from tools.script_recovery.wife_complete_candidate import generate
from tools.script_recovery.test_wife_complete_candidate import run


class WifeCompleteReadableTests(unittest.TestCase):
    def test_emitted_candidate_preserves_state_calls_and_cleanup(self):
        candidate, _ = generate()
        with tempfile.TemporaryDirectory() as directory:
            report = build(Path(directory))
            source = (Path(directory) / 'FSE/NewOakValeIntro/Entities/NOVI_AffairWife.lua').read_text()
            self.assertTrue(report['syntax']['ok'])
            self.assertEqual(source.count('RegisterBoundConsciousCondition()'), 1)
            for token in ('TODO(native)', 'goto '):
                self.assertNotIn(token, source)
            self.assertNotRegex(source, r'::LAB_[0-9a-fA-F]+::')
            self.assertIn('resources:InitializeWifeActor(me)', source)
            self.assertIn('resources:AddWifeWhereHusbandConversation(me)', source)
            row = next(r for r in report['functions']
                       if r['owner'] == 'NOVI_AffairWife' and r['function'] == 'Main')
            self.assertEqual(row['implementationFunction'], '__resource_main')
            for scenario in ({}, {'hit': True}, {'talk': True},
                             {'going': True, 'busy': True}):
                for stop in (1, 2, 4, 8, 16, 32):
                    options = dict(scenario, stop_check=stop, stop_frame=8, trace_frames=True)
                    self.assertEqual(run(candidate, **options), run(source, **options), options)
            for location in ('health', 'speak'):
                options = dict(hit=True, error_at=location, stop_frame=8)
                expected, actual = run(candidate, **options), run(source, **options)
                self.assertEqual(expected[0], actual[0])
                self.assertIsNotNone(actual[1])


if __name__ == '__main__':
    unittest.main()
