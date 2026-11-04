"""Actual PlayWave multi-spawn regression and fail-closed native recovery."""
import ast, copy, itertools, json
from pathlib import Path
from unittest.mock import patch
import pytest
from lupa.lua54 import LuaRuntime
from tools.script_recovery.convert_quest_unit import UnitConverter
from tools.script_recovery.build_readable_unit import readable_file
from tools.script_recovery.native_reviewed_arena_spawn_selection import recover_arena_spawn_selection
ROOT=Path(__file__).resolve().parents[2]
@pytest.fixture(scope='module')
def generated(tmp_path_factory):
 unit=json.loads((ROOT/'refs/script_recovery/arena/units/Q_Arena.json').read_text())
 c=UnitConverter(ROOT/'refs/script_recovery/arena/translation_unit_typed.json');seen={}
 def capture(source,fn,rdata):
  if int(fn['address'],16)==0xF1EED0:seen['input']=source,fn,rdata
  return recover_arena_spawn_selection(source,fn,rdata)
 out=tmp_path_factory.mktemp('spawn_select')
 with patch('tools.script_recovery.native_reviewed_arena_spawn_selection.recover_arena_spawn_selection',capture),patch.object(c,'constructor_defaults',return_value={}):c.convert(unit,out)
 body=(out/'FSE/Arena/Arena.lua').read_text()
 return body,readable_file(body)[0],seen['input']

def test_native_guard_and_all_occurrences(generated):
 source,fn,rdata=generated[2]
 assert recover_arena_spawn_selection(source,fn,rdata)!=source
 moved=copy.deepcopy(fn);moved['address']='0x00f1eed4'
 assert recover_arena_spawn_selection(source,moved,rdata)==source
 class Drifted:
  def bytes_at(self,at,size):
   b=bytearray(rdata.bytes_at(at,size));b[0]^=255;return bytes(b)
 assert recover_arena_spawn_selection(source,fn,Drifted())==source
 class Missing:
  def bytes_at(self,at,size):return None
 assert recover_arena_spawn_selection(source,fn,Missing())==source
 for token in ['x_stk_104 = (int *)(iVar4 + -1);','x_stk_104 = (int *)((int)x_stk_104 + -1);','x_stk_104 = (int *)(iVar6 + -1);','CCharString::CCharString(xStack_a0,(CCharString *)&DAT_0143e8ec);']:
  for changed in [source.replace(token,token.replace('x_stk_104','unknown').replace('xStack_a0','unknown'),1),source+'\n'+token,source.replace(token,'',1)]:
   assert recover_arena_spawn_selection(changed,fn,rdata)==changed

@pytest.fixture(scope="module")
def harness():
 t=ast.parse((ROOT/'tools/script_recovery/test_native_arena_rounds.py').read_text());fn=next(n for n in t.body if isinstance(n,ast.FunctionDef) and n.name=='test_actual_play_wave_reaches_hud_after_selected_group_spawn')
 h=next(n.value for n in ast.walk(fn) if isinstance(n,ast.Constant) and isinstance(n.value,str) and 'events, counters, updates' in n.value)
 h=h.replace("if k=='ArenaSpawn' then return{actor}else return{}end", "if k=='ArenaSpawn' then return spawns else return{}end")
 h=h.replace('function quest:GetFurthestWithScriptName(...)return actor end','function quest:GetFurthestWithScriptName(...)return furthest end')
 h=h.replace("table.insert(events,kind);return actor","table.insert(events,kind);table.insert(positions,pos.x-20);return actor")
 h=h.replace('assert(count==1 and scale==1)','assert(count==expectedCount and scale==1)').replace('assert(id==101 and count==1 and maximum==-1)','assert(id==101 and count==expectedCount and maximum==-1)')
 h='positions={}\n'+h
 return h

@pytest.mark.parametrize('style',[0,1])
@pytest.mark.parametrize('count,round_index,group,short,n,marker',[(count,*row) for count in (1,2,3,5) for row in itertools.product((2,3),range(3),(False,True),(1,2,4,7),tuple(range(count))+('nil','junk'))])
def test_actual_alternating_spawns(generated,harness,style,count,round_index,group,short,n,marker):
 source=generated[style];h=harness
 lua=LuaRuntime();lua.execute(source);lua.execute(h)
 spawns=lua.table()
 for i in range(count):spawns[i+1]=lua.execute('return {GetDataString=function()return "'+str(i)+'" end,GetPos=function()return{x='+str(i+20)+',y=2,z=3}end}')
 lua.globals().spawns=spawns
 if marker=='nil':furthest=None
 else:furthest=lua.execute('return{IsNull=function()return false end,GetDataString=function()return "'+str(marker)+'" end}')
 lua.globals().furthest=furthest;lua.globals().shortWave=short
 expected_count=n;lua.globals().expectedCount=expected_count
 state=lua.globals().state;state.ArenaRound=round_index;state.ArenaRoundWave=4
 prefix=f'Rounds_{round_index}_Waves_4';state[prefix+'_NumWaveCreatures']=3
 for g in range(3):state[f'{prefix}_Creatures_{g}_NumCreatures']=n if g==group else 0
 try:lua.globals().PlayWave(lua.globals().quest)
 except Exception as e:assert 'STOP_AFTER_SPAWNS' in str(e),str(e)
 else:raise AssertionError('did not reach HUD')
 selected=marker if isinstance(marker,int) else 0
 front=selected;back=(selected-1)%count;expected=[]
 for i in range(expected_count):
  if i%2==0:expected.append(front);front=(front+1)%count
  else:expected.append(back);back=(back-1)%count
 actual=list(lua.globals().positions.values());assert actual==expected,(actual,expected)
