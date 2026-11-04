"""Native target reset evidence and complete generated control-flow regressions.

Engine calls, output ownership and suspension are modeled; this does not prove
full quest gameplay, vector ownership or the native engine implementations.
"""
from pathlib import Path
from unittest.mock import patch
import json,copy,itertools,hashlib
import pytest
from lupa.lua54 import LuaRuntime
ROOT=next(p for p in Path(__file__).resolve().parents if (p/'tools/script_recovery/script_units.py').is_file())
HERE=Path(__file__).resolve().parent
from tools.script_recovery import native_reviewed_thing_outputs as outputs
from tools.script_recovery.convert_quest_unit import UnitConverter
from tools.script_recovery.build_readable_unit import readable_file
@pytest.fixture(scope='module')
def captured(tmp_path_factory):
 unit=json.loads((ROOT/'refs/script_recovery/trader_conflict/units/Q_TraderConflictGood.json').read_text())
 candidate=HERE/'candidate_unit.json'
 if candidate.exists():unit=json.loads(candidate.read_text())
 c=UnitConverter(ROOT/'refs/script_recovery/trader_conflict/translation_unit_typed.json');out=tmp_path_factory.mktemp('trader_target_clear');result={};original=outputs.restore
 def capture(source,todo,calls,fn,row,rd,*args,**kwargs):
  recovered=original(source,todo,calls,fn,row,rd,*args,**kwargs)
  if fn['address'].lower()=='0x00dfd600':result.update(source=source,fn=fn,row=row,rd=rd,recovered=recovered[0])
  return recovered
 with patch.object(c,'constructor_defaults',return_value={}),patch.object(outputs,'restore',capture):c.convert(unit,out)
 draft=(out/'FSE/TraderConflictGood/TraderConflictGood.lua').read_text()
 result.update(draft=draft,readable=readable_file(draft,rel='FSE/TraderConflictGood/TraderConflictGood.lua')[0])
 return result
def test_exact_reset_and_cleanup_operations(captured):
 c=captured;row=c['row'];assert row is not None
 result,todo,calls=outputs.restore(c['source'],['retained'],['prior'],c['fn'],row,c['rd'])
 assert result==c['recovered'] and hashlib.sha256(result.encode()).hexdigest()==row['recoveredSha256']
 assert todo==['retained'] and calls==['prior',*row['calls']]
 assert result.count('resources:AssignThingOutput(traderOwner, nil)')==1
 assert result.count('resources:TrackThingOutput(r1)')==1
 assert 'if traderOwner ~= 0 then' in result
@pytest.mark.parametrize('fault',['native','source','extent','anchor','dependency','sidecar','intermediate','operation','recovered'])
def test_changed_evidence_fails_closed(captured,fault):
 c=captured;fn,row=copy.deepcopy(c['fn']),copy.deepcopy(c['row']);source=c['source']
 if fault=='native':row['nativeSha256']='0'*64
 elif fault=='source':fn['decompile']+='\n'
 elif fault=='extent':row['end']=hex(int(row['end'],16)+1)
 elif fault=='anchor':row['anchors'][next(iter(row['anchors']))]='00'
 elif fault=='dependency':row['dependencies']['0x8ab980']['sha256']='0'*64
 elif fault=='sidecar':row['sidecar']['LuaRetailResources.h']['sha256']='0'*64
 elif fault=='intermediate':source+='\n'
 elif fault=='operation':row['operations'][2]['before']+=' changed'
 else:row['recoveredSha256']='0'*64
 with pytest.raises(ValueError):outputs.restore(source,[],[],fn,row,c['rd'])
@pytest.mark.parametrize('style',['draft','readable'])
def test_complete_generated_target_reset_and_retirement(captured,style):
 path=HERE/'harness.lua'
 if not path.exists():path=ROOT/'tools/script_recovery/fixtures/trader_target_clear/harness.lua'
 harness=path.read_text();count=0
 for following,state,refs,distance,aware,engage,limit,abort,timer_value in itertools.product([False,True],range(3),[0,1,2],[False,True],[False,True],[False,True],[1,3,8],[0,3,4,5,6,7,8],[0,1]):
  if state==0 and refs:continue
  case=dict(following=following,state=state,refs=refs,distance=distance,aware=aware,engage=engage,limit=limit,abort=abort,timerValue=timer_value)
  lua=LuaRuntime(unpack_returned_tuples=True);lua.execute(captured[style])
  try:lua.execute(harness,lua.table_from(case))
  except Exception as error:pytest.fail(str(case)+'\n'+str(error))
  count+=1
 assert count==4704
