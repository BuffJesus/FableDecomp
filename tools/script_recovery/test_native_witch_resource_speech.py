import copy,hashlib,itertools,json,struct,sys
from pathlib import Path
from unittest.mock import patch
import pytest
from lupa.lua54 import LuaRuntime
ROOT=next(p for p in Path(__file__).resolve().parents if (p/'tools/script_recovery/script_units.py').exists());sys.path.insert(0,str(ROOT))
from tools.script_recovery.convert_quest_unit import UnitConverter
from tools.script_recovery.build_readable_unit import readable_file
@pytest.fixture(scope='module')
def evidence(tmp_path_factory):
 from tools.script_recovery import native_reviewed_member_speech as adapter
 c=UnitConverter(ROOT/'refs/script_recovery/sick_child/translation_unit_typed.json');unit=json.loads((ROOT/'refs/script_recovery/sick_child/units/V_SickChild.json').read_text());row=unit['reviewedMemberSpeech']['0x00ece7f0'];original=adapter.restore;captured={}
 unit['entities']={'Witch':unit['entities']['Witch']};out=tmp_path_factory.mktemp('witch_speech')
 def capture(text,fn,review,rd,spec):
  if fn['address'].lower()=='0x00ece7f0':captured.update(text=text,fn=fn,rd=rd,spec=spec)
  return original(text,fn,review,rd,spec)
 with patch.object(c,'constructor_defaults',return_value={}),patch.object(adapter,'restore',capture):c.convert(unit,out)
 source=(out/'FSE/V_SickChild/Entities/Witch.lua').read_text()
 helper=(out/'FSE/V_SickChild/native_quest_helpers.lua').read_text()
 preload='package.preload["V_SickChild.native_quest_helpers"] = function()\n'+helper+'\nend\n'
 return captured,row,dict(draft=preload+source,readable=preload+readable_file(source)[0])
H=r'''return function(branch,health,speak,tasks,termination)
 local q,me,hero,r,member={},{},{},{},{}
 local boundary={};local paused=false;local frame,checks,spoken,created,destroyed=0,0,0,0,0
 local remaining=0;local cleanupHandle='unset'
 function q:RetailResources()return r end
 function r:IsHitByHeroExceptAbility(actor,ability)
     assert(actor==me and ability==14)
     if actor:MsgIsHitByHero() then return true end
     if not actor:MsgIsHitByAnySpecialAbilityFromHero() then return false end
     return not actor:MsgIsHitByHeroSpecialAbility(ability)
 end
 function q:GetStateBool(key)assert(key=='MotherIntroDone');return true end
 function q:PersistTransferBool(ctx,key,v)assert(key=='IntroDone');return true end
 function q:PersistTransferInt(ctx,key,v)assert(key=='ThingsForPotionGot');return branch=='hit_before' and 1 or 4 end
 for _,name in ipairs({'SetIsPushableByHero','SetThingHasInformation','EntitySetAsKillable','EntitySetAsToAddToComboMultiplierWhenHit'})do q[name]=function()end end
 function q:NewScriptFrame(actor)assert(actor==me);frame=frame+1;assert(frame<20);return true end
 function q:IsActiveThreadTerminating()checks=checks+1;return termination>0 and checks>=termination end
 function q:GetHero()return hero end
 function q:GetHealth(actor)assert(actor==me);return health and 10 or 0 end
 function q:PauseAllNonScriptedEntities(flag)assert(flag~=paused);paused=flag end
 function r:MemberResource(key)assert(key=='seh_Witch');return member end
 function r:PrepareResource(value)assert(value==member)end
 function r:TryAcquire(value,actor,priority)assert(value==member and actor==me and priority==4);return true end
 function r:ResourceThingHealthAbove(handle,minimum)assert(minimum==0);return q:GetHealth(self:ScriptThing(handle))>minimum end
 function r:ScriptThing(value)assert(value==member);return me end
 function r:StartMovie(key)assert(key=='' and created==0);created=1;return 31 end
 function r:DestroyMovie(handle)assert(not paused and created==1);destroyed=destroyed+1;cleanupHandle=handle;error(boundary)end
 function me:IsTalkedToByHero()return branch=='cure' end
 function me:MsgIsHitByHero()return branch~='cure' end
 function me:MsgIsHitByAnySpecialAbilityFromHero()return false end
 function me:IsPerformingScriptTask()if remaining>0 then remaining=remaining-1;return true end;return false end
 function me:Speak(listener,key,group,a,b,c)
  assert(listener==hero and not a and b and not c)
  assert(key==(branch=='cure' and 'TEXT_QST_B10_WITCH_GOT_CURE' or 'TEXT_QST_B10_WITCH_ONHIT_10'))
  assert(group==(branch=='cure' and 2 or 0));spoken=spoken+1;remaining=tasks;return speak
 end
 Init(q,me);OnPersist(q,me,{})
 local ok,err=pcall(Main,q,me);assert(ok or err==boundary,tostring(err))
 assert(spoken==(health and created==1 and 1 or 0),'missing speech')
 assert(not paused,'pause still held');assert(destroyed==created)
 -- Record existing movie-handle alias errors without substituting a handle.
 return spoken,created,destroyed,frame,checks,tostring(cleanupHandle)
end'''
CASES=list(itertools.product(('hit_before','cure','hit_after'),(False,True),(False,True),(0,2),(0,1,3,8,14)))

@pytest.mark.parametrize('style',('draft','readable'))
@pytest.mark.parametrize('case',CASES)
def test_complete_generated_speech_to_first_cleanup(evidence,style,case):
 lua=LuaRuntime();lua.execute(evidence[2][style]);lua.execute(H)(*case)

@pytest.mark.parametrize('fault',('source','native','window','old','token','definition','resource','listener','text_operand','literal','call_duplicate','zero','flag','buffer_getter','speech_wrapper'))
def test_stale_or_ambiguous_speech_is_rejected(evidence,fault):
 from tools.script_recovery.native_reviewed_member_speech import restore
 from tools.script_recovery.test_native_inline_self_argument import ChangedBytes
 captured,row,_=evidence;row=copy.deepcopy(row);fn=copy.deepcopy(captured['fn']);text=captured['text'];rd=captured['rd'];spec=copy.copy(captured['spec']);spec.parent_resources=dict(spec.parent_resources)
 if fault=='source':fn['decompile']+='\n'
 elif fault=='native':row['nativeSha256']='0'*64
 elif fault=='window':row['calls'][0]['windowSha256']='0'*64
 elif fault=='old':text=text.replace(row['calls'][0]['old'],'changed;')
 elif fault=='token':row['calls'][0]['textToken']='bad token'
 elif fault=='definition':text=text.replace(row['calls'][0]['textDefinition'],'changed;',1)
 elif fault=='resource':spec.parent_resources[88]='wrong'
 elif fault=='listener':row['calls'][0]['listener']='pCVar17'
 elif fault=='text_operand':row['calls'][0]['textToken']='xStack_a0'
 elif fault=='literal':text=text.replace('pcVar14 = "TEXT_QST_B10_WITCH_GOT_CURE";','pcVar14 = "wrong";')
 elif fault=='call_duplicate':row['calls'].append(copy.deepcopy(row['calls'][0]))
 else:
  address,data={'zero':(0xeceed3,'33ee'),'flag':(0xecf103,'6a00'),'buffer_getter':(0x99e4c0,'31c0'),'speech_wrapper':(0x7e7390,'31c9')}[fault]
  rd=ChangedBytes(rd,{address:bytes.fromhex(data)})
  # Refresh digests to require native semantics, not only stale hash rejection.
  start=int(fn['address'],16);row['nativeSha256']=hashlib.sha256(rd.bytes_at(start,int(fn['bodyEndExclusive'],16)-start)).hexdigest()
  for call in row['calls']:
   begin,site=int(call['start'],16),int(call['site'],16);call['windowSha256']=hashlib.sha256(rd.bytes_at(begin,site+3-begin)).hexdigest()
 with pytest.raises(ValueError,match='reviewed member speech'):restore(text,fn,row,rd,spec)

def test_no_review_preserves_unresolved_calls(evidence):
 from tools.script_recovery.native_reviewed_member_speech import restore
 c,row,_=evidence;assert restore(c['text'],c['fn'],None,c['rd'],c['spec'])==c['text']

def test_original_member_speech_and_borrowed_string_getter():
 import unicorn as u
 from unicorn import x86_const as x
 from tools.script_recovery.lift_native_lua import RData
 from tools.script_recovery.convert_quest_unit import callee_stack_words
 rd=RData();assert callee_stack_words(0x7e7390)==6
 cpu=u.Uc(u.UC_ARCH_X86,u.UC_MODE_32)
 for a,n in ((0xecf000,0x1000),(0x99e000,0x1000),(0x129a000,0x1000),(0x300000,0x10000),(0x500000,0x9000),(0x200000,0x1000)):cpu.mem_map(a,n)
 cpu.mem_write(0xecf000,rd.bytes_at(0xecf000,0x800));cpu.mem_write(0x99e4c0,rd.bytes_at(0x99e4c0,15));cpu.mem_write(0x200100,b'\xc3');cpu.mem_write(0x200200,b'\xc3')
 def put(a,v):cpu.mem_write(a,struct.pack('<I',v&0xffffffff))
 def word(a):return struct.unpack('<I',cpu.mem_read(a,4))[0]
 state={};rows=[]
 def hook(machine,a,size,user):
  if a==0x200100:
   sp=machine.reg_read(x.UC_X86_REG_ESP);assert machine.reg_read(x.UC_X86_REG_ECX)==state['gsi']
   assert [word(sp+4+4*i) for i in range(5)]==[state['text'],state['group'],0,1,0]
   machine.reg_write(x.UC_X86_REG_EAX,state['hero']);machine.reg_write(x.UC_X86_REG_EIP,word(sp));machine.reg_write(x.UC_X86_REG_ESP,sp+4)
  elif a==0x200200:
   sp=machine.reg_read(x.UC_X86_REG_ESP);args=[word(sp+4+4*i) for i in range(6)]
   assert machine.reg_read(x.UC_X86_REG_ECX)==state['parent']+0x58 and args==[state['hero'],state['text'],state['group'],0,1,0]
   state['args']=args;machine.reg_write(x.UC_X86_REG_EIP,word(sp));machine.reg_write(x.UC_X86_REG_ESP,sp+28)
 cpu.hook_add(u.UC_HOOK_CODE,hook)
 for gsi,owner,sp,empty,(begin,site,group) in itertools.product((0x500000,0x501000,0x502000),(0x503000,0x504000),(0x300800,0x301800),(False,True),((0xecf0f9,0xecf12a,0),(0xecf467,0xecf48a,2),(0xecf6b8,0xecf6e4,0))):
  parent=0x505000;table=0x506000;hero=0x507000;rep=0x508000
  text=0x12f2618 if group==2 else (0x129aaf4 if empty else 0x508100)
  state=dict(gsi=gsi,owner=owner,parent=parent,stack=sp,hero=hero,text=text,group=group,site=hex(site),empty=empty)
  put(gsi,gsi+0x800);put(gsi+0x800+0x118,0x200100);put(owner+4,gsi);put(owner+0x14,parent);put(parent+0x58,table);put(table+0x34,0x200200)
  put(sp+0x18,0 if empty else rep);put(rep,0x508100)
  cpu.reg_write(x.UC_X86_REG_ESI,owner);cpu.reg_write(x.UC_X86_REG_EBP,0);cpu.reg_write(x.UC_X86_REG_ESP,sp);cpu.emu_start(begin,site+3,count=100)
  assert cpu.reg_read(x.UC_X86_REG_ESP)==sp and 'args' in state;rows.append(state.copy())
 assert len(rows)==72
