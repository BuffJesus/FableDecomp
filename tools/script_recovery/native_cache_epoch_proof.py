"""Native cache-slot evidence under explicit ordinary caller-frame contracts."""
from collections import deque
import hashlib
import capstone as cs
from capstone import x86 as x

def prove(raw,start,row,contracts,snapshot_evidence,*,image=None):
    def require(ok,why):
        if not ok:raise ValueError('native cache epoch: '+why)
    require(hashlib.sha256(raw).hexdigest()==row['nativeSha256'],'native body changed')
    parent_writes=row.get('declaredParentWrites',{})
    require(isinstance(parent_writes,dict) and len(parent_writes)<=256,'invalid parent write inventory')
    abi=dict(ordinaryMsvcCalleeSaved=True,normalChildStackBalanceAssumed=True,
        callerFramePreservedExceptDeclaredBuffers=True,declaredBuffersStayInBounds=True,
        engineReentryVerified=False,exceptionsVerified=False,heapVerified=False)
    if parent_writes:abi['declaredParentBoolFieldsDoNotAliasCallerFrame']=True
    require(row['abiContract']==abi,'unsupported ABI contract')
    declared_parents={}
    for site,write in parent_writes.items():
        require(isinstance(write,dict) and set(write)=={'memberOffset','fieldOffset','value'},'invalid parent write descriptor')
        member,field=write['memberOffset'],write['fieldOffset']
        require(type(member)is int and 0<member<4096 and member%4==0
            and type(field)is int and 4<=field<4096 and type(write['value'])is bool,'invalid parent Bool field')
        try:a=int(site,16)
        except (ValueError,TypeError):raise ValueError('native cache epoch: invalid parent write site')
        require(a not in declared_parents,'duplicate parent write site')
        declared_parents[a]=write
    if parent_writes:require(row['abiContract']['declaredParentBoolFieldsDoNotAliasCallerFrame'] is True,'missing disjoint parent-field premise')
    decoder=cs.Cs(cs.CS_ARCH_X86,cs.CS_MODE_32);decoder.detail=True
    ins=list(decoder.disasm(raw,start));by={i.address:i for i in ins}
    require(ins and sum(i.size for i in ins)==len(raw),'incomplete instruction stream')
    load,store,read_pointer,read_table=(int(row[k],16) for k in ['pointerLoad','cacheStore','pointerRead','tableRead'])
    require(all(a in by for a in [load,store,read_pointer,read_table]),'missing endpoint')
    ptr,owner,table=row['pointerRegister'],row['ownerRegister'],row['tableRegister']
    require(ptr in ('ebx','esi','edi','ebp') and owner in ('ebx','esi','edi','ebp') and ptr!=owner,'invalid saved registers')
    pointer_register=by[read_pointer].reg_name(by[read_pointer].operands[0].reg)
    field=str(row['interfaceOffset']) if row['interfaceOffset']<10 else hex(row['interfaceOffset'])
    require((by[load].mnemonic,by[load].op_str)==('mov',ptr+', dword ptr ['+owner+' + '+field+']'),'interface load shape')
    branch_proof=None;branch_edges={}
    options=dict(interface_offset=row['interfaceOffset'])
    if row.get('branchTables'):
        require(image is not None and callable(getattr(image,'bytes_at',None)),'branch-table image required')
        from tools.script_recovery.native_verified_branch_tables import verify
        branch_proof=verify(raw,start,image,row['branchTables']);branch_edges=branch_proof['targets']
        options.update(branch_tables=row['branchTables'],image=image)
    if parent_writes:options['parent_offsets']=tuple(sorted({v['memberOffset'] for v in declared_parents.values()}))
    provenance=snapshot_evidence(raw,start,**options)['states']
    require(provenance.get(store,{}).get(ptr)==('interface_snapshot',load),'cache store pointer origin')
    store_ops=by[store].operands;read_ops=by[read_pointer].operands
    require(by[store].mnemonic=='mov' and len(store_ops)==2 and store_ops[0].type==x.X86_OP_MEM
        and store_ops[0].size==4 and store_ops[0].mem.base==x.X86_REG_ESP and not store_ops[0].mem.index and not store_ops[0].mem.segment
        and store_ops[1].type==x.X86_OP_REG and by[store].reg_name(store_ops[1].reg)==ptr,'cache write shape')
    require(by[read_pointer].mnemonic=='mov' and len(read_ops)==2 and read_ops[0].type==x.X86_OP_REG and read_ops[0].size==4
        and read_ops[1].type==x.X86_OP_MEM and read_ops[1].size==4 and read_ops[1].mem.base==x.X86_REG_ESP
        and not read_ops[1].mem.index and not read_ops[1].mem.segment,'cache reload shape')
    require(by[read_pointer].address+by[read_pointer].size==read_table
        and (by[read_table].mnemonic,by[read_table].op_str)==('mov',table+', dword ptr ['+pointer_register+']'),'table read shape')
    edges={};reverse={a:[] for a in by}
    for i in ins:
        targets=[]
        if i.group(cs.CS_GRP_JUMP):
            if i.operands[0].type==x.X86_OP_IMM:targets.append(i.operands[0].imm)
            else:
                require(i.address in branch_edges,'indirect branch')
                targets.extend(branch_edges[i.address])
        if not i.group(cs.CS_GRP_RET) and i.mnemonic!='jmp':targets.append(i.address+i.size)
        require(all(t in by for t in targets),'external branch/fallthrough')
        edges[i.address]=targets
        for t in targets:reverse[t].append(i.address)
    def reachable(seeds,graph,cut):
        found=set();pending=deque(seeds)
        while pending:
            a=pending.popleft()
            if a in cut or a in found:continue
            found.add(a);pending.extend(graph[a])
        return found
    require(read_table not in reachable([start],edges,{store}),'cache store does not dominate read')
    entry=store+by[store].size
    epoch=reachable([entry],edges,{store})&reachable([read_table],reverse,{store})
    require(read_pointer in epoch and entry in epoch,'missing store/read path')
    calls={a for a in epoch if by[a].group(cs.CS_GRP_CALL)}
    require(set(contracts)==calls,'call contract inventory mismatch')
    require(all(isinstance(c['purgeBytes'],int) and 0<=c['purgeBytes']<=256 and c['purgeBytes']%4==0 for c in contracts.values()),'invalid call purge')
    states={entry:0};pending=deque([entry]);writes=[];escapes=[];implicit=[];parent_reviews=[];parent_seen=set()
    expected_escapes={int(a,16):v for a,v in row['declaredBuffers'].items()}
    cache=store_ops[0].mem.disp
    def overlaps(coordinate,width):return coordinate<cache+4 and coordinate+width>cache
    while pending:
        a=pending.popleft();i=by[a];before=states[a];after=before;ops=i.operands
        if i.mnemonic=='push':
            require(not (ops[0].type==x.X86_OP_REG and ops[0].reg==x.X86_REG_ESP),'unreviewed direct stack-pointer escape')
            after-=4;implicit.append((a,after,4))
        elif i.mnemonic=='pop':
            require(not (ops[0].type==x.X86_OP_REG and ops[0].reg==x.X86_REG_ESP),'pop ESP')
            after+=4
        elif i.group(cs.CS_GRP_CALL):
            implicit.append((a,before-4,4));after+=contracts[a]['purgeBytes']
        elif x.X86_REG_ESP in i.regs_access()[1]:
            if i.mnemonic in ('add','sub') and ops[0].type==x.X86_OP_REG and ops[0].reg==x.X86_REG_ESP and ops[1].type==x.X86_OP_IMM:
                after+=ops[1].imm*(1 if i.mnemonic=='add' else -1)
            elif (i.mnemonic=='lea' and len(ops)==2 and ops[0].type==x.X86_OP_REG and ops[0].reg==x.X86_REG_ESP
                  and ops[1].type==x.X86_OP_MEM and ops[1].mem.base==x.X86_REG_ESP and not ops[1].mem.index and not ops[1].mem.segment):after+=ops[1].mem.disp
            else:require(False,'unsupported ESP write')
        for op in ops:
            if op.type==x.X86_OP_MEM and op.access&cs.CS_AC_WRITE:
                if op.mem.base==x.X86_REG_ESP and not op.mem.index and not op.mem.segment:
                    coordinate=before+op.mem.disp;require(not overlaps(coordinate,op.size),'direct write overlaps cache')
                    writes.append(dict(site=hex(a),coordinate=coordinate,width=op.size))
                else:
                    if not parent_writes:require(False,'non-ESP memory write in epoch')
                    require(a in declared_parents and a not in parent_seen,'unreviewed non-ESP memory write in epoch')
                    contract=declared_parents[a]
                    require(i.mnemonic=='mov' and len(ops)==2 and ops[0].type==x.X86_OP_MEM and ops[0].size==1
                        and ops[1].type==x.X86_OP_IMM and ops[1].imm==int(contract['value'])
                        and not op.mem.index and not op.mem.segment and op.mem.disp==contract['fieldOffset'],'parent Bool write shape changed')
                    origin=provenance.get(a,{}).get(i.reg_name(op.mem.base))
                    require(origin is not None and origin[:2]==('parent_snapshot',contract['memberOffset']),'unproved parent pointer load-site origin')
                    parent_seen.add(a);parent_reviews.append(dict(site=hex(a),fieldOffset=contract['fieldOffset'],value=contract['value'],origin=origin,
                        outsideCallerFrameAssumed=True,heapLifetimeVerified=False))
        if i.mnemonic=='mov' and len(ops)==2 and ops[1].type==x.X86_OP_REG and ops[1].reg==x.X86_REG_ESP:
            require(ops[0].type==x.X86_OP_REG and ops[0].size==4 and a in expected_escapes,'unreviewed copied ESP')
            buffer=expected_escapes[a]
            require(before==buffer['coordinate'] and buffer['bytes']>0,'copied ESP buffer coordinate/extent')
            require(not overlaps(before,buffer['bytes']),'copied ESP buffer overlaps cache')
            escapes.append(dict(site=hex(a),coordinate=before,bytes=buffer['bytes']))
        if i.mnemonic=='lea' and len(ops)==2 and ops[1].type==x.X86_OP_MEM and ops[1].mem.base==x.X86_REG_ESP and ops[0].reg!=x.X86_REG_ESP:
            require(not ops[1].mem.index and not ops[1].mem.segment and a in expected_escapes,'unreviewed stack address')
            buffer=expected_escapes[a];coordinate=before+ops[1].mem.disp
            require(coordinate==buffer['coordinate'] and buffer['bytes']>0,'declared buffer coordinate/extent')
            require(not overlaps(coordinate,buffer['bytes']),'declared buffer overlaps cache')
            escapes.append(dict(site=hex(a),coordinate=coordinate,bytes=buffer['bytes']))
        for t in edges[a]:
            if t not in epoch:continue
            require(t not in states or states[t]==after,'inconsistent ESP join')
            if t not in states:states[t]=after;pending.append(t)
    require(parent_seen==set(declared_parents),'unused or missing parent write contract')
    require(set(states)==epoch,'incomplete epoch equations')
    require({int(e['site'],16) for e in escapes}==set(expected_escapes),'unused/missing buffer declaration')
    require(not any(overlaps(a,n) for _,a,n in implicit),'implicit stack write overlaps cache')
    coordinate=states[read_pointer]+read_ops[1].mem.disp
    require(coordinate==cache,'cache read coordinate differs')
    result=dict(status='passed_native_cache_isolation_under_declared_ABI',instructions=len(epoch),calls=len(calls),
        pointerLoad=hex(load),cacheStore=hex(store),tableRead=hex(read_table),cacheCoordinate=cache,readCoordinate=coordinate,
        explicitWrites=writes,declaredBufferEscapes=escapes,abiContract=row['abiContract'],
        limits='Native pointer origin, normal CFG, ESP equations and direct caller writes are checked. Opaque children, static interface targets and bounded buffer writes use the declared ordinary ABI/caller-frame contract; actual heap/vtable lifetime, reentry, exceptions and gameplay are unverified. This proof alone does not classify Lua diagnostics.')

    if branch_proof is not None:result['verifiedBranchTables']=branch_proof
    if parent_reviews:
        result['parentFieldWrites']=parent_reviews
        result['limits']+=' Declared parent Bool fields are assumed in-bounds and outside the caller frame; pointer load-site origins and exact byte writes are checked,not physical object lifetime.'
    return result
