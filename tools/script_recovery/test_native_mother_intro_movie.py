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
 c=UnitConverter(ROOT/'refs/script_recovery/sick_child/translation_unit_typed.json');unit=json.loads((ROOT/'refs/script_recovery/sick_child/units/V_SickChild.json').read_text());row=unit['reviewedMovieMembers']['0x00ecd9e0'];original=adapter.restore;captured={}
 unit['entities']={'SickChildsMother':unit['entities']['SickChildsMother']};out=tmp_path_factory.mktemp('witch_speech')
 def capture(text,fn,review,rd):
  if fn['address'].lower()=='0x00ecd9e0':captured.update(text=text,fn=fn,rd=rd)
  return original(text,fn,review,rd)
 with patch.object(c,'constructor_defaults',return_value={}),patch.object(adapter,'restore',capture):c.convert(unit,out)
 source=(out/'FSE/V_SickChild/Entities/SickChildsMother.lua').read_text();return captured,row,dict(draft=source,readable=readable_file(source)[0]),(out/'FSE/V_SickChild/native_quest_helpers.lua').read_text()
H=r'''return function(near,potion,acquireWait,termination)
 local q,me,hero,r,mother,witch={},{},{},{},{},{}
 local boundary={};local paused=false;local frame,checks,created,destroyed=0,0,0,0
 local acquired=0;local intro=false;local macros,cards,objectives,cleared=0,0,0,0
 local maps,leases=0,0;local trace={};local function log(s)trace[#trace+1]=s end
 function q:RetailResources()return r end
 function q:GetStateBool(key)assert(key=='MotherIntroDone' or key=='FinishedQuest');return key=='MotherIntroDone' and intro end
 function q:SetStateBool(key,value)assert(key=='MotherIntroDone' and value and cards==1 and objectives==1);intro=true;log('intro:true')end
 function q:SetIsPushableByHero(actor,flag)assert(actor==me and not flag)end
 function q:NewScriptFrame(actor)assert(actor==me);frame=frame+1;assert(frame<30);return not (termination>0 and frame>=termination) end
 function q:IsActiveThreadTerminating()checks=checks+1;return termination>0 and checks>=termination end
 function q:GetHero()return hero end
 function q:ReadGlobalGameDataString(offset)assert(offset==0x744);return 'POTION' end
 function q:IsObjectInThingsPossession(key,actor)assert(key=='POTION' and actor==hero);return potion end
 function q:IsDistanceBetweenThingsUnder(a,b,distance)assert(a==me and b==hero and distance==7.0);return near or frame>=3 end
 function q:SetThingHasInformation(actor,a,b,c)assert(actor==me and not a and b and not c);log('info')end
 function q:PauseAllNonScriptedEntities(flag)assert(flag~=paused);paused=flag;log('pause:'..tostring(flag))end
 function q:FixMovieSequenceCamera(flag)log('camera:'..tostring(flag))end
 function q:GetActiveQuestName()return 'Q_SICK' end
 function q:GiveHeroQuestCardDirectly(card,quest,flag)assert(card=='OBJECT_QUEST_CARD_SICK_CHILD' and quest=='Q_SICK' and not flag and macros==1);cards=cards+1;log('card')end
 function q:SetQuestCardObjective(quest,key,a,b)assert(quest=='Q_SICK' and key=='TEXT_QUEST_SICK_CHILD_OBJECTIVE_01' and a=='' and b=='' and cards==1);objectives=objectives+1;log('objective')end
 function q:ClearThingHasInformation(actor)assert(actor==me and intro);cleared=cleared+1;log('clear')end
 function r:MemberResource(key)assert(key=='seh_Mother' or key=='seh_Witch');return key=='seh_Mother' and mother or witch end
 function r:PrepareResource(value)assert(value==mother)end
 function r:TryAcquire(value,actor,priority)
  assert(priority==4)
  if value==mother then assert(actor==me);acquired=acquired+1;return acquired>acquireWait end
  assert(value.kind=='resource' and actor==hero);value.actor=actor;return true
 end
 function r:NewResource()leases=leases+1;return {kind='resource'}end
 function r:ReleaseResource(value)assert(value.kind=='resource' and value.actor==hero);leases=leases-1;log('release')end
 function r:NewStringMap()maps=maps+1;return {kind='strings'}end
 function r:DestroyStringMap(map)assert(map.kind=='strings');maps=maps-1;log('strings:end')end
 function r:NewActorMap()maps=maps+1;return {kind='actors'}end
 function r:SetActor(map,key,value)assert(map.kind=='actors');map[key]=value end
 function r:RunMacroWithStrings(key,actors,strings,a,b)
  assert(key=='CS_SICK_INTRO' and actors.WITCH==witch and actors.MUM==mother and actors.HERO.actor==hero and strings.kind=='strings' and not a and b and maps==2 and leases==1)
  macros=macros+1;log('macro')
 end
 function r:DestroyActorMap(map)assert(map.kind=='actors');maps=maps-1;log('actors:end')end
 function r:StartMovie(key)assert(key=='' and created==0);created=1;log('start:31');return 31 end
 function r:DestroyMovie(handle)assert(handle==31 and not paused and created==1 and intro and macros==1 and cards==1 and objectives==1 and cleared==1 and maps==0 and leases==0);destroyed=destroyed+1;log('destroy:31');error(boundary)end
 function me:IsTalkedToByHero()return false end
 function me:MsgIsHitByHero()return false end
 function me:MsgIsHitByAnySpecialAbilityFromHero()return false end
 Init(q,me);OnPersist(q,me,{})
 local ok,err=pcall(Main,q,me);assert(ok or err==boundary,tostring(err));assert(not paused and created==destroyed and maps==0 and leases==0)
 if termination==0 then assert(created==1)end
 return table.concat(trace,'|'),created,destroyed,frame,checks,acquired,intro
end'''
CASES=list(itertools.product((False,True),(False,True),(0,2),(0,1,2,3,5,7,12,20)))
@pytest.mark.parametrize('style',('draft','readable'))
@pytest.mark.parametrize('case',CASES)
def test_talk_movie_cleanup_uses_started_handle(evidence,style,case):
 lua=LuaRuntime();lua.globals().helperSource=evidence[3];lua.execute("package.preload['V_SickChild.native_quest_helpers']=assert(load(helperSource))");lua.execute(evidence[2][style]);lua.execute(H)(*case)

@pytest.mark.parametrize('fault',('native','source','anchor','count','address_policy','constructor','ctor_receiver','start_args','printed_receiver','collision','duplicate','table','receiver_load','table_load','owner','receiver_clobber'))
def test_stale_or_ambiguous_movie_review_fails(evidence,fault):
 from tools.script_recovery.native_reviewed_movie_members import restore
 from tools.script_recovery.test_native_inline_self_argument import ChangedBytes
 from capstone import Cs,CS_ARCH_X86,CS_MODE_32
 c,row=evidence[:2];row=copy.deepcopy(row);fn=copy.deepcopy(c['fn']);rd=c['rd'];text=c['text'];alias=row['stackReceiverAliases'][2]
 if fault=='native':row['nativeSha256']='0'*64
 elif fault=='source':fn['decompile']+='\n'
 elif fault=='anchor':row['nativeInstructions']['0x00ECDB6E']='90'
 elif fault=='count':alias['count']=4
 elif fault=='address_policy':alias['allowAddressOf']='yes'
 elif fault=='constructor':alias['constructorTarget']='0x0099a380'
 elif fault=='ctor_receiver':alias['constructorReceiver']='wrongMovie'
 elif fault=='start_args':alias['startOperands']['sourceArgs']=alias['startOperands']['sourceArgs'][1:]
 elif fault=='printed_receiver':alias['startOperands']['printedInterfaceReceiver']='*(int **)((int)this + 8)'
 elif fault=='collision':text+='\nmotherIntroMovie;'
 elif fault=='duplicate':row['stackReceiverAliases'].append(copy.deepcopy(alias))
 elif fault=='table':rd=ChangedBytes(rd,{0x1260f0c+0x5c8:bytes.fromhex('00000000')})
 elif fault=='zero_sites':alias['inlineConstructor']['zeroSites']=[]
 elif fault=='zero_registers':alias['inlineConstructor']['zeroRegisters']=['ebx']
 elif fault=='array_policy':alias['inlineConstructor']['sourceIndexZero']='yes'
 elif fault=='array_source':alias['inlineConstructor']['sourceIndexZero']=False
 elif fault=='ctor_source':alias['inlineConstructor']['source']='wrong;'
 else:
  owner=next(i.address for i in Cs(CS_ARCH_X86,CS_MODE_32).disasm(rd.bytes_at(int(fn['address'],16),40),int(fn['address'],16)) if i.mnemonic=='mov' and i.op_str=='esi, ecx')
  replacements={'receiver_load':{0xecdb8a:bytes.fromhex('8b4e08')},'table_load':{0xecdb8d:bytes.fromhex('8b12')},'owner':{owner:bytes.fromhex('8bf9')},'receiver_clobber':{0xecdb8f:bytes.fromhex('8d8c2480000000'),0xecdb96:bytes.fromhex('51')},'zero_native':{0xecdd51:bytes.fromhex('33db')},'store_native':{0xecdd5c:bytes.fromhex('c7442428f00e2601')},'pending_native':{0xecdd4f:bytes.fromhex('6a00')}}[fault]
  rd=ChangedBytes(rd,replacements);start=int(fn['address'],16);row['nativeSha256']=hashlib.sha256(rd.bytes_at(start,int(fn['bodyEndExclusive'],16)-start)).hexdigest()
  for address,data in replacements.items():
   key=f'0x{address:08X}'
   if key in row['nativeInstructions']:row['nativeInstructions'][key]=data.hex()
 with pytest.raises(ValueError,match='reviewed movie members'):restore(text,fn,row,rd)

def test_no_review_preserves_original_operands(evidence):
 from tools.script_recovery.native_reviewed_movie_members import restore
 c,row=evidence[:2];assert restore(c['text'],c['fn'],None,c['rd'])==c['text']

def test_original_mother_intro_movie_constructor_and_cleanup():
 import unicorn as u
 from unicorn import x86_const as x
 from tools.script_recovery.lift_native_lua import RData
 from tools.script_recovery.convert_quest_unit import callee_stack_words
 rd=RData();target=int.from_bytes(rd.bytes_at(0x1260f0c+0x5c8,4),'little');assert target==0x89b110 and callee_stack_words(target)==2
 cpu=u.Uc(u.UC_ARCH_X86,u.UC_MODE_32)
 for a,n in ((0xecd000,0x2000),(0x6e7000,0x1000),(0x99a000,0x5000),(0x300000,0x10000),(0x500000,0x8000),(0x200000,0x1000)):cpu.mem_map(a,n)
 cpu.mem_write(0xecd000,rd.bytes_at(0xecd000,0x1800));helpers={a:rd.bytes_at(a,n) for a,n in ((0x6e7b60,26),(0x6e7b80,66),(0x6e7ab0,53),(0x99a380,16),(0x99a430,64))}
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
 groups=[(0xecdb6e,0xecdba2,0x80,0x18,[(0xecdcca,0xecdcd6)])]
 for gsi,owner,sp,cookie in itertools.product((0x500000,0x501000,0x502000),(0x503000,0x504000),(0x300800,0x301800),(0,1,713,0x12345678)):
  table=gsi+0x800;put(gsi,table);put(table+0x5c8,0x200100);put(owner+4,gsi)
  for begin,end,movie,string,cleanups in groups:
   for a,b in cleanups:
    state=dict(gsi=gsi,owner=owner,sp=sp,cookie=cookie,movie=movie,string=string,starts=0,ends=0);cpu.mem_write(sp,b'\xa5'*0xa0);cpu.reg_write(x.UC_X86_REG_ESI,owner);cpu.reg_write(x.UC_X86_REG_ESP,sp)
    cpu.emu_start(begin,end,count=300);assert cpu.reg_read(x.UC_X86_REG_ESP)==sp and word(sp+movie+8)==cookie
    cpu.emu_start(a,b,count=300);assert cpu.reg_read(x.UC_X86_REG_ESP)==sp and [word(sp+movie+i) for i in (4,8,12)]==[0,0,0] and state['starts']==state['ends']==1
    rows.append(dict(state,start=hex(begin),cleanup=hex(a),movie_entry_slot=movie-184,string_entry_slot=string-184))
 assert len(rows)==48
