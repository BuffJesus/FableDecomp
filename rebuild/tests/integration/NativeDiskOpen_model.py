"""Disk Open successful returns; helpers/OS controlled, no exception claim."""
from pathlib import Path
import sys,struct,itertools,json
R=Path(__file__).resolve().parents[3];sys.path.insert(0,str(R/'tools/decomp_pipeline'))
from unicorn import Uc,UC_ARCH_X86,UC_MODE_32,UC_HOOK_CODE
from unicorn.x86_const import *
from pe_oracle import EXE,read_virtual_data
from gate_game_component_abi import Coff
ENTRY=0x99a6a0;A=0x2000000;OBJ=A+0x103;NAME=A+0x400;PATH=A+0x600;STACK=0x3000000;STOP=0x4000000;API=0x5000000
def execute(code,args):
 mode,flags,handle,length,mutation,seed=args;u=Uc(UC_ARCH_X86,UC_MODE_32)
 for a,n in ((0x99a000,0x2000),(0x999000,0x1000),(0x143f000,0x1000),(A,0x4000),(STACK,0x10000),(STOP,0x1000),(API,0x1000)):u.mem_map(a,n)
 u.mem_write(ENTRY,code)
 def put(a,v):u.mem_write(a,struct.pack('<I',v&0xffffffff))
 def get(a):return struct.unpack('<I',u.mem_read(a,4))[0]
 u.mem_write(A,bytes((i*37+seed*19)&255 for i in range(0x4000)));expected=bytearray(u.mem_read(A,0x4000));trace=[]
 def model(a,v):struct.pack_into('<I',expected,a-A,v&0xffffffff)
 def change(a,v):put(a,v);model(a,v)
 def verify():assert bytes(u.mem_read(A,0x4000))==bytes(expected),trace
 for address,target in ((0x143fe2c,API),(0x143fdf0,API+16),(0x143fde8,API+32)):put(address,target)
 sp=STACK+0x8000;local=sp-24;put(sp,STOP);put(sp+4,NAME);put(sp+8,mode);put(sp+12,flags)
 hooks={0x99aed0,0x99b7d0,0x999140,0x99b220,0x999970,0x999620,0x99af00,0x99b510,API,API+16,API+32};real_handle=handle if mode in (1,2,3,4) else 0
 def hook(uc,a,size,_):
  if a not in hooks:return
  esp=uc.reg_read(UC_X86_REG_ESP);cx=uc.reg_read(UC_X86_REG_ECX);pop=0;result=0xbad00000
  if a==0x99aed0:
   assert not trace and cx==local;verify();put(local,0);result=cx;trace.append('default')
  elif a==0x99b7d0:
   assert trace[-1]=='default' and cx==OBJ+24 and get(esp+4)==NAME;verify();change(OBJ+24,0xcafebabe);result=cx;pop=4;trace.append('assign');model(OBJ+16,0)
  elif a==0x999140:
   assert trace[-1]=='assign' and cx==NAME and uc.reg_read(UC_X86_REG_EDX)==local;verify();put(local,0x12345678);result=0;trace.append('convert')
   if mutation==1:change(OBJ+16,0xaabbccdd);change(OBJ+4,0x55667788)
  elif a==0x99b220:
   assert trace[-1]=='convert' and cx==local;verify();result=23;trace.append('length')
  elif a==0x999970:
   assert mode in (2,3,4) and trace[-1]=='length' and cx==local;verify();trace.append('validate')
  elif a==0x999620:
   assert mode==4 and trace[-1]=='validate' and cx==local;verify();trace.append('directory')
  elif a==0x99af00:
   assert trace[-1]==('length' if mode==1 else 'directory' if mode==4 else 'validate') and cx==local;verify();result=PATH;trace.append('pointer')
  elif a==API:
   assert trace[-1]=='pointer';desired=0x80000000 if mode==1 else 0xc0000000;share=int(mode in (1,2));create=3 if mode in (1,2) else 4 if mode==3 else 2
   assert tuple(get(esp+4+i*4) for i in range(7))==(PATH,desired,share,0,create,0x8000000 if flags&2 else 0,0);verify();result=handle;pop=28;trace.append('create');model(OBJ+4,mode if mode<=2 else 2)
  elif a==API+16:
   assert trace[-1]==('create' if mode in (1,2,3,4) else 'length') and (get(esp+4),get(esp+8))==(real_handle,0);verify();result=length;pop=8;trace.append('size');model(OBJ+12,length)
  elif a==API+32:
   end=trace[-1]=='seek-begin';assert trace[-1] in ('size','seek-begin')
   assert tuple(get(esp+4+i*4) for i in range(4))==(real_handle,0,0,int(end));verify();result=0xffffffff;pop=16;trace.append('seek-current' if end else 'seek-begin')
   if not end:
    if mutation==2:change(OBJ+12,0x88888888)
    model(OBJ+16,real_handle);model(OBJ+8,0);expected[OBJ+20-A]=1
  elif a==0x99b510:
   assert trace[-1]=='seek-current' and cx==local and get(local)==0x12345678;verify();put(local,0);trace.append('destroy')
   if mutation==3:change(OBJ+8,0xabcdef01);change(OBJ+16,0x76543210)
  uc.reg_write(UC_X86_REG_EAX,result);uc.reg_write(UC_X86_REG_ECX,0xdeadbeef);uc.reg_write(UC_X86_REG_EDX,0x87654321);uc.reg_write(UC_X86_REG_EIP,get(esp));uc.reg_write(UC_X86_REG_ESP,esp+4+pop)
 u.hook_add(UC_HOOK_CODE,hook);saved=(UC_X86_REG_EBX,UC_X86_REG_ESI,UC_X86_REG_EDI,UC_X86_REG_EBP)
 for i,reg in enumerate(saved):u.reg_write(reg,0x55660000+seed+i)
 u.reg_write(UC_X86_REG_ECX,OBJ);u.reg_write(UC_X86_REG_ESP,sp);u.emu_start(ENTRY,STOP,count=250)
 assert u.reg_read(UC_X86_REG_EIP)==STOP and u.reg_read(UC_X86_REG_ESP)==sp+16
 assert all(u.reg_read(reg)==0x55660000+seed+i for i,reg in enumerate(saved));assert trace[-1]=='destroy';verify();return trace,bytes(expected)
def compare_disk_open(built):
 original=read_virtual_data(EXE.read_bytes(),ENTRY,640);assert built==original;cases=0
 for args in itertools.product((0,1,2,3,4,5,0xffffffff),(0,2,3,0x80000002),(0,0x12345678),(0,0xffffffff),range(4),(0,7)):
  assert execute(built,args)==execute(original,args);cases+=1
 specs=[('wrong-read-access',0x99a765,struct.pack('<I',0x40000000),1),('missing-sequential-flag',0x99a6cb,b'\0'*4,1),('invalid-enum-handle',0x99a8c9,b'\x90'*2,5),('wrong-seek-origin',0x99a8db,b'\x01',1),('wrong-position',0x99a8f3,struct.pack('<I',1),1),('missing-length',0x99a8e1,b'\x90'*3,1),('missing-wide-cleanup',0x99a901,b'\x90'*5,1)]
 detected=[]
 for name,address,replacement,mode in specs:
  bad=bytearray(built);off=address-ENTRY;bad[off:off+len(replacement)]=replacement;assert bytes(bad)!=built
  try:execute(bytes(bad),(mode,2,0x12345678,37,0,7))
  except AssertionError:detected.append(name)
  else:raise AssertionError(name)
 return dict(status='PASS',cases=cases,model_controls=detected,boundary='Success returns only: all access enums, null handles, cache flags, helper mutations, seek API failures and full16KiB state/ABI. Helpers and OS controlled; exception paths covered separately by Windows host.')
