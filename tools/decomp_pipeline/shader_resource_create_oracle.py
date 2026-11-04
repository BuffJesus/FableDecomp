"""Compare the factory and its STLport growth chain, including native null faults."""
import struct
from unicorn import Uc,UcError,UC_ARCH_X86,UC_MODE_32,UC_HOOK_CODE,UC_ERR_WRITE_UNMAPPED
from unicorn.x86_const import *
ENTRY=0x98b860;INIT=0xa5d4d0;CTOR=0xa5d870
OBJECT=0x2000000;RESOURCE=OBJECT+0x1000;INFO=OBJECT+0x2000;RESULT=OBJECT+0x3000;INITIAL=OBJECT+0x4000;GROW=OBJECT+0x6000;VTABLE=OBJECT+0x7000;DESTRUCTOR=0x2100000;STOP=0x2200000;STACK=0x3000000

def execute(codes,allocation,used,spare,mode,fill):
 u=Uc(UC_ARCH_X86,UC_MODE_32)
 for a,n in [(0x980000,0x10000),(0xa50000,0x10000),(0xbfe000,0x1000),(OBJECT,0x10000),(DESTRUCTOR,0x1000),(STOP,0x1000),(STACK,0x10000)]:u.mem_map(a,n)
 def put(a,v):u.mem_write(a,struct.pack('<I',v&0xffffffff))
 def get(a):return int.from_bytes(u.mem_read(a,4),'little')
 for a,code in codes.items():u.mem_write(a,bytes(code))
 u.mem_write(OBJECT,bytes([fill])*0x3dc);u.mem_write(RESOURCE,b'\x66'*60);u.mem_write(RESULT,b'\x77'*8)
 cap=used+spare;begin=INITIAL if cap else 0
 put(OBJECT+0x3a8,begin);put(OBJECT+0x3ac,begin+used*8);put(OBJECT+0x3b0,begin+cap*8);put(VTABLE,DESTRUCTOR)
 events=[];allocations=0
 def hook(u,a,size,data):
  nonlocal allocations
  if a==INIT:events.append(('init',u.reg_read(UC_X86_REG_ECX),get(u.reg_read(UC_X86_REG_ESP)+4)));return
  if a==INIT+7:
   if mode==1:put(OBJECT+0x3b0,get(OBJECT+0x3ac))
   if allocation!=2 and mode in (2,3):put(INFO,0 if mode==2 else 0xffffffff)
   return
  if a not in (0xbfea1a,CTOR,0xbfea0e,0xbfea14,0xbfe9bc,DESTRUCTOR):return
  sp=u.reg_read(UC_X86_REG_ESP);this=u.reg_read(UC_X86_REG_ECX);popped=4;value=0
  if a==0xbfea1a:
   allocations+=1;n=get(sp+4);assert n==(60 if allocations==1 else 12);events.append(('new',n));value=0 if allocation==allocations else RESOURCE if allocations==1 else INFO
  elif a==CTOR:
   assert this==RESOURCE;events.append(('construct',));u.mem_write(RESOURCE,b'\0'*60);put(RESOURCE,VTABLE);value=RESOURCE
  elif a==0xbfea0e:events.append(('malloc',get(sp+4)));value=GROW
  elif a==0xbfea14:events.append(('free',get(sp+4)))
  elif a==0xbfe9bc:events.append(('delete_info',get(sp+4)))
  else:assert this==RESOURCE and get(sp+4)==1;events.append(('delete_resource',));popped=8;value=RESOURCE
  u.reg_write(UC_X86_REG_EAX,value);u.reg_write(UC_X86_REG_ESP,sp+popped);u.reg_write(UC_X86_REG_EIP,get(sp))
 u.hook_add(UC_HOOK_CODE,hook);sp=STACK+0x8000;put(sp,STOP);put(sp+4,RESULT);u.reg_write(UC_X86_REG_ESP,sp);u.reg_write(UC_X86_REG_ECX,OBJECT)
 regs=[UC_X86_REG_EBX,UC_X86_REG_ESI,UC_X86_REG_EDI,UC_X86_REG_EBP]
 for i,r in enumerate(regs):u.reg_write(r,0x554400+i)
 fault=None
 try:u.emu_start(ENTRY,STOP,count=10000)
 except UcError as error:fault=error.errno
 if allocation==1:
  assert fault==UC_ERR_WRITE_UNMAPPED and events==[('new',60),('init',0,OBJECT)]
 else:
  assert fault is None and u.reg_read(UC_X86_REG_EIP)==STOP and u.reg_read(UC_X86_REG_ESP)==sp+8 and u.reg_read(UC_X86_REG_EAX)==RESULT
  assert all(u.reg_read(r)==0x554400+i for i,r in enumerate(regs))
  assert get(RESULT)==RESOURCE and get(RESULT+4)==(0 if allocation==2 else INFO)
  growing=not spare or mode==1;array=GROW if growing else INITIAL
  assert get(OBJECT+0x3a8)==array and get(OBJECT+0x3ac)==array+(used+1)*8
  assert get(array+used*8)==RESOURCE and get(array+used*8+4)==(0 if allocation==2 else INFO)
  expected=[('new',60),('construct',),('new',12),('init',RESOURCE,OBJECT)]
  if growing:
   expected.append(('malloc',(used+max(used,1))*8))
   if begin:expected.append(('free',begin))
  if allocation!=2:
   assert get(INFO)==(0 if mode==3 else 1 if mode==2 else 2)
   if mode==3:expected.extend([('delete_resource',),('delete_info',INFO)])
  assert events==expected,(events,expected)
 return fault,events,bytes(u.mem_read(OBJECT,0x3dc)),bytes(u.mem_read(RESOURCE,60)),bytes(u.mem_read(INFO,12)),bytes(u.mem_read(RESULT,8)),bytes(u.mem_read(INITIAL,32)),bytes(u.mem_read(GROW,32))

def compare(candidate,native):
 cases=0
 for allocation in range(3):
  for used in range(3):
   for spare in range(2):
    for mode in range(4):
     for fill in [0,0xa5]:
      assert execute(candidate,allocation,used,spare,mode,fill)==execute(native,allocation,used,spare,mode,fill)
      cases+=1
 return cases
