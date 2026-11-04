"""Recover CTimer destructor IDs from exact RegisterTimer result stores."""
import re
from collections import defaultdict


def result_stack_slot(fn, site, rdata):
    from capstone import Cs, CS_ARCH_X86, CS_MODE_32
    from capstone.x86 import X86_OP_REG, X86_OP_MEM, X86_OP_IMM
    start, size = int(fn['address'], 16), int(fn.get('size') or 0)
    if not size:
        return None
    cs=Cs(CS_ARCH_X86,CS_MODE_32);cs.detail=True
    insns=list(cs.disasm(rdata.bytes_at(start,size) or b'',start))
    if (not insns or insns[0].mnemonic!='sub' or insns[0].op_str.split(',')[0]!='esp'
            or insns[0].operands[1].type!=X86_OP_IMM):
        return None
    frame=insns[0].operands[1].imm
    for ins in insns[1:8]:
        if ins.mnemonic=='push' and ins.operands[0].type==X86_OP_REG:frame+=4
        elif ins.mnemonic=='mov' and all(o.type==X86_OP_REG for o in ins.operands):continue
        else:break
    if site.get('depth')!=frame:
        return None
    address=int(site['site'],16)
    index=next((k for k,i in enumerate(insns) if i.address==address),None)
    if index is None:
        return None
    call=insns[index]
    if (call.mnemonic!='call' or len(call.operands)!=1 or call.operands[0].type!=X86_OP_MEM
            or call.operands[0].mem.disp!=0x15c):
        return None
    extra=0
    for ins in insns[index+1:index+13]:
        ops=ins.operands
        if ins.mnemonic=='mov' and len(ops)==2 and ops[0].type==X86_OP_MEM and ops[1].type==X86_OP_REG:
            mem=ops[0].mem
            if ins.reg_name(ops[1].reg)=='eax' and ins.reg_name(mem.base)=='esp' and not mem.index and ops[0].size==4:
                slot=mem.disp-frame-extra
                return slot if slot<0 else None
        if ins.mnemonic=='push':
            extra+=4
        elif ins.mnemonic in ('add','sub') and len(ops)==2 and ops[0].type==X86_OP_REG and ins.reg_name(ops[0].reg)=='esp' and ops[1].type==X86_OP_IMM:
            extra+=ops[1].imm*(1 if ins.mnemonic=='sub' else -1)
            if extra<0:return None
        elif ins.mnemonic=='call' or ins.mnemonic.startswith('j') or ins.mnemonic in ('ret','pop','leave'):
            return None
        else:
            _,writes=ins.regs_access()
            if any(ins.reg_name(reg) in ('eax','ax','al','ah','esp','sp') for reg in writes):
                return None
    return None


def recover(source,fn,ordered,receiver_slot,rdata=None):
    if not ordered or not any(not vt and site.get('target','').lower()=='0x00cd4470' for _,_,_,site,_,vt in ordered):
        return source
    if rdata is None:
        from tools.script_recovery.lift_native_lua import RData
        rdata=RData()
    candidates=defaultdict(list)
    for pos,end,args,site,key,vt in ordered:
        if not vt or site.get('slot')!='0x15c':continue
        head=source[source.rfind('\n',0,pos)+1:pos]
        match=re.match(r'\s*(\w+) = \(\*\*\(code \*\*\)\(\*DAT_0143e8f8 \+ 0x15c\)\)\($',head)
        if not match:continue
        name=match[1]
        if len(re.findall(r'^\s*'+re.escape(name)+r' = ',source,re.M))!=1:continue
        slot=result_stack_slot(fn,site,rdata)
        if slot is not None:candidates[slot].append((int(site['site'],16),name))
    edits=[]
    for pos,end,args,site,key,vt in ordered:
        if vt or site.get('target','').lower()!='0x00cd4470' or len(args)!=1:continue
        address=int(site['site'],16)
        slot=receiver_slot(fn,address,'ecx',address=True)
        choices=candidates.get(slot,[])
        if len(choices)!=1 or choices[0][0]>=address:continue
        if not re.fullmatch(r'\(CTimer \*\)&\w+',args[0].strip()):continue
        edits.append((pos,end,'(CTimer *)&'+choices[0][1]))
    for start,end,value in reversed(edits):source=source[:start]+value+source[end:]
    return source
