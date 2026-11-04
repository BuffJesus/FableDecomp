"""Conditional known-bit proof for a shared native caller cleanup-mask word."""
from collections import deque
import capstone
from capstone import x86 as x
from tools.script_recovery.native_receiver_reaching import _cfg
FULL=0xffffffff;UNKNOWN=(0,0)
parents=[x.X86_REG_EAX,x.X86_REG_EBX,x.X86_REG_ECX,x.X86_REG_EDX,x.X86_REG_ESI,x.X86_REG_EDI,x.X86_REG_EBP];registers={r:(r,0,32) for r in parents}
for parent,names in [(x.X86_REG_EAX,[(x.X86_REG_AL,0,8),(x.X86_REG_AH,8,8),(x.X86_REG_AX,0,16)]),(x.X86_REG_EBX,[(x.X86_REG_BL,0,8),(x.X86_REG_BH,8,8),(x.X86_REG_BX,0,16)]),(x.X86_REG_ECX,[(x.X86_REG_CL,0,8),(x.X86_REG_CH,8,8),(x.X86_REG_CX,0,16)]),(x.X86_REG_EDX,[(x.X86_REG_DL,0,8),(x.X86_REG_DH,8,8),(x.X86_REG_DX,0,16)]),(x.X86_REG_ESI,[(x.X86_REG_SI,0,16)]),(x.X86_REG_EDI,[(x.X86_REG_DI,0,16)]),(x.X86_REG_EBP,[(x.X86_REG_BP,0,16)])]:
    for reg,shift,width in names:registers[reg]=(parent,shift,width)
constant=lambda n:((~n)&FULL,n&FULL)
def prove(code,start,depths,image,*,mask_entry_offset,scopes):
    md=capstone.Cs(capstone.CS_ARCH_X86,capstone.CS_MODE_32);md.detail=True;ins=list(md.disasm(code,start));assert sum(i.size for i in ins)==len(code);preds=_cfg(ins,x,capstone,image);assert preds is not None;succ={k:[] for k in preds}
    for k,ps in preds.items():
        for p in ps:succ[p].append(k)
    # State has word/register known-zero/one masks,register-to-current-mask
    # projections,and flags/refinements. Every mask store invalidates projections.
    fields=['cell',*parents];initial={key:UNKNOWN for key in fields};initial.update(tags={},zero=None,sign=None,zbit=None,sbit=None)
    incoming={0:initial};queue=deque([0]);writes={};steps=0;entry_offset=mask_entry_offset
    def merge(a,b):
        out={key:(a[key][0]&b[key][0],a[key][1]&b[key][1]) for key in fields};out['tags']={k:v for k,v in a['tags'].items() if b['tags'].get(k)==v}
        for key in ['zero','sign','zbit','sbit']:out[key]=a[key] if a[key]==b[key] else None
        return out
    while queue:
        k=queue.popleft();steps+=1;assert steps<100000;i=ins[k];state=incoming[k];s=dict(state,tags=dict(state['tags']));ops=i.operands
        def coordinate(op):
            return op.mem.disp-depths[i.address] if op.type==x.X86_OP_MEM and op.mem.base==x.X86_REG_ESP and not op.mem.index else None
        def read(op):
            width=op.size*8;mask=(1<<width)-1
            if op.type==x.X86_OP_IMM:return constant(op.imm),None
            if op.type==x.X86_OP_REG and op.reg in registers:
                parent,shift,bits=registers[op.reg];z,o=s[parent];tag=s['tags'].get(parent)
                return ((z>>shift)&mask,(o>>shift)&mask),((shift,width) if tag==(0,32) else tag if shift==0 and tag==(0,width) else None)
            offset=coordinate(op)
            if offset==entry_offset and op.size in (1,4):return (s['cell'][0]&mask,s['cell'][1]&mask),(0,width)
            return UNKNOWN,None
        def write(op,value,tag=None):
            if op.type==x.X86_OP_REG and op.reg in registers:
                parent,shift,width=registers[op.reg];bits=((1<<width)-1)<<shift;z,o=s[parent];s[parent]=((z&~bits)|((value[0]<<shift)&bits),(o&~bits)|((value[1]<<shift)&bits));s['tags'].pop(parent,None)
                if shift==0 and (width==32 or width==8) and tag is not None:s['tags'][parent]=tag
            elif op.type==x.X86_OP_MEM:
                offset=coordinate(op)
                if offset is not None and offset<entry_offset+4 and entry_offset<offset+op.size:
                    assert offset==entry_offset and op.size==4,(hex(i.address),'partial caller-mask store')
                    s['cell']=value;s['tags']={};writes[i.address]=i.mnemonic+' '+i.op_str
        handled=False
        if i.mnemonic=='mov' and len(ops)==2:
            value,tag=read(ops[1]);write(ops[0],value,tag);handled=True
        elif i.mnemonic in ('or','and','xor') and len(ops)==2:
            a,_=read(ops[0]);b,_=read(ops[1])
            if i.mnemonic=='xor' and ops[0].type==ops[1].type==x.X86_OP_REG and ops[0].reg==ops[1].reg:value=constant(0)
            elif i.mnemonic=='or':value=(a[0]&b[0],a[1]|b[1])
            elif i.mnemonic=='and':value=(a[0]|b[0],a[1]&b[1])
            else:value=UNKNOWN
            write(ops[0],value);handled=True
        if not handled:
            for reg in i.regs_access()[1]:
                if reg in registers:parent,_,_=registers[reg];s[parent]=UNKNOWN;s['tags'].pop(parent,None)
        if i.mnemonic=='call':
            for reg in [x.X86_REG_EAX,x.X86_REG_ECX,x.X86_REG_EDX]:s[reg]=UNKNOWN;s['tags'].pop(reg,None)
        if i.mnemonic=='call' or x.X86_REG_EFLAGS in i.regs_access()[1]:
            s.update(zero=None,sign=None,zbit=None,sbit=None)
        if i.mnemonic=='test':
            a,tag=read(ops[0]);b,_=read(ops[1]);width=ops[0].size*8;mask=(1<<width)-1;zero=(a[0]|b[0])&mask;one=(a[1]&b[1])&mask;s['zero']=True if zero==mask else False if one else None;signbit=1<<(width-1);s['sign']=False if zero&signbit else True if one&signbit else None
            if tag is not None:
                s['sbit']=tag[0]+width-1 if ops[1].type==x.X86_OP_REG and ops[0].reg==ops[1].reg or ops[1].type==x.X86_OP_IMM and ops[1].imm&signbit else None
                if ops[1].type==x.X86_OP_IMM:
                    immediate=ops[1].imm&mask
                    if immediate and immediate.bit_count()==1:s['zbit']=tag[0]+immediate.bit_length()-1
        target=ops[0].imm if i.mnemonic in ('je','jne','jns','js') and ops[0].type==x.X86_OP_IMM else None
        for dest in succ[k]:
            out=dict(s,tags=dict(s['tags']));taken=target is not None and ins[dest].address==target
            if target is not None:
                condition=s['zero'] if i.mnemonic=='je' else None if s['zero'] is None else not s['zero'] if i.mnemonic=='jne' else None
                if i.mnemonic in ('jns','js'):condition=None if s['sign'] is None else (not s['sign'] if i.mnemonic=='jns' else s['sign'])
                if condition is not None and taken!=condition:continue
                bit=s['zbit'] if i.mnemonic in ('je','jne') else s['sbit']
                if bit is not None:
                    is_set=(not taken if i.mnemonic=='je' else taken) if i.mnemonic in ('je','jne') else (not taken if i.mnemonic=='jns' else taken);z,o=out['cell'];flag=1<<bit
                    if (is_set and z&flag) or (not is_set and o&flag):continue
                    out['cell']=(z&~flag,o|flag) if is_set else (z|flag,o&~flag)
            if dest not in incoming:incoming[dest]=out;queue.append(dest)
            else:
                value=merge(incoming[dest],out)
                if value!=incoming[dest]:incoming[dest]=value;queue.append(dest)
    facts={}
    for address,mask in scopes:
        k=next(k for k,i in enumerate(ins) if i.address==address);z,o=incoming[k]['cell'];assert z&mask==mask and not o&mask,(hex(address),hex(z),hex(o),hex(mask));facts[hex(address)]=dict(temporaryMask=hex(mask),knownZero=hex(z),knownOne=hex(o))
    exposed=[]
    for i in ins:
        if i.address not in depths or i.mnemonic!='lea' or len(i.operands)!=2:continue
        op=i.operands[1]
        if op.type==x.X86_OP_MEM and op.mem.base==x.X86_REG_ESP and not op.mem.index and entry_offset<=op.mem.disp-depths[i.address]<entry_offset+4:exposed.append(hex(i.address))
    assert not exposed,'caller mask address exposed'
    return dict(scopeEntryFacts=facts,reachableInstructions=len(incoming),worklistSteps=steps,maskEntryOffset=entry_offset,directMaskAddressExposures=exposed,maskWrites=[dict(site=hex(site),instruction=text) for site,text in sorted(writes.items())],limits='Normal native CFG/checked frame-depth anchors;children preserve caller mask and disjoint locals under explicit ABI/footprint contracts. No indirect-child write,heap,exception/reentry/gameplay proof.')
