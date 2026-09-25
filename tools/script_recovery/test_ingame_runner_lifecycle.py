"""Replay staging must not affect an existing session or harvest its old save."""
import json
import os
from pathlib import Path
import tempfile
import unittest
from unittest.mock import patch

from tools.script_recovery import ingame_runner as runner


class RunnerLifecycleTests(unittest.TestCase):
    def test_live_game_rejected_before_staging_or_cleanup(self):
        args = ['runner', 'unused.json', '--launch', '--close', '--save', 'source']
        with patch('sys.argv', args), patch.object(runner, 'fable_running', return_value=True), \
                patch.object(runner, 'stage_save') as stage, \
                patch.object(runner, 'restore_save') as restore, patch('subprocess.run') as run:
            with self.assertRaises(SystemExit) as error:
                runner.main()
            self.assertEqual(error.exception.code, 2)
            stage.assert_not_called()
            restore.assert_not_called()
            run.assert_not_called()

    def test_staging_requires_launch(self):
        with patch('sys.argv', ['runner', 'unused.json', '--save', 'source']), \
                patch.object(runner, 'stage_save') as stage:
            with self.assertRaises(SystemExit) as error:
                runner.main()
            self.assertEqual(error.exception.code, 2)
            stage.assert_not_called()

    def test_harvest_uses_staged_timestamp_and_closes_before_restore(self):
        self.check_staged_run(False)

    def test_failed_load_closes_before_restore_without_harvesting(self):
        self.check_staged_run(True)

    def test_missing_harvest_fails_and_still_restores(self):
        self.check_staged_run(False, fail_harvest=True)

    def check_staged_run(self, fail_launch, fail_harvest=False):
        with tempfile.TemporaryDirectory() as directory:
            root = Path(directory)
            live = root / 'target' / 'AutoSave'
            live.parent.mkdir()
            live.write_bytes(b'original')
            os.utime(live, (100, 100))
            config = root / 'quest.json'
            config.write_text(json.dumps({}))
            events = []

            def stage(_):
                live.write_bytes(b'staged')
                os.utime(live, (500, 500))
                return root / 'backup'

            args = ['runner', str(config), '--launch', '--save', 'source', '--harvest', 'result']
            with patch('sys.argv', args), patch.object(runner, 'SAVES', root), \
                    patch.object(runner, 'FRONTEND_PROFILE_DIR', 'target'), \
                    patch.object(runner, 'fable_running', return_value=False), \
                    patch.object(runner, 'stage_save', side_effect=stage), \
                    patch.object(runner, 'launch_and_load', side_effect=RuntimeError('load failed') if fail_launch else None), \
                    patch.object(runner, 'play', return_value='done'), \
                    patch.object(runner, 'harvest_save', return_value=not fail_harvest) as harvest, \
                    patch.object(runner, 'restore_save', side_effect=lambda _: events.append('restore')), \
                    patch('subprocess.run', side_effect=lambda *a, **k: events.append('close')), \
                    patch.object(runner.time, 'sleep'):
                if fail_launch:
                    with self.assertRaisesRegex(RuntimeError, 'load failed'):
                        runner.main()
                    harvest.assert_not_called()
                elif fail_harvest:
                    with self.assertRaisesRegex(RuntimeError, 'no new autosave'):
                        runner.main()
                else:
                    with self.assertRaises(SystemExit) as error:
                        runner.main()
                    self.assertEqual(error.exception.code, 0)
                    harvest.assert_called_once_with('result', 'source', 500)
            self.assertEqual(events, ['close', 'restore'])


if __name__ == '__main__':
    unittest.main()
