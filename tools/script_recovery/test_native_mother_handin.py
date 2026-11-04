import itertools,sys
from pathlib import Path
import pytest
from lupa.lua54 import LuaRuntime
ROOT=next(p for p in Path(__file__).resolve().parents if (p/'tools/script_recovery/script_units.py').exists());sys.path.insert(0,str(ROOT))
@pytest.fixture(scope='module')
def sources():
 return {s:tuple((ROOT/'refs/script_recovery/lifted/SickChild'/s/'FSE/V_SickChild'/f).read_text() for f in ('Entities/SickChildsMother.lua','native_quest_helpers.lua')) for s in ('draft','readable')}
H=r'''return function(initialPotion,currentPotion,yes,health,speak,questionWait,taskWait,acquireWait,termination,handle,key)
 local q,me,hero,r,mother,witch={},{},{},{},{},{};local trace={};local paused=false;local camera=false;local finished=false
 local frames,reads,questions,answers,tasks,speeches,attempts,created,destroyed,takes,cleared,macros,prepares=0,0,0,0,0,0,0,0,0,0,0,0,0
 local leases,strings,actors=0,0,0;local boundary={};local canceled=false
 local function log(v)trace[#trace+1]=v end
 local function terminating()return termination>0 and frames>=termination end
 function q:RetailResources()return r end
 function q:SetIsPushableByHero(actor,flag)assert(actor==me and not flag)end
 function q:GetStateBool(name)assert(name=='MotherIntroDone' or name=='FinishedQuest');return name=='MotherIntroDone' or finished end
 function q:SetStateBool(name,value)assert(name=='FinishedQuest' and value==true and macros==1 and takes==1 and cleared==1);finished=true;log('finished')end
 function q:NewScriptFrame(actor)assert(actor==me);frames=frames+1;assert(frames<45);return not terminating()end
 function q:IsActiveThreadTerminating()return terminating()end
 function q:GetHero()return hero end
 function q:ReadGlobalGameDataString(offset)assert(offset==0x744);reads=reads+1;return reads==1 and 'INITIAL_POTION' or key end
 function q:IsObjectInThingsPossession(value,actor)assert(actor==hero and value==(reads==1 and 'INITIAL_POTION' or key));return reads==1 and initialPotion or reads~=1 and currentPotion end
 function q:SetThingHasInformation(actor,a,b,c)assert(actor==me and not a and b and not c and initialPotion);log('information')end
 function q:PauseAllNonScriptedEntities(value)assert(value~=paused);paused=value;log('pause:'..tostring(value))end
 function q:GiveHeroYesNoQuestion(text,a,b,c,d)assert(text=='TEXT_QST_B10_MOTHER_CURE_QUESTION' and a=='TEXT_OBJECT_HERO_ANSWER_YES' and b=='TEXT_OBJECT_HERO_ANSWER_NO' and c=='' and d==true);questions=questions+1;assert(questions==1);log('question')end
 function q:MsgIsQuestionAnsweredYesOrNo()answers=answers+1;if answers<=questionWait then return -1 end;return yes and 1 or 0 end
 function q:GetHealth(actor)assert(actor==me);return health and 10 or 0 end
 function q:FixMovieSequenceCamera(value)assert(value~=camera and paused);camera=value;log('camera:'..tostring(value))end
 function q:TakeObjectFromHero(value)assert(value==key and currentPotion and macros==1 and leases==0 and strings==0 and actors==0);takes=takes+1;log('take:'..value)end
 function q:ClearThingHasInformation(actor)assert(actor==me and takes==1);cleared=cleared+1;log('clear')end
 function me:IsTalkedToByHero()return true end
 function me:Speak(actor,text,group,a,b,c)
  assert(actor==hero and text==(currentPotion and 'TEXT_QST_B10_MOTHER_ASK_CURE' or 'TEXT_QST_B10_MOTHER_CURE_REMIND_10') and group==(currentPotion and 2 or 0) and not a and b and not c);speeches=speeches+1;tasks=0;canceled=not speak;log(text);return speak
 end
 function me:IsPerformingScriptTask()tasks=tasks+1;return tasks<=taskWait end
 function r:MemberResource(name)assert(name=='seh_Mother' or name=='seh_Witch');return name=='seh_Mother' and mother or witch end
 function r:PrepareResource(value)assert(value==mother);prepares=prepares+1;log('prepare')end
 function r:TryAcquire(value,actor,priority)
  assert(priority==4)
  if value==mother then assert(actor==me);attempts=attempts+1;return attempts>acquireWait end
  assert(value.kind=='lease' and actor==hero);return true
 end
 function r:StartMovie(name)assert(name=='' and created==0);created=created+1;log('start');return handle end
 function r:DestroyMovie(value)
  assert(value==handle and not paused and destroyed==0);destroyed=destroyed+1;log('destroy')
  if not finished and not canceled and not terminating() then error(boundary)end
 end
 function r:ResourceThingHealthAbove(handle,minimum)assert(minimum==0);return q:GetHealth(self:ScriptThing(handle))>minimum end
 function r:ScriptThing(value)assert(value==mother);return me end
 function r:NewStringMap()strings=strings+1;return {kind='strings'}end
 function r:NewResource()leases=leases+1;return {kind='lease'}end
 function r:NewActorMap()actors=actors+1;return {kind='actors'}end
 function r:SetActor(map,name,value)assert(map.kind=='actors' and (name=='MUM' and value==mother or name=='WITCH' and value==witch or name=='HERO' and value.kind=='lease'));map[name]=value end
 function r:RunMacroWithStrings(name,map,values,a,b)assert(name=='CS_SICK_OUTRO' and map.MUM==mother and map.WITCH==witch and map.HERO.kind=='lease' and values.kind=='strings' and not a and b and camera);macros=macros+1;log('outro')end
 function r:DestroyActorMap(map)assert(map.kind=='actors');actors=actors-1 end
 function r:ReleaseResource(value)assert(value.kind=='lease');leases=leases-1 end
 function r:DestroyStringMap(value)assert(value.kind=='strings');strings=strings-1 end
 Init(q,me);OnPersist(q,me,{})
 local ok,err=pcall(Main,q,me);assert(ok or err==boundary,tostring(err))
 assert(not paused and not camera and created==destroyed and leases==0 and strings==0 and actors==0)
 assert(takes==macros and takes==cleared and finished==(takes==1))
 if finished then assert(prepares==2 and speeches==0 and frames>=termination and terminating())end
 if termination==0 then assert(err==boundary or canceled)end
 return table.concat(trace,'|'),frames,reads,questions,answers,tasks,speeches,attempts,created,destroyed,takes,finished,prepares
end'''
CASES=list(itertools.product((False,True),(False,True),(False,True),(False,True),(False,True),(0,2),(0,2),(0,2),(1,2,3,5,7,12),(0,31,713),('', 'POTION_CURRENT')))
def execute(sources,case):
 lua=LuaRuntime(unpack_returned_tuples=True);lua.globals().helperSource=sources[1];lua.execute("package.preload['V_SickChild.native_quest_helpers']=assert(load(helperSource))");lua.execute(sources[0]);return lua.execute(H)(*case)
@pytest.mark.parametrize('case',CASES)
def test_complete_handin_or_refusal(sources,case):assert execute(sources['draft'],case)==execute(sources['readable'],case)

@pytest.mark.parametrize('style',('draft','readable'))
@pytest.mark.parametrize('fault',('potion','finished','macro','question','movie','camera'))
def test_wrong_handin_contract_is_rejected(sources,style,fault):
 main,helper=sources[style]
 old,new={'potion':('0x744','0x740') if style=='draft' else ('1860','1856'),'finished':('"FinishedQuest", true','"FinishedQuest", false'),'macro':('"CS_SICK_OUTRO"','"CS_WRONG_OUTRO"'),'question':('"TEXT_QST_B10_MOTHER_CURE_QUESTION"','"WRONG_QUESTION"'),'movie':('DestroyMovie(motherTalkMovie)','DestroyMovie(0)'),'camera':('FixMovieSequenceCamera(false)','FixMovieSequenceCamera(true)')}[fault]
 assert old in main or old in helper
 with pytest.raises(Exception):execute((main.replace(old,new),helper.replace(old,new)),(True,True,True,True,True,0,0,0,12,31,'CURRENT_POTION'))

def test_native_handin_state_and_final_wait():
 import struct
 import unicorn as u
 from unicorn import x86_const as x
 from tools.script_recovery.lift_native_lua import RData
 from tools.script_recovery.convert_quest_unit import callee_stack_words
 rd=RData();assert callee_stack_words(int.from_bytes(rd.bytes_at(0x1260f0c+0x1f4,4),'little'))==1
 cpu=u.Uc(u.UC_ARCH_X86,u.UC_MODE_32)
 for a,n in ((0xece000,0x1000),(0x415000,0x1000),(0x99e000,0x1000),(0x6e7000,0x1000),(0xcd2000,0x1000),(0xf35000,0x1000),(0x143e000,0x1000),(0x300000,0x10000),(0x500000,0x10000),(0x200000,0x1000)):cpu.mem_map(a,n)
 cpu.mem_write(0xece000,rd.bytes_at(0xece000,0x1000))
 for a in (0x415d70,0x99eae0,0x6e7b80,0xcd23b9,0xcd2770,0xf35b30,0x200100,0x200200,0x200300,0x200400):cpu.mem_write(a,b'\xc3')
 def put(a,v):cpu.mem_write(a,struct.pack('<I',v&0xffffffff))
 def word(a):return struct.unpack('<I',cpu.mem_read(a,4))[0]
 data={};rows=[]
 def hook(machine,a,size,user):
  if a in (0xece3ab,0xece44c):data['end']=a;machine.emu_stop();return
  sp=machine.reg_read(x.UC_X86_REG_ESP);ecx=machine.reg_read(x.UC_X86_REG_ECX)
  if a==0x415d70:
   assert ecx==0x509000+0x744 and word(sp+4)==data['sp']+0x1c;put(word(sp+4),data['cookie']);data['reads']+=1;purge=4
  elif a==0x200100:
   assert ecx==data['gsi'] and word(sp+4)==data['sp']+0x1c and word(word(sp+4))==data['cookie'];data['takes']+=1;purge=4
  elif a==0x99eae0:
   assert ecx==data['sp']+0x1c and data['takes']==1;data['dtors']+=1;purge=0
  elif a==0x200200:
   assert ecx==data['gsi'] and word(sp+4)==data['owner']+8 and data['takes']==data['dtors']==1;data['clears']+=1;purge=4
  elif a==0x200300:
   assert ecx==data['gsi'] and word(sp+4)==0 and bytes(cpu.mem_read(data['quest']+0x68,2))==b'\1\xa5';data['unpauses']+=1;purge=4
  elif a==0x6e7b80:
   assert ecx==data['sp']+0x20 and data['unpauses']==1;data['movies']+=1;purge=0
  elif a==0xf35b30:
   assert ecx==data['owner'];data['checks']+=1;machine.reg_write(x.UC_X86_REG_EAX,int(data['checks']>=data['deadline']));purge=0
  elif a==0xcd23b9:
   assert ecx==data['quest']+0x48;data['prepared']+=1;machine.reg_write(x.UC_X86_REG_EAX,int(data['acquired']));purge=0
  elif a==0xcd2770:
   assert ecx==data['quest']+0x48 and data['acquired'];data['released']+=1;purge=0
  elif a==0x200400:
   assert ecx==data['gsi'];data['frames']+=1;purge=0
  else:return
  machine.reg_write(x.UC_X86_REG_EIP,word(sp));machine.reg_write(x.UC_X86_REG_ESP,sp+4+purge)
 cpu.hook_add(u.UC_HOOK_CODE,hook)
 for gsi,owner,quest,sp,cookie,acquired,deadline in itertools.product((0x500000,0x501000,0x502000),(0x504000,0x505000),(0x506000,0x507000),(0x300800,0x301800),(0,31,713),(False,True),(1,2,4)):
  table=gsi+0x800;put(gsi,table);put(table+0x1f4,0x200100);put(table+0x5a4,0x200200);put(table+0x5ec,0x200300);put(table+0x1c,0x200400);put(owner+4,gsi);put(owner+0x14,quest);put(0x143e90c,0x509000);cpu.mem_write(quest+0x68,b'\0\xa5')
  cpu.reg_write(x.UC_X86_REG_ESI,owner);cpu.reg_write(x.UC_X86_REG_EBP,gsi);cpu.reg_write(x.UC_X86_REG_ESP,sp)
  data=dict(gsi=gsi,owner=owner,quest=quest,sp=sp,cookie=cookie,acquired=acquired,deadline=deadline,reads=0,takes=0,dtors=0,clears=0,unpauses=0,movies=0,checks=0,frames=0,prepared=0,released=0)
  cpu.emu_start(0xece316,0xece44d,count=150)
  assert all(data[k]==1 for k in ('reads','takes','dtors','clears','unpauses','movies'))
  assert data['checks']==deadline and data['frames']==deadline-1 and data['prepared']==int(deadline>1) and data['released']==int(deadline>1 and acquired)
  assert cpu.reg_read(x.UC_X86_REG_ESP)==sp and bytes(cpu.mem_read(quest+0x68,2))==b'\1\xa5'
  rows.append((gsi,owner,quest,sp,cookie,acquired,deadline))
 assert len(rows)==432
