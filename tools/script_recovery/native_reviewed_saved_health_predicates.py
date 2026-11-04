"""Opt-in ordered health predicates recovered from saved native x87 flags.

Only comparison text changes. Existing producer evaluation and all resource,
Thing, CString and cleanup lifetimes remain in their original emitted order.
Normal child ABI, valid actors/disjoint heaps and default masked x87 remain
contracts; this pass does not establish host lifecycle or gameplay behavior.
"""
import hashlib
import json
import re

from tools.script_recovery.native_hit_health_entry import prove_frame
from tools.script_recovery.native_saved_x87_predicate import prove
from tools.script_recovery.native_health_predicate_roles import prove_roles
from tools.script_recovery.native_reviewed_hit_query_lifetimes import masked

PROFILE = (
    (0x7e7490,57,'2e4b5ca9627d95c544d96c0c987f5d52c5dc1c9757d4bdeae1323802089375c6'),
    (0x890400,46,'79c07c4b10801c8f2a80873426ac09fa2590be1d9331bc3bde0b5b7a86c5cef4'),
    (0x126132c,4,'754dd8978d373e339fb743585106af5b6adfa3ef41b9fb4dffa07655c2bb067d'),
    (0x4aa840,66,'5aa58f8e6b624fb6abbc2496c0f63791f6a7ae480b3e86d9e93f148d23fe4afa'),
    (0x99a2e0,7,'b391b1043a81dbc361b75b6ef3b6f5551ecf4643eb44e742e4f605ce2382115a'),
)
PATTERN = re.compile(r'(?m)^(?P<indent> *)(?P<result>\w+) = quest:GetHealth\((?P<actor>\w+)\)\n'
                     r'(?P=indent)(?P<threshold>\w+) = 0\.0\n(?P=indent)if (?P<condition>[^\n]+?) then')
IDENT = re.compile(r'[A-Za-z_]\w*')


def recover(source,todo,calls,fn,row,unit,owner,image,manifest):
    if row is None:
        return source,list(todo),list(calls),[]

    def require(ok,why):
        if not ok:
            raise ValueError('reviewed saved health predicates '+fn['address']+': '+why)

    digest = lambda data: hashlib.sha256(data).hexdigest()
    serialized = lambda obj: json.dumps(obj,separators=(',',':')).encode()
    start,end = int(fn['address'],16),int(fn['bodyEndExclusive'],16)
    raw = image.bytes_at(start,end-start)
    for key,data in [('nativeSha256',raw),('sourceSha256',fn['decompile'].encode()),
                     ('luaPhaseSha256',source.encode()),('callsSha256',serialized(calls)),
                     ('todoSha256',serialized(todo))]:
        require(data is not None and digest(data)==row[key],key+' changed')
    require(owner==row['owner'] and owner in unit.get('entities',{}),'owner changed')
    entity = unit['entities'][owner]
    require(entity['vtable'].lower()==row['entityVtable'].lower()
            and entity['functions']['Main']['address'].lower()==fn['address'].lower(),'registered function changed')
    contract = manifest.get('GetHealth')
    require(contract==row['hostContract'] and contract.get('scope')=='Quest'
            and contract.get('returnType')=='float'
            and contract.get('parameters')==[dict(name='pThing',type='CScriptThing*',optional=False)],'host contract changed')
    require(row['healthSlot']==0x420 and row['actorProfile']=='entry-ebx','unsupported producer profile')
    require(len(row['dependencies'])==len(PROFILE),'dependency inventory changed')
    for part,expected in zip(row['dependencies'],PROFILE):
        address,size,sha = expected
        data = image.bytes_at(address,size)
        require((int(part['address'],16),part['size'],part['sha256'])==expected
                and data is not None and digest(data)==sha and data.hex()==part['bytes'],'native dependency changed')
    evidence = row['entryCallDepthEvidence']
    original = [fn['calls'],fn['indirectCalls']]
    require(digest(serialized(original))==evidence['originalCallsSha256'],'original depth records changed')
    depths = evidence['depths']
    allcalls = fn['calls']+fn['indirectCalls']
    require(set(depths)=={c['site'].lower() for c in allcalls}
            and len(depths)==len(allcalls)
            and all(type(d) is int and 0<=d<=4096 and d%4==0 for d in depths.values()),'depth inventory changed')
    require(digest(json.dumps(depths,sort_keys=True,separators=(',',':')).encode())==evidence['depthsSha256'],'depth certificate changed')
    entry_fn = dict(fn,**{key:[dict(c,depth=depths[c['site'].lower()]) for c in fn[key]] for key in ('calls','indirectCalls')})
    scopes = row['scopes']
    require(isinstance(scopes,list) and 0<len(scopes)<=64,'scope inventory changed')
    first = scopes[0]['native']
    frame = prove_frame(raw,entry_fn,image,entity_vtable=int(row['entityVtable'],16),
        scope_start=int(first['start'],16),scope_end=int(first['endExclusive'],16),
        frame_depth=row['frameDepth'],mask_offset=0,
        this_definition=int(row['thisDefinition'],16),actor_definition=int(row['actorDefinition'],16),
        actor_profile=row['actorProfile'])
    from tools.script_recovery.convert_quest_unit import _text_order_sites
    ordered = _text_order_sites(fn['decompile'],fn)
    require(ordered is not None,'source/native alignment unavailable')
    health = [s for s in ordered if s[3].get('slot')==hex(row['healthSlot'])]
    matches = list(PATTERN.finditer(source))
    require(len(health)==len(scopes)==len(matches),'health occurrence inventory changed')
    require(len(re.findall(r'\bquest:GetHealth\s*\(',masked(source)))==len(matches),'unsupported health expression')
    edits,reviews = [],[]
    for source_call,lua,scope in zip(health,matches,scopes):
        _,text_end,arguments,site,_,_ = source_call
        require(site==scope['call'] and arguments==scope['arguments'],'source call correspondence changed')
        require(arguments==['*(void **)((int)this + 4)',lua['actor']],'Lua actor correspondence changed')
        native = scope['native']
        a,b = int(native['start'],16),int(native['endExclusive'],16)
        require(start<a<b<=end,'scope outside function')
        actual = prove(raw[a-start:b-start],a,image.bytes_at,0x4aa840)
        require(actual==native and actual['threshold']==0,'native predicate changed')
        roles = prove_roles(frame,int(site['site'],16),actual,image,getter_target=0x7e7490,
                            this_definition=int(row['thisDefinition'],16),health_slot=row['healthSlot'])
        call_index = ordered.index(source_call)
        require(0<call_index<len(ordered)-1,'producer source window missing')
        producer_calls = [dict(metadata=s[3],arguments=s[2]) for s in ordered[call_index-1:call_index+2]]
        require(producer_calls==scope['sourceProducerCalls'],'producer source correspondence changed')
        getter,_,cleanup = producer_calls
        require(getter['metadata'].get('target','').lower()=='0x007e7490'
                and cleanup['metadata'].get('target','').lower()=='0x004aa840'
                and int(getter['metadata']['site'],16)==int(roles['getter'],16)
                and int(cleanup['metadata']['site'],16)==int(roles['cleanup'],16),'producer source identities changed')
        producer = scope['luaProducer']
        require(set(producer)=={'output','resource','actor','result'}
                and all(isinstance(v,str) and IDENT.fullmatch(v) for v in producer.values())
                and len(set(producer.values()))==4,'invalid Lua producer roles')
        require(producer['actor']==lua['actor'] and producer['result']==lua['result'],'Lua producer consumer changed')
        require(roles['resourceEntryOffset']<0
                and producer['resource']=='xStack_'+format(-roles['resourceEntryOffset'],'x'),
                'Lua resource/native stack identity changed')
        indent = lua['indent']
        prefix = (indent+producer['output']+' = resources:ScriptThing('+producer['resource']+')\n'
                  +indent+producer['actor']+' = '+producer['output']+'\n'+indent)
        require(scope['luaProducerPrefix']==prefix
                and digest(prefix.encode())==scope['luaProducerPrefixSha256']
                and source[:lua.start('result')].endswith(prefix),'Lua getter/resource/output/alias prefix changed')
        result,threshold = scope['sourceResult'],lua['threshold']
        require(IDENT.fullmatch(result) and IDENT.fullmatch(threshold),'invalid source identity')
        require(re.search(r'(?m)^\s*float10 '+re.escape(result)+r';',fn['decompile']) is not None,'source float type changed')
        tail = fn['decompile'][text_end+1:text_end+650]
        comparison = re.search(r'if \(([^\n]+?)\)',tail)
        require(comparison is not None and comparison[1]==scope['sourceComparison'],'source comparison changed')
        constant = threshold+' = (float10)_DAT_'+format(int(actual['thresholdAddress'],16),'08x')+';'
        require(tail.count(constant)==1 and tail.index(constant)<comparison.start(),'source threshold changed')
        positive = threshold+' < '+lua['result']
        inverse = lua['result']+' <= '+threshold
        require(lua['condition'] in (positive,inverse),'Lua relation changed')
        repair = lua['condition']==inverse
        require(comparison[1]==(result+' <= '+threshold if repair else threshold+' < '+result),'source/Lua relation correspondence changed')
        if repair:
            false_label = re.match(r'\s*goto (LAB_[0-9a-f]+)',source[lua.end():lua.end()+100])
            require(false_label is not None and int(false_label[1][4:],16)==int(actual['falseExit'],16),'false exit correspondence changed')
            replacement = 'not ('+lua['result']+' > '+threshold+')'
            edits.append((*lua.span('condition'),replacement))
        reviews.append(dict(call=site['site'],native=actual,roles=roles,sourceComparison=comparison[1],
                            sourceProducerCalls=producer_calls,luaProducer=dict(producer),
                            luaProducerPrefixSha256=scope['luaProducerPrefixSha256'],
                            oldLuaCondition=lua['condition'],changed=repair,
                            newLuaCondition=replacement if repair else lua['condition']))
    require(sum(r['changed'] for r in reviews)==row['repairCount'] and row['repairCount']>0,'repair inventory changed')
    out = source
    for a,b,text in reversed(edits):
        out = out[:a]+text+out[b:]
    return out,list(todo),list(calls),reviews
