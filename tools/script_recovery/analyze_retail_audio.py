"""Correlate observed sample records with listener poses, without inferring audibility."""
import math


def vector(value):
    if not isinstance(value, (list, tuple)) or len(value) != 3:
        raise ValueError('expected a three-component audio vector')
    if any(isinstance(v, bool) or not isinstance(v, (float, int)) or not math.isfinite(v) for v in value):
        raise ValueError('invalid audio vector component')
    return tuple(value)


def identity(value):
    if isinstance(value, bool) or not isinstance(value, int) or not 0 <= value < 2**32:
        raise ValueError('invalid audio identity field')
    return value


class AudioObservations:
    def __init__(self):
        self.scans = []
        self.groups = {}
        self.previous_pose = None
        self.previous_scan_time = None

    def add(self, row, time, line):
        if 'audio_samples' not in row:
            return  # Observer scans samples at 2 Hz; other rows are not absences.
        snapshot = row['audio_samples']
        scan = {'time': time, 'line': line, 'complete': snapshot.get('complete') is True,
                'samples': [], 'issues': []}
        self.scans.append(scan)
        if not scan['complete']:
            scan['issues'].append('partial scan: ' + str(snapshot.get('partial_reason', 'unspecified')))
        if self.previous_scan_time is not None and time - self.previous_scan_time > 1000:
            scan['issues'].append('more than 1000 ms between sample scans')
        self.previous_scan_time = time
        pose = row.get('audio_listener', {})
        current_pose = None
        if pose.get('complete') is True:
            try:
                position, forward = vector(pose['position']), vector(pose['forward'])
                length = math.sqrt(sum(v*v for v in forward))
                if not length:
                    raise ValueError('zero listener forward vector')
                forward = tuple(v/length for v in forward)
                scan['listener'] = {'position': position, 'forward': forward, 'up': vector(pose['up'])}
                current_pose = (position, forward)
                if self.previous_pose:
                    previous_position, previous_forward = self.previous_pose
                    scan['listener_translation_since_previous_scan'] = math.dist(position, previous_position)
                    dot = sum(a*b for a,b in zip(forward, previous_forward))
                    scan['listener_turn_degrees_since_previous_scan'] = math.degrees(math.acos(max(-1, min(1, dot))))
            except (KeyError, TypeError, ValueError) as exc:
                scan['issues'].append('listener: ' + str(exc))
                current_pose = None
        else:
            scan['issues'].append('listener pose unavailable')
        self.previous_pose = current_pose  # Do not silently bridge an unreadable pose.
        seen = set()
        for emitter in snapshot.get('emitters', []):
            for sample in emitter.get('samples', []):
                try:
                    key = tuple(identity(v) for v in (emitter['address'], sample['address'],
                                                     sample['handle'], sample['bank'], sample['sample_id']))
                    if key in seen:
                        raise ValueError('duplicate sample identity in scan')
                    seen.add(key)
                    item = dict(zip(('emitter', 'record', 'handle', 'bank', 'sample_id'), key))
                    for name in ('spatial', 'sound_in_2d'):
                        if sample[name] not in (0, 1):
                            raise ValueError('invalid sample flag: ' + name)
                        item[name] = int(sample[name])
                    item['emitter_position'] = vector(emitter['position'])
                    if current_pose:
                        position, forward = current_pose
                        offset = tuple(a-b for a,b in zip(item['emitter_position'], position))
                        item['emitter_distance_to_listener'] = math.sqrt(sum(v*v for v in offset))
                        item['emitter_forward_projection'] = sum(a*b for a,b in zip(offset, forward))
                        # For relative sounds this emitter distance is NOT the
                        # submitted source distance. Preserve that distinction.
                        if item['sound_in_2d']:
                            item['relative_source_game_axes'] = vector(row['audio_script_2d_position'])
                            item['relative_source_distance'] = math.sqrt(sum(v*v for v in item['relative_source_game_axes']))
                    scan['samples'].append(item)
                    group = self.groups.setdefault(key, {'identity': dict(zip(
                        ('emitter', 'record', 'handle', 'bank', 'sample_id'), key)),
                        'first_observed': time, 'last_observed': time, 'observations': [],
                        'absent_in_complete_scans': []})
                    group['last_observed'] = time
                    group['observations'].append({'line': line, 'time': time,
                                                  'spatial': item['spatial'], 'sound_in_2d': item['sound_in_2d']})
                except (KeyError, TypeError, ValueError) as exc:
                    scan['issues'].append('sample: ' + str(exc))
                    scan['complete'] = False
        if scan['complete']:
            for key, group in self.groups.items():
                if key not in seen:
                    group['absent_in_complete_scans'].append(time)
        scan['overlapping_registered_samples'] = len(scan['samples']) > 1

    def report(self):
        return {'scans': self.scans, 'sample_groups': list(self.groups.values()),
                'limits': ['Registered samples are not necessarily audible voices.',
                           'Group identity may be reused; first/last observation is not a lifetime measurement.',
                           'Overlapping records do not identify which line was spoken.',
                           'Emitter distance is not relative-source distance when sound_in_2d is set.',
                           'Listener and sample reads are non-atomic; snapshots can miss transitions.']}
