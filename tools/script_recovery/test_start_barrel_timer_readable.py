import tempfile
import unittest
from pathlib import Path

from tools.script_recovery.build_readable_new_oakvale import RAW, build
from tools.script_recovery.readable_start_barrel_timer import lower
from tools.script_recovery.start_barrel_timer_native import execute
from tools.script_recovery.test_start_barrel_timer import run


class StartBarrelTimerReadableTests(unittest.TestCase):
    def test_emitted_helper_preserves_native_wait_and_cleanup(self):
        with tempfile.TemporaryDirectory() as directory:
            report = build(Path(directory))
            source = (Path(directory)/'FSE/NewOakValeIntro/NewOakValeIntro.lua').read_text()
            self.assertTrue(report['syntax']['ok'])
            self.assertIsNotNone(report['startBarrelTimer']['evidence'])
            self.assertEqual('Quests = {}\n', (Path(directory)/'FSE/quests.lua').read_text())
            self.assertFalse((Path(directory)/'FSE/PartyMode').exists())
            self.assertNotIn('PARTY_MODE', source)
            self.assertIn('quest:RegisterTimer()', source)
            self.assertIn('quest:SetTimer(quest:GetStateInt("TalkIntermittentTimer"), 0)', source)
            start = source.index('\nfunction StartBarrelTimer(')
            end = source.index('\nfunction WatchBarrels(', start)
            for token in ('goto ', 'LAB_', 'TODO(native)', 'pCVar', 'RegisterBoundConsciousCondition'):
                self.assertNotIn(token, source[start:end])
            for cancel in (1, 2, 4, 6, 10, 99):
                for spoken in (0, 1, 3):
                    for populated in (False, True):
                        options = dict(cancel=cancel, spoken=spoken, populated=populated)
                        self.assertEqual(run(source=source, **options), execute(**options), options)

    def test_changed_draft_rejected(self):
        source = (RAW/'FSE/NewOakValeIntro/NewOakValeIntro.lua').read_text()
        start = source.index('\nfunction StartBarrelTimer(')
        source = source[:start] + source[start:].replace('WatchTimer', 'ChangedTimer', 1)
        with self.assertRaisesRegex(ValueError, 'StartBarrelTimer draft changed'):
            lower(source)


if __name__ == '__main__':
    unittest.main()
