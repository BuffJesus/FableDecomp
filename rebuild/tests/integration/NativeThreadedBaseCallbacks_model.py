"""Frozen independent register/arena model, callable immutable original/compiled runners."""
import itertools,struct
from unicorn import Uc,UC_ARCH_X86,UC_MODE_32
from unicorn.x86_const import UC_X86_REG_EAX,UC_X86_REG_EBX,UC_X86_REG_ECX,UC_X86_REG_EDX,UC_X86_REG_ESI,UC_X86_REG_EDI,UC_X86_REG_EBP,UC_X86_REG_ESP,UC_X86_REG_EFLAGS,UC_X86_REG_EIP

def run(bodies,mode):
 ARENA=0x200000;SIZE=0x8000;SP=ARENA+0x7000;STOP=0x100100
 registers=(UC_X86_REG_EAX,UC_X86_REG_EBX,UC_X86_REG_ECX,UC_X86_REG_EDX,UC_X86_REG_ESI,UC_X86_REG_EDI,UC_X86_REG_EBP)
 thisvalues=(0,1,ARENA+0x100,0x80000000);ptrvalues=(0,1,ARENA+0x300,0xffffffff);eaxvalues=(0,0xffffffff,0x12345678,0x80000000);flagvalues=(0x202,0x247,0xed7);receipts=[]
 for role,code in bodies.items():
  arguments=itertools.product(thisvalues,ptrvalues,(0,3,0x80000000,0xffffffff),eaxvalues,flagvalues) if role=='begin' else ((t,b,0,e,f) for t,b,e,f in itertools.product(thisvalues,(0,1,2,255,256,0x80000000,0xffffffff,0x12345678),eaxvalues,flagvalues))
  count=0
  for this,arg1,arg2,eax,flags in arguments:
   arena=bytearray((i*17+count)&255 for i in range(SIZE));struct.pack_into('<III',arena,SP-ARENA,STOP,arg1,arg2)
   u=Uc(UC_ARCH_X86,UC_MODE_32);u.mem_map(0x100000,0x1000);u.mem_map(ARENA,SIZE);u.mem_write(0x100000,code);u.mem_write(ARENA,bytes(arena))
   initial={r:((count+1)*0x1234567+i*0x3456789)&0xffffffff for i,r in enumerate(registers)};initial[UC_X86_REG_EAX]=eax;initial[UC_X86_REG_ECX]=this
   for r,v in initial.items():u.reg_write(r,v)
   u.reg_write(UC_X86_REG_ESP,SP);u.reg_write(UC_X86_REG_EFLAGS,flags);actualflags=u.reg_read(UC_X86_REG_EFLAGS)
   model=dict(initial)
   if role=='begin':model[UC_X86_REG_EAX]=(eax&0xffffff00)|1
   u.emu_start(0x100000,STOP,count=4)
   assert u.reg_read(UC_X86_REG_EIP)==STOP and u.reg_read(UC_X86_REG_ESP)==SP+(12 if role=='begin' else 8)
   assert all(u.reg_read(r)==v for r,v in model.items()) and u.reg_read(UC_X86_REG_EFLAGS)==actualflags and bytes(u.mem_read(ARENA,SIZE))==arena
   count+=1
  receipts.append(dict(role=role,cases=count,full_arena_bytes=SIZE,explicit_arg_bytes=8 if role=='begin' else 4,all_registers_modelled=True,flags_unchanged=True))
 return dict(mode=mode,runs=receipts)
