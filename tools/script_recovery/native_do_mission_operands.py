"""Recover reviewed DoMission call operands; ownership remains separately audited."""
import hashlib
import json
from pathlib import Path


def recover_do_mission_operands(function, source, rdata, manifest):
    return _recover(function, source, rdata, manifest, 'native_do_mission_operands_witness.json')


def recover_attack_stuff_operands(function, source, rdata, manifest):
    return _recover(function, source, rdata, manifest, 'native_attack_stuff_operands_witness.json')


def _recover(function, source, rdata, manifest, witness_file):
    witness = json.loads(Path(__file__).with_name(witness_file).read_text())
    if str(function.get('address', '')).lower() != witness['address'].lower():
        return source, []
    def reject(reason):
        return source, [{'status': 'rejected', 'reason': reason}]
    normalized = source.replace('\r', '')
    for value, key in ((function.get('decompile', ''), 'sourceSha256'), (normalized, 'annotatedSha256')):
        if hashlib.sha256(value.encode()).hexdigest() != witness[key]:
            return reject(key + ' changed')
    raw = rdata.bytes_at(int(witness['address'], 16), witness['size'])
    if raw is None or hashlib.sha256(raw).hexdigest() != witness['bytesSha256']:
        return reject('native mission operands changed')
    for region in witness['abiRegions']:
        raw = rdata.bytes_at(region['address'], region['size'])
        if raw is None or hashlib.sha256(raw).hexdigest() != region['sha256']:
            return reject('native call/string ABI evidence changed')
    if any(manifest.get(name) != contract for name, contract in witness['contracts'].items()):
        return reject('host mission contract changed')
    if any(normalized.count(edit['old']) != len(edit['replacements']) for edit in witness['edits']):
        return reject('mission call correspondence changed')
    for edit in witness['edits']:
        pieces = normalized.split(edit['old'])
        normalized = pieces[0] + ''.join(new + suffix for new, suffix in zip(edit['replacements'], pieces[1:]))
    return normalized, [dict(witness, status='recovered',
        limits=['Operand recovery does not prove native temporary-Thing/string lifetime parity.'])]
