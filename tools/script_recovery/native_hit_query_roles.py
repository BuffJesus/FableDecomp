"""Bounded caller-role proof under explicit normal CString/query ABI contracts."""
from dataclasses import dataclass
from itertools import product
from capstone import Cs,CS_ARCH_X86,CS_MODE_32,x86 as x

@dataclass(frozen=True)
class Mask:
    keep:int=0xffffffff
    set:int=0
    known_zero:int=0x70
    def bor(self,value):return Mask(self.keep,(self.set|value)&0xffffffff,self.known_zero)
    def band(self,value):return Mask(self.keep&value,self.set&value,self.known_zero)
    def bit(self,value):
        if value&~self.known_zero:raise ValueError('unknown incoming mask bit tested')
        return bool(self.set&value)  # Entry mask bits0x70 are explicitly zero.
    def is_initial(self):return self.set==0 and ((~self.keep)&~self.known_zero&0xffffffff)==0

@dataclass(frozen=True)
class Slice:
    mask:Mask
    shift:int
    width:int

def prove(raw,start,*,constructor,destructor,hero_literal,flag_offset=19,mask_offset=20,object_offsets=(60,64,32),temporary_mask=0x70,actor_profile='derived'):
    def require(ok,why):
        if not ok:raise ValueError('native packed query roles: '+why)
    md=Cs(CS_ARCH_X86,CS_MODE_32);md.detail=True;ins=list(md.disasm(raw,start))
    require(ins and sum(i.size for i in ins)==len(raw),'incomplete region')
    by={i.address:i for i in ins};end=start+len(raw);records=[]
    protected=(flag_offset-3,mask_offset+4)
    require(mask_offset==flag_offset+1,'flag/mask adjacency changed')
    for offset in object_offsets:
        require(offset>=protected[1] or offset+4<=protected[0],'temporary aliases flag/mask')
    require(len(set(object_offsets))==3,'duplicate temporary extent')
    require(all(abs(a-b)>=4 for a in object_offsets for b in object_offsets if a!=b),'overlapping temporary extents')
    require(0<temporary_mask<=0xffffffff and temporary_mask.bit_count()==3,'invalid three-temporary mask')
    require(actor_profile in ('derived','entry-ebx'),'unsupported actor profile')
    for hit,any_hit,heal in product([False,True],repeat=3):
        regs={x.X86_REG_ESI:('this',0)}
        if actor_profile=='entry-ebx':regs[x.X86_REG_EBX]=('this',8)
        delta=0;arguments={};live=[];created=[];destroyed=[]
        flag=None;physical_mask=Mask(known_zero=temporary_mask);zero=None;sign=None;site=start;steps=0;queries=[];writes=[];reads=[]
        def reg(register):
            if register==x.X86_REG_ESP:return ('stack',delta)
            if register==x.X86_REG_AL:
                value=regs.get(x.X86_REG_EAX)
                if isinstance(value,int):return value&255
                if isinstance(value,Mask):return Slice(value,0,8)
                require(isinstance(value,Slice) and value.shift==0 and value.width==8,'unknown AL');return value
            if register==x.X86_REG_AH:
                value=regs.get(x.X86_REG_EAX);require(isinstance(value,Mask),'unknown AH');return Slice(value,8,8)
            if register==x.X86_REG_BL:
                value=regs.get(x.X86_REG_EBX);require(isinstance(value,Mask),'unknown BL');return value
            return regs.get(register)
        def address(op):
            require(op.type==x.X86_OP_MEM and not op.mem.index and not op.mem.segment,'unknown memory form')
            value=reg(op.mem.base);require(isinstance(value,tuple) and value[0] in ('this','stack','vtable'),'unknown memory base')
            return value[0],value[1]+op.mem.disp
        def read(op):
            if op.type==x.X86_OP_REG:return reg(op.reg)
            if op.type==x.X86_OP_IMM:return op.imm&0xffffffff
            family,offset=address(op);reads.append((site,family,offset,op.size))
            if family=='this':require(offset==8 and op.size==4,'unexpected owner load');return ('vtable',8)
            require(family=='stack','unexpected memory read')
            if (offset,op.size)==(mask_offset,4):return physical_mask
            if (offset,op.size)==(mask_offset,1):return Slice(physical_mask,0,8)
            if (offset,op.size)==(flag_offset,1):require(flag is not None,'flag read before overwrite');return flag
            require(offset<0 and op.size==4 and offset in arguments,'unknown stack read');return arguments[offset]
        def store(offset,width,value):
            nonlocal flag,physical_mask
            writes.append((site,offset,width))
            if (offset,width)==(mask_offset,4):require(isinstance(value,Mask),'non-mask spill');physical_mask=value;return
            if (offset,width)==(flag_offset,1):require(value in (0,1),'non-boolean flag');flag=value;return
            require(offset<0 and width==4,'unexpected caller write');arguments[offset]=value
        while site!=end:
            steps+=1;require(steps<256,'unbounded path');i=by.get(site);require(i is not None,'path leaves region')
            ops=i.operands;next_site=site+i.size
            if i.mnemonic=='push':
                value=read(ops[0]);delta-=4;store(delta,4,value)
            elif i.mnemonic=='lea':
                require(ops[0].type==x.X86_OP_REG and ops[0].size==4,'unsupported LEA');regs[ops[0].reg]=address(ops[1])
            elif i.mnemonic=='mov':
                value=read(ops[1]);dst=ops[0]
                if dst.type==x.X86_OP_MEM:
                    family,offset=address(dst);require(family=='stack','nonstack caller write');store(offset,dst.size,value)
                else:
                    require(dst.type==x.X86_OP_REG,'unsupported MOV')
                    if dst.reg==x.X86_REG_AL:
                        require(isinstance(value,int) or isinstance(value,Slice) and value.shift==0 and value.width==8,'unknown partial return');regs[x.X86_REG_EAX]=value
                    else:require(dst.size==4 and dst.reg!=x.X86_REG_ESP,'partial/ESP write');regs[dst.reg]=value
            elif i.mnemonic in ('or','and'):
                require(ops[0].type==x.X86_OP_REG and ops[0].size==4 and ops[0].reg!=x.X86_REG_ESP and ops[1].type==x.X86_OP_IMM,'unsupported bit operation')
                value=reg(ops[0].reg);require(isinstance(value,Mask),'bit operation not on mask')
                immediate=ops[1].imm&0xffffffff
                regs[ops[0].reg]=value.bor(immediate) if i.mnemonic=='or' else value.band(immediate)
                zero=None;sign=None  # Full-mask arithmetic flags are outside this domain.
            elif i.mnemonic=='test':
                value=read(ops[0])
                other=read(ops[1]);width=ops[0].size*8
                if isinstance(value,(Mask,Slice)):
                    if isinstance(value,Mask):
                        require(isinstance(other,int),'unknown full-mask test');projection=Slice(value,0,width);bits=other
                    else:
                        projection=value
                        require(isinstance(other,int) or other==value,'unknown projected mask test')
                        bits=other if isinstance(other,int) else (1<<projection.width)-1
                    require(projection.width==width,'mask projection width changed')
                    bits&=(1<<width)-1
                    def known(bits):
                        try:return projection.mask.bit(bits<<projection.shift)
                        except ValueError:return None  # Arbitrary incoming bits are not guessed.
                    tested=known(bits);zero=None if tested is None else not tested
                    sign=known(bits & (1<<(width-1)))
                else:
                    require(isinstance(value,int) and isinstance(other,int),'unknown scalar test');tested=value&other;zero=tested==0;sign=bool(tested&(1<<(width-1)))
            elif i.mnemonic=='call':
                # Native CALL writes a return word below current outgoing args.
                require(delta-4+4<=protected[0],'return address aliases protected locals')
                receiver=reg(x.X86_REG_ECX);target=ops[0]
                if target.type==x.X86_OP_IMM:
                    if target.imm==constructor:
                        require(receiver in [('stack',offset) for offset in object_offsets],'constructor receiver mismatch')
                        offset=receiver[1];require(offset not in live,'double construction')
                        require(arguments.get(delta)==hero_literal and arguments.get(delta+4)==0xffffffff,'literal constructor arguments mismatch')
                        live.append(offset);created.append(offset);delta+=8;returned=None
                    else:
                        require(target.imm==destructor and receiver is not None and receiver[0]=='stack','unknown direct child')
                        offset=receiver[1];require(live and offset==live[-1],'cleanup order/receiver mismatch')
                        live.pop();destroyed.append(offset);returned=None
                else:
                    require(target.type==x.X86_OP_MEM and address(target)[0]=='vtable' and receiver==('this',8),'unproved query receiver/table')
                    # The vtable marker stores owner offset8; member displacement
                    # is read directly from the CALL operand, not that marker.
                    slot=target.mem.disp
                    require(slot in (0x54,0xa8,0xa4),'unknown query slot')
                    ordinal={0x54:0,0xa8:1,0xa4:2}[slot];offset=object_offsets[ordinal]
                    require(offset in live,'query uses inactive temporary')
                    purge=8 if slot==0xa4 else 4
                    require(arguments.get(delta+(4 if slot==0xa4 else 0))==('stack',offset),'query name pointer mismatch')
                    if slot==0xa4:require(arguments.get(delta)==14,'excluded ability mismatch')
                    queries.append(slot);returned=int((hit,any_hit,heal)[ordinal]);delta+=purge
                for register in [x.X86_REG_EAX,x.X86_REG_ECX,x.X86_REG_EDX]:regs.pop(register,None)
                if returned is not None:regs[x.X86_REG_EAX]=returned
                zero=None;sign=None  # Child EFLAGS are caller-clobbered.
            elif i.mnemonic in ('jmp','je','jne','jns'):
                require(ops[0].type==x.X86_OP_IMM,'unknown branch')
                if i.mnemonic=='jmp':next_site=ops[0].imm
                elif i.mnemonic=='jns':
                    require(sign is not None,'unknown signed branch predicate')
                    if not sign:next_site=ops[0].imm
                else:
                    require(zero is not None,'unknown branch predicate')
                    if (i.mnemonic=='je' and zero) or (i.mnemonic=='jne' and not zero):next_site=ops[0].imm
            else:require(False,'unsupported instruction '+i.mnemonic)
            site=next_site
        expected_queries=[0x54]+([] if hit else [0xa8]+([0xa4] if any_hit else []))
        expected_created=[object_offsets[0]]+([] if hit else [object_offsets[1]]+([object_offsets[2]] if any_hit else []))
        require(queries==expected_queries and created==expected_created,'query/lifetime topology mismatch')
        require(not live and destroyed==list(reversed(created)),'unbalanced temporary cleanup')
        require(delta==0 and flag==int(hit or (any_hit and not heal)) and reg(x.X86_REG_AL)==flag,'flag/stack result mismatch')
        if actor_profile=='derived':require(isinstance(reg(x.X86_REG_EBX),Mask) and reg(x.X86_REG_EBX).is_initial(),'incoming register mask not restored')
        else:require(reg(x.X86_REG_EBX)==('this',8) and reg(x.X86_REG_ESI)==('this',0),'entry actor/owner changed')
        require(physical_mask.is_initial(),'incoming physical mask not restored')
        require(all(not (family=='stack' and offset<flag_offset and flag_offset-3<offset+width) for _,family,offset,width in reads),'caller reads preserved low24 bytes')
        records.append(dict(hit=hit,anySpecial=any_hit,heal=heal,flag=flag,instructions=steps,
            querySlots=[hex(slot) for slot in queries],created=created,destroyed=destroyed,
            callerWrites=[dict(site=hex(a),offset=b,width=c) for a,b,c in writes]))
    return dict(status='passed_conditional_symbolic_mask_and_local_roles',booleanPaths=8,records=records,
        flagOffset=flag_offset,maskOffset=mask_offset,low24Preserved=True,incomingMaskPreserved=True,
        temporaryMask=temporary_mask,
        assumptions=['entry this is valid and disjoint from caller stack',f'incoming mask &{hex(temporary_mask)} ==0',
            'normal MSVC child balance and callee-saved registers','caller frame preserved except declared in-bounds disjoint4B CString objects',
            'CString heap/control/free footprints remain disjoint from caller locals','three virtual query slots return ordinary boolean values'],
        scope='Local caller region only; symbolic incoming mask covers arbitrary other29 bits and low24 bytes are never read/written. CString/API children retain explicit contracts, not active allocator/engine proof.')
