"""Scratch restoration of the native summoner-count comment branch."""

import hashlib

from capstone import Cs, CS_ARCH_X86, CS_MODE_32


OLD = '''          iVar9 = *(int *)(*(CQ_SummoningTheShipScript **)(this + 0x14) + 0x48);
          if (((0 < iVar9) && (iVar9 < 4)) && (bVar5 = NScript::CQ_SummoningTheShipScript::MakeBriarRoseComment (*(CQ_SummoningTheShipScript **)(this + 0x14),&xStack_80), bVar5)) {'''
NEW = '''          iVar9 = *(int *)(*(int *)(this + 0x14) + 0x48);
          if ((0 < iVar9) && (iVar9 < 4)) {
            bVar5 = NScript::CQ_SummoningTheShipScript::MakeBriarRoseComment(*(int **)(this + 0x14),pOther);
            if (bVar5) {'''
OLD_END = '''            i_stk_38 = *(int *)(*(int *)(this + 0x14) + 0x48);
          }
          std::_Cons_val'''
NEW_END = '''            i_stk_38 = *(int *)(*(int *)(this + 0x14) + 0x48);
            }
          }
          std::_Cons_val'''


def restore(source, fn, row, rdata):
    if row is None:
        return source
    if hashlib.sha256(fn['decompile'].encode()).hexdigest() != row['sourceSha256']:
        raise ValueError('Briar comment source changed')
    begin = int(fn['address'], 16)
    end = int(fn['bodyEndExclusive'], 16)
    if hashlib.sha256(rdata.bytes_at(begin, end - begin)).hexdigest() != row['nativeSha256']:
        raise ValueError('Briar comment native body changed')
    if not any(c['site'].lower() == '0x00df1bf2' and c['target'].lower() == '0x00df20f0' for c in fn['calls']):
        raise ValueError('Briar comment native call changed')
    window = rdata.bytes_at(0x00df1bde, 0x00df1c14 - 0x00df1bde)
    if hashlib.sha256(window).hexdigest() != '00eac1b79898c279538517d7934a961a640d6569251dc1c7ce870bcd843d2d40':
        raise ValueError('Briar comment branch changed')
    instructions = {i.address: (i.mnemonic, i.op_str) for i in Cs(CS_ARCH_X86, CS_MODE_32).disasm(window, 0x00df1bde)}
    expected = {
        0x00df1be1: ('mov', 'eax, dword ptr [ecx + 0x48]'),
        0x00df1be6: ('jle', '0xdf1c14'),
        0x00df1be8: ('cmp', 'eax, 3'),
        0x00df1beb: ('jg', '0xdf1c14'),
        0x00df1bf2: ('call', '0xdf20f0'),
        0x00df1bf7: ('test', 'al, al'),
        0x00df1bf9: ('je', '0xdf1c14'),
        0x00df1c0d: ('mov', 'ecx, dword ptr [eax + 0x48]'),
        0x00df1c10: ('mov', 'dword ptr [esp + 0x6c], ecx'),
    }
    if any(instructions.get(address) != instruction for address, instruction in expected.items()):
        raise ValueError('Briar comment branch ABI changed')
    for key in ('SUMMONER_ONE_LEFT', 'SUMMONER_TWO_LEFT', 'SUMMONER_THREE_LEFT'):
        if fn['decompile'].count('"' + key + '"') != 1:
            raise ValueError('Briar comment speech selection changed')
    if source.count(OLD) != 1 or source.count(OLD_END) != 1:
        raise ValueError('Briar comment source shape changed')
    return source.replace(OLD, NEW).replace(OLD_END, NEW_END)
