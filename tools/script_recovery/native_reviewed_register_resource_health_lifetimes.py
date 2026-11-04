"""Scratch opt-in register-saved resource health lowering; no converter hook."""
import hashlib,json,re
from tools.script_recovery.native_hit_health_entry import prove_frame
from tools.script_recovery.native_reviewed_saved_health_predicates import PROFILE
from tools.script_recovery.lua_staging_liveness import build_cfg,prove_unused_staging
from tools.script_recovery.native_saved_register_x87_predicate import prove as prove_predicate
from tools.script_recovery.native_register_health_roles import prove_roles
from tools.script_recovery.native_health_copy_correspondence import prove as prove_copy

METHOD='ResourceThingHealthAbove'
serial=lambda obj:json.dumps(obj,separators=(',',':')).encode()
digest=lambda data:hashlib.sha256(data).hexdigest()

def recover(source,todo,calls,fn,row,unit,owner,image,manifest,before,after,*,prior_hit_health_input=None,prior_register_comparison_input=None):
    if row is None:return source,list(todo),list(calls),[]
    def require(ok,why):
        if not ok:raise ValueError('register resource health: '+why)
    require(owner==row['owner'] and row['method']==METHOD,'owner/method')
    require(type(row['minimumHealth']) in (int,float) and row['minimumHealth']==0,'minimum')
    start,end=int(fn['address'],16),int(fn['bodyEndExclusive'],16)
    raw=image.bytes_at(start,end-start)
    for key,data in [('nativeSha256',raw),('fnSha256',serial(fn)),('luaPhaseSha256',source.encode()),('callsSha256',serial(calls)),('todoSha256',serial(todo)),('beforeSha256',serial(before)),('afterSha256',serial(after))]:
        require(data is not None and digest(data)==row[key],key)
    entity=unit['entities'][owner]
    require(entity['vtable'].lower()==row['entityVtable'].lower() and entity['functions']['Main']['address'].lower()==fn['address'].lower(),'registered Main')
    require(manifest['GetHealth']==row['hostContract'],'host contract')
    local_frame=row.get('ownedLocalResourceFrame');origin=None
    if local_frame is not None:
        require(row['healthSlot']==0x420 and row['actorProfile']=='owner-only' and row['actorDefinition'] is None and 'scopeStart' in row,'owned local resource profile')
        require(row.get('proveComposedDeadValues') is True and row.get('allowWrappedCallSpacing') is True,'local resource source/composition conditions')
        scope_start=int(row['scopeStart'],16)
    else:
        require(row['healthSlot']==0x420 and row['actorProfile'] in ('local-edi','entry-ebp'),'producer profile')
        require(row['actorProfile']!='entry-ebp' or 'scopeStart' in row,'entry actor scope missing')
        scope_start=int(row.get('scopeStart',row['actorDefinition']),16)
        require(row['actorProfile']!='local-edi' or scope_start==int(row['actorDefinition'],16),'local actor scope changed')
    for address,size,pin in PROFILE:
        data=image.bytes_at(address,size)
        require(data is not None and digest(data)==pin,'native dependency')
    frame_fn=fn;frame_projection=None
    prior_row=unit.get('reviewedHitHealthLifetimes',{}).get(fn['address'].lower())
    if prior_hit_health_input is not None and prior_row is not None and prior_row.get('entryCallDepthEvidence') is not None:
        from tools.script_recovery.native_prior_hit_health import replay,reviewed_frame_view
        replayed=replay(fn,source,input=prior_hit_health_input,todo=todo,calls=calls,unit=unit,owner=owner,image=image)
        frame_fn,frame_projection=reviewed_frame_view(fn,replayed)
    if local_frame is None:
        frame=prove_frame(raw,frame_fn,image,entity_vtable=int(row['entityVtable'],16),scope_start=scope_start,scope_end=end,frame_depth=row['frameDepth'],mask_offset=0,this_definition=int(row['thisDefinition'],16),actor_definition=int(row['actorDefinition'],16),actor_profile=row['actorProfile'])
        actor=frame['ins'][frame['by'][int(row['actorDefinition'],16)]]
        require(actor.mnemonic=='lea','actor address definition')
        require(row['actorProfile']!='entry-ebp' or scope_start==actor.address+actor.size,'entry actor scope adjacency')
    else:
        from tools.script_recovery.native_local_resource_frame_contracts import prove as prove_local_frame
        from tools.script_recovery.native_owned_local_resource_origin import prove as prove_origin
        require(prior_hit_health_input is None and prior_row is None,'local resource prior-hit composition unsupported')
        frame,frame_report=prove_local_frame(raw,fn,image,local_frame,entity=entity,scope_start=scope_start,scope_end=end,frame_depth=row['frameDepth'],this_definition=int(row['thisDefinition'],16),owner_register='esi')
        origin=prove_origin(image,fn,raw,row['resourceInitialization']);resource_alias='xStack_'+format(-origin['resourceEntryOffset'],'x')
        creation=re.compile(r'(?m)^ *'+re.escape(resource_alias)+r' = resources:NewResource\(\)$');matches=list(creation.finditer(source));require(len(matches)==1,'owned Lua resource creation')
        frame_projection=dict(frame=frame_report,resourceInitialization=origin)
    prior=dict(input=prior_hit_health_input,todo=todo,calls=calls,unit=unit,owner=owner,image=image) if prior_hit_health_input is not None else None
    options=dict(allow_wrapped_call_spacing=True) if local_frame is not None else {}
    if prior_register_comparison_input is not None:
        require(local_frame is not None and row.get('replayPriorRegisterComparison') is True,
            'prior comparison profile')
        options['prior_comparison']=dict(input=prior_register_comparison_input,todo=todo,calls=calls,
            unit=unit,owner=owner,image=image,manifest=manifest)
    else:
        require(not row.get('replayPriorRegisterComparison'),'prior comparison input missing')
    correspondence=prove_copy(fn,before,after,source,prior_health=prior,**options)
    if local_frame is not None:
        for scope in correspondence['scopes']:
            require(scope['resource']==resource_alias and scope['start']>matches[0].end(),
                'owned resource/source identity')
            if scope['condition']==scope['threshold']+' < '+scope['result']:
                require('priorComparisonReview' not in scope,'positive scope with inverse review')
            else:
                reviewed=scope.get('priorComparisonReview')
                require('priorRegisterComparison' in correspondence and reviewed is not None
                    and scope['condition']=='not ('+scope['result']+' > '+scope['threshold']+')'
                    and reviewed['originalCondition']==scope['result']+' <= '+scope['threshold'],
                    'owned resource/source ordered scopes')

    profiles={p['call'].lower():p for p in row['nativeProfiles']}
    require(len(profiles)==len(row['nativeProfiles'])==correspondence['nativeSites'],'native profile inventory')
    native={}
    for scope in correspondence['scopes']:
        site=scope['native']['call']['site'].lower()
        require(site in profiles,'unknown native site')
        require(start<scope_start<=int(site,16)<end,'health outside entry scope')
        if site in native:continue
        p=profiles[site]
        pred=prove_predicate(frame,int(site,16),image.bytes_at,0x4aa840,positive_exit=p['positiveExit'],false_exit=p['falseExit'])
        require(pred['threshold']==0,'native threshold')
        if 'priorComparisonReview' in scope:
            require(pred==scope['priorComparisonReview']['native'],'prior/folded native predicate correspondence')
        role_options=dict(call_window=p['callWindow']) if 'callWindow' in p else {}
        roles=prove_roles(frame,int(site,16),pred,image,getter_target=0x7e7490,this_definition=int(row['thisDefinition'],16),health_slot=row['healthSlot'],**role_options)
        if local_frame is not None:
            require(roles['resourceEntryOffset']==origin['resourceEntryOffset'],'local resource initial/getter identity')
            known={r['site']:r['target'] for r in frame_report['callReviews']}
            require(known.get(hex(int(site,16)))=='0x890400' and known.get(roles['getter'])=='0x7e7490' and known.get(roles['cleanup'])=='0x4aa840','folded query target contracts')
        require(int(scope['native']['getter']['site'],16)==int(roles['getter'],16) and int(scope['native']['cleanup']['site'],16)==int(roles['cleanup'],16),'native/source producer correspondence')
        native[site]=dict(predicate=pred,roles=roles)
        if local_frame is not None:
            resource_start=origin['resourceEntryOffset'];resource_end=resource_start+origin['resourceBytes']
            thing_start=roles['objectEntryOffset'];thing_end=thing_start+roles['objectBytes']
            require(resource_end<=thing_start or thing_end<=resource_start,'owned resource/Thing stack overlap')
            native[site]['localResourceFootprints']=dict(resource=[resource_start,resource_end],
                thing=[thing_start,thing_end],disjoint=True,
                limits='Native caller stack intervals only; no engine heap/current lifetime proof.')
    inline=re.compile(r'(?m)^(?P<i> *)if (?P<c>(?:not )?[A-Za-z_]\w*) then break end$')
    require(len(list(inline.finditer(source)))==row['analysisInlineBreaks'],'analysis inline breaks')
    expand=lambda text:inline.sub(lambda m:m['i']+'if '+m['c']+' then\n'+m['i']+'    break\n'+m['i']+'end',text)
    global_read=re.compile(r'(?m)^(?P<i> *)if (?P<c>(?:not )?[A-Za-z_]\w*) then (?P<target>[A-Za-z_]\w*) = _G\.(?P<field>[A-Za-z_]\w*) end$')
    require(len(list(global_read.finditer(source)))==row.get('analysisInlineGlobalReads',0),'analysis inline global reads')
    expand_breaks=expand
    def expand(text):
        return global_read.sub(lambda m:m['i']+'if '+m['c']+' then\n'+m['i']+'    '+m['target']+' = _G.'+m['field']+'\n'+m['i']+'end',expand_breaks(text))
    analysis=expand(source)
    cfg=build_cfg(analysis);predecessors={k:set() for k in cfg['nodes']}
    for node,(_,edges) in cfg['nodes'].items():
        for edge in edges:
            if edge is not None:predecessors[edge].add(node)
    indices=[n for n in range(len(calls)-1) if list(calls[n:n+2])==['RESOURCE_ScriptThing','GetHealth']]
    require(len(indices)==len(correspondence['scopes']),'call-pair inventory')
    edits=[];reviews=[]
    for scope,index in zip(correspondence['scopes'],indices):
        prefix=scope['prefix'];site=scope['native']['call']['site'].lower();proof=native[site]
        roles=proof['roles'];resource='xStack_'+format(-roles['resourceEntryOffset'],'x')
        require(roles['resourceEntryOffset']<0 and scope['resource']==resource,'resource stack identity')
        pos=scope['start'];at=len(expand(source[:pos]));indent=re.match(r' *',prefix)[0]
        require(source[pos:scope['end']]==analysis[at:at+len(prefix)]==prefix,'prefix occurrence correspondence')
        first=analysis[:at].count('\n');last=first+prefix.count('\n');nodes=set(range(first,last+1))
        require(nodes<=set(cfg['nodes']),'prefix nodes')
        incoming=[(a,b) for b in nodes for a in predecessors[b] if a not in nodes]
        require(incoming and all(b==first for a,b in incoming),'external prefix entry')
        health=indent+scope['result']+' = quest:GetHealth('+scope['actor']+')\n'
        after_health=at+prefix.index(health)+len(health)
        dead=[]
        for alias in scope['distinctThingAliases']:
            marker=indent+alias+' = nil\n';hypothetical=analysis[:after_health]+marker+analysis[after_health:]
            dead.append(dict(role='thing',alias=alias,proof=prove_unused_staging(hypothetical,after_health+len(indent),alias)))
        condition_at=at+len(prefix)-len(scope['condition'])-len(' then')
        independent=analysis[:condition_at]+'native_resource_health_decision'+analysis[condition_at+len(scope['condition']):]
        before_if=independent.rfind('\n',0,condition_at)+1
        for alias in [scope['result'],scope['threshold']]:
            marker=indent+alias+' = nil\n';hypothetical=independent[:before_if]+marker+independent[before_if:]
            dead.append(dict(role='numeric',alias=alias,proof=prove_unused_staging(hypothetical,before_if+len(indent),alias)))
        aliases=scope['distinctThingAliases']
        clear=indent+', '.join(aliases)+' = '+', '.join(['nil']*len(aliases))+'\n'
        negative=scope['condition']==scope['result']+' <= '+scope['threshold'] or 'priorComparisonReview' in scope
        condition=('not ' if negative else '')+'resources:'+METHOD+'('+resource+', 0.0)'
        replacement=clear+indent+'if '+condition+' then'
        edits.append((pos,pos+len(prefix),replacement))
        reviews.append(dict(call=site,callListIndex=index,native=proof,copyTarget=scope['copiedContinuation'],clearedThingAliases=aliases,deadValues=dead,incoming=incoming))
    require(all(a[1]<=b[0] for a,b in zip(edits,edits[1:])),'overlapping prefixes')
    out=source;updated=list(calls)
    for a,b,text in reversed(edits):out=out[:a]+text+out[b:]
    for index in reversed(indices):updated[index:index+2]=['RESOURCE_'+METHOD]
    if local_frame is not None:
        combined=source
        for scope in reversed(correspondence['scopes']):
            indent=re.match(r' *',scope['prefix'])[0];negative=scope['condition']==scope['result']+' <= '+scope['threshold'] or 'priorComparisonReview' in scope;combined=combined[:scope['start']]+indent+'if '+('not ' if negative else '')+'resources:'+METHOD+'('+scope['resource']+', 0.0) then'+combined[scope['end']:]
        helpers=list(re.finditer(r'(?m)^(?P<i> *)if (?:not )?resources:'+METHOD+r'\('+re.escape(resource_alias)+r', 0\.0\) then',combined));require(len(helpers)==len(reviews),'composed owned helper inventory')
        for scope,m,review in zip(correspondence['scopes'],helpers,reviews):
            indent=m['i'];proofs=[]
            for alias in scope['distinctThingAliases']+[scope['result'],scope['threshold']]:
                marker=indent+alias+' = nil\n';hypothetical=combined[:m.start()]+marker+combined[m.start():];pos=m.start()+len(indent);proofs.append(dict(alias=alias,proof=prove_unused_staging(expand(hypothetical),len(expand(hypothetical[:pos])),alias)))
            review['composedDeadValues']=proofs
        reviews[0]['sourceCopyCorrespondence']=correspondence
    if 'priorRegisterComparison' in correspondence:
        reviews[0]['reviewedPriorRegisterComparison']=correspondence['priorRegisterComparison']
    if 'priorHealth' in correspondence:
        reviews[0]['nativeHealthInventory']=dict(original=correspondence['priorHealth']['originalNativeSites'],lowered=correspondence['nativeSites'],priorSites=correspondence['priorHealth']['consumedSites'],priorReplayInputSha256=digest(prior_hit_health_input['source'].encode()),priorReplayOutputSha256=digest(source.encode()))
    if correspondence.get('priorHealth',{}).get('checkedSelfExpressions'):reviews[0]['nativeHealthInventory']['priorActorMappings']=correspondence['priorHealth']['checkedSelfExpressions']
    if frame_projection is not None:reviews[0]['reviewedNativeFrameProjection']=frame_projection
    return out,list(todo),updated,reviews
