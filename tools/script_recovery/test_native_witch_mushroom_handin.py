import itertools,sys
from pathlib import Path
import pytest
from lupa.lua54 import LuaRuntime
ROOT=next(p for p in Path(__file__).resolve().parents if (p/'tools/script_recovery/script_units.py').exists());sys.path.insert(0,str(ROOT))
@pytest.fixture(scope='module')
def sources():
 return {s:tuple((ROOT/'refs/script_recovery/lifted/SickChild'/s/'FSE/V_SickChild'/f).read_text() for f in ('Entities/Witch.lua','native_quest_helpers.lua')) for s in ('draft','readable')}
H=r'''return function(initial,inventory,yes,intro,health,speak,wait,termination,handle,argument)
 local q,me,hero,r,witch,mother={},{},{},{},{},{};local trace={};local paused=false;local camera=false;local inspect=false;local saved={};local boundary={}
 local frames,reads,questions,answers,tasks,speeches,attempts,created,destroyed,takes,cleared,potions,intros,objectives=0,0,0,0,0,0,0,0,0,0,0,0,0,0
 local leases,strings,actors=0,0,0;local canceled=false;local introWritten=false;local currentDefinition=nil;local available=inventory
 local function log(value)trace[#trace+1]=value end
 local function terminating()return termination>0 and frames>=termination end
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
 function q:GiveHeroYesNoQuestion(text,a,b,c,d)assert(text=='TEXT_QST_B10_QUESTION_GIVE_MUSHROOM' and a=='TEXT_OBJECT_HERO_ANSWER_YES' and b=='TEXT_OBJECT_HERO_ANSWER_NO' and c=='' and d==true);questions=questions+1;assert(questions==1);log('question')end
 function q:MsgIsQuestionAnsweredYesOrNo()answers=answers+1;if answers<=wait then return -1 end;return yes and 1 or 0 end
 function q:GetHealth(actor)assert(actor==me);return health and 10 or 0 end
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
 function me:IsTalkedToByHero()return initial<4 end
 function me:MsgIsHitByHero()return false end
 function me:MsgIsHitByAnySpecialAbilityFromHero()return false end
 function me:Speak(actor,text,group,a,b,c)
  local expected=inventory==0 and 'TEXT_QST_B10_WITCH_WAITING_FOR_CURE' or (yes and 'TEXT_QST_B10_WITCH_NEED_MORE_CURE_10' or 'TEXT_QST_B10_WITCH_NOT_GIVEN_MORE_MUSHROOMS_10')
  assert(actor==hero and text==expected and group==0 and not a and b and not c);speeches=speeches+1;tasks=0;canceled=not speak;log(text);return speak
 end
 function me:IsPerformingScriptTask()tasks=tasks+1;return tasks<=wait end
 function r:MemberResource(name)assert(name=='seh_Witch' or name=='seh_Mother');return name=='seh_Witch' and witch or mother end
 function r:PrepareResource(value)assert(value==witch)end
 function r:TryAcquire(value,actor,priority)
  assert(priority==4)
  if value==witch then assert(actor==me);attempts=attempts+1;return attempts>wait end
  assert(value.kind=='lease' and actor==hero);return true
 end
 function r:StartMovie(name)assert(name=='' and created==0);created=created+1;return handle end
 function r:DestroyMovie(value)assert(value==handle and not paused and destroyed==0);destroyed=destroyed+1;log('destroy');if not canceled and not terminating() then error(boundary)end end
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
 Init(q,me);OnPersist(q,me,{ });local ok,err=pcall(Main,q,me);assert(ok or err==boundary,tostring(err))
 local firstFrames=0
 if not intro and initial<4 and err==boundary then
  assert(intros==1 and introWritten and takes==0 and questions==0 and potions==0 and leases==0 and strings==0 and actors==0 and not paused and created==destroyed)
  log('second-main');firstFrames=frames;intro=true;created=0;destroyed=0;frames=0;attempts=0;tasks=0
  ok,err=pcall(Main,q,me);assert(ok or err==boundary,tostring(err))
 end
 inspect=true;OnPersist(q,me,{ })
 assert(not paused and not camera and leases==0 and strings==0 and actors==0 and created==destroyed)
 assert(saved.ThingsForPotionGot==initial+takes and saved.IntroDone==(intro or introWritten) and takes<=inventory and available==inventory-takes)
 assert(potions<=1 and objectives==potions and cleared==potions)
 if err==boundary then
  assert(created==1 and destroyed==1)
  if intro and yes and inventory>0 then assert(takes==inventory and potions==((initial+inventory>=4) and 1 or 0))else assert(takes==0 and potions==0)end
 end
 if initial>=4 then assert(created==0 and takes==0 and questions==0 and intros==0 and potions==0)end
 if termination==0 then assert(err==boundary or canceled)end
 return table.concat(trace,'|'),firstFrames+frames,reads,questions,answers,tasks,speeches,attempts,takes,potions,intros,saved.ThingsForPotionGot,saved.IntroDone
end'''
CASES=list(itertools.product((0,1,3),(0,1,3,4,7),(False,True),(False,True),(False,True),(False,True),(0,2),(0,),(0,31,713),('POTION','NULL','nUlL','')))
CASES+=list(itertools.product((0,1,3),(1,4,7),(False,True),(False,True),(False,True),(False,True),(2,),(1,2,4,8,16,32),(31,),('POTION',)))
CASES+=list(itertools.product((4,7),(0,4),(False,True),(False,True),(False,True),(True,),(0,2),(1,2,8,16),(31,),('POTION',)))
def execute(source,case):
 lua=LuaRuntime(unpack_returned_tuples=True);lua.globals().helperSource=source[1];lua.execute("package.preload['V_SickChild.native_quest_helpers']=assert(load(helperSource))");lua.execute(source[0]);return lua.execute(H)(*case)
@pytest.mark.parametrize('case',CASES)
def test_full_mushroom_recipe(sources,case):assert execute(sources['draft'],case)==execute(sources['readable'],case)

@pytest.mark.parametrize('style',('draft','readable'))
@pytest.mark.parametrize('fault',('definition','count','threshold','argument','macro','objective','string_key','camera'))
def test_wrong_recipe_contract_is_rejected(sources,style,fault):
 main,helper=sources[style]
 old,new={'definition':('0x738','0x740') if style=='draft' else ('1848','1856'),'count':('__native_entity_state:GetStateInt("ThingsForPotionGot") + 1','__native_entity_state:GetStateInt("ThingsForPotionGot") + 2') if style=='draft' else ('thingsForPotionGot_ + 1','thingsForPotionGot_ + 2'),'threshold':('if 3 <','if 7 <'),'argument':('0x744','0x740') if style=='draft' else ('1860','1856'),'macro':('"CS_SICK_POTION"','"CS_WRONG_POTION"'),'objective':('TEXT_QUEST_SICK_CHILD_OBJECTIVE_03','WRONG_OBJECTIVE'),'string_key':('"$ARG1"','"$WRONG"'),'camera':('FixMovieSequenceCamera(false)','FixMovieSequenceCamera(true)')}[fault]
 assert old in main or old in helper
 with pytest.raises(Exception):execute((main.replace(old,new),helper.replace(old,new)),(3,1,True,True,True,True,0,0,31,'POTION'))

def test_native_uncapped_inventory_consumption_and_definition_lifetimes():
 import struct
 import unicorn as u
 from unicorn import x86_const as x
 from tools.script_recovery.lift_native_lua import RData
 from tools.script_recovery.convert_quest_unit import callee_stack_words
 rd=RData()
 for slot,n in ((0x118,0),(0x2e0,2),(0x1f4,1),(0x1c,0)):assert callee_stack_words(int.from_bytes(rd.bytes_at(0x1260f0c+slot,4),'little'))==n
 cpu=u.Uc(u.UC_ARCH_X86,u.UC_MODE_32)
 for a,n in ((0xece000,0x2000),(0x415000,0x1000),(0x99e000,0x1000),(0xf35000,0x1000),(0x143e000,0x1000),(0x300000,0x10000),(0x500000,0x10000),(0x200000,0x1000)):cpu.mem_map(a,n)
 cpu.mem_write(0xece7f0,rd.bytes_at(0xece7f0,4083))
 for a in (0x415d70,0x99eae0,0xf35b30,0x200100,0x200200,0x200300,0x200400):cpu.mem_write(a,b'\xc3')
 def put(a,v):cpu.mem_write(a,struct.pack('<I',v&0xffffffff))
 def word(a):return struct.unpack('<I',cpu.mem_read(a,4))[0]
 data={};rows=[]
 def hook(machine,a,size,user):
  if a in (0xecec4d,0xeced2b):data['end']=a;machine.emu_stop();return
  sp=machine.reg_read(x.UC_X86_REG_ESP);ecx=machine.reg_read(x.UC_X86_REG_ECX)
  if a==0x415d70:
   out=word(sp+4);assert ecx==0x509000+0x738 and out in (data['sp']+0x44,data['sp']+0x3c);data['reads']+=1;put(out,713+data['reads']);data['latest']=out;purge=4
  elif a==0x200100:
   assert ecx==data['gsi'];machine.reg_write(x.UC_X86_REG_EAX,data['hero']);purge=0
  elif a==0x200200:
   assert ecx==data['gsi'] and [word(sp+4),word(sp+8)]==[data['sp']+0x44,data['hero']] and data['latest']==data['sp']+0x44 and word(word(sp+4))==713+data['reads']
   data['queries']+=1;machine.reg_write(x.UC_X86_REG_EAX,int(data['remaining']>0));purge=8
  elif a==0x200300:
   assert ecx==data['gsi'] and word(sp+4)==data['sp']+0x3c and data['latest']==data['sp']+0x3c and word(word(sp+4))==713+data['reads'] and data['remaining']>0
   assert word(data['owner']+0x20)==data['initial']+data['takes'];data['remaining']-=1;data['takes']+=1;purge=4
  elif a==0x99eae0:
   assert ecx==data['latest'];data['dtors']+=1;purge=0
  elif a==0x200400:
   assert ecx==data['gsi'];data['frames']+=1;purge=0
  elif a==0xf35b30:
   assert ecx==data['owner'];machine.reg_write(x.UC_X86_REG_EAX,int(data['deadline']>0 and data['frames']>=data['deadline']));purge=0
  else:return
  machine.reg_write(x.UC_X86_REG_EIP,word(sp));machine.reg_write(x.UC_X86_REG_ESP,sp+4+purge)
 cpu.hook_add(u.UC_HOOK_CODE,hook)
 for initial,inventory,deadline,gsi,owner,sp in itertools.product((0,1,3,4,7),(0,1,3,4,7),(0,1,2,4),(0x500000,0x501000,0x502000),(0x504000,0x505000),(0x300800,0x301800)):
  table=gsi+0x800;put(gsi,table)
  for slot,target in ((0x118,0x200100),(0x2e0,0x200200),(0x1f4,0x200300),(0x1c,0x200400)):put(table+slot,target)
  put(owner+4,gsi);put(owner+0x20,initial);put(0x143e90c,0x509000)
  cpu.reg_write(x.UC_X86_REG_ESI,owner);cpu.reg_write(x.UC_X86_REG_ESP,sp)
  data=dict(initial=initial,owner=owner,gsi=gsi,sp=sp,hero=0x508000,remaining=inventory,deadline=deadline,reads=0,queries=0,takes=0,dtors=0,frames=0)
  cpu.emu_start(0xecebc0,0xeced2c,count=1500)
  expected=inventory if deadline==0 else min(inventory,deadline-1)
  assert data['takes']==expected and word(owner+0x20)==initial+expected and data['remaining']==inventory-expected
  assert data['queries']==expected+1 and data['reads']==data['dtors']==data['queries']+data['takes']
  assert cpu.reg_read(x.UC_X86_REG_ESP)==sp and (data['end']==0xecec4d)==(data['remaining']==0)
  rows.append((initial,inventory,deadline,gsi,owner,sp))
 assert len(rows)==1200
