"""Bounded normal-return operands; opaque calls require a separate ABI contract."""
from collections import deque
import capstone as cs
from capstone import x86 as x

def inspect(image,entry,regions,*,max_instructions=4096):
    """Regions must be explicit, nonoverlapping (start, end) native byte bounds."""
    if type(max_instructions)is not int or not 1<=max_instructions<=65536:
        raise ValueError('invalid instruction budget')
    bounds=sorted(regions)
    if not bounds or any(a>=b for a,b in bounds) or any(b>c for (_,b),(c,_) in zip(bounds,bounds[1:])):
        raise ValueError('invalid or overlapping regions')
    decoder=cs.Cs(cs.CS_ARCH_X86,cs.CS_MODE_32);decoder.detail=True
    pending=deque([entry]);instructions={};owners={};edges={};returns={};calls=[]
    while pending:
        address=pending.popleft()
        if address in instructions:continue
        region=next(((a,b) for a,b in bounds if a<=address<b),None)
        if region is None:raise ValueError('edge outside reviewed regions')
        data=image.bytes_at(address,min(15,region[1]-address))
        decoded=[] if data is None else list(decoder.disasm(data,address,count=1))
        if len(decoded)!=1:raise ValueError('truncated instruction')
        i=decoded[0]
        if i.mnemonic in ('int3','ud2','hlt') or i.group(cs.CS_GRP_INT) or i.group(cs.CS_GRP_IRET):raise ValueError('abnormal transfer')
        if any(a in owners and owners[a]!=address for a in range(address,address+i.size)):raise ValueError('overlapping instruction entry')
        for a in range(address,address+i.size):owners[a]=address
        instructions[address]=dict(site=hex(address),bytes=i.bytes.hex(),assembly=i.mnemonic+' '+i.op_str)
        successors=[]
        if i.group(cs.CS_GRP_RET):
            if i.mnemonic!='ret' or len(i.operands)>1:raise ValueError('unsupported return')
            returns[address]=i.operands[0].imm if i.operands else 0
        else:
            if i.group(cs.CS_GRP_CALL):
                if i.mnemonic!='call':raise ValueError('unsupported call')
                calls.append(dict(site=hex(address),assembly=i.mnemonic+' '+i.op_str,opaque=True))
            if i.group(cs.CS_GRP_JUMP):
                if i.operands[0].type!=x.X86_OP_IMM:raise ValueError('indirect jump')
                successors.append(i.operands[0].imm)
            if i.mnemonic!='jmp':successors.append(address+i.size)
        edges[address]=successors;pending.extend(successors)
        if len(instructions)>max_instructions:raise ValueError('instruction bound exceeded')
    # Each reachable instruction must have some normal path to a RET. This does
    # not prove termination, branch feasibility, child balance or heap safety.
    reverse={a:[] for a in instructions}
    for a,targets in edges.items():
        for target in targets:reverse[target].append(a)
    can_return=set(returns);pending=deque(returns)
    while pending:
        for parent in reverse[pending.popleft()]:
            if parent not in can_return:can_return.add(parent);pending.append(parent)
    if can_return!=set(instructions):raise ValueError('reachable region without normal return path')
    purges=set(returns.values())
    if len(purges)!=1:raise ValueError('nonuniform return operands')
    return dict(status='normal_return_operand_contract_only',entry=hex(entry),purgeBytes=next(iter(purges)),
        instructions=[instructions[a] for a in sorted(instructions)],returns={hex(a):v for a,v in sorted(returns.items())},opaqueCalls=calls,
        limits='Explicit bounded direct CFG and uniform RET operands only. Calls are opaque normal-return edges; child stack balance, target identity, exceptions, reentry, object lifetime and caller memory preservation remain separate contracts.')
