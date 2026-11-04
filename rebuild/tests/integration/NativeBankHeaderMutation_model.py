"""Bounded retail header state/ABI oracle; stream, callbacks and callees controlled."""
from pathlib import Path
import sys,struct,itertools,json,hashlib
R=Path(__file__).resolve().parents[3];sys.path.insert(0,str(R/'tools/decomp_pipeline'))
from pe_oracle import EXE,read_virtual_data
from unicorn import Uc,UC_ARCH_X86,UC_MODE_32,UC_HOOK_CODE
from unicorn.x86_const import *
ENTRY=0x9d0880;A=0x2000000;BANK=A+0x100;FILE=A+0x400;VT=A+0x800;DATA=A+0x1000;STACK=0x3000000;STOP=0x4000000;API=0x5000000
image=EXE.read_bytes();code=read_virtual_data(image,ENTRY,1635)
helpers={0x99b2c0,0x994700,0x993bc0,0x993ca0,0x994780,0x99eae0,0xbfe9f9,0x9cfbc0,0x9d0380,0x99ebf0,API,API+16,API+32}

def execute(args):
 readonly,state,alignment,chunk,result,callback=args[:6]
 validmagic,version,format,streamsize=args[6:10] if len(args)>6 else (True,7,19,32)
 mutation=args[10]
 u=Uc(UC_ARCH_X86,UC_MODE_32)
 pages={a&~4095 for a in helpers}|{0x9d0000,0x129b000,0x139c000,STOP,API}
 for p in pages:u.mem_map(p,4096)
 for p,n in ((A,0x4000),(STACK,0x10000)):u.mem_map(p,n)
 u.mem_write(ENTRY,code);u.mem_write(0x129b000,read_virtual_data(image,0x129b000,4096))
 def put(a,v):u.mem_write(a,struct.pack('<I',v&0xffffffff))
 def get(a):return struct.unpack('<I',u.mem_read(a,4))[0]
 put(BANK,VT);put(BANK+0x7c,FILE);put(BANK+0x98,16);put(BANK+0x9c,7);u.mem_write(BANK+0x8d,bytes([readonly]));put(FILE,VT+0x100)
 put(VT+0x20,API);put(VT+0x124,API+16);put(BANK+0xec,A+0x600 if callback else 0);put(A+0x600,VT+0x200);put(VT+0x208,API+32)
 magic=get(0x129b5a4) if validmagic else 0;u.mem_write(DATA,struct.pack('<8I',magic,version,format,state,alignment,123,456,128))
 sp=STACK+0x8000;put(sp,STOP);put(sp+4,0);trace=[];stream=None;initial=bytes(u.mem_read(BANK,0x110))
 def hook(uc,a,size,_):
  nonlocal stream
  if a not in helpers:
   assert ENTRY<=a<ENTRY+len(code),('unexpected code',hex(a))
   return
  esp=uc.reg_read(UC_X86_REG_ESP);cx=uc.reg_read(UC_X86_REG_ECX);pop=0;value=0xdecafbad
  if a==0x99b2c0:
   assert cx==BANK+0x84;value=get(esp+4);put(value,0);pop=4;trace.append('filename')
  elif a==0x994700:
   assert get(esp+4)==FILE and get(esp+8)==0x4000;stream=cx;u.mem_write(cx,b'\0'*36);put(cx+8,streamsize);value=cx;pop=8;trace.append('stream-ctor')
  elif a==0x993bc0:
   assert cx==stream;p=get(esp+4);put(cx+4,p);put(cx+12,DATA+p);put(cx+20,chunk if p==0 else 0);pop=4;trace.append(('seek',p))
  elif a==0x993ca0:
   assert cx==stream and get(esp+8)==4;p=get(cx+4);u.mem_write(get(esp+4),bytes(u.mem_read(DATA+p,4)));put(cx+4,p+4);put(cx+12,DATA+p+4);put(cx+20,0);pop=8;trace.append(('overflow',p))
  elif a==API:assert cx==BANK;value=19;trace.append('format')
  elif a==API+16:
   assert cx==FILE;value=0x9000;trace.append('length')
   if mutation==1:put(BANK+0xa0,77);put(BANK+0x98,1234);put(BANK+0xa4,5678)
  elif a==0x9cfbc0:
   assert cx==BANK and (get(esp+4),get(esp+8),get(esp+12))==(stream,123,456);assert get(stream+4)==128;pop=12;value=result;trace.append('entries')
   if mutation==3:put(BANK+0xa8,0x76543210);u.mem_write(BANK+0x94,b'\1')
  elif a==0x9d0380:
   assert cx==BANK and get(esp+4)==state;pop=4;value=result;trace.append('recovery')
   if mutation==2:put(BANK+0xa0,99);put(BANK+0x7c,0x11223344)
  elif a==0x99ebf0:
   assert get(esp+8)==0xffffffff;put(cx,0);pop=8;value=cx;trace.append(('message',get(esp+4)))
  elif a==API+32:assert cx==A+0x600;pop=4;trace.append('callback')
  elif a==0x994780:assert cx==stream;trace.append('stream-dtor')
  elif a==0x99eae0:trace.append('char-dtor')
  elif a==0xbfe9f9:
   assert cx==0;value=uc.reg_read(UC_X86_REG_EAX);trace.append('cookie')
  else:raise AssertionError(hex(a))
  uc.reg_write(UC_X86_REG_EAX,value);uc.reg_write(UC_X86_REG_ECX,0xdeadbeef);uc.reg_write(UC_X86_REG_EDX,0x87654321);uc.reg_write(UC_X86_REG_EIP,get(esp));uc.reg_write(UC_X86_REG_ESP,esp+4+pop)
 u.hook_add(UC_HOOK_CODE,hook)
 saved=(UC_X86_REG_EBX,UC_X86_REG_ESI,UC_X86_REG_EDI,UC_X86_REG_EBP)
 for i,reg in enumerate(saved):u.reg_write(reg,0x55660000+i)
 u.reg_write(UC_X86_REG_ESP,sp);u.reg_write(UC_X86_REG_ECX,BANK);u.emu_start(ENTRY,STOP,count=2000)
 assert u.reg_read(UC_X86_REG_EIP)==STOP and u.reg_read(UC_X86_REG_ESP)==sp+8
 assert all(u.reg_read(reg)==0x55660000+i for i,reg in enumerate(saved))
 expected=bytearray(initial);badmagic=not streamsize or not validmagic;badversion=version!=7 or format!=19;early=badmagic or badversion or (not readonly and alignment!=16)
 if readonly and not badmagic and not badversion:struct.pack_into('<I',expected,0x98,alignment)
 if not early:
  struct.pack_into('<I',expected,0xa0,32);struct.pack_into('<I',expected,0xa8,0x9000)
 if not early and mutation==1:
  struct.pack_into('<I',expected,0xa0,77);struct.pack_into('<I',expected,0x98,1234);struct.pack_into('<I',expected,0xa4,5678)
 if 'recovery' in trace and mutation==2:
  struct.pack_into('<I',expected,0xa0,99);struct.pack_into('<I',expected,0x7c,0x11223344)
 if 'entries' in trace and mutation==3:
  struct.pack_into('<I',expected,0xa8,0x76543210);expected[0x94]=1
 assert bytes(u.mem_read(BANK,0x110))==expected,(args,trace)
 answer=u.reg_read(UC_X86_REG_EAX)&255
 wanted=0 if badmagic or (badversion and readonly) else (1 if early or (not readonly and state!=1) else (result if state==1 else 0))
 assert answer==wanted,(args,answer,wanted,trace)
 assert ('entries' in trace)==(not early and state==1)
 assert ('recovery' in trace)==(not early and not readonly and state!=1)
 assert trace[-3:]==['stream-dtor','char-dtor','cookie'],trace
 return dict(args=args,result=answer,trace=trace)

