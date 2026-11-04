"""Bounded bank read-open state/ABI oracle; file/header providers controlled."""
from pathlib import Path
import sys,struct,itertools,json
R=Path(__file__).resolve().parents[3];sys.path.insert(0,str(R/'tools/decomp_pipeline'))
from unicorn import Uc,UC_ARCH_X86,UC_MODE_32,UC_HOOK_CODE
from unicorn.x86_const import *
from gate_game_component_abi import Coff
from pe_oracle import EXE,read_virtual_data
ENTRY=0x9d1090;A=0x2000000;BANK=A+0x100;NAME=A+0x400;FILE=A+0x800;INFO=A+0xa00;VTABLE=A+0xc00;STACK=0x3000000;STOP=0x4000000;API=0x5000000
ADDRESS={'?PathExists@CAFile@@SI_NABVCWideString@@@Z':0x999230,'??2@YAPAXI@Z':0xbfea1a,'??0CBaseClassNonCopyable@@QAE@XZ':0x99a2f0,'??_7CDiskFileWin32@@6B@':0x122d06c,'??0CWideString@@QAE@XZ':0x99aed0,'?Reset@?$CCountedPointer@VCDiskFileWin32@@@@QAEXPAVCDiskFileWin32@@@Z':0x9d1ee0,'??4CWideString@@QAEAAV0@ABV0@@Z':0x99b7d0,'?ReadFileInformation@CBankFile@@AAE_N_N@Z':0x9d0880,'?CloseBankFileImpl@@YIXPAVCBankFile@@@Z':0x9cd850}
def relocated(obj):
 c=Coff(obj);s,o=c.symbols['?OpenFileReadOnly@CBankFile@@UAE_NABVCWideString@@K@Z'];body,rels=c.sections[s];assert o==0 and len(body)==204
 result=bytearray(body)
 for site,(name,kind) in rels.items():struct.pack_into('<I',result,site,(ADDRESS[name]+struct.unpack_from('<i',body,site)[0]-(ENTRY+site+4 if kind==20 else 0))&0xffffffff)
 return bytes(result)
def execute(code,args):
 exists,loaded,flags,mutation,seed=args;u=Uc(UC_ARCH_X86,UC_MODE_32)
 for a,n in ((0x9d0000,0x2000),(0x999000,0x3000),(0x9cd000,0x1000),(0xbfe000,0x1000),(0x122d000,0x1000),(A,0x4000),(STACK,0x10000),(STOP,0x1000),(API,0x1000)):u.mem_map(a,n)
 u.mem_write(ENTRY,code)
 def put(a,v):u.mem_write(a,struct.pack('<I',v&0xffffffff))
 def get(a):return struct.unpack('<I',u.mem_read(a,4))[0]
 u.mem_write(A,bytes((i*37+seed*19)&255 for i in range(0x4000)));put(BANK,VTABLE);put(VTABLE+44,API+16);put(0x122d070,API)
 expected=bytearray(u.mem_read(A,0x4000))
 def model(a,v):struct.pack_into('<I',expected,a-A,v&0xffffffff)
 def change(a,v):put(a,v);model(a,v)
 def byte(a,v):expected[a-A]=v
 def verify():assert bytes(u.mem_read(A,0x4000))==bytes(expected),trace
 model(BANK+0x78,flags);model(BANK+0x90,0)
 for off in (0x8c,0x8e,0x8f,0x94,0x95):byte(BANK+off,0)
 byte(BANK+0x8d,1)
 sp=STACK+0x8000;put(sp,STOP);put(sp+4,NAME);put(sp+8,flags);trace=[]
 def hook(uc,a,size,_):
  if a not in set(ADDRESS.values())|{API,API+16}:return
  esp=uc.reg_read(UC_X86_REG_ESP);cx=uc.reg_read(UC_X86_REG_ECX);pop=0;result=0xbad00000
  if a==0x999230:
   assert not trace and cx==NAME;verify();trace.append('exists');result=exists
   if mutation==1:change(BANK+0x78,0x87654321);change(BANK+0x90,0x11223344)
  elif a==0xbfea1a:
   assert exists and trace[-1]=='exists' and get(esp+4)==28;verify();result=FILE;trace.append('allocate')
  elif a==0x99a2f0:
   assert trace[-1]=='allocate' and cx==FILE;verify();change(FILE,0x129a7c4);result=FILE;trace.append('base');model(FILE,0x122d06c)
  elif a==0x99aed0:
   assert trace[-1]=='base' and cx==FILE+24;verify();change(FILE+24,0);result=cx;trace.append('wide');byte(FILE+20,0)
  elif a==0x9d1ee0:
   assert trace[-1]=='wide' and cx==BANK+0x7c and get(esp+4)==FILE;verify();change(BANK+0x7c,FILE);change(BANK+0x80,INFO);pop=4;trace.append('reset')
  elif a==API:
   assert trace[-1]=='reset' and cx==FILE and tuple(get(esp+i) for i in (4,8,12))==(NAME,1,4);verify();pop=12;trace.append('open')
   if mutation==2:change(BANK+0x78,0xfedcba98);change(BANK+0x90,0x55667788)
  elif a==0x99b7d0:
   assert trace[-1]=='open' and cx==BANK+0x84 and get(esp+4)==NAME;verify();change(BANK+0x84,0xcafebabe);result=cx;pop=4;trace.append('assign')
  elif a==0x9d0880:
   raw=((flags>>1)&0xffffff00)|((~(flags>>1))&1)
   assert trace[-1]=='assign' and cx==BANK and get(esp+4)==raw;verify();result=loaded;pop=4;trace.append('information')
   if mutation==3:change(BANK+0x90,0xdeadbeef)
  elif a==0x9cd850:
   assert not loaded and trace[-1]=='information' and cx==BANK;verify();change(BANK+0x7c,0);change(BANK+0x80,0);trace.append('close')
  elif a==API+16:
   assert loaded and trace[-1]=='information' and cx==BANK;verify();change(BANK+0x90,0x12345678);trace.append('notify')
  else:raise AssertionError(hex(a))
  uc.reg_write(UC_X86_REG_EAX,result);uc.reg_write(UC_X86_REG_ECX,0xdeadbeef);uc.reg_write(UC_X86_REG_EDX,0x87654321)
  uc.reg_write(UC_X86_REG_EIP,get(esp));uc.reg_write(UC_X86_REG_ESP,esp+4+pop)
 u.hook_add(UC_HOOK_CODE,hook)
 saved=(UC_X86_REG_EBX,UC_X86_REG_ESI,UC_X86_REG_EDI,UC_X86_REG_EBP)
 for i,reg in enumerate(saved):u.reg_write(reg,0x55660000+seed+i)
 u.reg_write(UC_X86_REG_ECX,BANK);u.reg_write(UC_X86_REG_ESP,sp);u.emu_start(ENTRY,STOP,count=200)
 assert u.reg_read(UC_X86_REG_EIP)==STOP and u.reg_read(UC_X86_REG_ESP)==sp+12
 assert all(u.reg_read(reg)==0x55660000+seed+i for i,reg in enumerate(saved));verify()
 result=u.reg_read(UC_X86_REG_EAX)&255;assert result==int(bool(exists) and bool(loaded))
 assert trace[-1]==('exists' if not exists else 'notify' if loaded else 'close')
 return result,trace,bytes(expected)
def compare_bank_open(obj):
 original=read_virtual_data(EXE.read_bytes(),ENTRY,204);built=relocated(obj);assert built==original;cases=0
 for args in itertools.product((0,1,255),(0,1,255),(0,1,2,3,0x80000002,0xffffffff),range(4),(0,7)):
  assert execute(built,args)==execute(original,args);cases+=1
 specs=[('read-only-state',0x9d10a7,b'\0',1),('wrong-open-flags-field',0x9d10aa,b'\x74',1),
  ('reversed-existence',0x9d10d6,b'\x75',1),('wrong-access',0x9d1115,b'\x02',1),
  ('missing-filename',0x9d1121,b'\x90'*5,1),('inverted-symbol-policy',0x9d112c,b'\x90'*2,1),
  ('missing-failure-close',0x9d1142,b'\x90'*5,0),('missing-notification',0x9d1151,b'\x90'*3,1),
  ('wrong-success-result',0x9d1157,b'\0',1)]
 detected=[]
 for name,address,replacement,loaded in specs:
  changed=bytearray(built);offset=address-ENTRY;changed[offset:offset+len(replacement)]=replacement;assert bytes(changed)!=built
  try:execute(bytes(changed),(1,loaded,0,0,7))
  except AssertionError:detected.append(name)
  else:raise AssertionError('Undetected '+name)
 c=Coff(obj);section,offset=c.symbols['?OpenFileReadOnly@CBankFile@@UAE_NABVCWideString@@K@Z'];body,rels=c.sections[section];assert len(rels)==9
 return dict(status='PASS',grade='MATCH',bytes=204,cases=cases,controls=detected,relocations=[dict(site=site,symbol=name,kind=kind,address=ADDRESS[name]) for site,(name,kind) in rels.items()],
  boundary='Full16KiB state and ABI, path absence/header success/failure, cleanup/notification and helper mutations. Helpers/virtual disk calls controlled. Null allocation and exception unwinding outside scope; no real bank header parsing claim.')
