import json
from pathlib import Path
import tempfile
from types import SimpleNamespace
import unittest
from unittest.mock import patch

from tools.script_recovery import run_campaign as campaign
from tools.script_recovery import autopilot


class CampaignTests(unittest.TestCase):
    def test_next_stage_uses_new_checkpoint(self):
        self.check_run('success', 2, 0)

    def test_failed_stage_stops_even_if_it_left_a_save(self):
        self.check_run('failed', 1, 1)

    def test_missing_harvest_stops_chain(self):
        self.check_run('missing', 1, 1)

    def check_run(self, mode, calls, status):
        with tempfile.TemporaryDirectory() as directory:
            root = Path(directory)
            saves = root / 'saves'
            source = saves / 'original'
            source.mkdir(parents=True)
            (source / 'AutoSave').write_bytes(b'original')
            config = root / 'campaign.json'
            config.write_text(json.dumps({'save': 'original', 'stages': [
                {'quest': 'wasp_boss'}, {'quest': 'guardian_sister_info'}]}))
            commands = []

            def replay(cmd, **kwargs):
                commands.append(cmd)
                if mode != 'missing':
                    checkpoint = saves / cmd[cmd.index('--harvest') + 1]
                    checkpoint.mkdir()
                    (checkpoint / 'AutoSave').write_bytes(b'new')
                return SimpleNamespace(returncode=1 if mode == 'failed' else 0)

            with patch.object(campaign, 'ROOT', root), patch.object(campaign, 'SAVES', saves), \
                    patch.object(campaign.subprocess, 'run', side_effect=replay):
                self.assertEqual(campaign.run(config, bundle='test', tag='test'), status)
            self.assertEqual(len(commands), calls)
            self.assertEqual(commands[0][commands[0].index('--save') + 1], 'original')
            if calls > 1:
                self.assertEqual(commands[1][commands[1].index('--save') + 1], 'test_wasp_boss')
            self.assertEqual((source / 'AutoSave').read_bytes(), b'original')
            report = json.loads((root / 'work/runner/test_campaign.json').read_text())
            self.assertEqual(report['stages'][-1]['status'], 'done' if status == 0 else 'failed')


class HarvestTests(unittest.TestCase):
    def test_no_new_save_reports_failure_and_preserves_existing_checkpoint(self):
        self.check_harvest(new=False)

    def test_new_save_drops_stale_optional_companions(self):
        self.check_harvest(new=True)

    def check_harvest(self, new):
        with tempfile.TemporaryDirectory() as directory:
            saves = Path(directory)
            live = saves / 'live'
            checkpoint = saves / 'checkpoint'
            live.mkdir()
            checkpoint.mkdir()
            (live / 'AutoSave').write_bytes(b'new')
            (checkpoint / 'AutoSave').write_bytes(b'old')
            (checkpoint / 'AutoSave.qs').write_bytes(b'stale companion')
            baseline = 0 if new else (live / 'AutoSave').stat().st_mtime
            with patch.object(autopilot, 'SAVES', saves), patch.object(autopilot, 'FRONTEND_PROFILE_DIR', 'live'):
                self.assertEqual(autopilot.harvest_save('checkpoint', None, baseline, wait=0), new)
            self.assertEqual((checkpoint / 'AutoSave').read_bytes(), b'new' if new else b'old')
            self.assertEqual((checkpoint / 'AutoSave.qs').exists(), not new)


if __name__ == '__main__':
    unittest.main()
