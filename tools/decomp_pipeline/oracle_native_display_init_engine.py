"""Full display InitEngine replay with explicit engine/string boundary events."""
import itertools,struct
from unicorn import Uc,UC_ARCH_X86,UC_MODE_32,UC_HOOK_CODE
from unicorn.x86_const import *
from pe_oracle import EXE,pe_sections
BASE=0x24000000;WIDTH=4096
SETTINGS=[0x13b8384,0x1375490,0x13b8649,0x1375459,0x13b864a,0x13b86a0,0x13b8774]
CONFIG=0x1375070;MEMORY=0x137549c
def cases():
 for flags,state,editor,world in itertools.product(range(128),range(4),range(2),range(2)):
  yield (flags&255,flags,state,editor,world,0,[0,1,2,0xffffffff][flags&3])
 for mode,state,editor,world in itertools.product(range(1,17),range(4),range(2),range(2)):
  yield (mode*13&255,0x55,state,editor,world,mode,1)

def collect():
 u=Uc(UC_ARCH_X86,UC_MODE_32);u.mem_map(0x400000,0x1200000);u.mem_map(BASE,0x40000);raw=EXE.read_bytes()
 for _,va,_,off,n in pe_sections(raw):
  if n:u.mem_write(0x400000+va,raw[off:off+n])
 inputs=bytearray();expected=bytearray();snapshots=count=0;init=0;cached=bytes(100);live=False;dtor_count=0
 def get(a):return struct.unpack('<I',u.mem_read(a,4))[0]
 def put(a,v):u.mem_write(a,struct.pack('<I',v&0xffffffff))
 def event(kind,a=0,b=0,c=0):
  nonlocal snapshots
  expected.extend(struct.pack('<4I',kind,a,b,c));expected.extend(u.mem_read(BASE,WIDTH));expected.extend(u.mem_read(init,100)if live else cached)
  expected.extend(struct.pack('<7I',*[u.mem_read(v,1)[0]if i in (2,3,4)else get(v)for i,v in enumerate(SETTINGS)]));expected.extend(u.mem_read(CONFIG,64));expected.extend(u.mem_read(MEMORY,40));expected.extend(u.mem_read(BASE+0x26144,4));snapshots+=1
 def mutate(kind,label=0):
  if mode==1 and kind==1:put(BASE+0x26144,BASE+0xe40)
  elif mode==2 and kind==2:put(BASE+0xf8,0);put(BASE+0x2c,BASE+0x480)
  elif mode==3 and kind==3:put(BASE+0x10,BASE+0x880)
  elif mode==4 and kind==20:put(init+8,BASE+0xe80)
  elif mode==5 and kind==30 and label==0x20:put(init+0x60,0x4000)
  elif mode==6 and kind==31 and label==0xff:
   put(SETTINGS[1],0x12345678);put(MEMORY,0xabcdef01)
  elif mode==7 and kind==4:
   put(init+0x60,0x2000);u.mem_write(SETTINGS[3],b'\x01');u.mem_write(CONFIG+0x35,b'\x01\x01\x01\x01')
  elif mode==8 and kind==40 and label==1:put(BASE+0x2c,BASE+0x440)
  elif mode==9 and kind==5:put(BASE+0x2c,BASE+0x440);put(init+0x5c,0x76543210)
  elif mode==10:
   if kind==40 and label==2:put(BASE+0x2c,BASE+0x440)
   elif kind==6:put(BASE+0x2c,BASE+0x400)
  elif mode==11 and kind==7:put(BASE+0x2c,BASE+0x440)
  elif mode==12 and kind==40 and label==3:put(BASE+0x2c,BASE+0x440)
  elif mode==13 and kind==8:put(BASE+0x10,BASE+0x880)
  elif mode==14 and kind==9:put(BASE+0xb50,0x3fc00000)
  elif mode==15 and kind==10:put(SETTINGS[6],0x7fc12345);put(BASE+0xdc,0x55667788)
  elif mode==16 and kind==31 and label!=0xff:put(BASE+0x2c,BASE+0x480)
 boundaries={0x42d830,0x9a4ec0,0xb26340,0x41a100,0x41a1c0,0x41a1e0,0x41a200,0x41a220,0x99b7d0,0x99b510,0x99ebf0,0x99eae0,0x9e9f40,0x436fb0,0x6404d0,0x430096,BASE+0xb000,BASE+0xb100,BASE+0xb200,BASE+0xb300}
 getters=[0x41a100,0x41a1c0,0x41a1e0,0x41a200,0x41a220]
 text_addresses=[0x1231600,0x12315e8,0x12315d0,0x1230c84]
 def hook(u,a,n,_):
  nonlocal init,cached,live,dtor_count
  if a not in boundaries:return
  sp=u.reg_read(UC_X86_REG_ESP);this=u.reg_read(UC_X86_REG_ECX);result=this;cleanup=0
  if a==0x42d830:
   init=this;live=True;u.mem_write(init,bytes([fill^0x5a])*100)
   for i in range(5):put(init+0x20+i*4,BASE+0xd80+i*16)
   event(1);mutate(1)
  elif a==0x9a4ec0:event(2);mutate(2);result=BASE+0xf00
  elif a==0xb26340:event(3);mutate(3);result=BASE+0x400 if state else 0
  elif a in getters:
   label=getters.index(a);event(20+label);mutate(20+label);put(this,BASE+0xd00+label*16)
  elif a==0x99b7d0:
   label=this-init;assert label in range(0x20,0x34,4);value=get(get(sp+4));event(30,label,value);put(this,value);mutate(30,label);cleanup=4
  elif a==0x99b510:
   label=this-init if init+0x20<=this<init+0x34 else 0xff;event(31,label,get(this));put(this,0);mutate(31,label)
   if label!=0xff:
    dtor_count+=1
    if dtor_count==5:cached=bytes(u.mem_read(init,100));live=False
  elif a==BASE+0xb100:event(4,this);mutate(4);result=editor
  elif a==0x99ebf0:
   assert get(sp+8)==0xffffffff;label=text_addresses.index(get(sp+4))+1;event(32,label);put(this,label);cleanup=8
  elif a==0x99eae0:event(33,get(this));put(this,0)
  elif a==0x9e9f40:
   label=get(this);assert get(sp+4)==0xbf800000 and get(sp+8)&255==0 and u.reg_read(UC_X86_REG_EDX)&255==0;event(40,label);mutate(40,label);cleanup=8
  elif a==BASE+0xb000:
   assert get(sp+4)==init;event(5,this);mutate(5);cleanup=4;result=1 if state==1 else 2 if state==3 else 0
  elif a==0x436fb0:event(6);mutate(6);result=BASE+0xc00
  elif a==0x6404d0:event(7,this,get(sp+4));mutate(7);cleanup=4
  elif a==BASE+0xb300:event(8,this);mutate(8)
  elif a==0x430096:
   assert get(get(sp+4))==4 and get(sp+8)==1;event(9,this);put(get(sp+12),BASE+0xb00);mutate(9);result=1;cleanup=12
  else:event(10,this);mutate(10)
  u.reg_write(UC_X86_REG_EAX,result);u.reg_write(UC_X86_REG_ESP,sp+4+cleanup);u.reg_write(UC_X86_REG_EIP,get(sp))
 u.hook_add(UC_HOOK_CODE,hook)
 for fill,flags,state,editor,world,mode,refs in cases():
  inputs.extend(struct.pack('<7I',fill,flags,state,editor,world,mode,refs));init=0;live=False;cached=bytes(100);dtor_count=0;u.mem_write(BASE,bytes([fill])*WIDTH)
  for off,v in [(8,BASE+0x10000),(0x10,BASE+0x800),(0x14,BASE+0xa40),(0x18,BASE+0xa80),(0xf8,BASE+0x600 if world else 0),(0x400,BASE+0x8000),(0x440,BASE+0x8000),(0x900,BASE+0x8100),(0xb00,BASE+0x8200),(0xb04,refs),(0xb50,0x3f400000),(0x26144,BASE+0xe00),(0x8008,BASE+0xb000),(0x80c4,BASE+0xb300),(0x8124,BASE+0xb100),(0x8204,BASE+0xb200)]:put(BASE+off,v)
  values=[BASE+0xa00,0xfedcba98,flags&1,(flags>>1)&1,(flags>>2)&1,BASE+0x900,0x55667788]
  for i,(address,value)in enumerate(zip(SETTINGS,values)):
   if i in (2,3,4):u.mem_write(address,bytes([value]))
   else:put(address,value)
  config=bytearray([fill^0xa5])*64
  for bit,off in enumerate([0x37,0x38,0x35,0x36],3):config[off]=(flags>>bit)&1
  u.mem_write(CONFIG,bytes(config));u.mem_write(MEMORY,struct.pack('<10I',*[0x10203040+i for i in range(10)]))
  sp=BASE+0x3e000;stop=BASE+0x3f000;put(sp,stop);u.reg_write(UC_X86_REG_ESP,sp);u.reg_write(UC_X86_REG_ECX,BASE);u.emu_start(0x435190,stop,count=100000);assert u.reg_read(UC_X86_REG_ESP)==sp+4 and dtor_count==5;event(99,u.reg_read(UC_X86_REG_EAX)&255);count+=1
 return inputs,expected,dict(cases=count,snapshots=snapshots)
