"""Actual ArtifactThief attack paths release their movie handle once."""
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
 unit=json.loads((ROOT/'refs/script_recovery/guild_training/units/Q_GuildTrainingWoodsDeparture.json').read_text());unit['entities']={'ArtifactThief':unit['entities']['ArtifactThief']};unit['quest']['functions']={};unit['quest']['helpers']={}
 out=tmp_path_factory.mktemp('artifact_movie_cleanup')
 with patch.object(c,'constructor_defaults',return_value={}):c.convert(unit,out)
 source=(out/'FSE/GuildTrainingWoodsDeparture/Entities/ArtifactThief.lua').read_text()
 return source,readable_file(source)[0]
H=r'''
return function(hitKind,hitAt,speechTicks,limit,health)
 local frame,events=0,{};local resources,movies,timers={},{},{};local nextMovie=0;local q,me,hero,r={},{},{},{}
 local function event(...)events[#events+1]={...}end
 local function term()return frame>=limit end
 function q:GetHero()return hero end
 function q:RetailResources()return r end
 function q:NewScriptFrame(actor)assert(actor==me);frame=frame+1;event('frame',frame);return not term()end
 function q:IsActiveThreadTerminating()return term()end
 function q:EntitySetAsKillable(actor,a,b)assert(actor==me and b);event('killable',a,frame)end
 function q:SetThingHasInformation(actor,a,b,c)assert(actor==me and not a and b and not c)end
 function q:AddItemToContainer(actor,key)assert(actor==me and key=='OBJECT_HAND_LAMP')end
 function q:RegisterTimer()timers[701]=0;return 701 end
 function q:SetTimer(id,value)assert(timers[id]~=nil);timers[id]=frame+value end
 function q:GetTimer(id)assert(timers[id]~=nil);return timers[id]-frame end
 function q:DeregisterTimer(id)assert(timers[id]~=nil);timers[id]=nil;event('deregister',frame)end
 function q:IsDistanceBetweenThingsUnder(a,b,distance)assert(a==hero and b==me and distance==5.5);return false end
 function me:MsgIsHitByHero()return frame>=hitAt and hitKind=='melee'end
 function me:MsgIsHitByAnySpecialAbilityFromHero()return frame>=hitAt and (hitKind=='ability' or hitKind=='heal')end
 function me:MsgIsHitByHeroSpecialAbility(kind)assert(kind==14);return hitKind=='heal'end
 function me:IsTalkedToByHero()return false end
 local taskUntil=0
 function me:IsPerformingScriptTask()return frame<taskUntil end
 function me:Speak(target,key,selection,a,b,c)
  assert(target==hero and selection==0 and not a and b and not c and key=='TEXT_QST_028_MAZE_WOODS_ARTIFACT_THIEF_PRE_HIT');event('speak',frame)
  local finish=frame+speechTicks;while frame<finish and not term()do q:NewScriptFrame(me)end;return not term()
 end
 function q:PauseAllNonScriptedEntities(v)event('pause',v,frame)end
 function q:GetHealth(actor)assert(actor==me);return health end
 function q:GiveHeroObject(key,count,flag)assert(key=='OBJECT_HAND_LAMP' and count==-1 and not flag);event('lamp',frame)end
 function q:RemoveItemFromContainer(actor,key)assert(actor==me and key=='OBJECT_HAND_LAMP');event('removeLamp',frame)end
 function q:GetThingWithScriptName(name)assert(name=='ArtifactThiefRunMarker');local thing={};function thing:GetPos()return {x=1,y=2,z=3}end;return thing end
 function me:MoveToPosition(pos,rate,kind,a,b)assert(pos.x==1 and rate==1 and kind==1 and not a and b);event('run',frame);taskUntil=frame+2 end
 function q:FadeOutAndKillEntity(actor,a,seconds,b)assert(actor==me and a and seconds==1 and b);event('fade',frame)end
 function r:NewResource()assert(next(resources)==nil);resources[117]=true;event('resource',117,frame);return 117 end
 function r:PrepareResource(id)assert(resources[id])end
 function r:TryAcquire(id,actor,kind)assert(resources[id] and actor==me and kind==4);return true end
 function r:ReleaseResource(id)assert(resources[id]);resources[id]=nil;event('release',id,frame)end
 function r:ScriptThing(id)assert(resources[id]);return me end
 function r:StartMovie(name)assert(name=='');nextMovie=nextMovie+1;movies[nextMovie]=true;event('movie',nextMovie,frame);return nextMovie end
 function r:DestroyMovie(id)assert(movies[id]);movies[id]=nil;event('destroyMovie',id,frame)end
 debug.sethook(function()error('instruction budget')end,'',50000);Init(q,me);Main(q,me);debug.sethook()
 assert(next(resources)==nil and next(timers)==nil,'resource/timer leak')
 return events,next(movies)==nil
end
'''
@pytest.mark.parametrize('style',(0,1))
@pytest.mark.parametrize('args',list(itertools.product(('melee','ability','heal','none'),(1,3),(0,2),(2,5,12),(0,100))))
def test_actual_attack_movie_lifetime(generated,style,args):
 lua=LuaRuntime();lua.execute(generated[style]);events,movies_clean=lua.execute(H)(*args)
 assert movies_clean,'retained native movie handle'
