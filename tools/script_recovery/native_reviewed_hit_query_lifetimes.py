"""Recover reviewed three-query hero-hit scopes with native CString lifetimes.

Supported existing resource helper excludes ability14. Native child balance,
valid disjoint objects/heaps and in-bounds CString footprints remain contracts.
Allocator, exception, reentry and gameplay behavior are not inferred here.
"""
import hashlib,json,re
from capstone import Cs,CS_ARCH_X86,CS_MODE_32,x86 as x
from tools.script_recovery.native_hit_query_roles import prove
from tools.script_recovery.native_register_snapshot_provenance import analyze
from tools.script_recovery.readable_lua import tokens

METHODS=('MsgIsHitByHero','MsgIsHitByAnySpecialAbilityFromHero','MsgIsHitByHeroSpecialAbility')
PROFILE={
    'constructor':(0x99ebf0,61,'8c27b64a613d8319d3e866b8224ecfcedf8e5ccaf92a81314c16631d41f3f100'),
    'destructor':(0x99eae0,12,'65d790bd8e62fef3658c4d1320e81e8b1c6d54ab12879256c0e4e4c533c1c53e'),
    'release':(0x99e9b0,87,'e0ccad7278cb78743022e17eb02dad9b144f7132dc1538d5adb393a625d3e5de'),
}
def significant(text):return [(t.lastgroup,t[0]) for t in tokens(text) if t.lastgroup not in ('space','comment','longcomment')]
def masked(text):
    return ''.join(re.sub(r'[^\r\n]',' ',t[0]) if t.lastgroup in ('comment','longcomment','string','longstring') else t[0] for t in tokens(text))

def recover(source,todo,calls,fn,row,unit,owner,image,liveness):
    if row is None:return source,list(todo),list(calls),[]
    def require(ok,why):
        if not ok:raise ValueError('reviewed hit-query lifetimes '+fn['address']+': '+why)
    digest=lambda data:hashlib.sha256(data).hexdigest()
    require(owner==row['owner'] and owner in unit.get('entities',{}),'owner changed')
    entity=unit['entities'][owner]
    require(entity['vtable'].lower()==row['entityVtable'].lower() and entity['functions']['Main']['address'].lower()==fn['address'].lower(),'registered owner/function changed')
    start,end=int(fn['address'],16),int(fn['bodyEndExclusive'],16);raw=image.bytes_at(start,end-start)
    require(raw is not None and digest(raw)==row['nativeExtentSha256'],'native body changed')
    require(digest(fn['decompile'].encode())==row['sourceSha256'],'exported source changed')
    require(digest(source.encode())==row['luaPhaseSha256'],'lifted phase changed')
    require(digest(json.dumps(calls,separators=(',',':')).encode())==row['callsSha256'],'call accounting changed')
    require(row['method']=='IsHitByHeroExceptAbility' and row['ability']==14 and row['literal']=='SCRIPT_NAME_HERO','unsupported helper contract')
    literal=int(row['heroLiteral'],16);require(literal==0x125d1c8 and image.string_at(literal)==row['literal'],'hero literal changed')
    require(len(row['helperBodies'])==3 and {b['name'] for b in row['helperBodies']}==set(PROFILE),'helper identities changed')
    for body in row['helperBodies']:
        address,size,sha=PROFILE[body['name']];data=image.bytes_at(address,size)
        require((int(body['address'],16),body['size'],body['sha256'])==(address,size,sha),'unsupported CString profile')
        require(data is not None and digest(data)==sha and data.hex()==body['bytes'],'CString body changed')
    shape=row.get('shape','legacy-byte-mask')
    require(shape in ('legacy-byte-mask','shared-mask-bool'),'unsupported reviewed query shape')
    if shape=='shared-mask-bool':
        from tools.script_recovery.native_shared_hit_query_lifetimes import recover_shared
        try:
            return recover_shared(source,todo,calls,fn,row,unit,owner,image,liveness,raw)
        except (AssertionError,KeyError,TypeError,IndexError) as exc:
            raise ValueError('reviewed shared-mask hit-query evidence refused: '+str(exc)) from exc
    md=Cs(CS_ARCH_X86,CS_MODE_32);md.detail=True;ins=list(md.disasm(raw,start))
    require(ins and sum(i.size for i in ins)==len(raw),'incomplete native extent')
    first=ins[0];require(first.mnemonic=='sub' and first.operands[0].type==x.X86_OP_REG and first.operands[0].reg==x.X86_REG_ESP and first.operands[1].type==x.X86_OP_IMM,'unsupported frame prologue')
    depth=first.operands[1].imm;saved=set()
    for i in ins[1:12]:
        if i.mnemonic=='push' and i.operands[0].type==x.X86_OP_REG:
            register=i.operands[0].reg
            require(register in (x.X86_REG_EBX,x.X86_REG_EBP,x.X86_REG_ESI,x.X86_REG_EDI) and register not in saved,'unsupported saved-register prologue')
            saved.add(register);depth+=4
        else:require(i.mnemonic=='mov' and i.op_str=='esi, ecx','unsupported prologue instruction')
        if len(saved)==4:break
    require(len(saved)==4 and depth==row['frameDepth'],'balanced frame changed')
    states=analyze(raw,start)['states']
    from tools.script_recovery.convert_quest_unit import _text_order_sites
    ordered=_text_order_sites(fn['decompile'],fn);require(ordered is not None,'call/source alignment unavailable')
    clean=masked(source);require(len(re.findall(r'\bfunction\b',clean))==1,'nested function/closure')
    locals_=[]
    for names in re.findall(r'(?m)^[ \t]*local ([^=\n]+)',clean):locals_.extend(n.strip() for n in names.split(','))
    require(len(locals_)==len(set(locals_)),'duplicate local declaration')
    inline=re.compile(r'(?m)^(?P<i>[ \t]*)if (?P<c>(?:not )?[A-Za-z_]\w*) then break end$')
    expansions=list(inline.finditer(source));require(len(expansions)==row['analysisInlineBreaks'],'analysis control shape changed')
    analysis=inline.sub(lambda m:m['i']+'if '+m['c']+' then\n'+m['i']+'    break\n'+m['i']+'end',source)
    remaining=list(todo);edits=[];call_edits=[];reviews=[];scopes=row['scopes']
    require(scopes and len({s['start'] for s in scopes})==len(scopes),'empty/duplicate scopes')
    call_indices=[n for n in range(len(calls)-2) if list(calls[n:n+3])==list(METHODS)]
    for scope in scopes:
        a,b=int(scope['start'],16),int(scope['endExclusive'],16)
        require(start<=a<b<=end and digest(raw[a-start:b-start])==scope['nativeSha256'],'native scope changed')
        require(states.get(a,{}).get('esi')==('entry_this',),'unproved native this origin')
        entries=[]
        for i in ins:
            if not i.group(x.X86_GRP_JUMP):continue
            require(i.operands[0].type==x.X86_OP_IMM,'unknown normal branch entry')
            if a<=i.operands[0].imm<b and not a<=i.address<b:
                require(i.operands[0].imm==a,'branch bypasses query scope entry');entries.append(hex(i.address))
        native=prove(raw[a-start:b-start],a,constructor=PROFILE['constructor'][0],destructor=PROFILE['destructor'][0],
                     hero_literal=literal,object_offsets=tuple(scope['objectOffsets']),temporary_mask=scope['temporaryMask'])
        actual=[dict(site=site['site'].lower(),arguments=args,metadata=site) for _,_,args,site,_,_ in ordered if a<=int(site['site'],16)<b]
        require(len(actual)==9 and actual==scope['sourceCalls'],'native call/source correspondence changed')
        constructors=[c['metadata'] for c in actual if int(c['metadata'].get('target','0'),16)==PROFILE['constructor'][0]]
        destructors=[c['metadata'] for c in actual if int(c['metadata'].get('target','0'),16)==PROFILE['destructor'][0]]
        queries=[c['metadata'] for c in actual if 'slot' in c['metadata']]
        objects=scope['objectOffsets'];require(len(constructors)==len(destructors)==len(queries)==3,'unsupported local call set')
        require([c['ecxStack'] for c in constructors]==[offset-depth for offset in objects] and all(c['depth']==depth+8 for c in constructors),'constructor coordinates changed')
        require([c['ecxStack'] for c in destructors]==[offset-depth for offset in reversed(objects)] and all(c['depth']==depth for c in destructors),'destructor coordinates changed')
        for index,c in enumerate(queries):
            require(int(c['slot'],16)==(0x54,0xa8,0xa4)[index] and c['depth']==depth+(8 if index==2 else 4),'query ABI changed')
            require(c['pushedStack']==[objects[index]-depth]+([None] if index==2 else []),'query name coordinate changed')
        prefix=scope['oldLuaPrefix'];require(prefix and source.count(prefix)==1 and digest(prefix.encode())==scope['luaPrefixSha256'],'ambiguous Lua prefix')
        pos=source.index(prefix);flag=scope['flagAlias'];dead=scope['deadResults'];preserve=scope['preserveFalse']
        names=[flag,*dead,*preserve]
        require(len(names)==len(set(names)) and all(re.fullmatch(r'[A-Za-z_]\w*',n) and n in locals_ for n in names),'invalid/escaping local roles')
        pm=masked(prefix);labels=re.findall(r'::([A-Za-z_]\w*)::',pm)
        require(len(labels)==2 and len(set(labels))==2,'unexpected query labels')
        outside=masked(source[:pos]+source[pos+len(prefix):])
        require(all(not re.search(r'\bgoto\s+'+re.escape(label)+r'\b|::'+re.escape(label)+r'::',outside) for label in labels),'external Lua label dependency')
        results=[];arguments=[]
        for method in METHODS:
            matches=list(re.finditer(r'(?m)^[ \t]*(\w+) = me:'+method+r'\(([^\n]*)\)$',pm))
            require(len(matches)==1,'query Lua call multiplicity changed');results.append(matches[0][1]);arguments.append(matches[0][2])
        require(arguments[:2]==['',''] and re.fullmatch(r'(?:14|0xe)',arguments[2]) is not None,'Lua query argument changed')
        hero,any_,special=results
        require(set(results).issubset(set(dead+preserve)),'unreviewed query scratch result')
        require(not preserve or len(preserve)==2 and preserve[0]==any_ and preserve[1] not in (hero,special,flag),'unsupported cleanup flag shape')
        tail=pm.split('::'+labels[1]+'::',1)[1]
        clear_pattern=r'(?m)^[ \t]*if (\w+) then\n[ \t]*\1 = false\n[ \t]*end\n?'
        clears=list(re.finditer(clear_pattern,tail))
        require({m[1] for m in clears}==set(preserve) and len(clears)==len(preserve),'cleanup exit changed')
        require(re.sub(clear_pattern,'',tail).strip()=='','unexpected exit statement')
        extras=''.join(name+' = true\n' for name in preserve)
        expected=(f'{hero} = me:{METHODS[0]}()\nif not {hero} then\n{any_} = me:{METHODS[1]}()\nif {any_} then\n'+extras+
            f'{special} = me:{METHODS[2]}({arguments[2]})\nif not {special} then goto {labels[0]} end\nend\n'+
            (any_+' = true\n' if preserve else '')+f'{flag} = 0\nelse\ngoto {labels[0]}\nend\ngoto {labels[1]}\n::{labels[0]}::\n{flag} = 1\n::{labels[1]}::\n'+tail)
        require(significant(prefix)==significant(expected),'Lua short-circuit/flag shape changed')
        invariants=[]
        for alias in preserve:
            assignment=re.compile(r'(?m)^[ \t]*(?:local )?([A-Za-z_]\w*(?:[ \t]*,[ \t]*[A-Za-z_]\w*)*) = ([^\n]+)$')
            writes=[m for m in assignment.finditer(clean) if alias in [v.strip() for v in m[1].split(',')]]
            require(writes and all(m[1]==alias for m in writes),'unsupported parallel/shadow write')
            require(len(writes)==len(re.findall(r'\b'+re.escape(alias)+r'\s*=(?!=)',clean)),'unparsed cleanup-local write')
            require(writes[0][2].strip()=='false','missing boolean initializer')
            control=re.search(r'(?m)^[ \t]*(?:if|while|repeat|goto|::)',clean)
            require(control is not None and writes[0].start()<control.start(),'cleanup initializer can be bypassed')
            allowed={'false','true',*[f'me:{method}()' for method in METHODS[:2]],'me:MsgIsHitByHeroSpecialAbility(0xe)','me:MsgIsHitByHeroSpecialAbility(14)'}
            require(all(m[2].strip() in allowed or re.fullmatch(r'not [A-Za-z_]\w*',m[2].strip()) for m in writes),'cleanup local is not always boolean')
            invariants.append(dict(alias=alias,initial='false',writes=[m[2].strip() for m in writes],normalExit=False))
        proofs=[];analysis_at=analysis.index(prefix)+len(prefix);indent=re.match(r'[ \t]*',prefix)[0]
        for alias in dead:
            marker=indent+alias+' = nil\n';hypothetical=analysis[:analysis_at]+marker+analysis[analysis_at:]
            proofs.append(dict(alias=alias,analysisOnlyAssignment=True,proof=liveness(hypothetical,analysis_at+len(indent),alias)))
        markers=re.findall(r'(?m)^[ \t]*-- TODO\(native\): (.*)$',prefix)
        require(markers==[diagnostic[10:] for diagnostic in scope['diagnostics']],'diagnostic correspondence changed')
        for diagnostic in scope['diagnostics']:
            require(remaining.count(diagnostic)==1,'diagnostic multiplicity changed');remaining.remove(diagnostic)
        occurrence=masked(source[:pos]).count('me:'+METHODS[0]+'(')
        require(occurrence==scope['callOccurrence'] and occurrence<len(call_indices) and call_indices[occurrence]==scope['callListIndex'],'query call occurrence changed')
        replacement=indent+flag+' = resources:IsHitByHeroExceptAbility(me, 14) and 1 or 0\n'
        replacement+=''.join(indent+alias+' = false\n' for alias in preserve)
        edits.append((pos,pos+len(prefix),replacement));call_edits.append((scope['callListIndex'],scope['callListIndex']+3,'RESOURCE_IsHitByHeroExceptAbility'))
        reviews.append(dict(nativeStart=scope['start'],nativeEndExclusive=scope['endExclusive'],nativeRoles=native,
            sourceCalls=actual,externalNativeEntries=entries,deadScratch=proofs,preservedBooleanLocals=invariants,
            numericFlagRetained=True,requiredCapability='IsHitByHeroExceptAbility',
            scope='Normal query/CString lifetime topology under stated native ABI/object contracts; source prefix shape and outgoing locals checked. No active heap, exceptions/reentry or gameplay proof.'))
    require(all(a[1]<=b[0] for a,b in zip(sorted(edits),sorted(edits)[1:])),'overlapping Lua edits')
    require(all(a[1]<=b[0] for a,b in zip(sorted(call_edits),sorted(call_edits)[1:])),'overlapping call edits')
    out=source;updated_calls=list(calls)
    for begin,stop,replacement in sorted(edits,reverse=True):out=out[:begin]+replacement+out[stop:]
    for begin,stop,replacement in sorted(call_edits,reverse=True):updated_calls[begin:stop]=[replacement]
    return out,remaining,updated_calls,reviews
