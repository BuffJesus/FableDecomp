import itertools,sys,struct
from pathlib import Path
import pytest
from lupa.lua54 import LuaRuntime
ROOT=next(p for p in Path(__file__).resolve().parents if (p/'tools/script_recovery/script_units.py').is_file());sys.path.insert(0,str(ROOT))
from tools.script_recovery.test_native_mans_letter_latch import generated
H='return function(fromAnswer,health,wait,deadline,handle,pattern,moveWait,savedStart)\n local mode,initial,inventory,cancel="presented",savedStart and (fromAnswer==0 and 2 or 1) or 0,true,""\n local me,hero={},{};local q,r={},{}\n local frame,attempts,starts,ends,questions,polls,speeches,takes,gifts,morality,clears,reads=0,0,0,0,0,0,0,0,0,0,0,0\n local held,live,paused,camera,finished=false,false,false,false,false\n local state=initial;local saved={};local trace={};local initialLetter,latestLetter,latestReward=nil,nil,nil\n local remaining=0;local questionKey=nil;local movieEnded=savedStart;local loading=true;local moves,fades,lookups=0,0,0;local marker=nil;local distances=({below={1.999},exact={2},step={2.001,2},multi={37,3,2}})[pattern]\n local function log(k)trace[#trace+1]=k end\n local function terminating()return finished or deadline>0 and frame>=deadline end\n function q:GetHero()return hero end\n function q:RetailResources()return r end\n function q:SetThingPersistent(a,b)assert(a==me and b)end\n function q:PersistTransferBool(c,k,v)assert(k==\'LeavingRegion\');if loading then v=savedStart end;saved[k]=v;return v end\n function q:GetStateInt(k)assert(k==\'MansLoverState\');return state end\n function q:SetStateInt(k,v)assert(k==\'MansLoverState\' and morality==1 and v==(fromAnswer==0 and 2 or 1));state=v;log(\'state:\'..v)end\n function q:NewScriptFrame(a)assert(a==me);frame=frame+1;assert(frame<40);return not terminating()end\n function q:IsActiveThreadTerminating()return terminating()end\n function q:SetThingHasInformation(a,b,c,d)assert(a==me and b and c and not d)end\n function q:ReadGlobalGameDataString(offset)\n  reads=reads+1\n  if offset==0x73c then latestLetter=\'LETTER_\'..reads;if initialLetter==nil then initialLetter=latestLetter end;return latestLetter end\n  assert(offset==0x740);latestReward=\'MAN_REPLY_\'..reads;return latestReward\n end\n function q:IsObjectInThingsPossession(item,a)assert(a==hero and item==latestLetter and item~=initialLetter);return inventory end\n function q:GetHealth(a)assert(a==me);return health and 1 or 0 end\n function q:PauseAllNonScriptedEntities(b)assert(live and paused~=b);paused=b;log(b and \'pause\' or \'resume\')end\n function q:FixMovieSequenceCamera(b)assert(live and paused);camera=b;log(b and \'cameraFix\' or \'cameraFree\')end\n function q:EntitySetFacingAngleTowardsThing(a,b,c)assert(a==me and b==hero and type(c)==\'boolean\')end\n function q:Pause(t)assert(t==1)end\n function q:CameraUseCameraPoint(k,a,p,m,d)assert(a==me and live and paused and p==-1 and m==0 and d==-1 and (k==\'CAM_REL_WOMAN\' or k==\'CAM_REL_WOMANHERO\'));log(k)end\n function q:GiveHeroYesNoQuestion(k,y,n,e,b)\n  assert(y==\'TEXT_OBJECT_HERO_ANSWER_YES\' and n==\'TEXT_OBJECT_HERO_ANSWER_NO\' and e==\'\' and b and live and paused)\n  assert(k==\'TEXT_QST_B10_MANS_LOVER_GIVE_LETTER_QUESTION\' or k==\'TEXT_QST_B10_MANS_LOVER_LETTER_FROM_QUESTION\');questionKey=k;questions=questions+1;polls=0;log(k)\n end\n function q:MsgIsQuestionAnsweredYesOrNo()\n  polls=polls+1;if polls<=wait then return -1 end\n  if questionKey==\'TEXT_QST_B10_MANS_LOVER_GIVE_LETTER_QUESTION\' then return mode==\'talk-no\' and 0 or 1 end\n  return fromAnswer\n end\n function q:TakeObjectFromHero(item)assert(item==latestLetter and item~=initialLetter and live and paused and takes==0);takes=1;log(\'take:\'..item)end\n function q:GiveHeroObject(item,count,b)\n  assert(takes==1 and gifts==0 and count==-1 and not b and live and paused)\n  assert(item==(fromAnswer==0 and latestReward or \'OBJECT_SICK_CHILD_WOMANS_HERO_LETTER\'));gifts=1;log(\'gift:\'..item)\n end\n function q:ReadGlobalGameDataFloat(offset)assert(offset==(fromAnswer==0 and 0x758 or 0x75c));return fromAnswer==0 and 1.25 or -2.5 end\n function q:GiveHeroMorality(value)assert(gifts==1 and morality==0 and state==initial and value==(fromAnswer==0 and 1.25 or -2.5));morality=1;log(\'morality\')end\n function q:ClearThingHasInformation(a)OnPersist(q,me,{});assert(a==me and morality==1 and not saved.LeavingRegion);clears=1;log(\'clear\')end\n function r:NewResource()assert(not held);held=true;return 21 end\n function r:PrepareResource(id)assert(id==21 and held)end\n function r:TryAcquire(id,a,k)assert(id==21 and a==me and k==4);attempts=attempts+1;return attempts>wait end\n function r:ReleaseResource(id)assert(id==21 and held and not live and not paused);held=false end\n function r:ResourceThingHealthAbove(value,minimum) assert(minimum==0.0);return q:GetHealth(self:ScriptThing(value))>minimum end\n function r:ScriptThing(id)assert(id==21 and held);return me end\n function r:StartMovie(k)assert(k==\'\' and held and not live);starts=starts+1;live=true;log(\'start\');return handle end\n function r:DestroyMovie(id)assert(id==handle and live and not paused);live=false;ends=ends+1;movieEnded=true;if clears==0 then finished=true end;log(\'end\')end\n function me:MsgIsPresentedWithItem()if not movieEnded then g_PresentedItemName=initialLetter;return true end;return false end\n function me:IsTalkedToByHero()return false end\n function me:ClearCommands()assert(live and paused);log(\'clearCommands\')end\n function me:Speak(a,k,g,b,c,d)\n  assert(a==hero and g==0 and not b and c and not d and k:find(\'TEXT_QST_B10_MANS_LOVER_\',1,true)==1);speeches=speeches+1;remaining=wait;log(k);return k~=cancel\n end\n function me:IsPerformingScriptTask()if remaining>0 then remaining=remaining-1;return true end;return false end\n\n function q:GetThingWithScriptName(key)\n  assert(key==\'MansLoverLeavesHere\' and not live and held);lookups=lookups+1;assert(lookups<=#distances);marker={sequence=lookups};log(\'marker:\'..lookups);return marker\n end\n function q:IsDistanceBetweenThingsOver(actor,target,radius)assert(actor==me and target==marker and radius==2);return distances[lookups]>2 end\n function me:MoveToThing(target,speed,kind)\n  assert(target==marker and speed==1 and kind==0 and not live and held and distances[lookups]>2);moves=moves+1;remaining=moveWait;log(\'move:\'..lookups)\n end\n function q:FadeOutAndKillEntity(actor,a,seconds,b)\n  assert(actor==me and a and seconds==1.5 and b and not live and held and distances[lookups]<=2);fades=fades+1;finished=true;log(\'fade\')\n end\n Init(q,me);OnPersist(q,me,{});loading=false;Main(q,me);OnPersist(q,me,{})\n assert(not held and not live and not paused and starts==ends and starts<=1)\n assert(takes<=1 and gifts<=takes and morality<=gifts and clears==morality and saved.LeavingRegion==(savedStart or clears==1))\n assert(state==(morality==1 and (fromAnswer==0 and 2 or 1) or initial))\n if fromAnswer==0 then assert(morality==gifts,\'man reply grant separated from morality\')end\n if not inventory or mode==\'talk-no\' then assert(takes==0 and gifts==0 and questions<=1)end\n if gifts==1 then assert(fromAnswer~=0 or latestReward~=nil)end\n\n assert(moves<=#distances-1 and fades<=1 and lookups<=moves+fades+1)\n if not saved.LeavingRegion then assert(moves==0 and fades==0 and lookups==0)end\n if savedStart then assert(starts==0 and takes==0 and gifts==0 and morality==0 and questions==0)end\n if deadline==0 then\n  assert(finished and saved.LeavingRegion and moves==#distances-1 and fades==1 and lookups==#distances)\n  assert(takes==(savedStart and 0 or 1) and gifts==takes and morality==gifts)\n end\n return table.concat(trace,\'|\'),frame,attempts,starts,ends,questions,polls,speeches,takes,gifts,morality,clears,reads,state,saved.LeavingRegion,camera,moves,fades,lookups\nend'
CASES=list(itertools.product((0,1),(False,True),(0,2),(0,1,2,3,5,9),(0,31,713),('below','exact','step','multi'),(0,2),(False,True)))
@pytest.mark.parametrize('case',CASES)
def test_real_handin_or_saved_leaving_through_departure(generated,case):
 results=[]
 for source in generated.values():
  lua=LuaRuntime();lua.execute(source);results.append(lua.execute(H)(*case))
 assert results[0]==results[1]

import itertools,struct,sys
from pathlib import Path
ROOT=next(p for p in Path(__file__).resolve().parents if (p/'tools/script_recovery/script_units.py').is_file());sys.path.insert(0,str(ROOT))

def test_native_departure_gate_operands_and_cleanup():
 from tools.script_recovery.lift_native_lua import RData
 from tools.script_recovery.convert_quest_unit import callee_stack_words
 from tools.script_recovery.annotate_interface_slots import load_slots
 import unicorn as u
 from unicorn import x86_const as x
 rd=RData();slots={0x120:('GetThingWithScriptName',2),0x1c:('NewScriptFrame',0)}
 for slot,(name,purge) in slots.items():
  assert load_slots()[slot]==name
  assert callee_stack_words(int.from_bytes(rd.bytes_at(0x1260f0c+slot,4),'little'))==purge
 cpu=u.Uc(u.UC_ARCH_X86,u.UC_MODE_32)
 for a,n in ((0xec9000,0x3000),(0x7e7000,0x1000),(0x99e000,0x1000),(0xf35000,0x1000),(0xcbe000,0x1000),(0x4aa000,0x1000),(0x300000,0x10000),(0x500000,0x10000),(0x200000,0x1000)):cpu.mem_map(a,n)
 cpu.mem_write(0xec9000,rd.bytes_at(0xec9000,0x3000));cpu.mem_write(0x7e7300,rd.bytes_at(0x7e7300,16))
 for a in (0x7e7450,0x99ebf0,0x99eae0,0xf35b30,0xcbe3ea,0xcbe716,0x4aa840,0x200100,0x200110,0x200120):cpu.mem_write(a,b'\xc3')
 def put(a,v):cpu.mem_write(a,struct.pack('<I',v&0xffffffff))
 def word(a):return struct.unpack('<I',cpu.mem_read(a,4))[0]
 state={};layouts=0
 def hook(m,a,size,user):
  if a in (0xecaf0c,0xecaf6e):state['exit']=a;m.emu_stop();return
  if a not in (0x7e7450,0x99ebf0,0x99eae0,0xf35b30,0xcbe3ea,0xcbe716,0x4aa840,0x200100,0x200110,0x200120):return
  sp=m.reg_read(x.UC_X86_REG_ESP);ecx=m.reg_read(x.UC_X86_REG_ECX);s=state['stack'];purge=0
  if a==0x7e7450:
   assert ecx==s+0x1c;state['trace'].append('task');m.reg_write(x.UC_X86_REG_EAX,state['task'])
  elif a==0xf35b30:
   assert ecx==state['owner'];state['terms']+=1;m.reg_write(x.UC_X86_REG_EAX,int(state['termAt']==state['terms']))
  elif a==0x99ebf0:
   assert ecx==s+0x48 and rd.string_at(word(sp+4))=='MansLoverLeavesHere' and word(sp+8)==0xffffffff
   put(ecx,state['cookie']);state['trace'].append('name');purge=8
  elif a==0x200100:
   assert ecx==state['gsi'] and word(sp+4)==s+0x94 and word(sp+8)==s+0x48 and word(s+0x48)==state['cookie']
   put(s+0x94,state['cookie']+1);m.reg_write(x.UC_X86_REG_EAX,s+0x94);state['trace'].append('get');purge=8
  elif a==0x99eae0:
   assert ecx==s+0x48 and word(ecx)==state['cookie'];state['trace'].append('nameDtor')
  elif a==0xcbe3ea:
   assert ecx==state['owner']+8 and m.reg_read(x.UC_X86_REG_EDX)==s+0x94 and word(s+0x94)==state['cookie']+1 and word(sp+4)==0x40000000
   m.reg_write(x.UC_X86_REG_EAX,state['distance']);state['trace'].append('distance');purge=4
  elif a==0x200120:
   assert ecx==0x508000 and [word(sp+i*4) for i in range(1,8)]==[s+0x94,0x3f800000,0,0,1,0,1]
   assert word(s+0x94)==state['cookie']+1;state['trace'].append('move');purge=28
  elif a==0xcbe716:
   assert ecx==state['owner']+8 and m.reg_read(x.UC_X86_REG_EDX)&255==1 and word(sp+4)==0x3fc00000 and word(sp+8)==1
   state['trace'].append('fade');purge=8
  elif a==0x4aa840:
   assert ecx==s+0x94 and word(ecx)==state['cookie']+1;state['trace'].append('markerDtor')
  else:
   assert ecx==state['gsi'];state['trace'].append('frame')
  m.reg_write(x.UC_X86_REG_EIP,word(sp));m.reg_write(x.UC_X86_REG_ESP,sp+4+purge)
 cpu.hook_add(u.UC_HOOK_CODE,hook)
 for leaving,task,distance,termAt,owner,gsi,sp,cookie in itertools.product((0,1,255),(0,1,255),(0,1,255),(0,1,2),(0x501000,0x502000),(0x500000,0x503000),(0x300800,0x301800),(713,0xdead0000)):
  state=dict(owner=owner,gsi=gsi,stack=sp,cookie=cookie,task=task,distance=distance,termAt=termAt,trace=[],terms=0,exit=None)
  put(gsi,gsi+0x800);put(gsi+0x920,0x200100);put(gsi+0x81c,0x200110);put(owner+4,gsi)
  cpu.mem_write(owner+0x1b,bytes([0x55,leaving,0x66]));put(sp+0x24,0x508000);put(0x508000,0x508800);put(0x508814,0x200120)
  cpu.reg_write(x.UC_X86_REG_ESI,owner);cpu.reg_write(x.UC_X86_REG_EBP,owner+8);cpu.reg_write(x.UC_X86_REG_ESP,sp)
  cpu.emu_start(0xecae38,0xecaf6f,count=200)
  eligible=bool(leaving and not task)
  expected=[] if not leaving else ['task']
  if eligible and termAt!=1:expected+=['name','get','nameDtor','distance']
  if eligible and termAt==0:expected+=['move' if distance else 'fade','markerDtor']
  if eligible and termAt==2:expected+=['markerDtor']
  if not eligible or termAt==0:expected+=['frame']
  assert state['trace']==expected
  assert state['terms']==(0 if not eligible else 1 if termAt==1 else 2)
  assert state['exit']==(0xecaf0c if not eligible or termAt==0 else 0xecaf6e)
  assert cpu.reg_read(x.UC_X86_REG_ESP)==sp and cpu.mem_read(owner+0x1b,3)==bytes([0x55,leaving,0x66])
  layouts+=1
 assert layouts==1296

def test_native_mans_death_full_callback():
 from tools.script_recovery.lift_native_lua import RData
 from tools.script_recovery.convert_quest_unit import UnitConverter,callee_stack_words
 from tools.script_recovery.annotate_interface_slots import load_thing_slots
 import unicorn as u
 from unicorn import x86_const as x
 rd=RData();assert load_thing_slots()[0x48][0]=='MsgIsKilledBy'
 assert callee_stack_words(int.from_bytes(rd.bytes_at(0x1238c8c+0x48,4),'little'))==1
 cpu=u.Uc(u.UC_ARCH_X86,u.UC_MODE_32)
 for a,n in ((0xec9000,0x1000),(0x99e000,0x1000),(0x300000,0x10000),(0x500000,0x10000),(0x200000,0x1000)):cpu.mem_map(a,n)
 fn=UnitConverter(ROOT/'refs/script_recovery/sick_child/translation_unit_typed.json').native('0x00ec9cd0');a=int(fn['address'],16);cpu.mem_write(a,rd.bytes_at(a,int(fn['bodyEndExclusive'],16)-a))
 for a in (0x99ebf0,0x99eae0,0x200100):cpu.mem_write(a,b'\xc3')
 def put(a,v):cpu.mem_write(a,struct.pack('<I',v&0xffffffff))
 def word(a):return struct.unpack('<I',cpu.mem_read(a,4))[0]
 state={};layouts=0
 def hook(m,a,size,user):
  if a==0x200800:m.emu_stop();return
  if a not in (0x99ebf0,0x99eae0,0x200100):return
  sp=m.reg_read(x.UC_X86_REG_ESP);ecx=m.reg_read(x.UC_X86_REG_ECX);purge=0
  if a==0x99ebf0:
   assert ecx==state['stack']-4 and word(sp+4)==0x122d70e and rd.bytes_at(word(sp+4),1)==b'\0' and word(sp+8)==0xffffffff
   put(ecx,713);state['ctors']+=1;purge=8
  elif a==0x99eae0:assert ecx==state['stack']-4 and word(ecx)==713;state['dtors']+=1
  else:
   assert ecx==state['owner']+8 and word(sp+4)==state['stack']-4 and word(word(sp+4))==713
   m.reg_write(x.UC_X86_REG_EAX,state['killed']);state['queries']+=1;purge=4
  m.reg_write(x.UC_X86_REG_EIP,word(sp));m.reg_write(x.UC_X86_REG_ESP,sp+4+purge)
 cpu.hook_add(u.UC_HOOK_CODE,hook)
 for owner,quest,sp,initial,leaving,killed in itertools.product((0x500000,0x501000),(0x502000,0x503000),(0x300800,0x301800),(0,1,2,3),(0,1,255),(0,1,255)):
  state=dict(owner=owner,quest=quest,stack=sp,killed=killed,ctors=0,dtors=0,queries=0)
  put(owner+8,0x505000);put(0x505048,0x200100);put(owner+0x14,quest);put(quest+0x68,0x12345678);put(quest+0x6c,initial);put(quest+0x70,0x76543210)
  cpu.mem_write(owner+0x1b,bytes([0x55,leaving,0x66]));put(sp,0x200800)
  cpu.reg_write(x.UC_X86_REG_ECX,owner);cpu.reg_write(x.UC_X86_REG_ESP,sp);cpu.reg_write(x.UC_X86_REG_EBX,0x12345678);cpu.reg_write(x.UC_X86_REG_ESI,0x76543210)
  cpu.emu_start(0xec9cd0,0x200801,count=100)
  assert word(quest+0x6c)==(3 if killed else initial) and word(quest+0x68)==0x12345678 and word(quest+0x70)==0x76543210
  assert cpu.mem_read(owner+0x1b,3)==bytes([0x55,leaving,0x66]) and state['ctors']==state['dtors']==state['queries']==1
  assert cpu.reg_read(x.UC_X86_REG_ESP)==sp+4 and cpu.reg_read(x.UC_X86_REG_EBX)==0x12345678 and cpu.reg_read(x.UC_X86_REG_ESI)==0x76543210
  layouts+=1
 assert layouts==288

PREDICATE_H=r"""return function(leaving,initial,killed,repeats)
 local me,q={},{};local state=initial;local loading=true;local fields={};local queries,writes=0,0
 function q:SetThingPersistent(a,b)assert(a==me and b)end
 function q:PersistTransferBool(c,k,v)assert(k=='LeavingRegion');if loading then return leaving end;fields[k]=v;return v end
 function q:SetStateInt(k,v)assert(k=='MansLoverState' and v==3);state=v;writes=writes+1 end
 function me:MsgIsKilledBy(name)assert(name=='');queries=queries+1;return killed end
 Init(q,me);OnPersist(q,me,{});loading=false
 for i=1,repeats do OnPredicateFail(q,me);OnPersist(q,me,{});assert(fields.LeavingRegion==leaving and state==(killed and 3 or initial))end
 assert(queries==repeats and writes==(killed and repeats or 0))
 return state,fields.LeavingRegion,queries,writes
end"""
@pytest.mark.parametrize('case',list(itertools.product((False,True),(0,1,2,3),(False,True),(1,2,3))))
def test_predicate_state_and_preserved_leaving(generated,case):
 results=[]
 for s in generated.values():
  lua=LuaRuntime();lua.execute(s);results.append(lua.execute(PREDICATE_H)(*case))
 assert results[0]==results[1]

@pytest.mark.parametrize('style',('draft','readable'))
@pytest.mark.parametrize('fault',('marker','distance','move_speed','move_mode','fade_time','fade_persistent','release','persist','death_state','death_actor'))
def test_wrong_departure_or_death_contract_is_detected(generated,style,fault):
 s=generated[style];target='r14' if style=='draft' else 'mansLoverLeavesHere'
 old,new={
  'marker':('"MansLoverLeavesHere"','"WrongMarker"'),
  'distance':(f'{target}, 2.0',f'{target}, 3.0'),
  'move_speed':(f'{target}, 1.0,',f'{target}, 2.0,'),
  'move_mode':(f'{target}, 1.0, '+('0' if style=='draft' else 'ENTITY_MOVE_WALK'),f'{target}, 1.0, 1'),
  'fade_time':('me, true, 1.5, true','me, true, 2.5, true'),
  'fade_persistent':('me, true, 1.5, true','me, true, 1.5, false'),
  'release':('resources:ReleaseResource('+('xStack_120' if style=='draft' else 'resource')+')','do end'),
  'persist':('"LeavingRegion"','"WrongLeaving"'),
  'death_state':('quest:SetStateInt("MansLoverState", 3)','quest:SetStateInt("MansLoverState", 2)'),
  'death_actor':('me:MsgIsKilledBy("")','me:MsgIsKilledBy("hero")'),
 }[fault]
 assert old in s;s=s.replace(old,new);assert s!=generated[style]
 lua=LuaRuntime();lua.execute(s)
 with pytest.raises(Exception):
  if fault.startswith('death'):lua.execute(PREDICATE_H)(True,0,True,2)
  else:lua.execute(H)(0,True,0,0,31,'multi',2,True)
