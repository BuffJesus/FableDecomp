"""Restore the native termination exit on both walk-off camera branches."""
import hashlib
import json
from pathlib import Path


def recover(source, data):
    witness = json.loads(Path(__file__).with_name(
        'native_barrel_camera_cancellation_witness.json').read_text())
    raw = data.bytes_at(witness['address'], witness['size'])
    if raw is None or hashlib.sha256(raw).hexdigest() != witness['sha256']:
        raise ValueError('Barrel camera cancellation native instructions changed')
    if source.count(witness['old']) != 1:
        raise ValueError('Barrel camera cancellation source correspondence changed')
    return source.replace(witness['old'], witness['new']), dict(
        status='recovered', nativeAddress=witness['address'], nativeSha256=witness['sha256'],
        behavior='Both camera outcomes check termination once before selecting a destination; termination reaches the existing cleanup join.',
        remaining='Retained marker destruction still requires the full resource-aware candidate.')
