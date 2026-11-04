import copy,hashlib,itertools,json,re,struct,sys
from pathlib import Path
from unittest.mock import patch
import pytest
from lupa.lua54 import LuaRuntime
ROOT=next(p for p in Path(__file__).resolve().parents if (p/'tools/script_recovery/script_units.py').exists());sys.path.insert(0,str(ROOT))
from tools.script_recovery.convert_quest_unit import UnitConverter
from tools.script_recovery.build_readable_unit import readable_file
@pytest.fixture(scope='module')
def evidence(tmp_path_factory):
 from tools.script_recovery import native_reviewed_stack_aliases as adapter
 from tools.script_recovery import native_reviewed_movie_members as movie
 c=UnitConverter(ROOT/'refs/script_recovery/sick_child/translation_unit_typed.json');unit=json.loads((ROOT/'refs/script_recovery/sick_child/units/V_SickChild.json').read_text());row=unit['reviewedStackScalarAliases']['0x00ec8300'];original=adapter.restore;captured={};capturedMovie={};originalMovie=movie.restore;movieRow=unit['reviewedMovieMembers']['0x00ec8300']
 unit['entities']={'ManInLove':unit['entities']['ManInLove']};out=tmp_path_factory.mktemp('man_letter')
 def capture(text,fn,review,rd):
  if fn['address'].lower()=='0x00ec8300':captured.update(text=text,fn=fn,rd=rd)
  return original(text,fn,review,rd)
 def captureMovie(text,fn,review,rd):
  if fn['address'].lower()=='0x00ec8300':capturedMovie.update(text=text,fn=fn,rd=rd)
  return originalMovie(text,fn,review,rd)
 with patch.object(movie,'restore',captureMovie),patch.object(c,'constructor_defaults',return_value={}),patch.object(adapter,'restore',capture):c.convert(unit,out)
 source=(out/'FSE/V_SickChild/Entities/ManInLove.lua').read_text();return captured,row,dict(draft=source,readable=readable_file(source)[0]),capturedMovie,movieRow
H=r'''return function(letter,womansLetter,gotReward,acquireWait,termination,handle,mode)
 local q,me,hero,r={},{},{},{};local boundary={};local paused=false
 local frame,checks,created,destroyed,attempts,released,queries=0,0,0,0,0,0,0
 local resource=nil;local branch='none';local asked=false;local trace={}
 local expected=(letter or womansLetter) and not gotReward
 function q:RetailResources()return r end
 function q:PersistTransferBool(ctx,name,value)return name=='GivenLetter' or (name=='GotReward' and gotReward)end
 function q:SetThingPersistent(actor,value)assert(actor==me and value)end
 function q:NewScriptFrame(actor)assert(actor==me);frame=frame+1;assert(frame<30);return not (termination>0 and frame>=termination)end
 function q:IsActiveThreadTerminating()checks=checks+1;return termination>0 and checks>=termination end
 function q:GetHero()return hero end
 function q:ReadGlobalGameDataString(offset)assert(offset==0x740);return 'LETTER' end
 function q:IsObjectInThingsPossession(value,actor)assert(actor==hero);queries=queries+1;assert(value=='LETTER' or value=='OBJECT_SICK_CHILD_WOMANS_HERO_LETTER');return value=='LETTER' and letter or (value=='OBJECT_SICK_CHILD_WOMANS_HERO_LETTER' and womansLetter)end
 function q:GetStateInt(name)assert(name=='MansLoverState');return 3 end
 function q:SetThingHasInformation(actor,a,b,c)assert(actor==me and a and b and not c)end
 function q:SetIsPushableByHero(actor,value)assert(actor==me and not value)end
 function q:SetCreatureBrain(actor,name)assert(actor==me and name=='BRAIN_PASSIVE_OVERRIDE')end
 function q:SetWanderCentrePoint(actor,pos)assert(actor==me and pos.x==1)end
 function q:SetWanderMinDistance(actor,value)assert(actor==me and value==0)end
 function q:SetWanderMaxDistance(actor,value)assert(actor==me and value==10)end
 function q:SetScriptingStateGroup(actor,value)assert(actor==me and value==4)end
 function q:PauseAllNonScriptedEntities(value)assert(value~=paused);paused=value;trace[#trace+1]='pause:'..tostring(value)end
 function q:GetHealth(actor)assert(actor==me);return 1 end
 function q:GiveHeroYesNoQuestion(text,yes,no,extra,flag)
  assert(expected and text=='TEXT_QST_B10_MAN_IN_LOVE_RETURNING_OBJECT_QUESTION' and yes=='TEXT_OBJECT_HERO_ANSWER_YES' and no=='TEXT_OBJECT_HERO_ANSWER_NO' and extra=='' and flag);branch='question';asked=true
  if mode=='boundary' then error(boundary)end
 end
 function q:MsgIsQuestionAnsweredYesOrNo()assert(asked);return 0 end
 function me:GetPos()return {x=1,y=2,z=3}end
 function me:IsTalkedToByHero()return true end
 function me:ClearCommands()assert(gotReward and not expected and not asked)end
 function me:Speak(actor,text,group,a,b,c)
  assert(actor==hero and group==0 and not a and b and not c)
  if asked then assert(expected and text=='TEXT_QST_B10_MAN_IN_LOVE_DIDNT_RETURN_OBJECT_TO_ME')
  elseif gotReward then assert(not expected and text=='TEXT_QST_B10_MAN_IN_LOVE_POST_QUEST_MYRA_DIED_10');branch='post'
  else assert(not expected and text=='TEXT_QST_B10_MAN_IN_LOVE_MYRA_DIED');branch='dead' end
  if mode=='boundary' then error(boundary)end
  return false
 end
 function r:NewResource()assert(not resource);resource={};return resource end
 function r:PrepareResource(value)assert(value==resource)end
 function r:TryAcquire(value,actor,priority)assert(value==resource and actor==me and priority==4);attempts=attempts+1;return attempts>acquireWait end
 function r:ReleaseResource(value)assert(value==resource);released=released+1;trace[#trace+1]='release' end
 function r:StartMovie(name)assert(name=='');created=created+1;return handle end
 function r:ResourceThingHealthAbove(value,minimum)assert(minimum==0.0);return q:GetHealth(self:ScriptThing(value))>minimum end
 function r:ScriptThing(value)assert(value==resource);return me end
 function r:DestroyMovie(value)assert(value==handle and not paused,'wrong talk movie handle');destroyed=destroyed+1;trace[#trace+1]='destroy:'..tostring(value)end
 Init(q,me);OnPersist(q,me,{})
 local ok,err=pcall(Main,q,me);assert(ok or err==boundary,tostring(err))
 if err==boundary then assert(mode=='boundary' and paused and created==1 and released==0 and destroyed==0)
 else assert(not paused and created==destroyed and released==(resource and 1 or 0))end
 if termination==0 then assert(branch~='none' and created==1)end
 return branch,frame,checks,queries,attempts,created,destroyed,released,table.concat(trace,'|')
end'''
CASES=list(itertools.product((False,True),(False,True),(False,True),(0,2),(0,1,2,3,5,7,12,20),(0,31,713),('boundary','cancel')))
@pytest.mark.parametrize('style',('draft','readable'))
@pytest.mark.parametrize('case',CASES)
def test_talk_possession_branch_and_handle(evidence,style,case):
 lua=LuaRuntime();lua.execute(evidence[2][style]);lua.execute(H)(*case)
@pytest.mark.parametrize('fault',('native','source','anchor','few_anchors','collision','duplicate_alias','duplicate_edit','missing_edit','one_edit','wrong_alias','count','bool_count'))
def test_stale_or_ambiguous_letter_review_fails(evidence,fault):
 from tools.script_recovery.native_reviewed_stack_aliases import restore
 c,row=evidence[:2];row=copy.deepcopy(row);fn=copy.deepcopy(c['fn']);text=c['text']
 if fault=='native':row['nativeSha256']='0'*64
 elif fault=='source':fn['decompile']+='\n'
 elif fault=='anchor':row['nativeInstructions']['0x00EC8A18']='90'
 elif fault=='few_anchors':row['nativeInstructions']={}
 elif fault=='collision':text+='\nlocal_manLetterDefinition;'
 elif fault=='duplicate_alias':row['additionalAliases']=[row['alias']]
 elif fault=='duplicate_edit':row['edits'].append(row['edits'][0])
 elif fault=='missing_edit':row['edits'][0][0]='missing_exported_operand;'
 elif fault=='one_edit':row['edits']=row['edits'][:1]
 elif fault=='wrong_alias':row['edits'][0][1]='unreviewed;'
 elif fault=='count':row['editCounts']=[2,1,1]
 elif fault=='bool_count':row['editCounts']=[True,1,1]
 with pytest.raises(ValueError):restore(text,fn,row,c['rd'])

def test_unreviewed_source_is_preserved(evidence):
 from tools.script_recovery.native_reviewed_stack_aliases import restore
 c=evidence[0];assert restore(c['text'],c['fn'],None,c['rd'])==c['text']

@pytest.mark.parametrize('fault',('first_output','remove_output','second_output','missing_alias','duplicate_reward_alias','extra_edit_count'))
def test_stale_reward_operand_review_fails(evidence,fault):
 from tools.script_recovery.native_reviewed_stack_aliases import restore
 c,row=evidence[:2];row=copy.deepcopy(row);fn=copy.deepcopy(c['fn']);text=c['text']
 if fault in ('first_output','remove_output','second_output'):
  site={'first_output':'0x00EC9319','remove_output':'0x00EC8D16','second_output':'0x00EC900A'}[fault];row['nativeInstructions'][site]='90'
 elif fault=='missing_alias':row['additionalAliases']=row['additionalAliases'][:-1]
 elif fault=='duplicate_reward_alias':row['additionalAliases'].append(row['additionalAliases'][0])
 elif fault=='extra_edit_count':row['editCounts']=[1]*(len(row['edits'])-1)
 with pytest.raises(ValueError):restore(text,fn,row,c['rd'])

@pytest.mark.parametrize('fault',('native','source','anchor','count','address_policy','constructor','ctor_receiver','start_args','printed_receiver','collision','duplicate','table','receiver_load','table_load','owner','receiver_clobber'))
def test_stale_or_ambiguous_talk_movie_review_fails(evidence,fault):
 from tools.script_recovery.native_reviewed_movie_members import restore
 from tools.script_recovery.test_native_inline_self_argument import ChangedBytes
 from capstone import Cs,CS_ARCH_X86,CS_MODE_32
 c,row=evidence[3:5];row=copy.deepcopy(row);fn=copy.deepcopy(c['fn']);rd=c['rd'];text=c['text'];alias=row['stackReceiverAliases'][0]
 if fault=='native':row['nativeSha256']='0'*64
 elif fault=='source':fn['decompile']+='\n'
 elif fault=='anchor':row['nativeInstructions']['0x00EC89C2']='90'
 elif fault=='count':alias['count']=4
 elif fault=='address_policy':alias['allowAddressOf']='yes'
 elif fault=='constructor':alias['constructorTarget']='0x0099a380'
 elif fault=='ctor_receiver':alias['constructorReceiver']='wrongMovie'
 elif fault=='start_args':alias['startOperands']['sourceArgs']=alias['startOperands']['sourceArgs'][1:]
 elif fault=='printed_receiver':alias['startOperands']['printedInterfaceReceiver']='*(int **)((int)this + 8)'
 elif fault=='collision':text+='\nmanTalkMovie;'
 elif fault=='duplicate':row['stackReceiverAliases'].append(copy.deepcopy(alias))
 elif fault=='table':rd=ChangedBytes(rd,{0x1260f0c+0x5c8:bytes.fromhex('00000000')})
 elif fault=='zero_sites':alias['inlineConstructor']['zeroSites']=[]
 elif fault=='zero_registers':alias['inlineConstructor']['zeroRegisters']=['ebx']
 elif fault=='array_policy':alias['inlineConstructor']['sourceIndexZero']='yes'
 elif fault=='array_source':alias['inlineConstructor']['sourceIndexZero']=False
 elif fault=='ctor_source':alias['inlineConstructor']['source']='wrong;'
 else:
  owner=next(i.address for i in Cs(CS_ARCH_X86,CS_MODE_32).disasm(rd.bytes_at(int(fn['address'],16),40),int(fn['address'],16)) if i.mnemonic=='mov' and i.op_str=='esi, ecx')
  replacements={'receiver_load':{0xec89de:bytes.fromhex('8b4e08')},'table_load':{0xec89e1:bytes.fromhex('8b12')},'owner':{owner:bytes.fromhex('8bf9')},'receiver_clobber':{0xec89e3:bytes.fromhex('8d4c2428'),0xec89e7:bytes.fromhex('51')},'zero_native':{0xecdd51:bytes.fromhex('33db')},'store_native':{0xecdd5c:bytes.fromhex('c7442428f00e2601')},'pending_native':{0xecdd4f:bytes.fromhex('6a00')}}[fault]
  rd=ChangedBytes(rd,replacements);start=int(fn['address'],16);row['nativeSha256']=hashlib.sha256(rd.bytes_at(start,int(fn['bodyEndExclusive'],16)-start)).hexdigest()
  for address,data in replacements.items():
   key=f'0x{address:08X}'
   if key in row['nativeInstructions']:row['nativeInstructions'][key]=data.hex()
 with pytest.raises(ValueError,match='reviewed movie members'):restore(text,fn,row,rd)

def test_unreviewed_movie_source_is_preserved(evidence):
 from tools.script_recovery.native_reviewed_movie_members import restore
 c=evidence[3];assert restore(c['text'],c['fn'],None,c['rd'])==c['text']
I=r'''return function(heroHit,specialHit,excluded,wait,termination)
 local q,me,hero,r={},{},{},{};local boundary={};local frame,created,released,attempts,queries=0,0,0,0,0;local resource
 local expected=heroHit or (specialHit and not excluded)
 function q:RetailResources()return r end
 function q:PersistTransferBool(ctx,name,value)return name=='GivenLetter' end
 function q:SetThingPersistent(actor,value)assert(actor==me and value)end
 function q:NewScriptFrame(actor)assert(actor==me);frame=frame+1;assert(frame<20);return termination==0 or frame<termination end
 function q:IsActiveThreadTerminating()return frame>=(termination==0 and 8 or termination)end
 function q:GetHero()return hero end
 function q:ReadGlobalGameDataString(offset)assert(offset==0x740);return 'LETTER' end
 function q:IsObjectInThingsPossession(value,actor)assert(actor==hero);return false end
 function q:GetStateInt(name)assert(name=='MansLoverState');return 3 end
 function q:SetThingHasInformation(actor,a,b,c)assert(actor==me and a and b and not c)end
 function q:SetIsPushableByHero(actor,value)assert(actor==me and not value)end
 function q:SetCreatureBrain(actor,name)assert(actor==me and name=='BRAIN_PASSIVE_OVERRIDE')end
 function q:SetWanderCentrePoint(actor,pos)assert(actor==me and pos.x==1)end
 function q:SetWanderMinDistance(actor,value)assert(actor==me and value==0)end
 function q:SetWanderMaxDistance(actor,value)assert(actor==me and value==10)end
 function q:SetScriptingStateGroup(actor,value)assert(actor==me and value==4)end
 function me:GetPos()return {x=1,y=2,z=3}end
 function me:IsTalkedToByHero()return false end
 function me:ClearCommands()assert(expected)end
 function me:MsgIsHitByHero()queries=queries+1;return heroHit end
 function me:MsgIsHitByAnySpecialAbilityFromHero()assert(not heroHit);queries=queries+1;return specialHit end
 function me:MsgIsHitByHeroSpecialAbility(id)assert(id==14 and not heroHit and specialHit);queries=queries+1;return excluded end
 function r:NewResource()assert(not resource);resource={};return resource end
 function r:PrepareResource(value)assert(value==resource)end
 function r:TryAcquire(value,actor,priority)assert(value==resource and actor==me and priority==4);attempts=attempts+1;return attempts>wait end
 function r:ReleaseResource(value)assert(value==resource);released=released+1 end
 function r:StartMovie(name)assert(name=='' and expected,'unexpected hit movie');created=created+1;error(boundary)end
 Init(q,me);OnPersist(q,me,{})
 local ok,err=pcall(Main,q,me);assert(ok or err==boundary,tostring(err))
 if err==boundary then assert(created==1 and released==0 and expected)else assert(created==0 and released==(resource and 1 or 0))end
 if termination==0 then assert((created==1)==expected)end
 return created,frame,released,attempts,queries
end'''
IDLE_CASES=list(itertools.product((False,True),(False,True),(False,True),(0,2),(0,1,3,5)))
@pytest.mark.parametrize('style',('draft','readable'))
@pytest.mark.parametrize('case',IDLE_CASES)
def test_idle_hit_decision_and_excluded_ability(evidence,style,case):
 lua=LuaRuntime();lua.execute(evidence[2][style]);lua.execute(I)(*case)

@pytest.mark.parametrize('site',('0x00EC976D','0x00EC9774','0x00EC97C1'))
def test_stale_idle_byte_witness_fails(evidence,site):
 from tools.script_recovery.native_reviewed_stack_aliases import restore
 c,row=evidence[:2];row=copy.deepcopy(row);row['nativeInstructions'][site]='90'
 with pytest.raises(ValueError):restore(c['text'],c['fn'],row,c['rd'])

def test_original_talk_movie_lifetime():
 import unicorn as u
 from unicorn import x86_const as x
 from tools.script_recovery.lift_native_lua import RData
 from tools.script_recovery.convert_quest_unit import callee_stack_words
 rd=RData();target=int.from_bytes(rd.bytes_at(0x1260f0c+0x5c8,4),'little');assert target==0x89b110 and callee_stack_words(target)==2
 cpu=u.Uc(u.UC_ARCH_X86,u.UC_MODE_32)
 for a,n in ((0xec8000,0x3000),(0x6e7000,0x1000),(0x99a000,0x5000),(0x300000,0x10000),(0x500000,0x8000),(0x200000,0x1000)):cpu.mem_map(a,n)
 cpu.mem_write(0xec8000,rd.bytes_at(0xec8000,0x3000));helpers={a:rd.bytes_at(a,n) for a,n in ((0x6e7b60,26),(0x6e7b80,66),(0x6e7ab0,53),(0x99a380,16),(0x99a430,64))}
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
 groups=[(0xec89c2,0xec89f6,0x28,0xa8,[(0xec95e0,0xec95e9),(0xec96c0,0xec96c9)])]
 for gsi,owner,sp,cookie in itertools.product((0x500000,0x501000,0x502000),(0x503000,0x504000),(0x300800,0x301800),(0,1,713,0x12345678)):
  table=gsi+0x800;put(gsi,table);put(table+0x5c8,0x200100);put(owner+4,gsi)
  for begin,end,movie,string,cleanups in groups:
   for a,b in cleanups:
    state=dict(gsi=gsi,owner=owner,sp=sp,cookie=cookie,movie=movie,string=string,starts=0,ends=0);cpu.mem_write(sp,b'\xa5'*0x180);cpu.reg_write(x.UC_X86_REG_ESI,owner);cpu.reg_write(x.UC_X86_REG_ESP,sp)
    cpu.emu_start(begin,end,count=300);assert cpu.reg_read(x.UC_X86_REG_ESP)==sp and word(sp+movie+8)==cookie
    cpu.emu_start(a,b,count=300);assert cpu.reg_read(x.UC_X86_REG_ESP)==sp and [word(sp+movie+i) for i in (4,8,12)]==[0,0,0] and state['starts']==state['ends']==1
    rows.append(dict(state,start=hex(begin),cleanup=hex(a),movie_entry_slot=movie-384,string_entry_slot=string-384))
 assert len(rows)==96

def test_original_possession_and_flag_branch():
 import unicorn as u
 from unicorn import x86_const as x
 from tools.script_recovery.lift_native_lua import RData
 from tools.script_recovery.convert_quest_unit import callee_stack_words
 rd=RData();assert callee_stack_words(0x415d70)==1 and callee_stack_words(0x99ebf0)==2 and callee_stack_words(0x99eae0)==0
 cpu=u.Uc(u.UC_ARCH_X86,u.UC_MODE_32)
 for a,n in ((0xec8000,0x3000),(0x415000,0x1000),(0x99e000,0x1000),(0x143e000,0x1000),(0x300000,0x10000),(0x500000,0xc000),(0x200000,0x1000)):cpu.mem_map(a,n)
 cpu.mem_write(0xec8300,rd.bytes_at(0xec8300,0x28));cpu.mem_write(0xec8a12,rd.bytes_at(0xec8a12,0xbd))
 for address in (0x415d70,0x99ebf0,0x99eae0,0x200100,0x200200):cpu.mem_write(address,b'\xc3')
 def put(a,v):cpu.mem_write(a,struct.pack('<I',v&0xffffffff))
 def word(a):return struct.unpack('<I',cpu.mem_read(a,4))[0]
 state={};rows=[]
 def returned(machine,sp,purge,value=0):
  machine.reg_write(x.UC_X86_REG_EAX,value);machine.reg_write(x.UC_X86_REG_EIP,word(sp));machine.reg_write(x.UC_X86_REG_ESP,sp+4+purge)
 def hook(machine,a,size,user):
  sp=machine.reg_read(x.UC_X86_REG_ESP)
  if a in (0xec8acf,0xec9227):state['question']=a==0xec8acf;machine.emu_stop()
  elif a==0x415d70:
   dst=word(sp+4);assert machine.reg_read(x.UC_X86_REG_ECX)==state['model']+0x740 and dst==state['stack']+0x48;put(dst,0);returned(machine,sp,4,dst)
  elif a==0x99ebf0:
   assert machine.reg_read(x.UC_X86_REG_ECX)==state['stack']+0x64 and [word(sp+4),word(sp+8)]==[0x12f1d4c,0xffffffff];put(state['stack']+0x64,0);returned(machine,sp,8)
  elif a==0x200100:
   assert machine.reg_read(x.UC_X86_REG_ECX)==state['gsi'];returned(machine,sp,0,state['hero'])
  elif a==0x200200:
   assert machine.reg_read(x.UC_X86_REG_ECX)==state['gsi'] and word(sp+8)==state['hero'];dst=word(sp+4);assert dst in (state['stack']+0x48,state['stack']+0x64)
   value=state['letter'] if dst==state['stack']+0x48 else state['woman'];state['queries']+=1;returned(machine,sp,8,int(value))
  elif a==0x99eae0:
   assert machine.reg_read(x.UC_X86_REG_ECX) in (state['stack']+0x48,state['stack']+0x64);state['cleanups']+=1;returned(machine,sp,0)
 cpu.hook_add(u.UC_HOOK_CODE,hook)
 for owner,entry in itertools.product((0x503000,0x504000),(0x300800,0x301800)):
  cpu.reg_write(x.UC_X86_REG_ECX,owner);cpu.reg_write(x.UC_X86_REG_ESP,entry);cpu.emu_start(0xec8300,0xec8328,count=100);assert word(entry-384+0x24)==0 and cpu.reg_read(x.UC_X86_REG_ESI)==owner and cpu.reg_read(x.UC_X86_REG_ESP)==entry-388
 for letter,woman,reward,gsi,owner,sp,flags in itertools.product((False,True),(False,True),(False,True),(0x500000,0x501000,0x502000),(0x503000,0x504000),(0x300800,0x301800),(0,1,0x10,0xfffffff0)):
  state=dict(letter=letter,woman=woman,reward=reward,gsi=gsi,owner=owner,stack=sp,flags=flags,model=0x505000,hero=0x506000,queries=0,cleanups=0)
  put(0x143e90c,state['model']);put(owner+4,gsi);cpu.mem_write(owner+0x1d,bytes([int(reward)]));put(gsi,gsi+0x800);put(gsi+0x800+0x118,0x200100);put(gsi+0x800+0x2e0,0x200200);put(sp+0x24,flags);cpu.mem_write(sp+0x23,b'\xa5')
  cpu.reg_write(x.UC_X86_REG_ESI,owner);cpu.reg_write(x.UC_X86_REG_ESP,sp);cpu.emu_start(0xec8a12,0xec9228,count=200)
  expected=(letter or woman) and not reward;assert state['question']==expected and cpu.mem_read(sp+0x23,1)==bytes([int(expected)]) and cpu.reg_read(x.UC_X86_REG_ESP)==sp and word(sp+0x24)==((flags|4)&~12)&0xffffffff and state['queries']==(1 if letter else 2) and state['cleanups']==(1 if letter else 2);rows.append(state.copy())
 assert len(rows)==384

def test_original_idle_hit_branch():
 import unicorn as u
 from unicorn import x86_const as x
 from tools.script_recovery.lift_native_lua import RData
 rd=RData();cpu=u.Uc(u.UC_ARCH_X86,u.UC_MODE_32)
 for a,n in ((0xec9000,0x1000),(0x99e000,0x1000),(0x300000,0x10000),(0x500000,0xc000),(0x200000,0x1000)):cpu.mem_map(a,n)
 cpu.mem_write(0xec96df,rd.bytes_at(0xec96df,0xe8))
 for address in (0x99ebf0,0x99eae0,0x200100,0x200200,0x200300):cpu.mem_write(address,b'\xc3')
 def put(a,v):cpu.mem_write(a,struct.pack('<I',v&0xffffffff))
 def word(a):return struct.unpack('<I',cpu.mem_read(a,4))[0]
 state={};rows=[]
 def returned(machine,sp,purge,value=0):
  machine.reg_write(x.UC_X86_REG_EAX,value);machine.reg_write(x.UC_X86_REG_EIP,word(sp));machine.reg_write(x.UC_X86_REG_ESP,sp+4+purge)
 def hook(machine,a,size,user):
  sp=machine.reg_read(x.UC_X86_REG_ESP)
  if a in (0xec97c7,0xec9969):state['react']=a==0xec97c7;machine.emu_stop()
  elif a==0x99ebf0:
   obj=machine.reg_read(x.UC_X86_REG_ECX);assert obj in [state['stack']+off for off in (0x44,0x80,0x78)] and [word(sp+4),word(sp+8)]==[0x125d1c8,0xffffffff];put(obj,0);returned(machine,sp,8)
  elif a in (0x200100,0x200200,0x200300):
   assert machine.reg_read(x.UC_X86_REG_ECX)==state['owner']+8
   off,purge,value={0x200100:(0x44,4,state['hero']),0x200200:(0x80,4,state['special']),0x200300:(0x78,8,state['excluded'])}[a]
   assert word(sp+4+(4 if a==0x200300 else 0))==state['stack']+off
   if a==0x200300:assert word(sp+4)==14
   state['queries']+=1;returned(machine,sp,purge,int(value))
  elif a==0x99eae0:
   assert machine.reg_read(x.UC_X86_REG_ECX) in [state['stack']+off for off in (0x44,0x80,0x78)];state['cleanups']+=1;returned(machine,sp,0)
 cpu.hook_add(u.UC_HOOK_CODE,hook)
 for hero,special,excluded,table,owner,sp,flags in itertools.product((False,True),(False,True),(False,True),(0x500000,0x501000,0x502000),(0x503000,0x504000),(0x300800,0x301800),(0,1,4,0xffffff80)):
  state=dict(hero=hero,special=special,excluded=excluded,table=table,owner=owner,stack=sp,flags=flags,queries=0,cleanups=0)
  put(owner+8,table);put(table+0x54,0x200100);put(table+0xa8,0x200200);put(table+0xa4,0x200300);put(sp+0x24,flags);cpu.mem_write(sp+0x23,b'\xa5')
  cpu.reg_write(x.UC_X86_REG_ESI,owner);cpu.reg_write(x.UC_X86_REG_ESP,sp);cpu.emu_start(0xec96df,0xec996a,count=250)
  expected=hero or (special and not excluded);count=1 if hero else (3 if special else 2)
  assert state['react']==expected and cpu.mem_read(sp+0x23,1)==bytes([int(expected)]) and cpu.reg_read(x.UC_X86_REG_ESP)==sp and word(sp+0x24)==flags and state['queries']==state['cleanups']==count;rows.append(state.copy())
 assert len(rows)==384
