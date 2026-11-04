from pathlib import Path
import copy
import hashlib
import json
import re

import pytest
from lupa.lua54 import LuaRuntime
from tools.script_recovery.native_reviewed_resource_health_lifetimes import recover
from tools.script_recovery.test_native_reviewed_interface_table_staging import FixtureImage
class RData(FixtureImage):
    def __init__(self):
        super().__init__([(int(p['address'],16),bytes.fromhex(p['bytes'])) for p in read(W/'image_parts.json')])
def load_manifest():
    unit=read(W/'candidate_unit.json')
    return {'GetHealth':unit['reviewedSavedHealthPredicates']['0x00ec68a0']['hostContract']}
@pytest.fixture(autouse=True)
def no_retail_image(monkeypatch):
    from tools.script_recovery import lift_native_lua
    monkeypatch.setattr(lift_native_lua.RData,'__init__',lambda *a,**k:(_ for _ in ()).throw(AssertionError('retail image opened')))
from tools.script_recovery.native_reviewed_saved_health_predicates import PATTERN

W=Path(__file__).resolve().parent/'fixtures/hit_lifetimes/resource_health'
read=lambda p:json.loads(p.read_text())
digest=lambda b:hashlib.sha256(b).hexdigest()
serial=lambda obj:json.dumps(obj,separators=(',',':')).encode()
def inputs():
    phase=read(W/'phase.json');row=read(W/'candidate_row.json');unit=read(W/'candidate_unit.json')
    return dict(source=(W/'phase.lua').read_text(encoding='utf-8'),todo=phase['todo'],calls=phase['calls'],fn=phase['fn'],
                row=row,unit=unit,owner=row['owner'],image=RData(),manifest=load_manifest())
def run(data):return recover(**data)
def repin_all(data):
    prior=data['unit']['reviewedSavedHealthPredicates'][data['fn']['address'].lower()]
    source=data['source'];edits=[]
    for m in PATTERN.finditer(source):
        if m['condition']=='not ('+m['result']+' > '+m['threshold']+')':edits.append((*m.span('condition'),m['result']+' <= '+m['threshold']))
    rewind=source
    for a,b,text in reversed(edits):rewind=rewind[:a]+text+rewind[b:]
    prior['luaPhaseSha256']=digest(rewind.encode())
    data['row']['predicateEvidenceSha256']=digest(serial(prior))
    data['row']['luaPhaseSha256']=digest(source.encode())

def test_unreviewed_preserves_source_without_opening_image():
    assert recover('source',['todo'],['call'],{},None,{},None,None,None)==('source',['todo'],['call'],[])

def test_all_eight_scopes_have_exact_native_and_dead_value_proofs():
    data=inputs();before=copy.deepcopy({k:data[k] for k in ('row','unit','fn','todo','calls')})
    out,todo,calls,reviews=run(data)
    assert todo==data['todo'] and len(calls)==len(data['calls'])-8
    assert out.count('resources:ResourceThingHealthAbove(')==out.count(' = nil, nil\n')==8
    assert 'quest:GetHealth(' not in out and 'resources:ScriptThing(' not in out
    assert len(reviews)==8 and sum(len(r['deadValues']) for r in reviews)==32
    assert all(r['prefixIncomingEdges'] and len(r['clearedThingAliases'])==2 for r in reviews)
    expected=list(data['calls'])
    for s in reversed(data['row']['scopes']):expected[s['callListIndex']:s['callListIndex']+2]=['RESOURCE_ResourceThingHealthAbove']
    assert calls==expected
    assert all(data[k]==before[k] for k in before)
    LuaRuntime().compile(out)

@pytest.mark.parametrize('fault',['phase','calls','todo','upstream','owner','method','minimum','boolMinimum','scopeCount','callIndex','callSite','prefix','prefixHash','inlineBreaks'])
def test_stale_ambiguous_or_unsupported_metadata_rejects(fault):
    data=inputs();row=data['row']
    hashes=dict(phase='luaPhaseSha256',calls='callsSha256',todo='todoSha256',upstream='predicateEvidenceSha256')
    if fault in hashes:row[hashes[fault]]='0'*64
    elif fault=='owner':row['owner']='other'
    elif fault=='method':row['method']='OtherMethod'
    elif fault=='minimum':row['minimumHealth']=1
    elif fault=='boolMinimum':row['minimumHealth']=False
    elif fault=='scopeCount':row['scopes'].pop()
    elif fault=='callIndex':row['scopes'][0]['callListIndex']+=1
    elif fault=='callSite':row['scopes'][0]['call']='0x0'
    elif fault=='prefix':row['scopes'][0]['oldLuaPrefix']+='\n'
    elif fault=='prefixHash':row['scopes'][0]['luaPrefixSha256']='0'*64
    elif fault=='inlineBreaks':row['analysisInlineBreaks']+=1
    with pytest.raises(ValueError):run(data)

@pytest.mark.parametrize('index',range(8))
@pytest.mark.parametrize('kind',['actor','output','resource','diagnostic'])
def test_rehashed_producer_change_still_requires_native_source_prefix(index,kind):
    data=inputs();prior=data['unit']['reviewedSavedHealthPredicates'][data['fn']['address'].lower()]
    scope=prior['scopes'][index];p=scope['luaProducer'];prefix=scope['luaProducerPrefix']
    assert data['source'].count(prefix)==1
    if kind=='actor':changed=prefix.replace(p['actor']+' = '+p['output'],p['actor']+' = wrongThing')
    elif kind=='output':changed=prefix.replace(p['output']+' = resources:', 'wrongOutput = resources:')
    elif kind=='resource':changed=prefix.replace('ScriptThing('+p['resource']+')','ScriptThing(otherResource)')
    else:changed=prefix.replace('\n','\n-- TODO(native): unexpected operation\n',1)
    assert changed!=prefix
    data['source']=data['source'].replace(prefix,changed,1);repin_all(data)
    with pytest.raises(ValueError):run(data)

@pytest.mark.parametrize('index',range(8))
@pytest.mark.parametrize('role',['output','actor','result','threshold'])
@pytest.mark.parametrize('conditional',[False,True])
def test_post_consumer_observable_read_rejects_even_after_certificates_rehashed(index,role,conditional):
    data=inputs();prior=data['unit']['reviewedSavedHealthPredicates'][data['fn']['address'].lower()]
    m=list(PATTERN.finditer(data['source']))[index]
    alias=m['threshold'] if role=='threshold' else prior['scopes'][index]['luaProducer'][role]
    tail=re.match(r'\n[ \t]*goto [A-Za-z_]\w*\n[ \t]*end\n',data['source'][m.end():])
    at=m.end()+tail.end() if tail else data['source'].index('\n',m.end())+1
    insert='quest:Observe('+alias+')\n'
    if conditional:insert='if unknownBranch then\n'+insert+'end\n'
    data['source']=data['source'][:at]+insert+data['source'][at:]
    repin_all(data)
    with pytest.raises(ValueError,match='live staging value read/escape'):run(data)

def test_closure_blocks_whole_cfg_proof_after_rehash():
    data=inputs();at=data['source'].index('\n',data['source'].index('local alive = true'))+1
    data['source']=data['source'][:at]+'local function observe() return pCVar7 end\n'+data['source'][at:]
    repin_all(data)
    with pytest.raises(ValueError,match='nested function/closure'):run(data)

def test_diagnostics_cannot_be_removed_to_obtain_certificate():
    data=inputs();data['todo']=data['todo'][1:]
    data['row']['todoSha256']=digest(serial(data['todo']))
    with pytest.raises(ValueError):run(data)


@pytest.mark.parametrize('address',[0x7e7490,0x890400,0x126132c,0x4aa840,0x99a2e0,0x122dedc])
def test_changed_primary_dependency_rejects(address):
    data=inputs();original=data['image']
    class Image:
        def bytes_at(self,a,n):
            raw=original.bytes_at(a,n)
            if raw is not None and a<=address<a+n:
                raw=bytearray(raw);raw[address-a]^=1;return bytes(raw)
            return raw
    data['image']=Image()
    with pytest.raises(ValueError):run(data)
