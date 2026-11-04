"""Restore a reviewed master integer equality and float32 online-score call."""
import hashlib
import re
from capstone import Cs, CS_ARCH_X86, CS_MODE_32, x86


def restore(source,todo,fn,unit,rd):
    row=unit.get('reviewedOnlineScoreCalls',{}).get(str(fn['address']).lower())
    if row is None:return source,todo
    def require(ok,why):
        if not ok:raise ValueError('Reviewed online score: '+why)
    source=source.replace('\r\n','\n').replace('\r','\n')
    start,end=int(fn['address'],16),int(fn['bodyEndExclusive'],16)
    raw=rd.bytes_at(start,end-start)
    for key,data in [('nativeSha256',raw),('sourceSha256',fn['decompile'].encode()),('luaSha256',source.encode())]:
        require(data is not None and hashlib.sha256(data).hexdigest()==row[key],key+' changed')
    require(unit.get('master',{}).get('fields',{}).get(hex(row['fieldOffset']))==[row['fieldName'],'Int'],
            'master field changed')
    integer=unit.get('reviewedMasterIntStores',{}).get(str(fn['address']).lower(),{})
    floating=unit.get('reviewedX87Thresholds',{}).get(str(fn['address']).lower(),{})
    require(integer.get('nativeSha256')==row['nativeSha256']
            and integer.get('fieldName')==row['fieldName'] and integer.get('fieldOffset')==row['fieldOffset']
            and integer.get('ownerOffset')==row['ownerOffset'] and integer.get('ownerRegister')==row['ownerRegister']
            and integer.get('valueRegister')==row['valueRegister'] and integer.get('valueLocal')==row['valueLocal'],
            'integer provenance changed')
    require(floating.get('nativeSha256')==row['nativeSha256'] and floating.get('floatSlot')==row['floatLocal']
            and floating.get('entrySlot')==row['floatEntrySlot'] and floating.get('frameBytes')==row['frameBytes'],
            'float32 provenance changed')
    require(source.count(row['floatAssignment'])==1 and row['floatAssignment'].startswith(row['floatLocal']+' = '),
            'float32 assignment changed')
    require(rd.bytes_at(row['vtable']+row['slot'],4)==row['target'].to_bytes(4,'little'), 'API slot target changed')
    require(row['slot']==0xb58 and row['target']==0x8a10c0, 'unreviewed API')
    require(len(row['dependencies'])==2 and {d['start'] for d in row['dependencies']}=={row['target'],0x55f3b0},
            'dependency inventory changed')
    for d in row['dependencies']:
        value=rd.bytes_at(d['start'],d['size'])
        require(value is not None and hashlib.sha256(value).hexdigest()==d['sha256'], 'API dependency changed')
    decoder=Cs(CS_ARCH_X86,CS_MODE_32);decoder.detail=True
    window=row['window'];data=rd.bytes_at(window['start'],window['end']-window['start'])
    require(data is not None and data.hex()==window['bytes'], 'caller window changed')
    seq=list(decoder.disasm(data,window['start']))
    require(len(seq)==12 and seq[-1].address+seq[-1].size==window['end'], 'caller shape changed')
    require([i.mnemonic for i in seq]==['mov','cmp','jne','mov','call','test','jne','mov','mov','mov','push','call'],
            'caller operation inventory changed')
    # Keep the original termination check and both destinations in place.
    return _restore_checked(source,todo,fn,row,seq,require)


def _restore_checked(source,todo,fn,row,seq,require):
    def reg(op,name):return op.type==x86.X86_OP_REG and op.reg==getattr(x86,'X86_REG_'+name.upper())
    def mem(op,base,offset):
        return op.type==x86.X86_OP_MEM and op.size==4 and op.mem.base==getattr(x86,'X86_REG_'+base.upper()) and not op.mem.index and op.mem.disp==offset
    owner,compare,skip,receiver,terminate,test,abort,game,argument,table,push,call=seq
    require(reg(owner.operands[0],'eax') and mem(owner.operands[1],row['ownerRegister'],row['ownerOffset']), 'master receiver changed')
    require(reg(compare.operands[0],row['valueRegister']) and mem(compare.operands[1],'eax',row['fieldOffset']), 'equality operands changed')
    require(skip.operands[0].type==x86.X86_OP_IMM and skip.operands[0].imm==row['window']['end'], 'unequal destination changed')
    require(reg(receiver.operands[0],'ecx') and reg(receiver.operands[1],row['ownerRegister']), 'termination receiver changed')
    require(terminate.operands[0].type==x86.X86_OP_IMM and terminate.operands[0].imm==row['terminationTarget'], 'termination API changed')
    require(all(reg(op,'al') for op in test.operands), 'termination boolean changed')
    require(abort.operands[0].type==x86.X86_OP_IMM and abort.operands[0].imm==row['abortTarget'], 'abort destination changed')
    require(reg(game.operands[0],'ecx') and mem(game.operands[1],row['ownerRegister'],4), 'game receiver changed')
    require(reg(argument.operands[0],'eax') and mem(argument.operands[1],'esp',row['frameBytes']+row['floatEntrySlot']), 'float argument slot changed')
    require(reg(table.operands[0],'edx') and mem(table.operands[1],'ecx',0) and reg(push.operands[0],'eax'), 'call operands changed')
    require(mem(call.operands[0],'edx',row['slot']), 'virtual call changed')
    require(all(re.fullmatch(r'[A-Za-z_]\w*',row[k]) for k in ['valueLocal','floatLocal','fieldName']), 'invalid source name')
    old=row['guardLines']
    require(len(old)==2 and old[1]=='if false then', 'unresolved guard shape changed')
    pattern=re.compile(r'(?m)^(?P<indent>[ \t]*)'+re.escape(old[0])+r'\n(?P=indent)'+re.escape(old[1])+r'$')
    require(len(pattern.findall(source))==1, 'unresolved equality changed')
    signed=f'(({row["valueLocal"]} + 2147483648) % 4294967296 - 2147483648)'
    source=pattern.sub(lambda m:m['indent']+f'if {signed} == quest:GetMasterGameState("{row["fieldName"]}") then',source)
    old=row['callLines']
    require(len(old)==2 and old[1]==f'quest:UpdateOnlineScore_ChickenKick({row["floatLocal"]})', 'unresolved call shape changed')
    pattern=re.compile(r'(?m)^(?P<indent>[ \t]*)'+re.escape(old[0])+r'\n(?P=indent)'+re.escape(old[1])+r'$')
    require(len(pattern.findall(source))==1, 'unresolved score call changed')
    source=pattern.sub(lambda m:m['indent']+f'resources:UpdateOnlineScore(0x{row["slot"]:x}, {row["floatLocal"]})',source)
    require(f'GetStateInt("self_{row["ownerOffset"]:#x}")' not in source, 'ambiguous remaining owner field')
    remaining=list(todo)
    for diagnostic in row['diagnostics']:
        require(remaining.count(diagnostic)==1, 'diagnostic inventory changed')
        remaining.remove(diagnostic)
    return source,remaining
