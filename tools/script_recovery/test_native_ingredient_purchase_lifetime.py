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
H="return function(bought,answer,gold,health,delay,tasks,handle,limit,cancel,repeats,acquireWait)\n local me,hero,ingredient={},{},{};local q,r={},{}\n local held,live,paused,registered=false,false,false,false\n local frame,starts,ends,removes,grants,paid,morality,clears,questions,polls=0,0,0,0,0,0,0,0,0,0\n local seeded=false;local fields={};local attempts=0;local speeches=0;local trace={};local remaining=0;local finished=false\n local function effect(s)trace[#trace+1]=s end\n function q:GetHero()return hero end\n function q:GetStateBool(k)assert(k=='OwnerAlive');return true end\n function q:PersistTransferBool(c,k,v)if not seeded then return k=='BoughtSpecialStuff' and bought or false end;fields[k]=v;return v end\n function q:RetailResources()return r end\n function q:NewScriptFrame(a)assert(a==me);frame=frame+1;assert(frame<40);return not finished and (limit==0 or frame<limit) end\n function q:IsActiveThreadTerminating()return finished or limit>0 and frame>=limit end\n function q:GetThingWithScriptName(k)assert(k=='Ingredient');return ingredient end\n function q:RemoveThing(a,b,c)assert(a==ingredient,'wrong removal actor');assert(b==false and c==true,'wrong removal flags');removes=removes+1;effect('remove') end\n function q:SetThingHasInformation(a,b,c,d)assert(a==me and not b and c and not d) end\n function q:SetIsPushableByHero(a,b)assert(a==me and not b)end\n function q:SetIsThingForcePushable(a,b)assert(a==me and not b)end\n function q:RegisterTimer()assert(not registered);registered=true;return 713 end\n function q:DeregisterTimer(id)assert(id==713 and registered);registered=false end\n function q:GetHealth(a)assert(a==me);return health and 1 or 0 end\n function q:PauseAllNonScriptedEntities(b)assert(live and paused~=b);paused=b end\n function q:GiveHeroYesNoQuestion(k,y,n,e,b)assert(k=='TEXT_QST_B10_TRADER_BUY_MUSHROOM_QUESTION' and y=='TEXT_OBJECT_HERO_ANSWER_YES' and n=='TEXT_OBJECT_HERO_ANSWER_NO' and e=='' and b);questions=questions+1 end\n function q:MsgIsQuestionAnsweredYesOrNo()polls=polls+1;if polls<=delay then return -1 end;return answer end\n function q:GetHeroGold()return gold end\n function q:ReadGlobalGameDataString(offset)assert(offset==0x738);return 'SPECIAL_MUSHROOM' end\n function q:GiveHeroObject(def,n,b)OnPersist(q,me,{});assert(not fields.BoughtSpecialStuff and not fields.StoleSpecialStuff);assert(def=='SPECIAL_MUSHROOM' and n==-1 and not b and live and paused);grants=grants+1;effect('grant')end\n function q:GiveHeroGold(n)assert(n==-1500 and removes==1);paid=paid+1;effect('heroGold')end\n function q:EntityGiveGold(a,n)assert(a==me and n==1500 and paid==1);effect('ownerGold')end\n function q:ReadGlobalGameDataFloat(n)assert(n==0x770);return 2.25 end\n function q:GiveHeroMorality(n)OnPersist(q,me,{});assert(fields.BoughtSpecialStuff and not fields.StoleSpecialStuff);assert(n==2.25 and paid==1);morality=morality+1;effect('morality')end\n function q:ClearThingHasInformation(a)assert(a==me and morality==1);clears=clears+1;effect('clear')end\n function r:NewResource()assert(not held);held=true;return 21 end\n function r:PrepareResource(id)assert(id==21 and held)end\n function r:TryAcquire(id,a,k)assert(id==21 and a==me and k==4);attempts=attempts+1;return attempts>acquireWait end\n function r:ReleaseResource(id)assert(id==21 and held and not live and not paused and not registered);held=false end\n function r:ResourceThingHealthAbove(id,minimum)assert(id==21 and held and minimum==0.0);return health end\n function r:ScriptThing(id)assert(id==21);return me end\n function r:StartMovie(k)assert(k=='' and held and not live);live=true;starts=starts+1;return handle end\n function r:DestroyMovie(id)assert(id==handle and live and not paused);live=false;ends=ends+1;finished=true end\n function me:IsTalkedToByHero()assert(starts==0);return true end\n function ingredient:IsAlive()return true end\n function me:Speak(a,k,g,b,c,d)assert(a==hero and g==0 and not b and c and not d);remaining=tasks;speeches=speeches+1;effect(k);return not (cancel==1 and k=='TEXT_QST_B10_TRADER_INTRO' or cancel==2 and k=='TEXT_QST_B10_TRADER_BOUGHT_MUSHROOM_10') end\n function me:IsPerformingScriptTask()if remaining>0 then remaining=remaining-1;return true end;return false end\n\n Init(q,me);OnPersist(q,me,{});seeded=true\n local result={}\n for run=1,repeats do\n  local entryBought=run==1 and bought or fields.BoughtSpecialStuff\n  held,live,paused,registered=false,false,false,false\n  frame,starts,ends,removes,grants,paid,morality,clears,questions,polls=0,0,0,0,0,0,0,0,0,0\n  finished=false;remaining=0;attempts=0;speeches=0;trace={}\n  Main(q,me);OnPersist(q,me,{})\n  assert(not held and not live and not paused and not registered and starts==ends and starts<=1,'leaked lifetime')\n  assert(grants<=1 and paid==grants and clears==morality and morality<=grants)\n  assert(fields.BoughtSpecialStuff==(entryBought or morality==1) and not fields.StoleSpecialStuff,'wrong interrupted state')\n  if entryBought then assert(grants==0 and questions==0) end\n  if grants==1 then\n   assert(not entryBought and answer==1 and gold>=1500 and removes==1)\n   local positions={};for i,v in ipairs(trace)do positions[v]=i end\n   assert(positions.grant<positions.remove and positions.remove<positions.heroGold and positions.heroGold<positions.ownerGold)\n   if morality==1 then assert(positions.ownerGold<positions.morality and positions.morality<positions.clear) end\n   if health and cancel==2 then assert(morality==0 and not fields.BoughtSpecialStuff,'canceled outro marked bought') end\n  end\n  if health and cancel==1 and not entryBought then assert(grants==0 and questions==0) end\n  if limit==0 and (cancel==0 or not health or entryBought) then\n   local purchase=not entryBought and answer==1 and gold>=1500\n   assert(starts==1 and finished and grants==(purchase and 1 or 0) and morality==grants)\n   assert(removes==((entryBought or purchase) and 1 or 0))\n  end\n  result[#result+1]=table.concat(trace,'|')..':'..frame..':'..starts..':'..ends..':'..removes..':'..grants..':'..morality..':'..tostring(fields.BoughtSpecialStuff)..':'..polls..':'..attempts\n end\n return table.concat(result,'\\n')\nend"
CASES=list(itertools.product((False,True),(0,1),(1499,1500),(False,True),(0,2),(0,2),(0,31,713),(0,1,2,3,5,8),(0,1,2),(1,2),(0,1)))
@pytest.mark.parametrize('case',CASES)
def test_purchase_interruption_and_next_main(generated,case):
 traces=[]
 for source in generated.values():
  lua=LuaRuntime();lua.execute(source);traces.append(lua.execute(H)(*case))
 assert traces[0]==traces[1]

def test_native_payment_and_post_dialogue_state_gate():
 from tools.script_recovery.lift_native_lua import RData
 from tools.script_recovery.convert_quest_unit import UnitConverter,callee_stack_words
 from tools.script_recovery.annotate_interface_slots import load_slots
 import unicorn as u
 from unicorn import x86_const as x
 rd=RData();slots={0x1f8:('GiveHeroGold',1),0x820:('EntityGiveGold',2),0x270:('GiveHeroMorality',1),0x5a4:('ClearThingHasInformation',1),0x1c:('NewScriptFrame',0)}
 for slot,(name,n) in slots.items():
  assert load_slots()[slot]==name
  target=int.from_bytes(rd.bytes_at(0x1260f0c+slot,4),'little');assert callee_stack_words(target)==n
 cpu=u.Uc(u.UC_ARCH_X86,u.UC_MODE_32)
 for a,n in ((0xecb000,0x2000),(0x300000,0x10000),(0x500000,0x10000),(0x200000,0x1000),(0x143e000,0x1000),(0x7e7000,0x1000),(0xf35000,0x1000)):cpu.mem_map(a,n)
 fn=UnitConverter(ROOT/'refs/script_recovery/sick_child/translation_unit_typed.json').native('0x00ecb0e0');a=int(fn['address'],16);cpu.mem_write(a,rd.bytes_at(a,int(fn['bodyEndExclusive'],16)-a))
 def put(a,v):cpu.mem_write(a,struct.pack('<I',v&0xffffffff))
 def word(a):return struct.unpack('<I',cpu.mem_read(a,4))[0]
 endpoints={0x200100+i*0x10:slot for i,slot in enumerate(slots)}
 for a in (*endpoints,0x7e7450,0xf35b30):cpu.mem_write(a,b'\xc3')
 state={};layouts=0
 def hook(m,a,size,user):
  if a in (0xecb793,0xecb6c3,0xecbbc9):state['exit']=a;m.emu_stop();return
  sp=m.reg_read(x.UC_X86_REG_ESP)
  if a in endpoints:
   slot=endpoints[a];assert m.reg_read(x.UC_X86_REG_ECX)==state['gsi'];purge=slots[slot][1]*4
   if slot==0x1f8:assert word(sp+4)==0xfffffa24;state['trace'].append('heroGold')
   elif slot==0x820:assert word(sp+4)==state['owner']+8 and word(sp+8)==1500;state['trace'].append('ownerGold')
   elif slot==0x270:
    assert cpu.mem_read(state['owner']+0x1c,1)==b'\x01' and word(sp+4)==state['float'];state['trace'].append('morality')
   elif slot==0x5a4:assert word(sp+4)==state['owner']+8;state['trace'].append('clear')
   else:state['frames']+=1
  elif a==0x7e7450:
   assert m.reg_read(x.UC_X86_REG_ECX)==state['stack']+0x14;purge=0
   m.reg_write(x.UC_X86_REG_EAX,int(state['tasks']>0));state['tasks']=max(0,state['tasks']-1)
  elif a==0xf35b30:
   assert m.reg_read(x.UC_X86_REG_ECX)==state['owner'];purge=0
   m.reg_write(x.UC_X86_REG_EAX,int(state['deadline']>=0 and state['frames']>=state['deadline']))
  else:return
  m.reg_write(x.UC_X86_REG_EIP,word(sp));m.reg_write(x.UC_X86_REG_ESP,sp+4+purge)
 cpu.hook_add(u.UC_HOOK_CODE,hook)
 for owner,gsi,sp,health,tasks,deadline,bits in itertools.product((0x503000,0x504000),(0x500000,0x501000),(0x300800,0x301800),(False,True),(0,1,3),(-1,0,1,2,4),(0x3f800000,0xbfa00000,0x40a80000)):
  state=dict(owner=owner,gsi=gsi,stack=sp,tasks=tasks,deadline=deadline,float=bits,frames=0,trace=[],exit=None)
  table=gsi+0x800;put(gsi,table);put(owner+4,gsi);put(0x143e90c,0x507000);put(0x507770,bits);cpu.mem_write(owner+0x1b,b'\x55\x00\x66\x77')
  for a,slot in endpoints.items():put(table+slot,a)
  cpu.reg_write(x.UC_X86_REG_ESI,owner);cpu.reg_write(x.UC_X86_REG_ESP,sp)
  cpu.emu_start(0xecb4fb,0xecb51f,count=100);assert state['trace']==['heroGold','ownerGold'] and cpu.reg_read(x.UC_X86_REG_ESP)==sp
  cpu.emu_start(0xecb584 if health else 0xecb5c4,0xecb794,count=1000)
  expected=not health or deadline<0 or min(tasks,deadline)<deadline
  assert state['trace']==['heroGold','ownerGold']+(['morality','clear'] if expected else [])
  assert cpu.mem_read(owner+0x1b,4)==bytes([0x55,int(expected),0x66,0x77])
  assert cpu.reg_read(x.UC_X86_REG_ESP)==sp and state['exit']==(0xecb793 if expected else 0xecb6c3 if tasks>0 and tasks>=max(1,deadline) else 0xecbbc9)
  layouts+=1
 assert layouts==720

def test_native_owner_alive_predicate_full_callback():
 from tools.script_recovery.lift_native_lua import RData
 from tools.script_recovery.convert_quest_unit import UnitConverter,callee_stack_words
 from tools.script_recovery.annotate_interface_slots import load_thing_slots
 import unicorn as u
 from unicorn import x86_const as x
 rd=RData();target=int.from_bytes(rd.bytes_at(0x1238c8c+0x48,4),'little')
 assert load_thing_slots()[0x48][0]=='MsgIsKilledBy' and callee_stack_words(target)==1
 cpu=u.Uc(u.UC_ARCH_X86,u.UC_MODE_32)
 for a,n in ((0xecb000,0x1000),(0x99e000,0x1000),(0x300000,0x10000),(0x500000,0x10000),(0x200000,0x1000)):cpu.mem_map(a,n)
 fn=UnitConverter(ROOT/'refs/script_recovery/sick_child/translation_unit_typed.json').native('0x00ecb080');a=int(fn['address'],16);cpu.mem_write(a,rd.bytes_at(a,int(fn['bodyEndExclusive'],16)-a))
 for a in (0x99ebf0,0x99eae0,0x200100):cpu.mem_write(a,b'\xc3')
 def put(a,v):cpu.mem_write(a,struct.pack('<I',v&0xffffffff))
 def word(a):return struct.unpack('<I',cpu.mem_read(a,4))[0]
 state={};layouts=0
 def hook(m,a,size,user):
  if a==0x200800:m.emu_stop();return
  if a not in (0x99ebf0,0x99eae0,0x200100):return
  sp=m.reg_read(x.UC_X86_REG_ESP);receiver=m.reg_read(x.UC_X86_REG_ECX)
  if a==0x99ebf0:
   assert receiver==state['stack']-4 and word(sp+4)==0x122d70e and word(sp+8)==0xffffffff
   assert rd.bytes_at(0x122d70e,1)==b'\0';put(receiver,713);state['ctors']+=1;purge=8
  elif a==0x99eae0:assert receiver==state['stack']-4 and word(receiver)==713;state['dtors']+=1;purge=0
  else:
   assert receiver==state['owner']+8 and word(sp+4)==state['stack']-4 and word(word(sp+4))==713
   m.reg_write(x.UC_X86_REG_EAX,state['killed']);state['queries']+=1;purge=4
  m.reg_write(x.UC_X86_REG_EIP,word(sp));m.reg_write(x.UC_X86_REG_ESP,sp+4+purge)
 cpu.hook_add(u.UC_HOOK_CODE,hook)
 for owner,quest,sp,alive,killed in itertools.product((0x500000,0x501000),(0x502000,0x503000),(0x300800,0x301800),(0,1,255),(0,1,255)):
  state=dict(owner=owner,quest=quest,stack=sp,killed=killed,ctors=0,dtors=0,queries=0)
  put(owner+8,0x505000);put(0x505000+0x48,0x200100);put(owner+0x14,quest);cpu.mem_write(quest+0x6f,bytes([0x55,alive,0x66]));cpu.mem_write(owner+0x1c,b'\xaa\xbb')
  put(sp,0x200800);cpu.reg_write(x.UC_X86_REG_ECX,owner);cpu.reg_write(x.UC_X86_REG_ESP,sp);cpu.reg_write(x.UC_X86_REG_EBX,0x12345678);cpu.reg_write(x.UC_X86_REG_ESI,0x76543210)
  cpu.emu_start(0xecb080,0x200801,count=100)
  assert cpu.mem_read(quest+0x6f,3)==bytes([0x55,0 if killed else alive,0x66]) and cpu.mem_read(owner+0x1c,2)==b'\xaa\xbb'
  assert state['ctors']==state['dtors']==state['queries']==1 and cpu.reg_read(x.UC_X86_REG_ESP)==sp+4
  assert cpu.reg_read(x.UC_X86_REG_EBX)==0x12345678 and cpu.reg_read(x.UC_X86_REG_ESI)==0x76543210
  layouts+=1
 assert layouts==72

PREDICATE_H=r"""return function(alive,bought,stole,killed,repeats)
 local me,hero,q={},{},{};local fields={};local seed=false;local removes,queries,writes=0,0,0
 function q:GetStateBool(key)assert(key=='OwnerAlive');return alive end
 function q:RemoveThing(actor,a,b)assert(actor==me and not a and b);removes=removes+1 end
 function q:PersistTransferBool(context,key,value)
  if not seed then if key=='BoughtSpecialStuff' then return bought else assert(key=='StoleSpecialStuff');return stole end end
  fields[key]=value;return value
 end
 function q:SetStateBool(key,value)assert(key=='OwnerAlive' and value==false);writes=writes+1;alive=false end
 function me:MsgIsKilledBy(name)assert(name=='');queries=queries+1;return killed end
 Init(q,me);OnPersist(q,me,{});seed=true
 assert(removes==(alive and 0 or 1))
 for i=1,repeats do OnPredicateFail(q,me);OnPersist(q,me,{});assert(fields.BoughtSpecialStuff==bought and fields.StoleSpecialStuff==stole)end
 assert(queries==repeats and writes==(killed and repeats or 0))
 if killed then assert(not alive)end
 return alive,queries,writes,removes,fields.BoughtSpecialStuff,fields.StoleSpecialStuff
end"""
@pytest.mark.parametrize('case',list(itertools.product((False,True),(False,True),(False,True),(False,True),(1,2,3))))
def test_owner_alive_callback_and_entity_fields(generated,case):
 traces=[]
 for source in generated.values():
  lua=LuaRuntime();lua.execute(source);traces.append(lua.execute(PREDICATE_H)(*case))
 assert traces[0]==traces[1]

@pytest.mark.parametrize('style',('draft','readable'))
@pytest.mark.parametrize('fault',('early_bought','debit','credit','morality','omit_clear','pause','movie','persist'))
def test_wrong_purchase_state_or_lifetime_is_detected(generated,style,fault):
 s=generated[style]
 if fault=='early_bought':
  if style=='draft':assignment='__native_entity_state:SetStateBool("BoughtSpecialStuff", true)'
  else:
   import re
   targets=re.findall(r'(?m)^\s*(\w+)\s*=\s*quest:PersistTransferBool\(context,\s*"BoughtSpecialStuff",',s)
   assert len(targets)==1
   assignment=targets[0]+' = true'
  target=s.index('quest:GiveHeroObject(');s=s[:target]+assignment+'; '+s[target:]
 elif fault=='debit':s=s.replace('quest:GiveHeroGold(-0x5dc)','quest:GiveHeroGold(-1)').replace('quest:GiveHeroGold(-1500)','quest:GiveHeroGold(-1)')
 elif fault=='credit':s=s.replace('quest:EntityGiveGold(me, 0x5dc)','quest:EntityGiveGold(me, 1499)').replace('quest:EntityGiveGold(me, 1500)','quest:EntityGiveGold(me, 1499)')
 elif fault=='morality':s=s.replace('quest:ReadGlobalGameDataFloat(0x770)','quest:ReadGlobalGameDataFloat(0x76c)').replace('quest:ReadGlobalGameDataFloat(1904)','quest:ReadGlobalGameDataFloat(1900)').replace('quest:ReadGlobalGameDataFloat(SCRIPT_DEF.BoughtMushroomInBarrowFields)','quest:ReadGlobalGameDataFloat(SCRIPT_DEF.StoleMushroomInBarrowFields)')
 elif fault=='omit_clear':s=s.replace('quest:ClearThingHasInformation(me)','do end',1)
 elif fault=='pause':s=s.replace('quest:PauseAllNonScriptedEntities(false)','quest:PauseAllNonScriptedEntities(true)')
 elif fault=='movie':
  import re
  s=re.sub(r'resources:DestroyMovie\([^)]*\)','resources:DestroyMovie(-1)',s)
 else:s=s.replace('"BoughtSpecialStuff", boughtSpecialStuff','"StoleSpecialStuff", boughtSpecialStuff')
 assert s!=generated[style]
 lua=LuaRuntime();lua.execute(s)
 with pytest.raises(Exception):lua.execute(H)(False,1,1500,True,0,0,31,0,0,1,0)
