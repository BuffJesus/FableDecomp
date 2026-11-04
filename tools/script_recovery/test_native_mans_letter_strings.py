import copy,json,sys
from pathlib import Path
from unittest.mock import patch
import pytest
ROOT=next(p for p in Path(__file__).resolve().parents if (p/'tools/script_recovery/script_units.py').is_file());sys.path.insert(0,str(ROOT))
from tools.script_recovery.test_native_mans_letter_latch import generated,latch_review_input
@pytest.mark.parametrize('fault',('snapshot_ctor','copy','presented_out','destroy','count','source','alias_duplicate'))
def test_stale_letter_string_review_rejected(latch_review_input,fault):
 from tools.script_recovery.native_reviewed_stack_aliases import restore
 from tools.script_recovery.test_native_inline_self_argument import ChangedBytes
 captured,row=latch_review_input;row=copy.deepcopy(row);source=captured['text'];rd=captured['rd'];fn=captured['fn']
 sites={'snapshot_ctor':(0xec9e38,'8d4c2410'),'copy':(0xec9e65,'e846510000'),'presented_out':(0xec9e87,'8d442414'),'destroy':(0xecaf77,'8d4c2414')}
 if fault in sites:
  site,raw=sites[fault];rd=ChangedBytes(rd,{site:bytes.fromhex(raw)})
 elif fault=='count':row['editCounts'][-1]-=1
 elif fault=='source':source=source.replace('undefined4 *xStack_128','undefined4 *wrongSnapshot',1)
 else:row['additionalAliases'].append('expectedLetterDefinition')
 with pytest.raises(ValueError,match='reviewed stack alias'):restore(source,fn,row,rd)
def test_unreviewed_letter_strings_remain_explicit(latch_review_input):
 from tools.script_recovery.native_reviewed_stack_aliases import restore
 captured,row=latch_review_input;assert restore(captured['text'],captured['fn'],None,captured['rd'])==captured['text']
def test_generated_letter_string_snapshot_and_output(generated):
 for s in generated.values():
  assert 'expectedLetterDefinition' in s and 'local_presentedItemName' in s
  assert 'local_presentedItemName = _G.g_PresentedItemName' in s and 'expectedLetterDefinition == local_presentedItemName' in s
  assert 'expectedLetterDefinition == ""' not in s

import itertools,struct,sys
from pathlib import Path
ROOT=next(p for p in Path(__file__).resolve().parents if (p/'tools/script_recovery/script_units.py').is_file());sys.path.insert(0,str(ROOT))
def test_native_letter_snapshot_and_presented_string_slots():
 from tools.script_recovery.lift_native_lua import RData
 from tools.script_recovery.annotate_interface_slots import load_thing_slots
 from tools.script_recovery.convert_quest_unit import callee_stack_words
 import unicorn as u
 from unicorn import x86_const as x
 rd=RData();assert load_thing_slots()[0x8c][0]=='MsgIsPresentedWithItem'
 assert callee_stack_words(int.from_bytes(rd.bytes_at(0x1238c8c+0x8c,4),'little'))==1
 cpu=u.Uc(u.UC_ARCH_X86,u.UC_MODE_32)
 for a,n in ((0xec9000,0x3000),(0x99e000,0x1000),(0x415000,0x1000),(0x143e000,0x1000),(0x300000,0x10000),(0x500000,0x10000),(0x200000,0x1000)):cpu.mem_map(a,n)
 cpu.mem_write(0xec9000,rd.bytes_at(0xec9000,0x3000))
 for a in (0x99e4b0,0x99efb0,0x99eae0,0x415d70,0x200100):cpu.mem_write(a,b'\xc3')
 def put(a,v):cpu.mem_write(a,struct.pack('<I',v&0xffffffff))
 def word(a):return struct.unpack('<I',cpu.mem_read(a,4))[0]
 state={};layouts=0
 def hook(m,a,size,user):
  if a not in (0x99e4b0,0x99efb0,0x99eae0,0x415d70,0x200100):return
  sp=m.reg_read(x.UC_X86_REG_ESP);ecx=m.reg_read(x.UC_X86_REG_ECX);s=state['stack'];purge=0
  if a==0x99e4b0:
   assert ecx in (s+0x10,s+0x14);put(ecx,0);state['ctors'].append(ecx)
  elif a==0x415d70:
   assert ecx==0x50773c and word(sp+4)==s+0x18;put(s+0x18,state['cookie']+1);m.reg_write(x.UC_X86_REG_EAX,s+0x18);purge=4;state['conversion']+=1
  elif a==0x99efb0:
   assert ecx==s+0x14 and word(sp+4)==s+0x18;put(ecx,word(s+0x18));purge=4;state['copies']+=1
  elif a==0x99eae0:
   assert ecx in (s+0x10,s+0x14,s+0x18);state['dtors'].append(ecx)
  else:
   assert ecx==state['owner']+8 and word(sp+4)==s+0x10
   if state['message']:put(s+0x10,state['cookie']+state['message'])
   m.reg_write(x.UC_X86_REG_EAX,state['message']);purge=4;state['queries']+=1
  m.reg_write(x.UC_X86_REG_EIP,word(sp));m.reg_write(x.UC_X86_REG_ESP,sp+4+purge)
 cpu.hook_add(u.UC_HOOK_CODE,hook)
 for owner,sp,cookie,messages in itertools.product((0x500000,0x501000),(0x300800,0x301800),(713,0xdead0000),((0,),(1,),(255,),(1,0),(255,0))):
  state=dict(owner=owner,stack=sp,cookie=cookie,message=0,ctors=[],conversion=0,copies=0,queries=0,dtors=[])
  put(owner+8,0x505000);put(0x50508c,0x200100);put(0x143e90c,0x507000)
  cpu.reg_write(x.UC_X86_REG_ESP,sp);cpu.reg_write(x.UC_X86_REG_EBP,owner+8)
  cpu.emu_start(0xec9e2f,0xec9e73,count=100)
  assert state['ctors']==[sp+0x10,sp+0x14] and state['copies']==state['conversion']==1 and state['dtors']==[sp+0x18]
  assert word(sp+0x14)==cookie+1 and word(sp+0x10)==0 and cpu.reg_read(x.UC_X86_REG_ESP)==sp
  presented=0
  for message in messages:
   state['message']=message;cpu.emu_start(0xec9e84,0xec9e94,count=100)
   if message:presented=cookie+message
   assert word(sp+0x14)==cookie+1 and word(sp+0x10)==presented and cpu.reg_read(x.UC_X86_REG_EAX)&255==message and cpu.reg_read(x.UC_X86_REG_ESP)==sp
  cpu.emu_start(0xecaf6e,0xecaf80,count=100)
  assert state['dtors']==[sp+0x18,sp+0x14,sp+0x10] and state['queries']==len(messages) and cpu.reg_read(x.UC_X86_REG_ESP)==sp
  layouts+=1
 assert layouts==40
