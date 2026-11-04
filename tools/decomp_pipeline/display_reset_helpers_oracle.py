import struct
from unicorn import Uc,UC_ARCH_X86,UC_MODE_32,UC_HOOK_CODE
from unicorn.x86_const import *
ENTRY=0x9bee40
PIXEL=0x9f2f60
TEXTURE=0x9fa280
OBJECT=0x2000000
STACK=0x3000000
STOP=0x2200000

def execute(code,caps,x,y,result,mutation):
 u=Uc(UC_ARCH_X86,UC_MODE_32)
 for a,n in [(0x9be000,4096),(0x9f2000,8192),(0x9fa000,4096),(OBJECT,4096),(STACK,0x10000),(STOP,4096)]:u.mem_map(a,n)
 def put(a,v):u.mem_write(a,struct.pack('<I',v&0xffffffff))
 def get(a):return struct.unpack('<I',u.mem_read(a,4))[0]
 u.mem_write(ENTRY,bytes(code));u.mem_write(OBJECT,b'\x9d'*0x2c8);put(OBJECT+0x98,caps);put(OBJECT+0x194,x);put(OBJECT+0x198,y)
 events=[];returned=False
 def hook(u,a,size,data):
  nonlocal returned
  if a==STOP:returned=True;u.emu_stop();return
  if a not in [PIXEL,TEXTURE]:return
  sp=u.reg_read(UC_X86_REG_ESP);obj=u.reg_read(UC_X86_REG_ECX)
  if a==PIXEL:
   assert obj==OBJECT+12;ret=get(sp+4);put(ret,0x76543210);cleanup=4;events.append('format')
   if mutation&1:put(OBJECT+0x98,get(OBJECT+0x98)^2)
   if mutation&2:put(OBJECT+0x194,73);put(OBJECT+0x198,-19)
  else:
   assert obj==OBJECT+0x4c;args=[get(sp+4+i*4) for i in range(7)];assert args[1]==1 and args[3:]==[1,0,0,0]
   dims=(get(args[0]),get(args[0]+4));power=get(OBJECT+0x98)&2
   assert dims==((1024,1024) if power else (get(OBJECT+0x194),get(OBJECT+0x198)))
   assert get(args[2])==0x76543210;events.append(('texture',dims));cleanup=28;ret=0xdead0000|result
  u.reg_write(UC_X86_REG_EAX,ret);u.reg_write(UC_X86_REG_ECX,0xdead0001);u.reg_write(UC_X86_REG_EDX,0xdead0002)
  u.reg_write(UC_X86_REG_EIP,get(sp));u.reg_write(UC_X86_REG_ESP,sp+4+cleanup)
 u.hook_add(UC_HOOK_CODE,hook);sp=STACK+0x8000;put(sp,STOP);u.reg_write(UC_X86_REG_ESP,sp);u.reg_write(UC_X86_REG_ECX,OBJECT)
 saved=[UC_X86_REG_EBX,UC_X86_REG_ESI,UC_X86_REG_EDI,UC_X86_REG_EBP]
 for i,r in enumerate(saved):u.reg_write(r,0x12340000+i)
 try:u.emu_start(ENTRY,STOP+1,count=10000)
 except Exception as error:raise AssertionError((hex(u.reg_read(UC_X86_REG_EIP)),hex(u.reg_read(UC_X86_REG_ESP)),events)) from error
 assert returned and u.reg_read(UC_X86_REG_ESP)==sp+4 and u.reg_read(UC_X86_REG_EAX)&255==result
 assert [u.reg_read(r) for r in saved]==[0x12340000+i for i in range(4)]
 assert len(events)==2
 return events,bytes(u.mem_read(OBJECT,0x2c8))
