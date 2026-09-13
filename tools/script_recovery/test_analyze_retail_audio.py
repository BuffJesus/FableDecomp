import copy
import unittest
from tools.script_recovery.analyze_retail_audio import AudioObservations


def row():
    return {'audio_listener': {'complete': True, 'position': [0, 0, 0],
                              'forward': [0, 1, 0], 'up': [0, 0, 1]},
            'audio_script_2d_position': [0, 0.5, 0],
            'audio_samples': {'complete': True, 'emitters': [
                {'address': 100, 'position': [0, 10, 0], 'samples': [
                    {'address': 200, 'handle': 3, 'bank': 400, 'sample_id': 500,
                     'spatial': 1, 'sound_in_2d': 1}]}]}}


class AudioReportTests(unittest.TestCase):
    def test_turn_and_relative_distance_are_distinguished(self):
        audio = AudioObservations(); a = row(); audio.add(a, 100, 1)
        b = copy.deepcopy(a); b['audio_listener']['forward'] = [1, 0, 0]
        audio.add(b, 600, 2)
        scan = audio.report()['scans'][1]
        self.assertEqual(scan['listener_turn_degrees_since_previous_scan'], 90)
        self.assertEqual(scan['samples'][0]['emitter_distance_to_listener'], 10)
        self.assertEqual(scan['samples'][0]['emitter_forward_projection'], 0)
        self.assertEqual(scan['samples'][0]['relative_source_distance'], 0.5)

    def test_partial_scan_does_not_mean_absence(self):
        audio = AudioObservations(); audio.add(row(), 100, 1)
        audio.add({}, 200, 2)
        a = row(); a['audio_samples'] = {'complete': False, 'emitters': [], 'partial_reason': 'time_budget'}
        audio.add(a, 600, 3)
        self.assertEqual(audio.report()['sample_groups'][0]['absent_in_complete_scans'], [])
        a['audio_samples']['complete'] = True; audio.add(a, 1100, 4)
        self.assertEqual(audio.report()['sample_groups'][0]['absent_in_complete_scans'], [1100])

    def test_handle_reuse_and_overlap_remain_distinct(self):
        audio = AudioObservations(); a = row()
        other = copy.deepcopy(a['audio_samples']['emitters'][0]['samples'][0]); other['sample_id'] = 501
        a['audio_samples']['emitters'][0]['samples'].append(other)
        audio.add(a, 100, 1)
        self.assertEqual(len(audio.report()['sample_groups']), 2)
        self.assertTrue(audio.report()['scans'][0]['overlapping_registered_samples'])

    def test_missing_pose_does_not_bridge_rotation(self):
        audio = AudioObservations(); audio.add(row(), 100, 1)
        a = row(); a['audio_listener'] = {'complete': False}; audio.add(a, 600, 2)
        audio.add(row(), 2100, 3)
        scan = audio.report()['scans'][2]
        self.assertNotIn('listener_turn_degrees_since_previous_scan', scan)
        self.assertIn('more than 1000 ms', scan['issues'][0])

    def test_malformed_sample_is_partial(self):
        audio = AudioObservations(); a = row()
        a['audio_samples']['emitters'][0]['samples'][0]['sample_id'] = -1
        audio.add(a, 100, 1)
        self.assertFalse(audio.report()['scans'][0]['complete'])
        self.assertEqual(audio.report()['sample_groups'], [])


if __name__ == '__main__':
    unittest.main()
