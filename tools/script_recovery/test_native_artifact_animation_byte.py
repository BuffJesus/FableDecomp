"""Actual ArtifactThief wave preserves the writable native animation byte."""
import copy,itertools,json,os,sys
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
 out=tmp_path_factory.mktemp('artifact_animation_byte')
 with patch.object(c,'constructor_defaults',return_value={}):c.convert(unit,out)
 source=(out/'FSE/GuildTrainingWoodsDeparture/Entities/ArtifactThief.lua').read_text()
 return source,readable_file(source)[0]
H="\nreturn function(hitKind,hitAt,speechTicks,limit,health,talkAt,answer,gold,answerDelay,near,raw,acquireDelay)\n local frame,events=0,{};local resources,movies,timers={},{},{};local nextMovie=0;local q,me,hero,r={},{},{},{}\n local function event(...)events[#events+1]={...}end\n local function term()return frame>=limit end\n function q:GetHero()return hero end\n function q:RetailResources()return r end\n function q:NewScriptFrame(actor)assert(actor==me);frame=frame+1;event('frame',frame);return not term()end\n function q:IsActiveThreadTerminating()return term()end\n function q:EntitySetAsKillable(actor,a,b)assert(actor==me and b);event('killable',a,frame)end\n function q:SetThingHasInformation(actor,a,b,c)assert(actor==me and not a and b and not c)end\n function q:AddItemToContainer(actor,key)assert(actor==me and key=='OBJECT_HAND_LAMP')end\n function q:RegisterTimer()timers[701]=0;return 701 end\n function q:SetTimer(id,value)assert(timers[id]~=nil);timers[id]=frame+value end\n function q:GetTimer(id)assert(timers[id]~=nil);return timers[id]-frame end\n function q:DeregisterTimer(id)assert(timers[id]~=nil);timers[id]=nil;event('deregister',frame)end\n function q:IsDistanceBetweenThingsUnder(a,b,distance)assert(a==hero and b==me and distance==5.5);return near end\n function me:MsgIsHitByHero()return frame>=hitAt and hitKind=='melee'end\n function me:MsgIsHitByAnySpecialAbilityFromHero()return frame>=hitAt and (hitKind=='ability' or hitKind=='heal')end\n function me:MsgIsHitByHeroSpecialAbility(kind)assert(kind==14);return hitKind=='heal'end\n function me:IsTalkedToByHero()\n  return talkAt~=0 and (frame==talkAt or frame==talkAt+8) and (hitKind=='none' or hitKind=='heal' or frame<hitAt)\n end\n local taskUntil=0\n function me:IsPerformingScriptTask()return frame<taskUntil end\n function me:Speak(target,key,selection,a,b,c)\n  assert(target==hero and selection==0 and not a and b and not c and key:match('^TEXT_QST_028_MAZE_WOODS_ARTIFACT_THIEF_'));event('speak',key,frame)\n  local finish=frame+speechTicks;while frame<finish and not term()do q:NewScriptFrame(me)end;return not term()\n end\n function q:PauseAllNonScriptedEntities(v)event('pause',v,frame)end\n function q:GetHealth(actor)assert(actor==me);return health end\n function q:GiveHeroObject(key,count,flag)assert(key=='OBJECT_HAND_LAMP' and count==-1 and not flag);event('lamp',frame)end\n function q:RemoveItemFromContainer(actor,key)assert(actor==me and key=='OBJECT_HAND_LAMP');event('removeLamp',frame)end\n function q:GetThingWithScriptName(name)assert(name=='ArtifactThiefRunMarker');local thing={};function thing:GetPos()return {x=1,y=2,z=3}end;return thing end\n function me:MoveToPosition(pos,rate,kind,a,b)assert(pos.x==1 and rate==1 and (kind==0 or kind==1) and not a and b);event('run',frame);taskUntil=frame+2 end\n local questionAt\n function q:GiveHeroYesNoQuestion(key,yes,no,empty,flag)\n  assert(key=='TEXT_QST_028_MAZE_WOODS_ARTIFACT_THIEF_QUESTION' and yes=='TEXT_OBJECT_HERO_ANSWER_YES' and no=='TEXT_OBJECT_HERO_ANSWER_NO' and empty=='' and flag);questionAt=frame;event('question',frame)\n end\n function q:MsgIsQuestionAnsweredYesOrNo()assert(questionAt);return frame<questionAt+answerDelay and -1 or answer end\n function q:GetHeroGold()return gold end\n function q:ReadGlobalGameDataFloat(offset)assert(offset==3860);return 50 end\n function q:GiveHeroGold(value)assert(value==-50);gold=gold+value;event('pay',value,frame)end\n function q:EntityGiveGold(actor,value)assert(actor==me and value==50);event('receive',value,frame)end\n function q:AddNewConversation(actor,a,b)assert(actor==me and not a and not b);return 901 end\n function q:AddPersonToConversation(id,actor)assert(id==901 and actor==hero)end\n function q:AddLineToConversation(id,key,actor,target,flag)assert(id==901 and actor==me and target==hero and not flag);event('line',key,frame)end\n function q:EntitySetFacingAngleTowardsThing(actor,target,flag)assert(actor==me and target==hero and not flag)end\n local function byte()return frame<5 and raw or (raw+128)%256 end\n function me:PlayAnimation(key,a,b,c,d,e,f,g)assert(key=='ST_WAVE_SPECIAL_02' and not a and not b and not c and d and e and not f and not g);event('animation',e and 1 or 0,byte(),frame);event('wave',frame)end\n function q:FadeOutAndKillEntity(actor,a,seconds,b)assert(actor==me and a and seconds==1 and b);event('fade',frame)end\n function r:NewResource()assert(next(resources)==nil);resources[117]=true;event('resource',117,frame);return 117 end\n function r:PrepareResource(id)assert(resources[id])end\n function r:TryAcquire(id,actor,kind)assert(resources[id] and actor==me and kind==4);return frame>=acquireDelay end\n function r:ReleaseResource(id)assert(resources[id]);resources[id]=nil;event('release',id,frame)end\n function r:ScriptThing(id)assert(resources[id]);return me end\n function r:StartMovie(name)assert(name=='');nextMovie=nextMovie+1;movies[nextMovie]=true;event('movie',nextMovie,frame);return nextMovie end\n function r:PlayAnimationWithNativeArgument5(id,key,a,b,c,d,f,g)\n  assert(resources[id] and id==117 and key=='ST_WAVE_SPECIAL_02' and not a and not b and not c and d and not f and not g)\n  event('animation',byte(),byte(),frame);event('wave',frame)\n end\n function r:DestroyMovie(id)assert(movies[id]);movies[id]=nil;event('destroyMovie',id,frame)end\n debug.sethook(function()error('instruction budget')end,'',50000);Init(q,me);Main(q,me);debug.sethook()\n assert(next(resources)==nil and next(timers)==nil,'resource/timer leak')\n return events,next(movies)==nil\nend\n"
def plain(v):
 if hasattr(v,'items'):return [plain(x) for x in v.values()]
 return v
@pytest.mark.parametrize('style',(0,1))
@pytest.mark.parametrize('raw,limit,near,acquire',list(itertools.product((0,1,2,128,255),(2,5,18),(False,True),(0,2))))
def test_actual_wave_animation_byte(generated,style,raw,limit,near,acquire):
 lua=LuaRuntime();lua.execute(generated[style]);events,clean=lua.execute(H)('none',30,0,limit,100,0,0,0,0,near,raw,acquire);events=plain(events)
 assert clean
 assert all(e[1]==e[2] for e in events if e[0]=='animation')
 if not near:assert not any(e[0]=='animation' for e in events)

@pytest.mark.parametrize('mode',('missing_call_order','changed_helper_bytes'))
def test_actual_converter_declines_unproven_animation_recovery(tmp_path,mode):
 c=UnitConverter(ROOT/'refs/script_recovery/guild_training/translation_unit_typed.json')
 unit=json.loads((ROOT/'refs/script_recovery/guild_training/units/Q_GuildTrainingWoodsDeparture.json').read_text());unit['entities']={'ArtifactThief':unit['entities']['ArtifactThief']};unit['quest']['functions']={};unit['quest']['helpers']={}
 # Isolate the animation guard: deleting callOrder also invalidates unrelated
 # reviewed movie source pairing, which the normal registered unit rejects.
 unit.get('reviewedMovieMembers',{}).pop('0x00d62480',None)
 if mode=='missing_call_order':
  original=c.native
  def native(address):
   fn=original(address)
   if fn and fn['address'].lower()=='0x00d62480':fn=copy.deepcopy(fn);fn['callOrder']=[]
   return fn
  c.native=native
 else:
  original=c.rdata
  class Changed:
   def __getattr__(self,name):return getattr(original,name)
   def bytes_at(self,address,size):return b'\x90'*size if address==0x7e73d0 else original.bytes_at(address,size)
  c.rdata=Changed()
 with patch.object(c,'constructor_defaults',return_value={}):c.convert(unit,tmp_path)
 source=(tmp_path/'FSE/GuildTrainingWoodsDeparture/Entities/ArtifactThief.lua').read_text()
 assert 'PlayAnimationWithNativeArgument5' not in source
 assert 'me:PlayAnimation("ST_WAVE_SPECIAL_02"' in source
