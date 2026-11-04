"""Fish operand/flag/position regression; engine and output lifetime boundaries modeled."""
from pathlib import Path
from unittest.mock import patch
import json,copy,itertools
import pytest
from lupa.lua54 import LuaRuntime
ROOT=next(p for p in Path(__file__).resolve().parents if (p/'tools/script_recovery/script_units.py').is_file())
HERE=Path(__file__).resolve().parent
from tools.script_recovery import native_reviewed_stack_aliases as aliases,native_reviewed_thing_outputs as outputs
from tools.script_recovery.convert_quest_unit import UnitConverter
from tools.script_recovery.build_readable_unit import readable_file
def test_water_vector_local_is_not_replaced_by_stale_boolean():
 from tools.script_recovery.lift_native_lua import load_manifest
 from tools.script_recovery.test_lift_native_lua import make
 lifter=make();lifter.manifest={**lifter.manifest,'GetWaterHeightAtPosition':load_manifest()['GetWaterHeightAtPosition']}
 body='\n'.join(lifter.lift('Main','''{
  C3DVector_bv position;
  bool stale;
  float water;
  position = ENGINE_Vector3(2.0, 3.0, 4.0);
  stale = false;
  water = GSI->GetWaterHeightAtPosition(position);
  return water;
}'''))
 assert 'GetWaterHeightAtPosition(position)' in body,body
 lua=LuaRuntime();lua.execute('function ENGINE_Vector3(x,y,z)return {x=x,y=y,z=z}end');seen=[]
 def query(q,v):seen.append((v['x'],v['y'],v['z']));return 7.25
 run=lua.execute('return function(Quest)\n'+body+'\nend')
 assert run(lua.table_from({'GetWaterHeightAtPosition':query}))==7.25 and seen==[(2,3,4)]
@pytest.fixture(scope='module')
def captured(tmp_path_factory):
 unit=json.loads((ROOT/'refs/script_recovery/expressions/units/Expression_Fish.json').read_text())
 if (HERE/'candidate_unit.json').exists():unit=json.loads((HERE/'candidate_unit.json').read_text())
 c=UnitConverter(ROOT/'refs/script_recovery/expressions/translation_unit_typed.json');out=tmp_path_factory.mktemp('fish_operands');result={};ar,orr=aliases.restore,outputs.restore
 def ca(source,fn,row,rd):
  recovered=ar(source,fn,row,rd)
  if fn['address'].lower()=='0x00ee95c0':result.update(aliasSource=source,aliasRow=row,fn=fn,rd=rd,aliasRecovered=recovered)
  return recovered
 def co(source,todo,calls,fn,row,rd,*args,**kwargs):
  recovered=orr(source,todo,calls,fn,row,rd,*args,**kwargs)
  if fn['address'].lower()=='0x00ee95c0':result.update(source=source,row=row,recovered=recovered[0])
  return recovered
 with patch.object(c,'constructor_defaults',return_value={}),patch.object(aliases,'restore',ca),patch.object(outputs,'restore',co):c.convert(unit,out)
 draft=(out/'FSE/Expression_Fish/Expression_Fish.lua').read_text()
 result.update(draft=draft,readable=readable_file(draft,rel='FSE/Expression_Fish/Expression_Fish.lua')[0]);return result
def test_vector_argument_survives_preceding_boolean(captured):
 assert 'GetWaterHeightAtPosition({x = native_fishingX, y = native_fishingY, z = native_fishingWater})'in captured['draft']
 assert 'HeroGoFishing((native_fishingAtSpot ~= 0))'in captured['draft']
 assert 'TODO' not in captured['draft'].split('function Init')[0]
@pytest.mark.parametrize('fault',['native','source','anchor','alias','count','edit'])
def test_scalar_evidence_fails_closed(captured,fault):
 c=captured;f,r=copy.deepcopy(c['fn']),copy.deepcopy(c['aliasRow'])
 if fault=='native':r['nativeSha256']='0'*64
 elif fault=='source':f['decompile']+='\n'
 elif fault=='anchor':r['nativeInstructions'][next(iter(r['nativeInstructions']))]='00'
 elif fault=='alias':r['alias']='native_fishingAtSpot'
 elif fault=='count':r['editCounts'][0]+=1
 else:r['edits'][0][0]+=' changed'
 with pytest.raises(ValueError):aliases.restore(c['aliasSource'],f,r,c['rd'])
@pytest.mark.parametrize('fault',['native','source','extent','anchor','dependency','sidecar','intermediate','operation','recovered'])
def test_scope_evidence_fails_closed(captured,fault):
 c=captured;f,r=copy.deepcopy(c['fn']),copy.deepcopy(c['row']);source=c['source']
 if fault=='native':r['nativeSha256']='0'*64
 elif fault=='source':f['decompile']+='\n'
 elif fault=='extent':r['end']=hex(int(r['end'],16)+1)
 elif fault=='anchor':r['anchors'][next(iter(r['anchors']))]='00'
 elif fault=='dependency':r['dependencies']['0x8ab980']['sha256']='0'*64
 elif fault=='sidecar':r['sidecar']['LuaRetailResources.h']['sha256']='0'*64
 elif fault=='intermediate':source+='\n'
 elif fault=='operation':r['operations'][2]['before']+=' changed'
 else:r['recoveredSha256']='0'*64
 with pytest.raises(ValueError):outputs.restore(source,[],[],f,r,c['rd'])
@pytest.mark.parametrize('style',['draft','readable'])
def test_complete_fishing_modes_rewards_and_cleanup(captured,style):
 path=HERE/'harness.lua'
 if not path.exists():path=ROOT/'tools/script_recovery/fixtures/fish_operands/harness.lua'
 harness=path.read_text();count=0
 for (n,selected),live,success,rod,(x,y,z) in itertools.product([(0,0),(1,0),(1,1),(3,2),(3,3)],[False,True],[False,True],range(3),[(2,3,7),(11.999,3,7),(12,3,7),(12.001,3,7),(2,13,7),(2,3,17),(float('nan'),3,7),(float('inf'),3,7),(2,3,4)]):
  case=dict(count=n,selected=selected,live=live,success=success,rod=rod,x=x,y=y,z=z,water=7,near=selected>0 and live and ((y-3)**2+(z-7)**2)+(x-2)**2<100)
  lua=LuaRuntime(unpack_returned_tuples=True);lua.execute(captured[style])
  try:lua.execute(harness,lua.table_from(case))
  except Exception as error:pytest.fail(str(case)+'\n'+str(error))
  count+=1
 for abort,control,acquire,msgwait,selected,live,success in itertools.product(range(1,30),[0,3],[0,5],[0,7],[0,2],[False,True],[False,True]):
  case=dict(count=3,selected=selected,live=live,success=success,rod=0,x=2,y=3,z=7,water=7,near=selected>0 and live,abort=abort,controlWait=control,acquireWait=acquire,msgWait=msgwait)
  lua=LuaRuntime(unpack_returned_tuples=True);lua.execute(captured[style])
  try:lua.execute(harness,lua.table_from(case))
  except Exception as error:pytest.fail(str(case)+'\n'+str(error))
  count+=1
 for nullInfo,selected,live,success in itertools.product([False,True],[2,3],[False,True],[False,True]):
  case=dict(count=3,selected=selected,live=live,success=success,rod=0,x=2,y=3,z=7,water=7,near=not nullInfo and live,nullInfo=nullInfo)
  lua=LuaRuntime(unpack_returned_tuples=True);lua.execute(captured[style])
  try:lua.execute(harness,lua.table_from(case))
  except Exception as error:pytest.fail(str(case)+'\n'+str(error))
  count+=1
 assert count==2412
