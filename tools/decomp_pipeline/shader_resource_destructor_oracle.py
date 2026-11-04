"""Execute the complete shader-resource cleanup chain against native code."""
import struct
from unicorn import Uc,UC_ARCH_X86,UC_MODE_32,UC_HOOK_CODE
from unicorn.x86_const import *
DTOR=0xa5d800;SCALAR=0xa5ebe0
OBJECT=0x2000000;HANDLES=OBJECT+0x1000;STREAMS=OBJECT+0x2000;REPLACEMENT=OBJECT+0x3000;ELEMENTS=OBJECT+0x4000;DECL=OBJECT+0x5000;SHADERS=OBJECT+0x6000;VTABLE=OBJECT+0x7000
DELETE_SHADER=0x2100000;STOP=0x2200000;STACK=0x3000000

def execute(codes,entry,mask,count,elements,declaration,mode,flags):
 u=Uc(UC_ARCH_X86,UC_MODE_32)
 for a,n in [(0x990000,0x300000),(OBJECT,0x10000),(DELETE_SHADER,0x1000),(STOP,0x1000),(STACK,0x10000)]:u.mem_map(a,n)
 def put(a,v):u.mem_write(a,struct.pack('<I',v))
 def get(a):return int.from_bytes(u.mem_read(a,4),'little')
 for a,code in codes.items():u.mem_write(a,bytes(code))
 u.mem_write(OBJECT,b'\x55'*60);u.mem_write(STREAMS,b'\x6a'*80);u.mem_write(REPLACEMENT,b'\0'*16)
 put(OBJECT+8,HANDLES);put(OBJECT+12,HANDLES+16);put(OBJECT+16,HANDLES+16)
 put(OBJECT+28,STREAMS);put(OBJECT+32,STREAMS+count*20);put(OBJECT+36,STREAMS+80);put(OBJECT+40,DECL if declaration else 0)
 put(VTABLE,DELETE_SHADER)
 for i in range(4):put(HANDLES+i*4,SHADERS+i*16 if mask&(1<<i) else 0);put(SHADERS+i*16,VTABLE);put(STREAMS+i*20+8,ELEMENTS+i*16 if elements else 0)
 events=[];shader_count=0;stream_count=0
 def hook(u,a,size,data):
  nonlocal shader_count,stream_count
  if a not in (DELETE_SHADER,0xbfea14,0x99a2e0,0x99a300,0xbfe9bc):return
  sp=u.reg_read(UC_X86_REG_ESP);this=u.reg_read(UC_X86_REG_ECX);popped=4
  if a==DELETE_SHADER:
   assert get(sp+4)==1 and get(OBJECT)==0x129d580;popped=8;index=(this-SHADERS)//16;assert 0<=index<4
   events.append(('shader',index));shader_count+=1
   if shader_count==1 and mode==1:put(OBJECT+8,REPLACEMENT);put(OBJECT+12,REPLACEMENT)
  elif a==0xbfea14:events.append(('free',get(sp+4)))
  elif a==0xbfe9bc:assert get(sp+4)==OBJECT;events.append(('delete',OBJECT))
  elif a==0x99a300:assert this==OBJECT;events.append(('owner_base',));put(this,0x1231710)
  else:
   assert get(OBJECT+4)==0 and get(OBJECT+20)==0
   if this==OBJECT+24:
    assert stream_count==count;events.append(('input_base',))
    if mode==3:put(OBJECT+8,0)
   else:
    assert this==STREAMS+stream_count*20+4;events.append(('stream_base',stream_count));stream_count+=1
    if stream_count==1 and mode==2:put(OBJECT+8,REPLACEMENT)
   put(this,0x1231710)
  u.reg_write(UC_X86_REG_ESP,sp+popped);u.reg_write(UC_X86_REG_EIP,get(sp))
 u.hook_add(UC_HOOK_CODE,hook);sp=STACK+0x8000;put(sp,STOP);u.reg_write(UC_X86_REG_ESP,sp);u.reg_write(UC_X86_REG_ECX,OBJECT)
 if entry==SCALAR:put(sp+4,flags)
 regs=[UC_X86_REG_EBX,UC_X86_REG_ESI,UC_X86_REG_EDI,UC_X86_REG_EBP]
 for i,r in enumerate(regs):u.reg_write(r,0x554400+i)
 u.emu_start(entry,STOP,count=3000)
 assert u.reg_read(UC_X86_REG_EIP)==STOP and u.reg_read(UC_X86_REG_ESP)==sp+(8 if entry==SCALAR else 4)
 assert all(u.reg_read(r)==0x554400+i for i,r in enumerate(regs))
 shaders=[i for i in range(4) if mask&(1<<i)]
 if mode==1:shaders=shaders[:1]
 expected=[('shader',i) for i in shaders]
 if declaration:expected.append(('free',DECL))
 for i in range(count):
  if elements:expected.append(('free',ELEMENTS+i*16))
  expected.append(('stream_base',i))
 expected.extend([('free',STREAMS),('input_base',)])
 if mode!=3:expected.append(('free',REPLACEMENT if (mode==1 and shaders) or (mode==2 and count) else HANDLES))
 expected.append(('owner_base',))
 if entry==SCALAR:
  assert u.reg_read(UC_X86_REG_EAX)==OBJECT
  if flags&1:expected.append(('delete',OBJECT))
 assert events==expected,(events,expected)
 return events,bytes(u.mem_read(OBJECT,60)),bytes(u.mem_read(HANDLES,16)),bytes(u.mem_read(REPLACEMENT,16)),bytes(u.mem_read(STREAMS,80))

def compare(candidate,native):
 cases=0
 for entry in [DTOR,SCALAR]:
  for mask in [0,1,5,15]:
   for count in [0,1,3]:
    for elements in [0,1]:
     for declaration in [0,1]:
      for mode in range(4):
       for flags in ([0,1,2,3,0xffffffff] if entry==SCALAR else [0]):
        assert execute(candidate,entry,mask,count,elements,declaration,mode,flags)==execute(native,entry,mask,count,elements,declaration,mode,flags)
        cases+=1
 return cases
