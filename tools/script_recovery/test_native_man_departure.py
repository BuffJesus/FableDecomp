import itertools,sys
from pathlib import Path
import pytest
from lupa.lua54 import LuaRuntime
ROOT=next(p for p in Path(__file__).resolve().parents if (p/'tools/script_recovery/script_units.py').exists());sys.path.insert(0,str(ROOT))
@pytest.fixture(scope='module')
def sources():
 return {s:(ROOT/'refs/script_recovery/lifted/SickChild'/s/'FSE/V_SickChild/Entities/ManInLove.lua').read_text() for s in ('draft','readable')}
H=r'''return function(rewarded,busyWait,acquireWait,distance,moveCycles,termination,targetTag)
 local q,me,hero,r={},{},{},{};local target={tag=targetTag};local resource=nil;local trace={};local saved={};local inspect=false
 local frames,taskQueries,lookups,prepares,attempts,moves,fades,releases=0,0,0,0,0,0,0,0
 local cycleAttempts=0;local function log(s)trace[#trace+1]=s end
 local function terminating()return fades>0 or termination>0 and frames>=termination end
 function q:RetailResources()return r end
 function q:SetThingPersistent(actor,value)assert(actor==me and value)end
 function q:PersistTransferBool(ctx,name,value)if not inspect then return name=='GivenLetter' or name=='GotReward' and rewarded end;saved[name]=value;return value end
 function q:NewScriptFrame(actor)assert(actor==me);frames=frames+1;assert(frames<40);return not terminating()end
 function q:IsActiveThreadTerminating()return terminating()end
 function q:GetHero()return hero end
 function q:ReadGlobalGameDataString(offset)assert(offset==0x740);return 'LETTER' end
 function q:IsObjectInThingsPossession(value,actor)assert(actor==hero and (value=='LETTER' or value=='OBJECT_SICK_CHILD_WOMANS_HERO_LETTER'));return false end
 function q:GetStateInt(name)assert(name=='MansLoverState');return 1 end
 function q:SetIsPushableByHero(actor,value)assert(actor==me and not value)end
 function q:SetCreatureBrain(actor,name)assert(actor==me and name=='BRAIN_PASSIVE_OVERRIDE')end
 function q:SetWanderCentrePoint(actor,pos)assert(actor==me and pos.x==1)end
 function q:SetWanderMinDistance(actor,value)assert(actor==me and value==0)end
 function q:SetWanderMaxDistance(actor,value)assert(actor==me and value==10)end
 function q:SetScriptingStateGroup(actor,value)assert(actor==me and value==4)end
 function q:GetNearestWithDefName(actor,name)assert(actor==me and name=='REGION_EXIT_POINT' and rewarded and taskQueries>busyWait);lookups=lookups+1;log('lookup:'..target.tag);return target end
 function q:IsDistanceBetweenThingsOver(actor,value,radius)assert(actor==me and value==target and radius==2.0);return distance>2 and moves<moveCycles end
 function q:FadeOutAndKillEntity(actor,kill,seconds,persistent)assert(actor==me and kill==true and seconds==1.5 and persistent==true and fades==0);fades=fades+1;log('fade')end
 function me:GetPos()return {x=1}end
 function me:IsTalkedToByHero()return false end
 function me:MsgIsHitByHero()return false end
 function me:MsgIsHitByAnySpecialAbilityFromHero()return false end
 function me:IsPerformingScriptTask()assert(rewarded);taskQueries=taskQueries+1;return taskQueries<=busyWait end
 function me:MoveToThing(value,radius,mode)assert(value==target and radius==1.0 and mode==0);moves=moves+1;log('move:'..target.tag)end
 function r:NewResource()assert(not resource);resource={};return resource end
 function r:PrepareResource(value)assert(value==resource);prepares=prepares+1;cycleAttempts=0 end
 function r:TryAcquire(value,actor,priority)assert(value==resource and actor==me and priority==4);attempts=attempts+1;cycleAttempts=cycleAttempts+1;return cycleAttempts>acquireWait end
 function r:ReleaseResource(value)assert(value==resource and releases==0);releases=releases+1;log('release')end
 Init(q,me);OnPersist(q,me,{ });Main(q,me);inspect=true;OnPersist(q,me,{ })
 assert(releases==(resource and 1 or 0) and saved.GotReward==rewarded and saved.GivenLetter)
 if not rewarded then assert(taskQueries==0 and lookups==0 and prepares==0 and attempts==0 and moves==0 and fades==0)end
 if termination==0 then assert(fades==1 and moves==(distance>2 and moveCycles or 0))end
 assert(prepares==lookups and fades<=1 and moves<=moveCycles)
 return table.concat(trace,'|'),frames,taskQueries,lookups,prepares,attempts,moves,fades,releases,saved.GotReward
end'''
CASES=list(itertools.product((True,),(0,2),(0,2),(-1.0,1.999,2.0,2.001,37.0),(1,2),(0,1,2,3,5,7,12,24),('EXIT_A','EXIT_B')))
CASES+=list(itertools.product((False,),(0,2),(0,2),(2.0,37.0),(1,2),(1,2,5,12),('EXIT_A',)))
def execute(source,case):
 lua=LuaRuntime(unpack_returned_tuples=True);lua.execute(source);return lua.execute(H)(*case)
@pytest.mark.parametrize('case',CASES)
def test_actual_departure(sources,case):assert execute(sources['draft'],case)==execute(sources['readable'],case)

@pytest.mark.parametrize('style',('draft','readable'))
@pytest.mark.parametrize('fault',('exit','distance','radius','move','fade_time','persistent'))
def test_wrong_departure_contract_is_rejected(sources,style,fault):
 source=sources[style]
 old,new={'exit':('"REGION_EXIT_POINT"','"WRONG_EXIT"'),'distance':('r15, 2.0','r15, 3.0') if style=='draft' else ('getNearestWithDefName, 2.0','getNearestWithDefName, 3.0'),'radius':('r15, 1.0, 0','r15, 2.0, 0') if style=='draft' else ('getNearestWithDefName, 1.0, ENTITY_MOVE_WALK','getNearestWithDefName, 2.0, ENTITY_MOVE_WALK'),'move':('r15, 1.0, 0','r15, 1.0, 1') if style=='draft' else ('getNearestWithDefName, 1.0, ENTITY_MOVE_WALK','getNearestWithDefName, 1.0, 1'),'fade_time':('me, true, 1.5, true','me, true, 2.5, true'),'persistent':('me, true, 1.5, true','me, true, 1.5, false')}[fault]
 assert old in source
 with pytest.raises(Exception):
  for distance in (2.0,2.001):execute(source.replace(old,new),(True,0,0,distance,1,0,'EXIT_A'))

def test_native_departure_distance_and_move_operands():
 import struct
 import unicorn as u
 from unicorn import x86_const as x
 from tools.script_recovery.lift_native_lua import RData
 rd=RData();cpu=u.Uc(u.UC_ARCH_X86,u.UC_MODE_32)
 for a,n in ((0xec9000,0x3000),(0xcbe000,0x1000),(0x7e7000,0x1000),(0x4aa000,0x1000),(0xf35000,0x1000),(0x300000,0x10000),(0x500000,0x10000),(0x200000,0x1000)):cpu.mem_map(a,n)
 cpu.mem_write(0xec9000,rd.bytes_at(0xec9000,0x3000));cpu.mem_write(0xcbe3ea,rd.bytes_at(0xcbe3ea,116));cpu.mem_write(0x7e7300,rd.bytes_at(0x7e7300,15))
 for a in (0xf35b30,0xcbe716,0x4aa840,0x200100,0x200200,0x200300):cpu.mem_write(a,b'\xc3')
 def put(a,v):cpu.mem_write(a,struct.pack('<I',v&0xffffffff))
 def word(a):return struct.unpack('<I',cpu.mem_read(a,4))[0]
 data={};rows=[]
 def hook(machine,a,size,user):
  if a in (0xec9a9e,0xec9bda):data['end']=a;machine.emu_stop();return
  sp=machine.reg_read(x.UC_X86_REG_ESP);ecx=machine.reg_read(x.UC_X86_REG_ECX)
  if a==0xf35b30:
   assert ecx==data['owner'];machine.reg_write(x.UC_X86_REG_EAX,int(data['term']));purge=0
  elif a==0x200100:
   assert ecx in (data['owner']+8,data['sp']+0xbc);machine.reg_write(x.UC_X86_REG_EAX,int(data['validMe'] if ecx==data['owner']+8 else data['validTarget']));purge=0
  elif a==0x200200:
   assert ecx in (data['owner']+8,data['sp']+0xbc);machine.reg_write(x.UC_X86_REG_EAX,0x507000 if ecx==data['owner']+8 else 0x507020);purge=0
  elif a==0x200300:
   assert ecx==data['expert'] and [word(sp+i) for i in range(4,32,4)]==[data['sp']+0xbc,0x3f800000,0,0,1,0,1];data['moves']+=1;purge=28
  elif a==0xcbe716:
   assert ecx==data['owner']+8 and machine.reg_read(x.UC_X86_REG_EDX)&255==1 and [word(sp+4),word(sp+8)]==[0x3fc00000,1];data['fades']+=1;purge=8
  elif a==0x4aa840:
   assert ecx==data['sp']+0xbc;data['destroyed']+=1;purge=0
  else:return
  machine.reg_write(x.UC_X86_REG_EIP,word(sp));machine.reg_write(x.UC_X86_REG_ESP,sp+4+purge)
 cpu.hook_add(u.UC_HOOK_CODE,hook)
 for vector,validMe,validTarget,term,owner,sp in itertools.product(((0.,0.,0.),(1.999,0.,0.),(2.,0.,0.),(0.,2.,0.),(0.,0.,2.),(1.5,1.5,0.),(2.001,0.,0.)),(False,True),(False,True),(False,True),(0x501000,0x502000),(0x300800,0x301800)):
  thingtable=0x504000;expert=0x505000;experttable=0x505800
  put(owner+8,thingtable);put(sp+0xbc,thingtable);put(thingtable+0x12c,0x200100);put(thingtable+0x18,0x200200);put(sp+0x18,expert);put(expert,experttable);put(experttable+0x14,0x200300)
  cpu.mem_write(0x507000,struct.pack('<fff',0,0,0));cpu.mem_write(0x507020,struct.pack('<fff',*vector))
  cpu.reg_write(x.UC_X86_REG_ESI,owner);cpu.reg_write(x.UC_X86_REG_EDI,owner+8);cpu.reg_write(x.UC_X86_REG_ESP,sp)
  data=dict(owner=owner,sp=sp,expert=expert,validMe=validMe,validTarget=validTarget,term=term,moves=0,fades=0,destroyed=0)
  cpu.emu_start(0xec9a2d,0xec9a9f,count=200)
  far=validMe and validTarget and sum(v*v for v in vector)>4
  assert data['moves']==int(far and not term) and data['fades']==int(not far and not term)
  assert data['destroyed']==int(not term) and cpu.reg_read(x.UC_X86_REG_ESP)==sp
  rows.append((vector,validMe,validTarget,term,owner,sp))
 assert len(rows)==224
