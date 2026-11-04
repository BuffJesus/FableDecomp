"""Recover scalar snapshots lost when an EH flag word was mistaken for a string."""
import hashlib
import re
from capstone import Cs,CS_ARCH_X86,CS_MODE_32,x86


def restore(source,todo,fn,unit,rd):
    row=unit.get('reviewedStackFlagUpdates',{}).get(str(fn['address']).lower())
    if row is None:return source,todo
    def require(ok,why):
        if not ok:raise ValueError('Reviewed stack flag: '+why)
    source=source.replace('\r\n','\n').replace('\r','\n')
    start,end=int(fn['address'],16),int(fn['bodyEndExclusive'],16)
    raw=rd.bytes_at(start,end-start)
    for key,data in [('nativeSha256',raw),('sourceSha256',fn['decompile'].encode()),('luaSha256',source.encode())]:
        require(data is not None and hashlib.sha256(data).hexdigest()==row[key],key+' changed')
    decoder=Cs(CS_ARCH_X86,CS_MODE_32);decoder.detail=True
    ins=list(decoder.disasm(raw,start));index={i.address:i for i in ins}
    require(ins and ins[-1].address+ins[-1].size==end,'incomplete native body')
    def reg(op,name):
        return op.type==x86.X86_OP_REG and op.reg==getattr(x86,'X86_REG_'+name.upper())
    def mem(op,base,offset):
        return op.type==x86.X86_OP_MEM and op.size==4 and op.mem.base==getattr(x86,'X86_REG_'+base.upper()) and not op.mem.index and op.mem.disp==offset
    require(ins[0].mnemonic=='sub' and reg(ins[0].operands[0],'esp') and ins[0].operands[1].type==x86.X86_OP_IMM,'unsupported frame')
    frame=ins[0].operands[1].imm
    for i in ins[1:]:
        if i.mnemonic=='call':break
        if i.mnemonic=='push':
            require(i.operands[0].type==x86.X86_OP_REG,'ambiguous prologue push');frame+=4
        else:require(i.mnemonic in ['mov','lea','xor'] and x86.X86_REG_ESP not in i.regs_access()[1],'ambiguous prologue')
    require(frame==row['frameBytes'],'frame changed')
    zero,initial=index[row['zeroRegister']],index[row['initialStore']]
    require(zero.mnemonic=='xor' and all(reg(op,row['valueRegister']) for op in zero.operands),'zero seed changed')
    require(initial.mnemonic=='mov' and mem(initial.operands[0],'esp',frame+row['entrySlot'])
            and reg(initial.operands[1],row['valueRegister']),'flag initializer slot changed')
    require(all(i.mnemonic in ['push','lea'] and getattr(x86,'X86_REG_'+row['valueRegister'].upper()) not in i.regs_access()[1]
                for i in ins if zero.address<i.address<initial.address),'zero seed clobbered')
    window=row['window'];seq=[i for i in ins if window['start']<=i.address<window['end']]
    require(len(seq)==8 and seq[0].address==window['start'] and seq[-1].address+seq[-1].size==window['end']
            and b''.join(i.bytes for i in seq).hex()==window['bytes'],'update window changed')
    require([i.mnemonic for i in seq]==['mov','mov','lea','push','or','mov','mov','call'],'update operations changed')
    load,table,arg,push,bit,receiver,store,call=seq
    require(reg(load.operands[0],row['valueRegister']) and mem(load.operands[1],'esp',frame+row['entrySlot']),'flag source changed')
    require(reg(bit.operands[0],row['valueRegister']) and bit.operands[1].type==x86.X86_OP_IMM
            and bit.operands[1].imm==row['mask'] and 0<row['mask']<=0xffffffff,'flag mask changed')
    require(reg(arg.operands[0],'ecx') and mem(arg.operands[1],'esp',row['nameStringOffset'])
            and reg(push.operands[0],'ecx'),'pushed name argument changed')
    require(mem(store.operands[0],'esp',frame+row['entrySlot']+4) and reg(store.operands[1],row['valueRegister']),'flag destination changed')
    require(reg(table.operands[0],'eax') and mem(table.operands[1],row['receiverRegister'],0)
            and reg(receiver.operands[0],'ecx') and reg(receiver.operands[1],row['receiverRegister'])
            and mem(call.operands[0],'eax',row['messageSlot']),'message receiver changed')
    cleanup=rd.bytes_at(row['cleanupStart'],row['cleanupEnd']-row['cleanupStart'])
    require(cleanup is not None and hashlib.sha256(cleanup).hexdigest()==row['cleanupSha256'],'temporary cleanup changed')
    require(len(row['destructors'])==3,'cleanup inventory changed')
    for address in row['destructors']:
        i=index[address]
        require(i.mnemonic=='call' and i.operands[0].type==x86.X86_OP_IMM and i.operands[0].imm==0x99eae0,'string cleanup target changed')
    snapshot,updated,word=(row[k] for k in ['snapshotLocal','updatedLocal','wordLocal'])
    require(len({snapshot,updated,word})==3 and all(re.fullmatch(r'[A-Za-z_]\w*',s) for s in [snapshot,updated,word]),'invalid aliases')
    require(word==f'C_stk_{-row["entrySlot"]:x}','word local slot changed')
    provenance=unit.get('reviewedMasterIntStores',{}).get(str(fn['address']).lower(),{})
    require(provenance.get('valueRegister')==row['valueRegister'] and provenance.get('valueLocal')==snapshot
            and provenance.get('nativeSha256')==row['nativeSha256'],'register snapshot identity changed')
    for name in [snapshot,updated,word]:
        require(re.search(r'(?m)^    local [^\n]*\b'+re.escape(name)+r'\b',source),'alias is not local')
    require(source.count(word+' = 0\n')==1,'flag initialization changed')
    for line in row['aliasUses']:
        require(sum(s.strip()==line for s in source.splitlines())==1,'scalar alias use changed')
    require(any(line==f'{updated} = {snapshot} | 3' for line in row['aliasUses']),'missing subsequent snapshot use')
    require(any(line==f'if ({updated} & 1) ~= 0 then' for line in row['aliasUses']),'missing updated mask use')
    comment='-- TODO(native): '+row['unresolved']
    pattern=re.compile(r'(?m)^(?P<indent>[ \t]*)'+re.escape(comment)+r'$')
    require(len(pattern.findall(source))==1,'unresolved update changed')
    diagnostic='unlifted: '+row['unresolved'];require(todo.count(diagnostic)==1,'diagnostic changed')
    # These are distinct SSA values of the same native register. Normalize the
    # loaded word to unsigned32 before applying the original OR and storing it.
    replacement=[f'{snapshot} = {word} % 4294967296',f'{updated} = {snapshot} | {row["mask"]}',f'{word} = {updated}']
    source=pattern.sub(lambda m:('\n'+m['indent']).join([m['indent']+replacement[0]]+replacement[1:]),source)
    return source,[t for t in todo if t!=diagnostic]
