"""Native and adversarial coverage for the omitted by-value self argument variant."""
import copy,importlib.util,json,struct,sys
from pathlib import Path
import pytest
from capstone import Cs,CS_ARCH_X86,CS_MODE_32
ROOT=next(p for p in Path(__file__).resolve().parents if (p/'tools/script_recovery/script_units.py').is_file());sys.path.insert(0,str(ROOT))
from tools.script_recovery.convert_quest_unit import _text_order_sites
from tools.script_recovery.lift_native_lua import RData
from tools.script_recovery.test_native_inline_self_argument import ChangedBytes
from tools.script_recovery import native_inline_self_argument as adapter
@pytest.fixture(scope='module')
def native():
 from tools.script_recovery.convert_quest_unit import UnitConverter
 c=UnitConverter(ROOT/'refs/script_recovery/sick_child/translation_unit_typed.json')
 fn=c.native('0x00ecf7f0');source=fn['decompile'];entries=_text_order_sites(source,fn)
 site=next(e for e in entries if e[4]=='0xd30');return fn,source,entries,site,RData()
def test_omitted_argument_list(native):
 fn,source,entries,site,rd=native;a,b=site[:2]
 assert not site[2]
 assert adapter.restore(source,fn,entries,rd)==source[:a]+'(CScriptThing *)((int)this + 8),0'+source[b:]
@pytest.mark.parametrize('kind',('owner','flag','data','info','type','null_test','null_target','refcount','slot','callee','vtable','interior_entry'))
def test_changed_native_remains_unresolved(native,kind):
 fn,source,entries,site,rd=native
 addresses=dict(owner=0xecf7f9,flag=0xecf8ce,data=0xecf8e1,info=0xecf8e7,type=0xecf8db,
 null_test=0xecf8ea,null_target=0xecf8ef,refcount=0xecf8f1,slot=0xecf8f8,
 callee=adapter.CALLEE,vtable=0x01260f0c+0xd30,interior_entry=0xecf8c8)
 at=addresses[kind]
 if kind=='interior_entry':at=0xecf8ad;patch=b'\xeb'+bytes([(0xecf8d3-(at+2))&255])
 elif kind=='vtable':patch=b'\x00'*4
 elif kind=='null_target':patch=b'\x74\x01'
 elif kind=='flag':patch=b'\x6a\x01'
 else:patch=b'\x90'
 assert adapter.restore(source,fn,entries,ChangedBytes(rd,{at:patch}))==source
@pytest.mark.parametrize('kind',('receiver','printed_operand','slot','not_vtable','missing_entries'))
def test_changed_source_metadata_remains_unresolved(native,kind):
 fn,source,entries,site,rd=native;changed=copy.deepcopy(entries)
 index=next(i for i,e in enumerate(changed) if e[4]=='0xd30')
 a,b,args,record,key,vt=changed[index]
 if kind=='missing_entries':changed=[]
 else:
  if kind=='receiver':record['ecxValue']=8
  elif kind=='printed_operand':args=['1']
  elif kind=='slot':record['slot']='0xd34'
  elif kind=='not_vtable':vt=False
  changed[index]=(a,b,args,record,key,vt)
 assert adapter.restore(source,fn,changed,rd)==source
@pytest.mark.parametrize('reference_count',(None,1,2,17))
def test_original_copy_layout(reference_count,native):
 import unicorn as u
 from unicorn import x86_const as x
 rd=native[-1];cpu=u.Uc(u.UC_ARCH_X86,u.UC_MODE_32)
 for a,n in ((0xecf000,0x1000),(0x300000,0x10000),(0x310000,0x10000),(adapter.CALLEE&~0xfff,0x1000)):
  cpu.mem_map(a,n)
 cpu.mem_write(0xecf8ce,rd.bytes_at(0xecf8ce,0xecf8fe-0xecf8ce));cpu.mem_write(adapter.CALLEE,b'\xc3')
 def put(a,v):cpu.mem_write(a,struct.pack('<I',v&0xffffffff))
 def word(a):return struct.unpack('<I',cpu.mem_read(a,4))[0]
 balanced,owner,info,gsi,vtable=0x300800,0x310000,0x312000,0x313000,0x314000
 put(owner+4,gsi);put(owner+12,0x12345678);put(owner+16,info if reference_count is not None else 0)
 put(gsi,vtable);put(vtable+0xd30,adapter.CALLEE)
 if reference_count is not None:put(info,reference_count)
 calls=[]
 def hook(machine,a,size,user):
  if a==adapter.CALLEE:
   sp=machine.reg_read(x.UC_X86_REG_ESP)
   calls.append((machine.reg_read(x.UC_X86_REG_ECX),[word(sp+k) for k in (4,8,12,16)]))
   if reference_count is not None:
    assert word(info)==reference_count+1;put(info,word(info)-1)
   machine.reg_write(x.UC_X86_REG_EIP,word(sp));machine.reg_write(x.UC_X86_REG_ESP,sp+20)
  elif a==0xecf8fe:machine.emu_stop()
 cpu.hook_add(u.UC_HOOK_CODE,hook);cpu.reg_write(x.UC_X86_REG_ESP,balanced);cpu.reg_write(x.UC_X86_REG_EBP,owner)
 cpu.emu_start(0xecf8ce,0xecf8fe,count=100)
 assert calls==[(gsi,[0x1238c8c,0x12345678,info if reference_count is not None else 0,0])]
 assert cpu.reg_read(x.UC_X86_REG_ESP)==balanced
 if reference_count is not None:assert word(info)==reference_count

from lupa.lua54 import LuaRuntime
import itertools

@pytest.fixture(scope='module')
def generated(tmp_path_factory):
 from unittest.mock import patch
 from tools.script_recovery.convert_quest_unit import UnitConverter
 from tools.script_recovery.build_readable_unit import readable_file
 c=UnitConverter(ROOT/'refs/script_recovery/sick_child/translation_unit_typed.json')
 unit=json.loads((ROOT/'refs/script_recovery/sick_child/units/V_SickChild.json').read_text())
 unit['entities']={'WomanToAttract':unit['entities']['WomanToAttract']};unit['quest']['functions']={};unit['quest']['helpers']={}
 out=tmp_path_factory.mktemp('woman_inline')
 with patch.object(c,'constructor_defaults',return_value={}):c.convert(unit,out)
 source=(out/'FSE/V_SickChild/Entities/WomanToAttract.lua').read_text()
 return {'draft':source,'readable':readable_file(source)[0]}

H=r'''
return function(limit,acquireDelay,killed)
 local frame,registered,pushes=0,false,0;local q,me,r={},{},{}
 local function term()return frame>=limit or registered end
 function q:GetHero()return {} end
 function q:RetailResources()return r end
 function q:NewScriptFrame(actor)assert(actor==me and not term());frame=frame+1;return not term()end
 function q:IsActiveThreadTerminating()return term()end
 function q:GetStateBool(name)assert(name=='LaughingWomanKilled');return killed end
 function q:RemoveThing(actor,a,b)assert(actor==me and not a and b)end
 function q:SetIsPushableByHero(actor,value)assert(actor==me and value==false,'missing native by-value actor/false');pushes=pushes+1 end
 function q:SetThingHasInformation(actor,a,b,c)assert(actor==me and not a and not b and not c)end
 function q:RegisterTimer()assert(not registered and pushes==1);registered=true;return 713 end
 function q:DeregisterTimer(id)assert(registered and id==713);registered=false;frame=limit end
 function r:MemberResource(name,actor)assert(name=='seh_me' and actor==me);return 811 end
 function r:PrepareResource(id)assert(id==811)end
 function me:AcquireControl(kind)assert(kind==4);return frame>=acquireDelay end
 debug.sethook(function()error('instruction budget')end,'',50000)
 Main(q,me);debug.sethook()
 assert(not registered and pushes==((limit>math.max(1,acquireDelay)) and 1 or 0))
 return pushes
end
'''
CASES=list(itertools.product((1,2,5),(0,3),(False,True)))
@pytest.mark.parametrize('style',('draft','readable'))
@pytest.mark.parametrize('limit,acquire,killed',CASES)
def test_startup(generated,style,limit,acquire,killed):
 source=generated[style]
 lua=LuaRuntime();lua.execute(source);lua.execute(H)(limit,acquire,killed)
