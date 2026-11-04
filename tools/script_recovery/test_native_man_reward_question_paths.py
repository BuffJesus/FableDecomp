import itertools,json,sys
from pathlib import Path
import pytest
from lupa.lua54 import LuaRuntime
ROOT=next(p for p in Path(__file__).resolve().parents if (p/'tools/script_recovery/script_units.py').exists());sys.path.insert(0,str(ROOT))
@pytest.fixture(scope='module')
def sources():
 return {s:(ROOT/'refs/script_recovery/lifted/SickChild'/s/'FSE/V_SickChild/Entities/ManInLove.lua').read_text() for s in ('draft','readable')}
T=r'''return function(path,state,inventory,health,cancelStage,questionWait,taskWait,acquireWait,termination,handle)
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
 return table.concat(trace,'|'),frame,attempts,released,destroyed,tasks,reads,saved.GotReward,saved.HelpedGuyOut,takes
end'''

CASES=list(itertools.product((1,2,3),(1,2,3),(1,2,3),(False,True),(0,1,2),(0,2),(0,2),(0,2),(0,),(0,31,713)))
CASES+=list(itertools.product((1,2,3),(1,2,3),(1,2),(True,),(0,),(2,),(2,),(0,2),(1,2,3,5,7,9,12,16,24),(31,)))
def execute(source,case):
 lua=LuaRuntime(unpack_returned_tuples=True)
 probe='__native_entity_state:GetStateBool("HelpedGuyOut")' if 'local __native_entity_state' in source else 'helpedGuyOut'
 lua.execute(source+'\nfunction ReadHelpedGuyOut()return '+probe+' end');return lua.execute(T)(*case)
@pytest.mark.parametrize('case',CASES)
def test_full_reward_questions(sources,case):
 assert execute(sources['draft'],case)==execute(sources['readable'],case)
@pytest.mark.parametrize('style',('draft','readable'))
@pytest.mark.parametrize('old,new',(('1888','1880'),('1892','1880'),('"", true','"", false'),('gotReward = true','gotReward = false'),('helpedGuyOut = true','helpedGuyOut = false')))
def test_wrong_reward_contract_is_detected(sources,style,old,new):
 source=sources[style]
 if style=='draft':
  old,new={'1888':('0x760','0x758'),'1892':('0x764','0x758'),'gotReward = true':('__native_entity_state:SetStateBool("GotReward", true)','__native_entity_state:SetStateBool("GotReward", false)'),'helpedGuyOut = true':('__native_entity_state:SetStateBool("HelpedGuyOut", true)','__native_entity_state:SetStateBool("HelpedGuyOut", false)')}.get(old,(old,new))
 if style=='readable' and old=='gotReward = true':
  import re
  targets=re.findall(r'(?m)^\s*(\w+)\s*=\s*quest:PersistTransferBool\(context,\s*"GotReward",',source)
  assert len(targets)==1
  old,new=targets[0]+' = true',targets[0]+' = false'
 assert old in source
 with pytest.raises(Exception):
  for path,state in itertools.product((1,2,3),(1,2,3)):execute(source.replace(old,new),(path,state,1,True,0,0,0,0,0,31))

def test_native_reward_morality_and_flag_order():
 import struct
 import unicorn as u
 from unicorn import x86_const as x
 from tools.script_recovery.lift_native_lua import RData
 from tools.script_recovery.convert_quest_unit import callee_stack_words
 rd=RData();assert callee_stack_words(int.from_bytes(rd.bytes_at(0x1260f0c+0x270,4),'little'))==1
 cpu=u.Uc(u.UC_ARCH_X86,u.UC_MODE_32)
 for a,n in ((0xec8000,0x3000),(0xf35000,0x1000),(0x143e000,0x1000),(0x300000,0x10000),(0x500000,0x10000),(0x200000,0x1000)):cpu.mem_map(a,n)
 cpu.mem_write(0xec8000,rd.bytes_at(0xec8000,0x3000));cpu.mem_write(0xf35b30,b'\xc3')
 for a in (0x200100,0x200200):cpu.mem_write(a,b'\xc3')
 def put(a,v):cpu.mem_write(a,struct.pack('<I',v&0xffffffff))
 def word(a):return struct.unpack('<I',cpu.mem_read(a,4))[0]
 data={};rows=[]
 def hook(machine,a,size,user):
  sp=machine.reg_read(x.UC_X86_REG_ESP)
  if a in (0xec96b3,0xec95d3,0xec9b58):data['end']=a;machine.emu_stop();return
  if a==0xf35b30:
   assert machine.reg_read(x.UC_X86_REG_ECX)==data['owner'];data['checks']+=1
   machine.reg_write(x.UC_X86_REG_EAX,int(data['terminate']));purge=0
  elif a==0x200100:
   assert machine.reg_read(x.UC_X86_REG_ECX)==data['gsi'] and word(sp+4)==data['bits']
   flags=bytes(machine.mem_read(data['owner']+0x1d,2));assert flags==(b'\0\0' if data['path']==1 else (b'\1\1' if data['path']==2 else b'\1\0'))
   data['morality']+=1;purge=4
  elif a==0x200200:
   assert machine.reg_read(x.UC_X86_REG_ECX)==data['gsi'] and word(sp+4)==data['owner']+8
   data['clear']+=1;purge=4
  else:return
  machine.reg_write(x.UC_X86_REG_EIP,word(sp));machine.reg_write(x.UC_X86_REG_ESP,sp+4+purge)
 cpu.hook_add(u.UC_HOOK_CODE,hook)
 for path,state,terminate,owner,sp,value in itertools.product((1,2,3),(1,2,3),(False,True),(0x501000,0x502000,0x503000),(0x300800,0x301800),(-31.25,0.0,713.375)):
  gsi=0x500000;table=0x500800;globals_=0x505000;quest=0x506000
  put(gsi,table);put(table+0x270,0x200100);put(table+0x5a4,0x200200);put(owner+4,gsi);put(owner+0x14,quest);put(quest+0x6c,state);put(0x143e90c,globals_)
  expected=0x760 if path==3 and state==2 else 0x758 if state==2 or path==3 else 0x764
  bits=struct.unpack('<I',struct.pack('<f',value))[0]
  for offset in (0x758,0x760,0x764):put(globals_+offset,bits if offset==expected else bits^0x13572468)
  cpu.mem_write(owner+0x1d,b'\0\0');cpu.reg_write(x.UC_X86_REG_ESI,owner);cpu.reg_write(x.UC_X86_REG_ESP,sp)
  data=dict(path=path,owner=owner,gsi=gsi,bits=bits,terminate=terminate,checks=0,morality=0,clear=0)
  cpu.emu_start({1:0xec8d52,2:0xec9076,3:0xec91c6}[path],0xec96b4,count=150)
  assert data['checks']==1 and cpu.reg_read(x.UC_X86_REG_ESP)==sp
  assert data['morality']==data['clear']==int(not terminate)
  assert (data['end']==0xec96b3)==(not terminate)
  flags=bytes(cpu.mem_read(owner+0x1d,2))
  assert flags==(b'\0\0' if path==1 and terminate else b'\1\0' if path==3 else b'\1\1')
  rows.append((path,state,terminate,owner,sp,value))
 assert len(rows)==324
