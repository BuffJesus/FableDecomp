"""Recover entity self addresses passed as vtable operands from native reaching definitions."""
import re

def restore(text,fn,entries,rdata):
    if not entries or not fn.get('bodyEndExclusive'):return text
    import capstone
    from capstone import x86
    from tools.script_recovery.native_receiver_reaching import _cfg,_reaching
    from tools.script_recovery.convert_quest_unit import _receiver_printed
    start,end=int(fn['address'],16),int(fn['bodyEndExclusive'],16)
    raw=rdata.bytes_at(start,end-start)
    if not raw:return text
    md=capstone.Cs(capstone.CS_ARCH_X86,capstone.CS_MODE_32);md.detail=True
    ins=list(md.disasm(raw,start))
    saved=(x86.X86_REG_ESI,x86.X86_REG_EDI,x86.X86_REG_EBX,x86.X86_REG_EBP)
    owner=next(((i.operands[0].reg,k) for k,i in enumerate(ins[:12]) if i.mnemonic=='mov' and len(i.operands)==2 and i.operands[0].type==x86.X86_OP_REG and i.operands[0].reg in saved and i.operands[1].type==x86.X86_OP_REG and i.operands[1].reg==x86.X86_REG_ECX),None)
    if owner is None:return text
    reg,defined=owner
    if any(reg in i.regs_access()[1] and i.mnemonic not in ('push','pop') for k,i in enumerate(ins) if k!=defined):return text
    preds=_cfg(ins,x86,capstone,rdata)
    if preds is None:return text
    indexed={i.address:k for k,i in enumerate(ins)};edits=[]
    for a,e,args,site,key,vtable in entries:
        if not vtable or site.get('ecxValue')!=4 or len(site.get('pushedStack',[]))!=len(args):continue
        if _receiver_printed(text,a,args):continue
        k=indexed.get(int(site['site'],16))
        if k is None:continue
        call=ins[k]
        if call.mnemonic!='call' or len(call.operands)!=1 or call.operands[0].type!=x86.X86_OP_MEM or call.operands[0].mem.disp!=int(site['slot'],16):continue
        # Each operand belongs to the adjacent call. Never cross another call,
        # a branch, a stack allocation or a block with alternate predecessors.
        pushes=[]
        for j in range(k-1,max(-1,k-40),-1):
            op=ins[j]
            if preds[j+1]!=[j] or op.mnemonic in ('call','ret','pop','enter','leave') or op.group(capstone.CS_GRP_JUMP):break
            if op.mnemonic=='push':pushes.append(j)
            elif x86.X86_REG_ESP in op.regs_access()[1]:break
            if len(pushes)==len(args):break
        if len(pushes)!=len(args):continue
        out=list(args)
        for n,j in enumerate(pushes):
            push=ins[j]
            if not re.fullmatch(r'[A-Za-z_]\w*',args[n].strip()) or list(reversed(site['pushedStack']))[n]!=8:continue
            if len(push.operands)!=1 or push.operands[0].type!=x86.X86_OP_REG or push.operands[0].reg not in saved:continue
            defs=_reaching(ins,preds,j,push.operands[0].reg,x86)
            if not defs or -1 in defs:continue
            def self_address(d):
                op=ins[d]
                return op.mnemonic=='lea' and len(op.operands)==2 and op.operands[1].type==x86.X86_OP_MEM and op.operands[1].mem.base==reg and not op.operands[1].mem.index and op.operands[1].mem.disp==8
            if all(self_address(d) for d in defs):out[n]='(CScriptThing *)((int)this + 8)'
        if out!=args:edits.append((a,e,','.join(out)))
    for a,e,new in sorted(edits,reverse=True):text=text[:a]+new+text[e:]
    return text
