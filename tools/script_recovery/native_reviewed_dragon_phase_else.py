"""Scratch restoration of the retail Dragon health-phase else-if chain."""

import hashlib

from capstone import Cs, CS_ARCH_X86, CS_MODE_32


OLD = '} else {\nif ((*(int *)(*(int *)(this + 0x14) + 0x48) == '
NEW = '}\nelse {\nif ((*(int *)(*(int *)(this + 0x14) + 0x48) == '
WINDOW_SHA = '48f53b529a530e7470c5b1be32453f3fbbdb7471ecfc8771a0862ff3611e000a'


def restore(source, fn, row, rdata):
    if row is None:
        return source
    if hashlib.sha256(fn['decompile'].encode()).hexdigest() != row['sourceSha256']:
        raise ValueError('Dragon phase source changed')
    begin, end = int(fn['address'], 16), int(fn['bodyEndExclusive'], 16)
    if hashlib.sha256(rdata.bytes_at(begin, end - begin)).hexdigest() != row['nativeSha256']:
        raise ValueError('Dragon phase native body changed')
    window = rdata.bytes_at(0x00d25ade, 0x00d25c00 - 0x00d25ade)
    if hashlib.sha256(window).hexdigest() != WINDOW_SHA or row['branchWindowSha256'] != WINDOW_SHA:
        raise ValueError('Dragon phase native branch changed')
    ins = {i.address: (i.mnemonic, i.op_str) for i in Cs(CS_ARCH_X86, CS_MODE_32).disasm(window, 0x00d25ade)}
    expected = {
        0x00d25ae1: ('mov', 'ecx, dword ptr [eax + 0x48]'),
        0x00d25ae4: ('test', 'ecx, ecx'),
        0x00d25ae6: ('jne', '0xd25b45'),
        0x00d25b12: ('mov', 'dword ptr [edi + 0x48], 1'),
        0x00d25b48: ('cmp', 'dword ptr [ecx + 0x48], 1'),
        0x00d25b4c: ('jne', '0xd25ba4'),
        0x00d25b78: ('mov', 'dword ptr [edi + 0x48], 2'),
        0x00d25ba7: ('cmp', 'dword ptr [ecx + 0x48], 2'),
        0x00d25bab: ('jne', '0xd25c00'),
        0x00d25bd3: ('mov', 'dword ptr [edi + 0x48], 3'),
    }
    if any(ins.get(address) != instruction for address, instruction in expected.items()):
        raise ValueError('Dragon phase transitions changed')
    if source.count(OLD) != 2 or source.count(OLD + '1)') != 1 or source.count(OLD + '2)') != 1:
        raise ValueError('Dragon phase else source changed')
    return source.replace(OLD, NEW)
