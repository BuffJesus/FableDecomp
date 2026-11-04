from pathlib import Path
import copy,json,sys
import pytest
from lupa.lua54 import LuaRuntime
W=Path(__file__).resolve().parent;R=W.parents[1];sys.path.insert(0,str(R))
from tools.script_recovery.native_reviewed_register_resource_health_lifetimes import recover,serial,digest
from tools.script_recovery.native_health_copy_correspondence import prove as prove_copy
from tools.script_recovery.test_native_reviewed_interface_table_staging import FixtureImage
from tools.script_recovery.lift_native_lua import RData
read=lambda p:json.loads(p.read_text());F=W/'fixtures/ingredient_resource_health'
@pytest.fixture(autouse=True)
def no_retail_image(monkeypatch):
    monkeypatch.setattr(RData,'__init__',lambda *a,**k:(_ for _ in ()).throw(AssertionError('retail image opened')))
def inputs():
    phase=read(F/'phase.json');row=read(F/'row.json')
    return dict(source=(F/'phase.lua').read_text(),todo=phase['todo'],calls=phase['calls'],fn=phase['fn'],row=row,unit=read(F/'unit.json'),owner=row['owner'],image=FixtureImage([(int(p['address'],16),bytes.fromhex(p['bytes'])) for p in read(F/'image_parts.json')]),manifest={'GetHealth':row['hostContract']},before=read(F/'before.json'),after=read(F/'after.json'))
def repin(data):
    for key,value in [('fnSha256',serial(data['fn'])),('luaPhaseSha256',data['source'].encode()),('callsSha256',serial(data['calls'])),('todoSha256',serial(data['todo'])),('beforeSha256',serial(data['before'])),('afterSha256',serial(data['after']))]:data['row'][key]=digest(value)
def test_unreviewed_untouched():
    assert recover('source',['todo'],['call'],{},None,{},None,None,None,None,None)==('source',['todo'],['call'],[])
def test_seven_existing_scopes_and_28_dead_values():
    data=inputs();snapshot=copy.deepcopy({k:v for k,v in data.items() if k!='image'})
    out,todo,calls,reviews=recover(**data)
    assert len(reviews)==7 and sum(len(r['deadValues']) for r in reviews)==28
    assert [len(r['clearedThingAliases']) for r in reviews]==[2]*7
    assert [r['call'] for r in reviews]==['0x00ecb32b','0x00ecb632','0x00ecb539','0x00ecb702','0x00ecb943','0x00ecb830','0x00ecba7b']
    assert out.count('resources:ResourceThingHealthAbove(')==7 and 'quest:GetHealth(' not in out
    assert out.count('if not resources:ResourceThingHealthAbove(')==2
    assert todo==data['todo'] and len(calls)==len(data['calls'])-7
    assert all(data[k]==v for k,v in snapshot.items());LuaRuntime().compile(out)
@pytest.mark.parametrize('key',['nativeSha256','fnSha256','luaPhaseSha256','callsSha256','todoSha256','beforeSha256','afterSha256'])
def test_stale_inputs_reject(key):
    data=inputs();data['row'][key]='0'*64
    with pytest.raises(ValueError):recover(**data)
@pytest.mark.parametrize('fault',['owner','method','minimum','boolMinimum','registration','vtable','slot','profile','inlineBreaks','missingSite','duplicateSite','positiveExit','falseExit','host'])
def test_changed_evidence_rejects(fault):
    data=inputs();row=data['row']
    if fault=='owner':row['owner']='other'
    elif fault=='method':row['method']='other'
    elif fault=='minimum':row['minimumHealth']=1
    elif fault=='boolMinimum':row['minimumHealth']=False
    elif fault=='registration':data['unit']['entities']['IngredientOwner']['functions']['Main']['address']='0x1'
    elif fault=='vtable':row['entityVtable']='0x1'
    elif fault=='slot':row['healthSlot']=0x424
    elif fault=='profile':row['actorProfile']='entry-ebx'
    elif fault=='inlineBreaks':row['analysisInlineBreaks']+=1
    elif fault=='missingSite':row['nativeProfiles'].pop()
    elif fault=='duplicateSite':row['nativeProfiles'].append(row['nativeProfiles'][0])
    elif fault in ['positiveExit','falseExit']:row['nativeProfiles'][0][fault]+=1
    else:data['manifest']['GetHealth']=dict(scope='Other')
    with pytest.raises((ValueError,AssertionError)):recover(**data)
@pytest.mark.parametrize('ordinal',range(7))
@pytest.mark.parametrize('role',['output','actor','result','threshold'])
@pytest.mark.parametrize('conditional',[False,True])
def test_each_live_value_read_rejects_after_repin(ordinal,role,conditional):
    data=inputs();proof=prove_copy(data['fn'],data['before'],data['after'],data['source'])['scopes'][ordinal]
    alias=proof[role];at=proof['end']+1;indent=' '*24
    insert=indent+'quest:Observe('+alias+')\n'
    if conditional:insert=indent+'if unknownBranch then\n'+insert+indent+'end\n'
    data['source']=data['source'][:at]+insert+data['source'][at:];repin(data)
    with pytest.raises(ValueError,match='live staging value read/escape'):recover(**data)
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
    with pytest.raises(ValueError):recover(**data)
