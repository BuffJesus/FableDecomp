"""Classify dead Lua aliases for evidenced immutable CString-vector base loads."""
import hashlib,re
from capstone import Cs,CS_ARCH_X86,CS_MODE_32
from capstone import x86 as x
from tools.script_recovery.readable_lua import tokens
from tools.script_recovery.annotate_interface_slots import VTABLE_BASE


def classify(source,todo,fn,row,unit,owner,spec,image,resource_evidence,liveness):
    if row is None:return source,list(todo),[]
    def require(ok,why):
        if not ok:raise ValueError('native definition staging '+fn['address']+': '+why)
    digest=lambda data:hashlib.sha256(data).hexdigest()
    canonical=lambda data:json_bytes(data)
    start,end=int(fn['address'],16),int(fn['bodyEndExclusive'],16)
    raw=image.bytes_at(start,end-start)
    require(raw is not None and digest(raw)==row['nativeSha256'],'native extent changed')
    require(digest(fn['decompile'].encode())==row['sourceSha256'],'exported source changed')
    require(digest(source.encode())==row['luaPhaseSha256'],'lifted phase changed')
    require(spec.entity and owner==row['owner'] and owner in unit.get('entities',{}),'owner changed')
    field=row['vectorOffset'];index_offset=row['indexOffset']
    definition=getattr(spec,'parent_string_snapshots',{}).get(field)
    require(definition is not None and digest(canonical(definition))==row['definitionSha256'],'definition snapshot changed')
    require(definition['element']=='CCharString' and definition['storage']=='questStringVector',
            'not an immutable definition string snapshot')
    require(not any(key in definition for key in ('writers','writeFunctions','mutations')),'mutable snapshot')
    require(definition['initialization']['stride']==4,'wrong element stride')
    readers={int(address,16) for address in definition['readFunctions']}
    initializers={int(address,16) for address in definition['initialization']['functions']}
    require(start in readers and start not in initializers,'not a declared readonly consumer')
    init=int(unit['quest']['functions']['Init']['address'],16)
    require(init in initializers,'quest initializer identity changed')
    index=spec.parent_fields.get(index_offset)
    require(index is not None and list(index)==row['indexField'] and index[1]=='Int','index field changed')
    require(row['consumer']=='IsObjectInThingsPossession','unsupported consumer')
    for slot,key in [(0x118,'heroTarget'),(0x2e0,'consumerTarget')]:
        target=image.bytes_at(VTABLE_BASE+slot,4)
        require(target is not None and int.from_bytes(target,'little')==int(row[key],16),'interface target changed')
    facts=resource_evidence(image,fn,spec,include_member_loads=True)
    site=int(row['nativeRead'],16)
    require(facts.get(site)==dict(family='parent',offset=field,kind='member_load'),'parent field load unproved')
    md=Cs(CS_ARCH_X86,CS_MODE_32);md.detail=True
    instructions=list(md.disasm(raw,start));by={i.address:i for i in instructions}
    require(sum(i.size for i in instructions)==len(raw),'incomplete native extent')
    span=[]
    for witness in row['span']:
        instruction=by.get(int(witness['address'],16))
        require(instruction is not None and instruction.bytes.hex()==witness['bytes'],'native witness changed')
        span.append(instruction)
    require(span and all(a.address+a.size==b.address for a,b in zip(span,span[1:])),'noncontiguous consumer span')
    number=lambda value:str(value) if 0<=value<10 else hex(value)
    expected=[('mov','eax, dword ptr [esi + '+number(spec.parent_off)+']'),
        ('mov','ecx, dword ptr [eax + '+number(index_offset)+']'),
        ('mov','edx, dword ptr [eax + '+number(field)+']'),('lea','ebp, [edx + ecx*4]'),
        ('mov','ecx, dword ptr [esi + 4]'),('mov','edx, dword ptr [ecx]'),('mov','eax, ecx'),
        ('mov','ebx, dword ptr [eax]'),('call','dword ptr [edx + 0x118]'),
        ('mov','ecx, dword ptr [esi + 4]'),('push','eax'),('push','ebp'),('call','dword ptr [ebx + 0x2e0]')]
    require([(i.mnemonic,i.op_str) for i in span]==expected and span[2].address==site,'field/index/consumer shape changed')
    for instruction in instructions:
        if instruction.group(x.X86_GRP_JUMP):
            require(instruction.operands[0].type==x.X86_OP_IMM,'unknown branch target')
            require(not span[0].address<instruction.operands[0].imm<=span[-1].address,'branch bypasses operand definitions')
    for call,slot in [(span[8],0x118),(span[-1],0x2e0)]:
        metadata=[item for item in fn['indirectCalls'] if int(item['site'],16)==call.address]
        require(len(metadata)==1 and int(metadata[0]['slot'],16)==slot,'indirect-call identity changed')
    alias=row['alias'];diagnostic=row['diagnostic'];marker=row['marker']
    require(all(re.fullmatch('[A-Za-z_]\\w*',name) for name in [alias,row['indexAlias'],row['heroAlias'],row['resultAlias']]),
            'invalid Lua alias')
    require(diagnostic.startswith('unlifted: '+alias+' = ') and marker.startswith(diagnostic[10:]),'diagnostic/marker mismatch')
    require(todo.count(diagnostic)==1,'diagnostic multiplicity changed')
    marker_pattern=re.compile(r'(?m)^[ \t]*-- TODO\(native\): '+re.escape(marker)+r'$')
    require(len(list(marker_pattern.finditer(source)))==1,'marker multiplicity changed')
    require(fn['decompile'].count(row['nativeSourceFragment'])==1,'ambiguous native source fragment')
    vector='quest:GetStateStringVectorAt("'+definition['name']+'", '+row['indexAlias']+')'
    before=row['indexAlias']+' = quest:GetStateInt("'+index[0]+'")'
    hero=row['heroAlias']+' = quest:GetHero()'
    consumer=row['resultAlias']+' = quest:IsObjectInThingsPossession('+vector+', '+row['heroAlias']+')'
    pattern=re.compile(r'(?m)^[ \t]*'+re.escape(before)+r'\n(?P<marker>[ \t]*-- TODO\(native\): '+re.escape(marker)+
        r'\n[ \t]*(?P<assignment>'+re.escape(alias)+r' = nil) --\[\[unresolved native value\]\])\n[ \t]*'+
        re.escape(hero)+r'\n[ \t]*'+re.escape(consumer)+r'$')
    matches=list(pattern.finditer(source));require(len(matches)==1,'ambiguous Lua field/index/consumer span')
    match=matches[0];proof=liveness(source,match.start('assignment'),alias)
    old=match['marker']
    # Preserve the original fragment as a reviewed annotation. The existing
    # entity-field presentation stage also scans this text; dropping it would
    # remove a legacy unused declaration and expand this change's token scope.
    replacement=old.replace('-- TODO(native): ','-- reviewed native definition-vector staging: ',1).replace(
        '--[[unresolved native value]]','--[[reviewed native definition-vector staging]]')
    out=source[:match.start('marker')]+replacement+source[match.end('marker'):]
    significant=lambda text:[(t.lastgroup,t[0]) for t in tokens(text) if t.lastgroup not in ('space','comment','longcomment')]
    require(significant(source)==significant(out),'executable Lua changed')
    remaining=list(todo);remaining.remove(diagnostic)
    return out,remaining,[dict(nativeRead=row['nativeRead'],field=facts[site],definition=definition['name'],
        indexField=index[0],consumer=row['consumer'],liveness=proof,executableLuaUnchanged=True,
        limits='Valid immutable string snapshot and valid indices; API/heap/exception/gameplay boundaries remain.')]


def json_bytes(data):
    import json
    return json.dumps(data,sort_keys=True,separators=(',',':')).encode()
