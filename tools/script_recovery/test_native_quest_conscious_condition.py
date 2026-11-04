import copy,json,importlib.util
from pathlib import Path
import pytest
from tools.script_recovery.lift_native_lua import RData
W=Path(__file__).resolve().parent;FIXTURE=W if (W/'pre_lift.c').exists()else W/'fixtures/quest_conscious_condition'
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
 if fault=='actor':source=source.replace('CScriptThing(xStack_c,pCVar4,','CScriptThing(xStack_c,pCVar5,')
 elif fault=='temporary':source=source.replace('xStack_c','xStack_8')
 elif fault=='escape':source+='\nuse(xStack_c);'
 elif fault=='duplicate':source+='\n'+next(module.PATTERN.finditer(source))[0]+'\nGSI->NewScriptFrame();'
 elif fault=='copy_depth':calls['0x00EEA481']['depth']+=4
 elif fault=='copy_stack':calls['0x00EEA481']['ecxStack']=-8
 elif fault=='register_stack':calls['0x00EEA48D']['pushedStack']=[-12]
 elif fault=='destroy_stack':calls['0x00EEA49A']['ecxStack']=-8
 elif fault=='hero_slot':indirect['0x00EEA46D']['slot']='0x11c'
 elif fault=='frame_depth':indirect['0x00EEA4A4']['depth']+=4
 elif fault=='no_function':fn=None
 elif fault=='no_bytes':rd=None
 else:
  address={'prefix':0xeea473,'table':0x12c2fe8,'copy':0x4abe90,'register':0xcb7920,'clone':0xcddae0,'predicate':0xcddab0,'destroy':0x4aa840,'branch_into':0xeea4b0}[fault]
  original=rd
  class Changed:
   def bytes_at(self,a,n):
    value=original.bytes_at(a,n)
    if value is not None and a<=address<a+n:
     value=bytearray(value)
     if fault=='branch_into':value[address-a+2:address-a+6]=(0xeea478-(address+6)).to_bytes(4,'little',signed=True)
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
def test_complete_main_condition_oracle_and_preserved_flow(style):
 from lupa.lua54 import LuaRuntime
 baseline=(FIXTURE/f'baseline_{style}.lua').read_text(encoding='utf-8');candidate=(FIXTURE/f'{style}.lua').read_text(encoding='utf-8');harness=(FIXTURE/'harness.lua').read_text(encoding='utf-8')
 def run(source,config):
  lua=LuaRuntime(unpack_returned_tuples=True);lua.execute(source);c=lua.table_from(config);c.workers=lua.table_from([lua.table_from(dict(name='FollowThread',id=0,flag=0))]);return lua.execute(harness,c)
 errors=['','hero','frame','terminating','acquire','movie','fadeOut','pause','cancel','deactivate','camera','fadeIn','activeName','deactivateLater','destroyMovie','release','heroAcquire','heroRelease']
 count=0
 for state in [0,1,2]:
  for delay in [0,1,3]:
   for term in [1,2,3,99]:
    for frame in [1,99]:
     for pause in [0,1,2]:
      for error in errors:
       common=dict(conditionState=state,delay=delay,termAt=term,frameStop=frame,yieldPause=pause!=0,abortPause=pause==2,error=error)
       before=run(baseline,{**common,'nativeReference':True});after=run(candidate,{**common,'nativeReference':False})
       a=list(after.events.values());b=list(before.events.values());assert [e for e in a if e!='condition:']==b
       assert after.ok==before.ok and after.status==before.status and after.suspensions==before.suspensions and after.workers[1].flag==before.workers[1].flag
       if error!='hero':
        assert a.count('condition:')==1 and a.index('condition:')<a.index('frame:1')
        assert after.conditionInstalled
       count+=1
 assert count==3888

@pytest.mark.parametrize('style',['draft','readable'])
def test_old_main_misses_condition_termination(style):
 from lupa.lua54 import LuaRuntime
 lua=LuaRuntime(unpack_returned_tuples=True);lua.execute((FIXTURE/f'baseline_{style}.lua').read_text(encoding='utf-8'))
 c=lua.table_from(dict(conditionState=1,nativeReference=False,delay=0,termAt=99,frameStop=99,yieldPause=False,abortPause=False,error=''));c.workers=lua.table_from([])
 result=lua.execute((FIXTURE/'harness.lua').read_text(encoding='utf-8'),c)
 assert 'deactivate:Expression_Follow,0'in list(result.events.values()) and not result.conditionInstalled

@pytest.mark.parametrize('style',['draft','readable'])
def test_condition_binding_failure_aborts_before_frame(style):
 from lupa.lua54 import LuaRuntime
 lua=LuaRuntime(unpack_returned_tuples=True);lua.execute((FIXTURE/f'{style}.lua').read_text(encoding='utf-8'))
 c=lua.table_from(dict(conditionState=0,nativeReference=False,delay=0,termAt=99,frameStop=99,yieldPause=False,abortPause=False,error='condition'));c.workers=lua.table_from([])
 result=lua.execute((FIXTURE/'harness.lua').read_text(encoding='utf-8'),c)
 assert not result.ok and result.frames==0 and list(result.events.values())==['hero:','condition:']
