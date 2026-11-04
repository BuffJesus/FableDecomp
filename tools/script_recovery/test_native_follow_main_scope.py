import copy,hashlib,json,itertools
from pathlib import Path
import pytest
from tools.script_recovery import native_reviewed_thing_outputs as scopes
from tools.script_recovery.lift_native_lua import RData
W=Path(__file__).resolve().parent;FIXTURE=W if(W/'candidate_scope_row.json').exists()else W/'fixtures/follow_main_scope'
ROW=json.loads((FIXTURE/'candidate_scope_row.json').read_text(encoding='utf-8'));FN=json.loads((FIXTURE/'function.json').read_text(encoding='utf-8'));SOURCE=(FIXTURE/'candidate_intermediate.lua').read_text(encoding='utf-8')

def test_original_cleanup_positions_and_fallback_scope():
 output=scopes.restore(SOURCE,[],[],FN,ROW,RData())[0]
 assert 'quest:WithRetailResources(function(resources)'in output
 assert output.index('quest:CreateThread(')<output.index('resources:DestroyMovie(')
 assert output.index('resources:DestroyMovie(')<output.index('resources:ReleaseResource(')
 assert 'TrackThingOutput'not in output

@pytest.mark.parametrize('fault',['extent','export','native','anchor','dependency','sidecar','intermediate','operation','recovered'])
def test_changed_ownership_evidence_is_rejected(fault):
 row=copy.deepcopy(ROW);source=SOURCE
 if fault=='extent':row['end']='0x00eea23e'
 elif fault=='export':row['sourceSha256']='0'*64
 elif fault=='native':row['nativeSha256']='0'*64
 elif fault=='anchor':row['anchors'][next(iter(row['anchors']))]='00'
 elif fault=='dependency':row['dependencies'][next(iter(row['dependencies']))]['sha256']='0'*64
 elif fault=='sidecar':row['sidecar'][next(iter(row['sidecar']))]['sha256']='0'*64
 elif fault=='intermediate':source+='\n'
 elif fault=='operation':row['operations'][0]['count']=2
 else:row['recoveredSha256']='0'*64
 with pytest.raises(ValueError):scopes.restore(source,[],[],FN,row,RData())

@pytest.mark.parametrize('style',['draft','readable'])
def test_complete_main_ownership_and_native_flow(style):
 from lupa.lua54 import LuaRuntime
 generated=(FIXTURE/f'{style}.lua').read_text(encoding='utf-8');native=(FIXTURE/'baseline_draft.lua').read_text(encoding='utf-8');harness=(FIXTURE/'harness.lua').read_text(encoding='utf-8')
 def run(text,case):
  lua=LuaRuntime(unpack_returned_tuples=True);lua.execute(text);return lua.execute(harness,lua.table_from(case))
 errors=['','hero','frame','terminating','resource','acquire','movie','fadeOut','pause','spawn','camera','fadeIn','destroyMovie','release']
 count=0
 for state,delay,term,conditionFrame,yielding,error in itertools.product([0],[0,1,3],[1,2,3,99],[1],['none','frame','pause','abandon'],errors):
  case=dict(conditionState=state,conditionFrame=conditionFrame,delay=delay,termAt=term,frameStop=99,yieldFrame=yielding in('frame','abandon'),yieldPause=yielding in('pause','abandon'),abandon=yielding=='abandon',error=error)
  before=run(native,case);after=run(generated,case)
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
 assert count==672

def test_old_unscoped_native_draft_retains_resource_after_lua_error():
 from lupa.lua54 import LuaRuntime
 lua=LuaRuntime(unpack_returned_tuples=True);lua.execute((FIXTURE/'baseline_draft.lua').read_text(encoding='utf-8'))
 case=dict(conditionState=0,conditionFrame=1,delay=0,termAt=99,frameStop=99,yieldFrame=False,yieldPause=False,abandon=False,error='pause')
 result=lua.execute((FIXTURE/'harness.lua').read_text(encoding='utf-8'),lua.table_from(case))
 assert not result.ok and not result.closed and list(result.active.values())==['resource','movie']
