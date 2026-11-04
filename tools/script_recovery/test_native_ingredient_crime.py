import json,struct,itertools,sys
from pathlib import Path
import pytest
ROOT=next(p for p in Path(__file__).resolve().parents if (p/"tools/script_recovery/script_units.py").exists())

def test_original_nested_getter_preserves_pending_crime_words():
 import unicorn as u
 from unicorn import x86_const as x
 from tools.script_recovery.lift_native_lua import RData
 from tools.script_recovery.convert_quest_unit import callee_stack_words
 from tools.script_recovery.annotate_interface_slots import load_slots
 rd=RData();getter=int.from_bytes(rd.bytes_at(0x1260f0c+0x120,4),'little');crime=int.from_bytes(rd.bytes_at(0x1260f0c+0x708,4),'little')
 assert load_slots()[0x708]=='AddCrimeCommitted' and callee_stack_words(crime)==6 and callee_stack_words(getter)==2
 cpu=u.Uc(u.UC_ARCH_X86,u.UC_MODE_32)
 for a,n in ((0xecb000,0x2000),(0x300000,0x10000),(0x500000,0x6000),(0x200000,0x1000)):cpu.mem_map(a,n)
 cpu.mem_write(0xecbb44,rd.bytes_at(0xecbb44,0x2c));cpu.mem_write(0x200100,b'\xc3');cpu.mem_write(0x200200,b'\xc3')
 def put(a,v):cpu.mem_write(a,struct.pack('<I',v&0xffffffff))
 def word(a):return struct.unpack('<I',cpu.mem_read(a,4))[0]
 state={};rows=[]
 def hook(machine,a,size,user):
  if a==0x200100:
   sp=machine.reg_read(x.UC_X86_REG_ESP);out,name=word(sp+4),word(sp+8)
   assert machine.reg_read(x.UC_X86_REG_ECX)==state['gsi'] and out==state['stack']+0xe4 and name==state['stack']+0x44
   assert [word(sp+12+4*i) for i in range(5)]==[6,1,0,0,7]
   put(out,0x505000);put(out+4,state['actor']);put(out+8,0);machine.reg_write(x.UC_X86_REG_EAX,out)
   machine.reg_write(x.UC_X86_REG_EIP,word(sp));machine.reg_write(x.UC_X86_REG_ESP,sp+12)
  elif a==0x200200:
   sp=machine.reg_read(x.UC_X86_REG_ESP);args=[word(sp+4+4*i) for i in range(6)]
   assert machine.reg_read(x.UC_X86_REG_ECX)==state['gsi'] and args==[state['stack']+0xe4,6,1,0,0,7]
   assert word(args[0]+4)==state['actor'];state['args']=args
   machine.reg_write(x.UC_X86_REG_EIP,word(sp));machine.reg_write(x.UC_X86_REG_ESP,sp+28)
 cpu.hook_add(u.UC_HOOK_CODE,hook)
 for gsi,owner,sp,actor in itertools.product((0x500000,0x501000,0x502000),(0x503000,0x504000),(0x300800,0x301800),(0,811,997)):
  state=dict(gsi=gsi,owner=owner,stack=sp,actor=actor);table=gsi+0x800;put(gsi,table);put(table+0x120,0x200100);put(table+0x708,0x200200);put(owner+4,gsi)
  cpu.reg_write(x.UC_X86_REG_ESI,owner);cpu.reg_write(x.UC_X86_REG_ESP,sp);cpu.emu_start(0xecbb44,0xecbb70,count=100)
  assert cpu.reg_read(x.UC_X86_REG_ESP)==sp and 'args' in state;rows.append(state.copy())
 assert len(rows)==36

def test_crime_manifest_preserves_nullable_actor_positions():
 from tools.script_recovery.lift_native_lua import load_manifest
 params=load_manifest()['AddCrimeCommitted']['parameters']
 assert [p['name'] for p in params]==['spVillage','crimeType','bUnknown','spCriminal','spVictim','deedType']
 assert all('CScriptThing' in params[i]['type'] for i in (0,3,4))
 assert params[5]['type']=='int'

from tools.script_recovery.test_native_ingredient_alive import review_input
@pytest.mark.parametrize('operand',('deed_assignment','null_call','native_push7','native_null_actor'))
def test_stale_crime_operands_fail_closed(review_input,operand):
 from tools.script_recovery.native_reviewed_stack_aliases import restore
 from tools.script_recovery.test_native_inline_self_argument import ChangedBytes
 captured,row=review_input;text=captured['text'];rd=captured['rd']
 if operand=='deed_assignment':text=text.replace('iVar11 = 7;','iVar11 = 0;')
 elif operand=='null_call':text=text.replace('pCVar5,pCVar9,iVar11);','pCVar14,pCVar9,iVar11);')
 elif operand=='native_push7':rd=ChangedBytes(rd,{0xecbb49:bytes.fromhex('6a00')})
 else:rd=ChangedBytes(rd,{0xecbb4b:bytes.fromhex('6a01')})
 with pytest.raises(ValueError,match='reviewed stack alias'):restore(text,captured['fn'],row,rd)
