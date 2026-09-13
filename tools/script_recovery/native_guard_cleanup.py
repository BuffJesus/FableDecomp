"""Recover the unconditional exit hidden by Guard's refcount cleanup branch."""
import hashlib
import json
from pathlib import Path


LIMITATION = 'Guard exit cleanup: native ownership callback and control-resource destruction unresolved'


def recover_guard_cleanup(function, source, rdata):
    witness = json.loads(Path(__file__).with_name('native_guard_cleanup_witness.json').read_text())
    if str(function.get('address', '')).lower() != witness['address'].lower():
        return source, []
    def reject(reason):
        return source, [{'status': 'rejected', 'reason': reason}]
    for value, key in ((function.get('decompile', ''), 'sourceSha256'), (source, 'annotatedSha256')):
        if hashlib.sha256(value.encode()).hexdigest() != witness[key]:
            return reject(key + ' changed')
    raw = rdata.bytes_at(int(witness['address'], 16), witness['size'])
    if raw is None or hashlib.sha256(raw).hexdigest() != witness['bytesSha256']:
        return reject('native exit cleanup changed')
    if source.count(witness['fragment']) != 1:
        return reject('exit cleanup correspondence changed')
    return source.replace(witness['fragment'], 'return;', 1), [
        dict(witness, status='recovered', ownershipLimitation=LIMITATION)]
