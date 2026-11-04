"""Scratch-only register-bit provenance; no heap/vtable lifetime claims."""
from collections import deque
import capstone as cs
from capstone import x86 as x

def analyze(raw, start, interface_offset=4, member_offsets=(), global_interface_addresses=(), parent_resource_profiles=(), member_table_loads=(), branch_tables=(), image=None, parent_offsets=()):
    if parent_offsets:
        member_offsets=tuple(member_offsets)
    parents=dict(parent_resource_profiles)
    if (len(parents)!=len(parent_resource_profiles) or any(type(k)is not int or not 0<k<4096 or k%4 or not isinstance(v,tuple) or not v or any(type(m)is not int or not 0<m<4096 or m%4 for m in v) for k,v in parents.items())
            or any(m not in member_offsets for m in member_table_loads)):
        raise ValueError('invalid parent/member table profiles')
    if (not isinstance(parent_offsets,(tuple,list))
            or any(type(offset)is not int or not 0<offset<4096 or offset%4 for offset in parent_offsets)
            or len(set(parent_offsets))!=len(parent_offsets)
            or (parent_offsets and set(parent_offsets)&(set(parents)|set(member_offsets)|{interface_offset}))):
        raise ValueError('invalid or ambiguous parent pointer offsets')
    parent_sites=set(parents)|set(parent_offsets)
    md=cs.Cs(cs.CS_ARCH_X86,cs.CS_MODE_32);md.detail=True
    ins=list(md.disasm(raw,start))
    if not ins or ins[-1].address+ins[-1].size!=start+len(raw):
        raise ValueError('incomplete decode')
    aliases={}
    for names in [('eax','ax','al','ah'),('ebx','bx','bl','bh'),('ecx','cx','cl','ch'),
                  ('edx','dx','dl','dh'),('esi','si'),('edi','di'),('ebp','bp'),('esp','sp')]:
        full=getattr(x,'X86_REG_'+names[0].upper())
        aliases.update({getattr(x,'X86_REG_'+name.upper()):full for name in names})
    index={i.address:k for k,i in enumerate(ins)}
    branch_proof=None
    branch_edges={}
    if branch_tables:
        if image is None or not callable(getattr(image,'bytes_at',None)):
            raise ValueError('branch-table image required')
        from tools.script_recovery.native_verified_branch_tables import verify
        branch_proof=verify(raw,start,image,branch_tables)
        branch_edges=branch_proof['targets']
    successors={}
    for k,i in enumerate(ins):
        if (i.group(cs.CS_GRP_INT) or i.group(cs.CS_GRP_IRET)
                or i.mnemonic in ('sysenter','sysexit','syscall','sysret','hlt','ud2','lcall','retf','retfd')
                or i.group(cs.CS_GRP_CALL) and i.mnemonic != 'call'):
            raise ValueError('unsupported control transfer')
        dest=[]
        if i.group(cs.CS_GRP_JUMP):
            if i.operands[0].type!=x.X86_OP_IMM:
                if i.address not in branch_edges:raise ValueError('indirect jump')
                dest.extend(index[target] for target in branch_edges[i.address])
            else:
                if i.operands[0].imm not in index:raise ValueError('external jump')
                dest.append(index[i.operands[0].imm])
        if not i.group(cs.CS_GRP_RET) and i.mnemonic!='jmp':
            if k+1<len(ins):dest.append(k+1)
            else:raise ValueError('unterminated body')
        successors[k]=dest

    def transfer(i,state):
        regs=dict(state);ops=i.operands;value=None;dest=None
        if i.mnemonic=='call':
            # Explicit ordinary MSVC ABI contract, not execution of child APIs.
            for reg in (x.X86_REG_EAX,x.X86_REG_ECX,x.X86_REG_EDX):regs.pop(reg,None)
            return regs
        if (i.mnemonic=='mov' and len(ops)==2 and ops[0].type==x.X86_OP_REG
                and ops[0].size==4):
            dest=ops[0].reg;src=ops[1]
            if src.type==x.X86_OP_REG and src.size==4:value=regs.get(src.reg)
            elif (src.type==x.X86_OP_MEM and src.size==4 and not src.mem.index
                  and not src.mem.segment):
                base=regs.get(src.mem.base)
                if src.mem.base==0 and src.mem.disp in global_interface_addresses:
                    value=('interface_snapshot',i.address)
                elif base==('entry_this',) and src.mem.disp in parent_sites:
                    value=('parent_snapshot',src.mem.disp,i.address)
                elif base and base[0]=='parent_snapshot' and src.mem.disp in parents.get(base[1],()):
                    value=('parent_member_table_snapshot',base[1],src.mem.disp,base[2],i.address)
                elif base==('entry_this',) and src.mem.disp in member_table_loads:
                    value=('member_table_snapshot',src.mem.disp,i.address)
                elif base==('entry_this',) and src.mem.disp==interface_offset:
                    # A value loaded at this site, never a claim of current field equality.
                    value=('interface_snapshot',i.address)
                elif base and base[0]=='parent_member_address' and src.mem.disp==0:
                    value=('parent_member_table_snapshot',base[1],base[2],base[3],i.address)
                elif base and base[0]=='interface_snapshot' and src.mem.disp==0:
                    value=('table_snapshot',base[1],i.address)
                elif base and base[0]=='member_address' and src.mem.disp==0:
                    value=('member_table_snapshot',base[1],i.address)
        elif (i.mnemonic=='lea' and len(ops)==2 and ops[0].type==x.X86_OP_REG and ops[0].size==4
              and ops[1].type==x.X86_OP_MEM and not ops[1].mem.index and not ops[1].mem.segment
              and regs.get(ops[1].mem.base)==('entry_this',) and ops[1].mem.disp in member_offsets):
            dest=ops[0].reg;value=('member_address',ops[1].mem.disp)
        if (i.mnemonic=='lea' and len(ops)==2 and ops[0].type==x.X86_OP_REG and ops[0].size==4
                and ops[1].type==x.X86_OP_MEM and not ops[1].mem.index and not ops[1].mem.segment):
            origin=regs.get(ops[1].mem.base)
            if origin and origin[0]=='parent_snapshot' and ops[1].mem.disp in parents.get(origin[1],()):
                dest=ops[0].reg;value=('parent_member_address',origin[1],ops[1].mem.disp,origin[2])
        if (i.mnemonic=='add' and len(ops)==2 and ops[0].type==x.X86_OP_REG and ops[0].size==4 and ops[1].type==x.X86_OP_IMM):
            origin=regs.get(ops[0].reg)
            if origin and origin[0]=='parent_snapshot' and ops[1].imm in parents.get(origin[1],()):
                dest=ops[0].reg;value=('parent_member_address',origin[1],ops[1].imm,origin[2])
        for reg in i.regs_access()[1]:regs.pop(aliases.get(reg,reg),None)
        if dest is not None and value is not None:regs[dest]=value
        # Memory writes cannot alter bits already held in a register. They can
        # invalidate object contents, which this analysis deliberately does not model.
        return regs

    states={0:{x.X86_REG_ECX:('entry_this',)}};pending=deque([0]);visits=0
    while pending:
        k=pending.popleft();visits+=1
        if visits>len(ins)*128:raise ValueError('nonconvergent CFG')
        after=transfer(ins[k],states[k])
        for nxt in successors[k]:
            old=states.get(nxt)
            merged=after if old is None else {r:v for r,v in old.items() if after.get(r)==v}
            if merged!=old:states[nxt]=merged;pending.append(nxt)
    result=dict(instructions=ins,states={ins[k].address:{md.reg_name(r):v for r,v in state.items()}
                                      for k,state in states.items()},visits=visits)
    if branch_proof is not None:result['verifiedBranchTables']=branch_proof
    return result
