"""Recover small Scythe entity entrypoints from isolated native exports."""
import hashlib
import json
from dataclasses import asdict
from pathlib import Path

from tools.script_recovery.native_call_setup_ir import read_call_window
from tools.script_recovery.native_resource_lifetime import check_single_resource_lifetime


def recover_scythe_entity(function, source, rdata, manifest):
    names = {'0x00e29f50': 'marker', '0x00e2a0f0': 'near_init', '0x00e2a320': 'near_setup'}
    name = names.get(str(function.get('address', '')).lower())
    if name is None:
        return source, []
    return _recover(function, source, rdata, manifest, name)


def recover_scythe_behavior(function, source, rdata, manifest):
    if str(function.get('address', '')).lower() != '0x00e2a320':
        return source, []
    return _recover(function, source, rdata, manifest, 'near_behavior')


def _recover(function, source, rdata, manifest, name):
    witness = json.loads(Path(__file__).with_name('native_scythe_' + name + '_witness.json').read_text())
    def reject(reason):
        return source, [{'status': 'rejected', 'reason': reason}]
    for value, key in ((function.get('decompile', ''), 'sourceSha256'), (source, 'annotatedSha256')):
        if hashlib.sha256(value.encode()).hexdigest() != witness[key]:
            return reject(key + ' changed')
    for region in witness['regions']:
        raw = rdata.bytes_at(region['address'], region['size'])
        if raw is None or hashlib.sha256(raw).hexdigest() != region['sha256']:
            return reject('native Scythe entity evidence changed')
    for address, value in witness['strings'].items():
        actual = ('' if not value and rdata.bytes_at(int(address, 16), 1) == b'\0'
                  else rdata.string_at(int(address, 16)))
        if actual != value:
            return reject('native Scythe entity name changed')
    if any(manifest.get(name) != contract for name, contract in witness['contracts'].items()):
        return reject('Scythe entity contract changed')
    for call in witness['calls']:
        setup = read_call_window(rdata, int(witness['address'], 16), witness['size'], call['site'],
            {int(k): tuple(v) for k, v in call['known'].items()}, argument_count=call['count'])
        if setup is None:
            return reject('Scythe entity call unavailable')
        actual = json.loads(json.dumps(asdict(setup)))
        if any(actual.get(k) != v for k, v in call['expected'].items()):
            return reject('Scythe entity operands changed')
    if any(source.count(edit['old']) != edit['count'] for edit in witness['edits']):
        return reject('Scythe entity source correspondence changed')
    for event_kind in ('timerEvents', 'movieEvents'):
        if event_kind not in witness:
            continue
        from capstone import Cs, CS_ARCH_X86, CS_MODE_32
        decoder = Cs(CS_ARCH_X86, CS_MODE_32)
        decoder.detail = True
        instructions = list(decoder.disasm(rdata.bytes_at(int(witness['address'], 16), witness['size']),
                                            int(witness['address'], 16)))
        events = {int(site, 16): tuple(event) for site, event in witness[event_kind].items()}
        if not check_single_resource_lifetime(instructions, events):
            return reject('Scythe entity ' + event_kind + ' lifetime changed')
    for edit in witness['edits']:
        source = source.replace(edit['old'], edit['new'])
    return source, [dict(witness, status='recovered',
        lifetimeLimitation='Native temporary ownership remains separately reviewable; registration disabled.')]
