"""Registered entry/frame proof, conditional on checked normal child ABI contracts."""
import hashlib
import capstone
from capstone import x86 as x
from tools.script_recovery.native_receiver_reaching import _cfg,_reaching

def prove_frame(raw,fn,image,*,entity_vtable,scope_start,scope_end,frame_depth,mask_offset,this_definition,actor_definition,actor_profile='entry-ebp',owner_register='esi'):
    start,end=int(fn['address'],16),int(fn['bodyEndExclusive'],16)
    assert len(raw)==end-start and start<scope_start<scope_end<=end
    assert int.from_bytes(image.bytes_at(entity_vtable+4,4),'little')==start
    md=capstone.Cs(capstone.CS_ARCH_X86,capstone.CS_MODE_32);md.detail=True;ins=list(md.disasm(raw,start));assert sum(i.size for i in ins)==len(raw);by={i.address:k for k,i in enumerate(ins)}
    raw_preds=_cfg(ins,x,capstone,image);assert raw_preds is not None
    succ={k:[] for k in raw_preds}
    for k,parents in raw_preds.items():
        for parent in parents:succ[parent].append(k)
    reachable=set();pending=[0]
    while pending:
        k=pending.pop()
        if k in reachable:continue
        reachable.add(k);pending.extend(succ[k])
    preds={k:[p for p in raw_preds[k] if p in reachable] if k in reachable else [] for k in raw_preds}
    calls={int(c['site'],16):c for c in fn['calls']+fn['indirectCalls']};assert len(calls)==len(fn['calls'])+len(fn['indirectCalls'])
    depths={0:0};contracts={};relations={k:[] for k in reachable}
    for k in sorted(reachable):
        i=ins[k];ops=i.operands;delta=0
        if i.mnemonic=='push':assert not(ops[0].type==x.X86_OP_REG and ops[0].reg==x.X86_REG_ESP);delta=4
        elif i.mnemonic=='pop':assert ops[0].reg!=x.X86_REG_ESP;delta=-4
        elif i.mnemonic in ['add','sub'] and ops[0].type==x.X86_OP_REG and ops[0].reg==x.X86_REG_ESP:
            assert ops[1].type==x.X86_OP_IMM;delta=ops[1].imm if i.mnemonic=='sub' else -ops[1].imm
        elif i.mnemonic=='lea' and ops[0].reg==x.X86_REG_ESP:
            assert ops[1].type==x.X86_OP_MEM and ops[1].mem.base==x.X86_REG_ESP and not ops[1].mem.index;delta=-ops[1].mem.disp
        elif i.mnemonic=='call':
            record=calls[i.address];assert isinstance(record['depth'],int);depths[k]=record['depth'];continue
        elif i.mnemonic=='ret':assert not ops;depths[k]=0
        else:assert x.X86_REG_ESP not in i.regs_access()[1],(hex(i.address),i.mnemonic,i.op_str)
        for target in succ[k]:
            if target not in reachable:continue
            relations[k].append((target,delta));relations[target].append((k,-delta))
    pending=list(depths)
    while pending:
        k=pending.pop()
        for target,delta in relations[k]:
            out=depths[k]+delta;assert out>=0
            if target in depths:assert depths[target]==out,(hex(ins[target].address),depths[target],out)
            else:depths[target]=out;pending.append(target)
    assert set(depths)==reachable
    for k in sorted(reachable):
        i=ins[k]
        if i.mnemonic!='call':continue
        assert len(succ[k])==1
        purge=depths[k]-depths[succ[k][0]];assert 0<=purge<=64 and purge%4==0
        record=calls[i.address];snapshot=record.get('pushedStack',[]);assert isinstance(snapshot,list)
        if i.operands[0].type==x.X86_OP_IMM and i.operands[0].imm in [0x99ebf0,0x99eae0,0xf35b30,0x99e4c0]:
            assert purge=={0x99ebf0:8,0x99eae0:0,0xf35b30:0,0x99e4c0:0}[i.operands[0].imm]
        if record.get('slot') in ['0x420','0x5b8','0x118']:
            assert purge=={'0x420':4,'0x5b8':20,'0x118':0}[record['slot']]
        contracts[hex(i.address)]=dict(exportedDepth=depths[k],derivedNormalPurge=purge,pendingStackSnapshotWords=len(snapshot))
    assert set(depths)==reachable and len(contracts)==len(calls)
    a,b=scope_start,scope_end;nodes={k for k in reachable if a<=ins[k].address<b};assert depths[by[a]]==frame_depth
    incoming=[(ins[p].address,ins[k].address) for k in nodes for p in preds[k] if p not in nodes];assert incoming and all(target==a for _,target in incoming)
    assert actor_profile in ('entry-ebp','local-edi','entry-ebx','entry-edi','owner-only')
    assert owner_register in ('esi','ebp')
    owner_id={'esi':x.X86_REG_ESI,'ebp':x.X86_REG_EBP}[owner_register]
    actor_id={'entry-ebp':x.X86_REG_EBP,'entry-ebx':x.X86_REG_EBX,'entry-edi':x.X86_REG_EDI}.get(actor_profile)
    assert actor_id is None or actor_id!=owner_id
    origins={}
    definitions_to_check=[(owner_id,this_definition)]+([] if actor_id is None else [(actor_id,actor_definition)])
    for register,expected in definitions_to_check:
        definitions=_reaching(ins,preds,by[a],register,x);assert -1 not in definitions and {ins[k].address for k in definitions}=={expected}
        origins[md.reg_name(register)]=dict(definition=hex(expected),instruction=ins[by[expected]].mnemonic+' '+ins[by[expected]].op_str)
    assert ins[by[this_definition]].op_str==owner_register+', ecx'
    if actor_profile=='owner-only':
        assert actor_definition is None
    else:
        actor_name={'entry-ebp':'ebp','entry-ebx':'ebx','entry-edi':'edi','local-edi':'edi'}[actor_profile]
        assert actor_profile!='local-edi' or a<=actor_definition<b
        assert ins[by[actor_definition]].op_str==actor_name+', ['+owner_register+' + 8]'
    assert not any(x.X86_REG_ECX in i.regs_access()[1] for i in ins[:by[this_definition]])
    return dict(start=start, end=end, md=md, ins=ins, by=by, preds=preds, reachable=reachable, calls=calls, depths=depths, contracts=contracts, a=a, b=b, incoming=incoming, origins=origins)


def prove(raw,fn,image,*,entity_vtable,scope_start,scope_end,frame_depth,mask_offset,this_definition,actor_definition,actor_profile='entry-ebp'):
    assert actor_profile!='owner-only', 'hit query proof requires an actor profile'
    ctx=prove_frame(raw,fn,image,entity_vtable=entity_vtable,scope_start=scope_start,scope_end=scope_end,frame_depth=frame_depth,mask_offset=mask_offset,this_definition=this_definition,actor_definition=actor_definition,actor_profile=actor_profile)
    start=ctx['start']
    end=ctx['end']
    md=ctx['md']
    ins=ctx['ins']
    by=ctx['by']
    preds=ctx['preds']
    reachable=ctx['reachable']
    calls=ctx['calls']
    depths=ctx['depths']
    contracts=ctx['contracts']
    a=ctx['a']
    b=ctx['b']
    incoming=ctx['incoming']
    origins=ctx['origins']
    mask=mask_offset-frame_depth;writes=[]
    for k in sorted(reachable):
        i=ins[k];ops=i.operands
        if not ops or ops[0].type!=x.X86_OP_MEM or not (ops[0].access&capstone.CS_AC_WRITE):continue
        mem=ops[0].mem
        if mem.base!=x.X86_REG_ESP or mem.index:continue
        offset=mem.disp-depths[k];width=ops[0].size
        if offset<mask+4 and mask<offset+width:
            assert offset==mask and width==4
            writes.append(dict(site=hex(i.address),entryOffset=offset,width=width,instruction=i.mnemonic+' '+i.op_str))
    outside=[row for row in writes if not a<=int(row['site'],16)<b];assert len(outside)==1
    init=by[int(outside[0]['site'],16)];zero=ins[init];assert zero.mnemonic=='mov'
    if zero.operands[1].type==x.X86_OP_IMM:
        assert zero.operands[1].imm==0
        definition=zero
    else:
        assert zero.operands[1].type==x.X86_OP_REG
        defs=_reaching(ins,preds,init,zero.operands[1].reg,x);assert len(defs)==1 and -1 not in defs
        definition=ins[next(iter(defs))];assert definition.mnemonic=='xor' and definition.operands[0].reg==definition.operands[1].reg==zero.operands[1].reg
    dom={k:({0} if k==0 else set(reachable)) for k in reachable};changed=True
    while changed:
        changed=False
        for k in sorted(reachable-{0}):
            value={k}|set.intersection(*(dom[p] for p in preds[k]));assert preds[k]
            if value!=dom[k]:dom[k]=value;changed=True
    assert init in dom[by[a]]

    return dict(status='passed_registered_entry_and_conditional_frame_mask_roles',nativeSha256=hashlib.sha256(raw).hexdigest(),registeredVtable=hex(entity_vtable),reachableInstructions=len(reachable),scopeEntry=hex(a),scopeDepth=frame_depth,normalIncomingEdges=[dict(source=hex(p),target=hex(q)) for p,q in incoming],origins=origins,maskEntryOffset=mask,maskInitializer=outside[0],maskZeroDefinition=hex(definition.address),maskInitializerDominates=True,maskDirectWrites=writes,childPurgeContracts=contracts,limits='Entry-reachable normal CFG and exported pre-call depth anchors. Derived purges remain explicit child ABI contracts, with disjoint output/object footprints and preserved caller locals. Indirect child writes, heap/ownership, exception/reentry/FP environment/gameplay not proved.')
