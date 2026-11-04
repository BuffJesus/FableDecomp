"""Original/rebuilt constructor, registration, entry SetupRead, metadata and file submission."""
import hashlib,itertools,json,re,struct,sys
from datetime import datetime,timezone
from pathlib import Path
R=Path(__file__).resolve().parents[2];sys.path.insert(0,str(R/'tools/decomp_pipeline'))
import gate_native_loading_restart_tree as T
import gate_native_bank_async_buffer_constructor as C
import installer_pause_oracle as L
from movable_pool_construct_oracle import compile_codes
from pe_oracle import EXE,read_virtual_data
from unicorn import Uc,UC_ARCH_X86,UC_MODE_32,UC_HOOK_CODE
from unicorn.x86_const import *
OUT=R/'rebuild/build/native-bank-entry-setup-composition/00/9d/009d5970'/datetime.now(timezone.utc).strftime('%Y%m%dT%H%M%S%fZ');OUT.mkdir(parents=True)
roots,addresses=T.providers();keep={0x9d7cf0,0x9d74e0,0x9d7990,0x42971d,0x42951b,0x42955b}
roots=[r for r in roots if r['address'] in keep]
roots.extend(dict(source=R/'rebuild/src/compiled'/src,symbol=symbol,address=address) for src,symbol,address in (
 ('00/9d/CBankFileAsyncData_CBankFileAsyncData_009d5df0.cpp',C.ROOT,0x9d5d30),
 ('00/99/CBaseClass_CBaseClass_0099a2d0.cpp',C.BASE,0x99a2f0),
 ('00/a6/CCriticalSectionLock_Constructor_00a62920.cpp',C.LOCK,0xa62920),
 ('00/a6/CCriticalSectionLock_Destructor_00a629a0.cpp',C.UNLOCK,0xa629a0),
 ('00/9d/CBankFileAsyncData_StartRead_009d5650.cpp','?StartRead@CBankFileAsyncData@@AAEXXZ',0x9d5650),
 ('00/98/CThreadedFile_AddObject_0098f350.cpp','?AddObject@CThreadedFile@@QAEKABVCThreadedFileObjectDesc@@@Z',0x98f350)))
roots=[r for r in roots if r['address']!=0x9d5650]
roots.extend(dict(source=R/'rebuild/src/compiled'/src,symbol=symbol,address=address) for src,symbol,address in (
 ('00/9d/CBankFileAsyncEntry_SetupRead_009d5970.cpp','?SetupRead@CBankFileAsyncEntry@@AAEXKAAVCBankFileAsyncData@@KKKW4EThreadedFilePriority@@@Z',0x9d5970),
 ('00/9c/CBankFile_GetEntryDataOffset_009cc270.cpp','?GetEntryDataOffset@CBankFile@@QBEKK@Z',0x9cc270),
 ('00/9d/CBankFileAsync_GetAsyncEntry_009d58d0.cpp','?Align@CBankFile@@QBEKK@Z',0x9cbb90)))
addresses.update({'?SetupBuffer@CBankFileAsyncData@@AAE_N_N@Z':0x9d52d0,'?ReadData@CBankFile@@IBEXKPAXK@Z':0x9cbf60,'??ACPackedUIntArray@@QBEKK@Z':0xa629f0})
addresses.update(L.ADDRESS);addresses.update({r['symbol']:r['address'] for r in roots})
addresses.update({name:a for name,site,a,names,values in C.TABLES})
addresses.update({'??_7CBaseClassNonCopyable@@6B@':0x129a7c4,'?g_FableThreadedFileManager_013BC870@@3VCThreadedFileManager@@A':0x13bc870,'?AddObject@CThreadedFileManager@@QAEKABVCThreadedFileObjectDesc@@AAVCThreadedFile@@@Z':0x98f0f0})
inputs={Path(__file__),Path(T.__file__),Path(C.__file__),Path(L.__file__),EXE}
def closure(p):
 if p in inputs:return
 inputs.add(p)
 for name in re.findall(r'^\s*#include\s*"([^"]+)"',p.read_text(),re.M):
  q=next(q for q in (p.parent/name,R/'rebuild/include'/name) if q.is_file());closure(q)
for root in roots:closure(root['source'])
for name in ('movable_pool_construct_oracle.py','check_one.py','gate_game_component_abi.py','pe_oracle.py'):
 inputs.add(R/'tools/decomp_pipeline'/name)
import check_one as V
inputs.update(p for p in (V.VC/'include').rglob('*.h'))
inputs.update(V.VC/'bin'/name for name in ('cl.exe','link.exe','c1xx.dll','c2.dll','mspdb71.dll'))
pins={str(p):hashlib.sha256(p.read_bytes()).hexdigest() for p in inputs}
(OUT/'input-pins.json').write_text(json.dumps(pins,indent=2))
codes=compile_codes(roots,addresses,OUT);raw=EXE.read_bytes()
native={a:read_virtual_data(raw,a,79 if a==0x9d74e0 else 191 if a==0x9d5970 else len(b)) for a,b in codes.items()}
assert all(b==native[a] for a,b in codes.items() if a not in (0x9d74e0,0x9d5970))
A,N,BANK,HDR,OBJ,FILE,HEAP,STACK,STOP,API=0x2000000,0x10000,0x2000100,0x2000800,0x2001000,0x2004000,0x2008000,0x3000000,0x4000000,0x5000000
def run(bodies,order,seed):
 u=Uc(UC_ARCH_X86,UC_MODE_32)
 for page in {a&~4095 for a in set(codes)|{0xbfea0e,0x98f0f0,0x143fe0c,0x143fd30}}:u.mem_map(page,4096);u.mem_write(page,b'\xcc'*4096)
 for a,n in ((A,N),(STACK,0x10000),(STOP,0x1000),(API,0x1000)):u.mem_map(a,n)
 for a,b in bodies.items():u.mem_write(a,b)
 u.mem_write(A,bytes((i*37+seed*19)&255 for i in range(N)))
 def put(a,v):u.mem_write(a,struct.pack('<I',v&0xffffffff))
 def get(a):return struct.unpack('<I',u.mem_read(a,4))[0]
 put(BANK+0x128,HDR);put(BANK+0x12c,0);put(BANK+0x110,FILE)
 u.mem_write(BANK+0x148,bytes([seed&1,0]));put(BANK+0x98,16);put(BANK+0x3c,0);put(BANK+0x20,A+0x6000)
 entry=A+0x400;put(entry+4,BANK)
 for i in range(7):put(A+0x6000+i*12,0xfffffff0+i*64)

 for off,val in ((0,0),(4,0),(8,HDR),(12,HDR)):put(HDR+off,val)
 for slot,target in ((0x143fe0c,API),(0x143fe10,API+32),(0x143fd30,API+64)):put(slot,target)
 held=[];blocks=[];seen=set();events=[];active={};snapshots=[];registered=[]
 def hook(uc,a,size,_):
  seen.add(a)
  if a not in (API,API+32,API+64,0xbfea0e,0x98f0f0):return
  sp=uc.reg_read(UC_X86_REG_ESP);cleanup=0
  if a==API:
   section=get(sp+4);assert section==BANK+0x14c and not held;held.append(section);events.append(('enter',section));cleanup=4;result=0xabcdef
  elif a==API+32:
   assert held==[get(sp+4)];events.append(('leave',held.pop()));cleanup=4;result=0xabc123
  elif a==API+64:raise AssertionError('try lock unexpectedly executed')
  elif a==0xbfea0e:
   assert held==[BANK+0x14c] and get(sp+4)==20
   result=HEAP+len(blocks)*32;blocks.append(result);events.append(('allocate',result))
  else:
   assert not held and uc.reg_read(UC_X86_REG_ECX)==0x13bc870
   assert get(sp+4)==active['obj']+0x2c and get(sp+8)==FILE
   assert bytes(uc.mem_read(active['obj']+0x2c,24))==active['desc']
   assert get(active['obj']+0x18)==0
   result=active['uid'];cleanup=8;events.append(('submit',active['obj'],result))
  uc.reg_write(UC_X86_REG_EAX,result);uc.reg_write(UC_X86_REG_ECX,0xdeadbeef);uc.reg_write(UC_X86_REG_EDX,0x87654321)
  uc.reg_write(UC_X86_REG_EIP,get(sp));uc.reg_write(UC_X86_REG_ESP,sp+4+cleanup)
 u.hook_add(UC_HOOK_CODE,hook)
 def call(a,args,this):
  sp=STACK+0x8000;put(sp,STOP)
  for i,arg in enumerate(args):put(sp+4+i*4,arg)
  saved=(UC_X86_REG_EBX,UC_X86_REG_ESI,UC_X86_REG_EDI,UC_X86_REG_EBP)
  for i,r in enumerate(saved):u.reg_write(r,0x55660000+i)
  u.reg_write(UC_X86_REG_ESP,sp);u.reg_write(UC_X86_REG_ECX,this);u.emu_start(a,STOP,count=5000)
  assert u.reg_read(UC_X86_REG_EIP)==STOP and u.reg_read(UC_X86_REG_ESP)==sp+4+4*len(args)
  assert all(u.reg_read(r)==0x55660000+i for i,r in enumerate(saved)) and not held
  return u.reg_read(UC_X86_REG_EAX)
 def tree():
  visited=set()
  def walk(n,parent):
   if not n:return [],1
   assert n in blocks and n not in visited;visited.add(n);assert get(n+4)==parent
   color=u.mem_read(n,1)[0];assert color in (0,1)
   left,right=get(n+8),get(n+12)
   if not color:assert (not left or u.mem_read(left,1)[0]==1) and (not right or u.mem_read(right,1)[0]==1)
   lv,lb=walk(left,n);rv,rb=walk(right,n);assert lb==rb
   return lv+[get(n+16)]+rv,lb+color
  values,bh=walk(get(HDR+4),HDR)
  assert values==sorted(registered) and len(visited)==len(blocks)==get(BANK+0x12c)
  if values:
   assert u.mem_read(get(HDR+4),1)[0]==1 and get(get(HDR+8)+16)==values[0] and get(get(HDR+12)+16)==values[-1]
 for step,index in enumerate(order):
  obj=OBJ+index*0x80;buf=A+0x5000+index*0x100;length=(0,1,17,0x7fffffff,0x80000000,0xffffffff,99)[index]
  before=bytes(u.mem_read(obj,0x48));expected=bytearray(before)
  for off,val in ((0,0x129b8b4),(4,0x129b8a0),(8,buf),(12,buf),(16,length),(20,0),(24,0),(28,0),(32,0xffffffff),(36,0),(40,BANK),(44,0),(48,0),(52,0),(56,0),(64,3)):struct.pack_into('<I',expected,off,val)
  expected[60]=1;expected[68:72]=b'\0'*4
  assert call(0x9d5d30,[buf,length,BANK],obj)==obj
  assert bytes(u.mem_read(obj,0x48))==bytes(expected)
  registered.append(obj);tree();snapshots.append(bytes(u.mem_read(A,N)))
  put(entry+8,index);offset=step*17;priority=6 if seed else 0
  uid=(0,1,0x7fffffff,0x80000000,0xfffffffe,0xffffffff,17)[step]
  expected_desc=bytearray(u.mem_read(obj+0x2c,24))
  for at,value in ((0,buf),(4,(0xfffffff0+index*64+offset)&0xffffffff),(8,((length+15)&0xfffffff0) if seed&1 else length),(12,obj),(20,priority)):struct.pack_into('<I',expected_desc,at,value)
  expected_desc[16]=0
  active.update(obj=obj,desc=bytes(expected_desc),uid=uid)
  prior=bytes(u.mem_read(A,N));expected_arena=bytearray(prior)
  expected_arena[obj-A+0x2c:obj-A+0x44]=expected_desc
  struct.pack_into('<I',expected_arena,obj-A+0x18,uid)
  call(0x9d5970,[offset,obj,length,0xdeadbeef,0x87654321,priority],entry)
  assert bytes(u.mem_read(A,N))==bytes(expected_arena);tree();snapshots.append(bytes(expected_arena))
 return snapshots,events,seen
cases=0;witnesses={}
orders=(tuple(range(7)),tuple(reversed(range(7))),(3,1,5,0,2,4,6),(2,6,0,4,1,5,3))
for order,seed in itertools.product(orders,(0,7)):
 a,events,seen=run(codes,order,seed);b,other,_=run(native,order,seed);assert a==b and events==other
 cases+=1
 for address in codes:
  if address in seen:witnesses.setdefault(address,(order,seed))
omissions=[]
for address,args in witnesses.items():
 reduced={a:b for a,b in codes.items() if a!=address}
 try:run(reduced,*args)
 except Exception:omissions.append(hex(address))
 else:raise AssertionError(('undetected omission',hex(address)))
assert len(witnesses)==len(codes),(set(codes)-set(witnesses))
assert all(hashlib.sha256(Path(p).read_bytes()).hexdigest()==h for p,h in pins.items())
report=dict(status='PASS',cases=cases,constructed=cases*7,submitted=cases*7,snapshots=cases*14,providers=len(codes),omissions=omissions,boundary='Actual constructors, base, lock RAII, pointer insertion/rebalancing, SetupRead/GetEntryDataOffset/Align and file wrapper. RuntimeData offset path only; packed lookup, synchronous SetupBuffer and ReadData paths remain cold. Win32 lock/CRT allocation/manager admission controlled; no Windows IO, destruction/unregistration or valid bank lifetime claim. Predecessor behavior only.')
report.update(build=str(OUT),input_sha256=pins,object_sha256={str(p):hashlib.sha256(p.read_bytes()).hexdigest() for p in OUT.glob('*.obj')})
(OUT/'receipt.json').write_text(json.dumps(report,indent=2))
(R/'rebuild/compile-gate/native-bank-entry-setup-composition.json').write_text(json.dumps(report,indent=2))
print('NATIVE_BANK_ENTRY_SETUP_COMPOSITION_PASS cases=8 constructors=56 submissions=56 snapshots=112 providers=14 omissions=14')
print('RECEIPT',OUT/'receipt.json')
