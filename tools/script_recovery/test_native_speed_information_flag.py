"""Native ApprenticeSpeedTest information byte must survive frames and race start."""
import itertools,json
from pathlib import Path
from unittest.mock import patch
import pytest
from lupa.lua54 import LuaRuntime
from tools.script_recovery.convert_quest_unit import UnitConverter
from tools.script_recovery.build_readable_unit import readable_file
ROOT=Path(__file__).resolve().parents[2]
@pytest.fixture(scope='module')
def generated(tmp_path_factory):
 c=UnitConverter(ROOT/'refs/script_recovery/guild_training/translation_unit_typed.json')
 unit=json.loads((ROOT/'refs/script_recovery/guild_training/units/Q_GuildTraining.json').read_text())
 unit['entities']={'ApprenticeSpeedTest':unit['entities']['ApprenticeSpeedTest']}
 unit['quest']['functions']={};unit['quest']['helpers']={}
 out=tmp_path_factory.mktemp('speed_information')
 with patch.object(c,'constructor_defaults',return_value={}):c.convert(unit,out)
 source=(out/'FSE/GuildTraining/Entities/ApprenticeSpeedTest.lua').read_text()
 return source,readable_file(source)[0]
def plain(v):
 if hasattr(v,'items'):return [plain(x) for x in v.values()]
 return v
IDLE='\nreturn function(limit,stateAt,delay)\n local frames,events,timers=0,{},0\n local function event(...)events[#events+1]={...}end\n local me={IsTalkedToByHero=function()return false end,SetFriendsWithEverythingFlag=function()end}\n local friend={IsNull=function()return false end,SetFriendsWithEverythingFlag=function()end}\n local hero,village={},{}\n local q={}\n function q:GetHero()return hero end\n function q:NewScriptFrame(actor)assert(actor==me);frames=frames+1;event("frame",frames);return frames<limit end\n function q:IsActiveThreadTerminating()return frames>=limit end\n function q:GetThingWithScriptName(name)assert(name=="SpeedFriend");return friend end\n function q:GetNearestWithDefName(actor,name)assert(actor==me and name=="VILLAGE_GUILD_COMPLEX_INSIDE");return village end\n function q:EntityAttachToVillage(actor,v)assert((actor==me or actor==friend) and v==village)end\n function q:SetIsPushableByHero(actor,value)assert(actor==me and not value)end\n function q:EntitySetAsKillable(actor,a,b)assert((actor==me or actor==friend) and not a and b)end\n function q:SetThingHasInformation(actor,a,b,c)assert(actor==me and not a and b and not c);event("info",frames)end\n function q:RegisterTimer()timers=timers+1;event("timer",timers);return timers end\n function q:DeregisterTimer(id)event("deregister",id)end\n function q:SetTimer(id,time)assert(id==2 and time==1)end\n function q:GetStateInt(name)assert(name=="GameState");return frames>=stateAt and 3 or 2 end\n function q:IsDistanceBetweenThingsUnder(a,b,d)assert(a==hero and b==me and d==10);return false end\n local r={}\n function q:RetailResources()return r end\n function r:NewResource()return 20 end\n function r:PrepareResource(id)assert(id==20)end\n function r:TryAcquire(id,actor,kind)assert(id==20 and actor==me and kind==4);return frames>=delay end\n function r:ReleaseResource(id)assert(id==20);event("release")end\n debug.sethook(function()error("instruction budget")end,"",20000)\n Main(q,me)\n debug.sethook()\n return events\nend\n'
RACE='\nreturn function(limit,stateAt,delay)\n local frames,events,timers=0,{},0\n local finished=false\n local function event(...)events[#events+1]={...}end\n local me={IsTalkedToByHero=function()return frames>=2 end,IsPerformingScriptTask=function()return false end,SetFriendsWithEverythingFlag=function()end}\n local friend={IsNull=function()return false end,SetFriendsWithEverythingFlag=function()end}\n local hero,village={},{}\n local q={}\n function q:GetHero()return hero end\n function q:NewScriptFrame(actor)assert(actor==me);frames=frames+1;event("frame",frames);return frames<limit end\n function q:IsActiveThreadTerminating()return finished or frames>=limit end\n function q:GetThingWithScriptName(name)if name=="RaceMarker" then return village end;assert(name=="SpeedFriend");return friend end\n function q:GetNearestWithDefName(actor,name)assert(actor==me and name=="VILLAGE_GUILD_COMPLEX_INSIDE");return village end\n function q:EntityAttachToVillage(actor,v)assert((actor==me or actor==friend) and v==village)end\n function q:SetIsPushableByHero(actor,value)assert(actor==me and not value)end\n function q:EntitySetAsKillable(actor,a,b)assert((actor==me or actor==friend) and not a and b)end\n function q:SetThingHasInformation(actor,a,b,c)assert(actor==me and not a and b and not c);event("info",frames)end\n function q:RegisterTimer()timers=timers+1;event("timer",timers);return timers end\n function q:DeregisterTimer(id)event("deregister",id)end\n function q:SetTimer(id,time)assert(id>=1 and id<=timers);event("setTimer",id,time)end\n function q:GetStateInt(name)assert(name=="GameState");return frames>=stateAt and 3 or 2 end\n function q:IsDistanceBetweenThingsUnder(a,b,d)assert(a==hero and b==me and d==10);return false end\n local r={}\n function q:RetailResources()return r end\n function r:NewResource()return 20 end\n function r:PrepareResource(id)assert(id==20)end\n function r:TryAcquire(id,actor,kind)assert(id==20 and actor==me and kind==4);return frames>=delay end\n function r:ReleaseResource(id)assert(id==20);event("release")end\n\n function q:GetHealth(actor)assert(actor==me);return 6 end\n function me:Speak(target,key,a,b,c,d)assert(target==hero and a==0 and not b and c and not d);event("speak",key);return true end\n function q:PauseAllNonScriptedEntities(a)event("pause",a)end\n function q:GiveHeroYesNoQuestion(...)event("question")end\n function q:MsgIsQuestionAnsweredYesOrNo()return 1 end\n function q:SetStateBool(name,value)assert(name=="ReachedPlatform" and not value)end\n function q:MiniMapAddMarker(actor,name)assert(actor==village and name=="HUD_ORB_QUEST_VIGNETTE")end\n function q:ReadGlobalGameDataFloat(offset)assert(offset==0xedc);return 5 end\n function q:AddQuestInfoTimer(id,icon,scale)assert(id==1 and icon=="HUD_CLOCK_ICON" and scale==1);return 101 end\n function q:DisplayQuestInfo(flag)assert(flag);event("display")end\n function r:StartMovie(name)assert(name=="");event("movie");return 30 end\n function r:DestroyMovie(id)assert(id==30);event("destroy");finished=true end\n function r:ScriptThing(id)assert(id==20);return me end\n\n debug.sethook(function()error("instruction budget")end,"",20000)\n Main(q,me)\n debug.sethook()\n \nreturn events\nend\n'

@pytest.mark.parametrize('style',(0,1))
@pytest.mark.parametrize('limit,state_at,delay',list(itertools.product((1,3,6,10),(0,3,7,20),(0,2,5))))
def test_information_updated_once_across_frames(generated,style,limit,state_at,delay):
 lua=LuaRuntime();lua.execute(generated[style]);events=plain(lua.execute(IDLE)(limit,state_at,delay))
 initial=max(1,delay)
 expected=[] if initial>=limit else [initial]+([max(initial+1,state_at)] if max(initial+1,state_at)<limit else [])
 assert [e[1] for e in events if e[0]=='info']==expected
 assert sum(e[0]=='release' for e in events)==int(limit>1)
@pytest.mark.parametrize('style',(0,1))
@pytest.mark.parametrize('limit,state_at,delay',list(itertools.product((3,6,10),(0,20),(0,2,5))))
def test_race_start_shares_information_byte(generated,style,limit,state_at,delay):
 lua=LuaRuntime();lua.execute(generated[style]);events=plain(lua.execute(RACE)(limit,state_at,delay))
 initial=max(1,delay);entered=any(e[0]=='display' for e in events)
 expected=0 if initial>=limit else 1+int(entered or max(initial+1,state_at)<limit)
 assert sum(e[0]=='info' for e in events)==expected
 assert sum(e[0]=='release' for e in events)==1
 assert sum(e[0]=='movie' for e in events)==sum(e[0]=='destroy' for e in events)
