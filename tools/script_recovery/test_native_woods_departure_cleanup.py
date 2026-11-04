"""Actual registered WoodsDeparture main survives delayed level loading."""
import itertools,json,os,sys
from pathlib import Path
from unittest.mock import patch
import pytest
from lupa.lua54 import LuaRuntime
ROOT=Path(__file__).resolve().parents[2];sys.path.insert(0,str(ROOT))
from tools.script_recovery.convert_quest_unit import UnitConverter
from tools.script_recovery.build_readable_unit import readable_file
@pytest.fixture(scope='module')
def generated(tmp_path_factory):
 c=UnitConverter(ROOT/'refs/script_recovery/guild_training/translation_unit_typed.json')
 unit=json.loads((ROOT/'refs/script_recovery/guild_training/units/Q_GuildTrainingWoodsDeparture.json').read_text());unit['entities']={}
 out=tmp_path_factory.mktemp('woods_departure_cleanup')
 with patch.object(c,'constructor_defaults',return_value={}):c.convert(unit,out)
 source=(out/'FSE/GuildTrainingWoodsDeparture/GuildTrainingWoodsDeparture.lua').read_text()
 return source,readable_file(source)[0]
H=r'''
return function(source,loadDelay,deathAt,overAt,successAt,lowHealth,limit,unload)
 local frame,events,jobs,state=0,{},{},{};local stopped=limit;local health=lowHealth and 3 or 50
 local q,hero={},{}
 local function event(...)events[#events+1]={...}end
 local function term()return frame>=stopped end
 local env=setmetatable({},{__index=_G});assert(load(source,'quest','t',env))()
 local function spawn(name)assert(env[name]);jobs[#jobs+1]={name=name,co=coroutine.create(function()env[name](q)end)};event('thread',name,frame)end
 function q:GetHero()return hero end
 function hero:IsAlive()return deathAt==0 or frame<deathAt end
 function q:IsActiveThreadTerminating()return term()end
 function q:NewScriptFrame()coroutine.yield('frame');return not term()end
 function q:IsLevelLoaded(name)assert(name=='GuildWoods');return frame>=loadDelay and not (unload and frame>=9 and frame<11)end
 function q:SetStateBool(name,v)assert(name=='MissionSucceeded' or name=='MissionFailed' or name=='MissionOver');assert(type(v)=='boolean');state[name]=v;event('state',name,v,frame)end
 function q:GetStateBool(name)
  assert(state[name]~=nil,name)
  if name=='MissionOver' and overAt~=0 and frame>=overAt then return true end
  return state[name]
 end
 function q:SetStateInt(name,v)assert(name=='DepartureMissionPoint' and v==0);event('point',frame)end
 function q:AddEntityBinding(name,path,kind)assert(path=='GuildTrainingWoodsDeparture/Entities/'..name and kind==1);event('binding',name,frame)end
 function q:FinalizeEntityBindings()event('finalize',frame)end
 function q:CreateThread(name)spawn(name)end
 function q:SetQuestCardObjective(name,key,a,b)assert(name=='Q_GuildTraining' and key=='TEXT_QUEST_GUILD_TRAINING_OBJECTIVE_12' and a=='' and b=='');event('objective',frame)end
 function q:GetThingWithScriptName(name)assert(name=='MazeCreationMarker' or name=='GuildWoodsTeleportExitHSP');local thing={name=name};function thing:GetPos()return {x=1,y=2,z=3}end;return thing end
 function q:CreateCreature(key,pos,name)assert(key=='CREATURE_RIVAL_HERO_MAZE_TUTORIAL' and pos.x==1 and pos.y==2 and pos.z==3 and name=='FinalMaze');event('createMaze',frame)end
 function q:GiveHeroNewQuestObjective(text,kind)assert(text=='first objective' and kind==1);event('heroObjective',frame)end
 function q:GetActiveQuestName()return 'Q_GuildTrainingWoodsDeparture'end
 function q:SetExperienceSpendingAsEnabled(v)assert(v);event('experience',frame)end
 function q:SetQuestAsCompleted(name,a,b,c)assert(name=='Q_GuildTrainingWoodsDeparture' and type(a)=='boolean' and b and not c);event('completed',a,frame)end
 function q:SetQuestAsFailed(name,a,key,b)assert(name=='Q_GuildTrainingWoodsDeparture' and a and key=='' and b);event('failed',frame)end
 function q:DeactivateQuestLater(name,delay)assert(name=='Q_GuildTrainingWoodsDeparture' and delay==0);event('deactivate',frame);stopped=math.min(stopped,frame+1)end
 function q:GetHealth(actor)assert(actor==hero);return health end
 function q:EntityTeleportToThing(actor,marker,flag)assert(actor==hero and marker.name=='GuildWoodsTeleportExitHSP' and not flag);event('teleport',frame)end
 function q:Pause(value)assert(value==2);local finish=frame+2;while frame<finish and not term()do q:NewScriptFrame()end end
 function q:AddNewConversation(actor,a,b)assert(actor==hero and not a and not b);return 101 end
 function q:AddLineToConversation(id,key,a,b,c)assert(id==101 and key=='TEXT_QST_028_GUILDMASTER_WOODS_DEPARTURE_TELEPORT_OUT_FIRST' and a==hero and b==hero and not c);event('line',frame)end
 function q:ChangeHeroHealthBy(value,a,b)assert(value==1000 and a and not b);health=50;event('heal',frame)end
 spawn('Main')
 while true do
  if successAt~=0 and frame>=successAt and state.MissionSucceeded~=nil and not state.MissionSucceeded then state.MissionSucceeded=true;event('externalSuccess',frame)end
  local active=false;local count=#jobs
  for i=1,count do local job=jobs[i]
   if coroutine.status(job.co)~='dead' then
    active=true;debug.sethook(job.co,function()error('instruction budget '..job.name)end,'',50000)
    local ok,message=coroutine.resume(job.co);assert(ok,job.name..': '..tostring(message))
   end
  end
  if not active then break end
  frame=frame+1;assert(frame<=limit+3,'failed termination')
 end
 return events
end
'''
def plain(v):
 if hasattr(v,'items'):return [plain(x) for x in v.values()]
 return v
@pytest.mark.parametrize('style',(0,1))
@pytest.mark.parametrize('args',list(itertools.product((0,3),(0,5),(0,8,20),(0,12),(False,True),(4,15,30),(False,True))))
def test_actual_delayed_departure_lifecycle(generated,style,args):
 events=plain(LuaRuntime().execute(H)(generated[style],*args))
 assert sum(e[0]=='createMaze' for e in events)==1
 assert [e[1] for e in events if e[0]=='binding']==['ArtifactThief','FinalMaze','ScorpionHome']
 assert sum(e[0]=='deactivate' for e in events)<=1
