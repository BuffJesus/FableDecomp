"""Recover the two presented-item queries and exact teddy-name comparisons."""
import hashlib
import json
from pathlib import Path


def recover_bully_presented_item(function, source, rdata, manifest):
    witness = json.loads(Path(__file__).with_name('native_bully_presented_item_witness.json').read_text())
    if str(function.get('address', '')).lower() != witness['address'].lower():
        return source, []
    def reject(reason):
        return source, [{'status': 'rejected', 'reason': reason}]
    if hashlib.sha256(source.encode()).hexdigest() != witness['annotatedSha256']:
        return reject('presented-item source correspondence changed')
    for region in witness['regions']:
        raw = rdata.bytes_at(region['address'], region['size'])
        if raw is None or hashlib.sha256(raw).hexdigest() != region['sha256']:
            return reject('native presented-item evidence changed')
    if rdata.string_at(int(witness['itemAddress'], 16)) != witness['itemName']:
        return reject('native teddy name changed')
    if manifest.get('MsgIsPresentedWithItem') != witness['hostContract']:
        return reject('host presented-item contract changed')
    if any(source.count(edit['old']) != edit['count'] for edit in witness['edits']):
        return reject('presented-item source operands changed')
    for edit in witness['edits']:
        source = source.replace(edit['old'], edit['new'])
    return source, [dict(witness, status='recovered', cleanupStatus='native output wrapper lifetime unresolved')]
