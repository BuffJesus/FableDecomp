"""Native horizontal call operands, boolean dispatch and adverse evidence guards."""
import copy,struct
import pytest
from pathlib import Path
from unittest.mock import patch
import itertools,json
from lupa.lua54 import LuaRuntime
from tools.script_recovery.native_horizontal_distance_calls import restore,TARGET,SIZE
from tools.script_recovery.native_evidence_lowering import finish_lua
from tools.script_recovery.lift_native_lua import RData
SITE=0xeeaaea
SOURCE='  first = GSI->GetHero();\n  value = Is2DDistanceBetweenThingsUnder(first,&xStack_3c,stale);\n'
FUNCTION={'calls':[{'site':hex(SITE),'target':hex(TARGET),'currentName':'Is2DDistanceBetweenThingsUnder','depth':84,'edxStack':-60}],'callOrder':[hex(SITE)]}
class Evidence:
 def __init__(self):self.body=RData().bytes_at(TARGET,SIZE);self.before=RData().bytes_at(SITE-17,17);self.after=RData().bytes_at(SITE,7)
 def bytes_at(self,address,size):
  return {TARGET:self.body,SITE-17:self.before,SITE:self.after}.get(address)
def test_original_operand_and_boolean_consumer():
 out=restore(SOURCE,FUNCTION,Evidence())
 assert out=='  first = GSI->GetHero();\n  value = ENGINE_IsHorizontalDistanceBetweenThingsUnder(first, xStack_3c, 3.0);\n'
 assert 'stale'not in out
@pytest.mark.parametrize('fault',['body','target','float_push','getter_slot','second_register','first_register','consumer','stack','depth','source_first','source_second','args','order','count','identity'])
def test_changed_or_ambiguous_evidence_remains_unresolved(fault):
 fn=copy.deepcopy(FUNCTION);source=SOURCE;e=Evidence()
 if fault=='body':e.body=b'\0'*SIZE
 elif fault=='target':e.after=b'\xe8'+struct.pack('<i',TARGET+1-SITE-5)+e.after[5:]
 elif fault=='float_push':e.before=b'\x50'+e.before[1:]
 elif fault=='getter_slot':e.before=e.before[:7]+b'\x1c'+e.before[8:]
 elif fault=='second_register':e.before=e.before[:12]+b'\x4c'+e.before[13:]
 elif fault=='first_register':e.before=e.before[:15]+b'\x8b\xd0'
 elif fault=='consumer':e.after=e.after[:5]+b'\x85\xc0'
 elif fault=='stack':fn['calls'][0]['edxStack']=-56
 elif fault=='depth':fn['calls'][0]['depth']=80
 elif fault=='source_first':source=source.replace('GSI->GetHero()','GSI->GetPlayer()')
 elif fault=='source_second':source=source.replace('xStack_3c','xStack_38')
 elif fault=='args':source=source.replace(',stale','')
 elif fault=='order':fn['callOrder']=[]
 elif fault=='count':source+=SOURCE
 else:fn['calls'][0]['target']='0xcbe2ff'
 assert restore(source,fn,e)==source
def test_name_without_native_function_evidence_is_preserved():assert restore(SOURCE,None,Evidence())==SOURCE
def test_raw_hero_getter_spelling_is_also_proven():
 raw=SOURCE.replace('GSI->GetHero()','(**(code **)(**(int **)(this + 0x40) + 0x118))(*(int **)(this + 0x40))')
 assert 'ENGINE_IsHorizontalDistanceBetweenThingsUnder(first, xStack_3c, 3.0)'in restore(raw,FUNCTION,Evidence())
@pytest.mark.parametrize('radius',[0.0,-3.0,0.125,2.999999761581421])
def test_range_comes_from_float_bits_not_decompiler_temporary(radius):
 e=Evidence();e.before=e.before[:1]+struct.pack('<f',radius)+e.before[5:]
 assert f'xStack_3c, {radius!r})'in restore(SOURCE,FUNCTION,e)
@pytest.mark.parametrize('radius',[float('nan'),float('inf'),-float('inf')])
def test_nonfinite_immediate_keeps_unresolved_source(radius):
 e=Evidence();e.before=e.before[:1]+struct.pack('<f',radius)+e.before[5:]
 assert restore(SOURCE,FUNCTION,e)==SOURCE
@pytest.mark.parametrize('value',[False,True])
def test_dispatched_predicate_stays_a_lua_boolean(value):
 from tools.script_recovery.test_lift_native_lua import make
 source='''{
 bool condition;
 condition = ENGINE_IsHorizontalDistanceBetweenThingsUnder(first, second, 3.0);
 if ((char)condition == '\\0') {
  return 2;
 }
 return 1;
}'''
 body=finish_lua('\n'.join(make().lift('Main',source)));lua=LuaRuntime();seen=[]
 resources=lua.table_from({'IsHorizontalDistanceBetweenThingsUnder':lambda _,a,b,r:seen.append((a,b,r))or value})
 run=lua.execute('return function(resources,first,second)\n'+body+'\nend')
 assert run(resources,'hero','spot')==(1 if value else 2)and seen==[('hero','spot',3)]
@pytest.fixture(scope='module')
def generated(tmp_path_factory):
 from tools.script_recovery.convert_quest_unit import UnitConverter
 from tools.script_recovery.build_readable_unit import readable_file
 root=next(p for p in Path(__file__).resolve().parents if(p/'tools/script_recovery/script_units.py').is_file())
 unit=json.loads((root/'refs/script_recovery/expressions/units/Expression_Dig.json').read_text());c=UnitConverter(root/'refs/script_recovery/expressions/translation_unit_typed.json');out=tmp_path_factory.mktemp('dig_horizontal')
 with patch.object(c,'constructor_defaults',return_value={}):c.convert(unit,out)
 draft=(out/'FSE/Expression_Dig/Expression_Dig.lua').read_text(encoding='utf-8')
 assert 'resources:IsHorizontalDistanceBetweenThingsUnder(pCVar6, r1, 3.0)'in draft
 assert 'Is2DDistanceBetweenThingsUnder'not in draft
 return dict(draft=draft,readable=readable_file(draft,rel='FSE/Expression_Dig/Expression_Dig.lua')[0])
@pytest.mark.parametrize('style',['draft','readable'])
def test_complete_generated_digging_query_branches_and_cleanup(generated,style):
 here=Path(__file__).resolve().parent;harness=here/'harness.lua'
 if not harness.exists():
  root=next(p for p in here.parents if(p/'tools/script_recovery/script_units.py').is_file());harness=root/'tools/script_recovery/fixtures/dig_horizontal_distance/harness.lua'
 source=harness.read_text(encoding='utf-8');cases=[]
 positions=[(0,0,10000),(2.999999,0,0),(3,0,0),(3.000001,0,0),(2,2,-10000),(float('nan'),0,0),(float('inf'),0,0)]
 for state,hero_live,query_live,weapon,(x,y,z)in itertools.product(range(3),[False,True],[False,True],range(4),positions):cases.append(dict(state=state,heroAlive=hero_live,queryLive=query_live,weapon=weapon,x=x,y=y,z=z))
 for state,weapon,control,acquire,(af,ap)in itertools.product(range(3),range(4),[0,3],[0,5],[(0,0),(1,0),(3,0),(5,0),(7,0),(0,2),(0,5),(0,7),(0,10)]):cases.append(dict(state=state,heroAlive=True,queryLive=True,weapon=weapon,x=0,y=0,z=10000,controlWait=control,acquireWait=acquire,abortFrame=af,abortPause=ap))
 for case in cases:
  lua=LuaRuntime(unpack_returned_tuples=True);lua.execute(generated[style])
  try:lua.execute(source,lua.table_from(case))
  except Exception as error:pytest.fail(str(case)+'\n'+str(error))
 assert len(cases)==768
