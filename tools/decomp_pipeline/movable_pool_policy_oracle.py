"""Execute pool move/permission/validation/free/set-resource policies against expected semantics."""
import struct
from unicorn import Uc,UC_ARCH_X86,UC_MODE_32,UC_HOOK_CODE
from unicorn.x86_const import *
POOL=0x2000000;DATA=0x2010000;RES=0x2020000;VT=0x2020100;SLOTS=0x5000000;HEADERS=0x2030000;INFOS=0x2030100;STACK=0x3000000;STOP=0x4000000
MEMMOVE=0xbfeae6;FREE=0xa02b30
BASE_MOVE,BASE_CAN,BASE_VALIDATE,SET=0xa02120,0xa03a40,0xa03a50,0xa028b0
MOVE,CAN,VALIDATE,FREE_RESOURCE=0xa62f30,0xa62f70,0xa62ec0,0xa62ed0
OFFSETS=(0,1,8,24,48);SIZES=(0,1,7,16,31);HANDLES=(0,1,0xffffffff,0x80000000)
def run(codes,entry,this,args,allow=False,verify=0x87654321,setup=None):
 u=Uc(UC_ARCH_X86,UC_MODE_32)
 for a,z in [(0xa02000,0x2000),(0xa62000,0x1000),(0xbfe000,0x1000),(POOL,0x40000),(STACK,0x10000),(STOP,0x1000),(SLOTS,0x1000)]:u.mem_map(a,z)
 def put(a,v):u.mem_write(a,struct.pack('<I',v&0xffffffff))
 def get(a):return struct.unpack('<I',u.mem_read(a,4))[0]
 for a,b in codes.items():u.mem_write(a,bytes(b))
 u.mem_write(POOL,b'\xa5'*0x464);put(RES,VT)
 for k in range(4):put(VT+4*k,SLOTS+k*16);u.mem_write(SLOTS+k*16,b'\xc3')
 for a in (MEMMOVE,FREE):u.mem_write(a,b'\xc3')
 if setup:setup(u,put)
 trace=[];stubs={MEMMOVE:'memmove',FREE:'free',SLOTS:'dtor',SLOTS+16:'relocate',SLOTS+32:'canrelocate',SLOTS+48:'verify'};pops=dict(memmove=0,free=4,relocate=16,canrelocate=4,verify=0,dtor=4)
 def hook(uc,a,size,user):
  name=stubs.get(a)
  if not name:return
  sp=uc.reg_read(UC_X86_REG_ESP);this=uc.reg_read(UC_X86_REG_ECX);result=0
  if name=='memmove':
   dst,src,n=get(sp+4),get(sp+8),get(sp+12);uc.mem_write(dst,bytes(uc.mem_read(src,n)));result=dst;trace.append(('memmove',dst,src,n))
  elif name=='free':trace.append(('free',this,get(sp+4)))
  elif name=='relocate':trace.append(('relocate',this,get(sp+4),get(sp+8),get(sp+12),get(sp+16),bytes(uc.mem_read(DATA,96))))
  elif name=='canrelocate':result=int(allow);trace.append(('canrelocate',this,get(sp+4)))
  elif name=='verify':result=verify;trace.append(('verify',this))
  else:raise AssertionError('destructor called')
  uc.reg_write(UC_X86_REG_EAX,result);uc.reg_write(UC_X86_REG_EIP,get(sp));uc.reg_write(UC_X86_REG_ESP,sp+4+pops[name])
 u.hook_add(UC_HOOK_CODE,hook);sp=STACK+0x8000;put(sp,STOP)
 for i,v in enumerate(args):put(sp+4+4*i,v)
 regs=[UC_X86_REG_EBX,UC_X86_REG_ESI,UC_X86_REG_EDI,UC_X86_REG_EBP]
 for i,r in enumerate(regs):u.reg_write(r,0x778800+i)
 u.reg_write(UC_X86_REG_ECX,this);u.reg_write(UC_X86_REG_ESP,sp);u.reg_write(UC_X86_REG_EAX,0xdeadbeef)
 u.emu_start(entry,STOP,count=1000);assert u.reg_read(UC_X86_REG_EIP)==STOP and u.reg_read(UC_X86_REG_ESP)==sp+4+4*len(args),hex(entry)
 assert all(u.reg_read(r)==0x778800+i for i,r in enumerate(regs)),hex(entry)
 return trace,u.reg_read(UC_X86_REG_EAX),bytes(u.mem_read(DATA,96)),bytes(u.mem_read(POOL,0x464)),bytes(u.mem_read(HEADERS,0x60))
def scenario(codes,f,t,z,present,can,h):
 handle=HANDLES[h];original=bytes((i*17+h*31+f)&0xff for i in range(96));expected=bytearray(original);expected[OFFSETS[t]:OFFSETS[t]+SIZES[z]]=original[OFFSETS[f]:OFFSETS[f]+SIZES[z]]
 frm,to,n=DATA+OFFSETS[f],DATA+OFFSETS[t],SIZES[z];r=RES if present else 0;fill=lambda u,put:u.mem_write(DATA,original);out=[]
 tr,_,data,pool,_=run(codes,BASE_MOVE,POOL,[r,handle,frm,to,n],setup=fill);assert tr==[('memmove',to,frm,n)] and data==bytes(expected) and pool==b'\xa5'*0x464;out.append(tr)
 tr,_,data,pool,_=run(codes,MOVE,POOL,[r,handle,frm,to,n],setup=fill);want=[('memmove',to,frm,n)]+([('relocate',RES,handle,frm,to,n,bytes(expected))] if present else [])
 assert tr==want and data==bytes(expected) and pool==b'\xa5'*0x464;out.append(tr)
 tr,eax,_,_,_=run(codes,BASE_CAN,POOL,[r,handle],allow=can);assert not tr and eax&0xff==1;out.append(eax&0xff)
 tr,eax,_,_,_=run(codes,CAN,POOL,[r,handle],allow=can);assert tr==([('canrelocate',RES,handle)] if present else []) and eax&0xff==int(present and can);out.append((tr,eax&0xff))
 tr,_,_,_,_=run(codes,BASE_VALIDATE,POOL,[r]);assert not tr
 if present:tr,_,_,_,_=run(codes,VALIDATE,POOL,[RES]);assert tr==[('verify',RES)];out.append(tr)
 tr,_,_,pool,_=run(codes,FREE_RESOURCE,POOL,[handle]);assert tr==[('free',POOL,handle)] and pool==b'\xa5'*0x464;out.append(tr)
 selected=(f+t+z)%3;value=0x45670000+h
 def table(u,put):
  u.mem_write(HEADERS,b'\x6b'*0x60);put(POOL+0x438,INFOS)
  for i in range(3):put(INFOS+4*i,HEADERS+0x20*i)
 tr,_,_,_,headers=run(codes,SET,POOL,[selected,value],setup=table);want=bytearray(b'\x6b'*0x60);struct.pack_into('<I',want,0x20*selected,value);assert not tr and headers==bytes(want);out.append(headers)
 return out
def compare(candidate,native):
 cases=0
 for f in range(5):
  for t in range(5):
   for z in range(5):
    for present in range(2):
     for can in range(2):
      for h in range(4):assert scenario(candidate,f,t,z,present,can,h)==scenario(native,f,t,z,present,can,h);cases+=1
 return cases
