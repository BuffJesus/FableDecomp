"""Actual post-intro reminder counter, fallback and cleanup behavior."""
import itertools
import json
import os
from pathlib import Path
import pytest
from lupa.lua54 import LuaRuntime
ROOT=next(p for p in Path(__file__).resolve().parents if (p/'tools/script_recovery/script_units.py').is_file())
from unittest.mock import patch
from tools.script_recovery.convert_quest_unit import UnitConverter
from tools.script_recovery.build_readable_unit import readable_file

@pytest.fixture(scope='module')
def generated(tmp_path_factory):
 c=UnitConverter(ROOT/'refs/script_recovery/sick_child/translation_unit_typed.json')
 unit=json.loads((ROOT/'refs/script_recovery/sick_child/units/V_SickChild.json').read_text())
 unit['entities']={'SickChildsSister':unit['entities']['SickChildsSister']};unit['quest']['functions']={};unit['quest']['helpers']={}
 out=tmp_path_factory.mktemp('sister_reminder')
 with patch.object(c,'constructor_defaults',return_value={}):c.convert(unit,out)
 source=(out/'FSE/V_SickChild/Entities/SickChildsSister.lua').read_text()
 return {'draft':source,'readable':readable_file(source)[0]}


H=r'''
return function(maximum,limit,talkEvery,health,delay,acquireDelay)
 local frame,expected,selected,taskUntil=0,10,nil,0
 local resource,movies,paused=nil,{},false
 local queries,speeches={},{};local q,me,hero,r={},{},{},{}
 local function term()return frame>=limit end
 function q:GetHero()return hero end
 function q:RetailResources()return r end
 function q:IsActiveThreadTerminating()return term()end
 function q:NewScriptFrame(actor)assert(actor==me);frame=frame+1;return not term()end
 function q:GetStateBool(name)assert(name=='MotherIntroDone' or name=='FinishedQuest');return name=='MotherIntroDone'end
 function q:ClearThingHasInformation(actor)assert(actor==me)end
 function q:TextEntryExists(key)
  local index=tonumber(key:match('^TEXT_QST_B10_SISTER_REMINDER_POSTINTRO_([%-0-9]+)$'))
  assert(index==expected,'counter did not advance/reset as native: '..tostring(index)..' expected '..expected)
  queries[#queries+1]=index;local exists=index<=maximum
  selected=exists and index or 10
  expected=((selected+10+0x80000000)&0xffffffff)-0x80000000
  return exists
 end
 function q:PauseAllNonScriptedEntities(value)paused=value end
 function q:GetHealth(actor)assert(actor==me);return health end
 function me:IsTalkedToByHero()return frame%talkEvery==0 end
 function me:MsgIsHitByHero()return false end
 function me:MsgIsHitByAnySpecialAbilityFromHero()return false end
 function me:IsPerformingScriptTask()return frame<taskUntil end
 function me:Speak(target,key,selection,a,b,c)
  assert(target==hero and key=='TEXT_QST_B10_SISTER_REMINDER_POSTINTRO_'..selected and selection==0 and not a and b and not c)
  speeches[#speeches+1]=selected;taskUntil=frame+delay
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
 function r:DestroyMovie(id)assert(movies[id]);movies[id]=nil end
 debug.sethook(function()error('instruction budget')end,'',500000)
 Main(q,me);debug.sethook()
 assert(not resource and next(movies)==nil and not paused,'resource/movie/pause leak')
 return queries,speeches
end
'''

@pytest.mark.parametrize('style',('draft','readable'))
@pytest.mark.parametrize('maximum,limit,talkEvery,health,delay,acquire',list(itertools.product((10,30,70),(1,3,10,17),(1,3),(0,50),(0,2),(0,2))))
def test_actual_reminder(generated,style,maximum,limit,talkEvery,health,delay,acquire):
 source=generated[style]
 lua=LuaRuntime();lua.execute(source);lua.execute(H)(maximum,limit,talkEvery,health,delay,acquire)


@pytest.fixture(scope='module')
def native_counter_cases():
 import struct
 import unicorn as u
 from unicorn import x86_const as x
 from tools.script_recovery.lift_native_lua import RData
 rd=RData();cpu=u.Uc(u.UC_ARCH_X86,u.UC_MODE_32)
 cpu.mem_map(0xec6000,0x2000);cpu.mem_write(0xec68a0,rd.bytes_at(0xec68a0,5962))
 cpu.mem_map(0x300000,0x10000);cpu.mem_map(0x99f000,0x1000)
 state={}
 def hook(machine,address,size,user):
     if address==state.get('end'):
         machine.emu_stop();return
     if address==0x99f830:
         bits=machine.reg_read(x.UC_X86_REG_EDX)
         state['formatted'].append(bits if bits<2**31 else bits-2**32)
         sp=machine.reg_read(x.UC_X86_REG_ESP)
         ret=struct.unpack('<I',machine.mem_read(sp,4))[0]
         machine.reg_write(x.UC_X86_REG_EAX,machine.reg_read(x.UC_X86_REG_ECX))
         machine.reg_write(x.UC_X86_REG_ESP,sp+4)
         machine.reg_write(x.UC_X86_REG_EIP,ret)
     if address==0xec7bad and state.get('stop_at_increment'):
         machine.emu_stop()
 cpu.hook_add(u.UC_HOOK_CODE,hook)
 def run(start,end,count):
     state['end']=end
     cpu.emu_start(start,end,count=count)
 rows=[]
 for seed,exists in itertools.product((0,10,20,90,0x7ffffff5,0x7fffffff,0x80000000,0xfffffff6,0xffffffff),(False,True)):
     state={'formatted':[],'stop_at_increment':False}
     cpu.reg_write(x.UC_X86_REG_ESP,0x300800)
     run(0xec7360,0xec736b,10)
     assert cpu.reg_read(x.UC_X86_REG_EDI)==10
     assert struct.unpack('<I',cpu.mem_read(0x300850,4))[0]==10
     cpu.reg_write(x.UC_X86_REG_EDI,seed)
     run(0xec7b16,0xec7b24,20)
     assert state['formatted']==[seed if seed<2**31 else seed-2**32]
     cpu.reg_write(x.UC_X86_REG_EAX,int(exists))
     state['stop_at_increment']=True
     run(0xec7b62,0xec7b79,20)
     assert state['formatted']==([seed if seed<2**31 else seed-2**32] if exists else [seed if seed<2**31 else seed-2**32,10])
     state['stop_at_increment']=False
     run(0xec7bad,0xec7bbb,10)
     bits=((seed if exists else 10)+10)&0xffffffff
     assert cpu.reg_read(x.UC_X86_REG_EDI)==bits
     assert struct.unpack('<I',cpu.mem_read(0x300850,4))[0]==bits
     cpu.reg_write(x.UC_X86_REG_EDI,0xdeadbeef)
     run(0xec7855,0xec7859,5)
     assert cpu.reg_read(x.UC_X86_REG_EDI)==bits and cpu.reg_read(x.UC_X86_REG_ESP)==0x300800
     rows.append({'seed':seed,'exists':exists,'formatted':state['formatted'],'next_bits':bits,'next_signed':bits if bits<2**31 else bits-2**32})
 return rows

@pytest.mark.parametrize('style',('draft','readable'))
def test_native_high_bit_and_wrap_cases(generated,style,native_counter_cases):
 import re
 expression=re.search(r'nextPostIntroReminderNumber = (\(\(postIntroReminderNumber[^\n]+)',generated[style])[1]
 lua=LuaRuntime();run=lua.execute('return function(postIntroReminderNumber) return '+expression+' end')
 for row in native_counter_cases:
  seed=row['seed'] if row['exists'] else 10
  assert run(seed)==row['next_signed']

@pytest.fixture(scope='module')
def review_input(tmp_path_factory):
 from tools.script_recovery import native_reviewed_stack_aliases as adapter
 c=UnitConverter(ROOT/'refs/script_recovery/sick_child/translation_unit_typed.json')
 unit=json.loads((ROOT/'refs/script_recovery/sick_child/units/V_SickChild.json').read_text())
 row=unit['reviewedStackScalarAliases']['0x00ec68a0']
 unit['entities']={'SickChildsSister':unit['entities']['SickChildsSister']};unit['quest']['functions']={};unit['quest']['helpers']={}
 original=adapter.restore;captured={}
 def capture(source,fn,row,data):
  if str(fn['address']).lower()=='0x00ec68a0':captured.update(source=source,fn=fn,data=data)
  return original(source,fn,row,data)
 with patch.object(c,'constructor_defaults',return_value={}),patch.object(adapter,'restore',capture):
  c.convert(unit,tmp_path_factory.mktemp('sister_counter_review'))
 return captured,row

@pytest.mark.parametrize('fault',('no_review','native','export','instruction','missing','duplicate','collision','alias_duplicate','few_native','empty_edits','invalid_edit'))
def test_native_counter_review_guards(review_input,fault):
 import copy
 from tools.script_recovery.native_reviewed_stack_aliases import restore
 captured,row=review_input;row=copy.deepcopy(row);fn=copy.deepcopy(captured['fn']);source=captured['source'];data=captured['data']
 if fault=='no_review':assert restore(source,fn,None,data)==source;return
 if fault=='native':
  original=data
  class Changed:
   def bytes_at(self,address,size):return b'\x90'*size if address==0xec68a0 else original.bytes_at(address,size)
  data=Changed()
 elif fault=='export':fn['decompile']+='\n'
 elif fault=='instruction':row['nativeInstructions']['0x00ec7bad']='00'
 elif fault=='missing':source=source.replace(row['edits'][0][0],'')
 elif fault=='duplicate':source+='\n'+row['edits'][0][0]
 elif fault=='collision':source+=' '+row['alias']
 elif fault=='alias_duplicate':row['additionalAliases'].append(row['alias'])
 elif fault=='few_native':row['nativeInstructions']=dict(list(row['nativeInstructions'].items())[:2])
 elif fault=='empty_edits':row['edits']=[]
 elif fault=='invalid_edit':row['edits'][0][1]='int unknown;'
 with pytest.raises(ValueError,match='reviewed stack alias'):restore(source,fn,row,data)
