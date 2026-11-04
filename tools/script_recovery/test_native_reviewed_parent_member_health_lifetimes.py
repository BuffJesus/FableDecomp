from pathlib import Path
import hashlib,json,re,sys
import pytest
from lupa.lua54 import LuaRuntime
R=Path(__file__).resolve().parents[2]
from tools.script_recovery import native_parent_member_frame_contracts as FRAME
from tools.script_recovery import native_register_snapshot_provenance as PROVENANCE
from tools.script_recovery import native_parent_member_health_roles as ROLES
from tools.script_recovery import native_reviewed_parent_member_health_lifetimes as MODULE
from tools.script_recovery.test_native_reviewed_interface_table_staging import FixtureImage
from tools.script_recovery.lift_native_lua import RData
F=Path(__file__).resolve().parent/'fixtures/parent_member_health';read=lambda p:json.loads(p.read_text());sha=lambda b:hashlib.sha256(b).hexdigest();serial=MODULE.serial
@pytest.fixture(autouse=True)
def no_retail(monkeypatch):monkeypatch.setattr(RData,'__init__',lambda *a,**k:(_ for _ in ()).throw(AssertionError('retail image opened')))
def inputs():
 p=read(F/'phase.json');row=read(F/'candidate_row.json');return dict(source=(F/'phase.lua').read_text(),todo=p['todo'],calls=p['calls'],fn=p['fn'],row=row,unit=read(F/'candidate_unit.json'),owner=row['owner'],image=FixtureImage([(int(r['address'],16),bytes.fromhex(r['bytes'])) for r in read(F/'recovery_image_parts.json')]),manifest={'GetHealth':row['hostContract']},before=p['before'],after=p['after'])
def run(d):return MODULE.recover(**d)
def repin(d):
 for k,data in [('phaseSha256',d['source'].encode()),('callsSha256',serial(d['calls'])),('todoSha256',serial(d['todo'])),('beforeSha256',serial(d['before'])),('afterSha256',serial(d['after']))]:d['row'][k]=sha(data)
def test_opt_out_identity_without_evidence():assert MODULE.recover('s',['t'],['c'],{},None,{},None,None,None,None,None)==('s',['t'],['c'],[])
def test_three_native_source_copy_scopes_nine_single_and_composed_dead_values():
 d=inputs();source,todo,calls,reviews=run(d);assert len(reviews)==3 and sum(len(r['deadValues']) for r in reviews)==sum(len(r['composedDeadValues']) for r in reviews)==9
 assert todo==d['todo'] and len(calls)==len(d['calls'])-3 and calls.count('RESOURCE_ResourceThingHealthAbove')==3 and source.count('resources:ResourceThingHealthAbove(')==3 and 'quest:GetHealth(' not in source
 assert reviews[0]['sourceCopyCorrespondence']['nativeSites']==reviews[0]['sourceCopyCorrespondence']['emittedUses']==3 and len(reviews[0]['frame']['conditionalNonNullWrappers'])==7 and reviews[0]['frame']['requiredIndirectABI']=={}
 LuaRuntime().compile(source)
@pytest.mark.parametrize('key',['nativeSha256','fnSha256','phaseSha256','callsSha256','todoSha256','beforeSha256','afterSha256'])
def test_stale_top_level_pin_rejects(key):
 d=inputs();d['row'][key]='0'*64
 with pytest.raises(ValueError):run(d)
@pytest.mark.parametrize('fault',['owner','method','resource-kind','minimum','minimum-bool','registered-Main','registered-entity','registered-parent','parent-getter','parent-offset','member-offset','member-name','quest-resource-map','health-slot','getter-slot','owner-register','actor-profile','host-return','host-argument','native-sites','duplicate-native-site','native-positive-exit'])
def test_forged_registration_scope_or_native_profile_rejects(fault):
 d=inputs();r=d['row'];entity=d['unit']['entities'][d['owner']]
 if fault=='owner':r['owner']='wrong'
 elif fault=='method':r['method']='wrong'
 elif fault=='resource-kind':r['resourceKind']='entity-member'
 elif fault=='minimum':r['minimumHealth']=1
 elif fault=='minimum-bool':r['minimumHealth']=False
 elif fault=='registered-Main':entity['functions']['Main']['address']='0x123'
 elif fault=='registered-entity':entity['vtable']='0x12f18c4'
 elif fault=='registered-parent':d['unit']['vtable']='0x12f17e0'
 elif fault=='parent-getter':entity['functions']['GetParentScript']['address']='0xec5c64'
 elif fault=='parent-offset':r['parentOffset']=24
 elif fault=='member-offset':r['memberOffset']=88
 elif fault=='member-name':r['memberName']='seh_Witch'
 elif fault=='quest-resource-map':d['unit']['quest']['resourceFields']={}
 elif fault=='health-slot':r['healthSlot']=0x424
 elif fault=='getter-slot':r['getterSlot']=0x34
 elif fault=='owner-register':r['ownerRegister']='ebp'
 elif fault=='actor-profile':r['actorProfile']='entry-edi'
 elif fault=='host-return':r['hostContract']['returnType']='int'
 elif fault=='host-argument':r['hostContract']['parameters'][0]['optional']=True
 elif fault=='native-sites':r['nativeSites']=4
 elif fault=='duplicate-native-site':r['nativePredicates'][1]=r['nativePredicates'][0]
 else:r['nativePredicates'][0]['positiveExit']='0xecdf2a'
 with pytest.raises((ValueError,AssertionError)):run(d)
@pytest.mark.parametrize('alias',['pCVar13','fret_0','fVar4'])
def test_repinned_observable_read_reaches_liveness_rejection(alias):
 d=inputs();marker=re.search(r'(?m)^(?P<i> *)if fVar4 < fret_0 then\n',d['source']);assert marker is not None;at=marker.end();d['source']=d['source'][:at]+marker['i']+'    quest:Observe('+alias+')\n'+d['source'][at:];repin(d)
 with pytest.raises(ValueError,match='live staging value'):run(d)
@pytest.mark.parametrize('fault',['actor','member','condition','calls','before','after'])
def test_repinned_source_or_accounting_forgery_rejects(fault):
 d=inputs()
 if fault=='actor':d['source']=d['source'].replace('quest:GetHealth(pCVar13)','quest:GetHealth(wrong)',1)
 elif fault=='member':d['source']=d['source'].replace('resources:ScriptThing(resources:MemberResource("seh_Mother"))','resources:ScriptThing(resources:MemberResource("seh_Witch"))',1)
 elif fault=='condition':d['source']=d['source'].replace('if fVar4 < fret_0 then','if fret_0 <= fVar4 then',1)
 elif fault=='calls':d['calls'].remove('GetHealth')
 elif fault=='before':d['before']=[s for s in d['before'] if 'fret_0 = GSI->GetHealth' not in s]
 else:d['after'].append('fret_0 = GSI->GetHealth(pCVar13);')
 repin(d)
 with pytest.raises(ValueError):run(d)
