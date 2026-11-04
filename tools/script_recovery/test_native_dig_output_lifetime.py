"""Evidence guards and complete Main coverage for Dig's selected output lifetime."""
import copy,json,itertools
from pathlib import Path
from unittest.mock import patch
import pytest
from lupa.lua54 import LuaRuntime
from tools.script_recovery.native_reviewed_thing_outputs import restore
from tools.script_recovery.lift_native_lua import RData
HERE=Path(__file__).resolve().parent
ROOT=next(p for p in HERE.parents if(p/'tools/script_recovery/script_units.py').is_file())
FIXTURE=HERE if(HERE/'candidate_scope_row.json').exists()else ROOT/'tools/script_recovery/fixtures/dig_output_lifetime'
@pytest.fixture(scope='module')
def evidence():
 export=json.loads((ROOT/'refs/script_recovery/expressions/translation_unit_typed.json').read_text(encoding='utf-8-sig'))
 fn=next(f for f in export['functions']if int(f['address'],16)==0xeea7c0)
 if (HERE/'candidate_scope_row.json').exists():row=json.loads((HERE/'candidate_scope_row.json').read_text(encoding='utf-8'))
 else:row=json.loads((ROOT/'refs/script_recovery/expressions/units/Expression_Dig.json').read_text(encoding='utf-8'))['reviewedThingOutputScopes']['0x00eea7c0']
 name='candidate_intermediate.lua'if FIXTURE==HERE else'lowering_input.lua'
 return fn,row,(FIXTURE/name).read_text(encoding='utf-8'),RData()
def test_native_cleanup_order_and_retained_diagnostics(evidence):
 fn,row,source,rd=evidence;output,todo,calls=restore(source,['retained'],['prior'],fn,row,rd,root=ROOT)
 assert todo==['retained']and calls==['prior','TrackThingOutput','RetireThingOutput']
 assert output.index('resources:TrackThingOutput(r1)')<output.index('resources:IsHorizontalDistanceBetweenThingsUnder(')
 assert output.index('resources:RetireThingOutput(digSpotOwner)')<output.index('resources:DestroyMovie(')<output.index('resources:ReleaseResource(')
 assert 'quest:WithRetailResources(function(resources)'in output
 assert 'TrackThingOutput(quest:GetHero'not in output
@pytest.mark.parametrize('fault',['extent','source','native','anchor','anchor_range','dependency','sidecar','input','output','scope','selected','retirement','closure'])
def test_changed_witness_or_lifetime_shape_rejected(evidence,fault):
 fn,row,source,rd=evidence;fn=copy.deepcopy(fn);row=copy.deepcopy(row)
 if fault=='extent':fn['bodyEndExclusive']='0xeeada3'
 elif fault=='source':fn['decompile']+='\n'
 elif fault=='native':row['nativeSha256']='0'*64
 elif fault=='anchor':row['anchors'][next(iter(row['anchors']))]='00'
 elif fault=='anchor_range':row['anchors']['0x4aa840']='00'
 elif fault=='dependency':next(iter(row['dependencies'].values()))['sha256']='0'*64
 elif fault=='sidecar':next(iter(row['sidecar'].values()))['sha256']='0'*64
 elif fault=='input':source+='\n'
 elif fault=='output':row['recoveredSha256']='0'*64
 else:row['operations'][{'scope':0,'selected':1,'retirement':2,'closure':3}[fault]]['before']+='CHANGED'
 with pytest.raises(ValueError,match='reviewed Thing outputs'):restore(source,[],[],fn,row,rd,root=ROOT)
@pytest.fixture(scope='module')
def generated(evidence,tmp_path_factory):
 from tools.script_recovery.convert_quest_unit import UnitConverter
 from tools.script_recovery.build_readable_unit import readable_file
 fn,row,_,_=evidence;unit=json.loads((ROOT/'refs/script_recovery/expressions/units/Expression_Dig.json').read_text(encoding='utf-8'));unit['reviewedThingOutputScopes']={fn['address'].lower():row}
 c=UnitConverter(ROOT/'refs/script_recovery/expressions/translation_unit_typed.json');out=tmp_path_factory.mktemp('dig_output_lifetime')
 with patch.object(c,'constructor_defaults',return_value={}):c.convert(unit,out)
 draft=(out/'FSE/Expression_Dig/Expression_Dig.lua').read_text(encoding='utf-8')
 assert 'digSpotOwner = resources:TrackThingOutput(r1)'in draft
 return dict(draft=draft,readable=readable_file(draft,rel='FSE/Expression_Dig/Expression_Dig.lua')[0])
@pytest.mark.parametrize('style',['draft','readable'])
def test_complete_main_ownership_waits_aborts_and_callback_errors(generated,style):
 harness=(FIXTURE/'harness.lua').read_text(encoding='utf-8');cases=[]
 positions=[(0,0,10000),(2.999999,0,0),(3,0,0),(3.000001,0,0),(2,2,-10000),(float('nan'),0,0),(float('inf'),0,0)]
 for state,ha,sa,weapon,(x,y,z)in itertools.product(range(3),[False,True],[False,True],range(4),positions):cases.append(dict(state=state,heroAlive=ha,queryLive=sa,weapon=weapon,x=x,y=y,z=z))
 for state,weapon,control,acquire,(af,ap)in itertools.product(range(3),range(4),[0,3],[0,5],[(0,0),(1,0),(3,0),(5,0),(7,0),(0,2),(0,5),(0,7),(0,10)]):cases.append(dict(state=state,heroAlive=True,queryLive=True,weapon=weapon,x=0,y=0,z=10000,controlWait=control,acquireWait=acquire,abortFrame=af,abortPause=ap))
 for state,weapon,event in itertools.product(range(3),range(4),['face','reward','ST_DIGGING_SUCCESS','ST_DIGGING_FAILURE','dig','pause']):cases.append(dict(state=state,heroAlive=True,queryLive=True,weapon=weapon,x=0,y=0,z=10000,failEvent=event))
 for case in cases:
  lua=LuaRuntime(unpack_returned_tuples=True);lua.execute(generated[style])
  try:lua.execute(harness,lua.table_from(case))
  except Exception as error:pytest.fail(str(case)+'\n'+str(error))
 assert len(cases)==840
