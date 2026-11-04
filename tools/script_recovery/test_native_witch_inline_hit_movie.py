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
 from tools.script_recovery import native_reviewed_movie_members as adapter
 c=UnitConverter(ROOT/'refs/script_recovery/sick_child/translation_unit_typed.json');unit=json.loads((ROOT/'refs/script_recovery/sick_child/units/V_SickChild.json').read_text());row=unit['reviewedMovieMembers']['0x00ece7f0'];original=adapter.restore;captured={}
 unit['entities']={'Witch':unit['entities']['Witch']};out=tmp_path_factory.mktemp('witch_movie')
 def capture(text,fn,review,rd):
  if fn['address'].lower()=='0x00ece7f0':captured.update(text=text,fn=fn,rd=rd)
  return original(text,fn,review,rd)
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
 function r:DestroyMovie(handle)assert(not paused and created==1);assert(handle==31,'wrong movie handle');destroyed=destroyed+1;cleanupHandle=handle;error(boundary)end
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
CASES=[case for case in CASES if case[0]=='hit_before']

@pytest.mark.parametrize('style',('draft','readable'))
@pytest.mark.parametrize('case',CASES)
def test_later_movie_cleanup_identity(evidence,style,case):
 lua=LuaRuntime();lua.execute(evidence[2][style]);lua.execute(H)(*case)
@pytest.mark.parametrize('fault',('native','source','anchor','count','address_policy','scope','phi','start_args','printed_receiver','collision','duplicate','table','receiver_load','table_load','owner','receiver_clobber'))
def test_stale_or_ambiguous_movie_review_fails(evidence,fault):
 from tools.script_recovery.native_reviewed_movie_members import restore
 from tools.script_recovery.test_native_inline_self_argument import ChangedBytes
 c,row,_=evidence;row=copy.deepcopy(row);fn=copy.deepcopy(c['fn']);rd=c['rd'];text=c['text'];alias=row['stackReceiverAliases'][2]
 if fault=='native':row['nativeSha256']='0'*64
 elif fault=='source':fn['decompile']+='\n'
 elif fault=='anchor':row['nativeInstructions']['0x00ECF016']='90'
 elif fault=='count':alias['count']=4
 elif fault=='address_policy':alias['allowAddressOf']='yes'
 elif fault=='scope':alias['scopeStart']='missing;'
 elif fault=='phi':row['stackReceiverAliases'][1]['phiCount']=2
 elif fault=='start_args':alias['startOperands']['sourceArgs']=alias['startOperands']['sourceArgs'][1:]
 elif fault=='printed_receiver':alias['startOperands']['printedInterfaceReceiver']='*(int **)((int)this + 8)'
 elif fault=='collision':text+='\nwitchBeforeCureHitMovie;'
 elif fault=='duplicate':row['stackReceiverAliases'].append(copy.deepcopy(alias))
 elif fault=='table':rd=ChangedBytes(rd,{0x1260f0c+0x5c8:bytes.fromhex('00000000')})
 else:
  replacements={'receiver_load':{0xecf03f:bytes.fromhex('8b4e08')},'table_load':{0xecf042:bytes.fromhex('8b12')},'owner':{0xece7f9:bytes.fromhex('8bf9')},'receiver_clobber':{0xecf044:bytes.fromhex('8d4c2448'),0xecf048:bytes.fromhex('51')}}[fault]
  rd=ChangedBytes(rd,replacements)
  start=int(fn['address'],16);row['nativeSha256']=hashlib.sha256(rd.bytes_at(start,int(fn['bodyEndExclusive'],16)-start)).hexdigest()
  for address,data in replacements.items():row['nativeInstructions'][f'0x{address:08X}']=data.hex()
 with pytest.raises(ValueError,match='reviewed movie members'):restore(text,fn,row,rd)
@pytest.mark.parametrize('fault',('store','pending','member','release','base','ctor_source','cleanup_source','cleanup_count','phi_early','scope_end'))
def test_inline_movie_review_rejects_changed_witness(evidence,fault):
 from tools.script_recovery.native_reviewed_movie_members import restore
 from tools.script_recovery.test_native_inline_self_argument import ChangedBytes
 c,row,_=evidence;row=copy.deepcopy(row);fn=copy.deepcopy(c['fn']);rd=c['rd'];text=c['text'];alias=row['stackReceiverAliases'][2]
 if fault=='ctor_source':alias['inlineConstructor']['source']='wrong;'
 elif fault=='cleanup_source':alias['inlineBaseCleanups']['sourceCast']='(CPhysicsMeshInfo *)&xStack_70'
 elif fault=='cleanup_count':alias['inlineBaseCleanups']['sites'].pop()
 elif fault=='phi_early':alias['phiCount']=2
 elif fault=='scope_end':alias['scopeEnd']='missing;'
 else:
  replacements={'store':{0xecf02a:bytes.fromhex('c7442450f00e2601')},'pending':{0xecf01f:bytes.fromhex('6a00')},'member':{0xecf183:bytes.fromhex('8d4c2454')},'release':{0xecf187:bytes.fromhex('e8f41cc2ff')},'base':{0xecf190:bytes.fromhex('c74424488c002701')}}[fault]
  rd=ChangedBytes(rd,replacements);start=int(fn['address'],16);row['nativeSha256']=hashlib.sha256(rd.bytes_at(start,int(fn['bodyEndExclusive'],16)-start)).hexdigest()
  for address,data in replacements.items():row['nativeInstructions'][f'0x{address:08X}']=data.hex()
 with pytest.raises(ValueError,match='reviewed movie members'):restore(text,fn,row,rd)

def test_no_review_preserves_original_movie_operands(evidence):
 from tools.script_recovery.native_reviewed_movie_members import restore
 c,row,_=evidence;assert restore(c['text'],c['fn'],None,c['rd'])==c['text']

def test_original_later_movie_constructors_and_null_ref_cleanup():
 import unicorn as u
 from unicorn import x86_const as x
 from tools.script_recovery.lift_native_lua import RData
 from tools.script_recovery.convert_quest_unit import callee_stack_words
 rd=RData();target=int.from_bytes(rd.bytes_at(0x1260f0c+0x5c8,4),'little');assert target==0x89b110 and callee_stack_words(target)==2
 cpu=u.Uc(u.UC_ARCH_X86,u.UC_MODE_32)
 for a,n in ((0xecf000,0x1000),(0x6e7000,0x1000),(0x99a000,0x5000),(0x300000,0x10000),(0x500000,0x8000),(0x200000,0x1000)):cpu.mem_map(a,n)
 cpu.mem_write(0xecf000,rd.bytes_at(0xecf000,0x800));helpers={a:rd.bytes_at(a,n) for a,n in ((0x6e7b60,26),(0x6e7b80,66),(0x6e7ab0,53),(0x99a380,16),(0x99a430,64))}
 for a,raw in helpers.items():cpu.mem_write(a,raw)
 cpu.mem_write(0x99ebf0,b'\xc3');cpu.mem_write(0x200100,b'\xc3')
 def put(a,v):cpu.mem_write(a,struct.pack('<I',v&0xffffffff))
 def word(a):return struct.unpack('<I',cpu.mem_read(a,4))[0]
 state={};rows=[]
 def hook(machine,a,size,user):
  if a==0x99ebf0:
   sp=machine.reg_read(x.UC_X86_REG_ESP);obj=machine.reg_read(x.UC_X86_REG_ECX);assert obj==state['sp']+state['string'] and [word(sp+4),word(sp+8)]==[0x122d70e,0xffffffff]
   put(obj,0);machine.reg_write(x.UC_X86_REG_EAX,obj);machine.reg_write(x.UC_X86_REG_EIP,word(sp));machine.reg_write(x.UC_X86_REG_ESP,sp+12)
  elif a==0x200100:
   sp=machine.reg_read(x.UC_X86_REG_ESP);assert machine.reg_read(x.UC_X86_REG_ECX)==state['gsi'] and [word(sp+4),word(sp+8)]==[state['sp']+state['string'],state['sp']+state['movie']]
   obj=word(sp+8);assert [word(obj+i) for i in (0,4,8,12)]==[0x1260ef4,0,0,0];put(obj+8,state['cookie']);state['starts']+=1
   machine.reg_write(x.UC_X86_REG_EIP,word(sp));machine.reg_write(x.UC_X86_REG_ESP,sp+12)
  elif a in (0x6e7b80,0x6e7ab0):assert machine.reg_read(x.UC_X86_REG_ECX)==state['sp']+state['movie']+(8 if a==0x6e7ab0 else 0);state['ends']+=1
 cpu.hook_add(u.UC_HOOK_CODE,hook)
 groups=[(0xecf016,0xecf054,0x48,0x28,[(0xecf183,0xecf19d),(0xecf412,0xecf42c),(0xecf44c,0xecf7d2)])]
 for gsi,owner,sp,cookie in itertools.product((0x500000,0x501000,0x502000),(0x503000,0x504000),(0x300800,0x301800),(0,1,713,0x12345678)):
  table=gsi+0x800;put(gsi,table);put(table+0x5c8,0x200100);put(owner+4,gsi)
  for begin,end,movie,string,cleanups in groups:
   for a,b in cleanups:
    state=dict(gsi=gsi,owner=owner,sp=sp,cookie=cookie,movie=movie,string=string,starts=0,ends=0);cpu.mem_write(sp,b'\xa5'*0xa0);cpu.reg_write(x.UC_X86_REG_ESI,owner);cpu.reg_write(x.UC_X86_REG_ESP,sp)
    cpu.emu_start(begin,end,count=300);assert cpu.reg_read(x.UC_X86_REG_ESP)==sp and word(sp+movie+8)==cookie
    cpu.emu_start(a,b,count=300);assert cpu.reg_read(x.UC_X86_REG_ESP)==sp and [word(sp+movie+i) for i in (4,8,12)]==[0,0,0] and state['starts']==state['ends']==1
    rows.append(dict(state,start=hex(begin),cleanup=hex(a),movie_entry_slot=movie-184,string_entry_slot=string-184))
 assert len(rows)==144
