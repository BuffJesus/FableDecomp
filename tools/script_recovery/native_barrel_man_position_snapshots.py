"""Pin the two retained-marker position snapshots before controlled queries.

These are value copies, including the live fallback vector for an empty marker.
This evidence does not authorize replacing marker ownership with cached entities.
"""
import hashlib
import json
from pathlib import Path


def verify(data, witness=None):
    witness = witness or json.loads(Path(__file__).with_name(
        'native_barrel_man_position_snapshots_witness.json').read_text())
    for region in witness['snapshots']:
        raw = data.bytes_at(region['address'], region['size'])
        if raw is None or hashlib.sha256(raw).hexdigest() != region['sha256']:
            raise ValueError('Barrel marker position snapshot instructions changed')
    return dict(witness, status='verified-native-snapshots',
                remaining='Integrate retained marker ownership and consumer lowering; camera position is a separate borrowed query.')
