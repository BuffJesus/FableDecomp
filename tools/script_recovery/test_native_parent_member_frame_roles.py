from pathlib import Path
import copy,hashlib,json,sys
import pytest
R=Path(__file__).resolve().parents[2]
from tools.script_recovery import native_parent_member_frame_contracts as FRAME
from tools.script_recovery import native_register_snapshot_provenance as PROVENANCE
from tools.script_recovery import native_parent_member_health_roles as ROLES
from tools.script_recovery import native_reviewed_parent_member_health_lifetimes as MODULE
from tools.script_recovery.test_native_reviewed_interface_table_staging import FixtureImage
from tools.script_recovery.lift_native_lua import RData
F=Path(__file__).resolve().parent/'fixtures/parent_member_health';read=lambda p:json.loads(p.read_text())
@pytest.fixture(autouse=True)
def no_retail(monkeypatch):monkeypatch.setattr(RData,'__init__',lambda *a,**k:(_ for _ in ()).throw(AssertionError('retail image opened')))
def inputs():
 fn=read(F/'phase.json')['fn'];image=FixtureImage([(int(p['address'],16),bytes.fromhex(p['bytes'])) for p in read(F/'image_parts.json')]);a,b=int(fn['address'],16),int(fn['bodyEndExclusive'],16)
 return dict(raw=image.bytes_at(a,b-a),fn=fn,image=image,row=read(F/'frame_profile.json'),entity_vtable=0x12f18c0,scope_start=0xecd9ef,scope_end=b,frame_depth=180,this_definition=0xecd9e9,actor_definition=0xecd9ec,actor_profile='entry-ebp',owner_register='esi')
def run(d):return FRAME.prove(**d)
def role(frame,image,scope,**changes):
 args=dict(this_definition=0xecd9e9,parent_offset=20,member_offset=72,member_vtable=0x127094c);args.update(changes)
 return ROLES.prove_roles(frame,int(scope['query'],16),scope['predicate'],image,**args)
def test_complete_static_frame_and_three_parent_member_roles():
 d=inputs();frame,report=run(d);assert report['reachableInstructions']==827 and report['callCount']==report['knownStaticProfileCalls']==157 and len(report['correctedExportDepths'])==27
 assert report['requiredIndirectABI']=={} and len(report['conditionalNonNullWrappers'])==7
 nested=[r for r in report['callReviews'] if r['profile']=='parent_member_profile'];assert nested and any(not r['receiverValueEquivalenceProved'] for r in nested)
 for scope in read(F/'predicates.json')['rows']:
  actual=role(frame,d['image'],scope);assert actual['resourceKind']=='parent-member' and actual['purges']==dict(getter=4,query=4,cleanup=0)
def test_frame_ignores_all_exported_depths():
 d=inputs()
 for key in ['calls','indirectCalls']:
  for call in d['fn'][key]:call['depth']+=4
 frame,report=run(d);assert report['reachableInstructions']==827 and report['callCount']==157
@pytest.mark.parametrize('fault',['native-pin','global-profile','parent-offset','resource-offset','thing-offset','resource-table','thing-table','parent-vtable','entity-vtable','parent-getter','entity-factory','parent-constructor','Thing-copy','missing-witness','missing-call','duplicate-call','static-target','callee-prefix','call-bytes','receiver-claim','slot','purge','return-region','wrapper-inventory','wrapper-bytes','wrapper-purge','wrapper-child','wrapper-closed','indirect-inventory'])
def test_forged_frame_or_constructor_evidence_rejects(fault):
 d=inputs();r=d['row'];p=r['parentProfile'];call=r['callContracts'][0]
 if fault=='native-pin':r['cacheProof']['nativeSha256']='0'*64
 elif fault=='global-profile':r['globalInterfaceAddresses']=[0x143e8fc]
 elif fault=='parent-offset':p['parentPointerOffset']=24
 elif fault=='resource-offset':p['resourceOffsets']=[76,88]
 elif fault=='thing-offset':p['entityThingOffset']=12
 elif fault=='resource-table':p['resourceVtable']='0x1270950'
 elif fault=='thing-table':p['entityThingVtable']='0x1238c90'
 elif fault=='parent-vtable':p['parentVtable']='0x12f17e0'
 elif fault=='entity-vtable':p['entityVtable']='0x12f18c4'
 elif fault in ['parent-getter','entity-factory','parent-constructor','Thing-copy']:
  name={'parent-getter':'parent_getter','entity-factory':'entity_factory','parent-constructor':'parent_constructor','Thing-copy':'thing_copy'}[fault];p['witnesses'][name]['bytes']='90'
 elif fault=='missing-witness':p['witnesses'].pop('parent_getter')
 elif fault=='missing-call':r['callContracts'].pop()
 elif fault=='duplicate-call':r['callContracts'].append(copy.deepcopy(call))
 elif fault=='static-target':call['target']='0x4abe94'
 elif fault=='callee-prefix':call['calleePrefix']='90'*16
 elif fault=='call-bytes':call['instructionBytes']='90'
 elif fault=='receiver-claim':
  c=next(c for c in r['callContracts'] if c['family']=='parent_member_profile' and not c['receiverValueEquivalenceProved']);c['receiverValueEquivalenceProved']=True
 elif fault=='slot':next(c for c in r['callContracts'] if c['family']=='parent_member_profile')['slot']='0x34'
 elif fault=='purge':call['purgeBytes']+=4
 elif fault=='return-region':next(iter(r['boundedReturnRegions'].values()))[0]['sha256']='0'*64
 elif fault=='wrapper-inventory':r['conditionalNonNullWrappers'].pop('0x4aab30')
 elif fault.startswith('wrapper-'):
  w=r['conditionalNonNullWrappers']['0x4aab30']
  if fault=='wrapper-bytes':w['bytes']='90'*17
  elif fault=='wrapper-purge':w['purgeBytes']+=4
  elif fault=='wrapper-child':w['nonNullChildABIRequired']['ordinaryThiscall']=False
  else:w['closedAllPathReturnProof']=True
 else:r['requiredIndirectABI']={'0xecdef5':dict(instructionBytes='ff5230',targetProvenanceVerified=False,requiredNormalPurge=4)}
 with pytest.raises((ValueError,AssertionError)):run(d)
@pytest.mark.parametrize('fault',['parent-offset','resource-offset','resource-table','getter-target','health-slot','getter-slot','owner-origin','owner-register','threshold','positive-exit'])
def test_forged_query_roles_reject(fault):
 d=inputs();frame,_=run(d);scope=read(F/'predicates.json')['rows'][0];kw={}
 if fault=='parent-offset':kw['parent_offset']=24
 elif fault=='resource-offset':kw['member_offset']=88
 elif fault=='resource-table':kw['member_vtable']=0x1270950
 elif fault=='getter-target':kw['getter_target']=0x7e7494
 elif fault=='health-slot':kw['health_slot']=0x424
 elif fault=='getter-slot':kw['getter_slot']=0x34
 elif fault=='owner-origin':kw['this_definition']=0xecd9ec
 elif fault=='owner-register':kw['owner_register']='ebp'
 elif fault=='threshold':scope['predicate']['threshold']=1
 else:scope['predicate']['positiveExit']='0xecdf2a'
 with pytest.raises((ValueError,AssertionError)):role(frame,d['image'],scope,**kw)
def test_default_provenance_matches_accepted_golden_and_nested_is_opt_in():
 d=inputs();a=int(d['fn']['address'],16);golden=read(F/'default_provenance.json');assert hashlib.sha256(d['raw']).hexdigest()==golden['nativeSha256'];default=PROVENANCE.analyze(d['raw'],a,member_offsets=(8,));canonical=json.dumps({hex(k):v for k,v in default['states'].items()},sort_keys=True,separators=(',',':')).encode();assert hashlib.sha256(canonical).hexdigest()==golden['statesSha256'] and default['visits']==golden['visits']
 explicit=PROVENANCE.analyze(d['raw'],a,member_offsets=(8,),member_table_loads=(8,),parent_resource_profiles=((20,(72,88)),));assert explicit['states'][0xecdef5]['edx'][0]=='parent_member_table_snapshot' and 'edx' not in default['states'][0xecdef5]
@pytest.mark.parametrize('options',[dict(parent_resource_profiles=((20,(73,)),)),dict(parent_resource_profiles=((20,()),)),dict(parent_resource_profiles=((20,(72,)),(20,(88,)))),dict(member_table_loads=(12,))])
def test_invalid_optional_provenance_profiles_reject(options):
 d=inputs()
 with pytest.raises(ValueError):PROVENANCE.analyze(d['raw'],int(d['fn']['address'],16),member_offsets=(8,),**options)
