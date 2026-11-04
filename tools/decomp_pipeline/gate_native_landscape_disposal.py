"""Landscape disposal queues against retail execution."""
import hashlib,itertools,json,struct,subprocess,tempfile
from pathlib import Path
from capstone import Cs,CS_ARCH_X86,CS_MODE_32
import check_one as V
from variable_allocator_parity import check
from pe_oracle import EXE,read_virtual_data
ROOT=Path(__file__).resolve().parents[2];w=Path(tempfile.mkdtemp(prefix='native-landscape-disposal-'));env=V.env()
sources=[ROOT/'rebuild/src/compiled'/p for p in ('00/bd/CEngineLandscapeMemoryManager_FreeDisposalList_00bd9ce0.cpp','00/9f/CMemoryAllocator_GetNoAllocations_009f1f10.cpp','00/bd/CEngineLandscapeMemoryManager_EnablePoolAllocation_00bda070.cpp')];fixture=ROOT/'rebuild/tests/integration/NativeLandscapeDisposal_test.cpp'
targets={'?EnterSynchronisation@CMemoryAllocator@@KIXXZ':0x9f2000,'?LeaveSynchronisation@CMemoryAllocator@@KIXXZ':0x9f2010,'?g_FableMemoryAllocator_013CAA90@@3PAVCMemoryAllocator@@A':0x13caa90,'?g_FableAllocatorShutdownRequested_013CAA94@@3_NA':0x13caa94,'?InternalDeallocate@CMemoryAllocator@@IAEXPAX@Z':0x9f2920,'?GetNoAllocations@CMemoryAllocator@@QAEKXZ':0x9f1f10,'??1CMemoryAllocator@@IAE@XZ':0x9f2310,'??3CMemoryAllocator@@KIXPAX@Z':0x9f1f90}
release='?FreeDisposalList@CEngineLandscapeMemoryManager@@QAEXXZ';counter='?GetNoAllocations@CMemoryAllocator@@QAEKXZ'
targets.update({'_free':0xbfea14,'?Uninitialise@CTexture@@QAEXXZ':0x9f9f70})
targets[release]=0xbd9ce0
aliases={'@Toggle@12':'?EnablePoolAllocation@CEngineLandscapeMemoryManager@@QAEX_N@Z','@Drain@4':release,'@TextureBoundary@4':'?Uninitialise@CTexture@@QAEXXZ','@EnterBoundary@0':'?EnterSynchronisation@CMemoryAllocator@@KIXXZ','@LeaveBoundary@0':'?LeaveSynchronisation@CMemoryAllocator@@KIXXZ','@DeallocateBoundary@12':'?InternalDeallocate@CMemoryAllocator@@IAEXPAX@Z','@DestroyBoundary@4':'??1CMemoryAllocator@@IAE@XZ','@DeleteBoundary@4':'??3CMemoryAllocator@@KIXPAX@Z'}
(w/'landscape_disposal_aliases.h').write_text('\n'.join('#pragma comment(linker,"/alternatename:'+a+'='+b+'")' for a,b in aliases.items())+'\n')
raw=EXE.read_bytes();code=bytearray(570);fixups=[];md=Cs(CS_ARCH_X86,CS_MODE_32);md.detail=True
for a,n,offset in [(0xbd9ce0,488,0),(0x9f1f10,7,512),(0xbda070,26,544)]:
 body=read_virtual_data(raw,a,n);code[offset:offset+n]=body
 for i in md.disasm(body,a):
  if i.mnemonic=='call' and i.operands[0].type==2:
   target=i.operands[0].imm;assert target in targets.values();fixups.append((offset+i.address-a+1,target,1))
  else:
   for op in i.operands:
    if op.type==3 and op.mem.disp in (0x13caa90,0x13caa94):fixups.append((offset+i.address-a+i.disp_offset,op.mem.disp,0))
assert len(fixups)==38
(w/'landscape_disposal_retail.h').write_text('static const unsigned char retailCode[]={'+','.join(map(str,code))+'};\nstatic const unsigned retailFixups[][3]={'+','.join('{%d,%d,%d}'%f for f in fixups)+'};\n')
objects=[]
for i,p in enumerate(sources+[fixture]+sources[:1]):
 o=w/(str(i)+'.obj');r=V.cl(['/nologo','/c','/O2','/MT','/I'+str(w),'/Fo'+str(o)]+(['/Dfree=DisposalFree'] if i==4 else [])+[str(p)],env,w);assert not r.returncode,r.stdout+r.stderr;objects.append(o)
proofs=[check(objects[0],release,0xbd9ce0,488,37,targets),check(objects[1],counter,0x9f1f10,7,0,{})];assert all(p['grade']=='MATCH' for p in proofs)
proofs.append(check(objects[2],'?EnablePoolAllocation@CEngineLandscapeMemoryManager@@QAEX_N@Z',0xbda070,26,1,targets))
from gate_game_component_abi import Coff
c,d=Coff(objects[0]),Coff(objects[4]);sec,off=c.symbols[release];sec1,off1=d.symbols[release];body,rel=c.sections[sec];body1,rel1=d.sections[sec1];assert off==off1 and body==body1 and {p:('_DisposalFree' if t=='_free' else t,k) for p,(t,k) in rel.items()}==rel1
cmd=[str(V.VC/'bin/link.exe'),'/nologo','/out:'+str(w/'fixture.exe'),'kernel32.lib'];linked=objects[1:]
for omit in (0,1,3):
 r=subprocess.run(cmd+[str(o) for i,o in enumerate(linked) if i!=omit],env=env,capture_output=True,text=True);assert r.returncode
r=subprocess.run(cmd+list(map(str,linked)),env=env,capture_output=True,text=True);assert not r.returncode,r.stdout+r.stderr
base=0x24000000;cases=0
masks=[0,0x1ff,0x155,0xaa]+[1<<i for i in range(9)]
with (w/'input.bin').open('wb') as f:
 for mode,present,shutdown,count,mask,n in itertools.product(range(6),(0,1),(0,1),(0,1,0xffffffff),masks,(1,3)):
  a=bytearray([0x95])*8192
  def put(off,v):struct.pack_into('<I',a,off,v&0xffffffff)
  put(0x60c,count);put(0xc0c,count);a[0x39]=mask&1
  for i in range(9):
   head=0x1000+i*16;nodes=[0x1200+(i*3+j)*16 for j in range(n)] if mask&(1<<i) else []
   put(0x10+i*4,base+head);put(head,base+(nodes[0] if nodes else head));put(head+4,base+(nodes[-1] if nodes else head))
   for j,node in enumerate(nodes):put(node,base+(nodes[j+1] if j+1<len(nodes) else head));put(node+4,base+(nodes[j-1] if j else head));put(node+8,base+0x1800+(i*3+j)*16)
  f.write(struct.pack('<3I',mode,present,shutdown));f.write(a);cases+=1
for kind in ('retail','compiled'):
 r=subprocess.run([str(w/'fixture.exe'),str(w/'input.bin'),str(w/(kind+'.bin')),kind],env=env,capture_output=True,text=True,timeout=60);assert not r.returncode,(w,r.returncode,r.stdout,r.stderr);print(r.stdout.strip(),kind,flush=True)
with (w/'retail.bin').open('rb') as e,(w/'compiled.bin').open('rb') as f:
 while True:
  x=e.read(1<<20);y=f.read(1<<20);assert x==y,(w,'native mismatch')
  if not x:break
size=(w/'compiled.bin').stat().st_size;assert size%8216==0;snapshots=size//8216
headers=[ROOT/'rebuild/include'/p for p in ('fable_memory_allocator.h','fable_memory_allocator_deallocate.h','engine/CEngineLandscapeMemoryManager.h','engine/CIndexBuffer.h','fable_font_vertex_buffer.h','fable_surface_types.h','fable_stlport_list.h')]
report=dict(cases=cases,snapshots=snapshots,omission_checks=3,providers=proofs,source_sha256={str(p.relative_to(ROOT)):hashlib.sha256(p.read_bytes()).hexdigest() for p in sources+headers},fixture_sha256=hashlib.sha256(fixture.read_bytes()).hexdigest(),boundary='Original488/7/26bytes with38audited fixups. Full8KiB snapshots plus singleton,shutdown,lock depth and boundary arguments. All nine queues isolated,empty/full/alternating masks,one/three nodes,counters0/1/ffffffff. Resource callbacks replace current list head or singleton;lock/deallocation/destructor mutate allocator state. Actual pool toggle precedes direct drain;equal flags skip cleanup until direct drain. Actual allocation counter;texture/virtual resource destruction,allocator lifetime/locks and node free remain boundaries;free-renamed object body/relocations audited.')
(ROOT/'rebuild/compile-gate/native-landscape-disposal.json').write_text(json.dumps(report,indent=2)+'\n');print('NATIVE_LANDSCAPE_DISPOSAL_GATE_PASS',snapshots,'snapshots')
