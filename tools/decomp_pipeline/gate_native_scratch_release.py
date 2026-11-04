"""Actual scratch page/entry cleanup, long-vector growth and texture teardown."""
import hashlib,itertools,json,struct,subprocess,tempfile
from pathlib import Path
from capstone import Cs,CS_ARCH_X86,CS_MODE_32
import check_one as V
from variable_allocator_parity import check
from gate_game_component_abi import Coff
from pe_oracle import EXE,read_virtual_data
ROOT=Path(__file__).resolve().parents[2];w=Path(tempfile.mkdtemp(prefix='native-scratch-release-'));env=V.env()
sources=[ROOT/'rebuild/src/compiled'/p for p in ('00/f3/CGPUScratchBufferTextureManager_FreeMemoryPage_00f37030.cpp','00/f3/CGPUScratchBufferTextureManager_FreeResourceEntry_00f36f40.cpp','00/9f/CTexture_Uninitialise_009f9f70.cpp')]
fixture=ROOT/'rebuild/tests/integration/NativeScratchRelease_test.cpp'
page='?FreeMemoryPage@CGPUScratchBufferTextureManager@@QAEXJ@Z';entry='?FreeResourceEntry@CGPUScratchBufferTextureManager@@AAEXJ@Z';grow='?_M_insert_overflow@?$vector@JV?$allocator@J@_STL@@@_STL@@QAEXPAJABJABU__true_type@2@I_N@Z';texture='?Uninitialise@CTexture@@QAEXXZ'
specs=[(0,page,0xf37030,140,2,0),(1,entry,0xf36f40,233,4,160),(0,grow,0x440990,216,4,416),(2,texture,0x9f9f70,34,0,640)]
targets={entry:0xf36f40,grow:0x440990,texture:0x9f9f70,'??3@YAXPAX@Z':0xbfe9bc,'_malloc':0xbfea0e,'_free':0xbfea14,'_memmove':0xbfeae6}
(w/'scratch_release_aliases.h').write_text('#pragma comment(linker,"/alternatename:@Page@12='+page+'")\n#pragma comment(linker,"/alternatename:@Entry@12='+entry+'")\n')
raw=EXE.read_bytes();body=bytearray(704);fixups=[];md=Cs(CS_ARCH_X86,CS_MODE_32);md.detail=True
for _,s,address,length,_,offset in specs:
 code=read_virtual_data(raw,address,length);body[offset:offset+length]=code
 for i in md.disasm(code,address):
  if i.mnemonic=='call' and i.operands[0].type==2:assert i.operands[0].imm in targets.values();fixups.append((offset+i.address-address+i.imm_offset,i.operands[0].imm))
assert len(fixups)==10
(w/'scratch_release_retail.h').write_text('static const unsigned char retailCode[]={'+','.join(map(str,body))+'};\nstatic const unsigned retailFixups[][2]={'+','.join('{%d,%d}'%f for f in fixups)+'};\n')
plain=[];objects=[]
for n,p in enumerate(sources):
 o=w/('plain'+str(n)+'.obj');r=V.cl(['/nologo','/c','/O2','/MT','/Fo'+str(o),str(p)],env,w);assert not r.returncode,r.stdout+r.stderr;plain.append(o)
 o=w/(str(n)+'.obj');r=V.cl(['/nologo','/c','/O2','/MT','/Dmalloc=Allocate','/Dfree=ArenaFree','/Fo'+str(o),str(p)],env,w);assert not r.returncode,r.stdout+r.stderr;objects.append(o)
 for symbol in ([page,grow] if n==0 else [entry,grow] if n==1 else [texture]):
  c,d=Coff(plain[n]),Coff(o);sc,off=c.symbols[symbol];sd,offd=d.symbols[symbol];b,rel=c.sections[sc];bd,reld=d.sections[sd]
  assert off==offd and b==bd and {p:({'_malloc':'_Allocate','_free':'_ArenaFree'}.get(t,t),k) for p,(t,k) in rel.items()}==reld
proofs=[check(plain[n],s,a,length,count,targets) for n,s,a,length,count,_ in specs]
o=w/'fixture.obj';r=V.cl(['/nologo','/c','/O2','/MT','/I'+str(w),'/Fo'+str(o),str(fixture)],env,w);assert not r.returncode,r.stdout+r.stderr;objects.append(o)
cmd=[str(V.VC/'bin/link.exe'),'/nologo','/out:'+str(w/'fixture.exe'),'kernel32.lib']
for omit in range(3):
 r=subprocess.run(cmd+[str(p) for i,p in enumerate(objects) if i!=omit],env=env,capture_output=True,text=True);assert r.returncode
r=subprocess.run(cmd+list(map(str,objects)),env=env,capture_output=True,text=True);assert not r.returncode,r.stdout+r.stderr
cases=0;BASE=0x24000000
with (w/'input.bin').open('wb') as f:
 for kind,page_index,count,presence,mode,capacity in itertools.product(range(2),range(2),(0,1,3),range(8),range(6),(0,1,4)):
  a=bytearray([0x95])*8192
  def put(off,*values):struct.pack_into('<'+'I'*len(values),a,off,*[v&0xffffffff for v in values])
  put(4,BASE+0x400,BASE+0x484,BASE+0x484);put(0x10,BASE+0x200,BASE+0x238,BASE+0x238)
  put(0x28,BASE+0xa00,BASE+0xa00+4*(capacity==1),BASE+0xa00+capacity*4);put(0x34,BASE+0x900,BASE+0x900+4*(capacity==1),BASE+0x900+capacity*4)
  for pool in (0x200,0x300):
   for j in range(2):put(pool+28*j,j,BASE+0x700,BASE+0x700+count*4,BASE+0x70c)
  for j in range(3):
   put(0x700+4*j,j)
   for table in (0x400,0x600):put(table+44*j,1,BASE+0xb00+16*j if presence&1 else 0,BASE+0xc00+32*j if presence&2 else 0,page_index)
   put(0xb00+16*j,BASE+0xb80+16*j if presence&4 else 0,0xf1234567)
  index=mode%3;f.write(struct.pack('<5I',kind,page_index,count,mode,index));f.write(a);cases+=1
for kind in ('retail','compiled'):
 r=subprocess.run([str(w/'fixture.exe'),str(w/'input.bin'),str(w/(kind+'.bin')),kind],env=env,capture_output=True,text=True,timeout=60);assert not r.returncode,(w,r.returncode,r.stdout,r.stderr);print(r.stdout.strip(),kind,flush=True)
with (w/'retail.bin').open('rb') as f,(w/'compiled.bin').open('rb') as g:
 while True:
  a=f.read(1<<20);b=g.read(1<<20);assert a==b,w
  if not a:break
size=(w/'compiled.bin').stat().st_size;assert size%8200==0;snapshots=size//8200
report=dict(cases=cases,snapshots=snapshots,omission_checks=3,providers=proofs,source_sha256={str(p.relative_to(ROOT)):hashlib.sha256(p.read_bytes()).hexdigest() for p in sources},fixture_sha256=hashlib.sha256(fixture.read_bytes()).hexdigest(),boundary='Actual FreeMemoryPage/FreeResourceEntry/long-vector growth/CTexture::Uninitialise;four original spans/10fixups,three mandatory TUs. Full8KiB snapshots. Page or direct resource deletion,empty/one/three lists,nullable textures/surfaces/D3D pointers,free-list spare/full/empty storage,callbacks replacing resource/page arrays or surfaces and mutating texture flags. Real vector copying/erase and CRT memmove. Bounded malloc/free/operator delete,COM Release and scalar surface deletion remain explicit boundaries. Original and allocator-renamed production code/relocations separately audited.')
(ROOT/'rebuild/compile-gate/native-scratch-release.json').write_text(json.dumps(report,indent=2)+'\n');print('NATIVE_SCRATCH_RELEASE_GATE_PASS',snapshots,'snapshots')
