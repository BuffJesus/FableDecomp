import json
import unittest
from tools.script_recovery.analyze_retail_cutscene_observer import analyze


def sample(tick, events=None, enabled=True):
    gui = bytearray(0x298)
    gui[0xF6] = gui[0x100] = 1
    gui[0xF8] = int(enabled)
    return {'tick': tick, 'foreground_pid': 42, 'gui_bytes': gui.hex(), 'busy': 0,
            'fade_bytes': bytes(0x40).hex(), 'escape_events': events or []}


def capture(*rows):
    return '\n'.join(json.dumps(row) for row in ({'type': 'provenance', 'pid': 42}, *rows))


class ObserverReportTests(unittest.TestCase):
    def test_input_gate_transition(self):
        report = analyze(capture(sample(100, enabled=False), sample(150, [{'message_tick': 125, 'down': True}])))
        event = report['escape'][0]
        self.assertTrue(report['valid_capture'])
        self.assertEqual(event['before']['eligibility']['blockers'], ['scene_not_skippable'])
        self.assertTrue(event['at_or_after']['eligibility']['event_eligible'])
        self.assertEqual(event['delivery_delay_ms'], 25)
        self.assertTrue(event['at_or_after']['game_foreground'])

    def test_tick_wrap_and_repeats(self):
        report = analyze(capture(sample(0xFFFFFFF0), sample(20, [
            {'message_tick': 0xFFFFFFF8, 'down': True}, {'message_tick': 4, 'down': True},
            {'message_tick': 8, 'down': False}])))
        self.assertTrue(report['valid_capture'])
        self.assertEqual([e['kind'] for e in report['escape']], ['down', 'repeat', 'up'])
        self.assertEqual(report['escape'][0]['delivery_delay_ms'], 28)

    def test_partial_line_and_bad_gui_not_success(self):
        row = sample(10); row['gui_bytes'] = '00'
        report = analyze(capture(row) + '\n{"tick":')
        self.assertFalse(report['valid_capture'])
        self.assertEqual(len(report['errors']), 2)
        self.assertEqual(report['valid_gate_samples'], 0)

    def test_read_gap_is_unknown(self):
        report = analyze(capture({'tick': 1000, 'read_error': 'unreadable',
                                 'escape_events': [{'message_tick': 100, 'down': True}]}))
        self.assertEqual(report['escape'][0]['context'], 'no nearby sample')
        self.assertEqual(len(report['read_errors']), 1)
        self.assertEqual(report['valid_gate_samples'], 0)

    def test_backwards_time_rejected(self):
        self.assertFalse(analyze(capture(sample(100), sample(90)))['valid_capture'])

    def test_no_input_is_not_successful_test(self):
        report = analyze(capture(sample(100)))
        self.assertTrue(report['valid_capture'])
        self.assertEqual(report['escape'], [])
        self.assertIn('not a successful Escape test', report['warnings'][0])


if __name__ == '__main__':
    unittest.main()
