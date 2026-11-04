from pathlib import Path
import copy,re,sys
import pytest
from tools.script_recovery import native_reviewed_register_resource_health_lifetimes as M
from tools.script_recovery import native_health_copy_correspondence as COPY
from tools.script_recovery.test_native_reviewed_trader1_resource_health_lifetimes import inputs,copyproof,repin,digest
from tools.script_recovery.lift_native_lua import RData
@pytest.fixture(autouse=True)
def no_retail(monkeypatch):monkeypatch.setattr(RData,'__init__',lambda *a,**k:(_ for _ in ()).throw(AssertionError('retail image opened')))
def row(data):return data['unit']['reviewedHitHealthLifetimes']['0x00ecbdf0']
def test_complete_inventory_and_prior_recovery_are_preserved():
 data=inputs();proof=copyproof(data);assert proof['nativeSites']==proof['emittedUses']==3
 assert proof['priorHealth']['originalNativeSites']==4 and proof['priorHealth']['consumedSites']==[0xecc5d4]
 assert len(proof['priorHealth']['proof'])==1
 out,todo,calls,reviews=M.recover(**data)
 assert out.count('resources:ResourceThingHealthAbove(')==3 and out.count('resources:IsHitByHeroExceptAbilityAndHealthAbove(')==1
 inventory=reviews[0]['nativeHealthInventory'];assert inventory['original']==4 and inventory['lowered']==3 and inventory['priorSites']==[0xecc5d4]
 assert todo==data['todo'] and calls.count('RESOURCE_IsHitByHeroExceptAbilityAndHealthAbove')==1
 assert data['prior_hit_health_input']['source'].count('quest:GetHealth(')==4
 assert data['source'].count('quest:GetHealth(')==3

def test_uncomposed_inventory_remains_closed():
 d=inputs()
 with pytest.raises(ValueError,match='producer/cleanup identities'):COPY.prove(d['fn'],d['before'],d['after'],d['source'])
@pytest.mark.parametrize('fault',['source','calls','todo','extraProof','missingReview','owner','outputSource','outputCalls','outputTodo','helperABI','healthMetadata'])
def test_unproved_or_stale_prior_transform_refuses(fault):
 d=inputs();prior=d['prior_hit_health_input']
 if fault=='source':prior['source']+='\n'
 elif fault=='calls':prior['calls'].append('GetHealth')
 elif fault=='todo':prior['todo'].append('unproved')
 elif fault=='extraProof':prior['proof']={'consumedSites':[0xecc5d4]}
 elif fault=='missingReview':d['unit']['reviewedHitHealthLifetimes'].clear()
 elif fault=='owner':d['owner']='Other'
 elif fault=='outputSource':d['source']+='\n'
 elif fault=='outputCalls':d['calls'].append('unknown')
 elif fault=='outputTodo':d['todo'].append('unresolved')
 elif fault=='helperABI':row(d)['minimumHealth']=1
 else:next(c for c in d['fn']['indirectCalls'] if int(c['site'],16)==0xecc5d4)['slot']='0x424'
 with pytest.raises((ValueError,AssertionError)):copyproof(d)

def test_consumed_ast_actor_still_matches_native_source():
 d=inputs()
 for key in ['before','after']:d[key]=[line.replace('fret_01 = GSI->GetHealth(p0);','fret_01 = GSI->GetHealth(otherActor);') for line in d[key]]
 with pytest.raises(ValueError,match='original AST actor'):copyproof(d)

def test_prior_lua_self_mapping_cannot_be_forged_after_repin():
 d=inputs();prior=d['prior_hit_health_input'];r=row(d)
 prior['source']=prior['source'].replace('quest:GetHealth(me)','quest:GetHealth(otherActor)')
 r['luaPhaseSha256']=digest(prior['source'].encode());r['scope']['oldLuaPrefix']=r['scope']['oldLuaPrefix'].replace('quest:GetHealth(me)','quest:GetHealth(otherActor)');r['scope']['luaPrefixSha256']=digest(r['scope']['oldLuaPrefix'].encode())
 with pytest.raises(ValueError,match='unsupported Lua control/statement shape'):copyproof(d)

def test_prior_result_name_must_still_map_to_original_ast():
 d=inputs();prior=d['prior_hit_health_input'];r=row(d);rename=lambda text:re.sub(r'\bfret_01\b','fret_forged',text)
 prior['source']=rename(prior['source']);d['source']=rename(d['source']);r['luaPhaseSha256']=digest(prior['source'].encode());r['scope']['oldLuaPrefix']=rename(r['scope']['oldLuaPrefix']);r['scope']['luaPrefixSha256']=digest(r['scope']['oldLuaPrefix'].encode());r['scope']['healthAlias']='fret_forged'
 with pytest.raises(ValueError,match='prior proved native-self Lua consumer'):copyproof(d)

def test_remaining_resource_actor_correspondence_stays_strict():
 d=inputs();prior=d['prior_hit_health_input'];change=lambda text:text.replace('quest:GetHealth(pCVar9)','quest:GetHealth(otherActor)',1)
 prior['source']=change(prior['source']);d['source']=change(d['source']);row(d)['luaPhaseSha256']=digest(prior['source'].encode())
 with pytest.raises(ValueError,match='prior Lua/AST health consumer'):copyproof(d)
