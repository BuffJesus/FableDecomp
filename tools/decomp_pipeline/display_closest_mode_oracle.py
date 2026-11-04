import struct
from unicorn import Uc,UC_ARCH_X86,UC_MODE_32,UC_HOOK_CODE
from unicorn.x86_const import *
ENTRY=0x9bf600
INIT=0x9e3830
DEPTH=0x9e3820
FORMATS=0x129ba40
OBJECT=0x2000000
ARRAY=OBJECT+0x1000
REQUEST=OBJECT+0x1100
OUTPUT=OBJECT+0x1200
API=OBJECT+0x1300
DEVICE=OBJECT+0x1310
API_TABLE=OBJECT+0x1400
DEVICE_TABLE=OBJECT+0x1500
COUNT=0x500000
ENUM=COUNT+0x100
DISPLAY=COUNT+0x200
STACK=0x3000000
STOP=0x2200000

def execute(code,profile,requested,depth,order,mutation,failure,exclusive=True,have_device=False):
 u=Uc(UC_ARCH_X86,UC_MODE_32)
 for a,n in [(0x9bf000,4096),(0x9e3000,8192),(0x129b000,8192),(OBJECT,0x4000),(COUNT,4096),(STACK,0x10000),(STOP,4096)]:u.mem_map(a,n)
 def put(a,v):u.mem_write(a,struct.pack('<I',v&0xffffffff))
 def get(a):return struct.unpack('<I',u.mem_read(a,4))[0]
 def count(f):return 0 if profile==0 else (profile+f)%4+1
 def mode(f,m):
  slot=1 if profile==5 else (profile+f+m)%4
  return ([640,800,1024,1280][slot],[480,600,768,720][slot],50+m+f*5,20+f)
 u.mem_write(ENTRY,bytes(code));put(OBJECT+0x54,API);put(OBJECT+0x58,DEVICE if have_device else 0);put(OBJECT+0x60,7)
 put(API,API_TABLE);put(DEVICE,DEVICE_TABLE)
 for off,target in [(24,COUNT),(28,ENUM),(32,DISPLAY)]:put(API_TABLE+off,target)
 put(DEVICE_TABLE+32,DISPLAY)
 for i in range(3):put(ARRAY+i*4,2-i if order else i);put(FORMATS+i*36,20+i)
 put(OBJECT+0x298,ARRAY);put(OBJECT+0x29c,ARRAY+(profile%4)*4);put(OBJECT+0x2a0,ARRAY+12)
 put(REQUEST,640+requested*160);put(REQUEST+4,480+requested*120)
 events=[];returned=False
 def mutate():
  if mutation:put(REQUEST,800);put(REQUEST+4,600)
 def hook(u,a,size,data):
  nonlocal returned
  if a==STOP:returned=True;u.emu_stop();return
  if a not in [INIT,DEPTH,COUNT,ENUM,DISPLAY]:return
  sp=u.reg_read(UC_X86_REG_ESP);obj=u.reg_read(UC_X86_REG_ECX);cleanup=0;result=0
  if a==INIT:put(obj,get(sp+4)-20);cleanup=4;events.append(('init',get(sp+4)))
  elif a==DEPTH:result=16+8*get(obj);mutate();events.append(('depth',result))
  elif a==COUNT:
   assert get(sp+4)==API and get(sp+8)==7;f=get(sp+12)-20;assert 0<=f<3;result=count(f);cleanup=12;events.append(('count',f,result))
  elif a==ENUM:
   assert get(sp+4)==API and get(sp+8)==7;f=get(sp+12)-20;m=get(sp+16);assert m<count(f)
   u.mem_write(get(sp+20),struct.pack('<4I',*mode(f,m)));cleanup=20;result=0x80004005 if failure else 0;events.append(('enum',f,m))
  else:
   assert (get(sp+4),get(sp+8))==((DEVICE,0) if have_device else (API,7))
   u.mem_write(get(sp+12),struct.pack('<4I',*mode(2,0)));cleanup=12;result=0x80004005 if failure else 0;mutate();events.append('display')
  u.reg_write(UC_X86_REG_EAX,result);u.reg_write(UC_X86_REG_ECX,0xdead0001);u.reg_write(UC_X86_REG_EDX,0xdead0002);u.reg_write(UC_X86_REG_EIP,get(sp));u.reg_write(UC_X86_REG_ESP,sp+4+cleanup)
 u.hook_add(UC_HOOK_CODE,hook);sp=STACK+0x8000
 for i,v in enumerate([STOP,OUTPUT,REQUEST,16+depth*8,int(exclusive)]):put(sp+i*4,v)
 u.reg_write(UC_X86_REG_ESP,sp);u.reg_write(UC_X86_REG_ECX,OBJECT)
 saved=[UC_X86_REG_EBX,UC_X86_REG_ESI,UC_X86_REG_EDI,UC_X86_REG_EBP]
 for i,r in enumerate(saved):u.reg_write(r,0x12340000+i)
 u.emu_start(ENTRY,STOP+1,count=100000)
 assert returned and u.reg_read(UC_X86_REG_ESP)==sp+20 and u.reg_read(UC_X86_REG_EAX)==OUTPUT
 assert [u.reg_read(r) for r in saved]==[0x12340000+i for i in range(4)]
 result=bytes(u.mem_read(OUTPUT,16));empty=exclusive and not any(isinstance(e,tuple) and e[0]=='enum' for e in events)
 if empty:assert result[12:]==b'\0'*4;result=result[12:]
 return result,events,bytes(u.mem_read(OBJECT,0x1000)),bytes(u.mem_read(REQUEST,8))
