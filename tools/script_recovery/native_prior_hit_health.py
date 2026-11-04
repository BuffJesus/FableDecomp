"""Replay an independently reviewed earlier health transform before source accounting."""
from tools.script_recovery.lua_staging_liveness import prove_unused_staging

def replay(fn,phase,*,input,todo,calls,unit,owner,image):
    def require(ok,why):
        if not ok:raise ValueError('prior hit-health correspondence: '+why)
    require(type(input) is dict and set(input)=={'source','todo','calls'},'input shape')
    row=unit.get('reviewedHitHealthLifetimes',{}).get(fn['address'].lower())
    require(row is not None,'missing independently reviewed transform')
    from tools.script_recovery.native_reviewed_hit_health_lifetimes import recover
    source,newtodo,newcalls,proof=recover(input['source'],list(input['todo']),list(input['calls']),fn,row,unit,owner,image,prove_unused_staging)
    require(source==phase and newtodo==todo and newcalls==calls,'replay output phase/calls/diagnostics')
    require(len(proof)==1,'transform inventory')
    actual=proof[0]['sourceCalls'];sites=[c['metadata'] for c in actual if c['metadata'].get('slot')=='0x420']
    require(len(sites)==1,'health site inventory')
    records={int(c['site'],16):c for c in fn['indirectCalls']}
    require(all(records.get(int(c['site'],16))==c for c in sites),'original health metadata')
    require(source.count('resources:'+row['method']+'(')==1 and newcalls.count('RESOURCE_'+row['method'])==1,'emitted prior helper inventory')
    return dict(source=input['source'],sites={int(c['site'],16) for c in sites},roles={int(c['site'],16):dict(result=row['scope']['healthAlias'],actor='me') for c in sites},proof=proof)


def reviewed_frame_view(fn,prior):
    """Rebase exported stack coordinates with the already revalidated depth map.

    Original source/call evidence is retained. Consumers must still check every
    projected coordinate they use against decoded native operands.
    """
    import hashlib,json
    def require(ok,why):
        if not ok:raise ValueError('prior hit-health frame view: '+why)
    evidence=prior['proof'][0].get('reviewedEntryCallDepthEvidence')
    if evidence is None:return fn,None
    serial=lambda value:json.dumps(value,separators=(',',':')).encode()
    digest=lambda value:hashlib.sha256(value).hexdigest()
    originals=[fn['calls'],fn['indirectCalls']];records=fn['calls']+fn['indirectCalls'];depths=evidence['depths']
    require(digest(serial(originals))==evidence['originalCallsSha256'],'original call evidence')
    require(set(depths)=={c['site'].lower() for c in records} and len(depths)==len(records),'depth site inventory')
    require(digest(json.dumps(depths,sort_keys=True,separators=(',',':')).encode())==evidence['depthsSha256'],'depth profile')
    require(all(type(d) is int and 0<=d<=4096 and d%4==0 for d in depths.values()),'depth domain')
    changes=[]
    def project(c):
        require(type(c['depth']) is int,'original depth')
        depth=depths[c['site'].lower()];delta=depth-c['depth'];out=dict(c,depth=depth);fields={}
        for field in ['ecxStack','edxStack']:
            if field not in c or c[field] is None:continue
            require(type(c[field]) is int,'stack coordinate schema')
            out[field]=c[field]-delta
            if delta:fields[field]=dict(original=c[field],projected=out[field])
        if 'pushedStack' in c:
            require(type(c['pushedStack']) is list and all(v is None or type(v) is int for v in c['pushedStack']),'stack argument schema')
            out['pushedStack']=[v-delta if v is not None else None for v in c['pushedStack']]
            if delta and out['pushedStack']!=c['pushedStack']:fields['pushedStack']=dict(original=c['pushedStack'],projected=out['pushedStack'])
        if delta:changes.append(dict(site=c['site'],originalDepth=c['depth'],projectedDepth=depth,delta=delta,coordinates=fields))
        return out
    view=dict(fn,**{key:[project(c) for c in fn[key]] for key in ['calls','indirectCalls']})
    return view,dict(originalCallsSha256=evidence['originalCallsSha256'],depthsSha256=evidence['depthsSha256'],projectedCallsSha256=digest(serial([view['calls'],view['indirectCalls']])),changedCalls=changes,contract='Depth-relative stack coordinate projection only;native getter/output/cleanup role checks remain mandatory;original fn/source metadata unchanged.')
