"""Recover BordelloLady's eleven health receivers from the retail entity wrapper."""
import hashlib

from capstone import Cs, CS_ARCH_X86, CS_MODE_32
from capstone.x86 import X86_OP_IMM, X86_OP_MEM, X86_OP_REG, X86_REG_EAX, X86_REG_ECX, X86_REG_ESP


OLD = 'quest:GetHealth(nil --[[missing]])'
NEW = 'quest:GetHealth(me)'


def restore(source, todo, fn, row, rdata):
    if row is None:
        return source, todo

    def require(ok, why):
        if not ok:
            raise ValueError(f'reviewed BordelloLady health {fn["address"]}: {why}')

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
    indexed = {ins.address: i for i, ins in enumerate(instructions)}
    sites = [int(site, 16) for site in row['nativeCalls']]
    actual = [int(call['site'], 16) for call in fn['indirectCalls'] if call['slot'] == '0x420']
    require(actual == sites and len(sites) == 11, 'health call inventory changed')
    # The same stack wrapper is used as the entity receiver of ClearCommands at
    # E3EDDB, and every health call gets its CScriptThing from that wrapper.
    clear = [ins for ins in instructions if ins.address == 0xE3EDDB]
    require(len(clear) == 1 and clear[0].mnemonic == 'lea'
            and clear[0].operands[0].reg == X86_REG_ECX
            and clear[0].operands[1].mem.base == X86_REG_ESP
            and clear[0].operands[1].mem.disp == 0x18,
            'entity wrapper provenance changed')
    clear_call = instructions[indexed[0xE3EDDB] + 1]
    require(clear_call.mnemonic == 'call'
            and clear_call.operands[0].type == X86_OP_IMM
            and clear_call.operands[0].imm == 0x7E7360,
            'entity ClearCommands call changed')
    for site in sites:
        i = indexed.get(site)
        require(i is not None and i >= 4, f'health call {site:x} missing')
        call = instructions[i]
        helper = instructions[i-3]
        wrapper = instructions[i-4]
        arg = instructions[i-1]
        require(call.mnemonic == 'call' and call.operands[0].type == X86_OP_MEM
                and call.operands[0].mem.disp == 0x420
                and helper.mnemonic == 'call' and helper.operands[0].type == X86_OP_IMM
                and helper.operands[0].imm == 0x7E7490
                and wrapper.mnemonic == 'lea' and wrapper.operands[0].reg == X86_REG_ECX
                and wrapper.operands[1].type == X86_OP_MEM
                and wrapper.operands[1].mem.base == X86_REG_ESP
                and wrapper.operands[1].mem.disp == 0x1C
                and arg.mnemonic == 'push' and arg.operands[0].type == X86_OP_REG
                and arg.operands[0].reg == X86_REG_EAX,
                f'health argument provenance changed at {site:x}')
    count = len(sites)
    diagnostic = row['diagnostic']
    require(source.count(OLD) == count and todo.count(diagnostic) == count,
            'lifted health calls changed')
    for _ in sites:
        todo.remove(diagnostic)
    return source.replace(OLD, NEW), todo
