import itertools,sys
from pathlib import Path
import pytest
from lupa.lua54 import LuaRuntime
ROOT=next(p for p in Path(__file__).resolve().parents if (p/'tools/script_recovery/script_units.py').exists());sys.path.insert(0,str(ROOT))
@pytest.fixture(scope='module')
def sources():
 return {s:(ROOT/'refs/script_recovery/lifted/SickChild'/s/'FSE/V_SickChild/Entities/ManInLove.lua').read_text() for s in ('draft','readable')}
T=r'''return function(path,state,inventory,health,cancelStage,questionWait,taskWait,acquireWait,termination,handle,postState,postHealth,postSpeak,postTaskWait,postAcquireWait,postTermination,postHandle)
 local q,me,hero,r={},{},{},{};local boundary={};local paused=false
 local frame,checks,created,reads,given,attempts,released,destroyed,tasks=0,0,0,0,0,0,0,0,0
 local key='DYNAMIC_REWARD';local resource=nil;local removed=0;local questions,answers,speeches,takes,morality,cleared=0,0,0,0,0,0;local trace={};local function log(s)trace[#trace+1]=s end;local phase="boot";local saved={}
 function q:RetailResources()return r end
 function q:PersistTransferBool(ctx,name,value)if phase=='boot' then return name=='GivenLetter' end;saved[name]=value;return value end
 function q:SetThingPersistent(actor,value)assert(actor==me and value)end
 function q:NewScriptFrame(actor)assert(actor==me);frame=frame+1;assert(frame<30);return not (termination>0 and frame>=termination)end
 function q:IsActiveThreadTerminating()checks=checks+1;return termination>0 and frame>=termination end
 function q:GetHero()return hero end
 function q:ReadGlobalGameDataString(offset)assert(offset==0x740 or offset==0x738);reads=reads+1;return offset==0x740 and 'LETTER' or key end
 function q:IsObjectInThingsPossession(value,actor)assert(actor==hero and (value=='LETTER' or value=='OBJECT_SICK_CHILD_WOMANS_HERO_LETTER'));return value=='LETTER' and inventory~=2 or value=='OBJECT_SICK_CHILD_WOMANS_HERO_LETTER' and inventory~=1 end
 function q:GetStateInt(name)assert(name=='MansLoverState');return state end
 function q:SetThingHasInformation(actor,a,b,c)assert(actor==me and a and b and not c)end
 function q:SetIsPushableByHero(actor,value)assert(actor==me and not value)end
 function q:SetCreatureBrain(actor,name)assert(actor==me and name=='BRAIN_PASSIVE_OVERRIDE')end
 function q:SetWanderCentrePoint(actor,pos)assert(actor==me and pos.x==1)end
 function q:SetWanderMinDistance(actor,value)assert(actor==me and value==0)end
 function q:SetWanderMaxDistance(actor,value)assert(actor==me and value==10)end
 function q:SetScriptingStateGroup(actor,value)assert(actor==me and value==4)end
 function q:PauseAllNonScriptedEntities(value)assert(value~=paused);paused=value end
 function q:GetHealth(actor)assert(actor==me);return health and 1 or 0 end
 function q:GiveHeroObject(value,amount,flag)assert(value==key and amount==-1 and flag==false,'wrong reward operands');given=given+1;log('give:'..value)end
 function q:RemoveItemFromContainer(actor,value)assert(actor==me and value==key and given==1);removed=removed+1;log('remove:'..value)end
 function me:GetPos()return {x=1,y=2,z=3}end
 function me:IsTalkedToByHero()return true end
 function q:GiveHeroYesNoQuestion(text,yes,no,extra,flag)
  questions=questions+1;answers=0;assert(questions<=2 and (questions==1 or path~=1))
  assert(text==(questions==1 and 'TEXT_QST_B10_MAN_IN_LOVE_RETURNING_OBJECT_QUESTION' or 'TEXT_QST_B10_MAN_IN_LOVE_WOMANS_OPINION_QUESTION'))
  assert(yes=='TEXT_OBJECT_HERO_ANSWER_YES' and no=='TEXT_OBJECT_HERO_ANSWER_NO' and extra=='' and flag==true);log('question:'..questions)
 end
 function q:MsgIsQuestionAnsweredYesOrNo()answers=answers+1;if answers<=questionWait then return -1 end;return (questions==1 and path==1 or questions==2 and path==2) and 1 or 0 end
 function q:TakeObjectFromHero(value)takes=takes+1;assert(path==1 and value==(takes==1 and 'LETTER' or 'OBJECT_SICK_CHILD_WOMANS_HERO_LETTER') and takes<=2);log('take:'..value)end
 function q:ReadGlobalGameDataFloat(offset)
  local expected=(path==3 and (state==2 and 0x760 or 0x758) or (state==2 and 0x758 or 0x764))
  assert(offset==expected,'wrong morality definition');log('float:'..offset);return offset+0.375
 end
 function q:GiveHeroMorality(value)
  local expected=(path==3 and (state==2 and 0x760 or 0x758) or (state==2 and 0x758 or 0x764))
  assert(value==expected+0.375 and given==1 and removed==1);morality=morality+1;log('morality:'..value)
 end
 function q:ClearThingHasInformation(actor)assert(actor==me and morality==1);cleared=cleared+1;log('clear')end
 function me:Speak(actor,text,group,a,b,c)
  speeches=speeches+1;tasks=0
  local expected=path==1 and 'TEXT_QST_B10_MAN_IN_LOVE_RETURNED_OBJECT_TO_ME' or (speeches==1 and 'TEXT_QST_B10_MAN_IN_LOVE_DIDNT_RETURN_OBJECT_TO_ME' or (path==2 and 'TEXT_QST_B10_MAN_IN_LOVE_HAPPY' or 'TEXT_QST_B10_MAN_IN_LOVE_UNHAPPY'))
  assert(actor==hero and text==expected and group==0 and not a and b and not c);log(text);return speeches~=cancelStage
 end
 function me:IsPerformingScriptTask()tasks=tasks+1;return tasks<=taskWait end
 function r:NewResource()assert(not resource);resource={};return resource end
 function r:PrepareResource(value)assert(value==resource)end
 function r:TryAcquire(value,actor,priority)assert(value==resource and actor==me and priority==4);attempts=attempts+1;return attempts>acquireWait end
 function r:ReleaseResource(value)assert(value==resource);released=released+1 end
 function r:StartMovie(name)assert(name=='');created=created+1;return handle end
 function r:ResourceThingHealthAbove(value,minimum)assert(minimum==0.0);return q:GetHealth(self:ScriptThing(value))>minimum end
 function r:ScriptThing(value)assert(value==resource);return me end
 function r:DestroyMovie(value)assert(value==handle and not paused and destroyed==0);destroyed=destroyed+1;if cleared==1 then error(boundary)end end
 Init(q,me);OnPersist(q,me,{})
 local ok,err=pcall(Main,q,me);assert(ok or err==boundary,tostring(err));phase='inspect';OnPersist(q,me,{})
 saved.HelpedGuyOut=ReadHelpedGuyOut()
 assert(not paused and created==destroyed and saved.GivenLetter)
 assert(given<=1 and removed==given and morality<=given)
 local expectedReward=path==1 and morality==1 or path~=1 and given==1
 assert(saved.GotReward==expectedReward and saved.HelpedGuyOut==(expectedReward and path~=3))
 if err==boundary then assert(given==1 and removed==1 and morality==1 and cleared==1 and released==0 and questions==(path==1 and 1 or 2))
 else assert(cleared==0 and released==(resource and 1 or 0))end
 if termination==0 and (not health or cancelStage==0 or path==1 and cancelStage==2) then assert(err==boundary)end

 assert(err==boundary,'initial reward must complete')
 local helped=ReadHelpedGuyOut();local postCanceled=false;local commands=0;local postSpeeches=0
 trace={};resource=nil;frame=0;checks=0;created=0;destroyed=0;attempts=0;released=0;tasks=0;cleared=0
 local firstGifts,firstRemoved=given,removed
 termination=postTermination;handle=postHandle;acquireWait=postAcquireWait
 function q:GetStateInt(name)assert(name=='MansLoverState');return postState end
 function q:IsObjectInThingsPossession(value,actor)assert(actor==hero and (value=='LETTER' or value=='OBJECT_SICK_CHILD_WOMANS_HERO_LETTER'));return false end
 function q:GetHealth(actor)assert(actor==me);return postHealth and 1 or 0 end
 function me:ClearCommands()commands=commands+1;log('clear-commands')end
 function me:Speak(actor,text,group,a,b,c)
  local expected=helped and 'TEXT_QST_B10_MAN_IN_LOVE_POST_QUEST_HAPPY_10' or (postState==3 and 'TEXT_QST_B10_MAN_IN_LOVE_POST_QUEST_MYRA_DIED_10' or 'TEXT_QST_B10_MAN_IN_LOVE_POST_QUEST_UNHAPPY_10')
  assert(actor==hero and text==expected and group==0 and not a and b and not c);postSpeeches=postSpeeches+1;tasks=0;log(text);postCanceled=not postSpeak;return postSpeak
 end
 function me:IsPerformingScriptTask()tasks=tasks+1;return tasks<=postTaskWait end
 function r:DestroyMovie(value)
  assert(value==handle and not paused and destroyed==0);destroyed=destroyed+1;log('destroy:'..handle)
  if not postCanceled and not (termination>0 and frame>=termination) then error(boundary)end
 end
 local postOK,postErr=pcall(Main,q,me);assert(postOK or postErr==boundary,tostring(postErr))
 OnPersist(q,me,{})
 assert(not paused and created==destroyed and given==firstGifts and removed==firstRemoved)
 assert(saved.GotReward and saved.GivenLetter and ReadHelpedGuyOut()==helped and cleared==0)
 assert(commands<=1 and postSpeeches==(commands==1 and postHealth and 1 or 0))
 if postErr==boundary then assert(commands==1 and created==1 and destroyed==1 and released==0)else assert(released==(resource and 1 or 0))end
 if termination==0 and (not postHealth or postSpeak) then assert(postErr==boundary)end
 return table.concat(trace,'|'),frame,attempts,released,destroyed,tasks,commands,postSpeeches,saved.GotReward,helped
end'''
S=r'''return function(state,health,speak,taskWait,acquireWait,termination,handle)
 local q,me,hero,r={},{},{},{};local boundary={};local paused=false
 local frame,checks,created,reads,given,attempts,released,destroyed,tasks=0,0,0,0,0,0,0,0,0
 local key='UNUSED_REWARD';local resource=nil;local removed=0;local phase="boot";local saved={}
 function q:RetailResources()return r end
 function q:PersistTransferBool(ctx,name,value)if phase=='boot' then return name=='GivenLetter' end;saved[name]=value;return value end
 function q:SetThingPersistent(actor,value)assert(actor==me and value)end
 function q:NewScriptFrame(actor)assert(actor==me);frame=frame+1;assert(frame<30);return not (termination>0 and frame>=termination)end
 function q:IsActiveThreadTerminating()checks=checks+1;return termination>0 and frame>=termination end
 function q:GetHero()return hero end
 function q:ReadGlobalGameDataString(offset)assert(offset==0x740 or offset==0x738);reads=reads+1;return offset==0x740 and 'LETTER' or key end
 function q:IsObjectInThingsPossession(value,actor)assert(actor==hero and (value=='LETTER' or value=='OBJECT_SICK_CHILD_WOMANS_HERO_LETTER'));return false end
 function q:GetStateInt(name)assert(name=='MansLoverState');return state end
 function q:SetThingHasInformation(actor,a,b,c)assert(actor==me and a and b and not c)end
 function q:SetIsPushableByHero(actor,value)assert(actor==me and not value)end
 function q:SetCreatureBrain(actor,name)assert(actor==me and name=='BRAIN_PASSIVE_OVERRIDE')end
 function q:SetWanderCentrePoint(actor,pos)assert(actor==me and pos.x==1)end
 function q:SetWanderMinDistance(actor,value)assert(actor==me and value==0)end
 function q:SetWanderMaxDistance(actor,value)assert(actor==me and value==10)end
 function q:SetScriptingStateGroup(actor,value)assert(actor==me and value==4)end
 function q:PauseAllNonScriptedEntities(value)assert(value~=paused);paused=value end
 function q:GetHealth(actor)assert(actor==me);return health and 1 or 0 end
 function q:GiveHeroObject(value,amount,flag)assert(value==key and amount==-1 and flag==false,'wrong reward operands');given=given+1;end
 function q:RemoveItemFromContainer(actor,value)assert(actor==me and value==key and given==1);removed=removed+1 end
 function me:GetPos()return {x=1,y=2,z=3}end
 function me:IsTalkedToByHero()return true end
 function me:Speak(actor,text,group,a,b,c)assert(actor==hero and text=='TEXT_QST_B10_MAN_IN_LOVE_DELIVER_LETTER' and group==0 and not a and b and not c);return speak end
 function me:IsPerformingScriptTask()tasks=tasks+1;return tasks<=taskWait end
 function r:NewResource()assert(not resource);resource={};return resource end
 function r:PrepareResource(value)assert(value==resource)end
 function r:TryAcquire(value,actor,priority)assert(value==resource and actor==me and priority==4);attempts=attempts+1;return attempts>acquireWait end
 function r:ReleaseResource(value)assert(value==resource);released=released+1 end
 function r:StartMovie(name)assert(name=='');created=created+1;return handle end
 function r:ResourceThingHealthAbove(value,minimum)assert(minimum==0.0);return q:GetHealth(self:ScriptThing(value))>minimum end
 function r:ScriptThing(value)assert(value==resource);return me end
 function r:DestroyMovie(value)assert(value==handle and not paused);destroyed=destroyed+1;if not (health and not speak) and not (termination>0 and frame>=termination) then error(boundary)end end
 Init(q,me);OnPersist(q,me,{})
 local ok,err=pcall(Main,q,me);assert(ok or err==boundary,tostring(err));phase='inspect';OnPersist(q,me,{})
 assert(not paused and created==destroyed and not ReadHelpedGuyOut())
 if err==boundary then assert(given==0 and removed==0 and created==1 and destroyed==1 and released==0 and not saved.GotReward)else assert(not saved.GotReward and released==(resource and 1 or 0))end
 if termination==0 and (not health or speak) then assert(err==boundary)end
 return given,removed,frame,attempts,released,destroyed,tasks,reads,saved.GotReward
end'''

CASES=list(itertools.product((1,2,3),(1,2,3),(False,True),(False,True),(0,2),(0,2),(0,1,2,3,5,7,12),(0,31,713)))
def execute(source,case):
 path,state,health,speak,taskWait,acquireWait,termination,handle=case
 lua=LuaRuntime(unpack_returned_tuples=True);probe='__native_entity_state:GetStateBool("HelpedGuyOut")' if 'local __native_entity_state' in source else 'helpedGuyOut'
 lua.execute(source+'\nfunction ReadHelpedGuyOut()return '+probe+' end')
 return lua.execute(T)(path,2,1,False,0,0,0,0,0,31,state,health,speak,taskWait,acquireWait,termination,handle)
@pytest.mark.parametrize('case',CASES)
def test_reward_to_followup(sources,case):assert execute(sources['draft'],case)==execute(sources['readable'],case)

REMINDER_CASES=list(itertools.product((1,2),(False,True),(False,True),(0,2),(0,2),(0,1,2,3,5,7,12,20),(0,31,713)))
@pytest.mark.parametrize('case',REMINDER_CASES)
def test_actual_unrewarded_reminder(sources,case):
 results=[]
 for source in sources.values():
  lua=LuaRuntime(unpack_returned_tuples=True);probe='__native_entity_state:GetStateBool("HelpedGuyOut")' if 'local __native_entity_state' in source else 'helpedGuyOut'
  lua.execute(source+'\nfunction ReadHelpedGuyOut()return '+probe+' end');results.append(lua.execute(S)(*case))
 assert results[0]==results[1]
@pytest.mark.parametrize('style',('draft','readable'))
@pytest.mark.parametrize('text',('POST_QUEST_HAPPY_10','POST_QUEST_UNHAPPY_10','POST_QUEST_MYRA_DIED_10','DELIVER_LETTER'))
def test_wrong_followup_dialogue_rejected(sources,style,text):
 source=sources[style];old='TEXT_QST_B10_MAN_IN_LOVE_'+text;assert old in source;source=source.replace(old,'WRONG_DIALOGUE')
 with pytest.raises(Exception):
  if text=='DELIVER_LETTER':
   lua=LuaRuntime(unpack_returned_tuples=True);lua.execute(source+'\nfunction ReadHelpedGuyOut()return false end');lua.execute(S)(1,True,True,0,0,0,31)
  else:
   for path,state in itertools.product((1,2,3),(1,2,3)):execute(source,(path,state,True,True,0,0,0,31))

def test_native_post_reward_dialogue_selection():
 import struct
 import unicorn as u
 from unicorn import x86_const as x
 from tools.script_recovery.lift_native_lua import RData
 rd=RData();cpu=u.Uc(u.UC_ARCH_X86,u.UC_MODE_32)
 for a,n in ((0xec9000,0x3000),(0x7e7000,0x1000),(0x4aa000,0x1000),(0xf35000,0x1000),(0x122d000,0x1000),(0x300000,0x10000),(0x500000,0x10000),(0x200000,0x1000)):cpu.mem_map(a,n)
 cpu.mem_write(0xec9000,rd.bytes_at(0xec9000,0x3000));cpu.mem_write(0x7e7390,rd.bytes_at(0x7e7390,15))
 for a in (0x7e7360,0x7e7490,0x7e7450,0x4aa840,0xf35b30,0x200100,0x200200,0x200300):cpu.mem_write(a,b'\xc3')
 cpu.mem_write(0x200100,b'\xd9\x05'+struct.pack('<I',0x507000)+b'\xc2\x04\x00')
 cpu.mem_write(0x122dedc,struct.pack('<f',0.0))
 def put(a,v):cpu.mem_write(a,struct.pack('<I',v&0xffffffff))
 def word(a):return struct.unpack('<I',cpu.mem_read(a,4))[0]
 data={};rows=[]
 def hook(machine,a,size,user):
  if a in (0xec96b3,0xec95d3,0xec9b58):data['end']=a;machine.emu_stop();return
  sp=machine.reg_read(x.UC_X86_REG_ESP);ecx=machine.reg_read(x.UC_X86_REG_ECX)
  if a==0xf35b30:
   assert ecx==data['owner'];data['checks']+=1;machine.reg_write(x.UC_X86_REG_EAX,int(data['term']>0 and data['checks']>=data['term']));purge=0
  elif a==0x7e7360:
   assert ecx==data['sp']+0x10;data['commands']+=1;purge=0
  elif a==0x7e7490:
   assert ecx==data['sp']+0x10;out=word(sp+4);put(out,0);machine.reg_write(x.UC_X86_REG_EAX,out);data['temporary']=out;purge=4
  elif a==0x200100:
   assert ecx==data['gsi'] and word(sp+4)==data['temporary'];return
  elif a==0x4aa840:
   assert ecx==data['temporary'];data['dtors']+=1;purge=0
  elif a==0x200200:
   assert ecx==data['gsi'];machine.reg_write(x.UC_X86_REG_EAX,data['hero']);purge=0
  elif a==0x200300:
   literal=word(sp+8);text=rd.bytes_at(literal,100).split(b'\0')[0].decode('ascii')
   expected='TEXT_QST_B10_MAN_IN_LOVE_POST_QUEST_'+('HAPPY_10' if data['helped'] else 'MYRA_DIED_10' if data['state']==3 else 'UNHAPPY_10')
   assert ecx==data['expert'] and text==expected and [word(sp+i) for i in (4,12,16,20,24)]==[data['hero'],0,0,1,0];data['speeches']+=1;purge=24
  elif a==0x7e7450:
   assert ecx==data['sp']+0x10;machine.reg_write(x.UC_X86_REG_EAX,0);purge=0
  else:return
  machine.reg_write(x.UC_X86_REG_EIP,word(sp));machine.reg_write(x.UC_X86_REG_ESP,sp+4+purge)
 cpu.hook_add(u.UC_HOOK_CODE,hook)
 for state,helped,health,term,owner,sp in itertools.product((1,2,3),(0,1,255),(0.,1.),(0,1,2,3),(0x501000,0x502000),(0x300800,0x301800)):
  gsi=0x500000;table=0x500800;quest=0x504000;expert=0x505000;experttable=0x505800
  put(gsi,table);put(table+0x420,0x200100);put(table+0x118,0x200200);put(owner+4,gsi);put(owner+0x14,quest);put(quest+0x6c,state);cpu.mem_write(owner+0x1d,bytes((1,helped)))
  put(sp+0x18,expert);put(expert,experttable);put(experttable+0x34,0x200300);cpu.mem_write(0x507000,struct.pack('<f',health))
  cpu.reg_write(x.UC_X86_REG_ESI,owner);cpu.reg_write(x.UC_X86_REG_ECX,owner);cpu.reg_write(x.UC_X86_REG_ESP,sp)
  data=dict(state=state,helped=helped,health=health,term=term,owner=owner,sp=sp,gsi=gsi,expert=expert,hero=0x506000,checks=0,commands=0,speeches=0,dtors=0)
  cpu.emu_start(0xec9439,0xec96b4,count=180)
  assert data['commands']==int(term!=1) and data['speeches']==int(health>0 and term not in (1,2))
  assert data['dtors']==int(term not in (1,2)) and cpu.reg_read(x.UC_X86_REG_ESP)==sp
  assert bytes(cpu.mem_read(owner+0x1d,2))==bytes((1,helped))
  assert data['end'] in (0xec96b3,0xec95d3,0xec9b58)
  rows.append((state,helped,health,term,owner,sp))
 assert len(rows)==288
