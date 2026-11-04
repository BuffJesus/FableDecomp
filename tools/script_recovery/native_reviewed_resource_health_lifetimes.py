"""Fuse reviewed resource Thing/health/ordered-flag scopes with immediate retirement.

Replay the upstream predicate certificate, then prove all removed values dead
over the complete normal Lua CFG. Explicitly clear the former Thing aliases so
their older values do not gain new strong references by removing overwrites.
Only normal child ABI/valid resource/disjoint footprints are claimed here.
"""
import hashlib
import json
import re

from tools.script_recovery.native_reviewed_saved_health_predicates import PATTERN,recover as verify_predicates
from tools.script_recovery.lua_staging_liveness import build_cfg,prove_unused_staging

METHOD='ResourceThingHealthAbove'
CALL='RESOURCE_'+METHOD


def recover(source,todo,calls,fn,row,unit,owner,image,manifest):
    if row is None:return source,list(todo),list(calls),[]
    def require(ok,why):
        if not ok:raise ValueError('reviewed resource health lifetime '+fn['address']+': '+why)
    digest=lambda data:hashlib.sha256(data).hexdigest()
    serial=lambda obj:json.dumps(obj,separators=(',',':')).encode()
    require(owner==row['owner'] and row['method']==METHOD,'owner/method changed')
    require(type(row['minimumHealth']) in (int,float) and row['minimumHealth']==0,'unsupported minimum')
    for key,data in [('luaPhaseSha256',source.encode()),('callsSha256',serial(calls)),('todoSha256',serial(todo))]:
        require(digest(data)==row[key],key+' changed')
    prior=unit.get('reviewedSavedHealthPredicates',{}).get(str(fn['address']).lower())
    require(prior is not None and digest(serial(prior))==row['predicateEvidenceSha256'],'upstream predicate evidence changed')
    require(prior['owner']==owner,'upstream owner changed')
    matches=list(PATTERN.finditer(source))
    require(len(matches)==len(prior['scopes'])==len(row['scopes']),'scope inventory changed')
    # Reconstruct only the previously certified input for validation. It is
    # never emitted. The existing verifier must reproduce this entire source.
    rewind=source;reversed_conditions=[]
    for m in matches:
        positive=m['threshold']+' < '+m['result']
        negative='not ('+m['result']+' > '+m['threshold']+')'
        require(m['condition'] in (positive,negative),'unexpected post-predicate condition')
        if m['condition']==negative:reversed_conditions.append((*m.span('condition'),m['result']+' <= '+m['threshold']))
    require(len(reversed_conditions)==prior['repairCount'],'upstream inverse inventory changed')
    for a,b,text in reversed(reversed_conditions):rewind=rewind[:a]+text+rewind[b:]
    require(digest(rewind.encode())==prior['luaPhaseSha256'],'cannot reconstruct upstream certified phase')
    checked,checked_todo,checked_calls,native_reviews=verify_predicates(rewind,todo,calls,fn,prior,unit,owner,image,manifest)
    require(checked==source and checked_todo==list(todo) and checked_calls==list(calls),'upstream certificate replay differs')
    inline=re.compile(r'(?m)^(?P<i> *)if (?P<c>(?:not )?[A-Za-z_]\w*) then break end$')
    require(len(list(inline.finditer(source)))==row['analysisInlineBreaks'],'analysis control inventory changed')
    analysis=inline.sub(lambda m:m['i']+'if '+m['c']+' then\n'+m['i']+'    break\n'+m['i']+'end',source)
    cfg=build_cfg(analysis)
    predecessors={k:set() for k in cfg['nodes']}
    for node,(_,edges) in cfg['nodes'].items():
        for edge in edges:
            if edge is not None:predecessors[edge].add(node)
    indices=[n for n in range(len(calls)-1) if list(calls[n:n+2])==['RESOURCE_ScriptThing','GetHealth']]
    require(len(indices)==len(matches),'call-pair inventory changed')
    prepared=[];reviews=[]
    for ordinal,(m,scope,old,proof,call_index) in enumerate(zip(matches,row['scopes'],prior['scopes'],native_reviews,indices)):
        require(scope['call']==old['call']['site'] and scope['callListIndex']==call_index,'call occurrence changed')
        p=old['luaProducer'];indent=m['indent']
        prefix=(indent+p['output']+' = resources:ScriptThing('+p['resource']+')\n'
                +indent+p['actor']+' = '+p['output']+'\n'
                +indent+p['result']+' = quest:GetHealth('+p['actor']+')\n'
                +indent+m['threshold']+' = 0.0\n'+indent+'if '+m['condition']+' then')
        require(prefix==scope['oldLuaPrefix'] and digest(prefix.encode())==scope['luaPrefixSha256']
                and source.count(prefix)==analysis.count(prefix)==1,'producer prefix changed')
        pos=source.index(prefix);analysis_pos=analysis.index(prefix)
        require(pos+len(prefix)==m.end(),'prefix/condition correspondence changed')
        first=analysis[:analysis_pos].count('\n');last=first+prefix.count('\n')
        nodes=set(range(first,last+1))
        require(nodes<=set(cfg['nodes']),'prefix nodes missing')
        incoming=[(a,b) for b in nodes for a in predecessors[b] if a not in nodes]
        require(incoming and all(b==first for a,b in incoming),'external entry inside producer prefix')
        health_line=indent+p['result']+' = quest:GetHealth('+p['actor']+')\n'
        after_health=analysis_pos+prefix.index(health_line)+len(health_line)
        liveness=[]
        for alias in (p['output'],p['actor']):
            marker=indent+alias+' = nil\n'
            hypothetical=analysis[:after_health]+marker+analysis[after_health:]
            liveness.append(dict(role='thingAlias',alias=alias,proof=prove_unused_staging(hypothetical,after_health+len(indent),alias)))
        condition_at=analysis_pos+len(prefix)-len(m['condition'])-len(' then')
        # Both CFG alternatives remain; only the consumed expression is replaced
        # with an unknown decision for the numeric value-use proof.
        independent=analysis[:condition_at]+'native_resource_health_decision'+analysis[condition_at+len(m['condition']):]
        before_if=independent.rfind('\n',0,condition_at)+1
        for alias in (p['result'],m['threshold']):
            marker=indent+alias+' = nil\n'
            hypothetical=independent[:before_if]+marker+independent[before_if:]
            liveness.append(dict(role='numericAlias',alias=alias,proof=prove_unused_staging(hypothetical,before_if+len(indent),alias)))
        negative=m['condition'].startswith('not (')
        condition=('not ' if negative else '')+'resources:'+METHOD+'('+p['resource']+', 0.0)'
        replacement=(indent+p['output']+', '+p['actor']+' = nil, nil\n'+indent+'if '+condition+' then')
        prepared.append((pos,pos+len(prefix),replacement))
        reviews.append(dict(call=scope['call'],method=METHOD,minimumHealth=0.0,native=proof,
                            removedCallPairIndex=call_index,oldLuaPrefixSha256=scope['luaPrefixSha256'],
                            prefixIncomingEdges=[dict(source=a+1,target=b+1) for a,b in incoming],
                            clearedThingAliases=[p['output'],p['actor']],deadValues=liveness,
                            helperContract='one native stack Thing;query/ordered x87 decision/destruction inside call;no Lua Thing output or opaque handle allocation'))
    require(all(a[1]<=b[0] for a,b in zip(prepared,prepared[1:])),'overlapping Lua scopes')
    out=source;updated=list(calls)
    for a,b,text in reversed(prepared):out=out[:a]+text+out[b:]
    for index in reversed(indices):updated[index:index+2]=[CALL]
    require(out.count('resources:'+METHOD+'(')==len(matches),'new helper occurrence mismatch')
    return out,list(todo),updated,reviews
