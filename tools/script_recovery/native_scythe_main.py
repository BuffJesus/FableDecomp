"""Byte-backed ScytheInfo Main operands and timer cleanup."""
import hashlib
import json
import re
from dataclasses import asdict
from pathlib import Path

from capstone import Cs, CS_ARCH_X86, CS_MODE_32
from tools.script_recovery.native_call_setup_ir import read_call_window
from tools.script_recovery.native_resource_lifetime import check_single_resource_lifetime


def recover_scythe_main(function, source, rdata, manifest):
    witness = json.loads(Path(__file__).with_name('native_scythe_main_witness.json').read_text())
    if str(function.get('address', '')).lower() != witness['address'].lower():
        return source, []
    def reject(reason):
        return source, [{'status': 'rejected', 'reason': reason}]
    for value, key in ((function.get('decompile', ''), 'sourceSha256'), (source, 'annotatedSha256')):
        if hashlib.sha256(value.encode()).hexdigest() != witness[key]:
            return reject(key + ' changed')
    for region in witness['regions']:
        raw = rdata.bytes_at(region['address'], region['size'])
        if raw is None or hashlib.sha256(raw).hexdigest() != region['sha256']:
            return reject('native Scythe main evidence changed')
    for address, expected in witness['strings'].items():
        if rdata.string_at(int(address, 16)) != expected:
            return reject('Scythe string evidence changed')
    if any(manifest.get(name) != contract for name, contract in witness['contracts'].items()):
        return reject('Scythe host contract changed')
    base, size = int(witness['address'], 16), witness['size']
    for call in witness['calls']:
        setup = read_call_window(rdata, base, size, call['site'],
            {int(k): tuple(v) for k, v in call['known'].items()}, argument_count=call['count'])
        if setup is None:
            return reject('Scythe native call setup unavailable')
        actual = json.loads(json.dumps(asdict(setup)))
        if any(actual.get(k) != v for k, v in call['expected'].items()):
            return reject('Scythe native call operands changed')
    decoder = Cs(CS_ARCH_X86, CS_MODE_32)
    decoder.detail = True
    instructions = list(decoder.disasm(rdata.bytes_at(base, size), base))
    events = {int(site, 16): tuple(event) for site, event in witness['timerEvents'].items()}
    if not check_single_resource_lifetime(instructions, events):
        return reject('Scythe timer is not released exactly once on every native exit')
    if any(source.count(edit['old']) != edit['count'] for edit in witness['edits']):
        return reject('Scythe source correspondence changed')
    for edit in witness['edits']:
        source = source.replace(edit['old'], edit['new'])
    source = re.sub(r'\buVar5\b', 'SnowspireReminder', source)
    return source, [dict(witness, status='recovered', timerLifetimeChecked=True,
        limitations=['Entity bodies and PlayCutscene helper require separate recovery.'])]
