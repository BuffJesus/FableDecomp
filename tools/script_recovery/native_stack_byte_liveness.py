"""Conservative native scratch-byte liveness under explicit helper contracts.

This does not infer helper side effects from a name or stack purge. Unknown calls,
memory bases, pointer escapes and caller-visible returns reject the proof.
"""
from collections import deque
from capstone import Cs,CS_ARCH_X86,CS_MODE_32,CS_AC_READ,CS_AC_WRITE
from capstone import x86 as x


def prove(raw,start,store_site,helpers,*,nonstack_registers=()):
    def require(ok,why):
        if not ok:raise ValueError('native scratch-byte liveness: '+why)
    decoder=Cs(CS_ARCH_X86,CS_MODE_32);decoder.detail=True
    instructions=list(decoder.disasm(raw,start));by={i.address:i for i in instructions}
    require(instructions and sum(i.size for i in instructions)==len(raw),'incomplete native extent')
    marker=by.get(store_site)
    require(marker is not None and marker.mnemonic=='mov' and len(marker.operands)==2,'missing byte store')
    destination,value=marker.operands
    require(destination.type==x.X86_OP_MEM and destination.size==1 and
        destination.mem.base==x.X86_REG_ESP and not destination.mem.index and
        not destination.mem.segment and value.type==x.X86_OP_REG and value.size==1,'unsupported byte store')
    slot=destination.mem.disp;limit=slot+4
    require(slot>=0,'unsupported local-slot origin')
    def overlaps(offset,size):return offset<limit and slot<offset+size
    pending=deque([(store_site+marker.size,0,(),frozenset(nonstack_registers))])
    seen=set();kills=[];returns=[];calls=[]
    while pending:
        site,delta,aliases_tuple,nonstack=pending.popleft()
        state=(site,delta,aliases_tuple,nonstack)
        if state in seen:continue
        seen.add(state);require(len(seen)<=10000 and abs(delta)<=4096,'unbounded state')
        i=by.get(site);require(i is not None,'path leaves native extent')
        aliases=dict(aliases_tuple);known=set(nonstack)
        ops=i.operands;next_site=site+i.size;terminal=False
        def address(op):
            require(op.type==x.X86_OP_MEM and not op.mem.index and not op.mem.segment,'unknown memory form')
            if op.mem.base==x.X86_REG_ESP:return delta+op.mem.disp
            if op.mem.base in aliases:return aliases[op.mem.base]+op.mem.disp
            raise ValueError('native scratch-byte liveness: unknown memory base')
        if i.mnemonic=='lea':
            require(len(ops)==2 and ops[0].type==x.X86_OP_REG,'invalid LEA')
            pointer=address(ops[1]);reg=ops[0].reg
            aliases[reg]=pointer;known.discard(reg)
        elif i.mnemonic=='mov':
            require(len(ops)==2,'invalid MOV')
            dst,src=ops
            if src.type==x.X86_OP_MEM:
                require(not overlaps(address(src),src.size),'read before full overwrite')
            if dst.type==x.X86_OP_MEM:
                require(src.type!=x.X86_OP_REG or src.reg not in aliases,'stack pointer stored/escaped')
                offset=address(dst)
                if overlaps(offset,dst.size):
                    require(offset==slot and dst.size==4,'partial overwrite does not kill spill')
                    kills.append(dict(site=hex(site),kind='full-store'));terminal=True
            elif dst.type==x.X86_OP_REG:
                require(dst.size==4,'unsupported partial register assignment')
                require(dst.reg!=x.X86_REG_ESP,'unknown ESP assignment')
                aliases.pop(dst.reg,None);known.discard(dst.reg)
                if src.type==x.X86_OP_REG:
                    if src.reg in dict(aliases_tuple):aliases[dst.reg]=dict(aliases_tuple)[src.reg]
                    elif src.reg in nonstack:known.add(dst.reg)
        elif i.mnemonic=='push':
            require(len(ops)==1,'invalid PUSH')
            if ops[0].type==x.X86_OP_MEM:
                require(not overlaps(address(ops[0]),ops[0].size),'stack argument reads spill')
            if ops[0].type==x.X86_OP_REG:
                require(ops[0].reg!=x.X86_REG_ESP and ops[0].reg not in aliases,'stack pointer argument escapes')
            delta-=4;require(not overlaps(delta,4),'PUSH aliases spill')
        elif i.mnemonic=='pop':
            require(not overlaps(delta,4),'POP reads spill')
            require(ops[0].type==x.X86_OP_REG and ops[0].reg!=x.X86_REG_ESP,'unsupported POP')
            aliases.pop(ops[0].reg,None);known.discard(ops[0].reg);delta+=4
        elif i.mnemonic=='add' and ops[0].type==x.X86_OP_REG and ops[0].reg==x.X86_REG_ESP:
            require(ops[1].type==x.X86_OP_IMM and ops[1].imm>=0,'unknown stack adjustment')
            delta+=ops[1].imm
        elif i.mnemonic=='call':
            require(ops[0].type==x.X86_OP_IMM,'indirect call')
            target=ops[0].imm;contract=helpers.get(target)
            require(contract is not None,'unknown helper contract')
            kind=contract['kind'];receiver=aliases.get(x.X86_REG_ECX)
            if kind=='constructor-overwrite':
                require(receiver==slot and contract['width']==4,'constructor targets different slot')
                kills.append(dict(site=hex(site),kind='constructor-overwrite',helper=hex(target)));terminal=True
            else:
                require(contract['callerStackAccess']=='none' and contract['stackArguments']==0,'helper may read caller stack')
                if kind=='receiver-object':
                    require(receiver is not None and not overlaps(receiver,contract['receiverBytes']),
                            'receiver object aliases spill')
                else:
                    require(kind=='nonstack-receiver' and x.X86_REG_ECX in known,
                            'helper receives unknown/stack pointer')
                calls.append(dict(site=hex(site),helper=hex(target),kind=kind))
                for reg in [x.X86_REG_EAX,x.X86_REG_ECX,x.X86_REG_EDX]:aliases.pop(reg,None);known.discard(reg)
        elif i.mnemonic=='ret':
            purge=ops[0].imm if ops else 0
            require(not aliases,'stack pointer escapes through return registers')
            require(not overlaps(delta,4) and delta+4+purge>=limit,'return preserves caller-visible spill')
            returns.append(dict(site=hex(site),stackDelta=delta,discardedThrough=delta+4+purge));terminal=True
        elif i.group(x.X86_GRP_JUMP):
            require(ops[0].type==x.X86_OP_IMM,'unknown jump target')
            pending.append((ops[0].imm,delta,tuple(sorted(aliases.items())),frozenset(known)))
            if i.mnemonic=='jmp':terminal=True
        elif i.mnemonic in ('test','cmp','nop'):
            for op in ops:
                if op.type==x.X86_OP_MEM:require(not overlaps(address(op),op.size),'predicate reads spill')
        else:require(False,'unsupported instruction '+i.mnemonic)
        if not terminal:pending.append((next_site,delta,tuple(sorted(aliases.items())),frozenset(known)))
    require(kills or returns,'no terminating proof boundary')
    return dict(store=hex(store_site),slotOffset=slot,width=4,visitedStates=len(seen),
        overwriteBoundaries=kills,returnBoundaries=returns,contractedCalls=calls,
        scope='Native reachable CFG after byte store, under explicit helper access contracts; unknown operations reject.')
