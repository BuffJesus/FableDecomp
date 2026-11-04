"""Validate missing CString/conversation call records inside an existing switch."""
from copy import deepcopy
from functools import wraps
import hashlib,json
from capstone import Cs,CS_ARCH_X86,CS_MODE_32,x86 as x
from tools.script_recovery.native_receiver_reaching import _cfg,_reaching,_switch_targets
CTOR=0x99ebf0;DTOR=0x99eae0
PROFILES={CTOR:(61,'8c27b64a613d8319d3e866b8224ecfcedf8e5ccaf92a81314c16631d41f3f100'),DTOR:(12,'65d790bd8e62fef3658c4d1320e81e8b1c6d54ab12879256c0e4e4c533c1c53e')}
def digest(data):return hashlib.sha256(data).hexdigest()
def original_metadata(fn):return {key:fn.get(key) for key in ['calls','indirectCalls','callOrder','bodyRanges','size']}
def augment(fn,row,image,unit,definition_addresses=()):
    def require(ok,why):
        if not ok:raise ValueError('reviewed missing calls '+fn['address']+': '+why)
    start,end=int(fn['address'],16),int(fn['bodyEndExclusive'],16);raw=image.bytes_at(start,end-start)
    require(raw is not None and digest(raw)==row['nativeExtentSha256'],'native extent changed')
    require(digest(fn['decompile'].encode())==row['sourceSha256'],'exported source changed')
    require(digest(json.dumps(original_metadata(fn),sort_keys=True,separators=(',',':')).encode())==row['originalMetadataSha256'],'original metadata changed')
    owner=row['owner'];require(owner in unit.get('entities',{}),'owner missing');entity=unit['entities'][owner]
    vt=int(entity['vtable'],16)
    require(vt==int(row['entityVtable'],16) and int(entity['functions']['Main']['address'],16)==start and image.bytes_at(vt+4,4)==start.to_bytes(4,'little'),'registered Main identity changed')
    for target,(size,sha) in PROFILES.items():require(digest(image.bytes_at(target,size))==sha,'CString child profile changed')
    require(CTOR not in definition_addresses and DTOR not in definition_addresses,'new in-unit return inference would require reindexing')
    md=Cs(CS_ARCH_X86,CS_MODE_32);md.detail=True;ins=list(md.disasm(raw,start));by={i.address:k for k,i in enumerate(ins)}
    require(ins and sum(i.size for i in ins)==len(raw),'incomplete native extent')
    preds=_cfg(ins,x,__import__('capstone'),image);require(preds is not None,'unbounded native control')
    successors={k:[] for k in preds}
    for k,parents in preds.items():
        for parent in parents:successors[parent].append(k)
    reachable=set();pending=[0]
    while pending:
        k=pending.pop()
        if k in reachable:continue
        reachable.add(k);pending.extend(successors[k])
    normal_preds={k:[p for p in parents if p in reachable] if k in reachable else [] for k,parents in preds.items()}
    def instruction(address):
        address=int(address,16);require(address in by,'instruction missing');return ins[by[address]]
    owner_def=instruction(row['ownerDefinition']);actor_def=instruction(row['actorDefinition'])
    require(owner_def.mnemonic=='mov' and owner_def.op_str=='esi, ecx' and actor_def.mnemonic=='lea' and actor_def.op_str=='ebp, [esi + 8]','unsupported saved owner/actor origins')
    require(not any(x.X86_REG_ECX in i.regs_access()[1] for i in ins[:by[owner_def.address]]),'entry this overwritten')
    selector=instruction(row['selector']);require(selector.mnemonic=='jmp' and selector.operands[0].type==x.X86_OP_MEM,'selector changed')
    si=by[selector.address];cmp_,guard=ins[si-2:si]
    require(cmp_.mnemonic=='cmp' and cmp_.operands[0].type==x.X86_OP_REG and cmp_.operands[0].reg==x.X86_REG_EBX and cmp_.operands[1].imm==3 and guard.mnemonic=='ja','selector unsigned bound changed')
    targets=_switch_targets(ins,si,selector.operands[0],x,image)
    require(targets is not None and [hex(a) for a in targets]==row['caseEntries'] and selector.operands[0].mem.index==x.X86_REG_EBX,'case targets changed')
    require(all(a in by for a in targets) and si in reachable,'case outside native extent/entry')
    def definitions(site,reg):
        found=_reaching(ins,normal_preds,by[site],reg,x);require(-1 not in found,'unknown saved register origin');return {ins[k].address for k in found}
    require(definitions(selector.address,x.X86_REG_ESI)=={owner_def.address} and definitions(selector.address,x.X86_REG_EBP)=={actor_def.address},'owner/actor reaching definitions changed')
    cid=instruction(row['idDefinition']);producer=instruction(row['idProducer'])
    require(cid.mnemonic=='mov' and cid.op_str=='edi, eax' and producer.mnemonic=='call' and producer.operands[0].type==x.X86_OP_MEM and producer.operands[0].mem.disp==0x5b0,'conversation ID producer changed')
    require(definitions(selector.address,x.X86_REG_EDI)=={cid.address},'conversation ID reaching definition changed')
    require(producer.address<cid.address,'ID producer order changed')
    for k in range(by[producer.address]+1,by[cid.address]+1):
        require(normal_preds[k]==[k-1],'branch bypasses ID producer')
        if k<by[cid.address]:require(not ins[k].group(x.X86_GRP_CALL) and x.X86_REG_EAX not in ins[k].regs_access()[1],'ID result overwritten')
    frame=row['frame'];allocation=instruction(frame['allocation'])
    require(allocation.address==start and allocation.mnemonic=='sub' and allocation.op_str=='esp, '+hex(frame['allocationBytes']),'frame allocation changed')
    saves=[instruction(a) for a in frame['savedRegisters']];registers={i.operands[0].reg for i in saves if i.mnemonic=='push' and i.operands[0].type==x.X86_OP_REG}
    require(registers=={x.X86_REG_EBX,x.X86_REG_EBP,x.X86_REG_ESI,x.X86_REG_EDI} and len(saves)==4 and frame['bias']==frame['allocationBytes']+16,'saved frame changed')
    # Each declared save dominates the selector from the registered entry.
    for save in saves:
        seen=set();pending=[0];blocked=by[save.address]
        while pending:
            k=pending.pop()
            if k==blocked or k in seen:continue
            seen.add(k);pending.extend(successors[k])
        require(si not in seen,'selector bypasses frame save')
    ordered_saves=sorted(saves,key=lambda i:i.address)
    require([i.address for i in ordered_saves[:3]]==[i.address for i in ins[1:4]],'early saves are not the frame prologue')
    late=ordered_saves[3];li=by[late.address]
    require(late.op_str=='edi' and ins[li+1].mnemonic=='mov' and ins[li+1].op_str=='ecx, esi' and ins[li+2].mnemonic=='call' and ins[li+2].operands[0].type==x.X86_OP_IMM and ins[li+2].operands[0].imm==0xf35b30,'unsupported late save/query pattern')
    require(image.bytes_at(0xf35b30,12)==bytes.fromhex('8b01ff500c8bc8e9041ed8ff'),'late save parent-query ABI changed')
    line=row['lineAPI'];target=int(line['target'],16);api=image.bytes_at(target,line['size'])
    require(image.bytes_at(0x1260f0c+0x5b8,4)==target.to_bytes(4,'little') and api is not None and digest(api)==line['sha256'],'conversation child profile changed')
    decoded=list(md.disasm(api,target));require(sum(i.size for i in decoded)==len(api) and {i.op_str for i in decoded if i.mnemonic=='ret'}=={'0x14'},'conversation child purge changed')
    bias=frame['bias'];other=row['otherThing'];require(other['entryOffset']<0 and bias+other['entryOffset']==0x1c,'unsupported other thing frame role')
    query=instruction(other['querySite']);require(query.mnemonic=='call' and query.operands[0].type==x.X86_OP_MEM and query.operands[0].mem.disp==0x120,'other thing query changed')
    existing={c['site'].lower():c for c in fn['calls']+fn['indirectCalls']}
    q=existing.get(f'0x{query.address:08x}');require(q is not None and other['entryOffset'] in q.get('pushedStack',[]),'other thing original stack evidence changed')
    name_ctor=instruction(other['nameConstructor']);require(name_ctor.mnemonic=='call' and name_ctor.operands[0].type==x.X86_OP_IMM and name_ctor.operands[0].imm==CTOR,'name constructor changed')
    literal_push=ins[by[name_ctor.address]-2];require(literal_push.mnemonic=='push' and literal_push.operands[0].type==x.X86_OP_IMM and image.string_at(literal_push.operands[0].imm)==other['name'],'other thing native name changed')
    names={target:{c.get('currentName') for c in fn['calls'] if int(c.get('target','0'),16)==target} for target in [CTOR,DTOR]}
    require(all(len(v)==1 and None not in v for v in names.values()),'CString labels ambiguous')
    records={};literal_by_site={};case_reviews=[];exits={int(a,16) for a in row['caseExits']}
    for case,entry in enumerate(targets):
        regs={x.X86_REG_ESI:('this',0),x.X86_REG_EBP:('me',0),x.X86_REG_EDI:('id',0),x.X86_REG_EBX:('counter',case)};pushes=[];delta=0;live={};pc=entry;seen=set();calls=[]
        def address(op):
            require(op.type==x.X86_OP_MEM and not op.mem.index and not op.mem.segment,'unsupported case address')
            if op.mem.base==x.X86_REG_ESP:return ('stack',delta+op.mem.disp)
            base=regs.get(op.mem.base);require(base is not None,'unknown case base');return (base[0],base[1]+op.mem.disp)
        while pc not in exits:
            require(pc in by and pc not in seen,'case leaves/repeats extent');seen.add(pc);i=ins[by[pc]];ops=i.operands;pc+=i.size
            if i.mnemonic=='push':
                value=('imm',ops[0].imm&0xffffffff) if ops[0].type==x.X86_OP_IMM else regs.get(ops[0].reg);require(value is not None,'unknown case push');pushes.append(value);delta-=4
            elif i.mnemonic=='lea':require(ops[0].type==x.X86_OP_REG,'case LEA destination');regs[ops[0].reg]=address(ops[1])
            elif i.mnemonic=='mov':
                require(ops[0].type==x.X86_OP_REG,'case memory write')
                if ops[1].type==x.X86_OP_REG:value=regs.get(ops[1].reg)
                else:
                    at=address(ops[1]);value=('gsi',0) if at==('this',4) else ('table',0) if at==('gsi',0) else None
                require(value is not None,'unknown case value');regs[ops[0].reg]=value
            elif i.mnemonic=='call':
                if ops[0].type==x.X86_OP_IMM:
                    target=ops[0].imm;require(target in [CTOR,DTOR],'unsupported direct child');n=2 if target==CTOR else 0
                    record=dict(site=f'0x{i.address:08X}',target=f'0x{target:08X}',currentName=next(iter(names[target])),depth=bias-delta)
                    receiver=regs.get(x.X86_REG_ECX);require(receiver and receiver[0]=='stack','CString receiver not frame');record['ecxStack']=receiver[1]-bias
                else:
                    require(ops[0].type==x.X86_OP_MEM and ops[0].mem.disp==0x5b8 and not ops[0].mem.index and regs.get(ops[0].mem.base)==('table',0) and regs.get(x.X86_REG_ECX)==('gsi',0),'case conversation receiver changed');target=None;n=5
                    record=dict(site=f'0x{i.address:08X}',slot='0x5b8',kind='vtable',depth=bias-delta)
                require(len(pushes)>=n,'missing case pushes');args=list(reversed(pushes[-n:])) if n else []
                if n:record['pushedStack']=[v[1]-bias if v[0]=='stack' else None for v in reversed(args)]
                if target==CTOR:
                    require(not live and receiver not in live and args[0][0]=='imm' and args[1]==('imm',0xffffffff),'CString construction shape changed');text=image.string_at(args[0][1]);require(text is not None,'literal unavailable');live[receiver]=text;literal_by_site[record['site'].lower()]=text
                elif target==DTOR:require(receiver in live and len(live)==1,'CString cleanup changed');live.pop(receiver)
                else:
                    require(args[0]==('id',0) and args[1] in live and args[2]==('imm',0) and args[3] in [('me',0),('stack',bias+other['entryOffset'])] and args[4] in [('me',0),('stack',bias+other['entryOffset'])],'conversation operands changed')
                if record['site'] in records:require(records[record['site']]==record,'duplicate differing case call')
                records[record['site']]=record;calls.append(record['site'])
                if n:del pushes[-n:]
                delta+=4*n
                for reg in [x.X86_REG_EAX,x.X86_REG_ECX,x.X86_REG_EDX]:regs.pop(reg,None)
            elif i.mnemonic=='jmp':require(ops[0].type==x.X86_OP_IMM,'indirect case branch');pc=ops[0].imm
            else:require(False,'unsupported case instruction')
        require(not live and not pushes and delta==0,'unbalanced case exit');case_reviews.append(dict(entry=hex(entry),exit=hex(pc),calls=calls))
    missing={site:record for site,record in records.items() if site.lower() not in existing}
    expected={record['site']:record for record in row['records']};require(len(expected)==len(row['records'])==len({int(record['site'],16) for record in row['records']}),'duplicate reviewed call sites');require(missing==expected,'observed missing records changed')
    holes={s.lower() for s in fn['callOrder'] if s.lower() not in existing};require(holes=={s.lower() for s in missing},'call order has other holes')
    for site,record in records.items():
        if site.lower() in existing:
            for key in ['depth','ecxStack','pushedStack','target','slot','kind']:
                if key in existing[site.lower()]:require(record.get(key)==existing[site.lower()][key],'original case stack calibration changed')
    out=deepcopy(fn)
    for record in missing.values():out['indirectCalls' if record.get('kind')=='vtable' else 'calls'].append(record)
    for key in ['calls','indirectCalls']:out[key].sort(key=lambda c:int(c['site'],16))
    from tools.script_recovery.convert_quest_unit import _text_order_sites
    ordered=_text_order_sites(out['decompile'],out,include_unslotted=True);require(ordered is not None and len(ordered)==len(fn['callOrder']),'source/native heads do not pair')
    literal_by_site[f'0x{name_ctor.address:08x}']=other['name']
    for entry in ordered:
        source_site=entry[3]['site'].lower()
        if source_site in literal_by_site:
            args=entry[2]
            try:literal=json.loads(args[1]) if len(args)==3 else None
            except (ValueError,TypeError):literal=None
            require(len(args)==3 and args[2].strip()=='-1' and literal==literal_by_site[source_site],'native/source CString literal changed')
    out['reviewedRetainedCStringSites']=[site for site,record in missing.items() if record.get('kind')=='vtable']
    out['reviewedMissingCallMetadata']=dict(owner=owner,addedCalls=len(missing),pairedHeads=len(ordered),frameBias=bias,caseScopes=case_reviews,
        contract='Normal saved-register/child stack ABI, literal CString4-byte footprint and existing valid participant storage. Scope only; no real heap/thread/conversation ownership, exceptions/reentry/gameplay proof.')
    return out

def scoped_function_evidence(method):
    @wraps(method)
    def wrapped(self,unit,*args,**kwargs):
        rows=unit.get('reviewedMissingCallMetadata',{})
        # A caller may narrow a registered unit to one owner/function. Reviews
        # for excluded functions must not validate or alter that conversion.
        owners=[unit.get('quest',{}),*unit.get('entities',{}).values()]
        selected={str(fn['address']).lower() for owner in owners
                  for group in ['functions','helpers'] for fn in owner.get(group,{}).values()
                  if isinstance(fn,dict) and fn.get('address')}
        rows={address:row for address,row in rows.items() if address.lower() in selected}
        if not rows:return method(self,unit,*args,**kwargs)
        old=self.by_address;new=dict(old)
        for address,row in rows.items():
            if address.lower() not in old:raise ValueError('reviewed missing calls: function absent')
            new[address.lower()]=augment(old[address.lower()],row,self.rdata,unit,{int(a,16) for a in old})
        self.by_address=new
        had_reviews=hasattr(self,'retained_cstring_reviews')
        old_reviews=getattr(self,'retained_cstring_reviews',None)
        self.retained_cstring_reviews=[]
        try:return method(self,unit,*args,**kwargs)
        finally:
            self.by_address=old
            if had_reviews:self.retained_cstring_reviews=old_reviews
            else:del self.retained_cstring_reviews
    return wrapped
