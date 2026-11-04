"""Actual texture mip-surface getter, surface copy and ownership initialization."""
import hashlib,itertools,json,struct,subprocess,tempfile
from pathlib import Path
from capstone import Cs,CS_ARCH_X86,CS_MODE_32
import check_one as V
from variable_allocator_parity import check
from pe_oracle import EXE,read_virtual_data
ROOT=Path(__file__).resolve().parents[2];w=Path(tempfile.mkdtemp(prefix='native-texture-surface-'));env=V.env()
names=('00/9f/CSurface_CopyConstructor_009f2d60.cpp','00/9f/CSurface_InitialiseFromTextureMipmap_009f2f10.cpp','00/9f/CTexture_GetSurfaceLevel_009f9e00.cpp','00/9f/CSurface_Uninitialise_009f2e20.cpp')
sources=[ROOT/'rebuild/src/compiled'/p for p in names];fixture=ROOT/'rebuild/tests/integration/NativeTextureSurface_test.cpp'
copy='??0CSurface@@QAE@ABV0@@Z';init='?InitialiseFromTextureMipmap@CSurface@@QAEXPAUIDirect3DSurface9@@@Z';getter='?GetSurfaceLevel@CTexture@@QBE?AVCSurface@@J@Z';surface='?Uninitialise@CSurface@@QAEXXZ';vtable='??_7CSurface@@6B@'
specs=[(copy,0x9f2d60,55,1,0),(init,0x9f2f10,66,0,64),(getter,0x9f9e00,96,5,144),(surface,0x9f2e20,32,0,240)]
targets={s:a for s,a,_,_,_ in specs};targets[vtable]=0x122f84c;offsets={a:o for _,a,_,_,o in specs}
(w/'texture_surface_aliases.h').write_text('\n'.join('#pragma comment(linker,"/alternatename:'+a+'='+b+'")' for a,b in (('@Copy@12',copy),('@Init@12',init),('@GetLevel@16',getter),('_SurfaceVtable',vtable)))+'\n')
raw=EXE.read_bytes();body=bytearray(272);fixups=[];md=Cs(CS_ARCH_X86,CS_MODE_32);md.detail=True
for s,address,length,_,offset in specs:
 code=read_virtual_data(raw,address,length);body[offset:offset+length]=code
 for i in md.disasm(code,address):
  if i.mnemonic in ('call','jmp') and i.operands[0].type==2:assert i.operands[0].imm in offsets;fixups.append((offset+i.address-address+i.imm_offset,offsets[i.operands[0].imm],1))
  elif any(o.type==2 and o.imm==0x122f84c for o in i.operands):fixups.append((offset+i.address-address+i.imm_offset,0,0))
assert len(fixups)==6
(w/'texture_surface_retail.h').write_text('static const unsigned char retailCode[]={'+','.join(map(str,body))+'};\nstatic const unsigned retailFixups[][3]={'+','.join('{%d,%d,%d}'%f for f in fixups)+'};\n')
objects=[]
for n,p in enumerate(sources+[fixture]):
 o=w/(str(n)+'.obj');r=V.cl(['/nologo','/c','/O2','/MT','/I'+str(w),'/Fo'+str(o),str(p)],env,w);assert not r.returncode,r.stdout+r.stderr;objects.append(o)
proofs=[check(objects[n],s,a,l,c,targets) for n,(s,a,l,c,_) in enumerate(specs)]
cmd=[str(V.VC/'bin/link.exe'),'/nologo','/out:'+str(w/'fixture.exe'),'kernel32.lib']
for omit in range(4):
 r=subprocess.run(cmd+[str(o) for i,o in enumerate(objects) if i!=omit],env=env,capture_output=True,text=True);assert r.returncode
r=subprocess.run(cmd+list(map(str,objects)),env=env,capture_output=True,text=True);assert not r.returncode,r.stdout+r.stderr
cases=0
with (w/'input.bin').open('wb') as f:
 for entry in range(3):
  if entry==0:
   for dst,state,alias,ptr,mode in itertools.product((0x400,0x800),(0,1,2,0xffffffff),range(3),(0,0x24000c00,0x24000c20),range(4)):
    a=bytearray([0x95])*4096;src=(0x200,dst,dst+0x10)[alias];struct.pack_into('<3I',a,src+4,ptr,state,0x12345678);f.write(struct.pack('<7I',entry,dst,src,mode,0,0,0));f.write(a);cases+=1
  elif entry==1:
   for dst,oldptr,ptr,mode in itertools.product((0x400,0x800),(0,0x24000c00),(0,0x24000c00,0x24000c40),range(4)):
    a=bytearray([0x95])*4096;struct.pack_into('<I',a,dst+4,oldptr);f.write(struct.pack('<7I',entry,dst,0,mode,ptr,0,0));f.write(a);cases+=1
  else:
   for dst,level,status,ptr,mode in itertools.product((0x400,0x800),(0,1,7,0x7fffffff,0x80000000,0xffffffff),(0,1,0x80004005,0xffffffff),(0,0x24000c20,0x24000c40),range(4)):
    a=bytearray([0x95])*4096;struct.pack_into('<I',a,0x100,0x24000c60);f.write(struct.pack('<7I',entry,dst,0,mode,ptr,level,status));f.write(a);cases+=1
for kind in ('retail','compiled'):
 r=subprocess.run([str(w/'fixture.exe'),str(w/'input.bin'),str(w/(kind+'.bin')),kind],env=env,capture_output=True,text=True,timeout=60);assert not r.returncode,(w,r.returncode,r.stdout,r.stderr);print(r.stdout.strip(),kind,flush=True)
with (w/'retail.bin').open('rb') as f,(w/'compiled.bin').open('rb') as g:
 while True:
  a=f.read(1<<20);b=g.read(1<<20);assert a==b,w
  if not a:break
size=(w/'compiled.bin').stat().st_size;assert size%4096==0;snapshots=size//4096
report=dict(cases=cases,snapshots=snapshots,omission_checks=4,providers=proofs,source_sha256={str(p.relative_to(ROOT)):hashlib.sha256(p.read_bytes()).hexdigest() for p in sources},fixture_sha256=hashlib.sha256(fixture.read_bytes()).hexdigest(),boundary='Full4KiB independent event/final model;actual texture mip getter with temporary surface copy/destruction,canonical copy constructor/mipmap ownership setter/cleanup. Six raw levels,four HRESULT values with initialized output even on failure,null/non-null outputs,source aliases,allocation types,two placements,four callback mutation modes. Original and compiled share actual CSurface vtable;stack temporary addresses checked but not compared. COM GetSurfaceLevel/AddRef/Release only boundaries.')
(ROOT/'rebuild/compile-gate/native-texture-surface.json').write_text(json.dumps(report,indent=2)+'\n');print('NATIVE_TEXTURE_SURFACE_GATE_PASS',snapshots,'snapshots')
