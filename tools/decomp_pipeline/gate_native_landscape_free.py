"""Landscape release and singleton counter against retail and independent events."""
import hashlib,itertools,json,struct,subprocess,tempfile
from pathlib import Path
from capstone import Cs,CS_ARCH_X86,CS_MODE_32
import check_one as V
from variable_allocator_parity import check
from pe_oracle import EXE,read_virtual_data
ROOT=Path(__file__).resolve().parents[2];w=Path(tempfile.mkdtemp(prefix='native-landscape-free-'));env=V.env()
sources=[ROOT/'rebuild/src/compiled'/p for p in ('00/bd/CEngineLandscapeMemoryManager_FreeVirtual_00bd9b90.cpp','00/9f/CMemoryAllocator_GetNoAllocations_009f1f10.cpp')];fixture=ROOT/'rebuild/tests/integration/NativeLandscapeFree_test.cpp'
targets={'?EnterSynchronisation@CMemoryAllocator@@KIXXZ':0x9f2000,'?LeaveSynchronisation@CMemoryAllocator@@KIXXZ':0x9f2010,'?g_FableMemoryAllocator_013CAA90@@3PAVCMemoryAllocator@@A':0x13caa90,'?g_FableAllocatorShutdownRequested_013CAA94@@3_NA':0x13caa94,'?InternalDeallocate@CMemoryAllocator@@IAEXPAX@Z':0x9f2920,'?GetNoAllocations@CMemoryAllocator@@QAEKXZ':0x9f1f10,'??1CMemoryAllocator@@IAE@XZ':0x9f2310,'??3CMemoryAllocator@@KIXPAX@Z':0x9f1f90}
release='?FreeVirtual@CEngineLandscapeMemoryManager@@QAEXPAX@Z';counter='?GetNoAllocations@CMemoryAllocator@@QAEKXZ'
aliases={'@FreeLandscape@12':release,'@EnterBoundary@0':'?EnterSynchronisation@CMemoryAllocator@@KIXXZ','@LeaveBoundary@0':'?LeaveSynchronisation@CMemoryAllocator@@KIXXZ','@DeallocateBoundary@12':'?InternalDeallocate@CMemoryAllocator@@IAEXPAX@Z','@DestroyBoundary@4':'??1CMemoryAllocator@@IAE@XZ','@DeleteBoundary@4':'??3CMemoryAllocator@@KIXPAX@Z'}
(w/'landscape_free_aliases.h').write_text('\n'.join('#pragma comment(linker,"/alternatename:'+a+'='+b+'")' for a,b in aliases.items())+'\n')
raw=EXE.read_bytes();code=bytearray(135);fixups=[];md=Cs(CS_ARCH_X86,CS_MODE_32);md.detail=True
for a,n,offset in [(0xbd9b90,97,0),(0x9f1f10,7,128)]:
 body=read_virtual_data(raw,a,n);code[offset:offset+n]=body
 for i in md.disasm(body,a):
  if i.mnemonic=='call' and i.operands[0].type==2:
   target=i.operands[0].imm;assert target in targets.values();fixups.append((offset+i.address-a+1,target,1))
  else:
   for op in i.operands:
    if op.type==3 and op.mem.disp in (0x13caa90,0x13caa94):fixups.append((offset+i.address-a+i.disp_offset,op.mem.disp,0))
assert len(fixups)==11
(w/'landscape_free_retail.h').write_text('static const unsigned char retailCode[]={'+','.join(map(str,code))+'};\nstatic const unsigned retailFixups[][3]={'+','.join('{%d,%d,%d}'%f for f in fixups)+'};\n')
objects=[]
for i,p in enumerate(sources+[fixture]):
 o=w/(str(i)+'.obj');r=V.cl(['/nologo','/c','/O2','/MT','/I'+str(w),'/Fo'+str(o),str(p)],env,w);assert not r.returncode,r.stdout+r.stderr;objects.append(o)
proofs=[check(objects[0],release,0xbd9b90,97,11,targets),check(objects[1],counter,0x9f1f10,7,0,{})];assert all(p['grade']=='MATCH' for p in proofs)
cmd=[str(V.VC/'bin/link.exe'),'/nologo','/out:'+str(w/'fixture.exe'),'kernel32.lib']
for omit in range(2):
 r=subprocess.run(cmd+[str(o) for i,o in enumerate(objects) if i!=omit],env=env,capture_output=True,text=True);assert r.returncode
r=subprocess.run(cmd+list(map(str,objects)),env=env,capture_output=True,text=True);assert not r.returncode,r.stdout+r.stderr
base=0x24000000;cases=snapshots=0
with (w/'input.bin').open('wb') as f,(w/'expected.bin').open('wb') as out:
 for fill,mode,present,shutdown,allocation,n1,n2 in itertools.product((0,0x95),range(6),(0,1),(0,1),(0,1),(0,1,2,0xffffffff),(0,1,2,0xffffffff)):
  a=bytearray([fill])*4096
  struct.pack_into('<I',a,0x40c,n1);struct.pack_into('<I',a,0x90c,n2)
  f.write(struct.pack('<4I',mode,present,shutdown,allocation));f.write(a);cases+=1
  owner=base if present else 0;flag=shutdown;depth=0
  def record(kind,this=0,arg=0):
   global snapshots
   out.write(struct.pack('<6I',kind,this,arg,owner,flag,depth));out.write(a);snapshots+=1
  if allocation:
   record(1);depth=1
   if mode==1:owner=base+0x500
   if owner:
    record(2,owner,base+0xe00)
    if mode==2:owner=base+0x500
    if mode==3:flag=1
    if mode==4:struct.pack_into('<I',a,owner-base+0x40c,0)
    if flag and struct.unpack_from('<I',a,owner-base+0x40c)[0]==0:
     deleted=owner;record(3,deleted)
     if mode==5:owner=base+0xa00
     record(4,deleted);owner=0
   record(5);depth=0
  record(6)
for kind in ('retail','compiled'):
 r=subprocess.run([str(w/'fixture.exe'),str(w/'input.bin'),str(w/(kind+'.bin')),kind],env=env,capture_output=True,text=True,timeout=60);assert not r.returncode,(w,r.returncode,r.stdout,r.stderr);print(r.stdout.strip(),kind,flush=True)
 with (w/'expected.bin').open('rb') as e,(w/(kind+'.bin')).open('rb') as f:
  while True:
   x=e.read(1<<20);y=f.read(1<<20);assert x==y,(w,kind,'native/expected mismatch')
   if not x:break
headers=[ROOT/'rebuild/include'/p for p in ('fable_memory_allocator.h','fable_memory_allocator_deallocate.h','engine/CEngineLandscapeMemoryManager.h')]
report=dict(cases=cases,snapshots=snapshots,omission_checks=2,providers=proofs,source_sha256={str(p.relative_to(ROOT)):hashlib.sha256(p.read_bytes()).hexdigest() for p in sources+headers},fixture_sha256=hashlib.sha256(fixture.read_bytes()).hexdigest(),boundary='Original97/7bytes with11audited fixups versus compiled providers and independent event/state prediction. Full4KiB snapshots plus singleton,shutdown,lock depth and callback arguments. Null/live input and owner;counter0/1/2/ffffffff;callbacks replace singleton on lock,deallocate,destruction;toggle shutdown or clear counter. Allocator synchronization,InternalDeallocate,destructor and class delete remain recording boundaries;real getter executes.')
(ROOT/'rebuild/compile-gate/native-landscape-free.json').write_text(json.dumps(report,indent=2)+'\n');print('NATIVE_LANDSCAPE_FREE_GATE_PASS',snapshots,'snapshots')
