"""Recover Bully's two hero talk queries and three hero hit queries."""
import hashlib
import json
from pathlib import Path


def recover_bully_messages(function, source, rdata, manifest):
    witness = json.loads(Path(__file__).with_name('native_bully_messages_witness.json').read_text())
    if str(function.get('address', '')).lower() != witness['address'].lower():
        return source, []
    def reject(reason):
        return source, [{'status': 'rejected', 'reason': reason}]
    for value, key in ((function.get('decompile', ''), 'sourceSha256'), (source, 'annotatedSha256')):
        if hashlib.sha256(value.encode()).hexdigest() != witness[key]:
            return reject(key + ' changed')
    raw = rdata.bytes_at(int(witness['address'], 16), witness['size'])
    if raw is None or hashlib.sha256(raw).hexdigest() != witness['bytesSha256']:
        return reject('native Bully message evidence changed')
    if rdata.string_at(0x125D1C8) != 'SCRIPT_NAME_HERO':
        return reject('native hero name changed')
    if any(manifest.get(name) != contract for name, contract in witness['contracts'].items()):
        return reject('host message contract changed')
    if any(source.count(edit['old']) != edit['count'] for edit in witness['edits']):
        return reject('message source correspondence changed')
    for edit in witness['edits']:
        source = source.replace(edit['old'], edit['new'])
    return source, [dict(witness, status='recovered')]
