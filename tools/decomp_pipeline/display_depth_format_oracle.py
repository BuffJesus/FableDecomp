import struct
from unicorn import Uc,UC_ARCH_X86,UC_MODE_32,UC_HOOK_CODE
from unicorn.x86_const import *
ENTRY=0x9bebb0
INIT=0x9e3830
FORMATS=0x129ba40
OBJECT=0x2000000
MODE=OBJECT+0x1000
OUTPUT=OBJECT+0x1100
API=OBJECT+0x1200
TABLE=OBJECT+0x1300
CHECK=0x500000
MATCH=CHECK+0x100
STACK=0x3000000
STOP=0x2200000
DEPTHS=[32,16,16,24,32,24,40,-1]
TARGET_DEPTHS=[-32,0,8,16,20,24,28,32,36,40,64,128]

def execute(code,depth,format_mask,match_mask,mutation):
 u=Uc(UC_ARCH_X86,UC_MODE_32)
 for a,n in [(0x9be000,4096),(0x9e3000,8192),(0x129b000,8192),(OBJECT,0x4000),(CHECK,4096),(STACK,0x10000),(STOP,4096)]:u.mem_map(a,n)
 def put(a,v):u.mem_write(a,struct.pack('<I',v&0xffffffff))
 def get(a):return struct.unpack('<I',u.mem_read(a,4))[0]
 def fails(mask,i):return 3<=i<=5 and mask&(1<<(i-3))
 u.mem_write(ENTRY,bytes(code));put(OBJECT+0x54,API);put(OBJECT+0x60,7);put(OBJECT+0x5c,1);put(API,TABLE);put(TABLE+40,CHECK);put(TABLE+48,MATCH)
 u.mem_write(MODE,struct.pack('<4I',640,480,60,22))
 for i,bits in enumerate(DEPTHS):
  put(FORMATS+i*36,100+i);put(FORMATS+i*36+4,2 if i==0 else 3);put(FORMATS+i*36+8,bits);put(FORMATS+i*36+28,7 if i==1 else 16 if i==5 else 8)
 events=[];returned=False
 def hook(u,a,size,data):
  nonlocal returned
  if a==STOP:returned=True;u.emu_stop();return
  if a not in [INIT,CHECK,MATCH]:return
  sp=u.reg_read(UC_X86_REG_ESP);result=0
  if a==INIT:
   obj=u.reg_read(UC_X86_REG_ECX);assert obj==OUTPUT;put(obj,get(sp+4)-100);cleanup=4;events.append(('init',get(sp+4)))
  else:
   args=[get(sp+4+i*4) for i in range(7 if a==CHECK else 6)]
   assert args[:4]==[API,get(OBJECT+0x60),1,get(MODE+12)];index=args[-1]-100;assert 2<=index<=6
   if a==CHECK:
    assert args[4:6]==[2,1];cleanup=28;result=0x80004005 if fails(format_mask,index) else index&1;events.append(('format',args))
    if mutation:put(MODE+12,42);put(OBJECT+0x60,9)
   else:
    assert args[4]==get(MODE+12) and not fails(format_mask,index);cleanup=24;result=0x80004005 if fails(match_mask,index) else index&1;events.append(('match',args))
  u.reg_write(UC_X86_REG_EAX,result);u.reg_write(UC_X86_REG_ECX,0xdead0001);u.reg_write(UC_X86_REG_EDX,0xdead0002);u.reg_write(UC_X86_REG_EIP,get(sp));u.reg_write(UC_X86_REG_ESP,sp+4+cleanup)
 u.hook_add(UC_HOOK_CODE,hook);sp=STACK+0x8000
 for i,v in enumerate([STOP,OUTPUT,MODE,depth]):put(sp+i*4,v)
 u.reg_write(UC_X86_REG_ESP,sp);u.reg_write(UC_X86_REG_ECX,OBJECT)
 saved=[UC_X86_REG_EBX,UC_X86_REG_ESI,UC_X86_REG_EDI,UC_X86_REG_EBP]
 for i,r in enumerate(saved):u.reg_write(r,0x12340000+i)
 u.emu_start(ENTRY,STOP+1,count=100000)
 assert returned and u.reg_read(UC_X86_REG_ESP)==sp+16 and u.reg_read(UC_X86_REG_EAX)==OUTPUT
 assert [u.reg_read(r) for r in saved]==[0x12340000+i for i in range(4)]
 assert 2<=get(OUTPUT)<=6
 return get(OUTPUT),events,bytes(u.mem_read(OBJECT,0x1400))
