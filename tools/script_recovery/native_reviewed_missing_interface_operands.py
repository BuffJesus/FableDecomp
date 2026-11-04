"""Recover reviewed GSI calls whose exported operands lost a receiver or value."""
import hashlib

def review(text, fn, row, rdata, entries):
    if row is None:
        return []
    def require(ok, reason):
        if not ok:
            raise ValueError(f'missing interface operands {fn["address"]}: {reason}')
    start,end=int(fn['address'],16),int(fn['bodyEndExclusive'],16)
    raw=rdata.bytes_at(start,end-start)
    require(raw is not None and hashlib.sha256(raw).hexdigest()==row['nativeSha256'],'native body changed')
    require(hashlib.sha256(fn['decompile'].encode()).hexdigest()==row['sourceSha256'],'source changed')
    from capstone import Cs,CS_ARCH_X86,CS_MODE_32,x86
    decoder=Cs(CS_ARCH_X86,CS_MODE_32);decoder.detail=True
    instructions=list(decoder.disasm(raw,start))
    by_site={i.address:i for i in instructions}
    require(instructions and instructions[-1].address+instructions[-1].size==end,'incomplete disassembly')
    require(len(row['nativeInstructions'])>=12,'incomplete native witness')
    for site,encoded in row['nativeInstructions'].items():
        ins=by_site.get(int(site,16))
        require(ins is not None and ins.bytes.hex()==encoded,'instruction changed at '+site)
    from tools.script_recovery.annotate_interface_slots import load_slots
    from tools.script_recovery.convert_quest_unit import callee_stack_words
    slot,table,target=(int(row[k],16) for k in ('slot','table','target'))
    require(table==0x01260f0c and load_slots().get(slot)==row['method'],'interface slot changed')
    require(rdata.bytes_at(table+slot,4)==target.to_bytes(4,'little'),'interface target changed')
    callee=bytes.fromhex(row['calleeBytes'])
    require(len(callee)>=16 and rdata.bytes_at(target,len(callee))==callee,'callee changed')
    require(row['purgeWords']==1 and callee_stack_words(target)==1,'callee purge changed')
    origin=row['receiverOrigin']
    def instruction(key):
        site=int(origin[key],16)
        require(hex(site) in row['nativeInstructions'],'unwitnessed receiver origin')
        return by_site[site]
    bind,load,store,reload=(instruction(k) for k in ('thisBind','load','store','reload'))
    holder=getattr(x86,'X86_REG_'+origin['holder'].upper())
    script=getattr(x86,'X86_REG_'+origin['script'].upper())
    def reg(op,value):return op.type==x86.X86_OP_REG and op.reg==value
    def mem(op,base,disp):return op.type==x86.X86_OP_MEM and op.size==4 and op.mem.base==base and not op.mem.index and op.mem.disp==disp
    require(bind.mnemonic=='mov' and reg(bind.operands[0],script) and reg(bind.operands[1],x86.X86_REG_ECX),'this receiver changed')
    require(load.mnemonic=='mov' and reg(load.operands[0],holder) and mem(load.operands[1],script,4),'GSI load changed')
    saved=origin['balancedStackOffset']
    require(saved>=0 and store.mnemonic=='mov' and mem(store.operands[0],x86.X86_REG_ESP,saved+4) and reg(store.operands[1],holder),'GSI stack save changed')
    index=instructions.index(store)
    push=instructions[index-2]
    require(push.mnemonic=='push' and push.operands[0].type==x86.X86_OP_IMM and push.operands[0].imm==1,'save stack depth changed')
    require(reload.mnemonic=='mov' and reg(reload.operands[0],holder) and mem(reload.operands[1],x86.X86_REG_ESP,saved),'GSI reload changed')
    edits=[];seen=set()
    require(row['calls'],'empty call inventory')
    for call in row['calls']:
        site=int(call['site'],16)
        require(site not in seen,'duplicate site');seen.add(site)
        matches=[entry for entry in entries or [] if int(entry[3]['site'],16)==site]
        require(len(matches)==1,'source site missing or ambiguous')
        a,b,args,metadata,key,vtable=matches[0]
        head=call['head'];begin=a-len(head)-1
        require(begin>=0 and text[begin:a]==head+'(' and text[b:b+1]==')' and args==call['printedArguments'],'printed call changed')
        require(vtable and int(key,16)==slot and int(metadata['slot'],16)==slot,'paired slot changed')
        native=by_site[site];index=instructions.index(native)
        require(hex(site) in row['nativeInstructions'] and native.mnemonic=='call' and native.operands[0].type==x86.X86_OP_MEM and native.operands[0].mem.disp==slot,'native call changed')
        setup=instructions[index-3:index]
        require(all(hex(i.address) in row['nativeInstructions'] for i in setup),'unwitnessed call setup')
        require(call['receiver']=='stack','unknown receiver witness')
        receiver,table_load,push=setup
        require(receiver.mnemonic=='mov' and reg(receiver.operands[0],x86.X86_REG_ECX) and mem(receiver.operands[1],x86.X86_REG_ESP,saved),'stack receiver changed')
        require(table_load.mnemonic=='mov' and mem(table_load.operands[1],x86.X86_REG_ECX,0) and reg(table_load.operands[0],native.operands[0].mem.base),'stack vtable changed')
        require(push.mnemonic=='push' and push.operands[0].type==x86.X86_OP_IMM and push.operands[0].imm==0 and call['arguments']==['false'],'reviewed argument changed')
        old=text[begin:b+1]
        require(text.count(old)==1,'reviewed call text is ambiguous')
        edits.append((old,'GSI->'+row['method']+'(false)'))
    return edits

def apply(text, edits):
    for old,value in edits:
        if text.count(old)!=1:
            raise ValueError('missing interface operands: reviewed call changed before lowering')
        text=text.replace(old,value,1)
    return text
