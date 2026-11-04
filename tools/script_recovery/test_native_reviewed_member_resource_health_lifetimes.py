from pathlib import Path
import copy,hashlib,importlib.util,json,re,sys
import pytest
from lupa.lua54 import LuaRuntime
R=Path(__file__).resolve().parents[2]
from tools.script_recovery import native_reviewed_member_resource_health_lifetimes as MODULE
from tools.script_recovery import native_member_frame_contracts as FRAME
from tools.script_recovery import native_register_snapshot_provenance as PROVENANCE
from tools.script_recovery.test_native_reviewed_interface_table_staging import FixtureImage
F=Path(__file__).resolve().parent/'fixtures/member_resource_health';read=lambda p:json.loads(p.read_text());serial=MODULE.serial;sha=lambda b:hashlib.sha256(b).hexdigest()
@pytest.fixture(autouse=True)
def no_retail(monkeypatch):
 from tools.script_recovery.lift_native_lua import RData
 monkeypatch.setattr(RData,'__init__',lambda *a,**k:(_ for _ in ()).throw(AssertionError('retail image opened')))
def inputs():
 phase=read(F/'phase.json');prior=read(F/'prior_input.json');row=read(F/'candidate_row.json')
 return dict(source=(F/'phase.lua').read_text(),todo=phase['todo'],calls=phase['calls'],fn=phase['fn'],row=row,unit=read(F/'candidate_unit.json'),owner=row['owner'],image=FixtureImage([(int(p['address'],16),bytes.fromhex(p['bytes'])) for p in read(F/'image_parts.json')]),manifest={'GetHealth':row['hostContract']},before=prior['before'],after=prior['after'],prior_typed_input={k:prior[k] for k in ['source','todo','calls']})
def run(d):return MODULE.recover(**d)
def repin(d):
 for key,data in [('fnSha256',serial(d['fn'])),('phaseSha256',d['source'].encode()),('callsSha256',serial(d['calls'])),('todoSha256',serial(d['todo'])),('beforeSha256',serial(d['before'])),('afterSha256',serial(d['after']))]:d['row'][key]=sha(data)

def test_opt_out_is_identity_without_evidence():
 assert MODULE.recover('s',['todo'],['call'],{},None,{},None,None,None,None,None)==('s',['todo'],['call'],[])

def test_seven_native_queries_prior_replay_and_composed_liveness_accounted():
 d=inputs();out,todo,calls,reviews=run(d)
 assert len(reviews)==7 and sum(len(r['deadValues']) for r in reviews)==21 and sum(len(r['composedDeadValues']) for r in reviews)==21
 assert todo==d['todo'] and calls.count('RESOURCE_ResourceThingHealthAbove')==7 and len(calls)==len(d['calls'])-7
 assert 'quest:GetHealth(' not in out and out.count('resources:ResourceThingHealthAbove(')==7
 assert [r['retainedFlag'] for r in reviews]==['cVar4']*4+['hb_stk_a8']*3
 assert len(reviews[0]['frame']['correctedExportDepths'])==65 and reviews[0]['priorTypedComparisonReplay']['originalHealthQueries']==7
 assert len(reviews[0]['frame']['requiredIndirectABI'])==6 and len(reviews[0]['frame']['conditionalNonNullWrappers'])==3
 LuaRuntime().compile(out)

@pytest.mark.parametrize('key',['nativeSha256','fnSha256','phaseSha256','callsSha256','todoSha256','beforeSha256','afterSha256','priorTypedInputSha256','priorTypedReviewsSha256'])
def test_stale_top_level_certificate_rejects(key):
 d=inputs();d['row'][key]='0'*64
 with pytest.raises(ValueError):run(d)

@pytest.mark.parametrize('fault',['owner','method','resource-kind','minimum','minimum-bool','member-offset','member-name','member-vtable','health-slot','getter-slot','registered-Main','registered-vtable','unit-resource-map','host-return','host-argument','site-count','prior-missing','prior-review-missing','prior-source-repinned'])
def test_forged_scope_or_host_contract_rejects(fault):
 d=inputs()
 if fault=='owner':d['row']['owner']='wrong'
 elif fault=='method':d['row']['method']='wrong'
 elif fault=='resource-kind':d['row']['resourceKind']='stack'
 elif fault=='minimum':d['row']['minimumHealth']=1
 elif fault=='minimum-bool':d['row']['minimumHealth']=False
 elif fault=='member-offset':d['row']['memberOffset']=40
 elif fault=='member-name':d['row']['memberName']='wrong'
 elif fault=='member-vtable':d['row']['memberVtable']+=4
 elif fault=='health-slot':d['row']['healthSlot']+=4
 elif fault=='getter-slot':d['row']['getterSlot']+=4
 elif fault=='registered-Main':d['unit']['entities'][d['owner']]['functions']['Main']['address']='0x123'
 elif fault=='registered-vtable':d['unit']['entities'][d['owner']]['vtable']='0x123'
 elif fault=='unit-resource-map':d['unit']['entities'][d['owner']]['resourceFields']={}
 elif fault=='host-return':d['manifest']=copy.deepcopy(d['manifest']);d['manifest']['GetHealth']['returnType']='int'
 elif fault=='host-argument':d['manifest']=copy.deepcopy(d['manifest']);d['manifest']['GetHealth']['parameters'][0]['optional']=True
 elif fault=='site-count':d['row']['nativeSites']=6
 elif fault=='prior-missing':d['prior_typed_input']=None
 elif fault=='prior-review-missing':d['unit']['reviewedTypedBooleanHealth']={}
 else:d['prior_typed_input']['source']+='\n';d['row']['priorTypedInputSha256']=sha(d['prior_typed_input']['source'].encode())
 with pytest.raises(ValueError):run(d)

@pytest.mark.parametrize('fault',['missing-call','duplicate-call','call-bytes','static-target','cached-equivalence','bounded-hash','missing-return-region','global-address','conditional-purge','conditional-child-ABI','conditional-claim','indirect-inventory','indirect-purge','member-profile-offset','factory-link','constructor-layout','base-header'])
def test_corrupt_native_frame_or_constructor_evidence_rejects(fault):
 d=inputs();p=d['row']['frameProfile']
 if fault=='missing-call':p['callContracts'].pop()
 elif fault=='duplicate-call':p['callContracts'].append(copy.deepcopy(p['callContracts'][0]))
 elif fault=='call-bytes':p['callContracts'][0]['instructionBytes']='90'
 elif fault=='static-target':next(r for r in p['callContracts'] if r['family']=='GSI_profile')['target']='0x123'
 elif fault=='cached-equivalence':r=next(r for r in p['callContracts'] if r['family']=='GSI_profile');r['receiverValueEquivalenceProved']=not r['receiverValueEquivalenceProved']
 elif fault=='bounded-hash':next(iter(p['boundedReturnRegions'].values()))[0]['sha256']='0'*64
 elif fault=='missing-return-region':p['boundedReturnRegions'].pop(next(iter(p['boundedReturnRegions'])))
 elif fault=='global-address':p['globalInterfaceAddresses']=[0x143e8fc]
 elif fault=='conditional-purge':p['conditionalNonNullWrappers']['0x7e7390']['purgeBytes']=20
 elif fault=='conditional-child-ABI':p['conditionalNonNullWrappers']['0x7e7390']['nonNullChildABIRequired']['stackPurge']=20
 elif fault=='conditional-claim':p['conditionalNonNullWrappers']['0x7e7390']['closedAllPathReturnProof']=True
 elif fault=='indirect-inventory':p['requiredIndirectABI'].pop(next(iter(p['requiredIndirectABI'])))
 elif fault=='indirect-purge':next(iter(p['requiredIndirectABI'].values()))['requiredNormalPurge']+=4
 elif fault=='member-profile-offset':p['memberProfile']['offset']=40
 elif fault=='factory-link':p['memberProfile']['witnesses']['factory']['address']='0xecd301'
 elif fault=='constructor-layout':p['memberProfile']['witnesses']['constructor']['bytes']='90'
 else:p['memberProfile']['witnesses']['base_constructor']['bytes']='90'
 with pytest.raises((ValueError,AssertionError)):run(d)

@pytest.mark.parametrize('kind',['Thing-output','Thing-actor','float'])
def test_repinned_new_observable_read_reaches_and_fails_liveness(kind):
 d=inputs();scope=read(F/'liveness_scopes.json')[0];alias={'Thing-output':scope['output'],'Thing-actor':scope['actor'],'float':scope['result']}[kind]
 marker=scope['indent']+'quest:Observe('+alias+')\n';pos=scope['end']
 # The first direct scope precedes all three earlier comparison repairs, so
 # its offset is identical in the actual prior input and current output.
 d['source']=d['source'][:pos]+marker+d['source'][pos:]
 prior=d['prior_typed_input'];prior['source']=prior['source'][:pos]+marker+prior['source'][pos:]
 row=d['unit']['reviewedTypedBooleanHealth'][d['fn']['address'].lower()];row['phaseSha256']=sha(prior['source'].encode())
 d['row']['priorTypedInputSha256']=sha(prior['source'].encode());repin(d)
 with pytest.raises(ValueError,match='live staging value'):run(d)

@pytest.mark.parametrize('fault',['source-result','source-actor','source-flag','resource-binding','flag-comparison','calls','AST-copies'])
def test_repinned_source_or_accounting_change_rejects(fault):
 d=inputs()
 if fault=='source-result':d['source']=d['source'].replace('fVar14 = quest:GetHealth(pCVar9)','wrong = quest:GetHealth(pCVar9)',1)
 elif fault=='source-actor':d['source']=d['source'].replace('fVar14 = quest:GetHealth(pCVar9)','fVar14 = quest:GetHealth(wrong)',1)
 elif fault=='source-flag':d['source']=d['source'].replace('cVar4 = 0.0 < fVar14','wrong = 0.0 < fVar14',1)
 elif fault=='resource-binding':d['source']=d['source'].replace('MemberResource("seh_me", me)','MemberResource("wrong", me)',1)
 elif fault=='flag-comparison':d['source']=d['source'].replace('if not (fVar14 > 0.0) then','if fVar14 <= 0.0 then',1)
 elif fault=='calls':d['calls'].remove('GetHealth')
 else:d['after'].append('fVar14 = (float10)GSI->GetHealth(uVar7);')
 repin(d)
 with pytest.raises(ValueError):run(d)

def test_frame_derives_without_original_export_depth_anchors():
 d=inputs();fn=copy.deepcopy(d['fn'])
 for key in ['calls','indirectCalls']:
  for c in fn[key]:c['depth']+=4
 row=d['row'];raw=d['image'].bytes_at(int(fn['address'],16),int(fn['bodyEndExclusive'],16)-int(fn['address'],16))
 frame,report=FRAME.prove(raw,fn,d['image'],row['frameProfile'],entity_vtable=int(row['entityVtable'],16),scope_start=int(row['scopeStart'],16),scope_end=int(fn['bodyEndExclusive'],16),frame_depth=228,this_definition=int(row['thisDefinition'],16),actor_definition=int(row['actorDefinition'],16),actor_profile=row['actorProfile'],owner_register=row['ownerRegister'])
 assert report['reachableInstructions']==1445 and report['callCount']==226 and frame['depths'][frame['by'][int(row['scopeStart'],16)]]==228

def test_default_snapshot_provenance_is_exact_and_globals_require_allowlist():
 d=inputs();a,b=int(d['fn']['address'],16),int(d['fn']['bodyEndExclusive'],16);raw=d['image'].bytes_at(a,b-a)
 golden=read(F/'provenance_default.json');assert sha(raw)==golden['nativeSha256']
 new=PROVENANCE.analyze(raw,a,member_offsets=(36,));wrong=PROVENANCE.analyze(raw,a,member_offsets=(36,),global_interface_addresses=(0x143e8fc,));explicit=PROVENANCE.analyze(raw,a,member_offsets=(36,),global_interface_addresses=(0x143e8f8,))
 canonical=lambda states:json.dumps({hex(k):v for k,v in states.items()},sort_keys=True,separators=(',',':')).encode()
 assert sha(canonical(new['states']))==golden['defaultStatesSha256'] and new['states']==wrong['states'] and new['visits']==wrong['visits']==golden['visits']
 assert explicit['states'][0xecf937]['ecx']==('interface_snapshot',0xecf92f) and 'ecx' not in new['states'][0xecf937]
