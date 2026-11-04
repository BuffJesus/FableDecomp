"""Classify dead aliases for restored resource dispatch through a retained table register."""
from collections import Counter
import hashlib,re
from capstone import Cs,CS_ARCH_X86,CS_MODE_32,x86
from tools.script_recovery.annotate_interface_slots import VTABLE_BASE,load_slots
from tools.script_recovery.readable_lua import tokens


def classify(source,todo,fn,row,spec,image,resource_evidence,liveness):
    if row is None:return source,list(todo),[]
    def require(ok,why):
        if not ok:raise ValueError('register dispatch staging '+fn['address']+': '+why)
    digest=lambda data:hashlib.sha256(data).hexdigest()
    require(getattr(image,'ok',False) and digest(image.data)==row['imageSha256'],'retail image changed')
    start,end=int(fn['address'],16),int(fn['bodyEndExclusive'],16)
    raw=image.bytes_at(start,end-start)
    require(raw is not None and digest(raw)==row['nativeSha256'],'native extent changed')
    require(digest(fn['decompile'].encode())==row['sourceSha256'],'exported source changed')
    require(digest(source.encode())==row['luaPhaseSha256'],'lifted phase changed')
    require(getattr(spec,'entity',False),'not an entity resource receiver')
    require(row['abiContract']==dict(ordinaryMsvcCalleeSaved=True,callerFramePreserved=True,
        exceptionsVerified=False,engineReentryVerified=False,heapVerified=False),'unsupported child ABI contract')
    require('ABI' in row['scopeLimit'] and 'unverified' in row['scopeLimit'],'missing proof limits')
    require(re.fullmatch('[0-9a-f]{64}',row['acceptedEvidenceSha256']) is not None,'missing bounded evidence seal')
    decoder=Cs(CS_ARCH_X86,CS_MODE_32);decoder.detail=True
    ins=list(decoder.disasm(raw,start));by={i.address:i for i in ins}
    require(ins and sum(i.size for i in ins)==len(raw),'incomplete caller instructions')
    getter=row['getter'];slot=int(getter['slot'],16);target=int(getter['target'],16)
    require(slot==0x118 and getter['method']=='GetHero' and load_slots().get(slot)=='GetHero','unsupported getter')
    require(image.bytes_at(VTABLE_BASE+slot,4)==target.to_bytes(4,'little'),'getter identity changed')
    body=bytes.fromhex(getter['bytes'])
    require(0<len(body)<=4096 and image.bytes_at(target,len(body))==body,'getter bytes changed')
    helper=list(decoder.disasm(body,target))
    require(helper and sum(i.size for i in helper)==len(body),'incomplete getter instructions')
    rets=[i for i in helper if i.group(x86.X86_GRP_RET)]
    require(rets and all(i.mnemonic=='ret' and not i.op_str for i in rets),'getter stack result changed')
    for i in helper:
        require(not i.group(x86.X86_GRP_INT) and not i.group(x86.X86_GRP_IRET),'unsupported getter transfer')
        if i.group(x86.X86_GRP_JUMP):
            require(i.operands[0].type==x86.X86_OP_IMM and target<=i.operands[0].imm<target+len(body),'getter tail transfer outside sealed body')
    facts=resource_evidence(image,fn,spec,include_resource_table_loads=True)
    reads=row['reads'];expected=Counter(read['diagnostic'] for read in reads)
    require(bool(reads) and all(todo.count(key)==count for key,count in expected.items()),'diagnostic multiplicity changed')
    require(len({read['nativeRead'] for read in reads})==len(reads),'duplicate native table read')
    aliases={'ebx':{'ebx','bx','bl','bh'},'ebp':{'ebp','bp'},'esi':{'esi','si'},'edi':{'edi','di'}}
    edits=[];reviews=[]
    for read in reads:
        site,dispatch,hero_site=(int(read[key],16) for key in ('nativeRead','dispatch','heroCall'))
        table_register=read['tableRegister'];require(table_register in aliases,'not a callee-saved table register')
        require(not any({i.reg_name(r) for r in i.regs_access()[1]}&aliases[table_register] for i in helper),'getter writes table register')
        table=facts.get(site);call=facts.get(dispatch)
        require(table is not None and table.get('kind')=='resource_vtable_load','unproved resource table load')
        require(table['family']=='self' and table['tableRegister']==table_register,'table family/register changed')
        offset=table['offset'];receiver=table['receiverRegister']
        require(spec.self_resources.get(offset)=='seh_me','not the entity actor resource')
        require(call is not None and call['family']=='self' and call['offset']==offset and call['slot']==0x34,'unproved restored Speak dispatch')
        pushes=call['pushes']
        require(len(pushes)==6 and tuple(pushes[0])==('hero',hero_site),'missing exact hero operand')
        require(all(part and part[0]=='imm' for part in pushes[1:]),'nonconstant speech operands')
        literal=image.string_at(pushes[1][1]);group=pushes[2][1];flags=[part[1] for part in pushes[3:]]
        require(literal==read['literal'] and group==read['group'] and flags==read['flags'],'speech operands changed')
        require(all(value in (0,1) for value in flags),'not native boolean flags')
        sequence=[i for i in ins if site<=i.address<=dispatch]
        expected_sequence=[('mov',table_register+', dword ptr ['+receiver+']')]
        expected_sequence.extend(('push',str(value)) for value in reversed([group,*flags]))
        expected_sequence.extend([('push',hex(pushes[1][1])),('call','dword ptr [edx + 0x118]'),
            ('push','eax'),('mov','ecx, '+receiver),('call','dword ptr ['+table_register+' + 0x34]')])
        require([(i.mnemonic,i.op_str) for i in sequence]==expected_sequence,'table/getter/dispatch sequence changed')
        require(all(a.address+a.size==b.address for a,b in zip(sequence,sequence[1:])),'noncontiguous table lifetime')
        require(by[hero_site].mnemonic=='call' and by[hero_site].op_str=='dword ptr [edx + 0x118]','getter call site changed')
        setup_index=ins.index(by[site]);setup=ins[setup_index-2:setup_index]
        require(len(setup)==2 and setup[0].mnemonic=='mov' and setup[0].op_str.startswith('ecx, dword ptr [')
            and setup[0].operands[1].type==x86.X86_OP_MEM and setup[0].operands[1].mem.disp==4
            and not setup[0].operands[1].mem.index and setup[1].mnemonic=='mov' and setup[1].op_str=='edx, dword ptr [ecx]',
            'getter receiver/table setup changed')
        owner=setup[0].operands[1].mem.base
        require(any(i.mnemonic=='mov' and len(i.operands)==2 and i.operands[0].type==x86.X86_OP_REG
            and i.operands[0].reg==owner and i.operands[1].type==x86.X86_OP_REG and i.operands[1].reg==x86.X86_REG_ECX
            for i in ins[:12]),'getter receiver not anchored to script')
        for i in ins:
            if i.group(x86.X86_GRP_JUMP):
                require(i.operands[0].type==x86.X86_OP_IMM,'unknown branch into table lifetime')
                require(not site<i.operands[0].imm<=dispatch,'branch bypasses table load')
        fragment=read['nativeSourceFragment'];require(bool(fragment) and fn['decompile'].count(fragment)==1,'ambiguous exported source fragment')
        alias=read['alias'];diagnostic=read['diagnostic'];region=read['luaRegion']
        require(re.fullmatch('[A-Za-z_]\\w*',alias) is not None and diagnostic.startswith('unlifted: '+alias+' = '),'invalid alias diagnostic')
        require(diagnostic[10:]+';' in fragment and '"'+literal+'"' in fragment,'source fragment correspondence changed')
        require(source.count(region)==1,'ambiguous Lua region')
        hero_alias=read['heroAlias'];result_alias=read['resultAlias']
        require(all(re.fullmatch('[A-Za-z_]\\w*',name) for name in (hero_alias,result_alias)),'invalid emitted aliases')
        booleans=', '.join('true' if value else 'false' for value in flags)
        hero_line=hero_alias+' = quest:GetHero()'
        speech=result_alias+' = me:Speak('+hero_alias+', "'+literal+'", '+str(group)+', '+booleans+')'
        pattern=re.compile(r'(?m)^[ \t]*-- TODO\(native\): '+re.escape(diagnostic[10:])+r'\n[ \t]*(?P<assignment>'+re.escape(alias)+
            r' = nil) --\[\[unresolved native value\]\]\n[ \t]*'+re.escape(hero_line)+r'\n[ \t]*'+re.escape(speech)+r'\n')
        matches=list(pattern.finditer(region));require(len(matches)==1,'restored Lua getter/Speak span changed')
        match=matches[0];begin=source.index(region)+match.start();end=source.index(region)+match.end()
        proof=liveness(source,source.index(region)+match.start('assignment'),alias)
        replacement=match[0].replace('-- TODO(native): ','-- reviewed native register-dispatch staging: ',1).replace(
            '--[[unresolved native value]]','--[[reviewed native register-dispatch staging]]',1)
        edits.append((begin,end,replacement))
        reviews.append(dict(nativeRead=read['nativeRead'],dispatch=read['dispatch'],resource=table,getter=hex(target),
            getterSha256=digest(body),liveness=proof,executableLuaUnchanged=True,limits=row['scopeLimit']))
    require(all(a[1]<=b[0] for a,b in zip(sorted(edits),sorted(edits)[1:])),'overlapping Lua edits')
    out=source
    for begin,end,replacement in sorted(edits,reverse=True):out=out[:begin]+replacement+out[end:]
    significant=lambda text:[(token.lastgroup,token[0]) for token in tokens(text) if token.lastgroup not in ('space','comment','longcomment')]
    require(significant(out)==significant(source),'executable Lua changed')
    remaining=list(todo)
    for read in reads:remaining.remove(read['diagnostic'])
    return out,remaining,reviews
