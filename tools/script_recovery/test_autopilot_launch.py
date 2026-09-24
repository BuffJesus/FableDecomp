"""Fresh replays must not consume previous-session commands or touch a live game."""
import tempfile
import unittest
from pathlib import Path
from unittest.mock import patch

from tools.script_recovery import autopilot, ab_playtest


class LaunchIsolationTests(unittest.TestCase):
    def test_input_failure_stops_later_menu_clicks(self):
        with patch.object(autopilot, 'Channel') as channel, patch.object(autopilot, 'LogTail') as tail, \
             patch.object(autopilot, 'game_input', side_effect=['', RuntimeError('tutorial blocked')]) as drive:
            channel.return_value.send.return_value = []
            tail.return_value.new_lines.return_value = []
            result = autopilot.run_checklist('test', [{
                'id': 'guarded_menu', 'do': ['input: clear_all', 'input: click 380 133'],
                'expect': 'accepted', 'forbid': 'Autopilot.*error', 'timeout': 1,
            }])
            self.assertEqual(result[0]['status'], 'FAIL')
            self.assertEqual([call.args[0] for call in drive.call_args_list], ['focus', 'clear_all'])

    def test_clear_all_stops_at_no_box_and_limits_tutorial_retries(self):
        drive = autopilot.game_input
        with patch.object(autopilot, 'game_input', side_effect=['', '', '(no box)']) as clear:
            self.assertEqual(drive('clear_all'), 'cleared 2 tutorial page(s)')
            self.assertEqual(clear.call_count, 3)
            self.assertTrue(all(call.args[0] == 'clear' for call in clear.call_args_list))
        with patch.object(autopilot, 'game_input', return_value='') as clear:
            with self.assertRaisesRegex(RuntimeError, 'eight'):
                drive('clear_all')
            self.assertEqual(clear.call_count, 8)

    def test_input_wait_is_bounded_and_does_not_send_keys(self):
        with patch.object(autopilot.time, 'sleep') as sleep, patch('subprocess.run') as run:
            autopilot.game_input('wait 3')
            sleep.assert_called_once_with(3)
            run.assert_not_called()
            for spec in ('wait -1', 'wait 31', 'wait nan'):
                with self.assertRaises(ValueError):
                    autopilot.game_input(spec)

    def test_missing_backup_does_not_remove_live_save(self):
        with tempfile.TemporaryDirectory() as directory:
            root = Path(directory)
            live = root / 'target'
            live.mkdir()
            (live / 'AutoSave').write_bytes(b'protected')
            with patch.object(autopilot, 'SAVES', root), patch.object(autopilot, 'FRONTEND_PROFILE_DIR', 'target'):
                with self.assertRaises(FileNotFoundError):
                    autopilot.restore_save(root / 'missing')
            self.assertEqual((live / 'AutoSave').read_bytes(), b'protected')

    def test_save_staging_and_restore_preserve_optional_companion_absence(self):
        for source_has_companion in (False, True):
            with self.subTest(source_has_companion=source_has_companion), tempfile.TemporaryDirectory() as directory:
                root = Path(directory)
                (root / 'scratchpad').mkdir()
                saves = root / 'saves'
                source = saves / 'source'
                target = saves / 'target'
                source.mkdir(parents=True)
                target.mkdir()
                (source / 'AutoSave').write_bytes(b'new save')
                (target / 'AutoSave').write_bytes(b'protected save')
                if source_has_companion:
                    (source / 'AutoSave.qs').write_bytes(b'new snapshot')
                else:
                    (target / 'AutoSave.qs').write_bytes(b'protected snapshot')
                original = {p.name: p.read_bytes() for p in target.iterdir()}
                with patch.object(autopilot, 'ROOT', root), patch.object(autopilot, 'SAVES', saves), \
                     patch.object(autopilot, 'FRONTEND_PROFILE_DIR', 'target'):
                    backup = autopilot.stage_save('source')
                    self.assertEqual((target / 'AutoSave').read_bytes(), b'new save')
                    self.assertEqual((target / 'AutoSave.qs').exists(), source_has_companion)
                    # Simulate an extra snapshot written by the running game.
                    (target / 'AutoSave.qs.hs').write_bytes(b'new hero snapshot')
                    autopilot.restore_save(backup)
                self.assertEqual({p.name: p.read_bytes() for p in target.iterdir()}, original)

    def test_running_game_preserves_pending_commands_and_log(self):
        self.check_launch(running=True)

    def test_fresh_launch_clears_previous_session_commands_and_log(self):
        self.check_launch(running=False)

    def check_launch(self, running):
        with tempfile.TemporaryDirectory() as directory:
            root = Path(directory)
            commands, log = root / 'commands.txt', root / 'game.log'
            commands.write_text("WaspBoss: quest:ActivateQuest('Q_WaspBoss')\n")
            log.write_text('previous session\n')
            with patch.object(autopilot, 'commands_path', return_value=commands), \
                 patch.object(autopilot, 'log_path', return_value=log), \
                 patch.object(ab_playtest, 'fable_running', return_value=running), \
                 patch('subprocess.Popen') as launch:
                # Stop at the process boundary: this test must never open the game.
                launch.side_effect = RuntimeError('launch boundary')
                if running:
                    with self.assertRaisesRegex(SystemExit, 'already running'):
                        autopilot.launch_and_load('test')
                    launch.assert_not_called()
                    self.assertIn('ActivateQuest', commands.read_text())
                    self.assertEqual(log.read_text(), 'previous session\n')
                else:
                    with self.assertRaisesRegex(RuntimeError, 'launch boundary'):
                        autopilot.launch_and_load('test')
                    launch.assert_called_once()
                    self.assertFalse(commands.exists())
                    self.assertFalse(log.exists())
