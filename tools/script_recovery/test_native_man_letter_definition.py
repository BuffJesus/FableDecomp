import copy,itertools,json,struct,sys
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
H=r'''return function(key,initialOwned,acquireWait,termination)
 local q,me,hero,r={},{},{},{}
 local boundary={};local paused=false;local frame,checks,created,queries,reads=0,0,0,0,0
 local resource=nil;local attempts=0;local released=0;local reached=false;local queriedKey='unset'
 function q:RetailResources()return r end
 function q:PersistTransferBool(ctx,name,value)assert(name=='GivenLetter' or name=='GotReward');return name=='GivenLetter' end
 function q:SetThingPersistent(actor,value)assert(actor==me and value)end
 function q:NewScriptFrame(actor)assert(actor==me);frame=frame+1;assert(frame<30);return not (termination>0 and frame>=termination)end
 function q:IsActiveThreadTerminating()checks=checks+1;return termination>0 and checks>=termination end
 function q:GetHero()return hero end
 function q:ReadGlobalGameDataString(offset)assert(offset==0x740);reads=reads+1;return created==1 and key or 'EARLIER_LETTER' end
 function q:IsObjectInThingsPossession(value,actor)
  assert(actor==hero);queries=queries+1
  if created==1 then reached=true;queriedKey=value;assert(value==key,'wrong current letter definition');error(boundary)end
  assert(value=='EARLIER_LETTER' or value=='OBJECT_SICK_CHILD_WOMANS_HERO_LETTER');return initialOwned
 end
 function q:GetStateInt(name)assert(name=='MansLoverState');return 3 end
 function q:SetThingHasInformation(actor,a,b,c)assert(actor==me and a and b and not c)end
 function q:SetIsPushableByHero(actor,value)assert(actor==me and not value)end
 function q:SetCreatureBrain(actor,name)assert(actor==me and name=='BRAIN_PASSIVE_OVERRIDE')end
 function q:SetWanderCentrePoint(actor,pos)assert(actor==me and pos.x==1)end
 function q:SetWanderMinDistance(actor,value)assert(actor==me and value==0)end
 function q:SetWanderMaxDistance(actor,value)assert(actor==me and value==10)end
 function q:SetScriptingStateGroup(actor,value)assert(actor==me and value==4)end
 function q:PauseAllNonScriptedEntities(value)assert(value and not paused);paused=value end
 function me:GetPos()return {x=1,y=2,z=3}end
 function me:IsTalkedToByHero()return true end
 function r:NewResource()assert(not resource);resource={};return resource end
 function r:PrepareResource(value)assert(value==resource)end
 function r:TryAcquire(value,actor,priority)assert(value==resource and actor==me and priority==4);attempts=attempts+1;return attempts>acquireWait end
 function r:ReleaseResource(value)assert(value==resource);released=released+1 end
 function r:StartMovie(name)assert(name=='' and created==0);created=1;return 31 end
 Init(q,me);OnPersist(q,me,{})
 local ok,err=pcall(Main,q,me);assert(ok or err==boundary,tostring(err))
 if reached then assert(created==1 and paused and released==0)else assert(created==0 and not paused)end
 -- Boundary is the query itself, before the existing flag/movie/reward defects.
 return reached,queriedKey,frame,checks,queries,attempts,released,reads
end'''
CASES=list(itertools.product(('', 'CURRENT_LETTER','MixedCase_12','LONG_'+('x'*64)),(False,True),(0,2),(0,1,2,3,5,7,12,20)))
@pytest.mark.parametrize('style',('draft','readable'))
@pytest.mark.parametrize('case',CASES)
def test_first_letter_query_reads_current_definition(evidence,style,case):
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

def test_original_letter_definition_output_matches_query():
 import unicorn as u
 from unicorn import x86_const as x
 from tools.script_recovery.lift_native_lua import RData
 from tools.script_recovery.convert_quest_unit import callee_stack_words
 rd=RData();assert callee_stack_words(0x415d70)==1
 query=int.from_bytes(rd.bytes_at(0x1260f0c+0x2e0,4),'little');assert callee_stack_words(query)==2
 cpu=u.Uc(u.UC_ARCH_X86,u.UC_MODE_32)
 for a,n in ((0xec8000,0x3000),(0x415000,0x1000),(0x143e000,0x1000),(0x300000,0x10000),(0x500000,0xc000),(0x200000,0x1000)):cpu.mem_map(a,n)
 cpu.mem_write(0xec8a12,rd.bytes_at(0xec8a12,0x3b));cpu.mem_write(0x415d70,b'\xc3');cpu.mem_write(0x200100,b'\xc3');cpu.mem_write(0x200200,b'\xc3')
 def put(a,v):cpu.mem_write(a,struct.pack('<I',v&0xffffffff))
 def word(a):return struct.unpack('<I',cpu.mem_read(a,4))[0]
 state={};rows=[]
 def returned(machine,sp,purge,value):
  machine.reg_write(x.UC_X86_REG_EAX,value);machine.reg_write(x.UC_X86_REG_EIP,word(sp));machine.reg_write(x.UC_X86_REG_ESP,sp+4+purge)
 def hook(machine,a,size,user):
  sp=machine.reg_read(x.UC_X86_REG_ESP)
  if a==0x415d70:
   dst=word(sp+4);assert machine.reg_read(x.UC_X86_REG_ECX)==state['model']+0x740 and dst==state['stack']+0x48
   assert word(state['model']+0x740)==state['token'];put(dst,state['representation']);state['filled']=dst;returned(machine,sp,4,dst)
  elif a==0x200100:
   assert machine.reg_read(x.UC_X86_REG_ECX)==state['gsi'];returned(machine,sp,0,state['hero'])
  elif a==0x200200:
   assert machine.reg_read(x.UC_X86_REG_ECX)==state['gsi'] and [word(sp+4),word(sp+8)]==[state['filled'],state['hero']] and word(word(sp+4))==state['representation']
   state['queried']=True;returned(machine,sp,8,int(state['owned']))
 cpu.hook_add(u.UC_HOOK_CODE,hook)
 for gsi,owner,sp,token,flags,owned in itertools.product((0x500000,0x501000,0x502000),(0x503000,0x504000),(0x300800,0x301800),(0,1,713,0xffffffff),(0,1,8,0xffffffff),(False,True)):
  model=0x505000;hero=0x506000;representation=0 if token==0 else 0x507000+(token&0xff)*4
  state=dict(gsi=gsi,owner=owner,stack=sp,model=model,hero=hero,token=token,representation=representation,flags=flags,owned=owned)
  put(0x143e90c,model);put(model+0x740,token);put(owner+4,gsi);put(gsi,gsi+0x800);put(gsi+0x800+0x118,0x200100);put(gsi+0x800+0x2e0,0x200200);put(sp+0x24,flags)
  cpu.reg_write(x.UC_X86_REG_ESI,owner);cpu.reg_write(x.UC_X86_REG_ESP,sp);cpu.emu_start(0xec8a12,0xec8a4d,count=100)
  assert cpu.reg_read(x.UC_X86_REG_ESP)==sp and word(sp+0x24)==(flags|4)&0xffffffff and state['queried'] and bool(cpu.reg_read(x.UC_X86_REG_EAX))==owned;rows.append(state.copy())
 assert len(rows)==384
