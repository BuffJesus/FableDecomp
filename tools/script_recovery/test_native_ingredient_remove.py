import itertools,json,struct,sys
from pathlib import Path
from unittest.mock import patch
import pytest
from lupa.lua54 import LuaRuntime
ROOT=next(p for p in Path(__file__).resolve().parents if (p/'tools/script_recovery/script_units.py').is_file());sys.path.insert(0,str(ROOT))
from tools.script_recovery.convert_quest_unit import UnitConverter
from tools.script_recovery.build_readable_unit import readable_file
@pytest.fixture(scope='module')
def generated(tmp_path_factory):
 unit=json.loads((ROOT/'refs/script_recovery/sick_child/units/V_SickChild.json').read_text())
 unit['entities']={'IngredientOwner':unit['entities']['IngredientOwner']};unit['quest']['functions']={};unit['quest']['helpers']={}
 c=UnitConverter(ROOT/'refs/script_recovery/sick_child/translation_unit_typed.json')
 with patch.object(c,'constructor_defaults',return_value={}):c.convert(unit,out:=tmp_path_factory.mktemp('ingredient_remove'))
 s=(out/'FSE/V_SickChild/Entities/IngredientOwner.lua').read_text();return {'draft':s,'readable':readable_file(s)[0]}
H=r'''return function(bought,answer,gold,health,delay,tasks,handle)
 local me,hero,ingredient={},{},{};local q,r={},{}
 local held,live,paused,registered=false,false,false,false
 local frame,starts,ends,removes,grants,paid,morality,clears,questions,polls=0,0,0,0,0,0,0,0,0,0
 local seeded=false;local fields={};local trace={};local remaining=0;local finished=false
 local function effect(s)trace[#trace+1]=s end
 function q:GetHero()return hero end
 function q:GetStateBool(k)assert(k=='OwnerAlive');return true end
 function q:PersistTransferBool(c,k,v)if not seeded then return k=='BoughtSpecialStuff' and bought or false end;fields[k]=v;return v end
 function q:RetailResources()return r end
 function q:NewScriptFrame(a)assert(a==me);frame=frame+1;assert(frame<30);return not finished end
 function q:IsActiveThreadTerminating()return finished end
 function q:GetThingWithScriptName(k)assert(k=='Ingredient');return ingredient end
 function q:RemoveThing(a,b,c)assert(a==ingredient,'wrong removal actor');assert(b==false and c==true,'wrong removal flags');removes=removes+1;effect('remove') end
 function q:SetThingHasInformation(a,b,c,d)assert(a==me and not b and c and not d) end
 function q:SetIsPushableByHero(a,b)assert(a==me and not b)end
 function q:SetIsThingForcePushable(a,b)assert(a==me and not b)end
 function q:RegisterTimer()assert(not registered);registered=true;return 713 end
 function q:DeregisterTimer(id)assert(id==713 and registered);registered=false end
 function q:GetHealth(a)assert(a==me);return health and 1 or 0 end
 function q:PauseAllNonScriptedEntities(b)assert(live and paused~=b);paused=b end
 function q:GiveHeroYesNoQuestion(k,y,n,e,b)assert(k=='TEXT_QST_B10_TRADER_BUY_MUSHROOM_QUESTION' and y=='TEXT_OBJECT_HERO_ANSWER_YES' and n=='TEXT_OBJECT_HERO_ANSWER_NO' and e=='' and b);questions=questions+1 end
 function q:MsgIsQuestionAnsweredYesOrNo()polls=polls+1;if polls<=delay then return -1 end;return answer end
 function q:GetHeroGold()return gold end
 function q:ReadGlobalGameDataString(offset)assert(offset==0x738);return 'SPECIAL_MUSHROOM' end
 function q:GiveHeroObject(def,n,b)assert(def=='SPECIAL_MUSHROOM' and n==-1 and not b and live and paused);grants=grants+1;effect('grant')end
 function q:GiveHeroGold(n)assert(n==-1500 and removes==1);paid=paid+1;effect('heroGold')end
 function q:EntityGiveGold(a,n)assert(a==me and n==1500 and paid==1);effect('ownerGold')end
 function q:ReadGlobalGameDataFloat(n)assert(n==0x770);return 2.25 end
 function q:GiveHeroMorality(n)assert(n==2.25 and paid==1);morality=morality+1;effect('morality')end
 function q:ClearThingHasInformation(a)assert(a==me and morality==1);clears=clears+1;effect('clear')end
 function r:NewResource()assert(not held);held=true;return 21 end
 function r:PrepareResource(id)assert(id==21 and held)end
 function r:TryAcquire(id,a,k)assert(id==21 and a==me and k==4);return true end
 function r:ReleaseResource(id)assert(id==21 and held and not live and not paused and not registered);held=false end
 function r:ResourceThingHealthAbove(id,minimum)assert(id==21 and held and minimum==0.0);return health end
 function r:ScriptThing(id)assert(id==21);return me end
 function r:StartMovie(k)assert(k=='' and held and not live);live=true;starts=starts+1;return handle end
 function r:DestroyMovie(id)assert(id==handle and live and not paused);live=false;ends=ends+1;finished=true end
 function me:IsTalkedToByHero()assert(starts==0);return true end
 function ingredient:IsAlive()return true end
 function me:Speak(a,k,g,b,c,d)assert(a==hero and g==0 and not b and c and not d);remaining=tasks;effect(k);return true end
 function me:IsPerformingScriptTask()if remaining>0 then remaining=remaining-1;return true end;return false end
 Init(q,me);OnPersist(q,me,{});seeded=true;Main(q,me);OnPersist(q,me,{})
 assert(not held and not live and not paused and not registered and starts==1 and ends==1)
 local purchase=not bought and answer==1 and gold>=1500
 assert(removes==((bought or purchase) and 1 or 0));assert(grants==(purchase and 1 or 0));assert(paid==grants and morality==grants and clears==grants)
 assert(fields.BoughtSpecialStuff==(bought or purchase) and not fields.StoleSpecialStuff)
 assert(questions==(bought and 0 or 1));assert(polls==(bought and 0 or delay+1))
 if purchase then
  local positions={};for i,v in ipairs(trace)do positions[v]=i end
  assert(positions.grant<positions.remove and positions.remove<positions.heroGold and positions.heroGold<positions.ownerGold and positions.ownerGold<positions.morality and positions.morality<positions.clear)
 end
 return table.concat(trace,'|'),frame,starts,ends,removes,grants,paid,morality,fields.BoughtSpecialStuff,fields.StoleSpecialStuff
end'''
CASES=list(itertools.product((False,True),(0,1),(0,1499,1500,9000),(False,True),(0,2),(0,2),(0,31,713)))
@pytest.mark.parametrize('case',CASES)
def test_actual_purchase_and_saved_removal(generated,case):
 traces=[]
 for s in generated.values():
  lua=LuaRuntime();lua.execute(s);traces.append(lua.execute(H)(*case))
 assert traces[0]==traces[1]

def test_native_lookup_and_both_removal_operands():
 from tools.script_recovery.lift_native_lua import RData
 from tools.script_recovery.convert_quest_unit import callee_stack_words
 from tools.script_recovery.annotate_interface_slots import load_slots
 import unicorn as u
 from unicorn import x86_const as x
 rd=RData();getter=int.from_bytes(rd.bytes_at(0x1260f0c+0x120,4),'little');remove=int.from_bytes(rd.bytes_at(0x1260f0c+0x1b0,4),'little')
 assert load_slots()[0x120]=='GetThingWithScriptName' and callee_stack_words(getter)==2
 assert load_slots()[0x1b0]=='RemoveThing' and callee_stack_words(remove)==3
 cpu=u.Uc(u.UC_ARCH_X86,u.UC_MODE_32)
 for a,n in ((0xecb000,0x2000),(0x300000,0x10000),(0x500000,0x6000),(0x200000,0x1000)):cpu.mem_map(a,n)
 fn=UnitConverter(ROOT/'refs/script_recovery/sick_child/translation_unit_typed.json').native('0x00ecb0e0');a=int(fn['address'],16);cpu.mem_write(a,rd.bytes_at(a,int(fn['bodyEndExclusive'],16)-a))
 for a in (0x200100,0x200200):cpu.mem_write(a,b'\xc3')
 def put(a,v):cpu.mem_write(a,struct.pack('<I',v&0xffffffff))
 def word(a):return struct.unpack('<I',cpu.mem_read(a,4))[0]
 state={}
 def hook(m,a,size,user):
  if a not in (0x200100,0x200200):return
  sp=m.reg_read(x.UC_X86_REG_ESP);assert m.reg_read(x.UC_X86_REG_ECX)==state['gsi']
  if a==0x200100:
   out=word(sp+4);assert out==state['stack']+0x24 and word(sp+8)==state['stack']+0x10
   put(out,0x505000);put(out+4,state['actor']);put(out+8,state['cookie']);m.reg_write(x.UC_X86_REG_EAX,out);purge=8
  else:
   out=word(sp+4);assert out==state['stack']+0x24 and word(out+4)==state['actor'] and word(out+8)==state['cookie']
   assert word(sp+8)==0 and word(sp+12)==1;state['removes']+=1;purge=12
  m.reg_write(x.UC_X86_REG_EIP,word(sp));m.reg_write(x.UC_X86_REG_ESP,sp+4+purge)
 cpu.hook_add(u.UC_HOOK_CODE,hook)
 for gsi,owner,sp,actor,cookie in itertools.product((0x500000,0x501000),(0x503000,0x504000),(0x300800,0x301800),(0,811,0xdeadbeef),(0,713)):
  state=dict(gsi=gsi,owner=owner,stack=sp,actor=actor,cookie=cookie,removes=0)
  table=gsi+0x800;put(gsi,table);put(table+0x120,0x200100);put(table+0x1b0,0x200200);put(owner+4,gsi)
  cpu.reg_write(x.UC_X86_REG_ESI,owner);cpu.reg_write(x.UC_X86_REG_ESP,sp)
  cpu.emu_start(0xecb1bc,0xecb1d1,count=100);assert cpu.reg_read(x.UC_X86_REG_ESP)==sp
  cpu.reg_write(x.UC_X86_REG_EBX,0);cpu.emu_start(0xecb1f0,0xecb203,count=100);assert cpu.reg_read(x.UC_X86_REG_ESP)==sp
  cpu.reg_write(x.UC_X86_REG_EDI,1);cpu.emu_start(0xecb4e8,0xecb4fb,count=100);assert cpu.reg_read(x.UC_X86_REG_ESP)==sp and state['removes']==2
 # EBX initialized zero; purchase EDI is the answered-yes value and is not overwritten before removal.
 from capstone import Cs,CS_ARCH_X86,CS_MODE_32
 assert rd.bytes_at(0xecb136,2)==bytes.fromhex('33db')
 ins=list(Cs(CS_ARCH_X86,CS_MODE_32).disasm(rd.bytes_at(0xecb487,0xecb4fb-0xecb487),0xecb487))
 assert ins[0].mnemonic=='cmp' and ins[0].op_str=='edi, 1' and ins[1].mnemonic=='jne'
 assert not any(i.mnemonic in ('mov','pop','xor','lea') and i.op_str.startswith('edi,') for i in ins[2:])

@pytest.mark.parametrize('style',('draft','readable'))
@pytest.mark.parametrize('fault',('actor','immediate','world','swapped'))
def test_bad_removal_is_detected(generated,style,fault):
 source=generated[style]
 import re
 calls=list(re.finditer(r'quest:RemoveThing\(([^,]+), false, true\)',source))
 assert len(calls)==3
 actor=calls[0].group(1)
 bad={'actor':'1, false, true','immediate':actor+', true, true','world':actor+', false, false','swapped':actor+', true, false'}[fault]
 source=source[:calls[1].start()]+('quest:RemoveThing('+bad+')')+source[calls[1].end():]
 lua=LuaRuntime();lua.execute(source)
 with pytest.raises(Exception,match='wrong removal'):lua.execute(H)(False,1,1500,True,0,0,31)
