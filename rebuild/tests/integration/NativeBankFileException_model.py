"""Original/rebuilt constructor state, argument lifetime and ABI checks."""
from pathlib import Path
import sys,struct,itertools
R=Path(__file__).resolve().parents[3];sys.path.insert(0,str(R/'tools/decomp_pipeline'))
from unicorn import Uc,UC_ARCH_X86,UC_MODE_32,UC_HOOK_CODE
from unicorn.x86_const import *
from gate_game_component_abi import Coff
from pe_oracle import EXE,read_virtual_data
ENTRY=0x9d1e70;A=0x2000000;SELF=A+0x100;STACK=0x3000000;STOP=0x4000000
SYMBOL='??0CBankFileException@@QAE@VCCharString@@W4EBankFileExceptionType@0@@Z'
TARGETS={'??BCCharString@@QBEPBDXZ':0x99e4c0,'??1CCharString@@QAE@XZ':0x99eae0}

def execute(code,args):
 value,storage,mutation,seed=args;u=Uc(UC_ARCH_X86,UC_MODE_32)
 for p,n in ((0x9d1000,4096),(0x99e000,4096),(A,4096),(STACK,0x10000),(STOP,4096)):u.mem_map(p,n)
 u.mem_write(ENTRY,code);u.mem_write(A,bytes((i*37+seed)&255 for i in range(4096)));expected=bytearray(u.mem_read(A,4096));sp=STACK+0x8000;trace=[]
 def put(a,v):u.mem_write(a,struct.pack('<I',v&0xffffffff))
 def get(a):return struct.unpack('<I',u.mem_read(a,4))[0]
 def change(a,v):put(a,v);struct.pack_into('<I',expected,a-A,v&0xffffffff)
 put(sp,STOP);put(sp+4,storage);put(sp+8,value);initial=get(SELF)
 def hook(uc,a,size,_):
  if a not in TARGETS.values():return
  esp=uc.reg_read(UC_X86_REG_ESP);cx=uc.reg_read(UC_X86_REG_ECX);assert cx==sp+4
  if a==0x99e4c0:
   assert not trace and get(SELF)==initial and get(cx)==storage;trace.append('convert')
   if mutation==1:put(sp+8,0xdeadbeef)
   if mutation==2:change(SELF,0x55667788)
   struct.pack_into('<I',expected,SELF-A,get(sp+8))
  else:
   assert trace==['convert'] and get(SELF)==get(sp+8);assert bytes(u.mem_read(A,4096))==expected;trace.append('destroy')
   if mutation==3:change(SELF,0x87654321)
   put(cx,0)
  uc.reg_write(UC_X86_REG_EAX,0x12345678);uc.reg_write(UC_X86_REG_ECX,0xabadcafe);uc.reg_write(UC_X86_REG_EDX,0x55667788);uc.reg_write(UC_X86_REG_EIP,get(esp));uc.reg_write(UC_X86_REG_ESP,esp+4)
 u.hook_add(UC_HOOK_CODE,hook);saved=(UC_X86_REG_EBX,UC_X86_REG_ESI,UC_X86_REG_EDI,UC_X86_REG_EBP)
 for i,reg in enumerate(saved):u.reg_write(reg,0x77665500+i)
 u.reg_write(UC_X86_REG_ECX,SELF);u.reg_write(UC_X86_REG_ESP,sp);u.emu_start(ENTRY,STOP,count=50)
 assert u.reg_read(UC_X86_REG_EIP)==STOP and u.reg_read(UC_X86_REG_ESP)==sp+12 and u.reg_read(UC_X86_REG_EAX)==SELF
 assert all(u.reg_read(reg)==0x77665500+i for i,reg in enumerate(saved));assert bytes(u.mem_read(A,4096))==expected and trace==['convert','destroy'] and get(sp+4)==0
 return bytes(expected),get(sp+8),trace

def compare(obj):
 c=Coff(obj);s,o=c.symbols[SYMBOL];body,rels=c.sections[s];assert not o and len(body)==33 and len(rels)==2;patched=bytearray(body)
 for site,(name,kind) in rels.items():struct.pack_into('<I',patched,site,(TARGETS[name]+struct.unpack_from('<i',body,site)[0]-(ENTRY+site+4 if kind==20 else 0))&0xffffffff)
 original=read_virtual_data(EXE.read_bytes(),ENTRY,33);assert bytes(patched)==original;cases=0
 for args in itertools.product((0,1,2,3,0xffffffff),(0,0x11223344,0xffffffff),range(4),(0,77)):
  assert execute(bytes(patched),args)==execute(original,args);cases+=1
 controls=[]
 for name,address,replacement,args in [('skip-conversion',0x9d1e77,b'\x90'*5,(1,0x11223344,1,77)),('skip-destruction',0x9d1e86,b'\x90'*5,(3,0xffffffff,3,77)),('wrong-type-offset',0x9d1e7f,b'\x08',(2,0x11223344,0,77))]:
  changed=bytearray(patched);offset=address-ENTRY;changed[offset:offset+len(replacement)]=replacement
  try:execute(bytes(changed),args)
  except AssertionError:controls.append(name)
  else:raise AssertionError('Undetected '+name)
 return dict(status='PASS',grade='MATCH',bytes=33,cases=cases,controls=controls,relocations=[dict(site=k,symbol=v[0],kind=v[1],address=TARGETS[v[0]]) for k,v in rels.items()],boundary='Full4KiB object surroundings, by-value argument cleanup, helper mutations and nonvolatile/stack/return ABI. String conversion/destruction controlled; no actual exception throw/unwind claim.')
