"""Prove exhaustive targets of contiguous unsigned CMP/JA/JMP switch guards."""
from capstone.x86 import X86_OP_IMM,X86_OP_MEM,X86_OP_REG


def resolve(instructions,data):
    instructions=tuple(instructions);positions={i.address for i in instructions}
    branches={};incoming=set();guarded=set()
    for index,ins in enumerate(instructions):
        if not ins.mnemonic.startswith(('j','loop')):continue
        if ins.operands and ins.operands[0].type==X86_OP_IMM:
            incoming.add(ins.operands[0].imm);continue
        if index<2 or ins.mnemonic!='jmp' or not ins.operands:
            raise ValueError('Unsupported indirect branch')
        compare,above=instructions[index-2:index];op=ins.operands[0]
        if (op.type!=X86_OP_MEM or op.mem.base or op.mem.segment or not op.mem.index or op.mem.scale!=4
                or compare.mnemonic!='cmp' or len(compare.operands)!=2
                or compare.operands[0].type!=X86_OP_REG or compare.operands[0].size!=4
                or compare.operands[0].reg!=op.mem.index or compare.operands[1].type!=X86_OP_IMM
                or not 0<=compare.operands[1].imm<1024 or above.mnemonic!='ja'
                or above.operands[0].type!=X86_OP_IMM
                or compare.address+compare.size!=above.address or above.address+above.size!=ins.address):
            raise ValueError('Switch selector is not bounded')
        size=4*(compare.operands[1].imm+1)
        if any(i.address<op.mem.disp+size and op.mem.disp<i.address+i.size for i in instructions):
            raise ValueError('Switch table overlaps code')
        raw=data.bytes_at(op.mem.disp,size)
        if raw is None or len(raw)!=size:raise ValueError('Switch table unavailable')
        targets=tuple(int.from_bytes(raw[i:i+4],'little') for i in range(0,size,4))
        if any(target not in positions for target in targets):raise ValueError('Unaligned switch target')
        branches[ins.address]=targets;incoming.update(targets);guarded.update((above.address,ins.address))
    if incoming & guarded:raise ValueError('Switch edge bypasses selector guard')
    if any(target not in positions for target in incoming):raise ValueError('Unaligned branch target')
    return branches
