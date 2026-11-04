import copy,json,importlib.util,itertools,hashlib
from pathlib import Path
import pytest
from lupa.lua54 import LuaRuntime
from tools.script_recovery.lift_native_lua import RData
W=Path(__file__).resolve().parent;FIXTURE=W if (W/'pre_lift.c').exists()else W/'fixtures/chest_container_shape'
if False:
 spec=importlib.util.spec_from_file_location('candidate_count',W/'native_container_count.py');module=importlib.util.module_from_spec(spec);spec.loader.exec_module(module)
else:
 from tools.script_recovery import native_container_count as module
SOURCE=(FIXTURE/'pre_lift.c').read_text();FN=json.loads((FIXTURE/'function.json').read_text());ROW=json.loads((FIXTURE/'reviewed_row.json').read_text())
def test_exact_native_count_only_recovery():
 result=module.restore(SOURCE,FN,RData(),ROW)
 assert 'ENGINE_GetContainerItemDefCount((CScriptThing *)xStack_38)'in result
 assert 'bVar11 = iStack_2c == 0;'in result and 'CCombatWheel::ResetRings'not in result
 assert result.count('ENGINE_RegisterQuestConsciousCondition')==SOURCE.count('ENGINE_RegisterQuestConsciousCondition')==1

@pytest.mark.parametrize('fault',['missing','address','source_hash','input_hash','body_hash','method_hash','binding_hash','operation','begin','end','target','slot','depth','pushes','receiver','destructor_receiver','destructor_target','destructor_missing','anchor','dtor_bytes','changed_source','escaped_vector','escaped_end','changed_assignment'])
def test_unreviewed_or_changed_evidence_stays_unmodified(fault):
 source=SOURCE;fn=copy.deepcopy(FN);row=copy.deepcopy(ROW);rd=RData()
 if fault=='missing':row=None
 elif fault=='address':row['address']='0x00eec891'
 elif fault in ['source_hash','input_hash','body_hash','method_hash','binding_hash']:
  key=dict(source_hash='sourceSha256',input_hash='inputSha256',body_hash='nativeSha256',method_hash='sdkMethodSha256',binding_hash='sdkBindingSha256')[fault];row[key]='0'*64
 elif fault=='operation':row['operation']='GetItemDefNamesFromContainer'
 elif fault in ['begin','end','target']:row[fault]='other'
 elif fault in ['slot','depth','pushes','receiver']:
  c=next(c for c in fn['indirectCalls']if c['site']==ROW['callSite'])
  key=dict(slot='slot',depth='depth',pushes='pushedStack',receiver='edxStack')[fault];c[key]=dict(slot='0x934',depth=104,pushes=[-40,-56],receiver=-52)[fault]
 elif fault.startswith('destructor_'):
  c=next(c for c in fn['calls']if c['site']==ROW['destructorSites'][0])
  if fault=='destructor_receiver':c['ecxStack']=-40
  elif fault=='destructor_target':c['target']='0x00414ef0'
  else:fn['calls'].remove(c)
 elif fault in ['anchor','dtor_bytes']:
  original=rd;address=0xeecb3d if fault=='anchor'else 0x414eb7
  class Changed:
   def bytes_at(self,a,n):
    value=original.bytes_at(a,n)
    if value is not None and a<=address<a+n:
     value=bytearray(value);value[address-a]^=1;return bytes(value)
    return value
  rd=Changed()
 else:
  if fault=='changed_source':source=source.replace('iStack_2c == i_stk_28','iStack_2c != i_stk_28')
  elif fault=='changed_assignment':source=source.replace('GetItemDefNamesFromContainer','GetThingsInSphere')
  else:source+='\nuse('+('iStack_2c'if fault=='escaped_vector'else'i_stk_28')+');'
  row['inputSha256']=hashlib.sha256(source.encode()).hexdigest()
 assert module.restore(source,fn,rd,row)==source

def run(source, case):
    scenario=case.get("scenario","normal")
    registered=False
    condition_failed=False
    native=case.get("nativeCondition",False)
    lua = LuaRuntime()
    trace, calls, frame = [], [], 0
    quest, hero, chest, resources = (lua.table() for _ in range(4))
    for obj,tag in [(quest,'quest'),(hero,'hero'),(chest,'chest'),(resources,'resources')]:obj['_identity']=tag
    def argument(value):
        if isinstance(value,(str,int,float,bool))or value is None:return value
        return value['_identity']or 'table'
    def bind(obj, prefix, name, result=None):
        def call(*args):
            event=prefix+name
            trace.append(event)
            calls.append((event,tuple(argument(a)for a in args)))
            if case.get('failEvent')==event:raise RuntimeError('injected '+event)
            return result() if callable(result) else result
        obj[name] = call
    def next_frame():
        nonlocal frame
        frame += 1
        assert frame <= 20, 'Unexpected unbounded wait'
        nonlocal condition_failed
        if registered and case.get('conditionState',0) and frame>=case.get('conditionFrame',1):condition_failed=True
        return not condition_failed and not (scenario == 'shutdown' and frame >= 2)
    bind(quest, 'quest.', 'NewScriptFrame', next_frame)
    def get_hero():
        nonlocal registered
        if native and not registered:
            assert frame==0
            registered=True
        return hero
    bind(quest, 'quest.', 'GetHero', get_hero)
    def register(*args):
        nonlocal registered
        assert not registered and frame==0 and lua.eval('rawequal')(args[-1],hero)
        registered=True
        if case.get('failEvent')=='condition':raise RuntimeError('injected condition')
    quest['RegisterQuestConsciousCondition']=register
    bind(quest, 'quest.', 'RetailResources', lambda: resources)
    bind(quest, 'quest.', 'GetActiveQuestName', 'Global_OpenChest')
    bind(quest, 'quest.', 'IsHeroControlledByPlayer', case.get('controlled',True))
    bind(quest, 'quest.', 'IsActiveThreadTerminating', lambda: condition_failed or (scenario == 'shutdown' and frame >= 2))
    bind(quest, 'quest.', 'GetMostRecentValidUsedTarget', lambda: chest)
    bind(quest, 'quest.', 'GetNumberOfKeysNeededToUnlockChest',case.get('keys',0))
    bind(quest, 'quest.', 'IsEntityWieldingMeleeWeapon',case.get('weapon',0)==1)
    bind(quest, 'quest.', 'IsEntityWieldingRangedWeapon',case.get('weapon',0)==2)
    bind(quest, 'quest.', 'OpenChest', scenario != 'failed_open')
    bind(quest, 'quest.', 'IsChestOpen', lambda: frame >= 3)
    bind(quest, 'quest.', 'MsgOnChestOpeningCancelled', lambda: scenario == 'cancel' and frame >= 2)
    native_count=case.get('nativeCount',0 if case.get('empty',False) else 2)
    def names():
        if case.get('nativeListOracle') and native_count==0:return None
        strings=case.get('stringsMode',0)
        values=['item' for n in range(native_count) if strings==0 or (strings==2 and n%2)]
        return lua.table_from(values)
    bind(quest, 'quest.', 'GetItemDefNamesFromContainer', names)
    def count():
        if case.get('naiveTableCount'):
            return len(list(names().values()))
        return native_count
    # Both public bridge names correspond to the same native slot930 query.
    # Normalize this name only; retain every call, argument and ordering effect.
    def get_count(*args):
        event='quest.GetItemDefNamesFromContainer'
        trace.append(event);calls.append((event,tuple(argument(a)for a in args)))
        if case.get('failEvent')==event:raise RuntimeError('injected '+event)
        return count()
    quest['GetContainerItemDefCount']=get_count
    bind(quest, 'quest.', 'MsgOnHeroRewardedWithItemsFrom', lambda: frame>=case.get('rewardFrame',1))
    for name in ('EntityUnsheatheMeleeWeapon','EntityUnsheatheRangedWeapon','PlayCriteriaSoundOnThing','DisplayLockedChestMessage','SetQuestAsPersistent', 'DeactivateQuestLater', 'SetToKeepHeroAbilitiesDuringCutscenes',
                 'SetToDisplayTutorialsDuringCutscenes', 'SetCutsceneMode', 'EntitySetCutsceneBehaviour',
                 'PauseAllEntities', 'CameraDefault'):
        bind(quest, 'quest.', name)
    # Draft handles use an explicit pure null check; readable handles are nullable.
    chest['IsNull'] = lambda *_: False
    hero['IsNull'] = lambda *_: False
    bind(chest, 'chest.', 'IsAlive', True)
    bind(hero, 'hero.', 'MsgIsHitBy', lambda: scenario in ('hit', 'both_hits') and frame >= 2)
    bind(hero, 'hero.', 'MsgIsHitByAnySpecialAbilityFrom', lambda: scenario in ('special', 'both_hits') and frame >= 2)
    bind(resources, 'resources.', 'NewResource', 1)
    bind(resources, 'resources.', 'TryAcquire', lambda: frame>=case.get('acquireFrame',1))
    bind(resources, 'resources.', 'StartMovie', 2)
    for name in ('ClearAllActionsIncludingLoopingAnimations', 'DestroyMovie', 'ReleaseResource'):
        bind(resources, 'resources.', name)
    bind(quest,'quest.','GetNumberOfItemsOfTypeInInventory',case.get('inventory',0))
    lua.execute(source)
    raised=False
    try:lua.globals().Main(quest)
    except Exception as error:
        if 'injected 'not in str(error):raise
        raised=True
    return dict(events=trace,calls=calls,frames=frame,registered=registered,failed=condition_failed,raised=raised)




@pytest.mark.parametrize('style',['draft','readable'])
def test_complete_main_native_count_oracle_preserves_calls_and_exits(style):
 before=(FIXTURE/('baseline_'+style+'.lua')).read_text();after=(FIXTURE/(style+'.lua')).read_text()
 cases=[dict(scenario=scenario,conditionState=state,conditionFrame=when,controlled=controlled,acquireFrame=acquire,keys=keys,inventory=inventory,weapon=weapon,empty=empty,rewardFrame=reward)
 for scenario,state,when,controlled,acquire,keys,inventory,weapon,empty,reward in itertools.product(['normal','failed_open','cancel','hit','special','both_hits','shutdown'],range(3),[1,2,3,6],[False,True],[1,3],[0,2],[0,2],range(3),[False,True],[1,5])]
 cases.extend(dict(scenario=scenario,conditionState=state,conditionFrame=when,nativeCount=count,stringsMode=strings,rewardFrame=5)
 for scenario,state,when,count,strings in itertools.product(['normal','failed_open','cancel','hit','special','both_hits','shutdown'],range(3),[1,3,6],[0,1,2,8],range(3)))
 for case in cases:assert run(before,dict(case,nativeListOracle=True))==run(after,case),case
 assert len(cases)==16884

@pytest.mark.parametrize('style',['draft','readable'])
def test_old_empty_chest_waits_for_reward_and_naive_string_count_misses_null_entries(style):
 before=(FIXTURE/('baseline_'+style+'.lua')).read_text();after=(FIXTURE/(style+'.lua')).read_text()
 case=dict(nativeCount=0,rewardFrame=5)
 old=run(before,case);new=run(after,case)
 assert old['frames']==5 and new['frames']==3
 assert old['events'].count('quest.MsgOnHeroRewardedWithItemsFrom')>0 and 'quest.MsgOnHeroRewardedWithItemsFrom'not in new['events']
 case=dict(nativeCount=2,stringsMode=1,rewardFrame=5)
 correct=run(after,case);naive=run(after,dict(case,naiveTableCount=True))
 assert correct['frames']==5 and naive['frames']==3

@pytest.mark.parametrize('style',['draft','readable'])
def test_count_errors_keep_existing_cleanup_boundary(style):
 before=(FIXTURE/('baseline_'+style+'.lua')).read_text();after=(FIXTURE/(style+'.lua')).read_text()
 for event in ['quest.GetItemDefNamesFromContainer','quest.OpenChest','quest.IsChestOpen','quest.MsgOnHeroRewardedWithItemsFrom','resources.DestroyMovie','resources.ReleaseResource']:
  case=dict(nativeCount=2,rewardFrame=5,failEvent=event)
  assert run(before,dict(case,nativeListOracle=True))==run(after,case)

@pytest.mark.parametrize('fault',['cochanged_begin_metadata','cochanged_frame_metadata','cochanged_target_source'])
def test_source_stack_identity_and_original_operand_bytes_override_cochanged_metadata(fault):
 source=SOURCE;fn=copy.deepcopy(FN);row=copy.deepcopy(ROW)
 call=next(c for c in fn['indirectCalls']if c['site']==ROW['callSite'])
 if fault=='cochanged_begin_metadata':
  row['beginSlot']=-40;call['pushedStack']=[-40,-56]
  for c in fn['calls']:
   if c['site']in row['destructorSites']:c['ecxStack']=-40
 elif fault=='cochanged_frame_metadata':
  row['frameDepth']+=4;row['callDepth']+=4;call['depth']+=4
  for c in fn['calls']:
   if c['site']in row['destructorSites']:c['depth']+=4
 else:
  source=source.replace('GetItemDefNamesFromContainer((CScriptThing *)xStack_38)','GetItemDefNamesFromContainer((CScriptThing *)xStack_3c)')
  row['target']='(CScriptThing *)xStack_3c';row['inputSha256']=hashlib.sha256(source.encode()).hexdigest()
 assert module.restore(source,fn,RData(),row)==source
