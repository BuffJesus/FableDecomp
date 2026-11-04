from pathlib import Path
from copy import deepcopy
import hashlib,json,re
import pytest
from tools.script_recovery import native_reviewed_hit_query_lifetimes as CLASSIFIER
from tools.script_recovery.lua_staging_liveness import prove_unused_staging
from tools.script_recovery.test_native_reviewed_interface_table_staging import FixtureImage
from lupa.lua54 import LuaRuntime
W=Path(__file__).resolve().parent/'fixtures/hit_lifetimes/sister';P=W;read=lambda p:json.loads(p.read_text());sha=lambda b:hashlib.sha256(b).hexdigest()
class Image(FixtureImage):
    def string_at(self,address):return 'SCRIPT_NAME_HERO' if address==0x125d1c8 else None
@pytest.fixture
def inputs(monkeypatch):
    from tools.script_recovery import lift_native_lua
    monkeypatch.setattr(lift_native_lua.RData,'__init__',lambda *a,**k:(_ for _ in ()).throw(AssertionError('retail image opened')))
    return dict(source=(P/'phase.lua').read_text(),todo=read(P/'phase_todo.json'),calls=read(P/'phase_calls.json'),fn=read(P/'native_function.json'),row=read(W/'candidate_row.json'),unit=read(W/'candidate_unit.json'),owner='SickChildsSister',image=Image([(int(r['address'],16),bytes.fromhex(r['bytes'])) for r in read(W/'image_parts.json')]),liveness=prove_unused_staging)
def run(inputs):return CLASSIFIER.recover(**inputs)
def repin(inputs):
    inputs['row']['luaPhaseSha256']=sha(inputs['source'].encode())
    for scope in inputs['row']['scopes']:scope['luaPrefixSha256']=sha(scope['oldLuaPrefix'].encode())
def change(inputs,index,old,new):
    scope=inputs['row']['scopes'][index];prefix=scope['oldLuaPrefix'];assert prefix.count(old)==1;updated=prefix.replace(old,new);assert inputs['source'].count(prefix)==1;inputs['source']=inputs['source'].replace(prefix,updated);scope['oldLuaPrefix']=updated;repin(inputs)
def test_three_strict_scopes_and_every_live_output_are_retained(inputs):
    before=deepcopy({k:inputs[k] for k in ['row','unit','fn','todo','calls']});out,todo,calls,reviews=run(inputs);assert out.count('resources:IsHitByHeroExceptAbility(me, 14)')==3 and len(reviews)==3 and todo==inputs['todo'] and len(calls)==len(inputs['calls'])-6;LuaRuntime().compile(out)
    assert reviews[0]['outgoingNumericConstants']=={'bVar6':0} and reviews[1]['outgoingNumericConstants']=={'bVar6':0} and reviews[2]['outgoingNumericConstants']=={'bVar6':0,'bVar12':0};assert [p['alias'] for p in reviews[2]['preservedBooleanLocals']]==['bVar3','bVar14'];assert all(r['nativeRoles']['booleanPaths']==8 for r in reviews)
    assert all(before[k]==inputs[k] for k in before)
def test_unreviewed_preserves_source_and_schemas(inputs):
    inputs['row']=None;assert run(inputs)==(inputs['source'],inputs['todo'],inputs['calls'],[])
@pytest.mark.parametrize('fault',['nativeHash','sourceHash','phaseHash','callsHash','owner','vtable','method','ability','literal','helpers','shape','actor','thisDefinition','actorDefinition','frame','maskOffset','flagOffset','depthHash','originalCallsHash','missingDepth','invalidDepth','rehashBadDepth','scopeNativeHash','scopeSourceCalls','scopePrefixHash','scopeObjects','scopeMask','scopeLabel','scopeFlag','scopeShape','scopeOccurrence','scopeIndex','scopeDiagnostic','inlineBreaks','numericAlias'])
def test_stale_and_ambiguous_evidence_refuses(inputs,fault):
    row=inputs['row'];s=row['scopes'][0];e=row['entryCallDepthEvidence']
    hashes={'nativeHash':'nativeExtentSha256','sourceHash':'sourceSha256','phaseHash':'luaPhaseSha256','callsHash':'callsSha256'}
    if fault in hashes:row[hashes[fault]]='0'*64
    elif fault=='owner':inputs['owner']='Other'
    elif fault=='vtable':row['entityVtable']='0x1'
    elif fault=='method':row['method']='Other'
    elif fault=='ability':row['ability']=13
    elif fault=='literal':row['heroLiteral']='0x125d1c9'
    elif fault=='helpers':row['helperBodies'].pop()
    elif fault=='shape':row['shape']='unknown'
    elif fault=='actor':row['actorProfile']='derived'
    elif fault=='thisDefinition':row['thisDefinition']='0xec68ab'
    elif fault=='actorDefinition':row['actorDefinition']='0xec68a8'
    elif fault=='frame':row['frameDepth']+=4
    elif fault=='maskOffset':row['maskOffset']+=4
    elif fault=='flagOffset':row['flagOffset']-=1
    elif fault=='depthHash':e['depthsSha256']='0'*64
    elif fault=='originalCallsHash':e['originalCallsSha256']='0'*64
    elif fault=='missingDepth':e['depths'].pop(next(iter(e['depths'])))
    elif fault=='invalidDepth':e['depths'][next(iter(e['depths']))]=1.5
    elif fault=='rehashBadDepth':
        e['depths'][next(iter(e['depths']))]+=4;e['depthsSha256']=sha(json.dumps(e['depths'],sort_keys=True,separators=(',',':')).encode())
    elif fault=='scopeNativeHash':s['nativeSha256']='0'*64
    elif fault=='scopeSourceCalls':s['sourceCalls'][0]['site']='0x1'
    elif fault=='scopePrefixHash':s['luaPrefixSha256']='0'*64
    elif fault=='scopeObjects':s['objectOffsets'][0]+=4
    elif fault=='scopeMask':s['temporaryMask']=7
    elif fault=='scopeLabel':s['labels'][1]=s['labels'][0]
    elif fault=='scopeFlag':s['flagAlias']='unrelated'
    elif fault=='scopeShape':s['luaShape']='unknown'
    elif fault=='scopeOccurrence':s['callOccurrence']+=1
    elif fault=='scopeIndex':s['callListIndex']+=1
    elif fault=='scopeDiagnostic':s['diagnostics']=['hidden']
    elif fault=='inlineBreaks':row['analysisInlineBreaks']+=1
    elif fault=='numericAlias':row['numericMaskAliases'][0]='unrelated'
    with pytest.raises(ValueError):run(inputs)
@pytest.mark.parametrize('index,old,new',[(0,'SpecialAbility(0xe)','SpecialAbility(13)'),(0,'if bVar3 then\n                        goto LAB_00ec6c50','if not bVar3 then\n                        goto LAB_00ec6c50'),(0,'bVar12 = bVar6 | 2','global = bVar6 | 2'),(1,'bVar6 & 0xef','bVar6 & 0xdf'),(2,'bVar14 = true\n                        bVar3 = true','bVar14 = nil\n                        bVar3 = true'),(2,'bVar12 = bVar6 & 0x7f','bVar12 = 1')])
def test_unsafe_prefix_rehash_is_not_enough(inputs,index,old,new):
    change(inputs,index,old,new)
    with pytest.raises(ValueError):run(inputs)
@pytest.mark.parametrize('fault',['observableDeadScratch','externalLabel','nonbooleanLiveAlias','badNumericInitializer','receiverRebinding','closure','parallelMaskWrite'])
def test_unsafe_outside_role_change_refuses_after_phase_rehash(inputs,fault):
    if fault=='badNumericInitializer':inputs['source']=inputs['source'].replace('bVar6 = 0','bVar6 = 1',1)
    elif fault=='receiverRebinding':inputs['source']=inputs['source'].replace('bVar6 = 0','resources = other\nbVar6 = 0',1)
    elif fault=='closure':inputs['source']=inputs['source'].replace('bVar6 = 0','local function hidden() return bVar6 end\nbVar6 = 0',1)
    elif fault=='parallelMaskWrite':inputs['source']=inputs['source'].replace('bVar6 = 0','other,bVar6 = 1,0',1)
    else:
        scope=inputs['row']['scopes'][0];at=inputs['source'].index(scope['oldLuaPrefix'])+len(scope['oldLuaPrefix']);insert={'observableDeadScratch':'quest:Observe(bVar12)','externalLabel':'goto '+scope['labels'][0],'nonbooleanLiveAlias':'bVar14 = 1'}[fault];inputs['source']=inputs['source'][:at]+insert+'\n'+inputs['source'][at:]
    repin(inputs)
    with pytest.raises(ValueError):run(inputs)
def mutate(inputs,site,old,new):
    start=int(inputs['fn']['address'],16);end=int(inputs['fn']['bodyEndExclusive'],16);image=inputs['image'];raw=bytearray(image.bytes_at(start,end-start));old,new=bytes.fromhex(old),bytes.fromhex(new);assert raw[site-start:site-start+len(old)]==old;raw[site-start:site-start+len(old)]=new;parts=[]
    for base,length,offset in image.raw_secs:parts.append((base,bytes(raw) if base==start and length==end-start else image.data[offset:offset+length]))
    inputs['image']=Image(parts);inputs['row']['nativeExtentSha256']=sha(raw)
    for s in inputs['row']['scopes']:s['nativeSha256']=sha(raw[int(s['start'],16)-start:int(s['endExclusive'],16)-start])
@pytest.mark.parametrize('site,old,new',[(0xec6c3c,'0e','0d'),(0xec6c66,'4c','50'),(0xec68b9,'7c','74'),(0xec6c98,'fd','fe'),(0xec7d77,'fffdffff','fffeffff'),(0xec7da9,'79','78')])
def test_mutated_native_semantics_refuse_even_with_rehashed_bytes(inputs,site,old,new):
    mutate(inputs,site,old,new)
    with pytest.raises(ValueError):run(inputs)
def test_equivalent_dead_branch_target_remains_supported(inputs):
    mutate(inputs,0xec7daa,'16','00');assert len(run(inputs)[3])==3

def test_unreviewed_diagnostic_cannot_be_hidden_by_rehash(inputs):
    change(inputs,0,'bVar12 = bVar6 | 2','-- TODO(native): unreviewed operation\n                    bVar12 = bVar6 | 2')
    inputs['todo'].append('unreviewed operation')
    with pytest.raises(ValueError):run(inputs)
