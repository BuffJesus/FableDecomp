"""Recover native quest-card operands split around a hidden string return.

Only the witnessed constructor/getter/card ABI bundle is accepted. The getter
purges its out pointer while the earlier boolean remains on the outer call stack.
Source call correspondence and matching literal storage are required as well.
"""
import json,re
from capstone import Cs,CS_ARCH_X86,CS_MODE_32,CS_GRP_JUMP
from capstone import x86 as x

GSI_VTABLE=0x1260F0C

def returns(rdata,address):
    raw=rdata.bytes_at(address,0x400)
    if raw is None:return set()
    result=set()
    for ins in Cs(CS_ARCH_X86,CS_MODE_32).disasm(raw,address):
        if ins.mnemonic=='int3':break
        if ins.mnemonic=='ret':result.add(int(ins.op_str,0) if ins.op_str else 0)
    return result

def lea_stack(ins):
    if ins.mnemonic!='lea' or len(ins.operands)!=2:return None
    dst,src=ins.operands
    if dst.type!=x.X86_OP_REG or src.type!=x.X86_OP_MEM or src.mem.base!=x.X86_REG_ESP or src.mem.index:return None
    return dst.reg,src.mem.disp

def pushes(ins,reg):
    return ins.mnemonic=='push' and ins.operands[0].type==x.X86_OP_REG and ins.operands[0].reg==reg

def vcall(ins,slot):
    return ins.mnemonic=='call' and ins.operands[0].type==x.X86_OP_MEM and ins.operands[0].mem.disp==slot and not ins.operands[0].mem.index

def restore(source,fn,rdata,sites):
    if not sites or not any(c.get('slot')=='0x49c' for c in fn.get('indirectCalls',[])):return source
    for slot,target,words in ((0xa3c,0x891880,4),(0x49c,0x8968c0,12)):
        if rdata.bytes_at(GSI_VTABLE+slot,4)!=target.to_bytes(4,'little') or returns(rdata,target)!={words}:return source
    if returns(rdata,0x99ebf0)!={8}:return source
    start=int(fn['address'],16);end=int(fn['bodyEndExclusive'],16)
    raw=rdata.bytes_at(start,end-start)
    if raw is None:return source
    md=Cs(CS_ARCH_X86,CS_MODE_32);md.detail=True
    ops=list(md.disasm(raw,start));indexed={i.address:k for k,i in enumerate(ops)}
    printed={int(site['site'],16):(a,e,args,key,vt) for a,e,args,site,key,vt in sites}
    changes=[]
    for site in fn['indirectCalls']:
        if site.get('slot')!='0x49c':continue
        address=int(site['site'],16);n=indexed.get(address)
        if n is None or n<8:continue
        outer=ops[n];flag,out_lea,out_push,getter,reload,result_push,card_lea,card_push=ops[n-8:n]
        out_address,card_address=lea_stack(out_lea),lea_stack(card_lea)
        if not (vcall(outer,0x49c) and vcall(getter,0xa3c) and out_address and card_address):continue
        if not (pushes(out_push,out_address[0]) and pushes(card_push,card_address[0]) and pushes(result_push,x.X86_REG_EAX)):continue
        if flag.mnemonic!='push' or flag.operands[0].type!=x.X86_OP_IMM or flag.operands[0].imm not in (0,1):continue
        if reload.mnemonic!='mov' or reload.operands[0].type!=x.X86_OP_REG or reload.operands[0].reg!=x.X86_REG_ECX:continue
        # Getter consumes only its out pointer; the earlier bool remains pending.
        # At the two LEAs, pending stack words differ by one.
        if abs((out_address[1]-4)-(card_address[1]-8))<16:continue
        if any(i.group(CS_GRP_JUMP) and i.operands and i.operands[0].type==x.X86_OP_IMM and flag.address<i.operands[0].imm<=outer.address for i in ops):continue
        j=next((k for k in range(n-9,-1,-1) if ops[k].mnemonic=='call'),None)
        if j is None or j<3 or n-8-j>8:continue
        ctor=ops[j]
        # Prove both virtual receivers come from this same quest interface.
        preamble=ops[j+1:n-8]
        if b''.join(i.bytes for i in preamble)!=bytes.fromhex('8b4d048b018bd18b32'):continue
        if reload.bytes!=bytes.fromhex('8b4d04'):continue
        if getter.operands[0].mem.base!=x.X86_REG_EAX or outer.operands[0].mem.base!=x.X86_REG_ESI:continue
        if ctor.operands[0].type!=x.X86_OP_IMM or ctor.operands[0].imm!=0x99ebf0:continue
        if any(i.mnemonic!='mov' or i.operands[0].type!=x.X86_OP_REG or i.operands[0].reg==x.X86_REG_ESP for i in ops[j+1:n-8]):continue
        ctor_lea=lea_stack(ops[j-1]);literal_push,length_push=ops[j-2],ops[j-3]
        if ctor_lea!=(x.X86_REG_ECX,card_address[1]):continue
        if literal_push.mnemonic!='push' or literal_push.operands[0].type!=x.X86_OP_IMM:continue
        if length_push.mnemonic!='push' or length_push.operands[0].type!=x.X86_OP_IMM or length_push.operands[0].imm!=-1:continue
        text=rdata.string_at(literal_push.operands[0].imm)
        if text is None:continue
        if not all(a in printed for a in (getter.address,ctor.address,outer.address)):continue
        ga,ge,gargs,gkey,gvirtual=printed[getter.address];ca,ce,cargs,_,cvirtual=printed[ctor.address];oa,oe,oargs,okey,ovirtual=printed[outer.address]
        if not (gvirtual and ovirtual and not cvirtual and gkey=='0xa3c' and okey=='0x49c'):continue
        if len(gargs)!=2 or len(oargs)!=2 or len(cargs)!=3:continue
        try:
            if int(gargs[1],0)!=flag.operands[0].imm or int(cargs[2],0)!=-1 or json.loads(cargs[1])!=text:continue
        except (ValueError,TypeError):continue
        card_name=re.fullmatch(r'(?:\(CCharString(?:_bv)? \*\)\s*)?&?(\w+)',cargs[0])
        if not card_name:continue
        gs=source.rfind('\n',0,ga)+1;os=source.rfind('\n',0,oa)+1
        assignment=re.fullmatch(r'\s*(\w+) = \(\*\*\(code \*\*\)\([^;\n]* \+ 0xa3c\)\)\(',source[gs:ga])
        outer_head=re.fullmatch(r'\s*\(\*\*\(code \*\*\)\([^;\n]* \+ 0x49c\)\)\(',source[os:oa])
        if not assignment or not outer_head or source[ge+1:os].strip()!=';':continue
        result=assignment[1]
        if oargs[1]!=result or result==card_name[1] or not ce<ga<ge<oa:continue
        if re.search(r'\b'+re.escape(card_name[1])+r'\b',source[ce+1:os]):continue
        changes.extend([(ga,ge,gargs[0]),(oa,oe,cargs[0]+','+result+','+str(flag.operands[0].imm))])
    for a,b,value in sorted(changes,reverse=True):source=source[:a]+value+source[b:]
    return source
