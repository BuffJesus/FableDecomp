"""Scratch split of the native Briar wave else branch for the Lua lifter."""

import hashlib

from capstone import Cs, CS_ARCH_X86, CS_MODE_32


OLD = '        } else {\nif ((x_stk_7c == 1) &&'
NEW = '        }\n        else {\nif ((x_stk_7c == 1) &&'


def restore(source, fn, row, rdata):
    if row is None:
        return source
    if hashlib.sha256(fn['decompile'].encode()).hexdigest() != row['sourceSha256']:
        raise ValueError('Briar wave source changed')
    begin, end = int(fn['address'], 16), int(fn['bodyEndExclusive'], 16)
    if hashlib.sha256(rdata.bytes_at(begin, end - begin)).hexdigest() != row['nativeSha256']:
        raise ValueError('Briar wave native body changed')
    window = rdata.bytes_at(0x00df19a8, 0x00df1a64 - 0x00df19a8)
    if hashlib.sha256(window).hexdigest() != '8adac21d60fe4b9429965a98f334e99d4a2e7d92c578e612040c3897a034a34b':
        raise ValueError('Briar wave branch changed')
    instructions = {i.address: (i.mnemonic, i.op_str) for i in Cs(CS_ARCH_X86, CS_MODE_32).disasm(window, 0x00df19a8)}
    expected = {
        0x00df19ab: ('mov', 'eax, dword ptr [eax + 0x4c]'),
        0x00df19b3: ('cmp', 'eax, ebp'),
        0x00df19b5: ('jne', '0xdf1a5a'),
        0x00df1a4d: ('jmp', '0xdf1b8b'),
        0x00df1a52: ('cmp', 'eax, ebp'),
        0x00df1a54: ('je', '0xdf1b8b'),
        0x00df1a5a: ('cmp', 'dword ptr [esp + 0x28], ebp'),
        0x00df1a5e: ('jne', '0xdf1b8b'),
    }
    if any(instructions.get(address) != instruction for address, instruction in expected.items()):
        raise ValueError('Briar wave control flow changed')
    if source.count(OLD) != 1:
        raise ValueError('Briar wave printed else changed')
    return source.replace(OLD, NEW)
