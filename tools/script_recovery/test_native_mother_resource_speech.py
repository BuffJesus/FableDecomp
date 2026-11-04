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
 c=UnitConverter(ROOT/'refs/script_recovery/sick_child/translation_unit_typed.json');unit=json.loads((ROOT/'refs/script_recovery/sick_child/units/V_SickChild.json').read_text());row=unit['reviewedMemberSpeech']['0x00ecd9e0'];original=adapter.restore;captured={}
 unit['entities']={'SickChildsMother':unit['entities']['SickChildsMother']};out=tmp_path_factory.mktemp('witch_speech')
 def capture(text,fn,review,rd,spec):
  if fn['address'].lower()=='0x00ecd9e0':captured.update(text=text,fn=fn,rd=rd,spec=spec)
  return original(text,fn,review,rd,spec)
 with patch.object(c,'constructor_defaults',return_value={}),patch.object(adapter,'restore',capture):c.convert(unit,out)
 source=(out/'FSE/V_SickChild/Entities/SickChildsMother.lua').read_text();return captured,row,dict(draft=source,readable=readable_file(source)[0]),(out/'FSE/V_SickChild/native_quest_helpers.lua').read_text()
H=r'''return function(health,speak,tasks,termination)
 local q,me,hero,r,member={},{},{},{},{}
 local boundary={};local paused=false;local frame,checks,spoken,created,destroyed=0,0,0,0,0
 local remaining=0;local cleanupHandle='unset';local healthRead=false
 function q:RetailResources()return r end
 function q:GetStateBool(key)assert(key=='MotherIntroDone' or key=='FinishedQuest');return key=='MotherIntroDone' end
 function q:SetIsPushableByHero(actor,flag)assert(actor==me and not flag)end
 function q:NewScriptFrame(actor)assert(actor==me);frame=frame+1;assert(frame<30);return true end
 function q:IsActiveThreadTerminating()checks=checks+1;return termination>0 and checks>=termination end
 function q:GetHero()return hero end
 function q:GetHealth(actor)assert(actor==me);healthRead=true;return health and 10 or 0 end
 function q:PauseAllNonScriptedEntities(flag)assert(flag~=paused);paused=flag end
 function q:ReadGlobalGameDataString(offset)assert(offset==0x744);return 'POTION' end
 function q:IsObjectInThingsPossession(key,actor)assert(key=='POTION' and actor==hero);return false end
 function r:MemberResource(key)assert(key=='seh_Mother');return member end
 function r:PrepareResource(value)assert(value==member)end
 function r:TryAcquire(value,actor,priority)assert(value==member and actor==me and priority==4);return true end
 function r:ResourceThingHealthAbove(handle,minimum)assert(minimum==0);return q:GetHealth(self:ScriptThing(handle))>minimum end
 function r:ScriptThing(value)assert(value==member);return me end
 function r:StartMovie(key)assert(key=='' and created==0);created=1;return 31 end
 function r:DestroyMovie(handle)assert(not paused and created==1);destroyed=destroyed+1;cleanupHandle=handle;error(boundary)end
 function me:IsTalkedToByHero()return true end
 function me:MsgIsHitByHero()return false end
 function me:MsgIsHitByAnySpecialAbilityFromHero()return false end
 function me:IsPerformingScriptTask()if remaining>0 then remaining=remaining-1;return true end;return false end
 function me:Speak(listener,key,group,a,b,c)
  assert(listener==hero and key=='TEXT_QST_B10_MOTHER_CURE_REMIND_10' and group==0 and not a and b and not c)
  spoken=spoken+1;remaining=tasks;return speak
 end
 Init(q,me);OnPersist(q,me,{})
 local ok,err=pcall(Main,q,me);assert(ok or err==boundary,tostring(err))
 assert(spoken==(healthRead and health and 1 or 0),'missing reminder speech')
 assert(not paused and destroyed==created)
 -- Existing Mother wait-cleanup handle faults remain visible and are outside this speech recovery.
 return spoken,created,destroyed,frame,checks,tostring(cleanupHandle)
end'''
CASES=list(itertools.product((False,True),(False,True),(0,2),(0,1,3,5,7,9,12,20)))
@pytest.mark.parametrize('style',('draft','readable'))
@pytest.mark.parametrize('case',CASES)
def test_complete_generated_speech_to_first_cleanup(evidence,style,case):
 lua=LuaRuntime();lua.globals().helperSource=evidence[3];lua.execute("package.preload['V_SickChild.native_quest_helpers']=assert(load(helperSource))");lua.execute(evidence[2][style]);lua.execute(H)(*case)

@pytest.mark.parametrize('fault',('native','source','window','old','resource','listener','literal','call_duplicate','zero','flag','buffer_getter','speech_wrapper','group','offset','literal_pointer'))
def test_stale_or_changed_reminder_review_fails(evidence,fault):
 from tools.script_recovery.native_reviewed_member_speech import restore
 from tools.script_recovery.test_native_inline_self_argument import ChangedBytes
 c,row=evidence[:2];row=copy.deepcopy(row);fn=copy.deepcopy(c['fn']);rd=c['rd'];text=c['text'];spec=copy.copy(c['spec']);spec.parent_resources=dict(spec.parent_resources)
 if fault=='native':row['nativeSha256']='0'*64
 elif fault=='source':fn['decompile']+='\n'
 elif fault=='window':row['calls'][0]['windowSha256']='0'*64
 elif fault=='old':text=text.replace(row['calls'][0]['old'],'changed;')
 elif fault=='resource':spec.parent_resources[72]='wrong'
 elif fault=='listener':row['calls'][0]['listener']='pCVar12'
 elif fault=='literal':text=text.replace('pcVar15 = "TEXT_QST_B10_MOTHER_CURE_REMIND_10";','pcVar15 = "wrong";')
 elif fault=='call_duplicate':row['calls'].append(copy.deepcopy(row['calls'][0]))
 elif fault=='group':row['calls'][0]['operands'][2][1]=2
 elif fault=='offset':row['calls'][0]['offset']=88
 elif fault=='literal_pointer':row['calls'][0]['operands'][1][1]=0x12f2578
 else:
  address,data={'zero':(0xecdd51,'33ef'),'flag':(0xece012,'6a00'),'buffer_getter':(0x99e4c0,'31c0'),'speech_wrapper':(0x7e7390,'31c9')}[fault]
  rd=ChangedBytes(rd,{address:bytes.fromhex(data)});start=int(fn['address'],16);row['nativeSha256']=hashlib.sha256(rd.bytes_at(start,int(fn['bodyEndExclusive'],16)-start)).hexdigest()
  for call in row['calls']:
   begin,site=int(call['start'],16),int(call['site'],16);call['windowSha256']=hashlib.sha256(rd.bytes_at(begin,site+3-begin)).hexdigest()
 with pytest.raises(ValueError,match='reviewed member speech'):restore(text,fn,row,rd,spec)

def test_no_review_preserves_unresolved_calls(evidence):
 from tools.script_recovery.native_reviewed_member_speech import restore
 c,row=evidence[:2];assert restore(c['text'],c['fn'],None,c['rd'],c['spec'])==c['text']

def test_original_mother_reminder_pending_stack_operands():
 import unicorn as u
 from unicorn import x86_const as x
 from tools.script_recovery.lift_native_lua import RData
 from tools.script_recovery.convert_quest_unit import callee_stack_words
 rd=RData();assert callee_stack_words(0x7e7390)==6
 cpu=u.Uc(u.UC_ARCH_X86,u.UC_MODE_32)
 for a,n in ((0xece000,0x1000),(0x300000,0x10000),(0x500000,0xc000),(0x200000,0x1000)):cpu.mem_map(a,n)
 cpu.mem_write(0xece006,rd.bytes_at(0xece006,0x25));cpu.mem_write(0x200100,b'\xc3');cpu.mem_write(0x200200,b'\xc3')
 def put(a,v):cpu.mem_write(a,struct.pack('<I',v&0xffffffff))
 def word(a):return struct.unpack('<I',cpu.mem_read(a,4))[0]
 state={};rows=[]
 def hook(machine,a,size,user):
  sp=machine.reg_read(x.UC_X86_REG_ESP)
  if a==0x200100:
   assert machine.reg_read(x.UC_X86_REG_ECX)==state['gsi'] and [word(sp+4+i*4) for i in range(5)]==[0x12f2554,0,0,1,0]
   machine.reg_write(x.UC_X86_REG_EAX,state['hero']);machine.reg_write(x.UC_X86_REG_EIP,word(sp));machine.reg_write(x.UC_X86_REG_ESP,sp+4)
  elif a==0x200200:
   args=[word(sp+4+i*4) for i in range(6)];assert machine.reg_read(x.UC_X86_REG_ECX)==state['parent']+0x48 and args==[state['hero'],0x12f2554,0,0,1,0]
   state['args']=args;machine.reg_write(x.UC_X86_REG_EIP,word(sp));machine.reg_write(x.UC_X86_REG_ESP,sp+28)
 cpu.hook_add(u.UC_HOOK_CODE,hook)
 for gsi,owner,sp,parent,hero in itertools.product((0x500000,0x501000,0x502000),(0x503000,0x504000),(0x300800,0x301800),(0x505000,0x506000),(0x508000,0x509000)):
  state=dict(gsi=gsi,owner=owner,stack=sp,parent=parent,hero=hero);table=0x507000
  put(gsi,gsi+0x800);put(gsi+0x800+0x118,0x200100);put(owner+4,gsi);put(owner+0x14,parent);put(parent+0x48,table);put(table+0x34,0x200200)
  cpu.reg_write(x.UC_X86_REG_ESI,owner);cpu.reg_write(x.UC_X86_REG_EDI,0);cpu.reg_write(x.UC_X86_REG_ESP,sp);cpu.emu_start(0xece006,0xece02b,count=100)
  assert cpu.reg_read(x.UC_X86_REG_ESP)==sp and 'args' in state;rows.append(state.copy())
 assert len(rows)==48
