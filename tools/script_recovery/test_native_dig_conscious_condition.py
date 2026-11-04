import copy,json,importlib.util,itertools
from pathlib import Path
import pytest
from tools.script_recovery.lift_native_lua import RData
W=Path(__file__).resolve().parent;FIXTURE=W if (W/'pre_lift.c').exists()else W/'fixtures/dig_conscious_condition'
if False:
 spec=importlib.util.spec_from_file_location('candidate_condition',W/'native_quest_conscious_condition.py');module=importlib.util.module_from_spec(spec);spec.loader.exec_module(module)
else:
 from tools.script_recovery import native_quest_conscious_condition as module
SOURCE=(FIXTURE/'pre_lift.c').read_text(encoding='utf-8');FN=json.loads((FIXTURE/'function.json').read_text(encoding='utf-8'))
ROW=json.loads((FIXTURE/'reviewed_row.json').read_text(encoding='utf-8'))

def test_original_entry_registration_before_frame():
 result=module.restore(SOURCE,FN,RData(),ROW)
 assert 'ENGINE_RegisterQuestConsciousCondition(pCVar6);'in result
 assert result.index('GSI->GetHero()')<result.index('ENGINE_RegisterQuestConsciousCondition')<result.index('GSI->NewScriptFrame()')
 assert '__atcb7920(this)'not in result

@pytest.mark.parametrize('fault',['actor','temporary','escape','duplicate','copy_depth','copy_stack','register_stack','destroy_stack','hero_slot','frame_depth','no_function','no_bytes','prefix','table','copy','register','clone','predicate','destroy','branch_into'])
def test_ambiguous_or_changed_evidence_is_preserved(fault):
 source=SOURCE;fn=copy.deepcopy(FN);rd=RData()
 calls={c['site']:c for c in fn['calls']};indirect={c['site']:c for c in fn['indirectCalls']}
 if fault=='actor':source=source.replace('CScriptThing(xStack_c,pCVar6,','CScriptThing(xStack_c,pCVar7,')
 elif fault=='temporary':source=source.replace('xStack_c','xStack_8')
 elif fault=='escape':source+='\nuse(xStack_c);'
 elif fault=='duplicate':source+='\n'+next(module.PATTERN.finditer(source))[0]+'\nGSI->NewScriptFrame();'
 elif fault=='copy_depth':calls['0x00EEA7E1']['depth']+=4
 elif fault=='copy_stack':calls['0x00EEA7E1']['ecxStack']=-8
 elif fault=='register_stack':calls['0x00EEA7ED']['pushedStack']=[-12]
 elif fault=='destroy_stack':calls['0x00EEA7FA']['ecxStack']=-8
 elif fault=='hero_slot':indirect['0x00EEA7CD']['slot']='0x11c'
 elif fault=='frame_depth':indirect['0x00EEA804']['depth']+=4
 elif fault=='no_function':fn=None
 elif fault=='no_bytes':rd=None
 else:
  address={'prefix':0xeea7d3,'table':0x12c2fe8,'copy':0x4abe90,'register':0xcb7920,'clone':0xcddae0,'predicate':0xcddab0,'destroy':0x4aa840,'branch_into':0xeea810}[fault]
  original=rd
  class Changed:
   def bytes_at(self,a,n):
    value=original.bytes_at(a,n)
    if value is not None and a<=address<a+n:
     value=bytearray(value)
     if fault=='branch_into':value[address-a+2:address-a+6]=(0xeea7d8-(address+6)).to_bytes(4,'little',signed=True)
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

def test_condition_preserves_counted_output_scope_operations():
 from tools.script_recovery.native_reviewed_thing_outputs import restore
 unit=json.loads((FIXTURE/'candidate_unit.json').read_text(encoding='utf-8'))if FIXTURE==W else json.loads((W.parents[1]/'refs/script_recovery/expressions/units/Expression_Dig.json').read_text(encoding='utf-8'))
 row=unit['reviewedThingOutputScopes']['0x00eea7c0']
 output=restore((FIXTURE/'new_scope_input.lua').read_text(encoding='utf-8'),[],[],FN,row,RData())[0]
 assert output.index('quest:RegisterQuestConsciousCondition(')<output.index('quest:NewScriptFrame()')
 assert output.index('resources:TrackThingOutput(r1)')<output.index('resources:IsHorizontalDistanceBetweenThingsUnder(')
 assert output.index('resources:RetireThingOutput(digSpotOwner)')<output.index('resources:DestroyMovie(')<output.index('resources:ReleaseResource(')


@pytest.mark.parametrize('style',['draft','readable'])
def test_complete_main_condition_oracle_and_counted_output_cleanup(style):
 from lupa.lua54 import LuaRuntime
 harness=(FIXTURE/'harness.lua').read_text(encoding='utf-8')
 def run(name,case):
  lua=LuaRuntime(unpack_returned_tuples=True);lua.execute((FIXTURE/name).read_text(encoding='utf-8'));return lua.execute(harness,lua.table_from(case))
 cases=[]
 for condition,when,spot,weapon,control,acquire in itertools.product(range(3),[1,3,6],range(3),range(4),[0,3],[0,5]):
  cases.append(dict(conditionState=condition,conditionFrame=when,state=spot,heroAlive=True,queryLive=True,weapon=weapon,x=0,y=0,z=10000,controlWait=control,acquireWait=acquire))
 for condition,spot,weapon,(af,ap),error in itertools.product(range(3),range(3),range(4),[(1,0),(3,0),(5,0),(0,2),(0,5),(0,7)],['','hero','condition','frame','face','reward','ST_DIGGING_SUCCESS','ST_DIGGING_FAILURE','dig','pause']):
  cases.append(dict(conditionState=condition,conditionFrame=3,state=spot,heroAlive=True,queryLive=True,weapon=weapon,x=0,y=0,z=10000,abortFrame=af,abortPause=ap,failEvent=error))
 for case in cases:
  before=run('baseline_'+style+'.lua',dict(case,nativeCondition=True));after=run(style+'.lua',case)
  clean=lambda result:[e for e in result.events.values()if e!='hero']
  assert clean(before)==clean(after),case
  assert after.ok==before.ok and after.frames==before.frames and after.pauses==before.pauses and after.retired==before.retired and after.conditionFailed==before.conditionFailed,case
  assert after.registered==before.registered and not after.resource and not after.movie and not after.spotOwned,case
 assert len(cases)==2592

def test_old_dig_main_continues_after_modeled_dead_hero_condition():
 from lupa.lua54 import LuaRuntime
 case=dict(conditionState=1,conditionFrame=1,state=2,heroAlive=True,queryLive=True,weapon=0,x=0,y=0,z=0)
 def run(name):
  lua=LuaRuntime(unpack_returned_tuples=True);lua.execute((FIXTURE/name).read_text(encoding='utf-8'));return lua.execute((FIXTURE/'harness.lua').read_text(encoding='utf-8'),lua.table_from(case))
 before=run('baseline_draft.lua');after=run('draft.lua')
 assert not before.registered and after.registered and after.conditionFailed
 assert 'dig'in list(before.events.values())and'dig'not in list(after.events.values())
 assert not after.resource and not after.movie and not after.spotOwned
