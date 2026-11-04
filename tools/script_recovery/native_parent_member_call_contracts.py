"""Re-derive reviewed stack purges without treating receiver reloads as equal."""
import hashlib,struct
from capstone import Cs,CS_ARCH_X86,CS_MODE_32,x86 as x
from tools.script_recovery.annotate_interface_slots import VTABLE_BASE
from tools.script_recovery.native_register_snapshot_provenance import analyze
from tools.script_recovery.native_bounded_return_contracts import inspect
from tools.script_recovery.native_saved_parent_table import prove as prove_saved_table

def resolve(image,raw,start,row,return_words,*,global_interface_addresses=()):
    def require(ok,why):
        if not ok:raise ValueError('native cache call contracts: '+why)
    md=Cs(CS_ARCH_X86,CS_MODE_32);md.detail=True;by={i.address:i for i in md.disasm(raw,start)}
    p=row['parentProfile'];offset=p['entityThingOffset'];vtable=int(p['entityThingVtable'],16);parent_offset=p['parentPointerOffset'];members=tuple(p['resourceOffsets']);resource_table=int(p['resourceVtable'],16)
    require(offset==8 and parent_offset==20 and members==(72,88) and vtable==0x1238c8c and resource_table==0x127094c,'supported parent/member layout')
    decoded={}
    for name,witness in p['witnesses'].items():
        a=int(witness['address'],16);data=bytes.fromhex(witness['bytes']);require(0<len(data)<=256 and image.bytes_at(a,len(data))==data,'initial layout witness '+name)
        code=list(md.disasm(data,a));require(sum(i.size for i in code)==len(data),'initial layout decode');decoded[name]=code
    require(set(decoded)=={'parent_constructor','entity_factory','parent_getter','resource_base_constructor','thing_copy'},'initial layout witness inventory')
    factory,constructor,getter=decoded['entity_factory'],decoded['parent_constructor'],decoded['parent_getter']
    require([(i.mnemonic,i.op_str) for i in getter]==[('mov','eax, dword ptr [ecx + '+hex(parent_offset)+']'),('ret','')],'parent getter field')
    require(any(i.mnemonic=='mov' and i.op_str=='ebx, edx' for i in factory) and any(i.mnemonic=='mov' and i.op_str=='dword ptr [esi + '+hex(parent_offset)+'], ebx' for i in factory),'entity parent link')
    require(any(i.mnemonic=='mov' and i.op_str=='dword ptr [esi], '+p['entityVtable'] for i in factory),'entity vtable link')
    thing=int(p['witnesses']['thing_copy']['address'],16);require(any(i.mnemonic=='lea' and i.op_str=='ecx, [esi + 8]' for i in factory) and any(i.mnemonic=='call' and i.operands[0].type==x.X86_OP_IMM and i.operands[0].imm==thing for i in factory),'entity Thing initialization')
    require(decoded['thing_copy'][0].mnemonic=='mov' and decoded['thing_copy'][0].op_str=='eax, ecx' and any(i.mnemonic=='mov' and i.op_str=='dword ptr [eax], '+hex(vtable) for i in decoded['thing_copy']),'Thing initial vtable')
    require(any(i.mnemonic=='mov' and i.op_str=='dword ptr [esi], '+p['parentVtable'] for i in constructor),'parent vtable link')
    member=None;initialized=[];base=int(p['witnesses']['resource_base_constructor']['address'],16);pending=False
    for i in constructor:
        if i.mnemonic=='lea' and i.operands[0].type==x.X86_OP_REG and i.operands[0].reg==x.X86_REG_EDI:
            op=i.operands[1];require(op.type==x.X86_OP_MEM and op.mem.base==x.X86_REG_ESI and not op.mem.index and op.mem.disp in members,'resource address');member=op.mem.disp
        if i.mnemonic=='call' and i.operands[0].type==x.X86_OP_IMM and i.operands[0].imm==base:pending=True
        if i.mnemonic=='mov' and i.op_str=='dword ptr [edi], '+hex(resource_table):require(member is not None and pending,'resource base link');initialized.append(member);pending=False
    require(initialized==list(members),'parent resource initialization inventory')
    options=dict(interface_offset=row['cacheProof']['interfaceOffset'],member_offsets=(offset,),member_table_loads=(offset,),parent_resource_profiles=((parent_offset,members),))
    if global_interface_addresses:options['global_interface_addresses']=global_interface_addresses
    states=analyze(raw,start,**options)['states']
    records=row['callContracts'];require(isinstance(records,list) and records,'missing reviewed calls')
    require(len({r['site'] for r in records})==len(records),'duplicate reviewed call')
    summaries={};reports=[]
    for record in records:
        a=int(record['site'],16);require(a in by and by[a].mnemonic=='call','missing native call')
        i=by[a];op=i.operands[0];state=states.get(a,{})
        require(i.bytes.hex()==record['instructionBytes'],'native call bytes changed')
        target=int(record['target'],16);table=None;cached=None
        if op.type==x.X86_OP_IMM:
            require(record['family']=='direct' and op.imm==target,'direct target changed')
        else:
            require(op.type==x.X86_OP_MEM and not op.mem.index and not op.mem.segment,'unsupported indirect operand')
            origin=state.get(i.reg_name(op.mem.base));require(origin is not None or record['family']=='parent_saved_table_profile','missing table provenance')
            if record['family']=='GSI_profile':
                require(origin[0]=='table_snapshot','not an interface table snapshot')
                table=VTABLE_BASE;equivalent=state.get('ecx')==('interface_snapshot',origin[1])
            elif record['family']=='parent_saved_table_profile':
                saved=row.get('savedParentTables',{}).get(hex(a));require(saved is not None and saved['parentOffset']==parent_offset and saved['memberOffset'] in members and int(saved['vtable'],16)==resource_table,'saved parent table profile')
                cached=prove_saved_table(image,raw,start,saved,states,records);require(cached['dispatch']==hex(a) and cached['expectedStaticTarget']==hex(target),'saved parent target');table=resource_table;equivalent=cached['receiverValueEquivalenceProved']
            elif record['family']=='parent_member_profile':
                require(origin[0]=='parent_member_table_snapshot' and origin[1]==parent_offset and origin[2] in members,'parent resource table provenance')
                equivalent=state.get('ecx')==('parent_member_address',origin[1],origin[2],origin[3])
                receiver=state.get('ecx');require(receiver is not None and receiver[:3]==('parent_member_address',origin[1],origin[2]),'parent resource receiver profile');table=resource_table
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
            proof=inspect(image,target,limits);purge=proof['purgeBytes']
        else:
            words=return_words(target);require(words is not None,'missing original return evidence')
            purge=4*words
        require(purge==record['purgeBytes'],'native purge changed')
        summaries[a]=dict(purgeBytes=purge,source='bounded_native_RET' if proof else 'reviewed_external_ABI_condition' if hex(target) in row.get('externalNormalABIs',{}) else 'original_RET_sweep')
        reports.append(dict(site=hex(a),target=hex(target),purgeBytes=purge,profile=record['family'],
            receiverValueEquivalenceProved=record['receiverValueEquivalenceProved'],
            opaqueChildStackBalanceAssumed=True,currentVtableLifetimeVerified=False))
        if cached is not None:reports[-1]['savedParentTable']=cached
    require(set(row.get('savedParentTables',{}))=={r['site'] for r in records if r['family']=='parent_saved_table_profile'},'saved parent table inventory')
    return summaries,reports
