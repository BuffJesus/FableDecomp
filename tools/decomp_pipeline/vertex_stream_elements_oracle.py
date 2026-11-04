"""Compare nested stream-element copy construction and complete assignment chains."""
from vertex_declaration_assign_oracle import *
from vertex_declaration_assign_oracle import execute as execute_assignment

def execute_constructor(codes,n,fill,sentinel):
 u=Uc(UC_ARCH_X86,UC_MODE_32)
 for a,size in [(0xa50000,0x10000),(0xbfe000,0x1000),(OBJECT,0x10000),(STOP,0x1000),(STACK,0x10000)]:u.mem_map(a,size)
 def put(a,v):u.mem_write(a,struct.pack('<I',v&0xffffffff))
 def get(a):return int.from_bytes(u.mem_read(a,4),'little')
 for a,c in codes.items():u.mem_write(a,bytes(c))
 u.mem_write(OBJECT,bytes([0xa5 if sentinel else 0])*12);cap=n+sentinel*2;src=SRC if cap else 0
 put(SOURCE,src);put(SOURCE+4,src+n*8);put(SOURCE+8,src+cap*8)
 sourcebytes=bytes((0x33+fill+i*17)&255 for i in range(n*8));u.mem_write(SRC,sourcebytes);u.mem_write(GROW,b'\xa5'*80);events=[]
 def hook(u,a,size,data):
  if a!=0xbfea0e:return
  sp=u.reg_read(UC_X86_REG_ESP);events.append(get(sp+4));u.reg_write(UC_X86_REG_EAX,GROW);u.reg_write(UC_X86_REG_ESP,sp+4);u.reg_write(UC_X86_REG_EIP,get(sp))
 u.hook_add(UC_HOOK_CODE,hook);sp=STACK+0x8000;put(sp,STOP);put(sp+4,SOURCE);u.reg_write(UC_X86_REG_ESP,sp);u.reg_write(UC_X86_REG_ECX,OBJECT)
 regs=[UC_X86_REG_EBX,UC_X86_REG_ESI,UC_X86_REG_EDI,UC_X86_REG_EBP]
 for i,r in enumerate(regs):u.reg_write(r,0x554400+i)
 u.emu_start(0xa5e0a0,STOP,count=10000)
 assert u.reg_read(UC_X86_REG_EIP)==STOP and u.reg_read(UC_X86_REG_ESP)==sp+8 and u.reg_read(UC_X86_REG_EAX)==OBJECT
 assert all(u.reg_read(r)==0x554400+i for i,r in enumerate(regs))
 begin=GROW if n else 0;assert (get(OBJECT),get(OBJECT+4),get(OBJECT+8))==(begin,begin+n*8,begin+n*8)
 assert events==([n*8] if n else [])
 if n:assert bytes(u.mem_read(GROW,n*8))==sourcebytes
 assert bytes(u.mem_read(SRC,n*8))==sourcebytes
 return events,bytes(u.mem_read(OBJECT,12)),bytes(u.mem_read(SOURCE,12)),bytes(u.mem_read(GROW,80))

def compare(candidate,native):
 cases=0
 for self in range(2):
  for used in range(5):
   for cap in range(used,7):
    for n in range(8):
     for fill in range(3):
      assert execute_assignment(candidate,self,used,cap,n,fill,0xa5e4d0)==execute_assignment(native,self,used,cap,n,fill,0xa5e4d0)
      cases+=1
 for n in range(9):
  for fill in range(3):
   for sentinel in range(2):
    assert execute_constructor(candidate,n,fill,sentinel)==execute_constructor(native,n,fill,sentinel)
    cases+=1
 return cases
