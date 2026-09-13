"""Recover the reviewed short-circuit hit classification and ability operand."""
import hashlib
import json
import re
from pathlib import Path


def recover_book_trader_hits(function, source, rdata, manifest):
    return _recover_hits(function, source, rdata, manifest, 'native_book_trader_hits_witness.json')


def recover_affair_wife_hits(function, source, rdata, manifest):
    return _recover_hits(function, source, rdata, manifest, 'native_affair_wife_hits_witness.json')


def recover_affair_wife_argument_hits(function, source, rdata, manifest):
    return _recover_hits(function, source, rdata, manifest, 'native_affair_wife_argument_hits_witness.json')


def recover_affair_man_hits(function, source, rdata, manifest):
    return _recover_hits(function, source, rdata, manifest, 'native_affair_man_hits_witness.json')


def _recover_hits(function, source, rdata, manifest, filename):
    witness = json.loads(Path(__file__).with_name(filename).read_text())
    if str(function.get('address', '')).lower() != witness['address'].lower():
        return source, []
    def reject(reason):
        return source, [{'status': 'rejected', 'reason': reason}]
    for value, key in ((function.get('decompile', ''), 'sourceSha256'), (source, 'annotatedSha256')):
        if hashlib.sha256(value.encode()).hexdigest() != witness[key]:
            return reject(key + ' changed')
    raw = rdata.bytes_at(int(witness['address'], 16), witness.get('size', 4042))
    if raw is None or hashlib.sha256(raw).hexdigest() != witness['bytesSha256']:
        return reject('native hit predicate evidence changed')
    if rdata.string_at(0x125D1C8) != 'SCRIPT_NAME_HERO':
        return reject('native hit source name changed')
    if any(manifest.get(name) != contract for name, contract in witness['contracts'].items()):
        return reject('host hit predicate contract changed')
    if source.count(witness['old']) != 1:
        return reject('hit predicate correspondence changed')
    for rename in witness.get('renames', []):
        if len(re.findall(r'\b' + re.escape(rename['old']) + r'\b', source)) != rename['count']:
            return reject('hit result uses changed')
    result = source.replace(witness['old'], witness['new'])
    for rename in witness.get('renames', []):
        result = re.sub(r'\b' + re.escape(rename['old']) + r'\b', rename['new'], result)
    return result, [dict(witness, status='recovered')]
