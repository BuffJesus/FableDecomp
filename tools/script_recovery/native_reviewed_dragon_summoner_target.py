"""Recover Dragon's shared native summoner-target write after phase transitions."""

import hashlib
from capstone import Cs, CS_ARCH_X86, CS_MODE_32

OLD = '*(undefined4 *)(this_00 + 0x6c) = uVar4;'
NEW = '*(undefined4 *)(*(int *)(this + 0x14) + 0x6c) = uVar4;'
WINDOW_SHA = '99d627b1c87b1fad3ac9c687aa57433fe3001fdd6785a5e54c394c587367f537'


def restore(source, fn, row, rdata):
    if row is None:
        return source
    if hashlib.sha256(fn['decompile'].encode()).hexdigest() != row['sourceSha256']:
        raise ValueError('Dragon target source changed')
    begin, end = int(fn['address'], 16), int(fn['bodyEndExclusive'], 16)
    if hashlib.sha256(rdata.bytes_at(begin, end - begin)).hexdigest() != row['nativeSha256']:
        raise ValueError('Dragon target native body changed')
    window = rdata.bytes_at(0x00d25b26, 0x00d25c00 - 0x00d25b26)
    if hashlib.sha256(window).hexdigest() != WINDOW_SHA or row['storeWindowSha256'] != WINDOW_SHA:
        raise ValueError('Dragon target store window changed')
    ins = {i.address: (i.mnemonic, i.op_str) for i in Cs(CS_ARCH_X86, CS_MODE_32).disasm(window, 0x00d25b26)}
    expected = {
        0x00d25b2b: ('mov', 'ecx, dword ptr [eax + 0xbc4]'),
        0x00d25b31: ('mov', 'dword ptr [edi + 0x68], ecx'),
        0x00d25b3a: ('mov', 'eax, dword ptr [edx + 0xbd4]'),
        0x00d25b40: ('jmp', '0xd25bfd'),
        0x00d25b8d: ('mov', 'ecx, dword ptr [eax + 0xbc8]'),
        0x00d25b93: ('mov', 'dword ptr [edi + 0x68], ecx'),
        0x00d25b9c: ('mov', 'eax, dword ptr [edx + 0xbd8]'),
        0x00d25ba2: ('jmp', '0xd25bfd'),
        0x00d25be8: ('mov', 'ecx, dword ptr [eax + 0xbcc]'),
        0x00d25bee: ('mov', 'dword ptr [edi + 0x68], ecx'),
        0x00d25bf7: ('mov', 'eax, dword ptr [edx + 0xbdc]'),
        0x00d25bfd: ('mov', 'dword ptr [edi + 0x6c], eax'),
    }
    if any(ins.get(address) != instruction for address, instruction in expected.items()):
        raise ValueError('Dragon target stores changed')
    if source.count(OLD) != 1 or source.count('this_00 = *(CScriptBase **)(this + 0x14);') != 3:
        raise ValueError('Dragon target alias source changed')
    return source.replace(OLD, NEW, 1)
