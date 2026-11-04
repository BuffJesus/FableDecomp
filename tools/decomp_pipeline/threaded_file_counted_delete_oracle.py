"""Typed counted-file deletion: null guard, virtual slot/flag and fastcall ABI."""
import itertools,struct
from unicorn import Uc,UC_ARCH_X86,UC_MODE_32,UC_HOOK_CODE
from unicorn.x86_const import *
ENTRY,ARENA,STACK,STOP,API=0x9a9040,0x2000000,0x3000000,0x4000000,0x5000000

def execute(body,args):
 present,alignment,slot,seed,mutation=args;obj=ARENA+64+alignment;table=ARENA+256
 u=Uc(UC_ARCH_X86,UC_MODE_32)
 for address,size in ((0x9a9000,0x1000),(ARENA,0x1000),(STACK,0x10000),(STOP,0x1000),(API,0x1000)):u.mem_map(address,size)
 u.mem_write(ENTRY,body)
 def put(a,value):u.mem_write(a,struct.pack('<I',value&0xffffffff))
 def get(a):return struct.unpack('<I',u.mem_read(a,4))[0]
 u.mem_write(ARENA,bytes((i*37+seed*19)&255 for i in range(0x1000)))
 put(obj,table);put(table,API+slot*16);expected=bytearray(u.mem_read(ARENA,0x1000));trace=[]
 if present:
  struct.pack_into('<I',expected,obj-ARENA+8,0x12340000+seed)
  if mutation&1:struct.pack_into('<I',expected,obj-ARENA,0xdeadbeef)
  if mutation&2:struct.pack_into('<I',expected,table-ARENA,API+(1-slot)*16)
 def hook(uc,address,size,_):
  if address not in (API,API+16):return
  sp=uc.reg_read(UC_X86_REG_ESP)
  assert present and not trace and address==API+slot*16
  assert uc.reg_read(UC_X86_REG_ECX)==obj and get(sp+4)==1
  trace.append(('virtual_delete',slot,obj,1));put(obj+8,0x12340000+seed)
  if mutation&1:put(obj,0xdeadbeef)
  if mutation&2:put(table,API+(1-slot)*16)
  for reg,value in ((UC_X86_REG_EAX,0x87654321),(UC_X86_REG_ECX,0xdeadbeef),(UC_X86_REG_EDX,0xaabbccdd)):uc.reg_write(reg,value)
  uc.reg_write(UC_X86_REG_EIP,get(sp));uc.reg_write(UC_X86_REG_ESP,sp+8)
 u.hook_add(UC_HOOK_CODE,hook)
 sp=STACK+0x8000;put(sp,STOP);saved=(UC_X86_REG_EBX,UC_X86_REG_ESI,UC_X86_REG_EDI,UC_X86_REG_EBP)
 for i,reg in enumerate(saved):u.reg_write(reg,0x55660000+seed+i)
 u.reg_write(UC_X86_REG_ESP,sp);u.reg_write(UC_X86_REG_ECX,obj if present else 0)
 u.emu_start(ENTRY,STOP,count=100)
 assert u.reg_read(UC_X86_REG_EIP)==STOP and u.reg_read(UC_X86_REG_ESP)==sp+4
 assert all(u.reg_read(reg)==0x55660000+seed+i for i,reg in enumerate(saved))
 assert bytes(u.mem_read(ARENA,0x1000))==bytes(expected)
 assert trace==([('virtual_delete',slot,obj,1)] if present else [])
 return bytes(expected),trace

def compare_deletion(original,built):
 cases=0
 for args in itertools.product((False,True),range(4),range(2),range(4),range(4)):
  assert execute(original,args)==execute(built,args);cases+=1
 detected=False
 try:execute(b'\xc3',(True,1,1,3,3))
 except AssertionError:detected=True
 assert detected
 return dict(status='PASS',cases=cases,controls=1,boundary='Independent full4KiB prediction and ABI; virtual deleting callback controlled, including object/table mutation and caller-saved register poisoning.')
