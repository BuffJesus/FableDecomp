"""Restore both native edges into the thank-you sequence and borrowed hero calls."""
import hashlib
import json
from pathlib import Path


def recover(source,data):
    witness=json.loads(Path(__file__).with_name('native_barrel_return_encounter_witness.json').read_text())
    raw=data.bytes_at(witness['address'],witness['size'])
    if raw is None or hashlib.sha256(raw).hexdigest()!=witness['sha256']:
        raise ValueError('Barrel return encounter native instructions changed')
    for block in witness['blocks']:
        if source.count(block['oldLua'])!=1:raise ValueError('Barrel return encounter source correspondence changed')
        source=source.replace(block['oldLua'],block['newLua'],1)
    return source,dict(witness,status='recovered',semantics='Facing uses fresh borrowed hero with false flag. Visibility uses hero then bound actor; false visibility queries a fresh hero for distance under10. Both positive branches enter THANKS; neither exits Main.')
