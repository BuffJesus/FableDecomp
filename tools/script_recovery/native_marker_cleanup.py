"""Separate reviewed wrapper bookkeeping from marker-thread control flow."""
import hashlib
import json
from pathlib import Path

OWNERSHIP_LIMITATION = ('marker wrapper cleanup: Forge shared_ptr/GC lifetime does not prove '
                       'retail zero-reference destructor callback parity')


def recover_marker_cleanup(function, source, rdata):
    witness = json.loads(Path(__file__).with_name('native_marker_cleanup_witness.json').read_text())
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
            return reject('native cleanup evidence changed')
    for fragment in [witness['earlyCleanup'], witness['staging']] + witness['normalCleanup']:
        if source.count(fragment) != 1:
            return reject('cleanup source correspondence changed')
    result = source.replace(witness['earlyCleanup'], 'if (bVar2) {\nreturn;\n}', 1)
    for fragment in [witness['staging']] + witness['normalCleanup']:
        result = result.replace(fragment, '', 1)
    return result, [dict(witness, status='recovered', ownershipLimitation=OWNERSHIP_LIMITATION)]
