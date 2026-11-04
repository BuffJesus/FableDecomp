"""Actual shadow/render-target/surface/texture cleanup with COM release boundaries."""
import hashlib,itertools,json,struct,subprocess,tempfile
from pathlib import Path
from capstone import Cs,CS_ARCH_X86,CS_MODE_32
import check_one as V
from variable_allocator_parity import check
from pe_oracle import EXE,read_virtual_data
ROOT=Path(__file__).resolve().parents[2];w=Path(tempfile.mkdtemp(prefix='native-shadow-cleanup-'));env=V.env()
names=('00/b4/CEngineShadowRenderer_TextureCleanup_00b4b830.cpp','00/b4/CEngineShadowRenderer_ReleaseResources_00b4b880.cpp','00/a1/CRenderTarget_Uninitialise_00a10e30.cpp','00/9f/CSurface_Uninitialise_009f2e20.cpp','00/9f/CTexture_Uninitialise_009f9f70.cpp')
sources=[ROOT/'rebuild/src/compiled'/p for p in names];fixture=ROOT/'rebuild/tests/integration/NativeShadowCleanup_test.cpp'
cleanup='?UninitialiseTextures@CEngineShadowRenderer@@AAEXXZ';release='?OnReleaseDefaultPoolResources@CEngineShadowRenderer@@UAEXXZ';target='?Uninitialise@CRenderTarget@@QAEXXZ';surface='?Uninitialise@CSurface@@QAEXXZ';texture='?Uninitialise@CTexture@@QAEXXZ'
specs=[(cleanup,0xb4b830,77,5,0),(release,0xb4b880,5,1,80),(target,0xa10e30,36,2,96),(surface,0x9f2e20,32,0,144),(texture,0x9f9f70,34,0,176)]
targets={s:a for s,a,_,_,_ in specs};offsets={a:o for _,a,_,_,o in specs}
(w/'shadow_cleanup_aliases.h').write_text('#pragma comment(linker,"/alternatename:@Cleanup@8='+cleanup+'")\n#pragma comment(linker,"/alternatename:@ReleaseResources@8='+release+'")\n')
raw=EXE.read_bytes();body=bytearray(210);fixups=[];md=Cs(CS_ARCH_X86,CS_MODE_32);md.detail=True
for s,address,length,_,offset in specs:
 code=read_virtual_data(raw,address,length);body[offset:offset+length]=code
 for i in md.disasm(code,address):
  if i.mnemonic in ('call','jmp') and i.operands[0].type==2:assert i.operands[0].imm in offsets;fixups.append((offset+i.address-address+i.imm_offset,offsets[i.operands[0].imm]))
assert len(fixups)==8
(w/'shadow_cleanup_retail.h').write_text('static const unsigned char retailCode[]={'+','.join(map(str,body))+'};\nstatic const unsigned retailFixups[][2]={'+','.join('{%d,%d}'%f for f in fixups)+'};\n')
objects=[]
for n,p in enumerate(sources+[fixture]):
 o=w/(str(n)+'.obj');r=V.cl(['/nologo','/c','/O2','/MT','/I'+str(w),'/Fo'+str(o),str(p)],env,w);assert not r.returncode,r.stdout+r.stderr;objects.append(o)
proofs=[check(objects[n],s,a,l,c,targets) for n,(s,a,l,c,_) in enumerate(specs)]
cmd=[str(V.VC/'bin/link.exe'),'/nologo','/out:'+str(w/'fixture.exe'),'kernel32.lib']
for omit in range(5):
 r=subprocess.run(cmd+[str(o) for i,o in enumerate(objects) if i!=omit],env=env,capture_output=True,text=True);assert r.returncode
r=subprocess.run(cmd+list(map(str,objects)),env=env,capture_output=True,text=True);assert not r.returncode,r.stdout+r.stderr
cases=0
with (w/'input.bin').open('wb') as f:
 for base,flag,mask,mode,entry in itertools.product((0,0x800),(0,1,2,0x7f,0x80,0xff),range(64),range(4),range(2)):
  a=bytearray([0x95])*4096;a[base+0x2af]=flag
  for j,slot in enumerate((0x2cc,0x2dc,0x30c,0x2fc,0x318,0x2e8)):struct.pack_into('<I',a,base+slot,0x24000c00+16*j if mask&(1<<j) else 0)
  f.write(struct.pack('<3I',base,mode,entry));f.write(a);cases+=1
for kind in ('retail','compiled'):
 r=subprocess.run([str(w/'fixture.exe'),str(w/'input.bin'),str(w/(kind+'.bin')),kind],env=env,capture_output=True,text=True,timeout=60);assert not r.returncode,(w,r.returncode,r.stdout,r.stderr);print(r.stdout.strip(),kind,flush=True)
with (w/'retail.bin').open('rb') as f,(w/'compiled.bin').open('rb') as g:
 while True:
  a=f.read(1<<20);b=g.read(1<<20);assert a==b,w
  if not a:break
size=(w/'compiled.bin').stat().st_size;assert size%4096==0;snapshots=size//4096
report=dict(cases=cases,snapshots=snapshots,omission_checks=5,providers=proofs,source_sha256={str(p.relative_to(ROOT)):hashlib.sha256(p.read_bytes()).hexdigest() for p in sources},fixture_sha256=hashlib.sha256(fixture.read_bytes()).hexdigest(),boundary='Actual shadow cleanup/release wrapper,render-target cleanup,surface cleanup and texture cleanup. Full4KiB expected event/final states from independent slot model. All64 null-resource combinations,six raw allocation flags,two placements,direct/wrapper entries,four callback mutation modes. Release callbacks mutate next/current pointer and allocation flag;surface whole-state zero and texture high-nibble-only zero preserved. COM Release only external boundary.')
(ROOT/'rebuild/compile-gate/native-shadow-cleanup.json').write_text(json.dumps(report,indent=2)+'\n');print('NATIVE_SHADOW_CLEANUP_GATE_PASS',snapshots,'snapshots')
