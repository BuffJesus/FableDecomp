import itertools,sys
from pathlib import Path
import pytest
from lupa.lua54 import LuaRuntime
ROOT=next(p for p in Path(__file__).resolve().parents if (p/'tools/script_recovery/script_units.py').exists());sys.path.insert(0,str(ROOT))
@pytest.fixture(scope='module')
def sources():
 return {s:(ROOT/'refs/script_recovery/lifted/SickChild'/s/'FSE/V_SickChild/Entities/ManInLove.lua').read_text() for s in ('draft','readable')}
H=r'''return function(rewarded,killed,key,morality,repeatCount)
 local q,me={},{ };local inspections=false;local queries,gifts,reads,floats,morals=0,0,0,0,0;local trace={};local saved={}
 local function log(value)trace[#trace+1]=value end
 function q:SetThingPersistent(actor,flag)assert(actor==me and flag)end
 function q:PersistTransferBool(ctx,name,value)if not inspections then return name=='GotReward' and rewarded end;saved[name]=value;return value end
 function me:MsgIsKilledBy(value)assert(value=='' and not rewarded);queries=queries+1;log('killed');return killed end
 function q:ReadGlobalGameDataString(offset)assert(offset==0x738);reads=reads+1;log('definition');return key end
 function q:GiveHeroObject(value,amount,flag)assert(value==key and amount==-1 and flag==false);gifts=gifts+1;log('give')end
 function q:ReadGlobalGameDataFloat(offset)assert(offset==0x768);floats=floats+1;log('morality-definition');return morality end
 function q:GiveHeroMorality(value)assert(value==morality and gifts==morals+1);morals=morals+1;log('morality')end
 Init(q,me);OnPersist(q,me,{})
 for i=1,repeatCount do OnPredicateFail(q,me)end
 inspections=true;OnPersist(q,me,{})
 local eligible=not rewarded and killed
 assert(queries==(rewarded and 0 or repeatCount))
 assert(gifts==(eligible and repeatCount or 0) and reads==gifts and floats==gifts and morals==gifts)
 assert(saved.GotReward==rewarded and saved.GivenLetter==false and ReadHelpedGuyOut()==false)
 return table.concat(trace,'|'),queries,gifts,reads,floats,morals,saved.GotReward
end'''
CASES=list(itertools.product((False,True),(False,True),('', 'REWARD_DEFINITION', 'MixedCase_42', 'x'*129),(-31.25,0.0,713.375),(1,2,3)))
def execute(source,case):
 lua=LuaRuntime(unpack_returned_tuples=True);probe='__native_entity_state:GetStateBool("HelpedGuyOut")' if 'local __native_entity_state' in source else 'helpedGuyOut'
 lua.execute(source+'\nfunction ReadHelpedGuyOut()return '+probe+' end');return lua.execute(H)(*case)
@pytest.mark.parametrize('case',CASES)
def test_actual_death_reward(sources,case):assert execute(sources['draft'],case)==execute(sources['readable'],case)
@pytest.mark.parametrize('style',('draft','readable'))
@pytest.mark.parametrize('fault',('definition','morality','amount','unknown','killer'))
def test_invalid_death_contract_is_rejected(sources,style,fault):
 source=sources[style];offset=lambda n:hex(n) if style=='draft' else str(n)
 old,new={'definition':(offset(0x738),offset(0x740)),'morality':(offset(0x768),offset(0x758)),'amount':('-1, false','1, false'),'unknown':('-1, false','-1, true'),'killer':('MsgIsKilledBy("")','MsgIsKilledBy("HERO")')}[fault]
 assert old in source
 with pytest.raises(Exception):execute(source.replace(old,new),(False,True,'DYNAMIC',-31.25,1))

def test_original_death_callback_operands_and_short_circuit():
 import struct
 import unicorn as u
 from unicorn import x86_const as x
 from tools.script_recovery.lift_native_lua import RData
 from tools.script_recovery.convert_quest_unit import callee_stack_words
 rd=RData();assert rd.bytes_at(0x122d70e,1)==b'\0'
 for slot,n in ((0x1e4,3),(0x270,1)):
  assert callee_stack_words(int.from_bytes(rd.bytes_at(0x1260f0c+slot,4),'little'))==n
 cpu=u.Uc(u.UC_ARCH_X86,u.UC_MODE_32)
 for a,n in ((0xec8000,0x1000),(0x99e000,0x1000),(0x415000,0x1000),(0x143e000,0x1000),(0x300000,0x10000),(0x500000,0x10000),(0x200000,0x1000)):cpu.mem_map(a,n)
 cpu.mem_write(0xec8230,rd.bytes_at(0xec8230,0xaa))
 for a in (0x99ebf0,0x99eae0,0x415d70,0x200100,0x200200,0x200300):cpu.mem_write(a,b'\xc3')
 def put(a,v):cpu.mem_write(a,struct.pack('<I',v&0xffffffff))
 def word(a):return struct.unpack('<I',cpu.mem_read(a,4))[0]
 data={};rows=[]
 def hook(machine,a,size,user):
  if a==0x200300:machine.emu_stop();return
  sp=machine.reg_read(x.UC_X86_REG_ESP);ecx=machine.reg_read(x.UC_X86_REG_ECX)
  if a==0x99ebf0:
   assert ecx==data['temp'] and [word(sp+4),word(sp+8)]==[0x122d70e,0xffffffff];put(ecx,0);machine.reg_write(x.UC_X86_REG_EAX,ecx);data['ctors']+=1;purge=8
  elif a==0x200100:
   assert ecx==data['owner']+8 and word(sp+4)==data['temp'] and word(data['temp'])==0
   data['queries']+=1;machine.reg_write(x.UC_X86_REG_EAX,data['killed']);purge=4
  elif a==0x415d70:
   assert ecx==data['globals']+0x738 and word(sp+4)==data['temp'];put(data['temp'],data['cookie']);data['reads']+=1;purge=4
  elif a==0x200200:
   assert ecx==data['gsi'] and [word(sp+4),word(sp+8),word(sp+12)]==[data['temp'],0xffffffff,0] and word(data['temp'])==data['cookie'];data['gifts']+=1;purge=12
  elif a==0x200400:
   assert ecx==data['gsi'] and word(sp+4)==data['bits'] and data['gifts']==1;data['morals']+=1;purge=4
  elif a==0x99eae0:
   assert ecx==data['temp'];data['dtors']+=1;put(ecx,0);purge=0
  else:return
  machine.reg_write(x.UC_X86_REG_EIP,word(sp));machine.reg_write(x.UC_X86_REG_ESP,sp+4+purge)
 cpu.mem_write(0x200400,b'\xc3');cpu.hook_add(u.UC_HOOK_CODE,hook)
 for rewarded,killed,owner,entrysp,cookie,value in itertools.product((0,1,255),(0,1,255),(0x501000,0x502000),(0x300800,0x301800),(0,713),(-31.25,0.0,713.375)):
  gsi=0x500000;table=0x500800;thingtable=0x504000;globals_=0x505000
  put(gsi,table);put(table+0x1e4,0x200200);put(table+0x270,0x200400);put(owner+4,gsi);put(owner+8,thingtable);put(thingtable+0x48,0x200100)
  cpu.mem_write(owner+0x1c,bytes((1,rewarded,1)));put(0x143e90c,globals_);bits=struct.unpack('<I',struct.pack('<f',value))[0];put(globals_+0x768,bits)
  put(entrysp,0x200300);cpu.reg_write(x.UC_X86_REG_ECX,owner);cpu.reg_write(x.UC_X86_REG_ESP,entrysp);cpu.reg_write(x.UC_X86_REG_EBX,0x12345678);cpu.reg_write(x.UC_X86_REG_ESI,0x87654321)
  data=dict(owner=owner,gsi=gsi,globals=globals_,temp=entrysp-4,killed=killed,cookie=cookie,bits=bits,ctors=0,queries=0,reads=0,gifts=0,morals=0,dtors=0)
  cpu.emu_start(0xec8230,0x200301,count=150)
  eligible=not rewarded and bool(killed)
  assert data['ctors']==data['queries']==int(not rewarded) and data['reads']==data['gifts']==data['morals']==int(eligible)
  assert data['dtors']==int(not rewarded)+int(eligible)
  assert bytes(cpu.mem_read(owner+0x1c,3))==bytes((1,rewarded,1))
  assert cpu.reg_read(x.UC_X86_REG_ESP)==entrysp+4 and cpu.reg_read(x.UC_X86_REG_EBX)==0x12345678 and cpu.reg_read(x.UC_X86_REG_ESI)==0x87654321
  rows.append((rewarded,killed,owner,entrysp,cookie,value))
 assert len(rows)==216
