from pathlib import Path
import json,importlib.util,itertools
import pytest
W=Path(__file__).resolve().parent/'fixtures/chest_key_result'
spec=importlib.util.spec_from_file_location('full_key_model',W/'model.py');model=importlib.util.module_from_spec(spec);spec.loader.exec_module(model)
@pytest.mark.parametrize('style',['draft','readable'])
def test_all_prior_chest_paths_with_bridge_table_and_key_name(style):
 before=(W/f'baseline_{style}.lua').read_text();after=(W/'candidate_readable.lua').read_text()if style=='readable'else next((W/'candidate_draft').rglob('*.lua')).read_text()
 key='OBJECT_SILVER_KEY';reference=before.replace(':GetNumberOfItemsOfTypeInInventory("")',':GetNumberOfItemsOfTypeInInventory('+json.dumps(key)+')')
 cases=[dict(scenario=scenario,conditionState=state,conditionFrame=when,controlled=controlled,acquireFrame=acquire,keys=keys,inventory=inventory,weapon=weapon,empty=empty,rewardFrame=reward,keyName=key)
 for scenario,state,when,controlled,acquire,keys,inventory,weapon,empty,reward in itertools.product(['normal','failed_open','cancel','hit','special','both_hits','shutdown'],range(3),[1,2,3,6],[False,True],[1,3],[0,2],[0,2],range(3),[False,True],[1,5])]
 cases += [dict(nullTarget=null,targetAlive=alive,controlled=controlled,keyName=key)for null,alive,controlled in itertools.product([False,True],repeat=3)]
 for case in cases:assert model.run(after,{**case,'bridgeTable':True})==model.run(reference,case),case
 assert len(cases)==16136
 events=['quest.GetHero','condition','quest.NewScriptFrame','quest.GetMostRecentValidUsedTarget','chest.IsAlive','quest.GetNumberOfKeysNeededToUnlockChest','quest.GetNumberOfItemsOfTypeInInventory','resources.NewResource','resources.TryAcquire','quest.SetToKeepHeroAbilitiesDuringCutscenes','resources.ClearAllActionsIncludingLoopingAnimations','resources.StartMovie','quest.SetCutsceneMode','quest.EntitySetCutsceneBehaviour','quest.PauseAllEntities','quest.GetItemDefNamesFromContainer','quest.OpenChest','quest.IsChestOpen','quest.MsgOnHeroRewardedWithItemsFrom','quest.CameraDefault','quest.DeactivateQuestLater']
 for event,keys,weapon in itertools.product(events,[0,2],range(3)):
  case=dict(failEvent=event,keys=keys,weapon=weapon,nativeCount=2,rewardFrame=5,keyName=key)
  assert model.run(after,{**case,'bridgeTable':True})==model.run(reference,case),case
