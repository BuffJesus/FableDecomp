"""Recover BookTrader's ordered-positive health gates and temporary actors."""
import hashlib
import json
from pathlib import Path

from tools.script_recovery.native_call_setup_ir import read_call_window


def recover_book_trader_health_values(function, source, rdata, manifest):
    witness = json.loads(Path(__file__).with_name('native_book_trader_health_values_witness.json').read_text())
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
            return reject('native health comparison evidence changed')
    if manifest.get('GetHealth') != witness['hostContract']:
        return reject('health contract changed')
    receiver = ('memory', ('address', ('register', 'esi'), 4))
    def target(slot):
        return ('memory', ('address', ('memory', ('address', receiver, 0)), slot))
    base, size = int(witness['address'], 16), witness['size']
    for site, priority in witness['acquisitions']:
        setup = read_call_window(rdata, base, size, site, argument_count=3)
        if (setup is None or setup.ecx != receiver or setup.target != target(0x20)
                or setup.stack_arguments != (('register', 'ebp'), ('stack', 20), ('constant', priority))):
            return reject('health actor acquisition changed')
    for getter, health, output in witness['queries']:
        setup = read_call_window(rdata, base, size, getter, argument_count=1)
        if (setup is None or setup.ecx != ('stack', 20) or setup.target != ('constant', 0x7E7490)
                or setup.stack_arguments != (('stack', output),)):
            return reject('health temporary actor changed')
        use = read_call_window(rdata, base, size, health,
                               {getter: (4, 'controlled_actor')}, argument_count=1)
        if (use is None or use.ecx != receiver or use.target != target(0x420)
                or use.stack_arguments != (('result', getter, 'controlled_actor'),)):
            return reject('health actor consumption changed')
    if any(source.count(edit['old']) != edit['count'] for edit in witness['edits']):
        return reject('health comparison source correspondence changed')
    for edit in witness['edits']:
        source = source.replace(edit['old'], edit['new'])
    return source, [dict(witness, status='recovered',
        resourceLifetime='unresolved; native temporary destructors retained',
        loweringStatus='seven ordered health > 0 gates and controlled actor aliases recovered')]
