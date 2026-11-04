from pathlib import Path
import copy
import hashlib
import json
import re
import struct

import pytest
from lupa.lua54 import LuaRuntime
from tools.script_recovery.test_native_reviewed_interface_table_staging import FixtureImage
class RData(FixtureImage):
    def __init__(self):
        super().__init__([(int(p['address'],16),bytes.fromhex(p['bytes'])) for p in read(W/'image_parts.json')])
def load_manifest():return {'GetHealth':read(W/'candidate_row.json')['hostContract']}
@pytest.fixture(autouse=True)
def no_retail_image(monkeypatch):
    from tools.script_recovery import lift_native_lua
    monkeypatch.setattr(lift_native_lua.RData,'__init__',lambda *a,**k:(_ for _ in ()).throw(AssertionError('retail image opened')))
from tools.script_recovery.native_reviewed_saved_health_predicates import recover

W=Path(__file__).resolve().parent/'fixtures/hit_lifetimes/saved_health'
read=lambda p:json.loads(p.read_text())
digest=lambda text:hashlib.sha256(text.encode()).hexdigest()

def inputs():
    return ((W/'phase.lua').read_text(encoding='utf-8'),read(W/'phase.json'),
            read(W/'candidate_row.json'),read(W/'candidate_unit.json'))

def run(source,phase,row,unit,image=None,manifest=None):
    return recover(source,phase['todo'],phase['calls'],phase['fn'],row,unit,
                   'SickChildsSister',image or RData(),manifest or load_manifest())

def test_opt_out_does_not_access_evidence():
    assert recover('source',['diagnostic'],['call'],{},None,{},None,None,None)==('source',['diagnostic'],['call'],[])

def test_only_three_predicate_expressions_change_and_call_count_is_preserved():
    source,phase,row,unit=inputs()
    out,todo,calls,reviews=run(source,phase,row,unit)
    assert calls==phase['calls'] and todo==phase['todo']
    expected=source
    for r in reviews:
        if r['changed']:
            assert expected.count('if '+r['oldLuaCondition']+' then')==1
            expected=expected.replace('if '+r['oldLuaCondition']+' then','if '+r['newLuaCondition']+' then')
    assert out==expected
    assert source.count('quest:GetHealth(')==out.count('quest:GetHealth(')==8
    assert len(reviews)==8 and sum(r['changed'] for r in reviews)==3
    LuaRuntime().compile(out)

@pytest.mark.parametrize('key',['nativeSha256','sourceSha256','luaPhaseSha256','callsSha256','todoSha256'])
def test_stale_top_level_certificate_rejects(key):
    source,phase,row,unit=inputs()
    row[key]='0'*64
    with pytest.raises(ValueError):run(source,phase,row,unit)

@pytest.mark.parametrize('kind',['owner','entity','host-return','host-argument','health-slot','actor-profile',
    'dependencies','dependency-bytes','depth-inventory','depth-hash','source-call','source-argument',
    'source-result','source-relation','native-exit','native-flag','native-threshold','repair-count','scope-order'])
def test_corrupt_reviewed_roles_reject(kind):
    source,phase,row,unit=inputs()
    if kind=='owner':row['owner']='other'
    elif kind=='entity':unit['entities']['SickChildsSister']['vtable']='0x0'
    elif kind=='host-return':row['hostContract']['returnType']='int'
    elif kind=='host-argument':row['hostContract']['parameters'][0]['optional']=True
    elif kind=='health-slot':row['healthSlot']+=4
    elif kind=='actor-profile':row['actorProfile']='entry-ebp'
    elif kind=='dependencies':row['dependencies'].pop()
    elif kind=='dependency-bytes':row['dependencies'][0]['bytes']='00'
    elif kind=='depth-inventory':row['entryCallDepthEvidence']['depths'].pop(next(iter(row['entryCallDepthEvidence']['depths'])))
    elif kind=='depth-hash':row['entryCallDepthEvidence']['depthsSha256']='0'*64
    elif kind=='source-call':row['scopes'][0]['call']['depth']+=4
    elif kind=='source-argument':row['scopes'][0]['arguments'][1]='wrong_actor'
    elif kind=='source-result':row['scopes'][0]['sourceResult']='not_declared'
    elif kind=='source-relation':row['scopes'][0]['sourceComparison']='1 < 2'
    elif kind=='native-exit':row['scopes'][0]['native']['falseExit']='0x0'
    elif kind=='native-flag':row['scopes'][0]['native']['flagOffset']+=1
    elif kind=='native-threshold':row['scopes'][0]['native']['threshold']=1
    elif kind=='repair-count':row['repairCount']=4
    elif kind=='scope-order':row['scopes'][0],row['scopes'][1]=row['scopes'][1],row['scopes'][0]
    with pytest.raises(ValueError):run(source,phase,row,unit)

@pytest.mark.parametrize('kind',['relation','actor','threshold','false-exit','diagnostic','duplicate','extra-evaluation','intervening-write'])
def test_rehashed_lua_still_requires_operand_control_and_occurrence_correspondence(kind):
    source,phase,row,unit=inputs()
    old='fret_00 = quest:GetHealth(pCVar7)'
    if kind=='relation':source=source.replace('if fret_00 <= fVar2 then','if fret_00 >= fVar2 then',1)
    elif kind=='actor':source=source.replace(old,'fret_00 = quest:GetHealth(wrongActor)',1)
    elif kind=='threshold':source=source.replace(old+'\n                fVar2 = 0.0',old+'\n                fVar2 = 1.0',1)
    elif kind=='false-exit':source=source.replace('if fret_00 <= fVar2 then goto LAB_00ec700a','if fret_00 <= fVar2 then goto LAB_00ec700b',1)
    elif kind=='diagnostic':source=source.replace(old,old+'\n                -- TODO(native): unreviewed operation',1)
    elif kind=='duplicate':source+='\n'+old+'\nfVar2 = 0.0\nif fret_00 <= fVar2 then return end\n'
    elif kind=='extra-evaluation':source+='\nquest:GetHealth(pCVar7)\n'
    elif kind=='intervening-write':source=source.replace(old,old+'\n                fret_00 = 1.0',1)
    assert digest(source)!=row['luaPhaseSha256']
    row['luaPhaseSha256']=digest(source)
    with pytest.raises(ValueError):run(source,phase,row,unit)

@pytest.mark.parametrize('address',[0x7e7490,0x890400,0x126132c,0x4aa840,0x99a2e0,0x122dedc])
def test_changed_primary_dependency_rejects(address):
    source,phase,row,unit=inputs()
    original=RData()
    class Image:
        def bytes_at(self,a,n):
            raw=original.bytes_at(a,n)
            if raw is not None and a<=address<a+n:
                changed=bytearray(raw);changed[address-a]^=1;return bytes(changed)
            return raw
    with pytest.raises((ValueError,AssertionError)):run(source,phase,row,unit,Image())

def test_recovered_conditions_match_320_original_native_trials():
    source,phase,row,unit=inputs()
    out,_,_,reviews=run(source,phase,row,unit)
    native=read(W/'native_predicate_trials.json')
    vm=LuaRuntime()
    count=0
    for review in reviews:
        scope=next(s for s in row['scopes'] if s['call']['site']==review['call'])
        old=review['oldLuaCondition']
        names=re.findall(r'\b(?:fret_\w+|fVar\w+)\b',old)
        health=next(n for n in names if n.startswith('fret_'))
        zero=next(n for n in names if n.startswith('fVar'))
        condition=review['newLuaCondition']
        if review['changed']:condition='not ('+condition+')'
        evaluate=vm.execute('return function(h) local '+health+'=h local '+zero+'=0.0 return '+condition+' end')
        for trial in native['rows']:
            if int(trial['call'],16)!=int(review['call'],16):continue
            value=struct.unpack('<f',struct.pack('<I',int(trial['bits'],16)))[0]
            assert bool(evaluate(value))==trial['positive']
            count+=1
    assert count==320

@pytest.mark.parametrize('index',range(8))
@pytest.mark.parametrize('mutation',['status-mask','positive-store','negative-store','join-target',
    'reload-slot','cleanup-target','false-branch','missing-threshold','nan-threshold','infinite-threshold'])
def test_native_saved_predicate_rejects_actual_byte_and_threshold_mutations(index,mutation):
    from capstone import Cs,CS_ARCH_X86,CS_MODE_32
    from tools.script_recovery.native_saved_x87_predicate import prove
    source,phase,row,unit=inputs()
    image=RData()
    scope=row['scopes'][index]['native']
    start,end=int(scope['start'],16),int(scope['endExclusive'],16)
    raw=image.bytes_at(start,end-start)
    assert prove(raw,start,image.bytes_at,0x4aa840)==scope
    code=list(Cs(CS_ARCH_X86,CS_MODE_32).disasm(raw,start))
    changes={
        'status-mask':(code[3].address+code[3].size-1,0x40),
        'positive-store':(code[1].address+code[1].size-1,0),
        'negative-store':(code[5].address+code[5].size-1,1),
        'join-target':(code[4].address+code[4].size-1,raw[code[4].address-start+code[4].size-1]^1),
        'reload-slot':(code[8].address+code[8].size-1,0x12),
        'cleanup-target':(code[7].address+1,raw[code[7].address-start+1]^1),
        'false-branch':(code[10].address+(1 if code[10].size==6 else 0),0x85 if code[10].size==6 else 0x75),
    }
    reader=image.bytes_at
    if mutation in changes:
        site,value=changes[mutation]
        changed=bytearray(raw)
        assert changed[site-start]!=value
        changed[site-start]=value
        raw=bytes(changed)
    elif mutation=='missing-threshold':reader=lambda a,n:None
    elif mutation=='nan-threshold':reader=lambda a,n:bytes.fromhex('0000c07f')
    elif mutation=='infinite-threshold':reader=lambda a,n:bytes.fromhex('0000807f')
    with pytest.raises(ValueError):prove(raw,start,reader,0x4aa840)


@pytest.mark.parametrize('index',range(8))
@pytest.mark.parametrize('kind',['alias-binding','getter-output','resource','getter-method','diagnostic','inline-comment','repinned-prefix'])
def test_rehashed_producer_prefix_requires_native_source_and_lua_roles(index,kind):
    source,phase,row,unit=inputs()
    scope=row['scopes'][index]
    producer=scope['luaProducer']
    prefix=scope['luaProducerPrefix']
    assert source.count(prefix)==1
    if kind=='alias-binding':changed=prefix.replace(producer['actor']+' = '+producer['output'],producer['actor']+' = wrongThing')
    elif kind=='getter-output':changed=prefix.replace(producer['output']+' = resources:', 'wrongOutput = resources:')
    elif kind=='resource':changed=prefix.replace('ScriptThing('+producer['resource']+')','ScriptThing(otherResource)')
    elif kind=='getter-method':changed=prefix.replace('resources:ScriptThing','resources:OtherThing')
    elif kind=='diagnostic':changed=prefix.replace('\n','\n-- TODO(native): unreviewed producer\n',1)
    elif kind=='inline-comment':changed=prefix.replace(' = resources:',' = --[[unexpected producer comment]] resources:')
    elif kind=='repinned-prefix':
        changed=prefix.replace(producer['actor']+' = '+producer['output'],producer['actor']+' = wrongThing')
        scope['luaProducerPrefix']=changed
        scope['luaProducerPrefixSha256']=digest(changed)
    assert changed!=prefix
    source=source.replace(prefix,changed,1)
    row['luaPhaseSha256']=digest(source)
    with pytest.raises(ValueError):run(source,phase,row,unit)

@pytest.mark.parametrize('index',range(8))
@pytest.mark.parametrize('kind',['getter-site','getter-args','cleanup-site','prefix-hash','fully-repinned-resource'])
def test_producer_certificate_requires_original_source_and_native_stack_identity(index,kind):
    source,phase,row,unit=inputs()
    scope=row['scopes'][index]
    if kind=='getter-site':scope['sourceProducerCalls'][0]['metadata']['site']='0x0'
    elif kind=='getter-args':scope['sourceProducerCalls'][0]['arguments'][0]='wrongResource'
    elif kind=='cleanup-site':scope['sourceProducerCalls'][2]['metadata']['site']='0x0'
    elif kind=='prefix-hash':scope['luaProducerPrefixSha256']='0'*64
    elif kind=='fully-repinned-resource':
        prefix=scope['luaProducerPrefix']
        changed=prefix.replace('ScriptThing('+scope['luaProducer']['resource']+')','ScriptThing(otherResource)')
        assert changed!=prefix and source.count(prefix)==1
        source=source.replace(prefix,changed,1)
        scope['luaProducer']['resource']='otherResource'
        scope['luaProducerPrefix']=changed
        scope['luaProducerPrefixSha256']=digest(changed)
        row['luaPhaseSha256']=digest(source)
    with pytest.raises(ValueError):run(source,phase,row,unit)
