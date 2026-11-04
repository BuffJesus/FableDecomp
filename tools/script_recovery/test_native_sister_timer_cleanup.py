"""Exercise real generated escort branches with two distinct owned timers."""
import itertools,os
from pathlib import Path
import pytest
from lupa.lua54 import LuaRuntime
ROOT=next(p for p in Path(__file__).resolve().parents if (p/'tools/script_recovery/script_units.py').is_file())
import json
from unittest.mock import patch
from tools.script_recovery.convert_quest_unit import UnitConverter
from tools.script_recovery.build_readable_unit import readable_file

@pytest.fixture(scope='module')
def generated(tmp_path_factory):
 c=UnitConverter(ROOT/'refs/script_recovery/sick_child/translation_unit_typed.json')
 unit=json.loads((ROOT/'refs/script_recovery/sick_child/units/V_SickChild.json').read_text())
 unit['entities']={'SickChildsSister':unit['entities']['SickChildsSister']};unit['quest']['functions']={};unit['quest']['helpers']={}
 out=tmp_path_factory.mktemp('sister_cleanup')
 with patch.object(c,'constructor_defaults',return_value={}):c.convert(unit,out)
 source=(out/'FSE/V_SickChild/Entities/SickChildsSister.lua').read_text()
 return {'draft':source,'readable':readable_file(source)[0]}


H=r'''
return function(mode,limit,speechTicks,health,acquireDelay)
 local frame,taskUntil,speeches=0,0,0;local resource,movies,timers,paused=nil,{},{},false
 local q,me,hero,mother,r={},{},{},{},{};local events={};local timerCount=0
 local function record(name,value)events[#events+1]={name,value}end
 local function term()return frame>=limit end
 function q:GetHero()return hero end
 function q:RetailResources()return r end
 function q:IsActiveThreadTerminating()return term()end
 function q:NewScriptFrame(actor)assert(actor==me and not term(),'frame after termination');frame=frame+1;return not term()end
 function q:GetStateBool(name)assert(name=='MotherIntroDone');return false end
 function q:RegisterTimer()timerCount=timerCount+1;local id=700+timerCount;assert(not timers[id]);timers[id]=true;record('register',id);return id end
 function q:DeregisterTimer(id)assert(timers[id],'wrong or repeated timer cleanup');timers[id]=nil;record('timer',id)end
 function q:FixMovieSequenceCamera(value)record('camera',value)end
 function q:CameraUseCameraPoint(name,actor,a,b,c)assert(name=='CAM_SC_GIRL' and actor==me and a==-1 and b==0 and c==-1)end
 function q:PauseAllNonScriptedEntities(value)paused=value;record('pause',value)end
 function q:GetHealth(actor)assert(actor==me);return health end
 function q:GetThingWithScriptName(name)assert(name=='SickChildsMother');return mother end
 function q:SetThingHasInformation(actor,a,b,c)assert(actor==me and not a and not b and not c)end
 function q:IsDistanceBetweenThingsUnder(a,b,d)
  if b==mother then assert(a==me and d==2);return false end
  assert(a==me and b==hero and (d==15 or d==11));return true
 end
 function r:DistanceBetweenThings(a,b)assert((a==me or a==hero) and b==mother);return a==me and 20 or 30 end
 function mother:IsNull()return false end
 function mother:GetPos()return {x=1,y=2,z=3}end
 function me:FollowThing(actor,speed,flag)assert(actor==hero and speed==1 and flag)end
 function me:StopFollowingThing(actor)assert(actor==hero)end
 function me:ClearCommands()end
 function me:MoveToPosition(pos,speed,kind,a,b)assert(pos.x==1 and speed==1 and (kind==0 or kind==1) and not a and b)end
 function me:IsTalkedToByHero()return timerCount<2 or mode=='talk'end
 function me:MsgIsHitByHero()return mode=='hit'end
 function me:MsgIsHitByAnySpecialAbilityFromHero()return mode=='ability'end
 function me:MsgIsHitByHeroSpecialAbility(kind)assert(kind==14);return false end
 function me:IsPerformingScriptTask()return frame<taskUntil end
 function me:Speak(target,key,selection,a,b,c)
  assert(target==hero and not a and b and not c)
  local delay=0
  if key=='TEXT_QST_B10_SISTER_TOLD_TO_FOLLOW_20' then assert(selection==0)
  else
   assert(timerCount==2)
   assert(key==(mode=='talk' and 'TEXT_QST_B10_SISTER_THIS_WAY' or 'TEXT_QST_B10_SISTER_ON_HIT'))
   assert(selection==(mode=='talk' and 0 or 2));delay=speechTicks;speeches=speeches+1
  end
  taskUntil=frame+delay
  while frame<taskUntil and not term()do q:NewScriptFrame(me)end
  return not term()
 end
 function r:NewResource()assert(not resource);resource=811;return resource end
 function r:PrepareResource(id)assert(id==resource)end
 function r:TryAcquire(id,actor,kind)assert(id==resource and actor==me and kind==4);return frame>=acquireDelay end
 function r:ScriptThing(id)assert(id==resource);return me end
 function r:ReleaseResource(id)assert(id==resource);assert(next(timers)==nil,'resource released before timers');resource=nil;record('resource',id)end
 local nextMovie=0
 function r:StartMovie(name)assert(name=='');nextMovie=nextMovie+1;movies[nextMovie]=true;record('movieStart',nextMovie);return nextMovie end
 function r:DestroyMovie(id)assert(movies[id],'wrong or repeated movie cleanup');movies[id]=nil;record('movie',id)end
 debug.sethook(function()error('instruction budget')end,'',500000)
 Main(q,me);debug.sethook()
 assert(not resource and next(movies)==nil and next(timers)==nil and not paused,'resource/movie/timer/pause leak')
 assert(health<=0 or limit<=math.max(2,acquireDelay+1) or speeches>0,'escort branch not reached')
 return speeches,events
end
'''
CASES=list(itertools.product(('talk','hit','ability'),(1,2,3,6,10,17),(0,1,3),(0,50),(0,2)))
@pytest.mark.parametrize('style',('draft','readable'))
@pytest.mark.parametrize('mode,limit,speech,health,acquire',CASES)
def test_actual_timer_cleanup(generated,style,mode,limit,speech,health,acquire):
 source=generated[style]
 lua=LuaRuntime();lua.execute(source);lua.execute(H)(mode,limit,speech,health,acquire)

@pytest.fixture(scope='module')
def review_input(tmp_path_factory):
 from tools.script_recovery import native_reviewed_cleanup_returns as adapter
 c=UnitConverter(ROOT/'refs/script_recovery/sick_child/translation_unit_typed.json')
 unit=json.loads((ROOT/'refs/script_recovery/sick_child/units/V_SickChild.json').read_text())
 row=unit['reviewedCleanupReturns']['0x00ec68a0']
 unit['entities']={'SickChildsSister':unit['entities']['SickChildsSister']};unit['quest']['functions']={};unit['quest']['helpers']={}
 original=adapter.restore;captured={}
 def capture(source,fn,row,data):
  if str(fn['address']).lower()=='0x00ec68a0':captured.update(source=source,fn=fn,data=data)
  return original(source,fn,row,data)
 with patch.object(c,'constructor_defaults',return_value={}),patch.object(adapter,'restore',capture):
  c.convert(unit,tmp_path_factory.mktemp('early_cleanup_review'))
 return captured,row

FAULTS=('no_continuation','duplicate','movieSite','tailEnd','continuationSite','frameBytes',
 'nativeInstructions','timerAliases','resourceAlias','movieAlias','target','kind','slot','metadata',
 'helper_movie','helper_timer','helper_thing','helper_base','helper_resource','source_label','source_epilogue')
@pytest.mark.parametrize('fault',FAULTS)
def test_continuation_review_guards(review_input,fault):
 import copy
 from tools.script_recovery.native_reviewed_cleanup_returns import restore_continuations
 captured,parent=review_input;rows=copy.deepcopy(parent['continuations']);fn=copy.deepcopy(captured['fn'])
 source=captured['source'];data=captured['data'];row=rows[0]
 if fault=='no_continuation':assert restore_continuations(source,fn,[],data)==source;return
 if fault=='duplicate':rows.append(copy.deepcopy(row))
 elif fault in ('movieSite','tailEnd','continuationSite','frameBytes'):row[fault]+=4
 elif fault=='nativeInstructions':row[fault]={}
 elif fault=='timerAliases':row[fault].reverse()
 elif fault in ('resourceAlias','movieAlias'):row[fault]='wrong'
 elif fault in ('target','slot'):row['calls'][1][fault]+=4
 elif fault=='kind':row['calls'][2]['kind']='resource'
 elif fault=='metadata':fn['calls']=[c for c in fn['calls'] if int(c['site'],16)!=row['calls'][1]['site']]
 elif fault.startswith('helper_'):
  address=dict(movie=0x6e7b80,timer=0xcd4470,thing=0x4aa840,base=0x99a2e0,resource=0x7e74d0)[fault.removeprefix('helper_')]
  class Changed:
   def bytes_at(self,a,n):return b'\x90'*n if a==address else data.bytes_at(a,n)
  changed=Changed()
 elif fault=='source_label':source=source.replace('LAB_00ec7821:','LAB_00ffffff:')
 elif fault=='source_epilogue':source=source.replace('GSI->DeregisterTimer(xStack_bc);','GSI->DeregisterTimer(wrong);')
 with pytest.raises(ValueError,match='reviewed cleanup continuation'):
  restore_continuations(source,fn,rows,changed if fault.startswith('helper_') else data)


def test_original_native_early_return():
 from tools.script_recovery.lift_native_lua import RData
 import unicorn as u
 from unicorn import x86_const as x
 import struct
 rd=RData();cpu=u.Uc(u.UC_ARCH_X86,u.UC_MODE_32)
 cpu.mem_map(0xec6000,0x2000);cpu.mem_write(0xec68a0,rd.bytes_at(0xec68a0,5962))
 cpu.mem_map(0x300000,0x10000)
 helpers={0x6e7b80:'movie',0xcd4470:'timer',0x4aa840:'thing',0x7e74d0:'resource'}
 for a in helpers:cpu.mem_map(a&~0xfff,0x1000);cpu.mem_write(a,b'\xc3')
 cpu.mem_map(0x204000,0x1000);cpu.mem_write(0x204000,b'\xc3')
 state={}
 def put(a,v):cpu.mem_write(a,struct.pack('<I',v&0xffffffff))
 def word(a):return struct.unpack('<I',cpu.mem_read(a,4))[0]
 def hook(machine,a,size,user):
  if a==0x204000:machine.emu_stop();return
  if a in helpers:
   ecx=machine.reg_read(x.UC_X86_REG_ECX)
   state['events'].append((helpers[a],word(ecx) if helpers[a]=='timer' else ecx))
   sp=machine.reg_read(x.UC_X86_REG_ESP)
   machine.reg_write(x.UC_X86_REG_EIP,word(sp));machine.reg_write(x.UC_X86_REG_ESP,sp+4)
 cpu.hook_add(u.UC_HOOK_CODE,hook)
 balanced=0x300800;rows=[]
 for (edge,offset),seed in itertools.product(((0xec77eb,0x60),(0xec77fd,0x60),(0xec780f,0x74),(0xec781d,0x74)),(0,0x12345678,0xffffffff)):
  state={'events':[]};saved=[(seed+i*0x1234567)&0xffffffff for i in range(4)]
  for index,value in enumerate(saved):put(balanced+index*4,value)
  put(balanced+0x1c,701);put(balanced+0x18,702);put(balanced+216,0x204000)
  cpu.reg_write(x.UC_X86_REG_ESP,balanced);cpu.reg_write(x.UC_X86_REG_ECX,0xdeadbeef)
  cpu.emu_start(edge,0x204000,count=100)
  assert state['events']==[('movie',balanced+offset),('timer',701),('thing',balanced+0x38),('timer',702),('resource',balanced+0x20)]
  assert cpu.reg_read(x.UC_X86_REG_ESP)==balanced+220
  assert [cpu.reg_read(r) for r in (x.UC_X86_REG_EBP,x.UC_X86_REG_EDI,x.UC_X86_REG_ESI,x.UC_X86_REG_EBX)]==saved
  rows.append(dict(edge=hex(edge),movieSlot=offset-216,seed=seed,events=state['events']))
