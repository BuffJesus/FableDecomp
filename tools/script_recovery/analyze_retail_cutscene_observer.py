"""Report read-only observer data; eligibility and fade state are not playback proof."""
import argparse
import bisect
import hashlib
import json
import math
from pathlib import Path
import struct

try:
    from .observe_retail_cutscene import eligibility
    from .analyze_retail_audio import AudioObservations
except ImportError:
    from observe_retail_cutscene import eligibility
    from analyze_retail_audio import AudioObservations


def tick(value):
    if isinstance(value, bool) or not isinstance(value, int) or not 0 <= value < 2**32:
        raise ValueError('invalid 32-bit tick')
    return value


def fade_state(raw):
    data = bytes.fromhex(raw)
    if len(data) < 0x30:
        raise ValueError('short fade snapshot')
    result = {name: data[offset] for name, offset in (
        ('active', 0), ('fading_in', 0xC), ('fading_out', 0xD), ('hold_until_in', 0x1C))}
    for name, offset in (('out_duration', 4), ('in_duration', 8),
                         ('out_elapsed', 0x10), ('in_remaining', 0x14)):
        value = struct.unpack_from('<f', data, offset)[0]
        if not math.isfinite(value):
            raise ValueError('non-finite fade timer')
        result[name] = value
    result['colour'] = hex(struct.unpack_from('<I', data, 0x18)[0])
    result['freeze_count'] = struct.unpack_from('<I', data, 0x2C)[0]
    return result


def analyze(text, max_context_ms=250):
    report = {'scope': __doc__, 'errors': [], 'warnings': [], 'escape': [],
              'samples': 0, 'valid_gate_samples': 0, 'read_errors': [],
              'limits': ['Samples are non-atomic and may miss short transitions.',
                         'Foreground PID is observed at sampling time, not at key-event time.',
                         'Eligibility does not establish input consumption; fade state does not establish pixels.']}
    samples, events, previous, epoch, down, provenance = [], [], None, 0, False, None
    audio = AudioObservations()
    for line_number, line in enumerate(text.splitlines(), 1):
        if not line.strip():
            continue
        try:
            row = json.loads(line)
            if not isinstance(row, dict):
                raise ValueError('record is not an object')
            if row.get('type') == 'provenance':
                if provenance is not None:
                    raise ValueError('duplicate provenance')
                if not isinstance(row.get('pid'), int):
                    raise ValueError('provenance PID missing')
                provenance = row
                report['provenance'] = row
                continue
            now = tick(row['tick'])
            if previous is None:
                epoch = now
            else:
                step = (now - previous) & 0xFFFFFFFF
                if step >= 0x80000000:
                    raise ValueError('sample ticks moved backwards or gap exceeds 2^31 ms')
                epoch += step
            previous = now
            sample = {'time': epoch, 'line': line_number, 'foreground_pid': row.get('foreground_pid')}
            report['samples'] += 1
            if row.get('read_error'):
                report['read_errors'].append({'line': line_number, 'error': row['read_error']})
            if 'gui_bytes' in row and 'busy' in row:
                try:
                    gui = bytes.fromhex(row['gui_bytes'])
                    if len(gui) < 0x111:
                        raise ValueError('short GUI snapshot')
                    if not isinstance(row['busy'], int):
                        raise ValueError('invalid busy value')
                    sample['eligibility'] = eligibility(gui, row['busy'])
                    report['valid_gate_samples'] += 1
                except (TypeError, ValueError) as exc:
                    report['errors'].append({'line': line_number, 'error': str(exc)})
            if 'fade_bytes' in row:
                try:
                    sample['fade'] = fade_state(row['fade_bytes'])
                except (TypeError, ValueError) as exc:
                    report['errors'].append({'line': line_number, 'error': str(exc)})
            samples.append(sample)
            try:
                audio.add(row, epoch, line_number)
            except (KeyError, TypeError, ValueError, AttributeError) as exc:
                report['errors'].append({'line': line_number, 'error': 'audio: ' + str(exc)})
            for event in row.get('escape_events', []):
                if not isinstance(event.get('down'), bool):
                    raise ValueError('invalid Escape transition')
                event_tick = tick(event['message_tick'])
                offset = ((event_tick - now + 0x80000000) & 0xFFFFFFFF) - 0x80000000
                kind = 'repeat' if event['down'] and down else ('down' if event['down'] else 'up')
                down = event['down']
                events.append({'time': epoch + offset, 'tick': event_tick, 'kind': kind,
                               'observed_line': line_number, 'delivery_delay_ms': -offset})
        except (KeyError, TypeError, ValueError, AttributeError) as exc:
            report['errors'].append({'line': line_number, 'error': str(exc)})
    if provenance is None:
        report['errors'].append({'error': 'missing provenance'})
    if not samples:
        report['errors'].append({'error': 'no samples'})
    times = [s['time'] for s in samples]
    for event in events:
        at = bisect.bisect_left(times, event['time'])
        for name, index in (('before', at - 1), ('at_or_after', at)):
            if 0 <= index < len(samples):
                sample = samples[index]
                distance = sample['time'] - event['time']
                if abs(distance) <= max_context_ms:
                    event[name] = {**sample, 'offset_ms': distance,
                                   'game_foreground': sample['foreground_pid'] == (provenance or {}).get('pid')}
        if 'before' not in event and 'at_or_after' not in event:
            event['context'] = 'no nearby sample'
        report['escape'].append(event)
    if not events:
        report['warnings'].append('No Escape transitions recorded; this is not a successful Escape test.')
    if report['valid_gate_samples'] != report['samples']:
        report['warnings'].append('Some samples lack valid eligibility data; missing data is not an eligible result.')
    report['valid_capture'] = not report['errors']
    report['audio'] = audio.report()
    return report


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('capture', type=Path)
    parser.add_argument('--output', type=Path, required=True)
    args = parser.parse_args()
    raw = args.capture.read_bytes()
    result = analyze(raw.decode('utf-8-sig'))
    result['capture_sha256'] = hashlib.sha256(raw).hexdigest()
    # Avoid overwriting reviewed reports.
    with args.output.open('x', encoding='utf-8') as out:
        json.dump(result, out, indent=2, allow_nan=False)
        out.write('\n')
    print(json.dumps({key: result[key] for key in ('valid_capture', 'samples', 'valid_gate_samples', 'errors', 'warnings')}))
    return 0 if result['valid_capture'] else 1


if __name__ == '__main__':
    raise SystemExit(main())
