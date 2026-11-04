"""Retail class-local engine-definition lookup and reference ownership."""
import itertools, struct
from unicorn import Uc, UC_ARCH_X86, UC_MODE_32, UC_HOOK_CODE
from unicorn.x86_const import *
from pe_oracle import EXE, pe_sections
BASE=0x24000000
SYMBOL='??$GetDef@VCEngineDef@@@CDefinitionManager@@QBE_NABVCCharString@@JPAV?$CDefPointer@$$CBVCEngineDef@@@@@Z'
LOOKUP='?GetPDefFromClassIndex@CDefinitionManager@@QBE?AV?$CDefPointer@$$CBVCDefClassBase@@@@ABVCCharString@@J@Z'

def collect():
 u=Uc(UC_ARCH_X86,UC_MODE_32);u.mem_map(0x400000,0x1200000);u.mem_map(BASE,0x10000)
 raw=EXE.read_bytes()
 for _,va,_,off,n in pe_sections(raw):
  if n:u.mem_write(0x400000+va,raw[off:off+n])
 def get(a):return struct.unpack('<I',u.mem_read(a,4))[0]
 def put(a,v):u.mem_write(a,struct.pack('<I',v&0xffffffff))
 pointers=(0,BASE+0x100,BASE+0x200,BASE+0x300)
 put(BASE+0x1004,BASE+0x1100);u.mem_write(BASE+0x1100,b'\xc3')
 inputs=bytearray();expected=bytearray();cases=snapshots=0;temporary=0
 def event(kind,self=0,arg=0):
  nonlocal snapshots
  expected.extend(struct.pack('<4I',kind,self,arg,get(temporary) if temporary else 0))
  expected.extend(u.mem_read(BASE,2048));snapshots+=1
 def hook(u,a,n,_):
  nonlocal temporary
  if a not in (0x9ade10,BASE+0x1100):return
  sp=u.reg_read(UC_X86_REG_ESP);self=u.reg_read(UC_X86_REG_ECX)
  if a==0x9ade10:
   assert self==BASE+0x400 and get(sp+8)==BASE+0x40 and get(sp+12)==index
   temporary=get(sp+4);event(1,self,index)
   selected=pointers[new]
   if selected:put(selected+4,get(selected+4)+1)
   put(temporary,selected);result=temporary;cleanup=12
  else:
   assert self in pointers[1:];event(2,self)
   if mode==1:put(temporary,0)
   elif mode==2:put(temporary,pointers[3])
   elif mode==3:put(BASE,pointers[3]);put(pointers[3]+4,2)
   result=0xbad0bad0;cleanup=0
  u.reg_write(UC_X86_REG_ESP,sp+4+cleanup);u.reg_write(UC_X86_REG_EIP,get(sp))
  u.reg_write(UC_X86_REG_EAX,result);u.reg_write(UC_X86_REG_ECX,0xbad1bad1);u.reg_write(UC_X86_REG_EDX,0xbad2bad2)
 u.hook_add(UC_HOOK_CODE,hook)
 scenarios=[]
 for counts in itertools.product((0,1,2,0xffffffff),repeat=3):
  for old,new,mode,index in itertools.product(range(4),range(4),range(4),(1,2,0x7fffffff)):
   scenarios.append((0x5a,counts,old,new,mode,index,BASE))
 for fill,old,new in itertools.product(range(256),range(4),range(4)):
  scenarios.append((fill,(1,1,1),old,new,fill%4,1,BASE))
 for fill,index,out in itertools.product(range(256),(0,0xffffffff,0x80000000),(0,BASE)):
  scenarios.append((fill,(1,2,0xffffffff),fill%4,1,0,index,out))
 for fill in range(256):scenarios.append((fill,(0,1,2),1,0,0,1,0))
 for fill,counts,old,new,mode,index,out in scenarios:
  arena=bytearray([fill])*2048;struct.pack_into('<I',arena,0,pointers[old])
  for ptr,count in zip(pointers[1:],counts):struct.pack_into('<2I',arena,ptr-BASE,BASE+0x1000,count)
  inputs.extend(struct.pack('<4I',new,mode,index,out));inputs.extend(arena);u.mem_write(BASE,bytes(arena));temporary=0
  sp=BASE+0xe000;stop=BASE+0xf000;u.mem_write(sp,struct.pack('<4I',stop,BASE+0x40,index,out))
  u.reg_write(UC_X86_REG_ESP,sp);u.reg_write(UC_X86_REG_ECX,BASE+0x400)
  regs=(UC_X86_REG_EBX,UC_X86_REG_ESI,UC_X86_REG_EDI,UC_X86_REG_EBP)
  for i,r in enumerate(regs):u.reg_write(r,0x123400+i)
  u.emu_start(0x430096,stop,count=200)
  assert u.reg_read(UC_X86_REG_ESP)==sp+16 and all(u.reg_read(r)==0x123400+i for i,r in enumerate(regs))
  result=u.reg_read(UC_X86_REG_EAX)&255;assert result==int(0<index<0x80000000 and new!=0)
  temporary=0;event(3,0,result);cases+=1
 return inputs,expected,dict(cases=cases,snapshots=snapshots)
