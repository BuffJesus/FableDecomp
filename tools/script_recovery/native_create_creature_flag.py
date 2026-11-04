"""Recover the staged immediate final CreateCreature bool across zero-argument GetPos."""
import re
import hashlib

def restore(text, fn, entries, rdata):
    if not entries or not fn.get('bodyEndExclusive'):return text
    entries=[entry for entry in entries if entry[5] and entry[3].get('slot')=='0x16c' and len(entry[2])==6 and re.fullmatch(r'SUB41\([A-Za-z_]\w*,\s*0\)',entry[2][-1].strip())]
    if not entries:return text
    import capstone
    from capstone import x86
    from tools.script_recovery.native_receiver_reaching import _cfg
    for address,size,digest in ((0x4aa980,18,'f5c90c753c25a2a2cac25d36b81d4dab11a2555b125fed4241097a24392f47e5'), (0x8a9100,532,'091e18be87c040fbdfb766929074f2163ef5fa7a5d0500985c5ecd0fc08f781d')):
        if hashlib.sha256(rdata.bytes_at(address,size) or b'').hexdigest()!=digest:return text
    start,end=int(fn['address'],16),int(fn['bodyEndExclusive'],16)
    raw=rdata.bytes_at(start,end-start)
    md=capstone.Cs(capstone.CS_ARCH_X86,capstone.CS_MODE_32);md.detail=True
    ins=list(md.disasm(raw or b'',start))
    if not ins or ins[-1].address+ins[-1].size!=end:return text
    preds=_cfg(ins,x86,capstone,rdata)
    if preds is None:return text
    index={op.address:k for k,op in enumerate(ins)};edits=[]
    for a,e,args,site,key,vtable in entries:
        if not vtable or site.get('slot')!='0x16c' or len(args)!=6:continue
        if args[0].strip()!='*(void **)(this + 0x40)':continue
        if not re.fullmatch(r'SUB41\([A-Za-z_]\w*,\s*0\)',args[-1].strip()):continue
        k=index.get(int(site['site'],16))
        if k is None:continue
        call=ins[k]
        if call.mnemonic!='call' or len(call.operands)!=1 or call.operands[0].type!=x86.X86_OP_MEM or call.operands[0].mem.disp!=0x16c:continue
        pushes=[];getpos=0
        for j in range(k-1,max(-1,k-32),-1):
            op=ins[j]
            if preds[j+1]!=[j] or op.group(capstone.CS_GRP_JUMP) or op.mnemonic in ('ret','pop','enter','leave'):break
            if op.mnemonic=='call':
                if getpos or len(pushes)!=3 or len(op.operands)!=1 or op.operands[0].type!=x86.X86_OP_MEM or op.operands[0].mem.disp!=0x18:break
                # Native CScriptThing GetPos forwards without popping arguments.
                following=ins[j+1]
                if following.mnemonic!='push' or following.operands[0].type!=x86.X86_OP_REG or following.operands[0].reg!=x86.X86_REG_EAX:break
                getpos+=1
            elif op.mnemonic=='push':pushes.append(j)
            elif x86.X86_REG_ESP in op.regs_access()[1]:break
            if len(pushes)==5:break
        if len(pushes)!=5 or getpos!=1:continue
        final=ins[pushes[-1]]
        if len(final.operands)!=1 or final.operands[0].type!=x86.X86_OP_IMM or final.operands[0].imm not in (0,1):continue
        updated=list(args);updated[-1]='true' if final.operands[0].imm else 'false'
        edits.append((a,e,','.join(updated)))
    for a,e,replacement in sorted(edits,reverse=True):text=text[:a]+replacement+text[e:]
    return text
