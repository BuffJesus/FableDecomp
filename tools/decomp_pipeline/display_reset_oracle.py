"""Retail reset-body emulator with typed D3D and engine method boundaries."""
import struct
from unicorn import Uc,UC_ARCH_X86,UC_MODE_32,UC_HOOK_CODE
from unicorn.x86_const import *
ENTRY=0x9bfb30
PRE=0x9bf500
SURFACE_CLEAR=0x9f2e20
TEXTURE_CLEAR=0x9f9f70
CLOSEST=0x9bf600
PIXEL_INIT=0x9e3830
DEPTH_FORMAT=0x9bebb0
INTERVAL=0x9be2d0
THROW=0xbfeb84
THROW_INFO=0x13692f8
SURFACE_INIT=0x9f2ec0
DIMENSIONS=0x9f2f90
PIXEL=0x9f2f60
ASSIGN=0x9f2e70
SETUP=0x9bee40
POST=0x9bf550
FORMATS=0x129ba40
OBJECT=0x2000000
PARAMETERS=OBJECT+0x1000
API=OBJECT+0x1200
DEVICE=OBJECT+0x1210
API_TABLE=OBJECT+0x1300
DEVICE_TABLE=OBJECT+0x1400
SURFACE=OBJECT+0x1800
CHECK=0x500000
RESET=CHECK+0x100
ADAPTER=CHECK+0x200
BACK=CHECK+0x300
DEPTH=CHECK+0x400
STACK=0x3000000
STOP=0x2200000

def execute(code,exclusive,sample,check_result,reset_result,fail_stage,mutation,alias):
 u=Uc(UC_ARCH_X86,UC_MODE_32)
 for a,n in [(0x9be000,0x2000),(0x9e3000,8192),(0x9f2000,8192),(0x9f9000,8192),(0xbfe000,8192),(0x129b000,8192),(OBJECT,0x4000),(CHECK,8192),(STACK,0x10000),(STOP,4096)]:u.mem_map(a,n)
 def put(a,v):u.mem_write(a,struct.pack('<I',v&0xffffffff))
 def get(a):return struct.unpack('<I',u.mem_read(a,4))[0]
 def byte(a):return u.mem_read(a,1)[0]
 def put_byte(a,v):u.mem_write(a,bytes([v]))
 u.mem_write(ENTRY,bytes(code));params=OBJECT+0x268 if alias else PARAMETERS
 u.mem_write(params,b'\xa5'*44)
 for off,value in [(0,0x1111),(4,640),(8,480),(12,32),(16,24),(20,2),(24,60),(36,1),(40,sample*2)]:put(params+off,value)
 put_byte(params+28,exclusive);put_byte(params+32,0);put_byte(params+33,1);copied=bytes(u.mem_read(params,44))
 put(OBJECT+0x54,API);put(OBJECT+0x58,DEVICE);put(OBJECT+0x5c,1);put(OBJECT+0x1b4,0x2222);put_byte(OBJECT+0x255,1);put(OBJECT+0x218,4)
 put(API,API_TABLE);put(DEVICE,DEVICE_TABLE);put(API_TABLE+32,ADAPTER);put(API_TABLE+44,CHECK);put(DEVICE_TABLE+64,RESET);put(DEVICE_TABLE+72,BACK);put(DEVICE_TABLE+160,DEPTH)
 put(FORMATS,22);put(FORMATS+36,75)
 events=[];returned=False;thrown=False
 boundaries=[PRE,SURFACE_CLEAR,TEXTURE_CLEAR,CLOSEST,PIXEL_INIT,DEPTH_FORMAT,INTERVAL,THROW,SURFACE_INIT,DIMENSIONS,PIXEL,ASSIGN,SETUP,POST,CHECK,RESET,ADAPTER,BACK,DEPTH]
 def hook(u,a,size,data):
  nonlocal returned,thrown
  if a==STOP:returned=True;u.emu_stop();return
  if a not in boundaries:return
  sp=u.reg_read(UC_X86_REG_ESP);obj=u.reg_read(UC_X86_REG_ECX);cleanup=0;result=0
  def arg(i):return get(sp+4+i*4)
  if a==PRE:
   assert obj==OBJECT and bytes(u.mem_read(OBJECT+0x268,44))==copied;events.append(1)
   if mutation&1:put(params+20,3);put(params+24,75);put(params+36,2)
  elif a==SURFACE_CLEAR:
   assert obj-OBJECT in [0x2c,0x3c,0xc,0x1c];events.append(obj-OBJECT);u.mem_write(obj+4,b'\0'*12)
  elif a==TEXTURE_CLEAR:assert obj==OBJECT+0x4c;events.append(0x4c);u.mem_write(obj,b'\0'*8)
  elif a==CLOSEST:
   assert obj==OBJECT and arg(1)==params+4 and arg(2)==get(params+12) and arg(3)&255==byte(params+28)
   result=arg(0);u.mem_write(result,struct.pack('<4I',800,600,60,22));cleanup=16;events.append(2)
  elif a==PIXEL_INIT:
   assert arg(0)==22;put(obj,0);cleanup=4;events.append(3)
   if mutation&2:put(params+40,4)
  elif a==CHECK:
   assert [arg(i) for i in range(7)]==[API,0,1,22,0,get(params+40),0];cleanup=28;result=0x80004005 if check_result==2 else check_result;events.append(4)
  elif a==DEPTH_FORMAT:
   assert obj==OBJECT and arg(1)==OBJECT+0x1b8 and arg(2)==get(params+16);result=arg(0);put(result,1);cleanup=12;events.append(5)
  elif a==INTERVAL:
   assert obj==OBJECT and arg(0)==OBJECT+0x1cc and bytes(u.mem_read(arg(0),8))==bytes(u.mem_read(params+32,8));result=4;cleanup=4;events.append(6)
  elif a==RESET:
   assert [arg(i) for i in range(2)]==[DEVICE,OBJECT+0x21c]
   ms=get(params+40) if exclusive and get(params+40) and check_result==0 else 0
   expected=[800,600,22,get(params+20),ms,0,1,0x2222,int(not exclusive),1,75,0,get(params+24) if exclusive else 0,4 if exclusive else 0]
   assert bytes(u.mem_read(arg(1),56))==struct.pack('<14I',*expected)
   assert get(OBJECT+0x218)==0 and get(OBJECT+0x19c)==get(params+20) and get(OBJECT+0x1a4)==ms
   result=[0,1,0x88760868,0x80004005][reset_result];cleanup=8;events.append(7)
  elif a==ADAPTER:
   assert arg(0)==API and arg(1)==0 and byte(OBJECT+0x255)==0;u.mem_write(arg(2),struct.pack('<4I',1600,900,50,22));result=0x80004005 if fail_stage==1 else 1;cleanup=12;events.append(8)
  elif a==THROW:
   assert arg(1)==THROW_INFO and fail_stage==1 and reset_result<2;events.append('throw');thrown=True;u.emu_stop();return
  elif a==BACK:
   assert [arg(i) for i in range(4)]==[DEVICE,0,0,0];assert get(OBJECT+0x1c8)==50
   assert bytes(u.mem_read(OBJECT+0x1d4,4))==struct.pack('<f',get(params+36)/50)
   put(arg(4),SURFACE);result=0x80004005 if fail_stage==2 else 1;cleanup=20;events.append(9)
  elif a==SURFACE_INIT:
   assert obj-OBJECT in [0x2c,0x3c] and arg(1)&255==0
   assert arg(0)==SURFACE+(4 if obj==OBJECT+0x3c else 0);put(obj+4,arg(0));cleanup=8;events.append(10 if obj==OBJECT+0x2c else 14)
  elif a==DIMENSIONS:
   assert obj==OBJECT+0x2c;result=arg(0);u.mem_write(result,struct.pack('<2I',1280,720));cleanup=4;events.append(11)
  elif a==PIXEL:
   assert obj-OBJECT in [0x2c,0x3c];result=arg(0);put(result,0 if obj==OBJECT+0x2c else 1);cleanup=4;events.append(12 if obj==OBJECT+0x2c else 15)
  elif a==DEPTH:
   assert arg(0)==DEVICE and [get(OBJECT+off) for off in [0x18c,0x190,0x1a8]]==[1280,720,0];put(arg(1),SURFACE+4);result=0x80004005 if fail_stage==3 else 1;cleanup=8;events.append(13)
  elif a==ASSIGN:
   assert obj-OBJECT in [0xc,0x1c] and arg(0)==obj+0x20 and get(OBJECT+0x1ac)==1
   u.mem_write(obj+4,bytes(u.mem_read(arg(0)+4,12)));result=obj;cleanup=4;events.append(16 if obj==OBJECT+0xc else 17)
  elif a==SETUP:
   assert obj==OBJECT and byte(OBJECT+0x1a0)==exclusive and [get(OBJECT+off) for off in [0x194,0x198]]==[1280,720];result=int(fail_stage!=4);events.append(18)
  else:assert a==POST and obj==OBJECT;result=int(fail_stage!=5);events.append(19)
  u.reg_write(UC_X86_REG_EAX,result);u.reg_write(UC_X86_REG_ECX,0xdead0001);u.reg_write(UC_X86_REG_EDX,0xdead0002);u.reg_write(UC_X86_REG_EIP,get(sp));u.reg_write(UC_X86_REG_ESP,sp+4+cleanup)
 u.hook_add(UC_HOOK_CODE,hook);sp=STACK+0x8000;put(sp,STOP);put(sp+4,params);u.reg_write(UC_X86_REG_ESP,sp);u.reg_write(UC_X86_REG_ECX,OBJECT)
 saved=[UC_X86_REG_EBX,UC_X86_REG_ESI,UC_X86_REG_EDI,UC_X86_REG_EBP]
 for i,r in enumerate(saved):u.reg_write(r,0x12340000+i)
 u.emu_start(ENTRY,STOP+1,count=100000)
 assert returned or thrown
 if returned:
  assert u.reg_read(UC_X86_REG_ESP)==sp+8 and [u.reg_read(r) for r in saved]==[0x12340000+i for i in range(4)]
  expected=1 if reset_result==2 else 0 if reset_result==3 or fail_stage else 2
  assert u.reg_read(UC_X86_REG_EAX)==expected
 assert byte(OBJECT+0x255)==int(reset_result==2)
 return thrown,events,bytes(u.mem_read(OBJECT,0x2c8)),bytes(u.mem_read(params,44))
