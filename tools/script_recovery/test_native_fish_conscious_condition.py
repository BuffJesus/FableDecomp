import copy,json,importlib.util,itertools
from pathlib import Path
import pytest
from tools.script_recovery.lift_native_lua import RData
W=Path(__file__).resolve().parent;FIXTURE=W if (W/'pre_lift.c').exists()else W/'fixtures/fish_conscious_condition'
if FIXTURE==W:
 spec=importlib.util.spec_from_file_location('candidate_condition',W/'native_quest_conscious_condition.py');module=importlib.util.module_from_spec(spec);spec.loader.exec_module(module)
else:
 from tools.script_recovery import native_quest_conscious_condition as module
SOURCE=(FIXTURE/'pre_lift.c').read_text(encoding='utf-8');FN=json.loads((FIXTURE/'function.json').read_text(encoding='utf-8'))
ROW=json.loads((FIXTURE/'reviewed_row.json').read_text(encoding='utf-8'))

def test_original_entry_registration_before_frame():
 result=module.restore(SOURCE,FN,RData(),ROW)
 assert 'ENGINE_RegisterQuestConsciousCondition(pCVar4);'in result
 assert result.index('GSI->GetHero()')<result.index('ENGINE_RegisterQuestConsciousCondition')<result.index('GSI->NewScriptFrame()')
 assert '__atcb7920(this)'not in result

@pytest.mark.parametrize('fault',['actor','temporary','escape','duplicate','copy_depth','copy_stack','register_stack','destroy_stack','hero_slot','frame_depth','no_function','no_bytes','prefix','table','copy','register','clone','predicate','destroy','branch_into'])
def test_ambiguous_or_changed_evidence_is_preserved(fault):
 source=SOURCE;fn=copy.deepcopy(FN);rd=RData()
 calls={c['site']:c for c in fn['calls']};indirect={c['site']:c for c in fn['indirectCalls']}
 if fault=='actor':source=source.replace('CScriptThing((CScriptThing *)&xStack_2c,pCVar4,','CScriptThing((CScriptThing *)&xStack_2c,pCVar5,')
 elif fault=='temporary':source=source.replace('xStack_2c','xStack_8')
 elif fault=='escape':source+='\nuse(xStack_2c);'
 elif fault=='duplicate':source+='\n'+next(module.CAST_PATTERN.finditer(source))[0]+'\nGSI->NewScriptFrame();'
 elif fault=='copy_depth':calls['0x00EE95E2']['depth']+=4
 elif fault=='copy_stack':calls['0x00EE95E2']['ecxStack']=-8
 elif fault=='register_stack':calls['0x00EE95EE']['pushedStack']=[-12]
 elif fault=='destroy_stack':calls['0x00EE95FB']['ecxStack']=-8
 elif fault=='hero_slot':indirect['0x00EE95CE']['slot']='0x11c'
 elif fault=='frame_depth':indirect['0x00EE9605']['depth']+=4
 elif fault=='no_function':fn=None
 elif fault=='no_bytes':rd=None
 else:
  address={'prefix':0xee95d4,'table':0x12c2fe8,'copy':0x4abe90,'register':0xcb7920,'clone':0xcddae0,'predicate':0xcddab0,'destroy':0x4aa840,'branch_into':0xee9611}[fault]
  original=rd
  class Changed:
   def bytes_at(self,a,n):
    value=original.bytes_at(a,n)
    if value is not None and a<=address<a+n:
     value=bytearray(value)
     if fault=='branch_into':value[address-a+2:address-a+6]=(0xee95d9-(address+6)).to_bytes(4,'little',signed=True)
     else:value[address-a]^=1
     return bytes(value)
    return value
  rd=Changed()
 assert module.restore(source,fn,rd,ROW)==source

@pytest.mark.parametrize('fault',['missing','different_address','different_source'])
def test_unreviewed_condition_is_preserved(fault):
 row=copy.deepcopy(ROW)
 if fault=='missing':row=None
 elif fault=='different_address':row['address']='0x00eea7c0'
 else:row['sourceSha256']='0'*64
 assert module.restore(SOURCE,FN,RData(),row)==SOURCE


@pytest.mark.parametrize('style',['draft','readable'])
def test_complete_main_condition_oracle_and_fishing_values(style):
 from lupa.lua54 import LuaRuntime
 harness=(FIXTURE/'harness.lua').read_text(encoding='utf-8')
 def run(name,case):
  lua=LuaRuntime(unpack_returned_tuples=True);lua.execute((FIXTURE/name).read_text(encoding='utf-8'));return lua.execute(harness,lua.table_from(case))
 cases=[]
 for condition,when,(n,selected),live,success,rod,(x,y,z)in itertools.product(range(3),[1,3,6],[(0,0),(1,0),(1,1),(3,2),(3,3)],[False,True],[False,True],range(3),[(2,3,7),(12,3,7),(float('nan'),3,7),(float('inf'),3,7)]):
  cases.append(dict(conditionState=condition,conditionFrame=when,count=n,selected=selected,live=live,success=success,rod=rod,x=x,y=y,z=z,water=7,near=selected>0 and live and ((y-3)**2+(z-7)**2)+(x-2)**2<100))
 for condition,when,control,acquire,msgwait,selected,live,success,nullInfo in itertools.product(range(3),[1,3,6],[0,3],[0,5],[0,7],[0,2],[False,True],[False,True],[False,True]):
  cases.append(dict(conditionState=condition,conditionFrame=when,count=3,selected=selected,live=live,success=success,rod=0,x=2,y=3,z=7,water=7,near=not nullInfo and selected>0 and live,nullInfo=nullInfo,controlWait=control,acquireWait=acquire,msgWait=msgwait))
 for condition,error in itertools.product(range(3),['hero','condition','frame','water','face','fishing','container','weight','give','remove']):
  cases.append(dict(conditionState=condition,conditionFrame=6,count=3,selected=2,live=True,success=True,rod=0,x=2,y=3,z=7,water=7,near=True,failEvent=error))
 for case in cases:
  before=run('baseline_'+style+'.lua',dict(case,nativeCondition=True));after=run(style+'.lua',case)
  clean=lambda result:[e for e in result.events.values()if e!='hero']
  assert clean(before)==clean(after),case
  assert after.ok==before.ok and after.frames==before.frames and after.checks==before.checks and after.conditionFailed==before.conditionFailed and after.registered==before.registered,case
  assert not after.acquired and not after.movie and after.ownerCount==0,case
 assert len(cases)==3342

def test_old_fish_main_fishes_after_dead_hero_condition():
 from lupa.lua54 import LuaRuntime
 case=dict(conditionState=1,conditionFrame=1,count=3,selected=2,live=True,success=True,rod=0,x=2,y=3,z=7,water=7,near=True)
 def run(name):
  lua=LuaRuntime(unpack_returned_tuples=True);lua.execute((FIXTURE/name).read_text(encoding='utf-8'));return lua.execute((FIXTURE/'harness.lua').read_text(encoding='utf-8'),lua.table_from(case))
 before=run('baseline_draft.lua');after=run('draft.lua')
 assert not before.registered and after.registered and after.conditionFailed
 assert 'fishing'in list(before.events.values())and'fishing'not in list(after.events.values())
 assert not after.acquired and not after.movie and after.ownerCount==0
