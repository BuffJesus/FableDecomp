"""Evidence guards and complete Main coverage for Steal's target output lifetime."""
import copy,json,itertools
from pathlib import Path
from unittest.mock import patch
import pytest
from lupa.lua54 import LuaRuntime
from tools.script_recovery.native_reviewed_thing_outputs import restore
from tools.script_recovery.lift_native_lua import RData
HERE=Path(__file__).resolve().parent
ROOT=next(p for p in HERE.parents if(p/'tools/script_recovery/script_units.py').is_file())
FIXTURE=HERE if(HERE/'candidate_scope_row.json').exists()else ROOT/'tools/script_recovery/fixtures/steal_output_lifetime'
@pytest.fixture(scope='module')
def evidence():
 export=json.loads((ROOT/'refs/script_recovery/expressions/translation_unit_typed.json').read_text(encoding='utf-8-sig'))
 fn=next(f for f in export['functions']if int(f['address'],16)==0xeebdf0)
 if (HERE/'candidate_scope_row.json').exists():row=json.loads((HERE/'candidate_scope_row.json').read_text(encoding='utf-8'))
 else:row=json.loads((ROOT/'refs/script_recovery/expressions/units/Expression_Steal.json').read_text(encoding='utf-8'))['reviewedThingOutputScopes']['0x00eebdf0']
 name='candidate_intermediate.lua'if FIXTURE==HERE else'lowering_input.lua'
 return fn,row,(FIXTURE/name).read_text(encoding='utf-8'),RData()
def test_native_cleanup_order_and_retained_diagnostics(evidence):
 fn,row,source,rd=evidence;output,todo,calls=restore(source,['retained'],['prior'],fn,row,rd,root=ROOT)
 assert todo==['retained']and calls==['prior','TrackThingOutput','RetireThingOutput']
 assert output.index('resources:TrackThingOutput(r1)')<output.index('resources:StartMovie(')
 end_movie=output.index('resources:DestroyMovie(')
 deactivate=output.index('quest:DeactivateQuestLater(',end_movie)
 retire=output.index('resources:RetireThingOutput(stealTargetOwner)',deactivate)
 assert end_movie<deactivate<retire<output.index('return',retire)
 assert 'quest:WithRetailResources(function(resources)'in output
 assert 'TrackThingOutput(quest:GetHero'not in output
@pytest.mark.parametrize('fault',['extent','source','native','anchor','anchor_range','dependency','sidecar','input','output','scope','selected','retirement','closure'])
def test_changed_witness_or_lifetime_shape_rejected(evidence,fault):
 fn,row,source,rd=evidence;fn=copy.deepcopy(fn);row=copy.deepcopy(row)
 if fault=='extent':fn['bodyEndExclusive']='0xeec2c9'
 elif fault=='source':fn['decompile']+='\n'
 elif fault=='native':row['nativeSha256']='0'*64
 elif fault=='anchor':row['anchors'][next(iter(row['anchors']))]='00'
 elif fault=='anchor_range':row['anchors']['0x4aa840']='00'
 elif fault=='dependency':next(iter(row['dependencies'].values()))['sha256']='0'*64
 elif fault=='sidecar':next(iter(row['sidecar'].values()))['sha256']='0'*64
 elif fault=='input':source+='\n'
 elif fault=='output':row['recoveredSha256']='0'*64
 else:row['operations'][{'scope':0,'selected':1,'retirement':2,'closure':len(row['operations'])-1}[fault]]['before']+='CHANGED'
 with pytest.raises(ValueError,match='reviewed Thing outputs'):restore(source,[],[],fn,row,rd,root=ROOT)
@pytest.fixture(scope='module')
def generated(evidence,tmp_path_factory):
 from tools.script_recovery.convert_quest_unit import UnitConverter
 from tools.script_recovery.build_readable_unit import readable_file
 fn,row,_,_=evidence;unit=json.loads((ROOT/'refs/script_recovery/expressions/units/Expression_Steal.json').read_text(encoding='utf-8'));unit['reviewedThingOutputScopes']={fn['address'].lower():row}
 c=UnitConverter(ROOT/'refs/script_recovery/expressions/translation_unit_typed.json');out=tmp_path_factory.mktemp('steal_output_lifetime')
 with patch.object(c,'constructor_defaults',return_value={}):c.convert(unit,out)
 draft=(out/'FSE/Expression_Steal/Expression_Steal.lua').read_text(encoding='utf-8')
 assert 'stealTargetOwner = resources:TrackThingOutput(r1)'in draft
 return dict(draft=draft,readable=readable_file(draft,rel='FSE/Expression_Steal/Expression_Steal.lua')[0])
@pytest.mark.parametrize('style',['draft','readable'])
def test_complete_main_ownership_completion_skips_aborts_and_callback_errors(generated,style):
 harness=(FIXTURE/'harness.lua').read_text(encoding='utf-8');cases=[]
 for state,pickable,movie,release,hit,witness,die,duration,rng in itertools.product(range(3),[False,True],[False,True],[0,2,4],[0,3],[0,3],[0,3],[3,3.5],[0,99]):cases.append(dict(targetState=state,pickable=pickable,inMovie=movie,releaseAt=release,hitAt=hit,witnessAt=witness,dieAt=die,duration=duration,random=rng))
 base=dict(releaseAt=0,hitAt=0,witnessAt=0,dieAt=0,duration=3,random=0)
 for state,pickable,movie,abort,release in itertools.product(range(3),[False,True],[False,True],range(1,8),[0,3]):cases.append(dict(base,targetState=state,pickable=pickable,inMovie=movie,abortFrame=abort,releaseAt=release))
 for state,pickable,movie,event in itertools.product(range(3),[False,True],[False,True],['query','movie-start','display-on','timer','stat','deed','button','hit','witness','bar','unlock','morality','deactivate']):cases.append(dict(base,targetState=state,pickable=pickable,inMovie=movie,failEvent=event))
 for case in cases:
  lua=LuaRuntime(unpack_returned_tuples=True);lua.execute(generated[style])
  try:lua.execute(harness,lua.table_from(case))
  except Exception as error:pytest.fail(str(case)+'\n'+str(error))
 assert len(cases)==1476
