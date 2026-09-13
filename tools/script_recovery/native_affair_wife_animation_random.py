"""Recover the remaining retail random animation selector from pinned evidence."""
import hashlib
import json
from pathlib import Path


def recover_affair_wife_animation_random(function, source, rdata):
    w = json.loads(Path(__file__).with_name('native_affair_wife_animation_random_witness.json').read_text())
    if str(function.get('address', '')).lower() != w['address'].lower():
        return source, []
    def reject(reason):
        return source, [{'status': 'rejected', 'reason': reason}]
    for value, key in ((function.get('decompile', ''), 'sourceSha256'), (source, 'annotatedSha256')):
        if hashlib.sha256(value.encode()).hexdigest() != w[key]:
            return reject(key + ' changed')
    raw = rdata.bytes_at(0xDB2B10, 4890)
    if raw is None or hashlib.sha256(raw).hexdigest() != w['nativeSha256']:
        return reject('native random branch changed')
    if source.count(w['old']) != 1:
        return reject('random source correspondence changed')
    return source.replace(w['old'], w['new']), [dict(w, status='recovered')]
