"""Recover BookTrader health operands from its reviewed self-control resource."""
import hashlib
import json
from pathlib import Path

from tools.script_recovery.native_call_setup_ir import read_call_window


def recover_affair_wife_health(function, source, rdata, manifest):
    return recover_book_trader_health(function, source, rdata, manifest,
                                     witness_file='native_affair_wife_health_witness.json')


def recover_affair_man_health(function, source, rdata, manifest):
    return recover_book_trader_health(function, source, rdata, manifest,
                                     witness_file='native_affair_man_health_witness.json')


def recover_book_trader_health(function, source, rdata, manifest, *,
                              witness_file='native_book_trader_health_witness.json'):
    witness = json.loads(Path(__file__).with_name(witness_file).read_text())
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
            return reject('native health/resource evidence changed')
    if manifest.get('GetHealth') != witness['hostContract']:
        return reject('health API contract changed')
    receiver = ('memory', ('address', ('register', 'esi'), 4))
    address = int(witness['address'], 16)
    size = witness.get('bodySize', 4039)
    resource = witness.get('resourceOffset', 20)
    actor = witness.get('selfRegister', 'ebp')
    def target(slot):
        return ('memory', ('address', ('memory', ('address', receiver, 0)), slot))
    for site, priority in witness['acquisitions']:
        setup = read_call_window(rdata, address, size, site, argument_count=3)
        if (setup is None or setup.ecx != receiver or setup.target != target(0x20)
                or setup.stack_arguments != (('register', actor), ('stack', resource), ('constant', priority))):
            return reject('self acquisition operands changed')
    for getter, health, output in witness['queries']:
        setup = read_call_window(rdata, address, size, getter, argument_count=1)
        if (setup is None or setup.ecx != ('stack', resource) or setup.target != ('constant', 0x7E7490)
                or setup.stack_arguments != (('stack', output),)):
            return reject('controlled actor query changed')
        use = read_call_window(rdata, address, size, health,
                               {getter: (4, 'controlled_actor')}, argument_count=1)
        if (use is None or use.ecx != receiver or use.target != target(0x420)
                or use.stack_arguments != (('result', getter, 'controlled_actor'),)):
            return reject('health actor consumption changed')
    if any(source.count(edit['old']) != edit['count'] for edit in witness['edits']):
        return reject('health source correspondence changed')
    for edit in witness['edits']:
        source = source.replace(edit['old'], edit['new'])
    return source, [dict(witness, status='recovered', cleanupStatus='unresolved')]
