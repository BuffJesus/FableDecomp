import copy,itertools,json,re,struct,sys
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
 c=UnitConverter(ROOT/'refs/script_recovery/sick_child/translation_unit_typed.json');unit=json.loads((ROOT/'refs/script_recovery/sick_child/units/V_SickChild.json').read_text());row=unit['reviewedStackScalarAliases']['0x00ec8300'];original=adapter.restore;captured={}
 unit['entities']={'ManInLove':unit['entities']['ManInLove']};out=tmp_path_factory.mktemp('man_letter')
 def capture(text,fn,review,rd):
  if fn['address'].lower()=='0x00ec8300':captured.update(text=text,fn=fn,rd=rd)
  return original(text,fn,review,rd)
 with patch.object(c,'constructor_defaults',return_value={}),patch.object(adapter,'restore',capture):c.convert(unit,out)
 source=(out/'FSE/V_SickChild/Entities/ManInLove.lua').read_text();return captured,row,dict(draft=source,readable=readable_file(source)[0])
H=r'''return function(key,health,speak,taskWait,acquireWait,termination)
 local q,me,hero,r={},{},{},{};local boundary={};local paused=false
 local frame,checks,created,reads,given,attempts,released,destroyed,tasks=0,0,0,0,0,0,0,0,0
 local resource=nil
 function q:RetailResources()return r end
 function q:PersistTransferBool(ctx,name,value)return name=='GivenLetter' end
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
 function q:GiveHeroObject(value,amount,flag)assert(value==key and amount==-1 and flag==false,'wrong reward operands');given=given+1;error(boundary)end
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
 function r:DestroyMovie(handle)assert(type(handle)=='number' and not paused);destroyed=destroyed+1 end
 Init(q,me);OnPersist(q,me,{})
 local ok,err=pcall(Main,q,me);assert(ok or err==boundary,tostring(err))
 if given==1 then assert(paused and created==1 and destroyed==0 and released==0)else assert(not paused and created==destroyed and released==(resource and 1 or 0))end
 return given,frame,checks,attempts,released,destroyed,tasks,reads
end'''
CASES=list(itertools.product(('', 'REWARD','MixedCase_12','LONG_'+('x'*64)),(False,True),(False,True),(0,2),(0,2),(0,1,2,3,5,7,12,20)))
def blocks(source):
 # Execute exact emitted operations only; two Main paths remain blocked by
 # a pre-existing literal flag condition, without modifying it to reach them.
 rows=[]
 for method,index,total in (('GiveHeroObject',1,6),('RemoveItemFromContainer',1,4),('GiveHeroObject',3,6)):
  lines=source.splitlines();sites=[k for k,s in enumerate(lines) if re.match(r'\s*quest:'+method+r'\(',s)];assert len(sites)==total,(method,len(sites))
  end=sites[index];start=end
  if re.match(r'\s*(?:local\s+)?\w+\s*=\s*quest:ReadGlobalGameDataString',lines[end-1]) or 'TODO(native): CDefString' in lines[end-1]:start=end-1
  block='\n'.join(lines[start:end+1])
  if 'SCRIPT_DEF.' in block:
   tables=re.findall(r'(?ms)^local SCRIPT_DEF = \{\n.*?^\}\n',source)
   assert len(tables)==1
   block=tables[0]+block
  rows.append(block)
 return rows
B=r'''return function(block,key)
 local q,me={},{};local reads,calls=0,0
 function q:ReadGlobalGameDataString(offset)assert(offset==0x738);reads=reads+1;return key end
 function q:GiveHeroObject(value,amount,flag)assert(value==key and amount==-1 and flag==false,'wrong give item operands');calls=calls+1 end
 function q:RemoveItemFromContainer(actor,value)assert(actor==me and value==key,'wrong remove item operands');calls=calls+1 end
 assert(load('return function(quest,me) '..block..' end'))()(q,me);assert(reads==1 and calls==1);return reads,calls
end'''
@pytest.mark.parametrize('style',('draft','readable'))
@pytest.mark.parametrize('case',CASES)
def test_first_reward_uses_current_item_operands(evidence,style,case):
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

@pytest.mark.parametrize('style',('draft','readable'))
@pytest.mark.parametrize('key',('', 'REWARD','MixedCase_12','LONG_'+('x'*64)))
@pytest.mark.parametrize('index',(0,1,2))
def test_emitted_reward_blocks_have_current_definitions(evidence,style,key,index):
 LuaRuntime().execute(B)(blocks(evidence[2][style])[index],key)

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

def test_original_three_reward_operand_windows():
 import unicorn as u
 from unicorn import x86_const as x
 from tools.script_recovery.lift_native_lua import RData
 from tools.script_recovery.convert_quest_unit import callee_stack_words
 rd=RData();assert callee_stack_words(0x415d70)==1 and callee_stack_words(0x99eae0)==0
 for slot,n in ((0x1e4,3),(0x928,2)):assert callee_stack_words(int.from_bytes(rd.bytes_at(0x1260f0c+slot,4),'little'))==n
 cpu=u.Uc(u.UC_ARCH_X86,u.UC_MODE_32)
 for a,n in ((0xec8000,0x3000),(0x415000,0x1000),(0x99e000,0x1000),(0x143e000,0x1000),(0x300000,0x10000),(0x500000,0xc000),(0x200000,0x1000)):cpu.mem_map(a,n)
 windows=((0xec9313,0xec9346,0x40,'give'),(0xec8d16,0xec8d52,0xa4,'remove'),(0xec9004,0xec903a,0x7c,'give'))
 for start,end,offset,kind in windows:cpu.mem_write(start,rd.bytes_at(start,end-start))
 for address in (0x415d70,0x99eae0,0x200100,0x200200):cpu.mem_write(address,b'\xc3')
 def put(a,v):cpu.mem_write(a,struct.pack('<I',v&0xffffffff))
 def word(a):return struct.unpack('<I',cpu.mem_read(a,4))[0]
 state={};rows=[]
 def returned(machine,sp,purge,value=0):
  machine.reg_write(x.UC_X86_REG_EAX,value);machine.reg_write(x.UC_X86_REG_EIP,word(sp));machine.reg_write(x.UC_X86_REG_ESP,sp+4+purge)
 def hook(machine,a,size,user):
  sp=machine.reg_read(x.UC_X86_REG_ESP)
  if a==0x415d70:
   dst=word(sp+4);assert machine.reg_read(x.UC_X86_REG_ECX)==state['model']+0x738 and dst==state['stack']+state['offset'];put(dst,state['token']);state['filled']=dst;returned(machine,sp,4,dst)
  elif a in (0x200100,0x200200):
   assert machine.reg_read(x.UC_X86_REG_ECX)==state['gsi']
   if a==0x200100:assert state['kind']=='give' and [word(sp+4),word(sp+8),word(sp+12)]==[state['filled'],0xffffffff,0];purge=12
   else:assert state['kind']=='remove' and [word(sp+4),word(sp+8)]==[state['owner']+8,state['filled']];purge=8
   assert word(state['filled'])==state['token'];state['called']=True;returned(machine,sp,purge)
  elif a==0x99eae0:
   assert machine.reg_read(x.UC_X86_REG_ECX)==state['filled'] and state['called'];state['destroyed']=True;returned(machine,sp,0)
 cpu.hook_add(u.UC_HOOK_CODE,hook)
 for (start,end,offset,kind),gsi,owner,sp,token in itertools.product(windows,(0x500000,0x501000,0x502000),(0x503000,0x504000),(0x300800,0x301800),(0,1,713,0xffffffff)):
  state=dict(start=start,gsi=gsi,owner=owner,stack=sp,model=0x505000,offset=offset,kind=kind,token=token)
  put(0x143e90c,state['model']);put(owner+4,gsi);put(gsi,gsi+0x800);put(gsi+0x800+0x1e4,0x200100);put(gsi+0x800+0x928,0x200200)
  cpu.reg_write(x.UC_X86_REG_ESI,owner);cpu.reg_write(x.UC_X86_REG_ESP,sp);cpu.emu_start(start,end,count=100)
  assert cpu.reg_read(x.UC_X86_REG_ESP)==sp and state['called'] and state['destroyed'];rows.append(state.copy())
 assert len(rows)==144
