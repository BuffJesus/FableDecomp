"""Recover reviewed scalar operands using native call-window data flow."""
import hashlib
import json
import struct
from dataclasses import asdict
from pathlib import Path

from tools.script_recovery.native_call_setup_ir import read_call_window


def recover_post_attack_scalars(function, source, rdata, manifest):
    return _recover_scalars(function, source, rdata, manifest, 'native_post_attack_scalars_witness.json')


def recover_post_attack_movie_flags(function, source, rdata, manifest):
    return _recover_scalars(function, source, rdata, manifest, 'native_post_attack_movie_flags_witness.json')


def _recover_scalars(function, source, rdata, manifest, witness_file):
    witness = json.loads(Path(__file__).with_name(witness_file).read_text())
    if str(function.get('address', '')).lower() != witness['address'].lower():
        return source, []
    def reject(reason):
        return source, [{'status': 'rejected', 'reason': reason}]
    for text, key in ((function.get('decompile', ''), 'sourceSha256'), (source, 'annotatedSha256')):
        if hashlib.sha256(text.encode()).hexdigest() != witness[key]:
            return reject(key + ' changed')
    start, size = int(witness['address'], 16), witness['size']
    raw = rdata.bytes_at(start, size)
    if raw is None or hashlib.sha256(raw).hexdigest() != witness['bytesSha256']:
        return reject('native scalar call evidence changed')
    receiver = ('memory', ('address', ('register', 'esi'), 0x40))
    vtable = ('memory', ('address', receiver, 0))
    edits, evidence = [], []
    for item in witness['calls']:
        contract = manifest.get(item['name'], {})
        if (contract.get('scope') != 'Quest' or contract.get('returnType') != 'void'
                or [p.get('type') for p in contract.get('parameters', [])] != [item['type']]):
            return reject('scalar API contract changed')
        if rdata.bytes_at(0x1260F0C + item['slot'], 4) != bytes.fromhex(item['vtableBytes']):
            return reject('scalar API vtable entry changed')
        setup = read_call_window(rdata, start, size, int(item['site'], 16), argument_count=1)
        if (setup is None or setup.ecx != receiver
                or setup.target != ('memory', ('address', vtable, item['slot']))
                or setup.stack_arguments != (('constant', item['word']),)):
            return reject('scalar operand data flow changed')
        if source[item['start']:item['end']] != item['original']:
            return reject('scalar call source correspondence changed')
        value = (struct.unpack('<f', struct.pack('<I', item['word']))[0] if item['type'] == 'float'
                 else struct.unpack('<i', struct.pack('<I', item['word']))[0])
        edits.append((item['start'], item['end'], f"GSI->{item['name']}({value!r});"))
        evidence.append(dict(item, status='recovered', setup=asdict(setup), value=value))
    for first, last, replacement in reversed(edits):
        source = source[:first] + replacement + source[last:]
    return source, evidence
