"""Recover the two incoming actor selections at AffairMan's shared facing call."""
import hashlib
import json
from pathlib import Path


def recover_affair_facing(function, source, rdata):
    witness = json.loads(Path(__file__).with_name('native_affair_facing_witness.json').read_text())
    if str(function.get('address', '')).lower() != witness['address'].lower():
        return source, []
    def reject(reason):
        return source, [{'status': 'rejected', 'reason': reason}]
    for value, key in ((function.get('decompile', ''), 'sourceSha256'), (source, 'annotatedSha256')):
        if hashlib.sha256(value.encode()).hexdigest() != witness[key]:
            return reject(key + ' changed')
    for item in (witness, witness['callee']):
        raw = rdata.bytes_at(int(item['address'], 16), item['size'])
        if raw is None or hashlib.sha256(raw).hexdigest() != item['bytesSha256']:
            return reject('native facing branch changed')
    if rdata.bytes_at(0x1260F0C + 0x76C, 4) != (0x88E620).to_bytes(4, 'little'):
        return reject('native facing binding changed')
    if source.count(witness['old']) != 1 or 'LAB_00db15e6' in source:
        return reject('facing branch source correspondence changed')
    return source.replace(witness['old'], witness['new'], 1), [dict(witness, status='recovered')]
