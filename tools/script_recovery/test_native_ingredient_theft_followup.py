import itertools,sys
from pathlib import Path
import pytest
from lupa.lua54 import LuaRuntime
ROOT=next(p for p in Path(__file__).resolve().parents if (p/'tools/script_recovery/script_units.py').is_file());sys.path.insert(0,str(ROOT))
from tools.script_recovery.test_native_ingredient_remove import generated
H=r'''return function(mode,bought,stole,deathFrame,health,wait,deadline,cancel,seed)
 local me,hero,ingredient,region={},{},{},{};local q,r={},{}
 local frames,queries,starts,ends,crimes,morality,clears,removes,attempts=0,0,0,0,0,0,0,0,0
 local held,live,paused,registered,loaded,finished=false,false,false,false,false,false
 local fields={};local trace={};local scene=nil;local handle=nil;local remaining=0
 local function log(s)trace[#trace+1]=s end
 local function term()return finished or deadline>0 and frames>=deadline end
 function q:GetHero()return hero end
 function q:GetStateBool(k)assert(k=='OwnerAlive');return true end
 function q:PersistTransferBool(c,k,v)
  if not loaded then if k=='BoughtSpecialStuff' then return bought else assert(k=='StoleSpecialStuff');return stole end end
  fields[k]=v;return v
 end
 function q:RetailResources()return r end
 function q:NewScriptFrame(a)assert(a==me);frames=frames+1;assert(frames<30);return not term()end
 function q:IsActiveThreadTerminating()return term()end
 function q:GetThingWithScriptName(k)if k=='Ingredient' then return ingredient end;assert(k=='VILLAGE_BARROWFIELDS');return region end
 function q:RemoveThing(a,b,c)assert(a==ingredient and not b and c);removes=removes+1;log('remove')end
 function q:SetThingHasInformation(a,b,c,d)assert(a==me and not b and c and not d)end
 function q:SetIsPushableByHero(a,b)assert(a==me and not b)end
 function q:SetIsThingForcePushable(a,b)assert(a==me and not b)end
 function q:RegisterTimer()assert(not registered);registered=true;return 713 end
 function q:DeregisterTimer(id)assert(id==713 and registered);registered=false end
 function q:GetHealth(a)assert(a==me);return health and 1 or 0 end
 function q:PauseAllNonScriptedEntities(b)assert(live and paused~=b);paused=b end
 function q:ReadGlobalGameDataFloat(n)assert(n==0x76c);return -2.25 end
 function q:GiveHeroMorality(n)
  assert(n==-2.25 and live and paused);OnPersist(q,me,{});assert(fields.BoughtSpecialStuff and fields.StoleSpecialStuff)
  morality=morality+1;log('morality')
 end
 function q:ClearThingHasInformation(a)assert(a==me and morality==1);clears=clears+1;log('clear')end
 function q:AddCrimeCommitted(a,k,b,v,c,d)assert(a==region and k==6 and b and v==nil and c==nil and d==7 and clears==1 and crimes==0);crimes=crimes+1;log('crime')end
 function r:NewResource()assert(not held);held=true;return 21 end
 function r:PrepareResource(id)assert(id==21 and held)end
 function r:TryAcquire(id,a,k)assert(id==21 and a==me and k==4);attempts=attempts+1;return attempts>wait end
 function r:ReleaseResource(id)assert(id==21 and held and not live and not paused and not registered);held=false end
 function r:ResourceThingHealthAbove(id,minimum)assert(id==21 and held and minimum==0.0);return health end
 function r:ScriptThing(id)assert(id==21 and held);return me end
 function r:StartMovie(k)
  assert(k=='' and held and not live);live=true;starts=starts+1;scene=mode=='theft' and crimes==0 and 'theft' or 'post'
  local handles={0,31,713};handle=handles[((starts-1+seed)%3)+1];log('start:'..scene..':'..handle);return handle
 end
 function r:DestroyMovie(id)
  assert(id==handle and live and not paused);live=false;ends=ends+1;log('destroy:'..scene..':'..id)
  if scene=='post' or crimes==0 then finished=true end
 end
 function me:IsTalkedToByHero()return mode=='followup' or crimes==1 end
 function ingredient:IsAlive()queries=queries+1;return frames<deathFrame end
 function me:Speak(a,k,g,b,c,d)
  local expected=scene=='theft' and 'TEXT_QST_B10_TRADER_STOLE_MUSHROOM_10' or (stole or crimes==1) and 'TEXT_QST_B10_TRADER_POST_STOLE_MUSHROOM_10' or 'TEXT_QST_B10_TRADER_POST_BOUGHT_MUSHROOM_10'
  assert(a==hero and k==expected and g==0 and not b and c and not d);remaining=wait;log(k);return starts~=cancel
 end
 function me:IsPerformingScriptTask()if remaining>0 then remaining=remaining-1;return true end;return false end
 Init(q,me);OnPersist(q,me,{});loaded=true;Main(q,me);OnPersist(q,me,{})
 assert(not held and not live and not paused and not registered and starts==ends and starts<=2,'leaked lifetime')
 assert(crimes<=1 and clears==crimes and morality==crimes and fields.BoughtSpecialStuff==(bought or crimes==1) and fields.StoleSpecialStuff==(stole or crimes==1))
 if mode=='followup' then assert(queries==0 and crimes==0 and starts<=1 and removes<=1)end
 if deadline==0 and (cancel==0 or not health) then
  assert(finished)
  if mode=='theft' then assert(crimes==1 and starts==2 and removes==0)else assert(starts==1 and removes==1)end
 end
 if health and cancel==1 and mode=='theft' and starts==1 then assert(crimes==0)end
 return table.concat(trace,'|'),frames,queries,starts,ends,crimes,morality,removes,fields.BoughtSpecialStuff,fields.StoleSpecialStuff
end'''
CASES=[('theft',False,False,*c) for c in itertools.product((1,2,4),(False,True),(0,1,2),(0,1,2,3,5,9),(0,1,2),(0,1,2))]
CASES += [('followup',True,stole,1,*c) for stole in (False,True) for c in itertools.product((False,True),(0,1,2),(0,1,2,3,5,9),(0,1,2),(0,1,2))]
@pytest.mark.parametrize('case',CASES)
def test_actual_theft_followup_and_persisted_reload(generated,case):
 results=[]
 for source in generated.values():
  lua=LuaRuntime();lua.execute(source);first=lua.execute(H)(*case);reload=None
  if case[0]=='theft' and first[-2]:
   fresh=LuaRuntime();fresh.execute(source)
   reload=fresh.execute(H)('followup',first[-2],first[-1],1,case[4],case[5],0,0,(case[-1]+1)%3)
   assert reload[-2:] == first[-2:]
  results.append((first,reload))
 assert results[0]==results[1]

import struct
def test_native_stolen_byte_branch_and_followup_speech():
 from tools.script_recovery.lift_native_lua import RData
 from tools.script_recovery.convert_quest_unit import callee_stack_words
 from tools.script_recovery.annotate_interface_slots import load_slots
 import unicorn as u
 from unicorn import x86_const as x
 rd=RData();target=int.from_bytes(rd.bytes_at(0x1260f0c+0x118,4),'little');assert load_slots()[0x118]=='GetHero' and callee_stack_words(target)==0
 cpu=u.Uc(u.UC_ARCH_X86,u.UC_MODE_32)
 for a,n in ((0xecb000,0x2000),(0x7e7000,0x1000),(0x300000,0x10000),(0x500000,0x10000),(0x200000,0x1000)):cpu.mem_map(a,n)
 cpu.mem_write(0xecb000,rd.bytes_at(0xecb000,0x2000));cpu.mem_write(0x7e7390,rd.bytes_at(0x7e7390,15))
 for a in (0x200100,0x200200):cpu.mem_write(a,b'\xc3')
 def put(a,v):cpu.mem_write(a,struct.pack('<I',v&0xffffffff))
 def word(a):return struct.unpack('<I',cpu.mem_read(a,4))[0]
 state={};layouts=0
 def hook(m,a,size,user):
  if a in (0xecb7c3,0xecb8d0):state['branch']=a;m.emu_stop();return
  if a not in (0x200100,0x200200):return
  sp=m.reg_read(x.UC_X86_REG_ESP);ecx=m.reg_read(x.UC_X86_REG_ECX)
  if a==0x200100:assert ecx==state['gsi'];m.reg_write(x.UC_X86_REG_EAX,state['hero']);purge=0
  else:
   assert ecx==state['expert'];expected='TEXT_QST_B10_TRADER_POST_'+('STOLE' if state['stole'] else 'BOUGHT')+'_MUSHROOM_10'
   assert rd.string_at(word(sp+8))==expected and [word(sp+i) for i in (4,12,16,20,24)]==[state['hero'],0,0,1,0]
   state['speech']+=1;purge=24
  m.reg_write(x.UC_X86_REG_EIP,word(sp));m.reg_write(x.UC_X86_REG_ESP,sp+4+purge)
 cpu.hook_add(u.UC_HOOK_CODE,hook)
 for stole,owner,sp,gsi,expert,hero in itertools.product((0,1,255),(0x501000,0x502000),(0x300800,0x301800),(0x500000,0x503000),(0x505000,0x506000),(0,713,997)):
  state=dict(stole=stole,owner=owner,stack=sp,gsi=gsi,expert=expert,hero=hero,speech=0,branch=None)
  table=gsi+0x800;put(gsi,table);put(table+0x118,0x200100);put(owner+4,gsi);cpu.mem_write(owner+0x1c,bytes([1,stole,0x55]))
  put(sp+0x1c,expert);put(expert,expert+0x800);put(expert+0x800+0x34,0x200200)
  cpu.reg_write(x.UC_X86_REG_ESI,owner);cpu.reg_write(x.UC_X86_REG_ESP,sp)
  cpu.emu_start(0xecb7b6,0xecb8d1,count=20);assert state['branch']==(0xecb8d0 if stole else 0xecb7c3)
  cpu.emu_start(0xecb96c if stole else 0xecb859,0xecb98e if stole else 0xecb87b,count=100)
  assert state['speech']==1 and cpu.reg_read(x.UC_X86_REG_ESP)==sp and cpu.mem_read(owner+0x1c,3)==bytes([1,stole,0x55]);layouts+=1
 assert layouts==144

@pytest.mark.parametrize('style',('draft','readable'))
@pytest.mark.parametrize('fault',('stole','bought','morality','deed','flag','victim','dialogue','movie'))
def test_wrong_theft_state_or_followup_is_detected(generated,style,fault):
 s=generated[style]
 if fault in ('stole','bought'):
  field='StoleSpecialStuff' if fault=='stole' else 'BoughtSpecialStuff'
  if style=='draft':old='__native_entity_state:SetStateBool("'+field+'", true)'
  else:
   import re
   targets=re.findall(r'(?m)^\s*(\w+)\s*=\s*quest:PersistTransferBool\(context,\s*"'+re.escape(field)+'",',s)
   assert len(targets)==1
   old=targets[0]+' = true'
  s=s.replace(old,'do end')
 elif fault=='morality':s=s.replace('ReadGlobalGameDataFloat(0x76c)','ReadGlobalGameDataFloat(0x770)').replace('ReadGlobalGameDataFloat(1900)','ReadGlobalGameDataFloat(1904)').replace('quest:ReadGlobalGameDataFloat(SCRIPT_DEF.StoleMushroomInBarrowFields)','quest:ReadGlobalGameDataFloat(SCRIPT_DEF.BoughtMushroomInBarrowFields)')
 elif fault=='deed':s=s.replace('local_ingredientDeed = 7','local_ingredientDeed = 6').replace(', nil, nil, 7)',', nil, nil, 6)')
 elif fault=='flag':s=s.replace('6, true, nil, nil','6, false, nil, nil').replace('(bUnknown ~= 0), nil, nil','false, nil, nil')
 elif fault=='victim':s=s.replace('6, true, nil, nil','6, true, me, nil').replace('(bUnknown ~= 0), nil, nil','(bUnknown ~= 0), me, nil')
 elif fault=='dialogue':s=s.replace('TEXT_QST_B10_TRADER_POST_STOLE_MUSHROOM_10','TEXT_QST_B10_TRADER_POST_BOUGHT_MUSHROOM_10')
 else:
  import re
  s=re.sub(r'resources:DestroyMovie\([^)]*\)','resources:DestroyMovie(-1)',s)
 assert s!=generated[style]
 lua=LuaRuntime();lua.execute(s)
 with pytest.raises(Exception):lua.execute(H)('theft',False,False,1,True,0,0,0,0)
