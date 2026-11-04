"""Execute the recovered initialization chain with only D3D calls mocked."""
import struct
from unicorn import Uc,UC_ARCH_X86,UC_MODE_32,UC_HOOK_CODE
from unicorn.x86_const import *
ENTRY=0xa07900
OBJECT=0x2000000
DEVICE=OBJECT+0x4000
TABLE=OBJECT+0x4100
STACK=0x3000000
CALLBACK=0x2200000
STOP=CALLBACK+0x100
GLOBAL=0x13cb508

def execute(bodies,data,caps_choice,flags,old_count,mutation,variant):
 u=Uc(UC_ARCH_X86,UC_MODE_32)
 for a,n in [(0xa04000,0x4000),(0x1396000,4096),(0x13cb000,4096),(OBJECT,0x5000),(STACK,0x10000),(CALLBACK,4096)]:u.mem_map(a,n)
 def put(a,v):u.mem_write(a,struct.pack('<I',v&0xffffffff))
 def get(a):return int.from_bytes(u.mem_read(a,4),'little')
 for a,b in bodies.items():u.mem_write(a,bytes(b))
 u.mem_write(0x1396000,data);u.mem_write(OBJECT,b'\xa5'*0x3a3c)
 if variant:
  for a in range(0x1396da8,0x1397000,4):put(a,0x80000000+a-0x1396000)
 put(OBJECT+0x2810,old_count);put(OBJECT+0x3a38,flags)
 for i in range(231):
  state=OBJECT+0x2818+i*20;u.mem_write(state+16,b'\0');put(OBJECT+0x2408+i*4,state)
 put(DEVICE,TABLE);put(DEVICE+4,TABLE)
 for offset,target in [(0x1c,CALLBACK),(0xe4,CALLBACK+16),(0x10c,CALLBACK+32),(0x114,CALLBACK+48)]:put(TABLE+offset,target)
 events=[]
 def hook(u,a,size,data):
  if a not in [CALLBACK,CALLBACK+16,CALLBACK+32,CALLBACK+48]:return
  sp=u.reg_read(UC_X86_REG_ESP);argc=2 if a==CALLBACK else 3 if a==CALLBACK+16 else 4;args=[get(sp+4+i*4) for i in range(argc)]
  if a==CALLBACK:
   assert args[0]==DEVICE and len(events)==0
   u.mem_write(args[1],b'\0'*304);caps=((caps_choice&1)!=0)*0x4000000+((caps_choice&2)!=0)*0x2000000+((caps_choice&4)!=0)*0x2000;put(args[1]+0x24,caps)
   if mutation&1:put(OBJECT+0x3a38,get(OBJECT+0x3a38)^2)
   if mutation&2:put(OBJECT+4,DEVICE+4)
   events.append(('caps',args[0],caps))
  else:
   assert args[0]==(DEVICE+4 if mutation&2 else DEVICE);events.append((a,args))
  u.reg_write(UC_X86_REG_EAX,0x80004005);u.reg_write(UC_X86_REG_ECX,0xdead0001);u.reg_write(UC_X86_REG_EDX,0xdead0002);u.reg_write(UC_X86_REG_EIP,get(sp));u.reg_write(UC_X86_REG_ESP,sp+4+argc*4)
 u.hook_add(UC_HOOK_CODE,hook,begin=CALLBACK,end=CALLBACK+48)
 sp=STACK+0x8000;put(sp,STOP);put(sp+4,DEVICE);u.reg_write(UC_X86_REG_ESP,sp);u.reg_write(UC_X86_REG_ECX,OBJECT)
 preserved=[UC_X86_REG_EBX,UC_X86_REG_EBP,UC_X86_REG_ESI,UC_X86_REG_EDI]
 for i,r in enumerate(preserved):u.reg_write(r,0xabcd0000+i)
 u.emu_start(ENTRY,STOP,count=100000)
 assert u.reg_read(UC_X86_REG_EIP)==STOP and u.reg_read(UC_X86_REG_ESP)==sp+8 and get(GLOBAL)==OBJECT
 for i,r in enumerate(preserved):assert u.reg_read(r)==0xabcd0000+i
 assert get(OBJECT+0x280c)==0 and get(OBJECT+0x2814)==1
 return events,bytes(u.mem_read(OBJECT,0x3a3c))
