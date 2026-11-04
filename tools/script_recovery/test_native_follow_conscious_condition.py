import copy,json,importlib.util,itertools
from pathlib import Path
import pytest
from tools.script_recovery.lift_native_lua import RData
W=Path(__file__).resolve().parent;FIXTURE=W if (W/'pre_lift.c').exists()else W/'fixtures/follow_conscious_condition'
if FIXTURE==W:
 spec=importlib.util.spec_from_file_location('candidate_condition',W/'native_quest_conscious_condition.py');module=importlib.util.module_from_spec(spec);spec.loader.exec_module(module)
else:
 from tools.script_recovery import native_quest_conscious_condition as module
SOURCE=(FIXTURE/'pre_lift.c').read_text(encoding='utf-8');FN=json.loads((FIXTURE/'function.json').read_text(encoding='utf-8'))
ROW=json.loads((FIXTURE/'reviewed_row.json').read_text(encoding='utf-8'))

def test_original_entry_registration_before_frame():
 result=module.restore(SOURCE,FN,RData(),ROW)
 assert 'ENGINE_RegisterQuestConsciousCondition(pCVar3);'in result
 assert result.index('GSI->GetHero()')<result.index('ENGINE_RegisterQuestConsciousCondition')<result.index('GSI->NewScriptFrame()')
 assert '__atcb7920(this)'not in result

@pytest.mark.parametrize('fault',['actor','temporary','escape','duplicate','copy_depth','copy_stack','register_stack','destroy_stack','hero_slot','frame_depth','no_function','no_bytes','prefix','table','copy','register','clone','predicate','destroy','branch_into'])
def test_ambiguous_or_changed_evidence_is_preserved(fault):
 source=SOURCE;fn=copy.deepcopy(FN);rd=RData()
 calls={c['site']:c for c in fn['calls']};indirect={c['site']:c for c in fn['indirectCalls']}
 if fault=='actor':source=source.replace('CScriptThing(xStack_c,pCVar3,','CScriptThing(xStack_c,pCVar4,')
 elif fault=='temporary':source=source.replace('xStack_c','xStack_8')
 elif fault=='escape':source+='\nuse(xStack_c);'
 elif fault=='duplicate':source+='\n'+next(module.PATTERN.finditer(source))[0]+'\nGSI->NewScriptFrame();'
 elif fault=='copy_depth':calls['0x00EEA047']['depth']+=4
 elif fault=='copy_stack':calls['0x00EEA047']['ecxStack']=-8
 elif fault=='register_stack':calls['0x00EEA053']['pushedStack']=[-12]
 elif fault=='destroy_stack':calls['0x00EEA060']['ecxStack']=-8
 elif fault=='hero_slot':indirect['0x00EEA033']['slot']='0x11c'
 elif fault=='frame_depth':indirect['0x00EEA06A']['depth']+=4
 elif fault=='no_function':fn=None
 elif fault=='no_bytes':rd=None
 else:
  address={'prefix':0xeea039,'table':0x12c2fe8,'copy':0x4abe90,'register':0xcb7920,'clone':0xcddae0,'predicate':0xcddab0,'destroy':0x4aa840,'branch_into':0xeea076}[fault]
  original=rd
  class Changed:
   def bytes_at(self,a,n):
    value=original.bytes_at(a,n)
    if value is not None and a<=address<a+n:
     value=bytearray(value)
     if fault=='branch_into':value[address-a+2:address-a+6]=(0xeea03e-(address+6)).to_bytes(4,'little',signed=True)
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
def test_complete_main_condition_and_preserved_cleanup(style):
 from lupa.lua54 import LuaRuntime
 generated=(FIXTURE/f'{style}.lua').read_text(encoding='utf-8');native=(FIXTURE/'baseline_draft.lua').read_text(encoding='utf-8');harness=(FIXTURE/'harness.lua').read_text(encoding='utf-8')
 def run(text,case):
  lua=LuaRuntime(unpack_returned_tuples=True);lua.execute(text);return lua.execute(harness,lua.table_from(case))
 errors=['','hero','condition','frame','terminating','resource','acquire','movie','fadeOut','pause','spawn','camera','fadeIn','destroyMovie','release']
 count=0
 for state,delay,term,conditionFrame,yielding,error in itertools.product([0,1,2],[0,1,3],[1,2,3,99],[1,3],['none','frame','pause','abandon'],errors):
  case=dict(conditionState=state,conditionFrame=conditionFrame,delay=delay,termAt=term,frameStop=99,yieldFrame=yielding in('frame','abandon'),yieldPause=yielding in('pause','abandon'),abandon=yielding=='abandon',error=error)
  before=run(native,dict(case,nativeCondition=True));after=run(generated,case)
  events=list(after.events.values());baseline=list(before.events.values())
  ignored=lambda e:e.startswith('hero:')or e in('scope:','close:','fallbackMovie:','fallbackResource:')
  assert [e for e in events if not ignored(e)]==[e for e in baseline if not ignored(e)]
  assert after.ok==before.ok and after.frames==before.frames and after.checks==before.checks and after.attempts==before.attempts and after.suspensions==before.suspensions and after.status==before.status
  if after.status=='suspended':assert not after.closed
  elif 'scope:'in events:
   assert after.closed and len(after.active)==0
   clean=[e for e in events if e in('fallbackMovie:','fallbackResource:','destroyMovie:','release:')]
   assert len(clean)==len(set(clean))
   movies=[i for i,e in enumerate(clean)if e in('fallbackMovie:','destroyMovie:')];resources=[i for i,e in enumerate(clean)if e in('fallbackResource:','release:')]
   if movies and resources:assert movies[0]<resources[0]
  count+=1
 assert count==4320

def test_old_scoped_main_misses_modeled_dead_hero_termination():
 from lupa.lua54 import LuaRuntime
 case=dict(conditionState=1,conditionFrame=1,delay=0,termAt=99,frameStop=99,yieldFrame=False,yieldPause=False,abandon=False,error='')
 def run(name):
  lua=LuaRuntime(unpack_returned_tuples=True);lua.execute((FIXTURE/name).read_text(encoding='utf-8'));return lua.execute((FIXTURE/'harness.lua').read_text(encoding='utf-8'),lua.table_from(case))
 before=run('baseline_draft.lua');after=run('draft.lua')
 assert not before.registered and after.registered
 assert 'spawn:'in list(before.events.values())and'spawn:'not in list(after.events.values())
 assert after.closed and len(after.active)==0
