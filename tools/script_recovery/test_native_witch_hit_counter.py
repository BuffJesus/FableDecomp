import copy,hashlib,itertools,json,sys
from pathlib import Path
from unittest.mock import patch
import pytest
from lupa.lua54 import LuaRuntime
ROOT=next(p for p in Path(__file__).resolve().parents if (p/'tools/script_recovery/script_units.py').exists());sys.path.insert(0,str(ROOT))
@pytest.fixture(scope='module')
def evidence(tmp_path_factory):
 from tools.script_recovery.convert_quest_unit import UnitConverter
 from tools.script_recovery.build_readable_unit import readable_file
 from tools.script_recovery import native_reviewed_stack_aliases as adapter
 c=UnitConverter(ROOT/'refs/script_recovery/sick_child/translation_unit_typed.json');unit=json.loads((ROOT/'refs/script_recovery/sick_child/units/V_SickChild.json').read_text());row=unit['reviewedStackScalarAliases']['0x00ece7f0'];unit['entities']={'Witch':unit['entities']['Witch']};captured={};original=adapter.restore;out=tmp_path_factory.mktemp('witch_counter')
 def capture(text,fn,review,rd):
  if fn['address'].lower()=='0x00ece7f0':captured.update(text=text,fn=fn,rd=rd)
  return original(text,fn,review,rd)
 with patch.object(adapter,'restore',capture),patch.object(c,'constructor_defaults',return_value={}):c.convert(unit,out)
 text=(out/'FSE/V_SickChild/Entities/Witch.lua').read_text();return captured,row,dict(draft=text,readable=readable_file(text)[0]),(out/'FSE/V_SickChild/native_quest_helpers.lua').read_text()
H=r'''return function(phase,count,health,speak,taskWait,acquireWait,termination,seed)
 local q,me,hero,r,member={},{},{},{},{};local paused=false;local inspect=false;local saved={};local trace={}
 local frames,attempts,created,destroyed,spoken,tasks=0,0,0,0,0,0;local current=nil;local canceled=false
 local function log(value)trace[#trace+1]=value end
 local function terminating()return termination>0 and frames>=termination or created>=count and destroyed==created end
 function q:RetailResources()return r end
 function r:IsHitByHeroExceptAbility(actor,ability)
     assert(actor==me and ability==14)
     if actor:MsgIsHitByHero() then return true end
     if not actor:MsgIsHitByAnySpecialAbilityFromHero() then return false end
     return not actor:MsgIsHitByHeroSpecialAbility(ability)
 end
 function q:GetStateBool(name)assert(name=='MotherIntroDone');return true end
 function q:PersistTransferBool(ctx,name,value)assert(name=='IntroDone');if inspect then saved.IntroDone=value;return value end;return true end
 function q:PersistTransferInt(ctx,name,value)assert(name=='ThingsForPotionGot');if inspect then saved.ThingsForPotionGot=value;return value end;return phase end
 function q:SetIsPushableByHero(actor,value)assert(actor==me and not value)end
 function q:SetThingHasInformation(actor,a,b,c)assert(actor==me and not a and b and not c)end
 function q:EntitySetAsKillable(actor,a,b)assert(actor==me and not a and not b)end
 function q:EntitySetAsToAddToComboMultiplierWhenHit(actor,value)assert(actor==me and not value)end
 function q:NewScriptFrame(actor)assert(actor==me);frames=frames+1;assert(frames<60);return not terminating()end
 function q:IsActiveThreadTerminating()return terminating()end
 function q:GetHero()return hero end
 function q:GetHealth(actor)assert(actor==me);return health and 10 or 0 end
 function q:PauseAllNonScriptedEntities(value)assert(value~=paused);paused=value;log('pause:'..tostring(value))end
 function r:MemberResource(name)assert(name=='seh_Witch');return member end
 function r:PrepareResource(value)assert(value==member)end
 function r:TryAcquire(value,actor,priority)assert(value==member and actor==me and priority==4);attempts=attempts+1;return attempts>acquireWait end
 function r:ResourceThingHealthAbove(handle,minimum)assert(minimum==0);return q:GetHealth(self:ScriptThing(handle))>minimum end
 function r:ScriptThing(value)assert(value==member);return me end
 function r:StartMovie(name)assert(name=='' and created==destroyed and created<count);created=created+1;local handles={0,31,713};current=handles[((created-1+seed)%3)+1];log('start:'..current);return current end
 function r:DestroyMovie(value)assert(value==current and not paused and destroyed==created-1);destroyed=destroyed+1;log('destroy:'..value)end
 function me:IsTalkedToByHero()return false end
 function me:MsgIsHitByHero()return true end
 function me:IsPerformingScriptTask()tasks=tasks+1;return tasks<=taskWait end
 function me:Speak(actor,text,group,a,b,c)
  local suffixes={'10','20','30'};assert(actor==hero and text=='TEXT_QST_B10_WITCH_ONHIT_'..suffixes[((created-1)%3)+1] and group==0 and not a and b and not c)
  spoken=spoken+1;tasks=0;canceled=not speak;log(text);return speak
 end
 Init(q,me);OnPersist(q,me,{ });Main(q,me);inspect=true;OnPersist(q,me,{ })
 assert(not paused and created==destroyed and spoken==(health and created or 0))
 assert(saved.IntroDone and saved.ThingsForPotionGot==phase)
 if termination==0 then assert(created==(health and not speak and 1 or count))end
 return table.concat(trace,'|'),frames,attempts,created,destroyed,spoken,tasks,saved.ThingsForPotionGot
end'''
CASES=list(itertools.product((1,4),(1,2,3,4,7),(False,True),(False,True),(0,2),(0,2),(0,1,2,5,9,15,25),(0,1,2)))
def execute(source,helper,case):
 lua=LuaRuntime(unpack_returned_tuples=True);lua.globals().helperSource=helper;lua.execute("package.preload['V_SickChild.native_quest_helpers']=assert(load(helperSource))");lua.execute(source);return lua.execute(H)(*case)
@pytest.mark.parametrize('style',('draft','readable'))
@pytest.mark.parametrize('case',CASES)
def test_repeated_hit_dialogue(evidence,style,case):execute(evidence[2][style],evidence[3],case)
@pytest.mark.parametrize('fault',('native','source','anchor','count','alias','duplicate'))
def test_stale_counter_review_rejected(evidence,fault):
 from tools.script_recovery.native_reviewed_stack_aliases import restore
 from tools.script_recovery.lift_native_lua import RData
 captured,row=evidence[:2];fn=copy.deepcopy(captured['fn']);row=copy.deepcopy(row);source=captured['text'];rd=captured['rd']
 if fault=='native':row['nativeSha256']='0'*64
 elif fault=='source':fn['decompile']+='\n'
 elif fault=='anchor':row['nativeInstructions']['0x00ECE8B1']='90'
 elif fault=='count':row['editCounts'][1]=4
 elif fault=='alias':source+='witchHitDialogueIndex;'
 elif fault=='duplicate':row['edits'].append(row['edits'][0]);row['editCounts'].append(1)
 with pytest.raises(ValueError):restore(source,fn,row,rd)
def test_unreviewed_counter_source_retained(evidence):
 from tools.script_recovery.native_reviewed_stack_aliases import restore
 from tools.script_recovery.lift_native_lua import RData
 captured=evidence[0];assert restore(captured['text'],captured['fn'],None,captured['rd'])==captured['text']
def test_original_counter_slot_and_cross_phase_sequences():
 import struct
 import unicorn as u
 from unicorn import x86_const as x
 from tools.script_recovery.lift_native_lua import RData
 rd=RData();cpu=u.Uc(u.UC_ARCH_X86,u.UC_MODE_32)
 for a,n in ((0xece000,0x2000),(0x99e000,0x1000),(0x300000,0x10000)):cpu.mem_map(a,n)
 cpu.mem_write(0xece7f0,rd.bytes_at(0xece7f0,4083))
 for a in (0x99e4b0,0x99efe0):cpu.mem_write(a,b'\xc3')
 def put(a,v):cpu.mem_write(a,struct.pack('<I',v&0xffffffff))
 def word(a):return struct.unpack('<I',cpu.mem_read(a,4))[0]
 data={};steps=[]
 def hook(machine,a,size,user):
  sp=machine.reg_read(x.UC_X86_REG_ESP);ecx=machine.reg_read(x.UC_X86_REG_ECX)
  if a==0x99e4b0:assert ecx==data['sp']+0x18;put(ecx,0);purge=0
  elif a==0x99efe0:
   assert ecx==data['sp']+0x18;literal=word(sp+4);data['text']=rd.bytes_at(literal,80).split(b'\0')[0].decode('ascii');put(ecx,literal);purge=4
  else:return
  machine.reg_write(x.UC_X86_REG_EIP,word(sp));machine.reg_write(x.UC_X86_REG_ESP,sp+4+purge)
 cpu.hook_add(u.UC_HOOK_CODE,hook)
 patterns=((0,),(1,),(0,0,1,1,1))
 for initial,sp,pattern in itertools.product((0,1,2,3,-1,0x7fffffff),(0x300800,0x301800),patterns):
  cpu.reg_write(x.UC_X86_REG_ESP,sp);cpu.reg_write(x.UC_X86_REG_EBP,713);cpu.emu_start(0xece7ff,0xece801,count=2);assert cpu.reg_read(x.UC_X86_REG_EBP)==0
  put(sp+0x1c,713);cpu.emu_start(0xece8b1,0xece8b5,count=2);assert word(sp+0x1c)==0
  put(sp+0x1c,initial);expected=initial
  for phase in pattern:
   begin,end=((0xecefc0,0xecf016),(0xecf5d5,0xecf62b))[phase];data=dict(sp=sp)
   cpu.emu_start(begin,end,count=80)
   suffix={0:'10',1:'20',2:'30'}.get(expected,'20');expected=expected+1 if expected in (0,1) else 0
   assert data['text']=='TEXT_QST_B10_WITCH_ONHIT_'+suffix and word(sp+0x1c)==expected and cpu.reg_read(x.UC_X86_REG_ESP)==sp
   steps.append((initial,sp,phase,data['text'],expected))
 assert len(steps)==84
