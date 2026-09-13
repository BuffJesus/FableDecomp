"""Recover self operands surrounding AffairWife's unresolved position movement."""
import hashlib
import json
from pathlib import Path

from tools.script_recovery.native_call_setup_ir import read_call_window


def recover_affair_wife_route(function, source, rdata, manifest):
    witness = json.loads(Path(__file__).with_name('native_affair_wife_route_witness.json').read_text())
    if str(function.get('address', '')).lower() != '0x00db2b10':
        return source, []
    def reject(reason):
        return source, [{'status': 'rejected', 'reason': reason}]
    for value, key in ((function.get('decompile', ''), 'sourceSha256'), (source, 'annotatedSha256')):
        if hashlib.sha256(value.encode()).hexdigest() != witness[key]:
            return reject(key + ' changed')
    for region in witness['regions']:
        raw = rdata.bytes_at(region['address'], region['size'])
        if raw is None or hashlib.sha256(raw).hexdigest() != region['sha256']:
            return reject('native route actor evidence changed')
    receiver = ('memory', ('address', ('register', 'esi'), 4))
    for name, site, slot, args in (
            ('EntitySetAsUseMovementInActions', 0xDB345C, 0x844, (('register', 'edi'), ('constant', 1))),
            ('ClearThingHasInformation', 0xDB3491, 0x5A4, (('register', 'edi'),))):
        if manifest.get(name) != witness['contracts'][name]:
            return reject('route API contract changed')
        setup = read_call_window(rdata, 0xDB2B10, 4887, site, argument_count=len(args))
        target = ('memory', ('address', ('memory', ('address', receiver, 0)), slot))
        if setup is None or setup.ecx != receiver or setup.target != target or setup.stack_arguments != args:
            return reject('route actor operands changed')
    if any(source.count(edit['old']) != 1 for edit in witness['edits']):
        return reject('route source correspondence changed')
    for edit in witness['edits']:
        source = source.replace(edit['old'], edit['new'])
    return source, [dict(witness, status='recovered', positionLowering='unresolved')]
