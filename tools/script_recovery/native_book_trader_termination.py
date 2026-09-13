"""Recover loop-carried termination results obscured by pure local assignments."""
import hashlib
import json
from pathlib import Path


def recover_book_trader_termination(function, source, rdata):
    return _recover_termination(function, source, rdata, 'native_book_trader_termination_witness.json')


def recover_affair_wife_termination(function, source, rdata):
    return _recover_termination(function, source, rdata, 'native_affair_wife_termination_witness.json')


def _recover_termination(function, source, rdata, filename):
    witness = json.loads(Path(__file__).with_name(filename).read_text())
    if str(function.get('address', '')).lower() != witness['address'].lower():
        return source, []
    def reject(reason):
        return source, [{'status': 'rejected', 'reason': reason}]
    for value, key in ((function.get('decompile', ''), 'sourceSha256'), (source, 'annotatedSha256')):
        if hashlib.sha256(value.encode()).hexdigest() != witness[key]:
            return reject(key + ' changed')
    for region in witness['regions']:
        raw = rdata.bytes_at(region['address'], region['size'])
        if raw is None or hashlib.sha256(raw).hexdigest() != region['sha256']:
            return reject('native termination evidence changed')
    if any(source.count(edit['old']) != 1 for edit in witness['edits']):
        return reject('termination result correspondence changed')
    for edit in witness['edits']:
        source = source.replace(edit['old'], edit['new'])
    return source, [dict(witness, status='recovered')]
