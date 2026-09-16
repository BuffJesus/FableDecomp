"""Recover BarrelMan Init's timer, copied home vector and explicit API operands."""
import hashlib
import json
from pathlib import Path


def recover_barrel_init(function, source, rdata, manifest, parent_state):
    witness = json.loads(Path(__file__).with_name('native_barrel_init_witness.json').read_text())
    if str(function.get('address', '')).lower() != witness['address'].lower():
        return source, []
    def reject(reason):
        return source, [{'status': 'rejected', 'reason': reason}]
    for value, key in ((function.get('decompile', ''), 'sourceSha256'), (source, 'annotatedSha256')):
        if hashlib.sha256(value.encode()).hexdigest() != witness[key]:
            return reject(key + ' changed')
    raw = rdata.bytes_at(int(witness['address'], 16), witness['size'])
    if raw is None or hashlib.sha256(raw).hexdigest() != witness['bytesSha256']:
        return reject('native Init operands changed')
    if any(manifest.get(name) != contract for name, contract in witness['contracts'].items()):
        return reject('host Init contract changed')
    if any(list(parent_state.get(offset, ())) != field for offset, field in witness['parentFields'].items()):
        return reject('parent timer/vector ownership unavailable')
    if any(source.count(edit['old']) != 1 for edit in witness['edits']):
        return reject('Init source correspondence changed')
    for edit in witness['edits']:
        source = source.replace(edit['old'], edit['new'])
    return source, [dict(witness, status='recovered')]
