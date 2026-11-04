"""Complete symbol-map scalar destruction with real container and recursive tree cleanup."""
import struct
from unicorn import Uc,UC_ARCH_X86,UC_MODE_32,UC_HOOK_CODE
from unicorn.x86_const import *
OBJECT=0x2000000;HEADER=OBJECT+0x100;NODES=OBJECT+0x200;ENTRIES=OBJECT+0x1000;REFS=OBJECT+0x2000;STOP=0x2200000;STACK=0x3000000

def execute(codes,count,entries,shape,flags,alias):
 u=Uc(UC_ARCH_X86,UC_MODE_32)
 for a,n in [(0x4ab000,0x3000),(0x6fb000,0x2000),(0x990000,0x10000),(0x9e0000,0x10000),(0xbfe000,0x1000),(OBJECT,0x10000),(STOP,0x1000),(STACK,0x10000)]:u.mem_map(a,n)
 def put(a,v):u.mem_write(a,struct.pack('<I',v&0xffffffff))
 def get(a):return int.from_bytes(u.mem_read(a,4),'little')
 for a,b in codes.items():u.mem_write(a,bytes(b))
 u.mem_write(OBJECT,b'\xa5'*36);put(OBJECT,0x129c158);put(OBJECT+4,ENTRIES if entries else 0);put(OBJECT+8,ENTRIES+entries*16 if entries else 0);put(OBJECT+12,ENTRIES+(entries+3)*16 if entries else 0);put(OBJECT+20,HEADER);put(OBJECT+24,count)
 for i in range(8):put(REFS+4*i,1)
 def key(address,index):
  ref=0 if alias==0 else REFS if alias==1 else REFS+4*(index%8);put(address,ref)
  if ref:put(ref,get(ref)+1)
 def build(lo,hi,parent):
  if lo==hi:return 0
  i=(lo+hi)//2 if shape==0 else lo if shape==1 else hi-1;p=NODES+32*i;key(p+16,i)
  put(p+4,parent);put(p+8,build(lo,i,p));put(p+12,build(i+1,hi,p));return p
 root=build(0,count,HEADER);put(HEADER+4,root);put(HEADER+8,NODES if count else HEADER);put(HEADER+12,NODES+32*(count-1) if count else HEADER)
 expected=[]
 def walk(p):
  if not p:return
  walk(get(p+12));expected.extend([('destroy',p+16),('free',p)]);walk(get(p+8))
 walk(root);tree_events=expected;expected=[]
 for i in range(entries):
  for j in range(3):key(ENTRIES+i*16+j*4,i*3+j)
  put(ENTRIES+i*16+12,i);expected.extend(('destroy',ENTRIES+i*16+j) for j in [8,4,0])
 if entries:expected.append(('free',ENTRIES))
 expected+=tree_events+[('free',HEADER),('base',)]
 if flags&1:expected.append(('delete',OBJECT))
 events=[]
 def hook(u,a,size,data):
  if a not in (0x99eae0,0xbfea14,0x99a300,0xbfe9bc,0x99efb0):return
  assert a!=0x99efb0
  sp=u.reg_read(UC_X86_REG_ESP);ecx=u.reg_read(UC_X86_REG_ECX)
  if a==0x99eae0:
   events.append(('destroy',ecx));ref=get(ecx)
   if ref:put(ref,get(ref)-1)
   put(ecx,0)
  elif a==0x99a300:assert ecx==OBJECT and all(get(REFS+4*i)==1 for i in range(8));events.append(('base',))
  else:events.append(('free' if a==0xbfea14 else 'delete',get(sp+4)))
  u.reg_write(UC_X86_REG_ESP,sp+4);u.reg_write(UC_X86_REG_EIP,get(sp))
 u.hook_add(UC_HOOK_CODE,hook);sp=STACK+0x8000;put(sp,STOP);put(sp+4,flags);u.reg_write(UC_X86_REG_ESP,sp);u.reg_write(UC_X86_REG_ECX,OBJECT)
 regs=[UC_X86_REG_EBX,UC_X86_REG_ESI,UC_X86_REG_EDI,UC_X86_REG_EBP]
 for i,r in enumerate(regs):u.reg_write(r,0x887700+i)
 u.mem_write(OBJECT+17,bytes([1]));u.mem_write(OBJECT+16,bytes([0x7a]));u.mem_write(OBJECT+32,bytes([alias==1]))
 for repetition in range(2):
  put(sp,STOP);u.reg_write(UC_X86_REG_ESP,sp);u.reg_write(UC_X86_REG_ECX,OBJECT);u.emu_start(0x9e5250,STOP,count=5000)
  assert u.reg_read(UC_X86_REG_EIP)==STOP and u.reg_read(UC_X86_REG_ESP)==sp+4
  assert [get(OBJECT+i) for i in [4,8,12,24]]==[0]*4 and get(OBJECT+20)==HEADER
  assert [get(HEADER+i) for i in [4,8,12]]==[0,HEADER,HEADER]
  assert bytes(u.mem_read(OBJECT+16,2))==bytes([0x7a,0]) and u.mem_read(OBJECT+32,1)[0]==(alias==1)
 put(sp,STOP);put(sp+4,flags);u.reg_write(UC_X86_REG_ESP,sp);u.reg_write(UC_X86_REG_ECX,OBJECT);u.emu_start(0x9e7bd0,STOP,count=5000)
 assert u.reg_read(UC_X86_REG_EIP)==STOP and u.reg_read(UC_X86_REG_ESP)==sp+8 and u.reg_read(UC_X86_REG_EAX)==OBJECT
 assert all(u.reg_read(r)==0x887700+i for i,r in enumerate(regs)) and events==expected
 return events,bytes(u.mem_read(OBJECT,0x2100))

def compare(candidate,native):
 cases=0
 for count in range(9):
  for entries in range(9):
   for shape in range(3):
    for flags in range(4):
     for alias in range(3):
      assert execute(candidate,count,entries,shape,flags,alias)==execute(native,count,entries,shape,flags,alias);cases+=1
 return cases
