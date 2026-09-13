"""Recover the reviewed EBX cleanup-mask flow without spurious EBP aliases."""
import hashlib
import json
from pathlib import Path


def recover_affair_man_mask(function, source, rdata):
    return recover_affair_wife_mask(function, source, rdata,
                                  witness_file='native_affair_man_mask_witness.json')


def recover_affair_wife_mask(function, source, rdata, *, witness_file='native_affair_wife_mask_witness.json'):
    witness = json.loads(Path(__file__).with_name(witness_file).read_text())
    if str(function.get('address', '')).lower() != witness['address'].lower():
        return source, []
    def reject(reason):
        return source, [{'status': 'rejected', 'reason': reason}]
    for value, key in ((function.get('decompile', ''), 'sourceSha256'), (source, 'annotatedSha256')):
        if hashlib.sha256(value.encode()).hexdigest() != witness[key]:
            return reject(key + ' changed')
    raw = rdata.bytes_at(int(witness['address'],16), witness.get('size',4890))
    if raw is None or hashlib.sha256(raw).hexdigest() != witness['bytesSha256']:
        return reject('native mask evidence changed')
    if any(source.count(e['old']) != e['count'] for e in witness['edits']):
        return reject('mask alias correspondence changed')
    for edit in witness['edits']:
        source = source.replace(edit['old'], edit['new'])
    return source, [dict(witness, status='recovered')]
