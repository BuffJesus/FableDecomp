"""Recover the two directed BookTrader/hero ally calls and their nested getters."""
import hashlib
import json
from pathlib import Path

from tools.script_recovery.native_call_setup_ir import read_call_window


def recover_book_trader_allies(function, source, rdata, manifest):
    return _recover_allies(function, source, rdata, manifest, 'native_book_trader_allies_witness.json')


def recover_affair_wife_allies(function, source, rdata, manifest):
    return _recover_allies(function, source, rdata, manifest, 'native_affair_wife_allies_witness.json')


def _recover_allies(function, source, rdata, manifest, filename):
    witness = json.loads(Path(__file__).with_name(filename).read_text())
    if str(function.get('address', '')).lower() != witness['address'].lower():
        return source, []
    def reject(reason):
        return source, [{'status': 'rejected', 'reason': reason}]
    for value, key in ((function.get('decompile', ''), 'sourceSha256'), (source, 'annotatedSha256')):
        if hashlib.sha256(value.encode()).hexdigest() != witness[key]:
            return reject(key + ' changed')
    base, size = int(witness['address'], 16), witness.get('size', 4039)
    raw = rdata.bytes_at(base, size)
    if raw is None or hashlib.sha256(raw).hexdigest() != witness['bytesSha256']:
        return reject('native ally evidence changed')
    if rdata.bytes_at(0x1260F0C + 0x95C, 4) != (0x8A7EC0).to_bytes(4, 'little'):
        return reject('native ally binding changed')
    if manifest.get('EntitySetThingAsAllyOfThing') != witness['hostContract']:
        return reject('host ally contract changed')
    receiver = ('memory', ('address', ('register', 'esi'), 4))
    target = ('memory', ('address', ('memory', ('address', receiver, 0)), 0x95C))
    for getter, call, reverse in witness.get('calls', ((0xDB432B, 0xDB4336, False), (0xDB4342, 0xDB434C, True))):
        setup = read_call_window(rdata, base, size, call,
                                {getter: (0, 'GetHero')}, argument_count=2)
        args = (('register', witness.get('selfRegister', 'ebp')), ('result', getter, 'GetHero'))
        if reverse:
            args = args[::-1]
        if setup is None or setup.ecx != receiver or setup.target != target or setup.stack_arguments != args:
            return reject('native ally operands changed')
    edits = witness.get('edits') or [{'old': witness['old'], 'new': witness['new']}]
    if any(source.count(edit['old']) != 1 for edit in edits):
        return reject('ally source correspondence changed')
    for edit in edits:
        source = source.replace(edit['old'], edit['new'])
    return source, [dict(witness, status='recovered')]
