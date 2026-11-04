import itertools,sys
from pathlib import Path
import pytest
from lupa.lua54 import LuaRuntime
ROOT=next(p for p in Path(__file__).resolve().parents if (p/'tools/script_recovery/script_units.py').exists());sys.path.insert(0,str(ROOT))
@pytest.fixture(scope='module')
def sources():
 return {s:tuple((ROOT/'refs/script_recovery/lifted/SickChild'/s/'FSE/V_SickChild'/f).read_text() for f in ('Entities/Witch.lua','native_quest_helpers.lua')) for s in ('draft','readable')}
H=r'''return function(initial,inventory,beforeCount,afterCount,overfill,beforeHealth,afterHealth,wait,termination,seed,cancelAt,argument)
 local q,me,hero,r,witch,mother={},{},{},{},{},{};local trace={};local paused=false;local camera=false;local inspect=false;local saved={};local boundary={}
 local yes,intro,health,speak=true,true,true,true;local beforeHits,afterHits,totalHits=0,0,0;local scene=nil;local currentHandle=nil;local delivered=0;local finished=false;local frames,reads,questions,answers,tasks,speeches,attempts,created,destroyed,takes,cleared,potions,intros,objectives=0,0,0,0,0,0,0,0,0,0,0,0,0,0
 local leases,strings,actors=0,0,0;local canceled=false;local introWritten=false;local currentDefinition=nil;local available=inventory
 local function log(value)trace[#trace+1]=value end
 local function terminating()return finished or termination>0 and frames>=termination end
 function q:RetailResources()return r end
 function r:IsHitByHeroExceptAbility(actor,ability)
     assert(actor==me and ability==14)
     if actor:MsgIsHitByHero() then return true end
     if not actor:MsgIsHitByAnySpecialAbilityFromHero() then return false end
     return not actor:MsgIsHitByHeroSpecialAbility(ability)
 end
 function q:GetStateBool(name)assert(name=='MotherIntroDone');return true end
 function q:PersistTransferBool(ctx,name,value)assert(name=='IntroDone');if not inspect then return intro end;saved.IntroDone=value;return value end
 function q:PersistTransferInt(ctx,name,value)assert(name=='ThingsForPotionGot');if not inspect then return initial end;saved.ThingsForPotionGot=value;return value end
 function q:SetIsPushableByHero(actor,value)assert(actor==me and not value)end
 function q:SetThingHasInformation(actor,a,b,c)assert(actor==me and not a and b and not c)end
 function q:EntitySetAsKillable(actor,a,b)assert(actor==me and not a and not b)end
 function q:EntitySetAsToAddToComboMultiplierWhenHit(actor,value)assert(actor==me and not value)end
 function q:NewScriptFrame(actor)assert(actor==me);frames=frames+1;assert(frames<60);return not terminating()end
 function q:IsActiveThreadTerminating()return terminating()end
 function q:GetHero()return hero end
 function q:ReadGlobalGameDataString(offset)
  assert(offset==0x738 or offset==0x744)
  if offset==0x744 then return argument end
  reads=reads+1;currentDefinition='MUSHROOM_DEFINITION_'..reads;return currentDefinition
 end
 function q:IsObjectInThingsPossession(value,actor)assert(actor==hero and value==currentDefinition);return available>0 end
 function q:TakeObjectFromHero(value)assert(value==currentDefinition and available>0);available=available-1;takes=takes+1;log('take:'..value)end
 function q:GiveHeroYesNoQuestion(text,a,b,c,d)assert(text=='TEXT_QST_B10_QUESTION_GIVE_MUSHROOM' and a=='TEXT_OBJECT_HERO_ANSWER_YES' and b=='TEXT_OBJECT_HERO_ANSWER_NO' and c=='' and d==true);questions=questions+1;assert(questions<=2);answers=0;log('question')end
 function q:MsgIsQuestionAnsweredYesOrNo()answers=answers+1;if answers<=wait then return -1 end;return yes and 1 or 0 end
 function q:GetHealth(actor)assert(actor==me);return (scene=='before' and beforeHealth or scene=='after' and afterHealth or scene=='talk') and 10 or 0 end
 function q:GetActiveQuestName()return 'V_SickChild' end
 function q:SetQuestCardObjective(quest,text,a,b)
  assert(quest=='V_SickChild' and a=='' and b=='')
  if text=='TEXT_QUEST_SICK_CHILD_OBJECTIVE_02' then assert(intros==1);introWritten=true
  else assert(text=='TEXT_QUEST_SICK_CHILD_OBJECTIVE_03' and potions==1 and initial+takes>=4 and available==0);objectives=objectives+1 end
  log(text)
 end
 function q:ClearThingHasInformation(actor)assert(actor==me and objectives==1);cleared=cleared+1;log('clear')end
 function q:PauseAllNonScriptedEntities(value)assert(value~=paused);paused=value;log('pause:'..tostring(value))end
 function q:FixMovieSequenceCamera(value)assert(value~=camera and paused);camera=value;log('camera:'..tostring(value))end
 function me:IsTalkedToByHero()if potions==0 and available>0 then scene='talk';return true end;return false end
 function me:MsgIsHitByHero()scene=potions==0 and 'before' or 'after';return true end
 function me:MsgIsHitByAnySpecialAbilityFromHero()return false end
 function me:Speak(actor,text,group,a,b,c)
  local expected
  if scene=='talk' then expected='TEXT_QST_B10_WITCH_NEED_MORE_CURE_10'
  else local suffixes={'10','20','30'};expected='TEXT_QST_B10_WITCH_ONHIT_'..suffixes[((totalHits-1)%3)+1]end
  assert(actor==hero and text==expected and group==0 and not a and b and not c);speeches=speeches+1;tasks=0;canceled=created==cancelAt;log(text);return not canceled
 end
 function me:IsPerformingScriptTask()tasks=tasks+1;return tasks<=wait end
 function r:MemberResource(name)assert(name=='seh_Witch' or name=='seh_Mother');return name=='seh_Witch' and witch or mother end
 function r:PrepareResource(value)assert(value==witch)end
 function r:TryAcquire(value,actor,priority)
  assert(priority==4)
  if value==witch then assert(actor==me);attempts=attempts+1;return attempts>wait end
  assert(value.kind=='lease' and actor==hero);return true
 end
 function r:StartMovie(name)
  assert(name=='' and created==destroyed);created=created+1;local handles={0,31,713};currentHandle=handles[((created-1+seed)%3)+1]
  if scene=='before' then beforeHits=beforeHits+1;totalHits=totalHits+1 elseif scene=='after' then afterHits=afterHits+1;totalHits=totalHits+1 else assert(scene=='talk')end
  log('start:'..scene..':'..currentHandle);return currentHandle
 end
 function r:DestroyMovie(value)
  assert(value==currentHandle and not paused and destroyed==created-1);destroyed=destroyed+1;log('destroy:'..scene..':'..value)
  if scene=='before' and beforeHits==beforeCount and not canceled and not terminating() then
   assert(available==0 and delivered==0);delivered=4-initial-inventory+overfill;assert(delivered>0);available=available+delivered;log('delivery:'..delivered)
  end
  if scene=='after' and afterHits==afterCount then finished=true end
 end
 function r:ResourceThingHealthAbove(handle,minimum)assert(minimum==0);return q:GetHealth(self:ScriptThing(handle))>minimum end
 function r:ScriptThing(value)assert(value==witch);return me end
 function r:NewStringMap()strings=strings+1;return {kind='strings'}end
 function r:NewResource()leases=leases+1;return {kind='lease'}end
 function r:NewActorMap()actors=actors+1;return {kind='actors'}end
 function r:SetString(map,name,value)assert(map.kind=='strings' and name=='$ARG1' and value==argument and string.lower(argument)~='null');map.ARG1=value end
 function r:SetActor(map,name,value)assert(map.kind=='actors' and (name=='WITCH' and value==witch or name=='MUM' and value==mother or name=='HERO' and value.kind=='lease'));map[name]=value end
 function r:RunMacroWithStrings(name,map,values,a,b)
  assert(map.WITCH==witch and map.MUM==mother and map.HERO.kind=='lease' and values.kind=='strings' and not a and b and camera)
  if name=='CS_SICK_WITCH' then assert(not intro and takes==0 and values.ARG1==nil);intros=intros+1
  else assert(name=='CS_SICK_POTION' and initial+takes>=4 and available==0);potions=potions+1;assert(values.ARG1==(string.lower(argument)~='null' and argument or nil))end
  log(name)
 end
 function r:DestroyActorMap(value)assert(value.kind=='actors');actors=actors-1 end
 function r:ReleaseResource(value)assert(value.kind=='lease');leases=leases-1 end
 function r:DestroyStringMap(value)assert(value.kind=='strings');strings=strings-1 end
 Init(q,me);OnPersist(q,me,{ });Main(q,me);inspect=true;OnPersist(q,me,{ })
 assert(not paused and not camera and leases==0 and strings==0 and actors==0 and created==destroyed)
 assert(saved.IntroDone and saved.ThingsForPotionGot==initial+takes and available==inventory+delivered-takes)
 assert(potions<=1 and objectives==potions and cleared==potions and intros==0)
 if finished then assert(beforeHits==beforeCount and afterHits==afterCount and potions==1 and takes==inventory+delivered and saved.ThingsForPotionGot==4+overfill and available==0)end
 if termination==0 and cancelAt==0 then assert(finished,'mixed transition incomplete')end
 return table.concat(trace,'|'),frames,reads,questions,answers,tasks,speeches,attempts,takes,potions,created,destroyed,beforeHits,afterHits,saved.ThingsForPotionGot
end'''

CASES=list(itertools.product((0,1,2),(0,1),(1,2,3,4),(1,2,4),(0,2),(False,True),(False,True),(0,2),(0,),(0,1,2),(0,),('POTION','nUlL')))
CASES+=list(itertools.product((0,1,2),(1,),(2,),(2,),(0,),(True,),(True,),(2,),(1,2,5,9,16,24,40),(0,1),(0,1,2,3,4,5),('POTION',)))
def execute(source,case):
 lua=LuaRuntime(unpack_returned_tuples=True);lua.globals().helperSource=source[1];lua.execute("package.preload['V_SickChild.native_quest_helpers']=assert(load(helperSource))");lua.execute(source[0]);return lua.execute(H)(*case)
@pytest.mark.parametrize('case',CASES)
def test_mixed_cure_transition(sources,case):assert execute(sources['draft'],case)==execute(sources['readable'],case)

@pytest.mark.parametrize('style',('draft','readable'))
@pytest.mark.parametrize('fault',('reset_on_potion','increment','count','threshold','hit_handle'))
def test_broken_mixed_transition_is_detected(sources,style,fault):
 main,helper=sources[style]
 old,new={'reset_on_potion':('quest:ClearThingHasInformation(me)','quest:ClearThingHasInformation(me)\n witchHitDialogueIndex = 0'),'increment':('witchHitDialogueIndex + 1','witchHitDialogueIndex + 2'),'count':('__native_entity_state:GetStateInt("ThingsForPotionGot") + 1','__native_entity_state:GetStateInt("ThingsForPotionGot") + 2') if style=='draft' else ('thingsForPotionGot_ + 1','thingsForPotionGot_ + 2'),'threshold':('if 3 <','if 4 <'),'hit_handle':('DestroyMovie(witchAfterCureHitMovie)','DestroyMovie(0)')}[fault]
 assert old in main
 with pytest.raises(Exception):execute((main.replace(old,new),helper),(0,1,2,2,0,True,True,0,0,0,0,'POTION'))
