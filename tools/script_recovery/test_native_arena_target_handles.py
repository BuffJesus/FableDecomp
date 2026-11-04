"""Preserve distinct addressed spawn handles in actual Arena PlayWave calls."""
import ast,itertools,json
from pathlib import Path
from unittest.mock import patch
import pytest
from lupa.lua54 import LuaRuntime
from tools.script_recovery.convert_quest_unit import UnitConverter
from tools.script_recovery.build_readable_unit import readable_file
ROOT=Path(__file__).resolve().parents[2]
@pytest.fixture(scope='module')
def generated(tmp_path_factory):
 c=UnitConverter(ROOT/'refs/script_recovery/arena/translation_unit_typed.json');unit=json.loads((ROOT/'refs/script_recovery/arena/units/Q_Arena.json').read_text());out=tmp_path_factory.mktemp('arena_targets')
 with patch.object(c,'constructor_defaults',return_value={}):c.convert(unit,out)
 source=(out/'FSE/Arena/Arena.lua').read_text();return source,readable_file(source)[0]
@pytest.fixture(scope='module')
def harness():
 t=ast.parse((ROOT/'tools/script_recovery/test_native_arena_rounds.py').read_text());fn=next(n for n in t.body if isinstance(n,ast.FunctionDef) and n.name=='test_actual_play_wave_reaches_hud_after_selected_group_spawn')
 h=next(n.value for n in ast.walk(fn) if isinstance(n,ast.Constant) and isinstance(n.value,str) and 'events, counters, updates' in n.value)
 h=h.replace("if k=='ArenaSpawn' then return{actor}else return{}end", "if k=='ArenaSpawn' then return spawns else return{}end")
 h=h.replace('function quest:GetFurthestWithScriptName(...)return actor end','function quest:GetFurthestWithScriptName(...)return furthest end')
 h=h.replace("table.insert(events,kind);return actor","table.insert(events,kind);table.insert(positions,pos.x-20);return actor")
 h=h.replace('assert(count==1 and scale==1)','assert(count==expectedCount and scale==1)').replace('assert(id==101 and count==1 and maximum==-1)','assert(id==101 and count==expectedCount and maximum==-1)')
 h=h.replace("function quest:GetHero()return actor end", "function quest:GetHero()return heroThing end")
 h=h.replace("function quest:GetThingWithScriptName(k)return actor end", "function quest:GetThingWithScriptName(name)if name=='WhisperAlly' then return whisperThing end;return actor end")
 h+='\nfunction quest:RetailRandModulo(n)assert(n==32768);rngCalls=rngCalls+1;return rngBit end\n'
 h=h.replace('who==actor','who==heroThing').replace('a==actor and b==actor','a==heroThing and b==heroThing')
 h=h.replace("return actor\nend", "local creature={id=#events};created[#created+1]=creature;return creature\nend")
 h=h.replace("assert(thing==actor and mode==1)","assert(created[#created]==thing and mode==1)")
 h=h.replace("assert(thing==actor and target==actor)","assert(thing==created[#created], 'wrong enemy receiver');assert(target==(allyAlive and rngBit==0 and whisperThing or heroThing), 'wrong enemy target');targetTrace[#targetTrace+1]=target==heroThing and 'hero' or 'whisper'")
 h='rngCalls=0 positions={} created={} targetTrace={} heroThing={id="hero"} whisperThing={id="whisper",IsNull=function()return false end,IsAlive=function()return allyAlive end}\n'+h
 return h
@pytest.mark.parametrize('style',[0,1])
@pytest.mark.parametrize('count,round_index,short,n,marker,ally_alive,rng_bit',[(count,*row) for count in (1,3) for row in itertools.product((2,3),(False,True),(1,2,4,7),tuple(range(count)),(False,True),(0,1))])
def test_distinct_enemy_and_target_handles(generated,harness,style,count,round_index,short,n,marker,ally_alive,rng_bit):
 source=generated[style];h=harness;group=0
 lua=LuaRuntime();lua.execute(source);lua.execute(h)
 spawns=lua.table()
 for i in range(count):spawns[i+1]=lua.execute('return {GetDataString=function()return "'+str(i)+'" end,GetPos=function()return{x='+str(i+20)+',y=2,z=3}end}')
 lua.globals().spawns=spawns
 if marker=='nil':furthest=None
 else:furthest=lua.execute('return{IsNull=function()return false end,GetDataString=function()return "'+str(marker)+'" end}')
 lua.globals().furthest=furthest;lua.globals().shortWave=short;lua.globals().allyAlive=ally_alive;lua.globals().rngBit=rng_bit
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
 assert lua.globals().rngCalls==(n if ally_alive else 0)
