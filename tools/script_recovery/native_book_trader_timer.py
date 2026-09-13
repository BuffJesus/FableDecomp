"""Recover the shared quest timer used by BookTrader's intermittent speech."""
import hashlib
import json
from pathlib import Path

from tools.script_recovery.native_call_setup_ir import read_call_window


def recover_book_trader_timer(function, source, rdata, manifest):
    return _recover_timer(function, source, rdata, manifest, 'native_book_trader_timer_witness.json')


def recover_affair_wife_timer(function, source, rdata, manifest):
    return _recover_timer(function, source, rdata, manifest, 'native_affair_wife_timer_witness.json')


def _recover_timer(function, source, rdata, manifest, filename):
    witness = json.loads(Path(__file__).with_name(filename).read_text())
    if str(function.get('address', '')).lower() != witness['address'].lower():
        return source, []
    def reject(reason):
        return source, [{'status': 'rejected', 'reason': reason}]
    for value, key in ((function.get('decompile', ''), 'sourceSha256'), (source, 'annotatedSha256')):
        if hashlib.sha256(value.encode()).hexdigest() != witness[key]:
            return reject(key + ' changed')
    raw = rdata.bytes_at(int(witness['address'], 16), witness['size'])
    if raw is None or hashlib.sha256(raw).hexdigest() != witness['bytesSha256']:
        return reject('native timer evidence changed')
    receiver = ('memory', ('constant', 0x143E8F8))
    timer = ('memory', ('address', ('memory', ('address', ('register', 'esi'), 0x14)), 0x104))
    for name, site, slot, args, result in (
            ('GetTimer', witness.get('getSite', 0xDB4CF9), 0x168, (timer,), 'int'),
            ('SetTimer', witness.get('setSite', 0xDB4D60), 0x164, (timer, ('constant', 3)), 'void')):
        spec = manifest.get(name, {})
        if (spec.get('scope') != 'Quest' or spec.get('returnType') != result
                or [p.get('type') for p in spec.get('parameters', [])] != ['int'] * len(args)):
            return reject('timer API contract changed')
        setup = read_call_window(rdata, int(witness['address'], 16), witness['size'], site, argument_count=len(args))
        target = ('memory', ('address', ('memory', ('address', receiver, 0)), slot))
        if setup is None or setup.ecx != receiver or setup.target != target or setup.stack_arguments != args:
            return reject('native timer operands changed')
    if any(source.count(edit['old']) != 1 for edit in witness['edits']):
        return reject('timer source correspondence changed')
    for edit in witness['edits']:
        source = source.replace(edit['old'], edit['new'])
    return source, [dict(witness, status='recovered')]
