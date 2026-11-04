"""Recover hash-pinned unresolved interface statements from reviewed native setups."""
import hashlib
import json
import re
from collections import deque

from capstone import Cs, CS_ARCH_X86, CS_MODE_32, CS_GRP_JUMP, CS_GRP_RET, x86

UNKNOWN = 'unknown'
INTERFACE = 'interface'


def register_facts(instructions, register, interface_load=None):
    """Conservative all-branch constants for a nonvolatile x86 register.

    Calls use the retail ABI's nonvolatile-register contract. Unsupported writes
    become unknown; flag correlations and string comparisons are not assumed.
    """
    indexed = {i.address: i for i in instructions}
    following = {i.address: instructions[n+1].address if n+1<len(instructions) else None
                 for n,i in enumerate(instructions)}
    incoming = {a:set() for a in indexed}
    entry=instructions[0].address
    incoming[entry]={UNKNOWN}
    pending=deque([entry])
    while pending:
        address=pending.popleft();i=indexed[address]
        values=incoming[address]
        _,writes=i.regs_access()
        if address==interface_load:
            values={INTERFACE}
        elif register in writes:
            operands=i.operands
            same=lambda op:op.type==x86.X86_OP_REG and op.reg==register
            if i.mnemonic=='xor' and len(operands)==2 and all(same(o) for o in operands):
                values={0}
            elif i.mnemonic=='mov' and len(operands)==2 and same(operands[0]) and operands[1].type==x86.X86_OP_IMM:
                values={operands[1].imm}
            elif i.mnemonic=='lea' and len(operands)==2 and same(operands[0]) and operands[1].type==x86.X86_OP_MEM and operands[1].mem.base==register and not operands[1].mem.index and operands[1].mem.disp==0:
                pass
            else:
                values={UNKNOWN}
        if i.group(CS_GRP_RET):
            targets=[]
        elif i.group(CS_GRP_JUMP):
            if len(i.operands)!=1 or i.operands[0].type!=x86.X86_OP_IMM:
                raise ValueError('lifted interface calls: indirect control flow')
            targets=[i.operands[0].imm]
            if i.mnemonic!='jmp':targets.append(following[address])
        else:
            targets=[following[address]]
        for target in targets:
            if target is None:continue
            if target not in indexed:raise ValueError('lifted interface calls: external branch')
            new=incoming[target]|values
            if new!=incoming[target]:incoming[target]=new;pending.append(target)
    return incoming


def restore(source, todo, fn, row, rd):
    if row is None:return source,todo
    def require(ok,why):
        if not ok:raise ValueError('lifted interface calls: '+why)
    source=source.replace('\r\n','\n').replace('\r','\n')
    start,end=int(fn['address'],16),int(fn['bodyEndExclusive'],16)
    raw=rd.bytes_at(start,end-start)
    for key,data in [('nativeSha256',raw),('sourceSha256',fn['decompile'].encode()),('luaSha256',source.encode())]:
        require(data is not None and hashlib.sha256(data).hexdigest()==row[key],key+' changed')
    decoder=Cs(CS_ARCH_X86,CS_MODE_32);decoder.detail=True
    instructions=list(decoder.disasm(raw,start));index={i.address:i for i in instructions}
    require(instructions and instructions[-1].address+instructions[-1].size==end,'incomplete body')
    for site,encoded in row['nativeInstructions'].items():
        i=index.get(int(site,16));require(i is not None and i.bytes.hex()==encoded,'instruction '+site)
    from tools.script_recovery.annotate_interface_slots import load_slots
    from tools.script_recovery.convert_quest_unit import ROOT, callee_stack_words
    slot,target=int(row['slot'],16),int(row['target'],16)
    require(load_slots().get(slot)==row['method'],'interface method')
    manifest=json.loads((ROOT/'refs/fse_api_manifest.json').read_text())
    methods=[m for m in manifest['functions'] if m.get('name')==row['method']]
    require(len(methods)==1 and len(methods[0]['parameters'])==1 and methods[0]['parameters'][0]['type']=='bool','boolean binding signature')
    require(rd.bytes_at(0x1260f0c+slot,4)==target.to_bytes(4,'little'),'interface target')
    require(callee_stack_words(target)==1,'callee purge')
    callee=bytes.fromhex(row['calleeBytes'])
    require(len(callee)>=16 and rd.bytes_at(target,len(callee))==callee,'callee bytes')
    reg=lambda op,r:op.type==x86.X86_OP_REG and op.reg==r
    mem=lambda op,r,d:op.type==x86.X86_OP_MEM and op.size==4 and op.mem.base==r and not op.mem.index and op.mem.disp==d
    origin=row['receiverOrigin']
    bind,load,store=(index[int(origin[k],16)] for k in ['bind','load','save'])
    require(bind.mnemonic=='mov' and reg(bind.operands[1],x86.X86_REG_ECX),'script receiver bind')
    owner=bind.operands[0].reg;holder=load.operands[0].reg
    require(owner in [x86.X86_REG_EBP,x86.X86_REG_ESI] and load.mnemonic=='mov' and mem(load.operands[1],owner,4),'entity interface load')
    require(holder in [x86.X86_REG_EBX,x86.X86_REG_ESI,x86.X86_REG_EDI],'nonvolatile holder')
    saved=origin['balancedStackOffset']
    require(store.mnemonic=='mov' and mem(store.operands[0],x86.X86_REG_ESP,saved+4) and reg(store.operands[1],holder),'interface stack save')
    before=instructions[instructions.index(store)-2]
    require(before.mnemonic=='push' and before.operands[0].type==x86.X86_OP_IMM and before.operands[0].imm==1,'save stack balance')
    holder_facts=register_facts(instructions,holder,load.address)
    require(holder_facts[store.address]=={INTERFACE},'saved receiver provenance')
    bool_facts=register_facts(instructions,x86.X86_REG_EDI)
    edits=[];seen=set()
    for call in row['calls']:
        site=int(call['site'],16);require(site not in seen,'duplicate call');seen.add(site)
        native=index[site]
        native_calls=[c for c in fn['indirectCalls'] if int(c['site'],16)==site]
        require(len(native_calls)==1 and int(native_calls[0]['slot'],16)==slot,'native call inventory')
        require(native.mnemonic=='call' and native.operands[0].type==x86.X86_OP_MEM and native.operands[0].mem.disp==slot,'dispatch slot')
        predecessors={i.address for i in instructions if i.address+i.size==site and not i.group(CS_GRP_RET) and i.mnemonic!='jmp'}
        predecessors|={i.address for i in instructions if i.group(CS_GRP_JUMP) and i.operands[0].type==x86.X86_OP_IMM and i.operands[0].imm==site}
        covered=set()
        for setup in call['setups']:
            block=[index[int(a,16)] for a in setup]
            require(len(block) in [3,4] and all(a.address+a.size==b.address for a,b in zip(block,block[1:])),'setup continuity')
            require(block[-1].address not in covered,'duplicate predecessor')
            covered.add(block[-1].address)
            require(all(hex(i.address) in row['nativeInstructions'] for i in block),'unwitnessed setup')
            if len(block)==4:
                jump=block.pop();require(jump.mnemonic=='jmp' and jump.operands[0].type==x86.X86_OP_IMM and jump.operands[0].imm==site,'setup jump')
            else:require(block[-1].address+block[-1].size==site,'setup call adjacency')
            if block[0].mnemonic=='mov' and mem(block[0].operands[1],x86.X86_REG_ESP,saved):
                receiver,table,push=block
                require(reg(receiver.operands[0],x86.X86_REG_ECX),'stack receiver')
                require(table.mnemonic=='mov' and mem(table.operands[1],x86.X86_REG_ECX,0) and reg(table.operands[0],native.operands[0].mem.base),'stack dispatch table')
            else:
                table,push,receiver=block
                require(table.mnemonic=='mov' and mem(table.operands[1],holder,0) and reg(table.operands[0],native.operands[0].mem.base),'register dispatch table')
                require(holder_facts[table.address]=={INTERFACE},'receiver provenance')
                require(receiver.mnemonic=='mov' and reg(receiver.operands[0],x86.X86_REG_ECX) and reg(receiver.operands[1],holder),'register receiver')
            require(push.mnemonic=='push','argument push')
            argument=push.operands[0]
            require((argument.type==x86.X86_OP_IMM and argument.imm==0) or
                    (reg(argument,x86.X86_REG_EDI) and bool_facts[push.address]=={0}),'false argument provenance')
        require(covered==predecessors,'call predecessor inventory')
        if 'printedCall' in call:
            require('statement' not in call and 'diagnosticCount' not in call,'mixed call recovery kind')
            old=call['printedCall']
            match=re.fullmatch('quest:'+re.escape(row['method'])+r'\(\((\w+) ~= 0\)\)',old)
            require(match is not None,'pure numeric flag spelling')
            name=match[1]
            declarations=re.findall(r'(?m)^\s*local ([\w, ]+)\s*$',source)
            require(any(name in [v.strip() for v in d.split(',')] for d in declarations),'function local flag')
            diagnostic=None
        else:
            old='-- TODO(native): '+call['statement']
            diagnostic='unlifted: '+call['statement']
            require(todo.count(diagnostic)==call['diagnosticCount'],'diagnostic inventory')
        require(source.count(old)==1,'lifted call inventory')
        edits.append((old,'quest:'+row['method']+'(false)',diagnostic))
    updated=list(todo)
    for old,new,diagnostic in edits:
        source=source.replace(old,new,1)
        if diagnostic is not None:updated.remove(diagnostic)
    return source,updated
