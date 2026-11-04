"""Recover signed int increments after existing reviewed ownership recovery.

An Int map alone is insufficient. Require matching explicit signed C declarations,
same-member read/store and immediate comparison, a unique retail instruction
sequence with a proven receiver register, and the corresponding lifted Lua pair.
"""
from collections import deque
import hashlib,json,re
from capstone import Cs,CS_ARCH_X86,CS_MODE_32,x86

TOKEN=re.compile(r'/\*[\s\S]*?\*/|//[^\n]*|"(?:\\.|[^"\\])*"|\'(?:\\.|[^\'\\])*\'')
NUMBER=r'(?:0x[0-9a-f]+|[0-9]+)'
FIELD=r'\*\(int \*\)\(this \+ (?P<offset>'+NUMBER+r')\)'
PAIR=re.compile(r'^[ \t]*(?P<local>[A-Za-z_]\w*) = (?P<field>'+FIELD+r') \+ 1;[ \t]*\n[ \t]*(?P=field) = (?P=local);[ \t]*\n[ \t]*if \((?P=local) < (?P<bound>'+NUMBER+r')\) \{',re.M)
REGISTERS=('eax','ebx','ecx','edx','esi','edi','ebp','esp')
REGISTER_ROOT={part:root for root,parts in {
    'eax':('eax','ax','al','ah'),'ebx':('ebx','bx','bl','bh'),
    'ecx':('ecx','cx','cl','ch'),'edx':('edx','dx','dl','dh'),
    'esi':('esi','si'),'edi':('edi','di'),'ebp':('ebp','bp'),'esp':('esp','sp'),
}.items() for part in parts}

def _receiver_states(instructions):
    """Meet receiver aliases over reachable x86 branches; calls clobber volatiles."""
    by={i.address:i for i in instructions};states={instructions[0].address:{r:r=='ecx' for r in REGISTERS}}
    pending=deque([instructions[0].address])
    while pending:
        address=pending.popleft();ins=by[address];after=states[address].copy()
        for register in ins.regs_access()[1]:
            name=REGISTER_ROOT.get(ins.reg_name(register))
            if name in after:after[name]=False
        operands=ins.operands
        if ins.mnemonic=='mov' and len(operands)==2 and all(op.type==x86.X86_OP_REG and op.size==4 for op in operands):
            dest,source=(ins.reg_name(op.reg) for op in operands)
            if dest in after:after[dest]=states[address].get(source,False)
        if ins.mnemonic=='call':
            for name in ('eax','ecx','edx'):after[name]=False
        following=address+ins.size
        if ins.mnemonic.startswith('ret'):successors=[]
        elif ins.mnemonic=='jmp':
            if not operands or operands[0].type!=x86.X86_OP_IMM:return {}
            successors=[operands[0].imm]
        elif ins.group(x86.X86_GRP_JUMP):
            if not operands or operands[0].type!=x86.X86_OP_IMM:return {}
            successors=[following,operands[0].imm]
        else:successors=[following] if following in by else []
        for target in successors:
            if target not in by:return {}
            merged=after.copy() if target not in states else {r:states[target][r] and after[r] for r in REGISTERS}
            if states.get(target)!=merged:states[target]=merged;pending.append(target)
    return states

def _native_sites(fn,rdata):
    start,end=int(fn['address'],16),int(fn['bodyEndExclusive'],16)
    data=rdata.bytes_at(start,end-start)
    if not data:return {},None
    decoder=Cs(CS_ARCH_X86,CS_MODE_32);decoder.detail=True;ins=list(decoder.disasm(data,start))
    if not ins or sum(i.size for i in ins)!=len(data):return {},None
    states=_receiver_states(ins);sites={}
    for group in zip(*(ins[offset:] for offset in range(6))):
        load,increment,copy,compare,store,branch=group
        if [i.mnemonic for i in group]!=['mov','inc','mov','cmp','mov','jl']:continue
        a,b,c,d,e=(i.operands for i in group[:5])
        if not (len(a)==2 and a[0].type==x86.X86_OP_REG and a[1].type==x86.X86_OP_MEM and a[1].size==4):continue
        memory=a[1].mem;base=load.reg_name(memory.base)
        if memory.index or memory.segment or not states.get(load.address,{}).get(base,False):continue
        reg=a[0].reg
        if not (len(b)==1 and b[0].type==x86.X86_OP_REG and b[0].reg==reg):continue
        if not (len(c)==2 and all(op.type==x86.X86_OP_REG for op in c) and c[1].reg==reg):continue
        if not (len(d)==2 and d[0].type==x86.X86_OP_REG and d[0].reg==c[0].reg and d[1].type==x86.X86_OP_IMM and 0<=d[1].imm<=0x7fffffff):continue
        if not (len(e)==2 and e[0].type==x86.X86_OP_MEM and e[0].size==4 and e[1].type==x86.X86_OP_REG and e[1].reg==reg):continue
        destination=e[0].mem
        if destination.segment or (destination.base,destination.index,destination.disp)!=(memory.base,memory.index,memory.disp):continue
        if not states.get(store.address,{}).get(store.reg_name(destination.base),False):continue
        key=(memory.disp,d[1].imm)
        sites.setdefault(key,[]).append(dict(begin=hex(load.address),end=hex(branch.address+branch.size),bytes=b''.join(bytes(i.bytes) for i in group).hex(),receiver_register=base))
    return sites,hashlib.sha256(data).hexdigest()

def recover(source,fn,spec,rdata):
    c=fn.get('decompile','').replace('\r\n','\n')
    masked=TOKEN.sub(lambda m:''.join('\n' if char=='\n' else ' ' for char in m[0]),c)
    pairs=list(PAIR.finditer(masked))
    if not pairs:return source,[]
    sites,body_digest=_native_sites(fn,rdata);edits=[];evidence=[];counts={}
    for pair in pairs:
        key=tuple(int(pair[name],16 if pair[name].startswith('0x') else 10) for name in ('offset','bound'));counts[key]=counts.get(key,0)+1
    for pair in pairs:
        local=pair['local'];offset,bound=(int(pair[name],16 if pair[name].startswith('0x') else 10) for name in ('offset','bound'))
        if counts[(offset,bound)]!=1 or len(sites.get((offset,bound),[]))!=1:continue
        if len(re.findall(r'^[ \t]*int '+re.escape(local)+r';[ \t]*$',masked,re.M))!=1:continue
        field=getattr(spec,'self_fields',{}).get(offset)
        if not field or field[1]!='Int':continue
        receiver='__native_entity_state' if getattr(spec,'entity',False) else 'quest';quoted=json.dumps(field[0])
        getter=receiver+':GetStateInt('+quoted+')'
        pattern=re.compile(r'^(?P<indent>[ \t]*)(?:local )?'+re.escape(local)+r' = (?P<value>'+re.escape(getter)+r' \+ 1)\n[ \t]*'+re.escape(receiver+':SetStateInt('+quoted+', '+local+')')+r'\n[ \t]*if '+re.escape(local)+r' < (?:'+str(bound)+'|'+hex(bound)+r') then',re.M)
        matches=list(pattern.finditer(source))
        if len(matches)!=1:continue
        match=matches[0];value=match['value'];replacement='(('+value+' + 2147483648) % 4294967296 - 2147483648)'
        edits.append((match.start('value'),match.end('value'),replacement))
        evidence.append(dict(field=field[0],offset=hex(offset),local=local,bound=bound,native=sites[(offset,bound)][0],body_sha256=body_digest,export_sha256=hashlib.sha256(fn['decompile'].encode()).hexdigest(),before=value,after=replacement,basis='Explicit int local/member pair, unique MOV/INC/MOV/CMP/store/JL and receiver alias proven over reachable branches; apply after prior reviewed recovery.'))
    for start,end,replacement in sorted(edits,reverse=True):source=source[:start]+replacement+source[end:]
    return source,evidence


# Direct member increments have no temporary/comparison pair. Retain a separate
# certificate: explicit signed C read/store, unique native memory INC32 and a
# proven entry-this receiver, mapped Int member, and one actual Lua setter.
DIRECT_FIELD=r'\*\(int \*\)\((?:\(int\))?this \+ (?P<offset>'+NUMBER+r')\)'
DIRECT_PAIR=re.compile(r'^[ \t]*(?P<field>'+DIRECT_FIELD+r') = (?P=field) \+ 1;[ \t]*$',re.M)

def _direct_sites(fn,rdata):
    start,end=int(fn['address'],16),int(fn['bodyEndExclusive'],16)
    raw=rdata.bytes_at(start,end-start)
    if not raw:return {},None
    decoder=Cs(CS_ARCH_X86,CS_MODE_32);decoder.detail=True;ins=list(decoder.disasm(raw,start))
    if not ins or sum(i.size for i in ins)!=len(raw):return {},None
    states=_receiver_states(ins);sites={}
    for i in ins:
        if i.mnemonic!='inc' or len(i.operands)!=1:continue
        op=i.operands[0]
        if op.type!=x86.X86_OP_MEM or op.size!=4:continue
        memory=op.mem;base=i.reg_name(memory.base)
        if memory.index or memory.segment or not states.get(i.address,{}).get(base,False):continue
        sites.setdefault(memory.disp,[]).append(dict(begin=hex(i.address),end=hex(i.address+i.size),bytes=i.bytes.hex(),receiver_register=base))
    return sites,hashlib.sha256(raw).hexdigest()

def recover_direct(source,fn,spec,rdata):
    c=fn.get('decompile','').replace('\r\n','\n')
    clean=TOKEN.sub(lambda m:''.join('\n' if char=='\n' else ' ' for char in m[0]),c)
    pairs=list(DIRECT_PAIR.finditer(clean))
    if not pairs:return source,[]
    from tools.script_recovery.readable_lua import tokens
    code_starts={t.start() for t in tokens(source) if t.lastgroup=='identifier'}
    sites,body_digest=_direct_sites(fn,rdata);edits=[];evidence=[];counts={}
    for pair in pairs:
        offset=int(pair['offset'],0);counts[offset]=counts.get(offset,0)+1
    fields=getattr(spec,'self_fields',{})
    for pair in pairs:
        offset=int(pair['offset'],0)
        if counts[offset]!=1 or len(sites.get(offset,[]))!=1:continue
        field=fields.get(offset)
        if not field or field[1]!='Int':continue
        if sum(other[0]==field[0] for other in fields.values())!=1:continue
        receiver='__native_entity_state' if getattr(spec,'entity',False) else 'quest';quoted=json.dumps(field[0])
        getter=receiver+':GetStateInt('+quoted+')';value=getter+' + 1'
        pattern=re.compile(r'^(?P<indent>[ \t]*)(?P<receiver>'+re.escape(receiver)+r'):SetStateInt\('+re.escape(quoted)+r', (?P<value>'+re.escape(value)+r')\)[ \t]*$',re.M)
        matches=[m for m in pattern.finditer(source) if m.start('receiver') in code_starts]
        if len(matches)!=1:continue
        match=matches[0];replacement='(('+value+' + 2147483648) % 4294967296 - 2147483648)'
        edits.append((match.start('value'),match.end('value'),replacement))
        evidence.append(dict(kind='direct_memory_inc',field=field[0],offset=hex(offset),native=sites[offset][0],body_sha256=body_digest,export_sha256=hashlib.sha256(fn['decompile'].encode()).hexdigest(),before=value,after=replacement,basis='Explicit signed C same-member +1, mapped Int, unique native memory INC32 and entry-this receiver over reachable branches; one executable setter/getter with single evaluation. No helper, callback or control flow moved.'))
    for begin,end,replacement in sorted(edits,reverse=True):source=source[:begin]+replacement+source[end:]
    return source,evidence
