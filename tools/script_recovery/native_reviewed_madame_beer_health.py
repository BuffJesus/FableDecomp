"""Restore Madame's native health gate before the returned-beer speech."""
import hashlib
import struct

from capstone import Cs, CS_ARCH_X86, CS_MODE_32
from capstone.x86 import X86_OP_IMM, X86_OP_MEM, X86_REG_ESP


BEFORE = '''            CVar2 = xStack_164
            if fVar14 <= 0.0 then
                -- TODO(native): xStack_164 = CVar2 & 0xffffff;
            end
            if 1 ~= 0 then'''
AFTER = '''            CVar2 = xStack_164
            xStack_124_b3 = true
            if not (fVar14 > 0.0) then
                xStack_124_b3 = false
            end
            if xStack_124_b3 then'''
DIAGNOSTIC = 'unlifted: xStack_164 = CVar2 & 0xffffff;'


def restore(source, todo, fn, row, rdata):
    if row is None:
        return source, todo

    def require(ok, why):
        if not ok:
            raise ValueError(f'reviewed Madame beer health {fn["address"]}: {why}')

    start = int(fn['address'], 16)
    size = int(fn['bodyEndExclusive'], 16) - start
    raw = rdata.bytes_at(start, size)
    require(raw is not None and hashlib.sha256(raw).hexdigest() == row['nativeSha256'],
            'native body changed')
    require(hashlib.sha256(fn['decompile'].encode()).hexdigest() == row['sourceSha256'],
            'exported source changed')
    zero = rdata.bytes_at(int(row['zeroAddress'], 16), 4)
    require(zero is not None and struct.unpack('<f', zero)[0] == 0.0,
            'health threshold changed')
    decoder = Cs(CS_ARCH_X86, CS_MODE_32)
    decoder.detail = True
    instructions = list(decoder.disasm(raw, start))
    require(instructions and instructions[-1].address + instructions[-1].size == start + size,
            'native disassembly incomplete')
    indexed = {ins.address: ins for ins in instructions}
    for site, encoded in row['nativeInstructions'].items():
        ins = indexed.get(int(site, 16))
        require(ins is not None and ins.bytes.hex() == encoded,
                f'native instruction changed at {site}')
    health = next((c for c in fn['indirectCalls'] if c['site'].lower() == '0x00e3bfc1'), None)
    speech = next((c for c in fn['calls'] if c['site'].lower() == '0x00e3c00f'), None)
    require(health is not None and health['slot'] == '0x420'
            and speech is not None and speech['target'].lower() == '0x007e7390',
            'native health or speech call changed')
    for site, value in ((0xE3BFCD, 1), (0xE3BFD9, 0)):
        ins = indexed[site]
        require(ins.mnemonic == 'mov' and ins.operands[0].type == X86_OP_MEM
                and ins.operands[0].mem.base == X86_REG_ESP
                and ins.operands[0].mem.disp == 0x13
                and ins.operands[1].type == X86_OP_IMM
                and ins.operands[1].imm == value,
                f'health flag store changed at {site:08x}')
    branch = indexed[0xE3BFF0]
    require(branch.mnemonic == 'je' and branch.operands[0].type == X86_OP_IMM
            and branch.operands[0].imm == 0xE3C054,
            'health-gated speech branch changed')
    anchor = '::LAB_00e3bf28::'
    end_anchor = 'quest:GiveHeroObject("OBJECT_BEER_TANKARD", -1, true)'
    first = source.find(anchor)
    last = source.find(end_anchor, first)
    require(first >= 0 and last > first and source.count(anchor) == 1,
            'beer response anchors changed')
    segment = source[first:last]
    require(segment.count(BEFORE) == 1 and todo.count(DIAGNOSTIC) == 1,
            'lifted health gate changed')
    segment = segment.replace(BEFORE, AFTER, 1)
    source = source[:first] + segment + source[last:]
    todo = [entry for entry in todo if entry != DIAGNOSTIC]
    return source, todo
