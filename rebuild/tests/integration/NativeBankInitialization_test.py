"""Original/rebuilt bank initialization with allocation-time full-arena observations."""
import struct,itertools
from unicorn import Uc,UC_ARCH_X86,UC_MODE_32,UC_HOOK_CODE
from unicorn.x86_const import *
ARENA=0x2000000;STACK=0x3000000;STOP=0x2200000;INIT=0x2200100

def run(code,entry,fill,seed,displacement):
 u=Uc(UC_ARCH_X86,UC_MODE_32)
 for a,size in [(0x990000,0x270000),(0x1390000,0x30000),(0x143f000,0x1000),(ARENA,0x10000),(STACK,0x10000),(STOP,0x1000)]:u.mem_map(a,size)
 for a,b in code.items():u.mem_write(a,b)
 u.mem_write(ARENA,bytes([fill])*0x10000)
 def put(a,v):u.mem_write(a,struct.pack('<I',v&0xffffffff))
 def get(a):return struct.unpack('<I',u.mem_read(a,4))[0]
 put(0x143fee4,INIT);put(0x13bca20,seed^0xffffffff);put(0x13bd800,seed)
 obj=ARENA+displacement;sp=STACK+0x8000;put(sp,STOP)
 u.reg_write(UC_X86_REG_ESP,sp);u.reg_write(UC_X86_REG_ECX,obj)
 preserved=[UC_X86_REG_EBX,UC_X86_REG_ESI,UC_X86_REG_EDI,UC_X86_REG_EBP]
 for i,r in enumerate(preserved):u.reg_write(r,0x11223300+i)
 alloc=[];events=[];cursor=ARENA+0x1000
 def hook(machine,a,size,user):
  nonlocal cursor
  if a==STOP:machine.emu_stop();return
  if a not in (0xbfea0e,INIT):return
  esp=machine.reg_read(UC_X86_REG_ESP);ret=get(esp);arg=get(esp+4)
  if a==0xbfea0e:
   assert arg in (16,20,24,52);ptr=cursor+16;cursor=((ptr+arg+31)//16)*16
   alloc.append((ptr,arg));events.append(('malloc',arg,bytes(machine.mem_read(ARENA,0x10000))));machine.reg_write(UC_X86_REG_EAX,ptr)
   machine.reg_write(UC_X86_REG_ESP,esp+4)
  else:
   assert arg==obj+0x14c;events.append(('InitializeCriticalSection',arg,bytes(machine.mem_read(ARENA,0x10000))));machine.mem_write(arg,bytes(range(24)));machine.reg_write(UC_X86_REG_ESP,esp+8)
  machine.reg_write(UC_X86_REG_EIP,ret)
 u.hook_add(UC_HOOK_CODE,hook);u.emu_start(entry,STOP+1,count=5000)
 assert u.reg_read(UC_X86_REG_EIP)==STOP and u.reg_read(UC_X86_REG_ESP)==sp+4 and u.reg_read(UC_X86_REG_EAX)==obj
 assert [u.reg_read(r) for r in preserved]==[0x11223300+i for i in range(4)]
 expected_sizes=[24,24,24,52]+([20,20,16] if entry==0x9d5f80 else [])
 assert [n for p,n in alloc]==expected_sizes
 assert get(obj)==(0x129b8d4 if entry==0x9d5f80 else 0x129b54c)
 assert get(obj+0x98)==1 and get(obj+0x9c)==0x73 and get(obj+0x108)==0x10000
 for at,(ptr,n) in zip([0xac,0xb8,0xc4,0xfc,0x128,0x134,0x144],alloc):
  assert get(obj+at)==ptr
  if n==16:assert get(ptr)==ptr and get(ptr+4)==ptr
  else:assert u.mem_read(ptr,1)==b'\0' and get(ptr+4)==0 and get(ptr+8)==ptr and get(ptr+12)==ptr and get(obj+at+4)==0
  assert u.mem_read(ptr-16,16)==bytes([fill])*16 and u.mem_read(ptr+n,16)==bytes([fill])*16
 assert get(0x13bd800)==(seed+2)&0xffffffff and get(0x13bca20)==((seed^0xffffffff)+2)&0xffffffff
 if entry==0x9d5f80:assert get(obj+0x140)==0 and u.mem_read(obj+0x148,2)==b'\0\0' and events[-1][:2]==('InitializeCriticalSection',obj+0x14c)
 return bytes(u.mem_read(ARENA,0x10000)),bytes(u.mem_read(0x13b0000,0x10000)),events

def compare(candidate,native):
 cases=0
 for entry,fill,seed,disp in itertools.product((0x9cd480,0x9d5f80),(0,0x95,0xcd,0xff),(0,7,0xffffffff),(0x100,0x104,0x140)):
  expected=run(native,entry,fill,seed,disp);actual=run(candidate,entry,fill,seed,disp)
  assert expected==actual,(hex(entry),fill,seed,disp,'initialization snapshot or event mismatch')
  cases+=1
 return cases
