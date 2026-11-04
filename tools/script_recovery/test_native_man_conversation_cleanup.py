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
H=r'''return function(mask,cancelStage,taskWait,acquireWait,termination,handle)
 local q,me,hero,r={},{},{},{};local boundary={};local paused=false;local resource=nil
 local frame,checks,created,destroyed,attempts,released,healths,speeches,tasks,gifts,cleared=0,0,0,0,0,0,0,0,0,0,0
 local phase='boot';local saved={};local trace={};local function log(value)trace[#trace+1]=value end
 function q:RetailResources()return r end
 function q:PersistTransferBool(ctx,name,value)if phase=='boot' then return false end;saved[name]=value;return value end
 function q:SetThingPersistent(actor,value)assert(actor==me and value)end
 function q:NewScriptFrame(actor)assert(actor==me);frame=frame+1;assert(frame<40);return not (termination>0 and frame>=termination)end
 function q:IsActiveThreadTerminating()checks=checks+1;return termination>0 and checks>=termination end
 function q:GetHero()return hero end
 function q:ReadGlobalGameDataString(offset)assert(offset==0x740 or offset==0x73c);return offset==0x740 and 'EARLIER_LETTER' or 'NEW_LETTER' end
 function q:IsObjectInThingsPossession(value,actor)assert(actor==hero and value=='EARLIER_LETTER');return true end
 function q:GetStateInt(name)assert(name=='MansLoverState');return 1 end
 function q:SetThingHasInformation(actor,a,b,c)assert(actor==me and a and b and not c)end
 function q:SetIsPushableByHero(actor,value)assert(actor==me and not value)end
 function q:SetCreatureBrain(actor,name)assert(actor==me and name=='BRAIN_PASSIVE_OVERRIDE')end
 function q:SetWanderCentrePoint(actor,pos)assert(actor==me and pos.x==1)end
 function q:SetWanderMinDistance(actor,value)assert(actor==me and value==0)end
 function q:SetWanderMaxDistance(actor,value)assert(actor==me and value==10)end
 function q:SetScriptingStateGroup(actor,value)assert(actor==me and value==4)end
 function q:PauseAllNonScriptedEntities(value)assert(value~=paused);paused=value;log('pause:'..tostring(value))end
 function q:GetHealth(actor)assert(actor==me);healths=healths+1;assert(healths<=4);return ((mask & (1 << (healths-1)))~=0) and 1 or 0 end
 function q:GiveHeroObject(value,amount,flag)assert(value=='NEW_LETTER' and amount==-1 and flag==false and gifts==0);gifts=gifts+1;log('give:NEW_LETTER')end
 function q:ClearThingHasInformation(actor)assert(actor==me and gifts==1 and healths==4);cleared=cleared+1;log('clear')end
 function me:GetPos()return {x=1,y=2,z=3}end
 function me:IsTalkedToByHero()return true end
 function me:Speak(actor,text,group,a,b,c)
  assert(actor==hero and group==0 and not a and b and not c)
  local names={'TEXT_QST_B10_MAN_IN_LOVE_INTRO','TEXT_QST_B10_MAN_IN_LOVE_INTRO_50','TEXT_QST_B10_MAN_IN_LOVE_INTRO_60','TEXT_QST_B10_MAN_IN_LOVE_INTRO_70'}
  assert(text==names[healths]);speeches=speeches+1;tasks=0;log(text);return healths~=cancelStage
 end
 function me:IsPerformingScriptTask()tasks=tasks+1;return tasks<=taskWait end
 function r:NewResource()assert(not resource);resource={};return resource end
 function r:PrepareResource(value)assert(value==resource)end
 function r:TryAcquire(value,actor,priority)assert(value==resource and actor==me and priority==4);attempts=attempts+1;return attempts>acquireWait end
 function r:ReleaseResource(value)assert(value==resource and not paused);released=released+1;log('release')end
 function r:StartMovie(name)assert(name=='');created=created+1;log('start:'..tostring(handle));return handle end
 function r:ResourceThingHealthAbove(value,minimum)assert(minimum==0.0);return q:GetHealth(self:ScriptThing(value))>minimum end
 function r:ScriptThing(value)assert(value==resource);return me end
 function r:DestroyMovie(value)assert(value==handle and not paused and destroyed==0);destroyed=destroyed+1;log('destroy:'..tostring(value));if cleared==1 then error(boundary)end end
 Init(q,me);OnPersist(q,me,{})
 local ok,err=pcall(Main,q,me);assert(ok or err==boundary,tostring(err));phase='inspect';OnPersist(q,me,{})
 assert(not paused and created==destroyed)
 if err==boundary then assert(saved.GivenLetter and gifts==1 and cleared==1 and released==0)
 else assert(not saved.GivenLetter and cleared==0 and released==(resource and 1 or 0))end
 if termination==0 then assert(created==1)end
 return table.concat(trace,'|'),frame,checks,attempts,created,destroyed,released,healths,speeches,gifts,cleared,saved.GivenLetter
end'''
CASES=list(itertools.product((0,5,10,15),(0,1,2,3,4),(0,2),(0,2),(0,1,2,3,5,7,12,20),(0,31,713)))
T=r'''return function(key,health,speak,taskWait,acquireWait,termination)
 local q,me,hero,r={},{},{},{};local boundary={};local paused=false
 local frame,checks,created,reads,given,attempts,released,destroyed,tasks=0,0,0,0,0,0,0,0,0
 local resource=nil;local removed=0;local phase="boot";local saved={}
 function q:RetailResources()return r end
 function q:PersistTransferBool(ctx,name,value)if phase=='boot' then return name=='GivenLetter' end;saved[name]=value;return value end
 function q:SetThingPersistent(actor,value)assert(actor==me and value)end
 function q:NewScriptFrame(actor)assert(actor==me);frame=frame+1;assert(frame<30);return not (termination>0 and frame>=termination)end
 function q:IsActiveThreadTerminating()checks=checks+1;return termination>0 and checks>=termination end
 function q:GetHero()return hero end
 function q:ReadGlobalGameDataString(offset)assert(offset==0x740 or offset==0x738);reads=reads+1;return offset==0x740 and 'LETTER' or key end
 function q:IsObjectInThingsPossession(value,actor)assert(actor==hero and (value=='LETTER' or value=='OBJECT_SICK_CHILD_WOMANS_HERO_LETTER'));return false end
 function q:GetStateInt(name)assert(name=='MansLoverState');return 3 end
 function q:SetThingHasInformation(actor,a,b,c)assert(actor==me and a and b and not c)end
 function q:SetIsPushableByHero(actor,value)assert(actor==me and not value)end
 function q:SetCreatureBrain(actor,name)assert(actor==me and name=='BRAIN_PASSIVE_OVERRIDE')end
 function q:SetWanderCentrePoint(actor,pos)assert(actor==me and pos.x==1)end
 function q:SetWanderMinDistance(actor,value)assert(actor==me and value==0)end
 function q:SetWanderMaxDistance(actor,value)assert(actor==me and value==10)end
 function q:SetScriptingStateGroup(actor,value)assert(actor==me and value==4)end
 function q:PauseAllNonScriptedEntities(value)assert(value~=paused);paused=value end
 function q:GetHealth(actor)assert(actor==me);return health and 1 or 0 end
 function q:GiveHeroObject(value,amount,flag)assert(value==key and amount==-1 and flag==false,'wrong reward operands');given=given+1;end
 function q:RemoveItemFromContainer(actor,value)assert(actor==me and value==key and given==1);removed=removed+1 end
 function me:GetPos()return {x=1,y=2,z=3}end
 function me:IsTalkedToByHero()return true end
 function me:Speak(actor,text,group,a,b,c)assert(actor==hero and text=='TEXT_QST_B10_MAN_IN_LOVE_MYRA_DIED' and group==0 and not a and b and not c);return speak end
 function me:IsPerformingScriptTask()tasks=tasks+1;return tasks<=taskWait end
 function r:NewResource()assert(not resource);resource={};return resource end
 function r:PrepareResource(value)assert(value==resource)end
 function r:TryAcquire(value,actor,priority)assert(value==resource and actor==me and priority==4);attempts=attempts+1;return attempts>acquireWait end
 function r:ReleaseResource(value)assert(value==resource);released=released+1 end
 function r:StartMovie(name)assert(name=='');created=created+1;return 31 end
 function r:ResourceThingHealthAbove(value,minimum)assert(minimum==0.0);return q:GetHealth(self:ScriptThing(value))>minimum end
 function r:ScriptThing(value)assert(value==resource);return me end
 function r:DestroyMovie(handle)assert(handle==31 and not paused);destroyed=destroyed+1;if removed==1 then error(boundary)end end
 Init(q,me);OnPersist(q,me,{})
 local ok,err=pcall(Main,q,me);assert(ok or err==boundary,tostring(err));phase='inspect';OnPersist(q,me,{})
 assert(not paused and created==destroyed and not saved.HelpedGuyOut)
 if err==boundary then assert(given==1 and removed==1 and created==1 and destroyed==1 and released==0 and saved.GotReward)else assert(not saved.GotReward and released==(resource and 1 or 0))end
 return given,removed,frame,checks,attempts,released,destroyed,tasks,reads,saved.GotReward
end'''
TALK_CASES=list(itertools.product(('', 'REWARD','MixedCase_12','LONG_'+('x'*64)),(False,True),(False,True),(0,2),(0,2),(0,1,2,3,5,7,12,20)))
@pytest.mark.parametrize('style',('draft','readable'))
@pytest.mark.parametrize('case',CASES)
def test_intro_scene_shared_cleanup(evidence,style,case):
 lua=LuaRuntime();lua.execute(evidence[2][style]);lua.execute(H)(*case)

@pytest.mark.parametrize('style',('draft','readable'))
@pytest.mark.parametrize('case',TALK_CASES)
def test_talk_reward_scene_shared_cleanup(evidence,style,case):
 lua=LuaRuntime();lua.execute(evidence[2][style]);lua.execute(T)(*case)

@pytest.mark.parametrize('fault',('native','source','anchor','count','missing_alias','collision','duplicate_edit','wrong_new'))
def test_stale_or_ambiguous_conversation_alias_fails(evidence,fault):
 from tools.script_recovery.native_reviewed_stack_aliases import restore
 c,row=evidence[:2];row=copy.deepcopy(row);fn=copy.deepcopy(c['fn']);text=c['text']
 if fault=='native':row['nativeSha256']='0'*64
 elif fault=='source':fn['decompile']+='\n'
 elif fault=='anchor':row['nativeInstructions']['0x00EC89A7']='90'
 elif fault=='count':row['editCounts'][-1]=3
 elif fault=='missing_alias':row['additionalAliases'].pop()
 elif fault=='collision':text+='\nmanConversationMovie;'
 elif fault=='duplicate_edit':row['edits'].append(row['edits'][-1])
 elif fault=='wrong_new':row['edits'][-1][1]='unreviewedMovie'
 with pytest.raises(ValueError):restore(text,fn,row,c['rd'])

@pytest.mark.parametrize('index',(0,1))
@pytest.mark.parametrize('fault',('variable','count'))
def test_stale_movie_phi_review_fails(evidence,index,fault):
 from tools.script_recovery.native_reviewed_movie_members import restore
 c,row=evidence[3:5];row=copy.deepcopy(row);alias=row['stackReceiverAliases'][index]
 if fault=='variable':alias['phiVariable']='oldMovie'
 else:alias['phiCount']=2
 with pytest.raises(ValueError):restore(c['text'],c['fn'],row,c['rd'])

def test_unreviewed_scalar_source_unchanged(evidence):
 from tools.script_recovery.native_reviewed_stack_aliases import restore
 c=evidence[0];assert restore(c['text'],c['fn'],None,c['rd'])==c['text']

def test_unreviewed_movie_source_unchanged(evidence):
 from tools.script_recovery.native_reviewed_movie_members import restore
 c=evidence[3];assert restore(c['text'],c['fn'],None,c['rd'])==c['text']

def test_original_shared_movie_selection_and_cleanup():
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
 groups=[(0xec8671,0xec86a5,0xac,0x3c,[(0xec89a7,0xec96c9)]),(0xec89c2,0xec89f6,0x28,0xa8,[(0xec96c0,0xec96c9)])]
 for gsi,owner,sp,cookie in itertools.product((0x500000,0x501000,0x502000),(0x503000,0x504000),(0x300800,0x301800),(0,1,713,0x12345678)):
  table=gsi+0x800;put(gsi,table);put(table+0x5c8,0x200100);put(owner+4,gsi)
  for begin,end,movie,string,cleanups in groups:
   for a,b in cleanups:
    state=dict(gsi=gsi,owner=owner,sp=sp,cookie=cookie,movie=movie,string=string,starts=0,ends=0);cpu.mem_write(sp,b'\xa5'*0x180);cpu.reg_write(x.UC_X86_REG_ESI,owner);cpu.reg_write(x.UC_X86_REG_ESP,sp)
    cpu.emu_start(begin,end,count=300);assert cpu.reg_read(x.UC_X86_REG_ESP)==sp and word(sp+movie+8)==cookie
    cpu.emu_start(a,b,count=300);assert cpu.reg_read(x.UC_X86_REG_ESP)==sp and [word(sp+movie+i) for i in (4,8,12)]==[0,0,0] and state['starts']==state['ends']==1
    rows.append(dict(state,start=hex(begin),cleanup=hex(a),movie_entry_slot=movie-384,string_entry_slot=string-384))
 assert len(rows)==96
