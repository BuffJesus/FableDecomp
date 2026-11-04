"""Reviewed shared-word hit scopes with checked native/Lua outgoing constants."""
import hashlib,json,re
from tools.script_recovery.native_reviewed_hit_query_lifetimes import METHODS,PROFILE,masked,significant
from tools.script_recovery.native_hit_health_entry import prove_frame
from tools.script_recovery.native_shared_mask_invariants import prove as prove_mask
from tools.script_recovery.native_hit_query_roles import prove as prove_scope
from tools.script_recovery.lua_numeric_alias_invariants import prove as prove_numeric
from tools.script_recovery.lua_staging_liveness import build_cfg
from tools.script_recovery.lift_native_lua import load_manifest

def _template(scope,mask,scratch):
    flag=scope['flagAlias'];yes,done=scope['labels'];bits=[1<<n for n in range(32) if scope['temporaryMask']&(1<<n)];assert len(bits)==3 and bits[1]==bits[0]*2 and bits[2]==bits[1]*2
    lit=lambda n:str(n) if n<10 else hex(n)
    a,b,c=map(lit,bits);ab=lit(bits[0]|bits[1]);allbits=lit(sum(bits));inverses=[lit(255^bit) for bit in bits] if bits[2]<256 else []
    clear=lambda alias,bit,constant:f'if ({alias} & {bit}) ~= 0 then\n{alias} = {alias} & {constant}\nend\n'
    if scope['luaShape']=='mask-before-query':
        assert inverses
        return f'''{scratch} = {mask} | {a}
{flag} = me:{METHODS[0]}()
if {flag} then
goto {yes}
else
{scratch} = {mask} | {ab}
{flag} = me:{METHODS[1]}()
if {flag} then
{scratch} = {allbits}
{flag} = me:{METHODS[2]}(0xe)
if not {flag} then goto {yes} end
end
{flag} = false
end
goto {done}
::{yes}::
{flag} = true
::{done}::
'''+clear(scratch,c,inverses[2])+clear(scratch,b,inverses[1])+f'''{mask} = {scratch}
if ({scratch} & {a}) ~= 0 then
{mask} = {scratch} & {inverses[0]}
end
'''
    if scope['luaShape']=='query-before-mask':
        assert inverses
        return f'''{flag} = me:{METHODS[0]}()
{scratch} = {mask} | {a}
if {flag} then
goto {yes}
else
{flag} = me:{METHODS[1]}()
{scratch} = {mask} | {ab}
if {flag} then
{flag} = me:{METHODS[2]}(0xe)
{scratch} = {mask} | {allbits}
if not {flag} then goto {yes} end
end
{mask} = {scratch}
{flag} = false
end
goto {done}
::{yes}::
{mask} = {scratch}
{flag} = true
::{done}::
'''+''.join(clear(mask,lit(bit),lit(255^bit)) for bit in reversed(bits))
    assert scope['luaShape']=='boolean-high-mask' and bits==[128,256,512]
    special,any_=scope['booleanExitFalse'];assert len({flag,special,any_,mask,scratch})==5
    return f'''{flag} = me:{METHODS[0]}()
if {flag} then
goto {yes}
else
{any_} = me:{METHODS[1]}()
if {any_} then
{any_} = true
{special} = true
{flag} = me:{METHODS[2]}(0xe)
if not {flag} then goto {yes} end
end
{any_} = true
{flag} = false
end
goto {done}
::{yes}::
{flag} = true
::{done}::
{scratch} = {mask} | 0x80
if {special} then
{special} = false
end
if {any_} then
{any_} = false
end
if (({scratch} & 0x80) ~= 0) then
{scratch} = {mask} & 0x7f
end
'''

def _boolean_outputs(source,cfg,aliases,boundary):
    nodes=cfg['nodes'];entry=cfg['entry'];manifest=load_manifest();boolean={name for name,spec in manifest.items() if spec['returnType']=='bool'};reachable=set();pending=[entry];preds={k:[] for k in nodes}
    for k,(_,edges) in nodes.items():
        for dest in edges:
            if dest is not None:preds[dest].append(k)
    while pending:
        k=pending.pop()
        if k in reachable:continue
        reachable.add(k);pending.extend(dest for dest in nodes[k][1] if dest is not None)
    dom={k:({entry} if k==entry else set(reachable)) for k in reachable};changed=True
    while changed:
        changed=False
        for k in sorted(reachable-{entry}):
            parents=[p for p in preds[k] if p in reachable];assert parents;value={k}|set.intersection(*(dom[p] for p in parents))
            if value!=dom[k]:dom[k]=value;changed=True
    result=[]
    for alias in aliases:
        writes=[(k,line.split('=',1)[1].strip()) for k,(line,_) in nodes.items() if re.fullmatch(re.escape(alias)+r'\s*=\s*.+',line)];assert writes and alias in cfg['locals']
        assert len(writes)==len(re.findall(r'\b'+re.escape(alias)+r'\s*=(?!=)',masked(source))),'unparsed boolean write'
        for k,rhs in writes:
            call=re.fullmatch(r'(me|quest|resources):([A-Za-z_]\w*)\(.*\)',rhs)
            assert rhs in ('true','false') or rhs.startswith('not ') or re.search(r'<=|>=|==|~=|<|>',rhs) or call and (call[2] in boolean or call.groups()==('resources','TryAcquire')),'nonboolean cleanup alias'
        initializers=[k for k,_ in writes if k<boundary and k in dom[boundary]];assert initializers,'boolean initializer can be bypassed'
        result.append(dict(alias=alias,writes=sorted({rhs for _,rhs in writes}),dominatingAssignmentLines=[k+1 for k in sorted(initializers)],normalExit=False))
    return result

def recover_shared(source,todo,calls,fn,row,unit,owner,image,liveness,raw):
    """Caller applies the common source/native/helper-profile hash guards first."""
    def require(ok,why):
        if not ok:raise ValueError('reviewed shared-mask hit query: '+why)
    digest=lambda data:hashlib.sha256(data).hexdigest();start=int(fn['address'],16);scopes=row['scopes'];require(scopes and row['actorProfile']=='entry-ebx','unsupported actor/scopes');mask,scratch=row['numericMaskAliases'];require(mask!=scratch,'duplicate numeric alias')
    require(re.fullmatch(r'function [A-Za-z_]\w*\(quest, me\)',masked(source).splitlines()[0]) is not None,'receiver parameters changed')
    require(len(re.findall(r'(?m)^[ \t]*local resources = quest:RetailResources\(\)$',source))==1,'resource binding changed');require(not re.search(r'(?m)^[ \t]*(?:quest|me|resources)\s*=(?!=)',masked(source)),'receiver reassigned')
    original=[fn['calls'],fn['indirectCalls']];evidence=row['entryCallDepthEvidence'];require(digest(json.dumps(original,separators=(',',':')).encode())==evidence['originalCallsSha256'],'stale original entry records');depths=evidence['depths'];allcalls=fn['calls']+fn['indirectCalls'];require(set(depths)=={c['site'].lower() for c in allcalls} and len(depths)==len(allcalls),'entry sites changed');require(all(type(v) is int and 0<=v<=4096 and v%4==0 for v in depths.values()),'invalid entry depth');require(digest(json.dumps(depths,sort_keys=True,separators=(',',':')).encode())==evidence['depthsSha256'],'entry depth certificate changed');entry_fn=dict(fn,**{key:[dict(c,depth=depths[c['site'].lower()]) for c in fn[key]] for key in ('calls','indirectCalls')})
    inline=re.compile(r'(?m)^(?P<i>[ \t]*)if (?P<c>(?:not )?[A-Za-z_]\w*) then break end$');require(len(list(inline.finditer(source)))==row['analysisInlineBreaks'],'analysis control changed');analysis=inline.sub(lambda m:m['i']+'if '+m['c']+' then\n'+m['i']+'    break\n'+m['i']+'end',source);cfg=build_cfg(analysis);require(all(a in cfg['locals'] for a in (mask,scratch)),'numeric mask nonlocal');frames=[];regions=[];prepared=[]
    from tools.script_recovery.convert_quest_unit import _text_order_sites
    ordered=_text_order_sites(fn['decompile'],fn);require(ordered is not None,'source/native alignment missing')
    indices=[n for n in range(len(calls)-2) if list(calls[n:n+3])==list(METHODS)]
    require(len({s['start'] for s in scopes})==len(scopes),'duplicate native scopes')
    for scope in scopes:
        a,b=int(scope['start'],16),int(scope['endExclusive'],16);require(start<a<b<=start+len(raw) and digest(raw[a-start:b-start])==scope['nativeSha256'],'native scope changed');require(scope['flagOffset']==row['flagOffset'] and scope['maskOffset']==row['maskOffset'],'inconsistent shared geometry');require(all(depths[c['site'].lower()]==c['depth'] for c in allcalls if a<=int(c['site'],16)<b),'local scope anchors changed')
        frame=prove_frame(raw,entry_fn,image,entity_vtable=int(row['entityVtable'],16),scope_start=a,scope_end=b,frame_depth=row['frameDepth'],mask_offset=row['maskOffset'],this_definition=int(row['thisDefinition'],16),actor_definition=int(row['actorDefinition'],16),actor_profile='entry-ebx');frames.append(frame)
        native=prove_scope(raw[a-start:b-start],a,constructor=PROFILE['constructor'][0],destructor=PROFILE['destructor'][0],hero_literal=int(row['heroLiteral'],16),flag_offset=scope['flagOffset'],mask_offset=scope['maskOffset'],object_offsets=tuple(scope['objectOffsets']),temporary_mask=scope['temporaryMask'],actor_profile='entry-ebx')
        actual=[dict(site=site['site'].lower(),arguments=args,metadata=site) for _,_,args,site,_,_ in ordered if a<=int(site['site'],16)<b];require(len(actual)==9 and actual==scope['sourceCalls'],'scope call correspondence changed')
        constructors=[c['metadata'] for c in actual if c['metadata'].get('target','').lower()==hex(PROFILE['constructor'][0]) or int(c['metadata'].get('target','0'),16)==PROFILE['constructor'][0]];destructors=[c['metadata'] for c in actual if int(c['metadata'].get('target','0'),16)==PROFILE['destructor'][0]];queries=[c['metadata'] for c in actual if 'slot' in c['metadata']];d=row['frameDepth'];objects=scope['objectOffsets'];require(len(constructors)==len(destructors)==len(queries)==3,'local call set changed');require([c['ecxStack'] for c in constructors]==[o-d for o in objects] and all(c['depth']==d+8 and c['pushedStack']==[None,None] for c in constructors),'constructor coordinates changed');require([c['ecxStack'] for c in destructors]==[o-d for o in reversed(objects)] and all(c['depth']==d for c in destructors),'destructor coordinates changed')
        for n,c in enumerate(queries):require(c['slot']==('0x54','0xa8','0xa4')[n] and c['depth']==d+(8 if n==2 else 4) and c['pushedStack']==[objects[n]-d]+([None] if n==2 else []),'query ABI coordinates changed')
        prefix=scope['oldLuaPrefix'];require(prefix and source.count(prefix)==1 and digest(prefix.encode())==scope['luaPrefixSha256'],'ambiguous Lua prefix');flag=scope['flagAlias'];booleans=scope['booleanExitFalse'];require(len(set([flag,mask,scratch,*booleans]))==3+len(booleans) and all(a in cfg['locals'] for a in [flag,mask,scratch,*booleans]),'invalid local roles');require(len(scope['labels'])==2 and len(set(scope['labels']))==2 and all(re.fullmatch(r'[A-Za-z_]\w*',s) for s in scope['labels']),'invalid labels');require(significant(prefix)==significant(_template(scope,mask,scratch)),'unsupported Lua control/mask shape');pos=source.index(prefix);outside=masked(source[:pos]+source[pos+len(prefix):]);require(all(not re.search(r'\bgoto\s+'+re.escape(label)+r'\b|::'+re.escape(label)+r'::',outside) for label in scope['labels']),'external label consumer');require(scope['diagnostics']==[] and not re.search(r'(?m)^[ \t]*-- TODO\(native\):',prefix),'unexpected diagnostics')
        occurrence=masked(source[:pos]).count('me:'+METHODS[0]+'(');require(occurrence==scope['callOccurrence'] and occurrence<len(indices) and indices[occurrence]==scope['callListIndex'],'query call accounting changed')
        entry={mask:0};exit={mask:0,**({scratch:0} if booleans else {})};regions.append(dict(prefix=prefix,entry=entry,exit=exit));indent=re.match(r'[ \t]*',prefix)[0];at=analysis.index(prefix)+len(prefix);dead=[]
        if not booleans:
            marker=indent+scratch+' = nil\n';hypothetical=analysis[:at]+marker+analysis[at:];dead.append(dict(alias=scratch,proof=liveness(hypothetical,at+len(indent),scratch)))
        preserved=_boolean_outputs(analysis,cfg,booleans,analysis[:analysis.index(prefix)].count('\n')) if booleans else []
        replacement=indent+flag+' = resources:IsHitByHeroExceptAbility(me, 14)\n'+(indent+scratch+' = 0\n'+''.join(indent+alias+' = false\n' for alias in booleans) if booleans else indent+mask+' = 0\n');prepared.append(dict(scope=scope,pos=pos,prefix=prefix,replacement=replacement,native=native,actual=actual,dead=dead,preserved=preserved))
    first=frames[0];native_depths={first['ins'][k].address:depth for k,depth in first['depths'].items()};maskproof=prove_mask(raw,start,native_depths,image,mask_entry_offset=row['maskOffset']-row['frameDepth'],scopes=[(int(s['start'],16),s['temporaryMask']) for s in scopes]);require(all(f['knownZero']=='0xffffffff' and f['knownOne']=='0x0' for f in maskproof['scopeEntryFacts'].values()),'numeric mask entry is not zero');luaproof=prove_numeric(analysis,[mask,scratch],regions);edits=sorted((p['pos'],p['pos']+len(p['prefix'])) for p in prepared);require(all(a[1]<=b[0] for a,b in zip(edits,edits[1:])),'overlapping Lua scopes');out=source;updated=list(calls);reviews=[]
    for p in sorted(prepared,key=lambda p:p['pos'],reverse=True):out=out[:p['pos']]+p['replacement']+out[p['pos']+len(p['prefix']):]
    for p in sorted(prepared,key=lambda p:p['scope']['callListIndex'],reverse=True):n=p['scope']['callListIndex'];updated[n:n+3]=['RESOURCE_IsHitByHeroExceptAbility']
    for n,p in enumerate(prepared):
        ctx=frames[n];reviews.append(dict(nativeStart=p['scope']['start'],nativeEndExclusive=p['scope']['endExclusive'],nativeRoles=p['native'],registeredEntryFrame=dict(instructions=len(ctx['reachable']),origins=ctx['origins'],scopeDepth=row['frameDepth'],normalIncomingEdges=[dict(source=hex(a),target=hex(b)) for a,b in ctx['incoming']]),sourceCalls=p['actual'],deadScratch=p['dead'],preservedBooleanLocals=p['preserved'],outgoingNumericConstants=regions[n]['exit'],luaNumericFacts=luaproof['facts'][n],nativeSharedMask=maskproof if n==0 else dict(sharedWithScope=scopes[0]['start']),reviewedEntryCallDepthEvidence=evidence if n==0 else dict(sharedWithScope=scopes[0]['start']),booleanFlagRetained=True,requiredCapability='IsHitByHeroExceptAbility',scope='Reviewed normal CString/query/packed-mask region under checked entry and explicit child ABI/disjoint-footprint contracts. No real heap,exceptions/reentry or gameplay proof.'))
    return out,list(todo),updated,reviews
