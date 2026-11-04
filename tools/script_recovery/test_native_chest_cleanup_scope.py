from pathlib import Path
import copy,json,itertools,importlib.util
import pytest
from tools.script_recovery import native_reviewed_thing_outputs as scopes
from tools.script_recovery.lift_native_lua import RData
W=Path(__file__).resolve().parent;FIXTURE=W if (W/'candidate_scope_row.json').exists() else W/'fixtures/chest_cleanup_scope'
spec=importlib.util.spec_from_file_location('_chest_cleanup_scope_model',FIXTURE/'model.py');model=importlib.util.module_from_spec(spec);spec.loader.exec_module(model);run=model.run
ROW=json.loads((FIXTURE/'candidate_scope_row.json').read_text());FN=json.loads((FIXTURE/'function.json').read_text());SOURCE=(FIXTURE/'candidate_intermediate.lua').read_text()

@pytest.mark.parametrize('fault',['extent','export','native','anchor','dependency','sidecar','intermediate','operation','recovered'])
def test_changed_evidence_rejects_cleanup(fault):
 row=copy.deepcopy(ROW);source=SOURCE
 if fault=='extent':row['end']='0x00eece97'
 elif fault=='export':row['sourceSha256']='0'*64
 elif fault=='native':row['nativeSha256']='0'*64
 elif fault=='anchor':row['anchors'][next(iter(row['anchors']))]='00'
 elif fault=='dependency':row['dependencies'][next(iter(row['dependencies']))]['sha256']='0'*64
 elif fault=='sidecar':row['sidecar'][next(iter(row['sidecar']))]['sha256']='0'*64
 elif fault=='intermediate':source+='\n'
 elif fault=='operation':row['operations'][0]['count']=2
 else:row['recoveredSha256']='0'*64
 with pytest.raises(ValueError):scopes.restore(source,[],[],FN,row,RData())

def text(style,candidate):
 if style=='readable':return (FIXTURE/('candidate_readable.lua'if candidate else 'baseline_readable.lua')).read_text()
 return (FIXTURE/('candidate_draft'if candidate else 'baseline_draft')/'FSE/Global_OpenChest/Global_OpenChest.lua').read_text()

def native_target_oracle(source):
 # Only model the selected-output destructor at its verified native sites.
 # Callback/error cleanup is deliberately absent from this native normal-exit oracle.
 for operation in ROW['operations'][1:4]:
  assert source.count(operation['before'])==1
  source=source.replace(operation['before'],operation['after'])
 return source.replace('    local resources = quest:RetailResources()', '    local resources = quest:RetailResources()\n    local chestTargetOwner = 0')

@pytest.mark.parametrize('style',['draft','readable'])
def test_whole_main_preserves_native_calls_and_ownership(style):
 # Compare each presentation with the same draft native cleanup oracle.
 before=native_target_oracle(text('draft',False));after=text(style,True)
 cases=[dict(scenario=scenario,conditionState=state,conditionFrame=when,controlled=controlled,acquireFrame=acquire,keys=keys,inventory=inventory,weapon=weapon,empty=empty,rewardFrame=reward)
 for scenario,state,when,controlled,acquire,keys,inventory,weapon,empty,reward in itertools.product(['normal','failed_open','cancel','hit','special','both_hits','shutdown'],range(3),[1,2,3,6],[False,True],[1,3],[0,2],[0,2],range(3),[False,True],[1,5])]
 cases.extend(dict(nullTarget=null,targetAlive=alive,controlled=controlled)for null,alive,controlled in itertools.product([False,True],[False,True],[False,True]))
 for case in cases:
  old=run(before,case);new=run(after,case)
  assert new['closed'] and not new['active'],case
  assert not old['closed'] and not old['active'],case
  assert {k:v for k,v in new.items()if k!='closed'}=={k:v for k,v in old.items()if k!='closed'},case
 assert len(cases)==16136

@pytest.mark.parametrize('style',['draft','readable'])
def test_error_fallback_retires_constructed_objects_in_order(style):
 before=native_target_oracle(text('draft',False));after=text(style,True)
 errors=['quest.GetHero','condition','quest.NewScriptFrame','quest.IsActiveThreadTerminating','quest.GetMostRecentValidUsedTarget','chest.IsAlive','quest.GetNumberOfKeysNeededToUnlockChest','resources.NewResource','resources.TryAcquire','quest.SetToKeepHeroAbilitiesDuringCutscenes','resources.ClearAllActionsIncludingLoopingAnimations','resources.StartMovie','quest.SetCutsceneMode','quest.EntitySetCutsceneBehaviour','quest.PauseAllEntities','quest.GetItemDefNamesFromContainer','quest.OpenChest','quest.IsChestOpen','quest.MsgOnHeroRewardedWithItemsFrom','quest.CameraDefault','quest.DeactivateQuestLater']
 for error,keys,weapon in itertools.product(errors,[0,2],range(3)):
  case=dict(failEvent=error,keys=keys,weapon=weapon,nativeCount=2,rewardFrame=5)
  old=run(before,case);new=run(after,case)
  assert new['closed'] and not new['active'],case
  for key in ['frames','registered','failed','raised']:assert new[key]==old[key],(case,key)
  assert new['calls'][:len(old['calls'])]==old['calls'],case
  # The extra calls are solely resource destructor fallback, after the error.
  assert all(c[0]in ['resources.DestroyMovie','resources.ReleaseResource']for c in new['calls'][len(old['calls']):]),case
  kinds=[k for k,_ in new['ownership'] if k!='track']
  assert len(kinds)==len(set(kinds)),case
  assert kinds==[k for k in ['movie','resource','target']if k in kinds],case

def test_unscoped_error_witness_retains_all_three_outputs():
 source=native_target_oracle(text('draft',False));case=dict(failEvent='quest.OpenChest')
 old=run(source,case);new=run(text('draft',True),case)
 assert old['raised'] and old['active']==['target','resource','movie']
 assert new['raised'] and new['closed'] and not new['active']
