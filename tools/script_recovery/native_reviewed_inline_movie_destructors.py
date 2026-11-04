"""Recover reviewed movie destruction split into member and base destructor calls."""
import hashlib
import re
from capstone import Cs, CS_ARCH_X86, CS_MODE_32, x86


def restore(source, todo, fn, unit, rd):
    row=unit.get('reviewedInlineMovieDestructors',{}).get(str(fn['address']).lower())
    if row is None:return source,todo
    def require(ok,why):
        if not ok:raise ValueError('Reviewed inline movie destructor: '+why)
    source=source.replace('\r\n','\n').replace('\r','\n')
    start,end=int(fn['address'],16),int(fn['bodyEndExclusive'],16)
    raw=rd.bytes_at(start,end-start)
    for key,data in [('nativeSha256',raw),('sourceSha256',fn['decompile'].encode()),('luaSha256',source.encode())]:
        require(data is not None and hashlib.sha256(data).hexdigest()==row[key],key+' changed')
    cs=Cs(CS_ARCH_X86,CS_MODE_32);cs.detail=True
    ins=list(cs.disasm(raw,start));index={i.address:i for i in ins}
    require(ins and ins[-1].address+ins[-1].size==end,'incomplete body')
    def reg(op,name):return op.type==x86.X86_OP_REG and op.reg==getattr(x86,'X86_REG_'+name.upper())
    def mem(op,base,disp):return op.type==x86.X86_OP_MEM and op.mem.base==getattr(x86,'X86_REG_'+base.upper()) and not op.mem.index and op.mem.disp==disp
    def direct(address,target):
        i=index[address]
        require(i.mnemonic=='call' and i.operands[0].type==x86.X86_OP_IMM and i.operands[0].imm==target,'destructor target changed')
    require(ins[0].mnemonic=='sub' and reg(ins[0].operands[0],'esp'),'frame prologue')
    frame=ins[0].operands[1].imm
    for i in ins[1:]:
        if i.mnemonic=='call':break
        if i.mnemonic=='push':
            require(i.operands[0].type==x86.X86_OP_REG,'ambiguous prologue');frame+=4
        else:require(i.mnemonic in ['mov','lea','xor'] and x86.X86_REG_ESP not in i.regs_access()[1],'ambiguous prologue')
    require(frame==row['frameBytes'],'frame changed')
    require(set(row['dependencies'])=={'movie','member','base'},'dependency inventory')
    for dependency in row['dependencies'].values():
        data=rd.bytes_at(dependency['start'],dependency['end']-dependency['start'])
        require(data is not None and hashlib.sha256(data).hexdigest()==dependency['sha256'],'destructor dependency changed')
    require(row['dependencies']['movie']['start']==0x6e7b80 and row['dependencies']['member']['start']==0x6e7ab0 and row['dependencies']['base']['start']==0x99a430,'unsupported destructor family')
    require(len(row['objects'])==2 and len({o['local'] for o in row['objects']})==2,'object inventory')
    replacements=[]
    for obj in row['objects']:
        name=obj['local'];offset=frame+obj['entrySlot']
        require(name==f'xStack_{-obj["entrySlot"]:x}','movie local identity')
        ctor=index[obj['constructorLea']]
        require(ctor.mnemonic=='lea' and reg(ctor.operands[0],'ecx') and mem(ctor.operands[1],'esp',offset),'constructor receiver')
        direct(obj['constructorCall'],0x99a380)
        receiver=index[obj['startReceiver']];call=index[obj['startCall']]
        require(receiver.mnemonic=='lea' and reg(receiver.operands[0],'eax') and mem(receiver.operands[1],'esp',offset),'movie start receiver')
        require(call.mnemonic=='call' and mem(call.operands[0],'edx',0x5c8),'movie start slot')
        window=obj['constructionWindow'];data=rd.bytes_at(window['start'],window['end']-window['start'])
        require(data is not None and hashlib.sha256(data).hexdigest()==window['sha256'],'construction changed')
        require(source.count(name+' = resources:StartMovie("")')==obj['creationCount']==2,'Lua movie creation')
        require(len(obj['paths'])==2,'cleanup path inventory')
        for path in obj['paths']:
            window=path['window'];data=rd.bytes_at(window['start'],window['end']-window['start'])
            require(data is not None and hashlib.sha256(data).hexdigest()==window['sha256'],'cleanup window changed')
            member=index[path['memberLea']];base=index[path['baseLea']];vtable=index[path['vtableStore']]
            require(member.mnemonic=='lea' and reg(member.operands[0],'ecx') and mem(member.operands[1],'esp',offset+8),'member receiver')
            direct(path['memberCall'],0x6e7ab0)
            require(base.mnemonic=='lea' and reg(base.operands[0],'ecx') and mem(base.operands[1],'esp',offset),'base receiver')
            require(vtable.mnemonic=='mov' and mem(vtable.operands[0],'esp',offset) and vtable.operands[1].type==x86.X86_OP_IMM and vtable.operands[1].imm==0x126008c,'base vtable store')
            direct(path['baseCall'],0x99a430)
            if path['jump'] is not None:
                jump=index[path['jump']]
                require(jump.mnemonic=='jmp' and jump.operands[0].type==x86.X86_OP_IMM and jump.operands[0].imm==path['baseCall'],'shared base destructor edge')
        require(re.fullmatch(r'[A-Za-z_]\w*',row['alias']) is not None,'alias name')
        pattern=re.compile(r'(?m)^(?P<indent>[ \t]*)'+re.escape(row['alias']+' = '+name)+r'$')
        matches=list(pattern.finditer(source));require(len(matches)==obj['sourceCount']==2,'emitted destructor inventory')
        for match in matches:
            previous=source[:match.start()].rstrip().splitlines()[-1].strip()
            require(previous=='quest:PauseAllNonScriptedEntities(false)','cleanup order changed')
            replacements.append((match.start(),match.end(),match['indent']+f'resources:DestroyMovie({name})'))
    for a,b,value in sorted(replacements,reverse=True):source=source[:a]+value+source[b:]
    return source,todo
