import unittest
from unittest.mock import Mock, patch

from tools.script_recovery import ingame_runner as runner


class TravelSettleTests(unittest.TestCase):
    def test_arrival_tutorial_resets_the_quiet_window(self):
        ready = Mock(return_value=True)
        with patch.object(runner, 'game_input', side_effect=['(no box)', '', '(no box)', '(no box)']) as ui, \
                patch.object(runner.time, 'sleep'):
            self.assertTrue(runner.settle_world(ready))
        self.assertEqual(ui.call_count, 4)

    def test_scene_or_missing_ack_prevents_departure(self):
        ready = Mock(side_effect=[False, True, True])
        with patch.object(runner, 'game_input', return_value='(no box)'), \
                patch.object(runner.time, 'sleep'):
            self.assertTrue(runner.settle_world(ready))
        self.assertEqual(ready.call_count, 3)

    def test_unresolved_scene_times_out(self):
        with patch.object(runner, 'game_input', return_value='(no box)'), \
                patch.object(runner.time, 'sleep'), \
                patch.object(runner.time, 'monotonic', side_effect=[0, 1, 3]):
            self.assertFalse(runner.settle_world(lambda: False, timeout=2))
