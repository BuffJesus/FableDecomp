"""Recover BookTrader's intermittent line, including native-to-host argument order."""
import hashlib
import json
from pathlib import Path

from tools.script_recovery.native_call_setup_ir import read_call_window


def recover_book_trader_line(function, source, rdata, manifest):
    witness = json.loads(Path(__file__).with_name('native_book_trader_line_witness.json').read_text())
    if str(function.get('address', '')).lower() != witness['address'].lower():
        return source, []
    def reject(reason):
        return source, [{'status': 'rejected', 'reason': reason}]
    for value, key in ((function.get('decompile', ''), 'sourceSha256'), (source, 'annotatedSha256')):
        if hashlib.sha256(value.encode()).hexdigest() != witness[key]:
            return reject(key + ' changed')
    raw = rdata.bytes_at(0xDB3FA0, 4039)
    if raw is None or hashlib.sha256(raw).hexdigest() != witness['bytesSha256']:
        return reject('native line evidence changed')
    if rdata.bytes_at(0x1260F0C + 0x5B8, 4) != (0x890750).to_bytes(4, 'little'):
        return reject('native conversation binding changed')
    if manifest.get('AddLineToConversation') != witness['hostContract']:
        return reject('host conversation contract changed')
    receiver = ('memory', ('address', ('register', 'esi'), 4))
    target = ('memory', ('address', ('memory', ('address', receiver, 0)), 0x5B8))
    setup = read_call_window(rdata, 0xDB3FA0, 4039, 0xDB4E55,
                            {0xDB4E42: (0, 'GetHero')}, argument_count=5)
    expected = (('register', 'ebx'), ('stack', 112), ('constant', 0),
                ('register', 'ebp'), ('result', 0xDB4E42, 'GetHero'))
    if setup is None or setup.ecx != receiver or setup.target != target or setup.stack_arguments != expected:
        return reject('native conversation operands changed')
    if source.count(witness['old']) != 1:
        return reject('line source correspondence changed')
    return source.replace(witness['old'], witness['new']), [dict(witness, status='recovered')]
