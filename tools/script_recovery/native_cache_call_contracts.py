"""Re-derive reviewed stack purges without treating receiver reloads as equal."""
import hashlib,struct
from capstone import Cs,CS_ARCH_X86,CS_MODE_32,x86 as x
from tools.script_recovery.annotate_interface_slots import VTABLE_BASE
from tools.script_recovery.native_register_snapshot_provenance import analyze
from tools.script_recovery.native_bounded_return_contracts import inspect

def resolve(image,raw,start,row,return_words,*,global_interface_addresses=()):
    def require(ok,why):
        if not ok:raise ValueError('native cache call contracts: '+why)
    md=Cs(CS_ARCH_X86,CS_MODE_32);md.detail=True;by={i.address:i for i in md.disasm(raw,start)}
    profile=row['memberProfile'];offset=profile['offset'];vtable=int(profile['vtable'],16)
    require(0<offset<0x1000 and offset%4==0,'invalid member offset')
    require(offset!=row['cacheProof']['interfaceOffset'],'member profile overlaps interface field')
    regions={}
    for name,witness in profile['witnesses'].items():
        address=int(witness['address'],16);data=bytes.fromhex(witness['bytes'])
        require(0<len(data)<=256 and image.bytes_at(address,len(data))==data,'member '+name+' bytes changed')
        regions[name]=list(md.disasm(data,address))
        require(sum(i.size for i in regions[name])==len(data),'incomplete member '+name)
    field=str(offset) if offset<10 else hex(offset)
    require((regions['factory'][0].mnemonic,regions['factory'][0].op_str)==('lea','ecx, [esi + '+field+']'),'factory member address changed')
    constructor_alias=profile.get('constructorReceiverAlias','esi')
    if constructor_alias=='esi':
        require(any((i.mnemonic,i.op_str)==('mov','dword ptr [esi], '+hex(vtable)) for i in regions['constructor']),'member constructor vtable changed')
    elif constructor_alias=='eax':
        require([(i.mnemonic,i.op_str) for i in regions['constructor'][:3]]==[
            ('mov','eax, ecx'),('mov','ecx, dword ptr [esp + 4]'),('mov','dword ptr [eax], '+hex(vtable))],'copy constructor entry-this alias changed')
        factory=regions['factory'];target=int(profile['witnesses']['constructor']['address'],16)
        require(len(factory)>=2 and factory[-1].mnemonic=='call' and factory[-1].operands[0].type==x.X86_OP_IMM
            and factory[-1].operands[0].imm==target,'factory/copy-constructor link changed')
        for i in factory[1:-1]:
            require(not any(i.group(g) for g in [x.X86_GRP_CALL,x.X86_GRP_JUMP,x.X86_GRP_RET])
                and not ({'ecx','cx','cl','ch'} & {i.reg_name(r) for r in i.regs_access()[1]}),'factory member receiver changed before constructor')
    else:raise ValueError('native cache call contracts: unsupported constructor receiver alias')
    options=dict(interface_offset=row['cacheProof']['interfaceOffset'],member_offsets=(offset,))
    if global_interface_addresses:options['global_interface_addresses']=global_interface_addresses
    if row['cacheProof'].get('branchTables'):options.update(branch_tables=row['cacheProof']['branchTables'],image=image)
    states=analyze(raw,start,**options)['states']
    records=row['callContracts'];require(isinstance(records,list) and records,'missing reviewed calls')
    require(len({r['site'] for r in records})==len(records),'duplicate reviewed call')
    budgets=row.get('boundedReturnInstructionBudgets',{})
    require(isinstance(budgets,dict) and len(budgets)<=256,'invalid bounded-return budget inventory')
    for key,budget in budgets.items():
        try:canonical=hex(int(key,16))
        except (TypeError,ValueError):raise ValueError('native cache call contracts: invalid bounded-return target')
        require(key==canonical and key in row['boundedReturnRegions'] and type(budget)is int and 1<=budget<=65536,'invalid bounded-return budget')
    used_budgets=set()
    summaries={};reports=[]
    for record in records:
        a=int(record['site'],16);require(a in by and by[a].mnemonic=='call','missing native call')
        i=by[a];op=i.operands[0];state=states.get(a,{})
        require(i.bytes.hex()==record['instructionBytes'],'native call bytes changed')
        target=int(record['target'],16);table=None
        if op.type==x.X86_OP_IMM:
            require(record['family']=='direct' and op.imm==target,'direct target changed')
            require('receiverValueEquivalenceProved' in record and (record['receiverValueEquivalenceProved'] is None or record['receiverValueEquivalenceProved'] is False),'unproved direct-call receiver equality')
        else:
            require(op.type==x.X86_OP_MEM and not op.mem.index and not op.mem.segment,'unsupported indirect operand')
            origin=state.get(i.reg_name(op.mem.base));require(origin is not None,'missing table provenance')
            if record['family']=='GSI_profile':
                require(origin[0]=='table_snapshot','not an interface table snapshot')
                table=VTABLE_BASE;equivalent=state.get('ecx')==('interface_snapshot',origin[1])
            else:
                require(record['family']=='constructor_member_profile' and origin[0]=='member_table_snapshot'
                    and origin[1]==offset and state.get('ecx')==('member_address',offset),'member receiver/table changed')
                table=vtable;equivalent=True
            require(record['receiverValueEquivalenceProved'] is equivalent,'receiver provenance claim changed')
            require(record['vtable']==hex(table) and record['slot']==hex(op.mem.disp),'vtable slot changed')
            require(image.bytes_at(table+op.mem.disp,4)==struct.pack('<I',target),'static target profile changed')
        prefix=bytes.fromhex(record['calleePrefix'])
        require(len(prefix)>=16 and image.bytes_at(target,len(prefix))==prefix,'callee prefix changed')
        bounded=row['boundedReturnRegions'].get(hex(target));proof=None
        if bounded:
            limits=[]
            for region in bounded:
                first,last=int(region['start'],16),int(region['endExclusive'],16)
                data=image.bytes_at(first,last-first)
                require(data is not None and hashlib.sha256(data).hexdigest()==region['sha256'],'bounded return region changed')
                limits.append((first,last))
            target_key=hex(target)
            extra=dict(max_instructions=budgets[target_key]) if target_key in budgets else {}
            proof=inspect(image,target,limits,**extra);purge=proof['purgeBytes']
            if target_key in budgets:used_budgets.add(target_key)
        else:
            words=return_words(target);require(words is not None,'missing original return evidence')
            purge=4*words
        require(purge==record['purgeBytes'],'native purge changed')
        summaries[a]=dict(purgeBytes=purge,source='bounded_native_RET' if proof else 'original_RET_sweep')
        reports.append(dict(site=hex(a),target=hex(target),purgeBytes=purge,profile=record['family'],
            receiverValueEquivalenceProved=record['receiverValueEquivalenceProved'],
            opaqueChildStackBalanceAssumed=True,currentVtableLifetimeVerified=False))
        if hex(target) in budgets:reports[-1]['boundedReturnInstructionBudget']=budgets[hex(target)]
        if record['family']=='constructor_member_profile' and constructor_alias=='eax':reports[-1]['constructorReceiverAlias']='eax'
    require(used_budgets==set(budgets),'unused bounded-return budget')
    return summaries,reports
