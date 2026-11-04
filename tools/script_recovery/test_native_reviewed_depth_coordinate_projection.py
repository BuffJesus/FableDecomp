from pathlib import Path
import copy,json,sys
import pytest
W=Path(__file__).resolve().parent;R=next(p for p in W.parents if (p/'tools/script_recovery/script_units.py').is_file())
from tools.script_recovery import native_reviewed_register_resource_health_lifetimes as M
from tools.script_recovery import native_prior_hit_health as PRIOR
from tools.script_recovery.test_native_reviewed_trader2_resource_health_lifetimes import inputs,copyproof,repin,digest,serial,FixtureImage
from tools.script_recovery.lift_native_lua import RData
@pytest.fixture(autouse=True)
def no_retail(monkeypatch):monkeypatch.setattr(RData,'__init__',lambda *a,**k:(_ for _ in ()).throw(AssertionError('retail image opened')))
def prior(data):
 return PRIOR.replay(data['fn'],data['source'],input=data['prior_hit_health_input'],todo=data['todo'],calls=data['calls'],unit=data['unit'],owner=data['owner'],image=data['image'])
def row(data):return data['unit']['reviewedHitHealthLifetimes']['0x00eccad0']
def test_projected_coordinates_and_original_evidence_are_distinct():
 d=inputs();original=copy.deepcopy(d['fn']);view,proof=PRIOR.reviewed_frame_view(d['fn'],prior(d));assert d['fn']==original and view!=original and len(proof['changedCalls'])==34
 calls={int(c['site'],16):c for c in view['calls']+view['indirectCalls']};old={int(c['site'],16):c for c in d['fn']['calls']+d['fn']['indirectCalls']}
 assert old[0xecd12d]['depth']==186 and old[0xecd12d]['pushedStack']==[-26] and old[0xecd12d]['ecxStack']==-158
 assert calls[0xecd12d]['depth']==196 and calls[0xecd12d]['pushedStack']==[-36] and calls[0xecd12d]['ecxStack']==-168 and calls[0xecd12d]['edxStack']==-36
 assert calls[0xecd156]['depth']==192 and calls[0xecd156]['ecxStack']==-36
 out,todo,updated,reviews=M.recover(**d);assert reviews[0]['reviewedNativeFrameProjection']==proof
 assert [r['native']['roles']['objectEntryOffset'] for r in reviews]==[-24,-12,-36]
 assert [r['native']['roles']['resourceEntryOffset'] for r in reviews]==[-168]*3
 assert todo==d['todo'] and len(updated)==len(d['calls'])-3

def test_no_reviewed_depth_map_has_no_projection():
 F=R/'tools/script_recovery/fixtures/trader_resource_health';read=lambda name:json.loads((F/name).read_text());p=read('phase.json');unit=read('unit.json');image=FixtureImage([(int(v['address'],16),bytes.fromhex(v['bytes'])) for v in read('image_parts.json')])
 replay=PRIOR.replay(p['fn'],(F/'phase.lua').read_text(),input=read('prior_input.json'),todo=p['todo'],calls=p['calls'],unit=unit,owner='TalkingTrader1',image=image)
 view,proof=PRIOR.reviewed_frame_view(p['fn'],replay);assert view is p['fn'] and proof is None
@pytest.mark.parametrize('fault',['originalHash','depthHash','missingSite','extraSite','nonInteger','unaligned','overflow','originalDepth','coordinateSchema','pushSchema'])
def test_frame_projection_schema_and_pins_reject(fault):
 d=inputs();replay=copy.deepcopy(prior(d));e=replay['proof'][0]['reviewedEntryCallDepthEvidence'];depths=e['depths'];key=next(iter(depths))
 if fault=='originalHash':e['originalCallsSha256']='0'*64
 elif fault=='depthHash':e['depthsSha256']='0'*64
 elif fault=='missingSite':depths.pop(key)
 elif fault=='extraSite':depths['0x00abcdef']=192
 elif fault in ['nonInteger','unaligned','overflow']:depths[key]={'nonInteger':True,'unaligned':3,'overflow':8192}[fault]
 else:
  c=d['fn']['calls'][0]
  if fault=='originalDepth':c['depth']=False
  elif fault=='coordinateSchema':c['ecxStack']='unproved'
  else:c['pushedStack']=[True]
  e['originalCallsSha256']=digest(serial([d['fn']['calls'],d['fn']['indirectCalls']]))
 if fault not in ['originalHash','depthHash']:e['depthsSha256']=digest(json.dumps(depths,sort_keys=True,separators=(',',':')).encode())
 with pytest.raises(ValueError):PRIOR.reviewed_frame_view(d['fn'],replay)
@pytest.mark.parametrize('site,field,value,why',[(0xecd12d,'pushedStack',[0],'exported object coordinates'),(0xecd12d,'ecxStack',0,'exported resource coordinates'),(0xecd156,'ecxStack',0,'exported object coordinates')])
def test_projection_does_not_replace_native_operand_checks(site,field,value,why):
 d=inputs();c=next(c for c in d['fn']['calls'] if int(c['site'],16)==site);c[field]=value
 row(d)['entryCallDepthEvidence']['originalCallsSha256']=digest(serial([d['fn']['calls'],d['fn']['indirectCalls']]))
 repin(d)
 with pytest.raises(ValueError,match=why):M.recover(**d)

def test_self_expression_is_limited_to_previously_proved_site():
 d=inputs();proof=copyproof(d);mapping=proof['priorHealth']['checkedSelfExpressions'];assert len(mapping)==1
 assert mapping[0]['astActor']=='(CScriptThing *)(this + 8)' and mapping[0]['luaActor']=='me' and mapping[0]['nativeSourceActor']=='pCVar7'
 assert proof['nativeSites']==3 and proof['priorHealth']['originalNativeSites']==4
@pytest.mark.parametrize('expression',['(CScriptThing *)(this + 12)','(CScriptThing *)(other + 8)','(OtherThing *)(this + 8)'])
def test_unknown_self_expressions_refuse(expression):
 d=inputs()
 for key in ['before','after']:d[key]=[line.replace('(CScriptThing *)(this + 8)',expression) for line in d[key]]
 with pytest.raises(ValueError,match='unsupported AST health expression'):copyproof(d)

def test_unconsumed_resource_actor_cannot_claim_self_expression():
 d=inputs()
 for key in ['before','after']:d[key]=[line.replace('fret_00 = GSI->GetHealth(pCVar7);','fret_00 = GSI->GetHealth((CScriptThing *)(this + 8));') for line in d[key]]
 with pytest.raises(ValueError,match='unsupported AST health expression'):copyproof(d)
