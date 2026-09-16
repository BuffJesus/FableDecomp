"""Verify all seven ordered-positive health results and fix the inverse Lua branch."""
import hashlib
import json
from pathlib import Path


def verify(data):
    witness=json.loads(Path(__file__).with_name('native_barrel_man_health_branches_witness.json').read_text())
    if data.bytes_at(witness['thresholdAddress'],4)!=b'\0'*4:
        raise ValueError('Barrel health threshold changed')
    for region in witness['branches']:
        raw=data.bytes_at(region['address'],region['size'])
        if raw is None or hashlib.sha256(raw).hexdigest()!=region['sha256']:
            raise ValueError('Barrel health comparison instructions changed')
    return witness


def recover(source,data):
    witness=verify(data)
    old='            fVar20 = 0.0\n            if fVar19 <= fVar20 then'
    if source.count(old)!=1:raise ValueError('Barrel inverse health source correspondence changed')
    return source.replace(old,'            fVar20 = 0.0\n            if not (fVar19 > fVar20) then'),dict(
        witness,status='recovered',behavior='All seven native results require ordered health > 0; inverse branch includes NaN.')
