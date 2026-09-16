"""Verify the scoped intermittent-conversation call and borrowed hero evidence."""
import hashlib
import json
from pathlib import Path

from tools.script_recovery.native_call_setup_ir import read_call_window


def verify(data):
    witness = json.loads(Path(__file__).with_name('native_book_trader_conversation_witness.json').read_text())
    for row in witness['windows']:
        raw = data.bytes_at(row['address'], row['size'])
        if raw is None or hashlib.sha256(raw).hexdigest() != row['sha256']:
            raise ValueError('BookTrader conversation native bytes changed')
    for offset, target in witness['bindings'].items():
        if data.bytes_at(0x1260F0C + int(offset, 16), 4) != target.to_bytes(4, 'little'):
            raise ValueError('BookTrader conversation binding changed')
    if data.string_at(0x12D8E9C) != 'TEXT_QST_048_TRADER_ROLL_UP':
        raise ValueError('BookTrader conversation literal changed')
    receiver = ('memory', ('address', ('register', 'esi'), 4))
    specs = [
        (0xDB4D70, 0x5B0, {}, (('register', 'ebp'), ('constant', 0), ('constant', 0))),
        (0xDB4D88, 0x5B4, {0xDB4D70: (12, 'conversation'), 0xDB4D7D: (0, 'GetHero')},
            (('result', 0xDB4D70, 'conversation'), ('result', 0xDB4D7D, 'GetHero'))),
        (0xDB4E55, 0x5B8, {0xDB4E42: (0, 'GetHero')},
            (('register', 'ebx'), ('stack', 112), ('constant', 0), ('register', 'ebp'),
             ('result', 0xDB4E42, 'GetHero')))]
    for site, offset, returns, arguments in specs:
        setup = read_call_window(data, 0xDB3FA0, 4042, site, returns, argument_count=len(arguments))
        target = ('memory', ('address', ('memory', ('address', receiver, 0)), offset))
        if setup is None or setup.ecx != receiver or setup.target != target or setup.stack_arguments != arguments:
            raise ValueError('BookTrader conversation operands changed')
    return dict(witness, status='verified', heroOwnership='borrowed interface+0x30',
                lineAdapter='AddConversationLineToHero', setupAdapter='StartConversationWithHero',
                remaining='full runtime/gameplay integration')
