import struct
from unicorn import Uc,UC_ARCH_X86,UC_MODE_32,UC_HOOK_CODE
from unicorn.x86_const import *
ENTRY=0x9c00c0
COPY=0x9c11a0
RESET=0x9bfb30
COOPERATIVE=0x500000
OBJECT=0x2000000
PARAMETERS=OBJECT+0x268
DEVICE=OBJECT+0x1000
TABLE=OBJECT+0x1100
STACK=0x3000000
STOP=0x2200000

def execute(code,copy,width,height,sample,cooperative,results,mutation):
 u=Uc(UC_ARCH_X86,UC_MODE_32)
 for a,n in [(0x9bf000,0x3000),(COOPERATIVE,8192),(OBJECT,8192),(STACK,0x10000),(STOP,4096)]:u.mem_map(a,n)
 def put(a,v):u.mem_write(a,struct.pack('<I',v&0xffffffff))
 def get(a):return struct.unpack('<I',u.mem_read(a,4))[0]
 u.mem_write(ENTRY,bytes(code));u.mem_write(COPY,bytes(copy));put(OBJECT+0x58,DEVICE);put(DEVICE,TABLE);put(TABLE+12,COOPERATIVE)
 u.mem_write(PARAMETERS,b'\xa7'*44);put(PARAMETERS+4,width);put(PARAMETERS+8,height);put(PARAMETERS+40,sample)
 u.mem_write(PARAMETERS+28,b'\1');u.mem_write(PARAMETERS+32,b'\0\1')
 events=[];returned=False
 def hook(u,a,size,data):
  nonlocal returned
  if a==STOP:returned=True;u.emu_stop();return
  if a not in [RESET,COOPERATIVE]:return
  sp=u.reg_read(UC_X86_REG_ESP)
  if a==COOPERATIVE:assert get(sp+4)==DEVICE;result=cooperative
  else:
   assert u.reg_read(UC_X86_REG_ECX)==OBJECT and len(events)<3;params=get(sp+4);snapshot=bytes(u.mem_read(params,44));call=len(events)
   events.append((params==PARAMETERS,snapshot[:29]+snapshot[32:]));result=results[call]
   if mutation==1 and call==0:put(params+4,2048);put(params+8,900);put(params+20,3);put(params+40,4)
   if mutation==2 and call==1:put(params+4,4096);put(params+8,1200);put(params+24,30);put(params+40,4)
  u.reg_write(UC_X86_REG_EAX,result&0xffffffff);u.reg_write(UC_X86_REG_ECX,0xdead0001);u.reg_write(UC_X86_REG_EDX,0xdead0002);u.reg_write(UC_X86_REG_EIP,get(sp));u.reg_write(UC_X86_REG_ESP,sp+8)
 u.hook_add(UC_HOOK_CODE,hook);sp=STACK+0x8000;put(sp,STOP);u.reg_write(UC_X86_REG_ESP,sp);u.reg_write(UC_X86_REG_ECX,OBJECT)
 saved=[UC_X86_REG_EBX,UC_X86_REG_ESI,UC_X86_REG_EDI,UC_X86_REG_EBP]
 for i,r in enumerate(saved):u.reg_write(r,0x12340000+i)
 u.emu_start(ENTRY,STOP+1,count=100000)
 assert returned and u.reg_read(UC_X86_REG_ESP)==sp+4 and [u.reg_read(r) for r in saved]==[0x12340000+i for i in range(4)]
 result=u.reg_read(UC_X86_REG_EAX)&255;assert result in [0,1]
 return result,events,bytes(u.mem_read(OBJECT,0x2c8))
