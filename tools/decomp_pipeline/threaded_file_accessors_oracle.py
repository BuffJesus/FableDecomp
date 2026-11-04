"""Retail file field reads and global worker wake, with full storage/ABI guards."""
import hashlib,itertools,struct,tempfile
from pathlib import Path
from movable_pool_construct_oracle import compile_codes
from pe_oracle import EXE,read_virtual_data
from unicorn import Uc,UC_ARCH_X86,UC_MODE_32,UC_HOOK_CODE
from unicorn.x86_const import *
R=Path(__file__).resolve().parents[2]
SOURCE=R/'rebuild/src/compiled/00/98/CThreadedFileManager_SetDeviceTimeSlice_0098e170.cpp'
ROOTS=[dict(name=n,source=SOURCE,symbol=s,address=a,size=z,instructions=i,relocations=r) for n,s,a,z,i,r in (
 ('is_open_for_write','?IsOpenForWrite@CThreadedFile@@QBE_NXZ',0x98df40,4,2,0),
 ('peek_device_id','?PeekDeviceID@CThreadedFile@@QBE?AW4EThreadedFileDeviceID@@XZ',0x98df50,4,2,0),
 ('peek_physical_sort_key','?PeekPhysicalSortKey@CThreadedFile@@QBEKXZ',0x98df60,4,2,0),
 ('file_keep_updating','?KeepUpdating@CThreadedFile@@QAEXXZ',0x98e1c0,23,7,3))]
ADDRESS={'?g_FableThreadedFileManager_013BC870@@3VCThreadedFileManager@@A':0x13bc870,
 '__imp__SetEvent@4':0x143fde0,'__imp__SleepEx@8':0x143fddc}
ARENA,OBJ,GLOBAL,STACK,STOP,API=0x2000000,0x2000103,0x13bc000,0x3000000,0x4000000,0x5000000

def execute(codes,root,value,seed=0,event_result=1,sleep_result=0):
 u=Uc(UC_ARCH_X86,UC_MODE_32)
 for a,n in ((0x98d000,0x2000),(0x143f000,0x1000),(ARENA,0x1000),(GLOBAL,0x1000),(STACK,0x10000),(STOP,0x1000),(API,0x1000)):u.mem_map(a,n)
 for a,b in codes.items():u.mem_write(a,b)
 def put(a,v):u.mem_write(a,struct.pack('<I',v&0xffffffff))
 def get(a):return struct.unpack('<I',u.mem_read(a,4))[0]
 for a in (ARENA,GLOBAL):u.mem_write(a,bytes((i*37+seed*19)&255 for i in range(0x1000)))
 name=root['name'];wake=name=='file_keep_updating'
 if wake:put(0x13bc9d4,value)
 elif name=='is_open_for_write':u.mem_write(OBJ+0x18,bytes([value]))
 else:put(OBJ+(4 if name=='peek_device_id' else 0x14),value)
 expected={a:bytearray(u.mem_read(a,0x1000)) for a in (ARENA,GLOBAL)};trace=[]
 put(0x143fde0,API);put(0x143fddc,API+16)
 def hook(uc,a,size,_):
  if a not in (API,API+16):return
  sp=uc.reg_read(UC_X86_REG_ESP)
  if a==API:
   assert wake and not trace and get(sp+4)==value
   trace.append(('set_event',value));pop=4;result=event_result
   # The next call must survive caller-saved register clobbers and a changed global.
   put(0x13bc9d4,value^0xffffffff)
   struct.pack_into('<I',expected[GLOBAL],0x9d4,value^0xffffffff)
  else:
   assert trace==[('set_event',value)] and (get(sp+4),get(sp+8))==(0,1)
   trace.append(('sleep_ex',0,1));pop=8;result=sleep_result
  uc.reg_write(UC_X86_REG_EAX,result);uc.reg_write(UC_X86_REG_ECX,0xdeadbeef);uc.reg_write(UC_X86_REG_EDX,0x87654321)
  uc.reg_write(UC_X86_REG_EIP,get(sp));uc.reg_write(UC_X86_REG_ESP,sp+4+pop)
 u.hook_add(UC_HOOK_CODE,hook)
 sp=STACK+0x8000;put(sp,STOP);saved=(UC_X86_REG_EBX,UC_X86_REG_ESI,UC_X86_REG_EDI,UC_X86_REG_EBP)
 for i,reg in enumerate(saved):u.reg_write(reg,0x55660000+seed+i)
 u.reg_write(UC_X86_REG_ECX,0 if wake else OBJ);u.reg_write(UC_X86_REG_EAX,0xabc12300);u.reg_write(UC_X86_REG_ESP,sp)
 u.emu_start(root['address'],STOP,count=100)
 assert u.reg_read(UC_X86_REG_EIP)==STOP and u.reg_read(UC_X86_REG_ESP)==sp+4
 assert all(u.reg_read(reg)==0x55660000+seed+i for i,reg in enumerate(saved))
 assert all(bytes(u.mem_read(a,0x1000))==bytes(b) for a,b in expected.items())
 result=u.reg_read(UC_X86_REG_EAX)
 if wake:assert trace==[('set_event',value),('sleep_ex',0,1)]
 else:assert not trace and (result&255 if name=='is_open_for_write' else result)==value
 return result,trace

def compare_accessors():
 import re
 inputs={Path(__file__),EXE,R/'tools/decomp_pipeline/movable_pool_construct_oracle.py'}
 def closure(p):
  if p in inputs:return
  inputs.add(p)
  for name in re.findall(r'^\s*#include\s*"([^"]+)"',p.read_text(encoding='utf-8'),re.M):
   q=next(q for q in (p.parent/name,R/'rebuild/include'/name) if q.is_file());closure(q)
 closure(SOURCE);pins={str(p):hashlib.sha256(p.read_bytes()).hexdigest() for p in inputs}
 with tempfile.TemporaryDirectory(prefix='threaded-file-accessors-') as tmp:built=compile_codes(ROOTS,ADDRESS,Path(tmp))
 raw=EXE.read_bytes();native={r['address']:read_virtual_data(raw,r['address'],r['size']) for r in ROOTS}
 assert built==native;cases=0
 values=(0,1,2,0x7fffffff,0x80000000,0xfffffffe,0xffffffff,0x12345678)
 for root in ROOTS:
  if root['name']=='file_keep_updating':params=itertools.product((0,0x12345678,0xffffffff),range(3),(0,1),(0,0xc0,0xffffffff))
  else:params=((v,s,1,0) for v in (range(256) if root['name']=='is_open_for_write' else values) for s in range(3))
  for args in params:
   assert execute(built,root,*args)==execute(native,root,*args);cases+=1
  detected=False
  try:execute({**built,root['address']:b'\xcc'},root,0)
  except Exception:detected=True
  assert detected,root['name']
 assert all(hashlib.sha256(Path(p).read_bytes()).hexdigest()==h for p,h in pins.items())
 return dict(status='PASS',cases=cases,missing_provider_checks=4,input_sha256=pins,
  boundaries=['Synthetic file storage, including raw bool bytes and unsigned field patterns; no file ownership or open operation.',
  'SetEvent and SleepEx controlled; global manager event changes during SetEvent, file this is null for wake; no actual worker thread.'])
