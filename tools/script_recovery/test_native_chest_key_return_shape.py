from pathlib import Path
import json,importlib.util,itertools
import pytest
from lupa.lua54 import LuaError
W=Path(__file__).resolve().parent/'fixtures/chest_key_result'
spec=importlib.util.spec_from_file_location('key_model',W/'model.py');model=importlib.util.module_from_spec(spec);spec.loader.exec_module(model)
def sources(style):
 before=(W/f'baseline_{style}.lua').read_text()
 after=(W/'candidate_readable.lua').read_text()if style=='readable'else next((W/'candidate_draft').rglob('*.lua')).read_text()
 return before,after
@pytest.mark.parametrize('style',['draft','readable'])
def test_actual_bridge_table_witness_and_recovered_full_main(style):
 before,after=sources(style)
 case=dict(keys=0,bridgeTable=True)
 with pytest.raises(LuaError,match='attempt to compare table with number'):model.run(before,case)
 new=model.run(after,case)
 assert 'quest.OpenChest'in new['events']
@pytest.mark.parametrize('style',['draft','readable'])
def test_key_name_inventory_and_signed_count_branches(style):
 before,after=sources(style)
 count=0
 for scenario,keys,inventory,key,controlled in itertools.product(['normal','failed_open','cancel','hit','special','shutdown'],[-2,0,1,2,10],[0,1,2,10],[None,'','OBJECT_SILVER_KEY','OBJECT_CUSTOM_KEY'],[False,True]):
  case=dict(scenario=scenario,keys=keys,inventory=inventory,keyName=key,controlled=controlled)
  # The native integer/CString-out oracle passes the returned key text to
  # inventory. This is a test reference, never an edit to generated output.
  reference=before.replace(':GetNumberOfItemsOfTypeInInventory("")',':GetNumberOfItemsOfTypeInInventory('+json.dumps(key or '')+')')
  actual=model.run(after,{**case,'bridgeTable':True});expected=model.run(reference,case)
  assert actual==expected,case
  inventory_calls=[c for c in actual['calls']if c[0]=='quest.GetNumberOfItemsOfTypeInInventory']
  assert all(c[1][-1]==(key or '')for c in inventory_calls)
  assert actual['events'].count('quest.GetNumberOfKeysNeededToUnlockChest')<=1
  count+=1
 assert count==960
