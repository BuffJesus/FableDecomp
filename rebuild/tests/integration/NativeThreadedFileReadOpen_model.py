"""Whole read-open ABI/state probe; helpers and Windows APIs remain controlled."""
import itertools,struct,sys,json,hashlib
from pathlib import Path
R=Path(__file__).resolve().parents[3];sys.path.insert(0,str(R/'tools/decomp_pipeline'))
from unicorn import Uc,UC_ARCH_X86,UC_MODE_32,UC_HOOK_CODE
from unicorn.x86_const import *
from pe_oracle import EXE,read_virtual_data
from threaded_file_open_counters_oracle import relocated
ENTRY=0x98e1e0
A,O,N,P,G,S,STOP,API=0x2000000,0x2000103,0x2000300,0x2000400,0x13bc000,0x3000000,0x4000000,0x5000000

def execute(code,args):
 cache,drive,handle,length,counter,mutation,seed=args
 u=Uc(UC_ARCH_X86,UC_MODE_32)
 for a,n in ((0x98e000,0x1000),(0x997000,0x1000),(0x999000,0x1000),(0x99a000,0x2000),(0x99e000,0x1000),(0x411000,0x1000),(0x122d000,0x1000),(0x129a000,0x1000),(0x143f000,0x1000),(A,0x4000),(G,0x1000),(S,0x10000),(STOP,0x1000),(API,0x1000)):u.mem_map(a,n)
 u.mem_write(ENTRY,code)
 def put(a,v):u.mem_write(a,struct.pack('<I',v&0xffffffff))
 def get(a):return struct.unpack('<I',u.mem_read(a,4))[0]
 for a,n in ((A,0x4000),(G,0x1000)):u.mem_write(a,bytes((i*37+seed*19)&255 for i in range(n)))
 put(G+0x9ec,counter);expected={a:bytearray(u.mem_read(a,n)) for a,n in ((A,0x4000),(G,0x1000))}
 def model(a,v):
  base=A if A<=a<A+0x4000 else G
  struct.pack_into('<I',expected[base],a-base,v&0xffffffff)
 def change(a,v):put(a,v);model(a,v)
 def flag(off,v):expected[A][O+off-A]=v
 def verify():assert all(bytes(u.mem_read(a,len(b)))==bytes(b) for a,b in expected.items()),trace
 put(0x143fe2c,API);put(0x143fdf0,API+16)
 u.mem_write(0x122d70e,b'\0\0\0');u.mem_write(0x129a15c,b'D:\0')
 sp=S+0x8000;put(sp,STOP);put(sp+4,N);put(sp+8,cache)
 full,drv,upper,narrow=sp-12,sp-16,sp-8,sp-4
 trace=[]
 hooks={0x999110,0x99aed0,0x997620,0x99b2c0,0x99ee20,0x4115a0,0x99eae0,0x99af00,0x99b7d0,0x99b510,API,API+16}
 def hook(uc,a,size,_):
  if a not in hooks:return
  esp=uc.reg_read(UC_X86_REG_ESP);cx=uc.reg_read(UC_X86_REG_ECX);pop=0;result=0xbad00000
  if a==0x999110:
   assert not trace and cx==full and uc.reg_read(UC_X86_REG_EDX)==N
   verify();put(full,0x11111111);result=full;trace.append('convert')
  elif a==0x99aed0:
   assert trace==['convert'] and cx==drv;put(drv,0);result=drv;trace.append('default')
  elif a==0x997620:
   assert trace[-1]=='default' and cx==full and uc.reg_read(UC_X86_REG_EDX)==drv
   assert [get(esp+i) for i in (4,8,12)]==[0,0,0]
   put(drv,0x22222222);pop=12;flag(0x18,0);trace.append('split')
  elif a==0x99b2c0:
   assert trace[-1]=='split' and cx==drv and get(esp+4)==narrow
   verify();put(narrow,0x33333333);result=narrow;pop=4;trace.append('narrow')
  elif a==0x99ee20:
   assert trace[-1]=='narrow' and cx==narrow and get(esp+4)==upper
   put(upper,0 if drive==0 else A+0x900);result=upper;pop=4;trace.append('upper')
  elif a==0x4115a0:
   assert trace[-1]=='upper' and drive!=0 and cx==A+0x900 and get(esp+4)==0x129a15c
   result=int(drive==2);pop=4;trace.append('equal')
  elif a==0x99eae0:
   assert cx==(narrow if trace[-1]=='destroy-upper' else upper)
   if cx==upper:assert trace[-1]==('upper' if drive==0 else 'equal');trace.append('destroy-upper')
   else:
    trace.append('destroy-narrow');model(O+4,int(drive==2))
   put(cx,0)
  elif a==0x99af00:
   assert trace[-1]=='destroy-narrow' and cx==full
   verify();result=P;trace.append('pointer')
  elif a==API:
   assert trace[-1]=='pointer'
   assert tuple(get(esp+4+i*4) for i in range(7))==(P,0x80000000,1,0,3,0x60000001 if cache else 0x40000001,0)
   verify();result=handle;pop=28;trace.append('create');model(O+8,handle)
  elif a==0x99b7d0:
   assert trace[-1]=='create' and handle!=0xffffffff and cx==O+12 and get(esp+4)==N
   verify();change(O+12,0x44444444);result=cx;pop=4;trace.append('assign')
   if mutation==1:change(O+8,0x12345555)
  elif a==API+16:
   assert trace[-1]=='assign' and (get(esp+4),get(esp+8))==(get(O+8),0)
   verify();result=length;pop=8;trace.append('size')
   if mutation==2:change(G+0x9ec,counter^0xffffffff)
   next_key=(get(G+0x9ec)+1)&0xffffffff
   model(O+16,length);model(O+20,next_key);model(G+0x9ec,next_key);flag(0x19,1)
  elif a==0x99b510:
   if cx==drv:assert trace[-1]==('create' if handle==0xffffffff else 'size');trace.append('destroy-drive')
   else:assert cx==full and trace[-1]=='destroy-drive';trace.append('destroy-full')
   verify();put(cx,0)
   if mutation==3:change(O+16,0xcafebabe)
  uc.reg_write(UC_X86_REG_EAX,result);uc.reg_write(UC_X86_REG_ECX,0xdeadbeef);uc.reg_write(UC_X86_REG_EDX,0x87654321)
  uc.reg_write(UC_X86_REG_EIP,get(esp));uc.reg_write(UC_X86_REG_ESP,esp+4+pop)
 u.hook_add(UC_HOOK_CODE,hook)
 saved=(UC_X86_REG_EBX,UC_X86_REG_ESI,UC_X86_REG_EDI,UC_X86_REG_EBP)
 for i,reg in enumerate(saved):u.reg_write(reg,0x55660000+seed+i)
 u.reg_write(UC_X86_REG_ECX,O);u.reg_write(UC_X86_REG_ESP,sp)
 u.emu_start(ENTRY,STOP,count=250)
 assert u.reg_read(UC_X86_REG_EIP)==STOP and u.reg_read(UC_X86_REG_ESP)==sp+12
 assert all(u.reg_read(reg)==0x55660000+seed+i for i,reg in enumerate(saved))
 result=u.reg_read(UC_X86_REG_EAX)&255;assert result==int(handle!=0xffffffff)
 assert trace[-1]=='destroy-full';verify()
 return result,trace,{a:bytes(b) for a,b in expected.items()}

def compare_read_open(obj):
 original=read_virtual_data(EXE.read_bytes(),ENTRY,310)
 built,roots=relocated(obj);built=built[ENTRY];count=0
 for args in itertools.product((0,1,255),range(3),(0xffffffff,0,0x12345678),(0,37,0xffffffff),(0,0xffffffff),range(4),(0,7)):
  assert execute(built,args)==execute(original,args);count+=1
 controls=[('write-flag',0x98e221,b'\x01'),('wrong-share',0x98e292,b'\x00'),
  ('cache-flags',0x98e286,struct.pack('<I',0x40000001)),
  ('wrong-access',0x98e294,struct.pack('<I',0x40000000)),
  ('missing-filename',0x98e2d4,b'\x90'*5),
  ('shared-sort-counter',0x98e2e9,struct.pack('<I',0x13bc9e8)),
  ('missing-open-flag',0x98e2fd,b'\x00'),('false-success',0x98e30e,b'\x00')]
 rejected=[]
 for name,address,replacement in controls:
  changed=bytearray(built);off=address-ENTRY;changed[off:off+len(replacement)]=replacement
  assert bytes(changed)!=built
  try:execute(bytes(changed),(1,2,0x12345678,37,0xffffffff,0,7))
  except AssertionError:rejected.append(name)
  else:raise AssertionError('Undetected '+name)
 return dict(status='PASS',cases=count,controls=rejected,roots=roots,
  boundary='Whole read-open ABI, 20KiB state, ordered cleanup and raw bool/counter/failure paths checked against original. Path/string helpers and Win32 APIs controlled; no actual opening, exception unwinding or production ownership claim.')
