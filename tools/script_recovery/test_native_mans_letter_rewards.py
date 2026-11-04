import itertools,sys
from pathlib import Path
import pytest
from lupa.lua54 import LuaRuntime
ROOT=next(p for p in Path(__file__).resolve().parents if (p/'tools/script_recovery/script_units.py').is_file());sys.path.insert(0,str(ROOT))
from tools.script_recovery.test_native_mans_letter_latch import generated
H=r'''return function(mode,initial,fromAnswer,inventory,health,wait,deadline,cancel,handle)
 local me,hero={},{};local q,r={},{}
 local frame,attempts,starts,ends,questions,polls,speeches,takes,gifts,morality,clears,reads=0,0,0,0,0,0,0,0,0,0,0,0
 local held,live,paused,camera,finished=false,false,false,false,false
 local state=initial;local saved={};local trace={};local initialLetter,latestLetter,latestReward=nil,nil,nil
 local remaining=0;local questionKey=nil
 local function log(k)trace[#trace+1]=k end
 local function terminating()return finished or deadline>0 and frame>=deadline end
 function q:GetHero()return hero end
 function q:RetailResources()return r end
 function q:SetThingPersistent(a,b)assert(a==me and b)end
 function q:PersistTransferBool(c,k,v)assert(k=='LeavingRegion');saved[k]=v;return v end
 function q:GetStateInt(k)assert(k=='MansLoverState');return state end
 function q:SetStateInt(k,v)assert(k=='MansLoverState' and morality==1 and v==(fromAnswer==0 and 2 or 1));state=v;log('state:'..v)end
 function q:NewScriptFrame(a)assert(a==me);frame=frame+1;assert(frame<40);return not terminating()end
 function q:IsActiveThreadTerminating()return terminating()end
 function q:SetThingHasInformation(a,b,c,d)assert(a==me and b and c and not d)end
 function q:ReadGlobalGameDataString(offset)
  reads=reads+1
  if offset==0x73c then latestLetter='LETTER_'..reads;if initialLetter==nil then initialLetter=latestLetter end;return latestLetter end
  assert(offset==0x740);latestReward='MAN_REPLY_'..reads;return latestReward
 end
 function q:IsObjectInThingsPossession(item,a)assert(a==hero and item==latestLetter and item~=initialLetter);return inventory end
 function q:GetHealth(a)assert(a==me);return health and 1 or 0 end
 function q:PauseAllNonScriptedEntities(b)assert(live and paused~=b);paused=b;log(b and 'pause' or 'resume')end
 function q:FixMovieSequenceCamera(b)assert(live and paused);camera=b;log(b and 'cameraFix' or 'cameraFree')end
 function q:EntitySetFacingAngleTowardsThing(a,b,c)assert(a==me and b==hero and type(c)=='boolean')end
 function q:Pause(t)assert(t==1)end
 function q:CameraUseCameraPoint(k,a,p,m,d)assert(a==me and live and paused and p==-1 and m==0 and d==-1 and (k=='CAM_REL_WOMAN' or k=='CAM_REL_WOMANHERO'));log(k)end
 function q:GiveHeroYesNoQuestion(k,y,n,e,b)
  assert(y=='TEXT_OBJECT_HERO_ANSWER_YES' and n=='TEXT_OBJECT_HERO_ANSWER_NO' and e=='' and b and live and paused)
  assert(k=='TEXT_QST_B10_MANS_LOVER_GIVE_LETTER_QUESTION' or k=='TEXT_QST_B10_MANS_LOVER_LETTER_FROM_QUESTION');questionKey=k;questions=questions+1;polls=0;log(k)
 end
 function q:MsgIsQuestionAnsweredYesOrNo()
  polls=polls+1;if polls<=wait then return -1 end
  if questionKey=='TEXT_QST_B10_MANS_LOVER_GIVE_LETTER_QUESTION' then return mode=='talk-no' and 0 or 1 end
  return fromAnswer
 end
 function q:TakeObjectFromHero(item)assert(item==latestLetter and item~=initialLetter and live and paused and takes==0);takes=1;log('take:'..item)end
 function q:GiveHeroObject(item,count,b)
  assert(takes==1 and gifts==0 and count==-1 and not b and live and paused)
  assert(item==(fromAnswer==0 and latestReward or 'OBJECT_SICK_CHILD_WOMANS_HERO_LETTER'));gifts=1;log('gift:'..item)
 end
 function q:ReadGlobalGameDataFloat(offset)assert(offset==(fromAnswer==0 and 0x758 or 0x75c));return fromAnswer==0 and 1.25 or -2.5 end
 function q:GiveHeroMorality(value)assert(gifts==1 and morality==0 and state==initial and value==(fromAnswer==0 and 1.25 or -2.5));morality=1;log('morality')end
 function q:ClearThingHasInformation(a)OnPersist(q,me,{});assert(a==me and morality==1 and not saved.LeavingRegion);clears=1;log('clear')end
 function r:NewResource()assert(not held);held=true;return 21 end
 function r:PrepareResource(id)assert(id==21 and held)end
 function r:TryAcquire(id,a,k)assert(id==21 and a==me and k==4);attempts=attempts+1;return attempts>wait end
 function r:ReleaseResource(id)assert(id==21 and held and not live and not paused);held=false end
 function r:ResourceThingHealthAbove(value,minimum) assert(minimum==0.0);return q:GetHealth(self:ScriptThing(value))>minimum end
 function r:ScriptThing(id)assert(id==21 and held);return me end
 function r:StartMovie(k)assert(k=='' and held and not live);starts=starts+1;live=true;log('start');return handle end
 function r:DestroyMovie(id)assert(id==handle and live and not paused);live=false;ends=ends+1;finished=true;log('end')end
 function me:MsgIsPresentedWithItem()if mode=='presented' then g_PresentedItemName=initialLetter;return true end;return false end
 function me:IsTalkedToByHero()return mode~='presented' end
 function me:ClearCommands()assert(live and paused);log('clearCommands')end
 function me:Speak(a,k,g,b,c,d)
  assert(a==hero and g==0 and not b and c and not d and k:find('TEXT_QST_B10_MANS_LOVER_',1,true)==1);speeches=speeches+1;remaining=wait;log(k);return k~=cancel
 end
 function me:IsPerformingScriptTask()if remaining>0 then remaining=remaining-1;return true end;return false end
 Init(q,me);Main(q,me);OnPersist(q,me,{})
 assert(not held and not live and not paused and starts==ends and starts<=1)
 assert(takes<=1 and gifts<=takes and morality<=gifts and clears==morality and saved.LeavingRegion==(clears==1))
 assert(state==(morality==1 and (fromAnswer==0 and 2 or 1) or initial))
 if fromAnswer==0 then assert(morality==gifts,'man reply grant separated from morality')end
 if not inventory or mode=='talk-no' then assert(takes==0 and gifts==0 and questions<=1)end
 if gifts==1 then assert(fromAnswer~=0 or latestReward~=nil)end
 if deadline==0 and cancel=='' then
  local success=inventory and mode~='talk-no';assert(finished and takes==(success and 1 or 0) and gifts==takes and morality==gifts)
  assert(questions==(success and (mode=='presented' and 1 or 2) or inventory and 1 or 0))
 end
 return table.concat(trace,'|'),frame,attempts,starts,ends,questions,polls,speeches,takes,gifts,morality,clears,reads,state,saved.LeavingRegion,camera
end'''
CANCELS=('','TEXT_QST_B10_MANS_LOVER_INTRO','TEXT_QST_B10_MANS_LOVER_FROM_YOU_10','TEXT_QST_B10_MANS_LOVER_MAN_LIKES_ME_10','TEXT_QST_B10_MANS_LOVER_MAN_LIKES_ME_20','TEXT_QST_B10_MANS_LOVER_FROM_YOU_20','TEXT_QST_B10_MANS_LOVER_FROM_YOU_30','TEXT_QST_B10_MANS_LOVER_LIKES_YOU_10')
CASES=list(itertools.product(('presented','talk-yes','talk-no'),(0,1,2),(0,1,2),(False,True),(False,True),(0,2),(0,1,2,3,5,9),CANCELS,(0,31,713)))
@pytest.mark.parametrize('case',CASES)
def test_actual_letter_source_reward_and_interruption(generated,case):
 traces=[]
 for source in generated.values():
  lua=LuaRuntime();lua.execute(source);traces.append(lua.execute(H)(*case))
 assert traces[0]==traces[1]

import struct
def test_native_take_reply_and_state_timing_windows():
 from tools.script_recovery.lift_native_lua import RData
 from tools.script_recovery.convert_quest_unit import callee_stack_words
 from tools.script_recovery.annotate_interface_slots import load_slots
 import unicorn as u
 from unicorn import x86_const as x
 rd=RData();slots={0x1f4:('TakeObjectFromHero',1),0x1e4:('GiveHeroObject',3),0x270:('GiveHeroMorality',1),0x5a4:('ClearThingHasInformation',1)}
 for slot,(name,purge) in slots.items():
  assert load_slots()[slot]==name
  target=int.from_bytes(rd.bytes_at(0x1260f0c+slot,4),'little');assert callee_stack_words(target)==purge
 cpu=u.Uc(u.UC_ARCH_X86,u.UC_MODE_32)
 for a,n in ((0xec9000,0x3000),(0x415000,0x1000),(0x99e000,0x1000),(0xf35000,0x1000),(0x143e000,0x1000),(0x300000,0x10000),(0x500000,0x10000),(0x200000,0x1000)):cpu.mem_map(a,n)
 cpu.mem_write(0xec9000,rd.bytes_at(0xec9000,0x3000))
 endpoints={0x200100+i*0x10:slot for i,slot in enumerate(slots)}
 for a in (*endpoints,0x415d70,0x99ebf0,0x99eae0,0xf35b30):cpu.mem_write(a,b'\xc3')
 def put(a,v):cpu.mem_write(a,struct.pack('<I',v&0xffffffff))
 def word(a):return struct.unpack('<I',cpu.mem_read(a,4))[0]
 state={};layouts=0
 def hook(m,a,size,user):
  if a in (0xeca651,0xeca801):state['selection']=a;m.emu_stop();return
  if a in (0xecaf49,0xeca325,0xecae14):state['exit']=a;m.emu_stop();return
  sp=m.reg_read(x.UC_X86_REG_ESP);ecx=m.reg_read(x.UC_X86_REG_ECX)
  if a==0x415d70:
   offset=ecx-0x507000;out=word(sp+4);assert offset==(0x73c if state['phase']=='take' else 0x740)
   assert out==state['stack']+(0x6c if state['phase']=='take' else 0x70)
   put(out,state['cookie']+(1 if state['phase']=='take' else 2));state['strings'][out]=state['phase'];m.reg_write(x.UC_X86_REG_EAX,out);purge=4
  elif a==0x99ebf0:
   assert ecx==state['stack']+0x78 and word(sp+8)==0xffffffff and rd.string_at(word(sp+4))=='OBJECT_SICK_CHILD_WOMANS_HERO_LETTER'
   put(ecx,state['cookie']+3);state['strings'][ecx]='hero';purge=8
  elif a==0x99eae0:
   assert ecx in state['strings'];state['dtors']+=1;del state['strings'][ecx];purge=0
  elif a==0xf35b30:
   assert ecx==state['owner'];m.reg_write(x.UC_X86_REG_EAX,state['firstTerm'] if state['phase']=='grant' else state['secondTerm']);purge=0
  elif a in endpoints:
   slot=endpoints[a];assert ecx==state['gsi'];purge=slots[slot][1]*4
   assert cpu.mem_read(state['owner']+0x1c,1)==b'\0','LeavingRegion written before clear'
   if slot==0x1f4:
    out=word(sp+4);assert out==state['stack']+0x6c and word(out)==state['cookie']+1;state['trace'].append('take')
   elif slot==0x1e4:
    out=word(sp+4);assert out==state['stack']+(0x70 if state['from']==0 else 0x78) and word(out)==state['cookie']+(2 if state['from']==0 else 3)
    assert word(sp+8)==0xffffffff and word(sp+12)==0;state['trace'].append('gift')
   elif slot==0x270:
    assert word(sp+4)==state['bits'] and word(state['quest']+0x6c)==state['initial'];state['trace'].append('morality')
   else:
    assert word(sp+4)==state['owner']+8 and word(state['quest']+0x6c)==(2 if state['from']==0 else 1);state['trace'].append('clear')
  else:return
  m.reg_write(x.UC_X86_REG_EIP,word(sp));m.reg_write(x.UC_X86_REG_ESP,sp+4+purge)
 cpu.hook_add(u.UC_HOOK_CODE,hook)
 for source,firstTerm,secondTerm,initial,bits,owner,quest,sp,cookie in itertools.product((0,1,2),(0,1),(0,1),(0,1,2),(0x3fa00000,0xc0200000,0x41200000),(0x501000,0x502000),(0x503000,0x504000),(0x300800,0x301800),(713,0xdead0000)):
  state=dict(from_=source,firstTerm=firstTerm,secondTerm=secondTerm,initial=initial,bits=bits,owner=owner,quest=quest,stack=sp,cookie=cookie,phase='take',trace=[],strings={},dtors=0,exit=None);state['from']=source
  gsi=state['gsi']=0x500000;table=gsi+0x800;put(gsi,table);put(owner+4,gsi);put(owner+0x14,quest);put(0x143e90c,0x507000)
  put(0x507758,bits);put(0x50775c,bits);put(quest+0x68,0x12345678);put(quest+0x6c,initial);put(quest+0x70,0x76543210);cpu.mem_write(owner+0x1b,b'\x55\0\x66')
  for a,slot in endpoints.items():put(table+slot,a)
  cpu.reg_write(x.UC_X86_REG_ESI,owner);cpu.reg_write(x.UC_X86_REG_EBP,owner+8);cpu.reg_write(x.UC_X86_REG_ESP,sp)
  cpu.reg_write(x.UC_X86_REG_EDI,source);cpu.emu_start(0xeca649,0xeca802,count=10)
  assert state['selection']==(0xeca651 if source==0 else 0xeca801)
  cpu.emu_start(0xeca465,0xeca494,count=100);assert state['trace']==['take'] and state['dtors']==1 and cpu.reg_read(x.UC_X86_REG_ESP)==sp
  state['phase']='grant';cpu.emu_start(0xeca799 if source==0 else 0xeca949,0xeca98b if source else 0xecae15,count=200)
  complete=not firstTerm and (source==0 or not secondTerm)
  if source and not firstTerm:
   assert state['trace']==['take','gift'] and word(quest+0x6c)==initial and cpu.mem_read(owner+0x1c,1)==b'\0'
   state['phase']='afterGift';cpu.emu_start(0xecaa24,0xecae15,count=100)
  assert state['trace']==['take']+([] if firstTerm else ['gift'])+(['morality','clear'] if complete else [])
  assert word(quest+0x6c)==(2 if source==0 else 1) if complete else word(quest+0x6c)==initial
  assert cpu.mem_read(owner+0x1b,3)==bytes([0x55,int(complete),0x66]) and not state['strings'] and state['dtors']==(1 if firstTerm else 2)
  assert word(quest+0x68)==0x12345678 and word(quest+0x70)==0x76543210 and cpu.reg_read(x.UC_X86_REG_ESP)==sp
  layouts+=1
 assert layouts==1728

PARTIAL_CASES=[]
for mode,handle in itertools.product(('presented','talk-yes'),(0,31,713)):
 for answer in (1,2):PARTIAL_CASES.append((mode,answer,'TEXT_QST_B10_MANS_LOVER_LIKES_YOU_10',handle,1))
 PARTIAL_CASES.append((mode,0,'TEXT_QST_B10_MANS_LOVER_MAN_LIKES_ME_20',handle,0))
 for answer in (0,1):PARTIAL_CASES.append((mode,answer,'TEXT_QST_B10_MANS_LOVER_FROM_YOU_10',handle,0))
@pytest.mark.parametrize('case',PARTIAL_CASES)
def test_partial_letter_effects_before_departure(generated,case):
 mode,answer,cancel,handle,gift=case;traces=[]
 for source in generated.values():
  lua=LuaRuntime();lua.execute(source);result=lua.execute(H)(mode,0,answer,True,True,0,0,cancel,handle)
  assert result[8]==1 and result[9]==gift and result[10]==0 and result[11]==0 and result[13]==0 and result[14]==False
  traces.append(result)
 assert traces[0]==traces[1]

@pytest.mark.parametrize('style',('draft','readable'))
@pytest.mark.parametrize('fault',('take','stale_take','man_def','hero_literal','gift_count','gift_flag','morality','state','early_leave','clear','pause','movie'))
def test_wrong_letter_reward_is_detected(generated,style,fault):
 import re
 s=generated[style];answer=1 if fault=='hero_literal' else 0
 if fault=='take':s=re.sub(r'quest:TakeObjectFromHero\([^)]*\)','do end',s)
 elif fault=='stale_take':s=re.sub(r'quest:TakeObjectFromHero\([^)]*\)','quest:TakeObjectFromHero("STALE")',s)
 elif fault=='man_def':s=s.replace('ReadGlobalGameDataString(0x740)','ReadGlobalGameDataString(0x73c)').replace('ReadGlobalGameDataString(1856)','ReadGlobalGameDataString(1852)').replace('SCRIPT_DEF.ObjectFromMansLover','SCRIPT_DEF.ObjectToGiveToMansLover')
 elif fault=='hero_literal':s=s.replace('OBJECT_SICK_CHILD_WOMANS_HERO_LETTER','OBJECT_SICK_CHILD_WOMANS_MAN_LETTER')
 elif fault=='gift_count':s=s.replace(', -1, false)',', 1, false)')
 elif fault=='gift_flag':s=s.replace(', -1, false)',', -1, true)')
 elif fault=='morality':s=s.replace('ReadGlobalGameDataFloat(0x758)','ReadGlobalGameDataFloat(0x75c)').replace('ReadGlobalGameDataFloat(1880)','ReadGlobalGameDataFloat(1884)').replace('SCRIPT_DEF.TellingTheTruth','SCRIPT_DEF.LyingToWomanMorality')
 elif fault=='state':s=s.replace('SetStateInt("MansLoverState", 2)','SetStateInt("MansLoverState", 1)')
 elif fault=='early_leave':
  if style=='draft':write='__native_entity_state:SetStateBool("LeavingRegion", true)'
  else:
   targets=re.findall(r'(?m)^\s*(\w+)\s*=\s*quest:PersistTransferBool\(context,\s*"LeavingRegion",',s)
   assert len(targets)==1
   write=targets[0]+' = true'
  pos=s.index('quest:GiveHeroObject(');s=s[:pos]+write+'; '+s[pos:]
 elif fault=='clear':s=s.replace('quest:ClearThingHasInformation(me)','do end')
 elif fault=='pause':s=s.replace('quest:PauseAllNonScriptedEntities(false)','quest:PauseAllNonScriptedEntities(true)')
 else:s=re.sub(r'resources:DestroyMovie\([^)]*\)','resources:DestroyMovie(-1)',s)
 assert s!=generated[style]
 lua=LuaRuntime();lua.execute(s)
 with pytest.raises(Exception):lua.execute(H)('presented',0,answer,True,True,0,0,'',31)
