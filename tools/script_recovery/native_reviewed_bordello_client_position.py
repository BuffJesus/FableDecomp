"""Recover BordelloClient's second controlled resource position lookup."""
import hashlib

from capstone import Cs, CS_ARCH_X86, CS_MODE_32
from capstone.x86 import X86_OP_MEM, X86_REG_ECX, X86_REG_ESP


OLD_CHECK = 'if xStack_e4 == nil then'
NEW_CHECK = 'if not (xStack_118 ~= nil and not resources:ScriptThing(xStack_118):IsNull()) then'
OLD_VALUE = ('-- TODO(native): puVar8 = (**(*xStack_e4 + 0x18))()\n'
             '                                puVar8 = nil --[[unresolved native value]]')
NEW_VALUE = 'puVar8 = resources:ScriptThing(xStack_118):GetPos()'


def restore(source, todo, fn, row, rdata):
    if row is None:
        return source, todo

    def require(ok, why):
        if not ok:
            raise ValueError(f'reviewed BordelloClient position {fn["address"]}: {why}')

    start = int(fn['address'], 16)
    size = int(fn['bodyEndExclusive'], 16) - start
    raw = rdata.bytes_at(start, size)
    require(raw is not None and hashlib.sha256(raw).hexdigest() == row['nativeSha256'],
            'native body changed')
    require(hashlib.sha256(fn['decompile'].encode()).hexdigest() == row['sourceSha256'],
            'exported source changed')
    decoder = Cs(CS_ARCH_X86, CS_MODE_32)
    decoder.detail = True
    instructions = list(decoder.disasm(raw, start))
    require(instructions and instructions[-1].address + instructions[-1].size == start + size,
            'native disassembly incomplete')
    indexed = {ins.address: ins for ins in instructions}
    for address, encoded in row['nativeInstructions'].items():
        ins = indexed.get(int(address, 16))
        require(ins is not None and ins.bytes.hex() == encoded,
                f'native instruction changed at {address}')
    for site in (0xE45074, 0xE45724):
        load = indexed[site]
        require(load.mnemonic == 'mov' and load.operands[1].type == X86_OP_MEM
                and load.operands[1].mem.base == X86_REG_ESP
                and load.operands[1].mem.disp == 0x2C
                and load.operands[0].reg == X86_REG_ECX,
                f'controlled thing pointer changed at {site:x}')
    for site in ('0x00E45085', '0x00E45735'):
        calls = [call for call in fn['indirectCalls'] if call['site'].lower() == site.lower()]
        require(len(calls) == 1 and calls[0]['slot'] == '0x18',
                f'position vtable call changed at {site}')
    require(source.count(OLD_CHECK) == 1 and source.count(OLD_VALUE) == 1
            and source.count('xStack_118 = resources:NewResource()') == 1
            and source.count('resources:ScriptThing(xStack_118):GetPos()') >= 1
            and todo.count(row['diagnostic']) == 1,
            'lifted position lookup changed')
    todo.remove(row['diagnostic'])
    return source.replace(OLD_CHECK, NEW_CHECK, 1).replace(OLD_VALUE, NEW_VALUE, 1), todo
