"""Review the two retail FadeInThing calls before replacing their printed name."""

import hashlib

from capstone import Cs, CS_ARCH_X86, CS_MODE_32


OLD = 'FadeInThing(p0,0x40000000);'
NEW = 'ReviewedFadeInThing(p0,0x40000000);'
CALLEE = 0x00CBE7AA
CALLEE_END = 0x00CBE814


def restore(text, fn, row, rdata):
    if row is None:
        return text

    def require(condition, reason):
        if not condition:
            raise ValueError(f'reviewed FadeInThing: {reason}')

    begin = int(fn['address'], 16)
    end = int(fn['bodyEndExclusive'], 16)
    site = int(row['site'], 16)
    window_start = int(row['windowStart'], 16)
    require(hashlib.sha256(fn['decompile'].encode()).hexdigest() == row['sourceSha256'], 'source changed')
    require(hashlib.sha256(rdata.bytes_at(begin, end - begin)).hexdigest() == row['nativeSha256'], 'native function changed')
    require(text.count(OLD) == 1 and fn['decompile'].count(OLD) == 1, 'printed call changed')
    require('p0 = (CScriptThing_bv *)(this + 8);' in fn['decompile'], 'embedded thing changed')
    require(any(int(call['site'], 16) == site and int(call['target'], 16) == CALLEE for call in fn['calls']), 'call target changed')

    cs = Cs(CS_ARCH_X86, CS_MODE_32)
    body = list(cs.disasm(rdata.bytes_at(begin, end - begin), begin))
    by_address = {instruction.address: instruction for instruction in body}
    require(any(i.mnemonic == 'mov' and i.op_str == 'esi, ecx' for i in body[:8]) and
            any(i.mnemonic == 'lea' and i.op_str == 'edi, [esi + 8]' for i in body[:10]), 'thing receiver path changed')
    window = [i for i in body if window_start <= i.address <= site]
    require(len(window) == 3 and [i.mnemonic for i in window] == ['push', 'mov', 'call'], 'call window shape changed')
    require(window[0].op_str == '0x40000000' and window[1].op_str == 'ecx, edi' and
            window[2].address == site and window[2].op_str == hex(CALLEE), 'call ABI changed')
    require(hashlib.sha256(rdata.bytes_at(window_start, site + by_address[site].size - window_start)).hexdigest() == row['windowSha256'], 'call window changed')

    helper = rdata.bytes_at(CALLEE, CALLEE_END - CALLEE)
    require(hashlib.sha256(helper).hexdigest() == row['helperSha256'], 'fade helper changed')
    instructions = list(cs.disasm(helper, CALLEE))
    require(instructions[-1].address == 0x00CBE811 and instructions[-1].mnemonic == 'ret' and instructions[-1].op_str == '4', 'fade helper stack cleanup changed')
    require(any(i.mnemonic == 'call' and i.op_str == 'dword ptr [eax + 0x12c]' for i in instructions) and
            any(i.mnemonic == 'call' and i.op_str == 'dword ptr [eax + 0x7f8]' for i in instructions) and
            any(i.mnemonic == 'call' and i.op_str == 'dword ptr [eax + 0x1c]' for i in instructions), 'fade helper behavior changed')
    require(rdata.bytes_at(0x0124E640, 4) == bytes.fromhex('00007041') and
            rdata.bytes_at(0x0122DED8, 4) == bytes.fromhex('0000803f'), 'fade helper constants changed')
    return text.replace(OLD, NEW)
