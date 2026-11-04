"""Preserve reviewed owned Thing call arguments across an intervening result overwrite."""
import hashlib,re
from capstone import Cs,CS_ARCH_X86,CS_MODE_32,x86


def restore(source,todo,fn,unit,rd):
    row=unit.get('reviewedSavedThingArguments',{}).get(str(fn['address']).lower())
    if row is None:return source,todo
    def require(ok,why):
        if not ok:raise ValueError('Reviewed saved Thing argument: '+why)
    source=source.replace('\r\n','\n').replace('\r','\n')
    start,end=int(fn['address'],16),int(fn['bodyEndExclusive'],16);raw=rd.bytes_at(start,end-start)
    for key,data in [('nativeSha256',raw),('sourceSha256',fn['decompile'].encode()),('luaSha256',source.encode())]:
        require(data is not None and hashlib.sha256(data).hexdigest()==row[key],key+' changed')
    cs=Cs(CS_ARCH_X86,CS_MODE_32);cs.detail=True
    ins=list(cs.disasm(raw,start));index={i.address:i for i in ins}
    require(ins and ins[-1].address+ins[-1].size==end,'incomplete body')
    def reg(op,name):return op.type==x86.X86_OP_REG and op.reg==getattr(x86,'X86_REG_'+name.upper())
    def mem(op,base,disp):return op.type==x86.X86_OP_MEM and op.mem.base==getattr(x86,'X86_REG_'+base.upper()) and not op.mem.index and op.mem.disp==disp
    require(ins[0].mnemonic=='sub' and reg(ins[0].operands[0],'esp'),'frame prologue')
    frame=ins[0].operands[1].imm
    for i in ins[1:]:
        if i.mnemonic=='call':break
        if i.mnemonic=='push':
            require(i.operands[0].type==x86.X86_OP_REG,'ambiguous prologue');frame+=4
        else:require(i.mnemonic in ['mov','lea','xor'] and x86.X86_REG_ESP not in i.regs_access()[1],'ambiguous prologue')
    require(frame==row['frameBytes'],'frame changed')
    require(set(row['dependencies'])=={'lookup','hero','teleport','thingDestructor','thingBase'},'dependency inventory')
    for window in [row['window'],*row['dependencies'].values()]:
        data=rd.bytes_at(window['start'],window['end']-window['start'])
        require(data is not None and hashlib.sha256(data).hexdigest()==window['sha256'],'native dependency changed')
    for name,slot,base in [('lookup',0x120,'ebx'),('hero',0x118,'edx'),('teleport',0x760,'ebx')]:
        i=index[row[name+'Call']]
        require(i.mnemonic=='call' and mem(i.operands[0],base,slot),'call slot changed')
        require(rd.bytes_at(row['vtable']+slot,4)==row['dependencies'][name]['start'].to_bytes(4,'little'),'callee target changed')
    lookup,hero,call=(index[row[k+'Call']] for k in ['lookup','hero','teleport'])
    saved,moving=index[row['savedPush']],index[row['movingPush']]
    require(all(i.mnemonic=='push' and reg(i.operands[0],'eax') for i in [saved,moving]),'result push changed')
    require(lookup.address<saved.address<hero.address<moving.address<call.address,'argument evaluation order')
    for a,b in [(lookup.address+lookup.size,saved.address),(hero.address+hero.size,moving.address)]:
        require(all(i.mnemonic=='mov' and x86.X86_REG_EAX not in i.regs_access()[1] for i in ins if a<=i.address<b),'result clobbered before push')
    flag=index[row['flagPush']]
    require(flag.mnemonic=='push' and flag.operands[0].type==x86.X86_OP_IMM and flag.operands[0].imm==0,'native boolean argument')
    buffer=index[row['resultLea']]
    require(buffer.mnemonic=='lea' and reg(buffer.operands[0],'ecx') and mem(buffer.operands[1],'esp',frame+row['resultEntrySlot']+8),'owned lookup result slot')
    destructor=index[row['destructorCall']];receiver=index[row['destructorLea']]
    require(receiver.mnemonic=='lea' and reg(receiver.operands[0],'ecx') and mem(receiver.operands[1],'esp',frame+row['resultEntrySlot']),'retirement receiver')
    require(destructor.mnemonic=='call' and destructor.operands[0].type==x86.X86_OP_IMM and destructor.operands[0].imm==row['dependencies']['thingDestructor']['start']==0x4aa840,'retirement target')
    require(call.address<receiver.address<destructor.address,'retirement order')
    literal=index[row['literalPush']]
    require(literal.mnemonic=='push' and literal.operands[0].type==x86.X86_OP_IMM and literal.operands[0].imm==row['literalAddress'],'lookup literal push')
    literal_bytes=row['literal'].encode('ascii')+b'\0'
    require(rd.bytes_at(row['literalAddress'],len(literal_bytes))==literal_bytes,'lookup literal changed')
    old=row['overwrittenLocal'];saved_name=row['savedLocal'];token=row['ownerToken']
    require(all(re.fullmatch(r'[A-Za-z_]\w*',s) for s in [old,saved_name,token]) and len({old,saved_name,token})==3,'invalid aliases')
    require(all(re.search(r'\b'+re.escape(s)+r'\b',source) is None for s in [saved_name,token]),'alias collision')
    flag_local=row['flagLocal'];require(re.fullmatch(r'[A-Za-z_]\w*',flag_local),'invalid flag alias')
    lines=[f'{old} = quest:GetThingWithScriptName("{row["literal"]}")',f'{old} = quest:GetHero()',f'quest:EntityTeleportToThing({old}, {old}, ({flag_local} ~= 0))']
    pattern=re.compile(r'(?m)^(?P<indent>[ \t]*)'+re.escape(lines[0])+r'\n(?P=indent)'+re.escape(lines[1])+r'\n(?P=indent)'+re.escape(lines[2])+r'$')
    matches=list(pattern.finditer(source));require(len(matches)==1,'ambiguous Lua argument block');m=matches[0]
    require(source[:m.start()].rstrip().splitlines()[-1].strip()==flag_local+' = 0x0','Lua boolean value changed')
    replacement=[f'{saved_name} = quest:GetThingWithScriptName("{row["literal"]}")',f'{token} = resources:TrackThingOutput({saved_name})',lines[1],f'quest:EntityTeleportToThing({old}, {saved_name}, ({flag_local} ~= 0))',f'resources:RetireThingOutput({token})',f'{saved_name} = nil']
    source=source[:m.start()]+('\n'+m['indent']).join([m['indent']+replacement[0]]+replacement[1:])+source[m.end():]
    anchor='    local resources = quest:RetailResources()\n';require(source.count(anchor)==1,'resource scope missing')
    source=source.replace(anchor,anchor+f'    local {saved_name}, {token}\n',1)
    return source,todo
