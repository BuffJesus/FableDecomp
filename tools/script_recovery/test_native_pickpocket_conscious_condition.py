import copy,json,importlib.util,itertools
from pathlib import Path
import pytest
from tools.script_recovery.lift_native_lua import RData
W=Path(__file__).resolve().parent;FIXTURE=W if (W/'pre_lift.c').exists()else W/'fixtures/pickpocket_conscious_condition'
if False:
 spec=importlib.util.spec_from_file_location('candidate_condition',W/'native_quest_conscious_condition.py');module=importlib.util.module_from_spec(spec);spec.loader.exec_module(module)
else:
 from tools.script_recovery import native_quest_conscious_condition as module
SOURCE=(FIXTURE/'pre_lift.c').read_text(encoding='utf-8');FN=json.loads((FIXTURE/'function.json').read_text(encoding='utf-8'))
ROW=json.loads((FIXTURE/'reviewed_row.json').read_text(encoding='utf-8'))

def test_original_entry_registration_before_frame():
 result=module.restore(SOURCE,FN,RData(),ROW)
 assert 'ENGINE_RegisterQuestConsciousCondition(pCVar11);'in result
 assert result.index('GSI->GetHero()')<result.index('ENGINE_RegisterQuestConsciousCondition')<result.index('GSI->NewScriptFrame()')
 assert '__atcb7920(this)'not in result

@pytest.mark.parametrize('fault',['actor','temporary','escape','duplicate','copy_depth','copy_stack','register_stack','destroy_stack','hero_slot','frame_depth','no_function','no_bytes','prefix','table','copy','register','clone','predicate','destroy','branch_into'])
def test_ambiguous_or_changed_evidence_is_preserved(fault):
 source=SOURCE;fn=copy.deepcopy(FN);rd=RData()
 calls={c['site']:c for c in fn['calls']};indirect={c['site']:c for c in fn['indirectCalls']}
 if fault=='actor':source=source.replace('CScriptThing(xStack_10,pCVar11,','CScriptThing(xStack_10,pCVar12,')
 elif fault=='temporary':source=source.replace('xStack_10','xStack_8')
 elif fault=='escape':source+='\nuse(xStack_10);'
 elif fault=='duplicate':source+='\n'+next(module.ALIAS_PATTERN.finditer(source))[0]+'\nGSI->NewScriptFrame();'
 elif fault=='copy_depth':calls['0x00EEAF10']['depth']+=4
 elif fault=='copy_stack':calls['0x00EEAF10']['ecxStack']=-8
 elif fault=='register_stack':calls['0x00EEAF1C']['pushedStack']=[-12]
 elif fault=='destroy_stack':calls['0x00EEAF29']['ecxStack']=-8
 elif fault=='hero_slot':indirect['0x00EEAEFC']['slot']='0x11c'
 elif fault=='frame_depth':indirect['0x00EEAF33']['depth']+=4
 elif fault=='no_function':fn=None
 elif fault=='no_bytes':rd=None
 else:
  address={'prefix':0xeeaf02,'table':0x12c2fe8,'copy':0x4abe90,'register':0xcb7920,'clone':0xcddae0,'predicate':0xcddab0,'destroy':0x4aa840,'branch_into':0xeeaf3f}[fault]
  original=rd
  class Changed:
   def bytes_at(self,a,n):
    value=original.bytes_at(a,n)
    if value is not None and a<=address<a+n:
     value=bytearray(value)
     if fault=='branch_into':value[address-a+2:address-a+6]=(0xeeaf07-(address+6)).to_bytes(4,'little',signed=True)
     else:value[address-a]^=1
     return bytes(value)
    return value
  rd=Changed()
 assert module.restore(source,fn,rd,ROW)==source

@pytest.mark.parametrize('fault',['missing','different_address','different_source'])
def test_unreviewed_condition_is_preserved(fault):
 row=copy.deepcopy(ROW)
 if fault=='missing':row=None
 elif fault=='different_address':row['address']='0x00eea460'
 else:row['sourceSha256']='0'*64
 assert module.restore(SOURCE,FN,RData(),row)==SOURCE


@pytest.mark.parametrize('fault',['missing','source_hash','assignment','native_site','native_stack','early_read','changed_vtable'])
def test_condition_movie_alias_requires_exact_reviewed_redefinition(fault):
 row=copy.deepcopy(ROW);source=SOURCE
 if fault=='missing':row.pop('temporaryReuse')
 elif fault=='source_hash':row['temporaryReuse']['inputSha256']='0'*64
 elif fault=='assignment':row['temporaryReuse']['sourceAssignment']='xStack_10 = RESOURCE_StartMovie("different");'
 elif fault=='native_site':row['temporaryReuse']['nativeSite']='0x00EEAF1C'
 elif fault=='native_stack':row['temporaryReuse']['pushedStack']=[-12,-40]
 else:
  source=source.replace('GSI->NewScriptFrame();','use(xStack_10);\nGSI->NewScriptFrame();',1)if fault=='early_read'else source.replace('&PTR_LAB_012c2fe8','&PTR_LAB_012c2fec',1)
  row['temporaryReuse']['inputSha256']=__import__('hashlib').sha256(source.encode()).hexdigest()
 assert module.restore(source,FN,RData(),row)==source


@pytest.mark.parametrize('style',['draft','readable'])
def test_complete_main_condition_assignment_and_payload_oracle(style):
 from lupa.lua54 import LuaRuntime
 harness=(FIXTURE/'harness.lua').read_text(encoding='utf-8')
 def run(name,case):
  lua=LuaRuntime(unpack_returned_tuples=True);lua.execute((FIXTURE/name).read_text(encoding='utf-8'));return lua.execute(harness,lua.table_from(case))
 base=dict(pickable=True,inMovie=False,chance=100,window=0,releaseAt=0,hitAt=0,switchAt=0,mutate=False,payloadZero=False,requireRecovery=True)
 cases=[]
 for condition,when,state,info,switch,pickable,movie,mutate in itertools.product(range(3),[1,3,6],range(3),range(3),[0,2],[False,True],[False,True],[False,True]):
  cases.append(dict(base,conditionState=condition,conditionFrame=when,state=state,infoMode=info,switchAt=switch,pickable=pickable,inMovie=movie,mutate=mutate))
 for condition,info,abort,error in itertools.product(range(3),range(3),range(1,8),['','condition','query','track','movie-start','bar','assign','temporary-retire','face','data1','hero','send','movie-end','deactivate','selected-retire']):
  cases.append(dict(base,conditionState=condition,conditionFrame=3,state=2,infoMode=info,abortFrame=abort,switchAt=2,failEvent=error))
 for case in cases:
  before=run('baseline_'+style+'.lua',dict(case,nativeCondition=True));after=run(style+'.lua',case)
  clean=lambda result:[e for e in result.events.values()if e not in ('hero','condition')]
  assert clean(before)==clean(after),case
  packet=lambda result:[(v.receiver,v.payload)for v in result.packets.values()]
  assert packet(before)==packet(after),case
  assert after.raised==before.raised and after.frames==before.frames and after.randoms==before.randoms and after.assignments==before.assignments and after.queries==before.queries and after.retired==before.retired and after.conditionFailed==before.conditionFailed,case
  assert after.registered==before.registered and not after.movie,case
 assert len(cases)==2241

def test_old_pickpocket_main_sends_after_dead_hero_condition():
 from lupa.lua54 import LuaRuntime
 case=dict(conditionState=1,conditionFrame=1,state=2,infoMode=2,pickable=True,inMovie=False,chance=100,window=0,releaseAt=0,hitAt=0,switchAt=0,mutate=False,payloadZero=False,requireRecovery=True)
 def run(name):
  lua=LuaRuntime(unpack_returned_tuples=True);lua.execute((FIXTURE/name).read_text(encoding='utf-8'));return lua.execute((FIXTURE/'harness.lua').read_text(encoding='utf-8'),lua.table_from(case))
 before=run('baseline_draft.lua');after=run('draft.lua')
 assert not before.registered and after.registered and after.conditionFailed
 assert len(before.packets)==1 and len(after.packets)==0 and not after.movie
