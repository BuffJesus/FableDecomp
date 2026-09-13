"""Remove witnessed temporary-wrapper bookkeeping from the initial availability wait."""
import hashlib
import json
from pathlib import Path

OWNERSHIP_LIMITATION = ('post-attack wrapper cleanup: Forge shared_ptr/GC lifetime does not prove '
                       'retail zero-reference destructor callback parity')


def recover_post_attack_cleanup(function, source, rdata):
    witness = json.loads(Path(__file__).with_name('native_post_attack_cleanup_witness.json').read_text())
    if str(function.get('address', '')).lower() != witness['address'].lower():
        return source, []
    def reject(reason):
        return source, [{'status': 'rejected', 'reason': reason}]
    for text, key in ((function.get('decompile', ''), 'sourceSha256'), (source, 'annotatedSha256')):
        if hashlib.sha256(text.encode()).hexdigest() != witness[key]:
            return reject(key + ' changed')
    for item in [witness] + witness['callees']:
        raw = rdata.bytes_at(int(item['address'], 16), item['size'])
        if raw is None or hashlib.sha256(raw).hexdigest() != item['bytesSha256']:
            return reject('native availability cleanup evidence changed')
    for key in ('staging', 'cleanup'):
        if source.count(witness[key]) != 1:
            return reject('availability cleanup source correspondence changed')
    result = source.replace(witness['staging'], '', 1).replace(witness['cleanup'], '', 1)
    return result, [dict(witness, status='recovered', ownershipLimitation=OWNERSHIP_LIMITATION)]
