"""Recover the fade, wait, and two teleports that start the barrel watch."""
import hashlib
import json
from pathlib import Path


def recover_barrel_departure(function, source, rdata, manifest):
    witness = json.loads(Path(__file__).with_name('native_barrel_departure_witness.json').read_text())
    if str(function.get('address', '')).lower() != witness['address'].lower():
        return source, []
    def reject(reason):
        return source, [{'status': 'rejected', 'reason': reason}]
    if hashlib.sha256(source.encode()).hexdigest() != witness['annotatedSha256']:
        return reject('departure source correspondence changed')
    for region in witness['regions']:
        raw = rdata.bytes_at(region['address'], region['size'])
        if raw is None or hashlib.sha256(raw).hexdigest() != region['sha256']:
            return reject('native departure operands changed')
    if any(rdata.string_at(int(address, 16)) != value for address, value in witness['strings'].items()):
        return reject('departure marker name changed')
    if any(manifest.get(name) != contract for name, contract in witness['contracts'].items()):
        return reject('departure API contract changed')
    if any(source.count(edit['old']) != edit['count'] for edit in witness['edits']):
        return reject('departure source operands changed')
    for edit in witness['edits']:
        source = source.replace(edit['old'], edit['new'])
    return source, [dict(witness, status='recovered', cleanupStatus='temporary Thing/string cleanup unresolved')]
