"""Recover the hero-talk plus teddy-possession predicate before offer dialogue."""
import hashlib
import json
from pathlib import Path


def recover_bully_hit_result(function, source, rdata, manifest):
    return recover_bully_teddy_offer(function, source, rdata, manifest,
                                    witness_file='native_bully_hit_result_witness.json')


def recover_bully_teddy_offer(function, source, rdata, manifest, *,
                             witness_file='native_bully_teddy_offer_witness.json'):
    witness = json.loads(Path(__file__).with_name(witness_file).read_text())
    if str(function.get('address', '')).lower() != witness['address'].lower():
        return source, []
    def reject(reason):
        return source, [{'status': 'rejected', 'reason': reason}]
    if hashlib.sha256(source.encode()).hexdigest() != witness['annotatedSha256']:
        return reject('teddy offer source correspondence changed')
    for region in witness['regions']:
        raw = rdata.bytes_at(region['address'], region['size'])
        if raw is None or hashlib.sha256(raw).hexdigest() != region['sha256']:
            return reject('native teddy offer predicate changed')
    if rdata.string_at(int(witness['stringAddress'], 16)) != witness['itemName']:
        return reject('native teddy definition changed')
    if any(manifest.get(name) != contract for name, contract in witness['contracts'].items()):
        return reject('teddy offer API contract changed')
    if any(source.count(edit['old']) != edit['count'] for edit in witness['edits']):
        return reject('teddy offer result uses changed')
    for edit in witness['edits']:
        source = source.replace(edit['old'], edit['new'])
    return source, [dict(witness, status='recovered')]
