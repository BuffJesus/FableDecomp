"""Actual late shared cleanup branches keep movie/resource/pause ownership."""
import itertools
import os
from pathlib import Path
import pytest
from lupa.lua54 import LuaRuntime
ROOT=next(p for p in Path(__file__).resolve().parents if (p/'tools/script_recovery/script_units.py').is_file())
import json
from unittest.mock import patch
from tools.script_recovery.convert_quest_unit import UnitConverter
from tools.script_recovery.build_readable_unit import readable_file

@pytest.fixture(scope='module')
def generated(tmp_path_factory):
 c=UnitConverter(ROOT/'refs/script_recovery/sick_child/translation_unit_typed.json')
 unit=json.loads((ROOT/'refs/script_recovery/sick_child/units/V_SickChild.json').read_text())
 unit['entities']={'SickChildsSister':unit['entities']['SickChildsSister']};unit['quest']['functions']={};unit['quest']['helpers']={}
 out=tmp_path_factory.mktemp('sister_cleanup')
 with patch.object(c,'constructor_defaults',return_value={}):c.convert(unit,out)
 source=(out/'FSE/V_SickChild/Entities/SickChildsSister.lua').read_text()
 return {'draft':source,'readable':readable_file(source)[0]}


H=r'''
return function(mode,limit,speechTicks,health,acquireDelay)
 local frame,taskUntil,speeches=0,0,0;local resource,movies,paused=nil,{},false
 local q,me,hero,r={},{},{},{};local context={}
 local function term()return frame>=limit end
 function q:PersistTransferBool(c,name,value)assert(c==context and name=='LeadHeroToSickChild');return true end
 function q:GetHero()return hero end
 function q:RetailResources()return r end
 function q:IsActiveThreadTerminating()return term()end
 function q:NewScriptFrame(actor)assert(actor==me and not term(),'frame advanced after termination');frame=frame+1;return not term()end
 function q:GetStateBool(name)
  assert(name=='MotherIntroDone' or name=='FinishedQuest')
  return name=='MotherIntroDone' and mode~='reminder' or name=='FinishedQuest' and mode=='thanks'
 end
 function q:ClearThingHasInformation(actor)assert(actor==me)end
 function q:PauseAllNonScriptedEntities(value)paused=value end
 function q:GetHealth(actor)assert(actor==me);return health end
 function me:IsTalkedToByHero()return mode~='hit' and mode~='ability' end
 function me:MsgIsHitByHero()return mode=='hit'end
 function me:MsgIsHitByAnySpecialAbilityFromHero()return mode=='ability'end
 function me:MsgIsHitByHeroSpecialAbility(kind)assert(kind==14);return false end
 function me:IsPerformingScriptTask()return frame<taskUntil end
 function me:Speak(target,key,selection,a,b,c)
  local expected=(mode=='thanks' and 'TEXT_QST_B10_SISTER_THANKS') or (mode=='reminder' and 'TEXT_QST_B10_SISTER_REMINDER') or 'TEXT_QST_B10_SISTER_ON_HIT'
  assert(target==hero and key==expected and selection==0 and not a and b and not c)
  speeches=speeches+1;taskUntil=frame+speechTicks
  while frame<taskUntil and not term()do q:NewScriptFrame(me)end
  return not term()
 end
 function r:NewResource()assert(not resource);resource=811;return resource end
 function r:PrepareResource(id)assert(id==resource)end
 function r:TryAcquire(id,actor,kind)assert(id==resource and actor==me and kind==4);return frame>=acquireDelay end
 function r:ScriptThing(id)assert(id==resource);return me end
 function r:ReleaseResource(id)assert(id==resource);resource=nil end
 local nextMovie=0
 function r:StartMovie(name)assert(name=='');nextMovie=nextMovie+1;movies[nextMovie]=true;return nextMovie end
 function r:DestroyMovie(id)assert(movies[id],'wrong or repeated movie cleanup');movies[id]=nil end
 debug.sethook(function()error('instruction budget')end,'',500000)
 OnPersist(q,me,context);Main(q,me);debug.sethook()
 assert(not resource and next(movies)==nil and not paused,'resource/movie/pause leak')
 assert(health<=0 or limit<=math.max(1,acquireDelay) or speeches>0,'expected speech branch was not exercised')
 return speeches
end
'''
@pytest.mark.parametrize('style',('draft','readable'))
@pytest.mark.parametrize('mode,limit,speech,health,acquire',list(itertools.product(('thanks','reminder','hit','ability'),(1,2,3,6,10,17),(0,1,3),(0,50),(0,2))))
def test_actual_shared_cleanup(generated,style,mode,limit,speech,health,acquire):
 source=generated[style]
 lua=LuaRuntime();lua.execute(source);lua.execute(H)(mode,limit,speech,health,acquire)

@pytest.fixture(scope='module')
def review_input(tmp_path_factory):
 from tools.script_recovery import native_reviewed_cleanup_returns as adapter
 c=UnitConverter(ROOT/'refs/script_recovery/sick_child/translation_unit_typed.json')
 unit=json.loads((ROOT/'refs/script_recovery/sick_child/units/V_SickChild.json').read_text())
 row=unit['reviewedCleanupReturns']['0x00ec68a0']
 unit['entities']={'SickChildsSister':unit['entities']['SickChildsSister']};unit['quest']['functions']={};unit['quest']['helpers']={}
 original=adapter.restore;captured={}
 def capture(source,fn,row,data):
  if str(fn['address']).lower()=='0x00ec68a0':captured.update(source=source,fn=fn,data=data)
  return original(source,fn,row,data)
 with patch.object(c,'constructor_defaults',return_value={}),patch.object(adapter,'restore',capture):
  c.convert(unit,tmp_path_factory.mktemp('sister_cleanup_review'))
 return captured,row

@pytest.mark.parametrize('fault',('no_review','nativeSha256','sourceSha256','loweredSha256',
 'movieSite','releaseSite','tailEnd','frameBytes','resourceSlot','movieAlias','resourceAlias',
 'instruction','missingInstruction','helper','missingHelper','truncatedHelper','phi_inventory',
 'metadata_depth','metadata_target','metadata_receiver','source_label','source_duplicate',
 'source_alias','resource_constructor'))
def test_native_cleanup_return_guards(review_input,fault):
 import copy,hashlib
 from tools.script_recovery.native_reviewed_cleanup_returns import restore
 captured,row=review_input;row=copy.deepcopy(row);fn=copy.deepcopy(captured['fn']);source=captured['source'];data=captured['data']
 if fault=='no_review':assert restore(source,fn,None,data)==source;return
 if fault.endswith('Sha256'):row[fault]='0'*64
 elif fault in ('movieSite','releaseSite','tailEnd','frameBytes','resourceSlot'):row[fault]+=4
 elif fault in ('movieAlias','resourceAlias'):row[fault]='unknown'
 elif fault=='instruction':row['nativeInstructions']['0xec7fda']='00'
 elif fault=='missingInstruction':del row['nativeInstructions']['0xec7fda']
 elif fault=='helper':row['helperBytes']['0x7e74d0']='00'*66
 elif fault=='missingHelper':del row['helperBytes']['0x6e7b80']
 elif fault=='truncatedHelper':row['helperBytes']['0x6e7b80']=row['helperBytes']['0x6e7b80'][:64]
 elif fault=='phi_inventory':row['movieAssignments']=[]
 elif fault.startswith('metadata_'):
  call=next(c for c in fn['calls'] if int(c['site'],16)==row['releaseSite'])
  if fault=='metadata_depth':call['depth']+=4
  elif fault=='metadata_target':call['target']='0x006E7B80'
  else:call['ecxStack']+=4
 else:
  if fault=='source_label':source=source.replace('LAB_00ec7fd1:','LAB_00ec7fd2:')
  elif fault=='source_duplicate':source+='\nLAB_00ec7fd1:\n  RESOURCE_DestroyMovie(pCVar11);'
  elif fault=='source_alias':source=source.replace('RESOURCE_DestroyMovie(pCVar11);','RESOURCE_DestroyMovie(unknown);')
  else:source=source.replace('xStack_b8 = RESOURCE_NewResource();','unknown = RESOURCE_NewResource();')
  row['loweredSha256']=hashlib.sha256(source.replace('\r\n','\n').encode()).hexdigest()
 with pytest.raises(ValueError,match='reviewed cleanup return'):restore(source,fn,row,data)

def test_original_native_phi_edges_and_return():
 import itertools,struct
 import unicorn as u
 from unicorn import x86_const as x
 from tools.script_recovery.lift_native_lua import RData
 rd=RData();cpu=u.Uc(u.UC_ARCH_X86,u.UC_MODE_32)
 cpu.mem_map(0xec6000,0x2000);cpu.mem_write(0xec68a0,rd.bytes_at(0xec68a0,5962))
 cpu.mem_map(0x300000,0x10000);cpu.mem_map(0x6e7000,0x1000);cpu.mem_map(0x7e7000,0x1000)
 cpu.mem_map(0x204000,0x1000);cpu.mem_write(0x204000,b'\xc3')
 cpu.mem_write(0x6e7b80,b'\xc3');cpu.mem_write(0x7e74d0,b'\xc3')
 state={}
 def put(address,value):cpu.mem_write(address,struct.pack('<I',value&0xffffffff))
 def word(address):return struct.unpack('<I',cpu.mem_read(address,4))[0]
 def hook(machine,address,size,user):
  if address==0x204000:machine.emu_stop();return
  if address in (0x6e7b80,0x7e74d0):
   state['events'].append(('movie' if address==0x6e7b80 else 'resource',machine.reg_read(x.UC_X86_REG_ECX)))
   sp=machine.reg_read(x.UC_X86_REG_ESP)
   machine.reg_write(x.UC_X86_REG_EIP,word(sp));machine.reg_write(x.UC_X86_REG_ESP,sp+4)
 cpu.hook_add(u.UC_HOOK_CODE,hook)
 rows=[];balanced=0x300800
 for (edge,slot),seed in itertools.product(((0xec7f22,0x74),(0xec7f37,0x74),(0xec7f4c,0xc8),(0xec7f64,0xc8),(0xec7fca,0x84)),(0,0x12345678,0xffffffff)):
  state={'events':[]}
  saved=[(seed+i*0x1234567)&0xffffffff for i in range(4)]
  for index,value in enumerate(saved):put(balanced+index*4,value)
  put(balanced+216,0x204000)
  cpu.reg_write(x.UC_X86_REG_ESP,balanced);cpu.reg_write(x.UC_X86_REG_ECX,0xdeadbeef)
  cpu.emu_start(edge,0x204000,count=100)
  assert state['events']==[('movie',balanced+slot),('resource',balanced+0x20)]
  assert cpu.reg_read(x.UC_X86_REG_ESP)==balanced+220
  assert [cpu.reg_read(reg) for reg in (x.UC_X86_REG_EBP,x.UC_X86_REG_EDI,x.UC_X86_REG_ESI,x.UC_X86_REG_EBX)]==saved
  rows.append({'edge':hex(edge),'movie_entry_slot':slot-216,'saved_seed':seed,'events':state['events']})
 assert len(rows)==15
