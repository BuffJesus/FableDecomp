"""Recover both village-limbo flags and their fresh lookup results."""
import hashlib
import json
from dataclasses import asdict
from pathlib import Path

from tools.script_recovery.native_call_setup_ir import read_call_window


def recover_post_attack_limbo(function, source, rdata, manifest):
    witness = json.loads(Path(__file__).with_name('native_post_attack_limbo_witness.json').read_text())
    if str(function.get('address', '')).lower() != witness['address'].lower():
        return source, []
    def reject(reason):
        return source, [{'status': 'rejected', 'reason': reason}]
    for text, key in ((function.get('decompile', ''), 'sourceSha256'), (source, 'annotatedSha256')):
        if hashlib.sha256(text.encode()).hexdigest() != witness[key]:
            return reject(key + ' changed')
    for address, size, digest in ((int(witness['address'], 16), witness['size'], witness['bytesSha256']),
                                   (0x8A7D60, 151, witness['lookupBytesSha256'])):
        raw = rdata.bytes_at(address, size)
        if raw is None or hashlib.sha256(raw).hexdigest() != digest:
            return reject('native limbo evidence changed')
    if (rdata.bytes_at(0x1260F0C + 0x120, 4) != (0x8A7D60).to_bytes(4, 'little')
            or rdata.bytes_at(0x1260F0C + 0x6E0, 4) != bytes.fromhex(witness['vtableBytes'])
            or rdata.string_at(0x12D9DF8) != 'V_OakVale'):
        return reject('village lookup or limbo binding changed')
    contract = manifest.get('SetVillageLimbo', {})
    if (contract.get('scope') != 'Quest' or contract.get('returnType') != 'void'
            or [p.get('type') for p in contract.get('parameters', [])] !=
                ['const std::shared_ptr<CScriptThing>&', 'bool']):
        return reject('village limbo contract changed')
    receiver = ('memory', ('address', ('register', 'esi'), 0x40))
    vtable = ('memory', ('address', receiver, 0))
    evidence, edits = [], []
    for item in witness['calls']:
        lookup_site = int(item['lookup'], 16)
        lookup = read_call_window(rdata, 0xDBEB20, witness['size'], lookup_site, argument_count=2)
        setup = read_call_window(rdata, 0xDBEB20, witness['size'], int(item['site'], 16),
                                 {lookup_site: (8, 'village')}, argument_count=2)
        if (lookup is None or lookup.ecx != receiver
                or lookup.target != ('memory', ('address', vtable, 0x120))
                or setup is None or setup.ecx != receiver
                or setup.target != ('memory', ('address', vtable, 0x6E0))
                or setup.stack_arguments != (('result', lookup_site, 'village'), ('constant', item['value']))):
            return reject('village result or limbo flag data flow changed')
        if source[item['start']:item['end']] != item['original']:
            return reject('limbo call source correspondence changed')
        edits.append((item['start'], item['end'], f"GSI->SetVillageLimbo({item['actor']},{item['value']});"))
        evidence.append(dict(item, status='recovered', lookupSetup=asdict(lookup), setup=asdict(setup)))
    for start, end, replacement in reversed(edits):
        source = source[:start] + replacement + source[end:]
    return source, evidence
