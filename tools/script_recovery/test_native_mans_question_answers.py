import copy,itertools,sys
from pathlib import Path
import pytest
from lupa.lua54 import LuaRuntime
ROOT=next(p for p in Path(__file__).resolve().parents if (p/'tools/script_recovery/script_units.py').is_file());sys.path.insert(0,str(ROOT))
from tools.script_recovery.test_native_mans_letter_latch import generated,latch_review_input
from tools.script_recovery.test_native_mans_letter_rewards import H
@pytest.mark.parametrize('fault',('first_poll','second_poll','pending_gate','accept_test','source_test','count','source'))
def test_stale_question_answer_review_rejected(latch_review_input,fault):
 from tools.script_recovery.native_reviewed_stack_aliases import restore
 from tools.script_recovery.test_native_inline_self_argument import ChangedBytes
 captured,row=latch_review_input;row=copy.deepcopy(row);rd=captured['rd'];source=captured['text'];fn=captured['fn']
 sites={'first_poll':(0xeca29a,'ff92a0000000'),'second_poll':(0xeca5cc,'ff90a0000000'),'pending_gate':(0xeca2a4,'7c28'),'accept_test':(0xeca2e0,'83ff00'),'source_test':(0xeca649,'85c0')}
 if fault in sites:
  site,raw=sites[fault];rd=ChangedBytes(rd,{site:bytes.fromhex(raw)})
 elif fault=='count':row['editCounts'][-4]=3
 else:source=source.replace('while (iVar5 < 0)','while (iVar5 <= 0)',1)
 with pytest.raises(ValueError,match='reviewed stack alias'):restore(source,fn,row,rd)
def test_unreviewed_question_answers_remain_explicit(latch_review_input):
 from tools.script_recovery.native_reviewed_stack_aliases import restore
 c,row=latch_review_input;assert restore(c['text'],c['fn'],None,c['rd'])==c['text']
@pytest.mark.parametrize('mode,answer,pending',list(itertools.product(('presented','talk-yes','talk-no'),(0,1,2),(-1,-2,-99))))
def test_signed_pending_question_outputs(generated,mode,answer,pending):
 h=H.replace('if polls<=wait then return -1 end',f'if polls<=wait then return {pending} end');assert f'return {pending} end' in h
 results=[]
 for s in generated.values():
  lua=LuaRuntime();lua.execute(s);results.append(lua.execute(h)(mode,0,answer,True,True,2,0,'',713))
 assert results[0]==results[1]
@pytest.mark.parametrize('style',('draft','readable'))
@pytest.mark.parametrize('fault',('pending_zero','pending_invert','accept_invert','source_invert','source_clobber','boolean_answer'))
def test_wrong_question_answer_lifetime_detected(generated,style,fault):
 s=generated[style]
 old,new={
  'pending_zero':('while letterQuestionAnswer < 0 do','while letterQuestionAnswer <= 0 do'),
  'pending_invert':('while letterQuestionAnswer < 0 do','while letterQuestionAnswer > 0 do'),
  'accept_invert':('if letterQuestionAnswer ~= 1 then','if letterQuestionAnswer == 1 then'),
  'source_invert':('if letterQuestionAnswer == 0 then','if letterQuestionAnswer ~= 0 then'),
  'source_clobber':('if letterQuestionAnswer == 0 then','letterQuestionAnswer = 0; if letterQuestionAnswer == 0 then'),
  'boolean_answer':('letterQuestionAnswer = quest:MsgIsQuestionAnsweredYesOrNo()','letterQuestionAnswer = quest:MsgIsQuestionAnsweredYesOrNo() ~= 0'),
 }[fault]
 assert old in s;s=s.replace(old,new);lua=LuaRuntime();lua.execute(s)
 with pytest.raises(Exception):lua.execute(H)('talk-yes',0,0 if fault=='pending_zero' else 1,True,True,2,0,'',31)
def test_named_question_answer_keeps_numeric_tests(generated):
 for s in generated.values():
  assert s.count('letterQuestionAnswer = quest:MsgIsQuestionAnsweredYesOrNo()')==4
  assert s.count('while letterQuestionAnswer < 0 do')==2
  assert 'if letterQuestionAnswer ~= 1 then' in s and 'if letterQuestionAnswer == 0 then' in s

import itertools,struct,sys
from pathlib import Path
ROOT=next(p for p in Path(__file__).resolve().parents if (p/'tools/script_recovery/script_units.py').is_file());sys.path.insert(0,str(ROOT))
def test_native_question_answer_wait_and_selection():
 from tools.script_recovery.lift_native_lua import RData
 from tools.script_recovery.convert_quest_unit import callee_stack_words
 from tools.script_recovery.annotate_interface_slots import load_slots
 import unicorn as u
 from unicorn import x86_const as x
 rd=RData();slots={0x9c:('MsgIsQuestionAnsweredYesOrNo',0),0x1c:('NewScriptFrame',0),0x678:('CameraUseCameraPoint',5)}
 for slot,(name,purge) in slots.items():
  assert load_slots()[slot]==name and callee_stack_words(int.from_bytes(rd.bytes_at(0x1260f0c+slot,4),'little'))==purge
 cpu=u.Uc(u.UC_ARCH_X86,u.UC_MODE_32)
 for a,n in ((0xec9000,0x3000),(0x99e000,0x1000),(0xf35000,0x1000),(0x300000,0x10000),(0x500000,0x10000),(0x200000,0x1000)):cpu.mem_map(a,n)
 cpu.mem_write(0xec9000,rd.bytes_at(0xec9000,0x3000))
 endpoints={0x200100+i*16:slot for i,slot in enumerate(slots)}
 for a in (*endpoints,0x99ebf0,0x99eae0,0xf35b30):cpu.mem_write(a,b'\xc3')
 def put(a,v):cpu.mem_write(a,struct.pack('<I',v&0xffffffff))
 def word(a):return struct.unpack('<I',cpu.mem_read(a,4))[0]
 state={};layouts=0
 def hook(m,a,size,user):
  if a in (0xeca2e5,0xeca341,0xeca651,0xeca801,0xecaf49,0xeca325):state['exit']=a;m.emu_stop();return
  if a not in (*endpoints,0x99ebf0,0x99eae0,0xf35b30):return
  sp=m.reg_read(x.UC_X86_REG_ESP);ecx=m.reg_read(x.UC_X86_REG_ECX);s=state['stack'];purge=0
  if a==0xf35b30:
   assert ecx==state['owner'];state['terms']+=1;m.reg_write(x.UC_X86_REG_EAX,int(state['termAt']==state['terms']))
  elif a==0x99ebf0:
   assert ecx==s+0x68 and rd.string_at(word(sp+4))=='CAM_REL_WOMAN' and word(sp+8)==0xffffffff
   put(ecx,state['cookie']);state['ctors']+=1;purge=8
  elif a==0x99eae0:assert ecx==s+0x68 and word(ecx)==state['cookie'];state['dtors']+=1
  else:
   slot=endpoints[a];assert ecx==state['gsi'];purge=slots[slot][1]*4
   if slot==0x9c:
    state['queries']+=1;answer=state['pending'] if state['queries']<=state['wait'] else state['answer'];m.reg_write(x.UC_X86_REG_EAX,answer&0xffffffff)
   elif slot==0x1c:state['frames']+=1;m.reg_write(x.UC_X86_REG_EAX,0x13579bdf)
   else:
    assert [word(sp+i*4) for i in range(1,6)]==[s+0x68,state['owner']+8,0xbf800000,0,0xffffffff]
    assert word(s+0x68)==state['cookie'] and m.reg_read(x.UC_X86_REG_EDI)==state['answer'];state['cameras']+=1;m.reg_write(x.UC_X86_REG_EAX,0x76543210)
  m.reg_write(x.UC_X86_REG_EIP,word(sp));m.reg_write(x.UC_X86_REG_ESP,sp+4+purge)
 cpu.hook_add(u.UC_HOOK_CODE,hook)
 for role,answer,wait,pending,termAt,owner,gsi,sp,cookie in itertools.product(('give','source'),(0,1,2),(0,1,3),(-1,-2),(0,1,2,4,5),(0x501000,0x502000),(0x500000,0x503000),(0x300800,0x301800),(713,0xdead0000)):
  state=dict(role=role,answer=answer,wait=wait,pending=pending,termAt=termAt,owner=owner,gsi=gsi,stack=sp,cookie=cookie,terms=0,queries=0,frames=0,ctors=0,dtors=0,cameras=0,exit=None)
  put(gsi,gsi+0x800);put(owner+4,gsi)
  for endpoint,slot in endpoints.items():put(gsi+0x800+slot,endpoint)
  cpu.reg_write(x.UC_X86_REG_ESI,owner);cpu.reg_write(x.UC_X86_REG_EBP,owner+8);cpu.reg_write(x.UC_X86_REG_ESP,sp)
  cpu.emu_start(0xeca295 if role=='give' else 0xeca5c7,0xecaf4a,count=500)
  abortedPending=bool(termAt and termAt<=wait);abortedAfter=termAt==wait+1
  if abortedPending:assert state['exit']==0xecaf49 and state['frames']==termAt and state['queries']==termAt
  elif abortedAfter:assert state['exit']==0xeca325 and state['frames']==wait and state['queries']==wait+1
  else:
   assert state['exit']==(0xeca2e5 if answer==1 else 0xeca341) if role=='give' else state['exit']==(0xeca651 if answer==0 else 0xeca801)
   assert state['queries']==wait+1 and state['frames']==wait and state['terms']==wait+2 and m_answer(cpu,x)==answer
  camera=int(role=='source' and not abortedPending and not abortedAfter)
  assert state['ctors']==state['dtors']==state['cameras']==camera and cpu.reg_read(x.UC_X86_REG_ESP)==sp
  layouts+=1
 assert layouts==2880
def m_answer(cpu,x):return cpu.reg_read(x.UC_X86_REG_EDI)
