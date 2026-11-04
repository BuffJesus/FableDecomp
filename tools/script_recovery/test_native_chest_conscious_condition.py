import copy,json,importlib.util,itertools
from pathlib import Path
import pytest
from lupa.lua54 import LuaRuntime
from tools.script_recovery.lift_native_lua import RData
W=Path(__file__).resolve().parent;FIXTURE=W if (W/'pre_lift.c').exists()else W/'fixtures/chest_conscious_condition'
if False:
 spec=importlib.util.spec_from_file_location('candidate_condition',W/'native_quest_conscious_condition.py');module=importlib.util.module_from_spec(spec);spec.loader.exec_module(module)
else:
 from tools.script_recovery import native_quest_conscious_condition as module
SOURCE=(FIXTURE/'pre_lift.c').read_text(encoding='utf-8');FN=json.loads((FIXTURE/'function.json').read_text(encoding='utf-8'))
ROW=json.loads((FIXTURE/'reviewed_row.json').read_text(encoding='utf-8'))

def test_original_entry_registration_before_frame():
 result=module.restore(SOURCE,FN,RData(),ROW)
 assert 'ENGINE_RegisterQuestConsciousCondition(pCVar7);'in result
 assert result.index('GSI->GetHero()')<result.index('ENGINE_RegisterQuestConsciousCondition')<result.index('GSI->NewScriptFrame()')
 assert '__atcb7920(this)'not in result

@pytest.mark.parametrize('fault',['actor','temporary','escape','duplicate','copy_depth','copy_stack','register_stack','destroy_stack','hero_slot','frame_depth','no_function','no_bytes','prefix','table','copy','register','clone','predicate','destroy','branch_into'])
def test_ambiguous_or_changed_evidence_is_preserved(fault):
 source=SOURCE;fn=copy.deepcopy(FN);rd=RData()
 calls={c['site']:c for c in fn['calls']};indirect={c['site']:c for c in fn['indirectCalls']}
 if fault=='actor':source=source.replace('CScriptThing(xStack_10,pCVar7,','CScriptThing(xStack_10,pCVar8,')
 elif fault=='temporary':source=source.replace('xStack_10','xStack_8')
 elif fault=='escape':source+='\nuse(xStack_10);'
 elif fault=='duplicate':source+='\n'+next(module.ALIAS_PATTERN.finditer(source))[0]+'\nGSI->NewScriptFrame();'
 elif fault=='copy_depth':calls['0x00EEC8B7']['depth']+=4
 elif fault=='copy_stack':calls['0x00EEC8B7']['ecxStack']=-8
 elif fault=='register_stack':calls['0x00EEC8C3']['pushedStack']=[-12]
 elif fault=='destroy_stack':calls['0x00EEC8D0']['ecxStack']=-8
 elif fault=='hero_slot':indirect['0x00EEC8A3']['slot']='0x11c'
 elif fault=='frame_depth':indirect['0x00EEC8DA']['depth']+=4
 elif fault=='no_function':fn=None
 elif fault=='no_bytes':rd=None
 else:
  address={'prefix':0xeec8a9,'table':0x12c2fe8,'copy':0x4abe90,'register':0xcb7920,'clone':0xcddae0,'predicate':0xcddab0,'destroy':0x4aa840,'branch_into':0xeec8e6}[fault]
  original=rd
  class Changed:
   def bytes_at(self,a,n):
    value=original.bytes_at(a,n)
    if value is not None and a<=address<a+n:
     value=bytearray(value)
     if fault=='branch_into':value[address-a+2:address-a+6]=(0xeec8ac-(address+6)).to_bytes(4,'little',signed=True)
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
 elif fault=='native_site':row['temporaryReuse']['nativeSite']='0x00EEC8C3'
 elif fault=='native_stack':row['temporaryReuse']['pushedStack']=[-12,-40]
 else:
  source=source.replace('GSI->NewScriptFrame();','use(xStack_10);\nGSI->NewScriptFrame();',1)if fault=='early_read'else source.replace('&PTR_LAB_012c2fe8','&PTR_LAB_012c2fec',1)
  row['temporaryReuse']['inputSha256']=__import__('hashlib').sha256(source.encode()).hexdigest()
 assert module.restore(source,FN,RData(),row)==source



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
    bind(quest, 'quest.', 'GetItemDefNamesFromContainer', lambda: lua.table() if case.get('empty',False) else lua.table_from(['item']))
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
def test_complete_chest_condition_preserves_lookup_and_cleanup(style):
 before=(FIXTURE/('baseline_'+style+'.lua')).read_text();after=(FIXTURE/(style+'.lua')).read_text()
 cases=[dict(scenario=scenario,conditionState=state,conditionFrame=when,controlled=controlled,acquireFrame=acquire,keys=keys,inventory=inventory,weapon=weapon,empty=empty,rewardFrame=reward)
 for scenario,state,when,controlled,acquire,keys,inventory,weapon,empty,reward in itertools.product(['normal','failed_open','cancel','hit','special','both_hits','shutdown'],range(3),[1,2,3,6],[False,True],[1,3],[0,2],[0,2],range(3),[False,True],[1,5])]
 for case in cases:
  old=run(before,dict(case,nativeCondition=True));new=run(after,case)
  assert old==new,case
 assert len(cases)==16128

@pytest.mark.parametrize('style',['draft','readable'])
def test_old_chest_runs_without_native_condition(style):
 case=dict(conditionState=1,conditionFrame=1)
 old=run((FIXTURE/('baseline_'+style+'.lua')).read_text(),case)
 new=run((FIXTURE/(style+'.lua')).read_text(),case)
 assert not old['failed'] and new['failed']
 assert 'resources.StartMovie'in old['events'] and 'resources.StartMovie'not in new['events']

@pytest.mark.parametrize('style',['draft','readable'])
def test_chest_injected_errors_preserve_existing_unscoped_cleanup(style):
 before=(FIXTURE/('baseline_'+style+'.lua')).read_text();after=(FIXTURE/(style+'.lua')).read_text()
 for event in ['quest.GetActiveQuestName','quest.GetMostRecentValidUsedTarget','resources.NewResource','resources.TryAcquire','resources.StartMovie','quest.OpenChest','quest.IsChestOpen','resources.DestroyMovie','resources.ReleaseResource']:
  case=dict(failEvent=event)
  assert run(before,dict(case,nativeCondition=True))==run(after,case),event
 # This round adds a condition, not resource RAII; both retain the old error behavior.
 result=run(after,dict(failEvent='condition'))
 assert result['raised'] and result['frames']==0 and 'resources.NewResource'not in result['events']
