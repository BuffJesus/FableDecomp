"""Actual surface assignment and render-target attachment with COM callbacks."""
import hashlib,itertools,json,struct,subprocess,tempfile
from pathlib import Path
from capstone import Cs,CS_ARCH_X86,CS_MODE_32
import check_one as V
from variable_allocator_parity import check
from pe_oracle import EXE,read_virtual_data
ROOT=Path(__file__).resolve().parents[2];w=Path(tempfile.mkdtemp(prefix='native-surface-attachment-'));env=V.env()
names=('00/a1/CRenderTarget_AttachSurfaces_00a10e60.cpp','00/9f/CSurface_Assignment_009f2e70.cpp','00/9f/CSurface_Uninitialise_009f2e20.cpp')
sources=[ROOT/'rebuild/src/compiled'/p for p in names];fixture=ROOT/'rebuild/tests/integration/NativeSurfaceAttachment_test.cpp'
attach='?AttachTarget@CRenderTarget@@QAEXABVCSurface@@@Z';attachZ='?AttachZBuffer@CRenderTarget@@QAEXABVCSurface@@@Z';assign='??4CSurface@@QAEAAV0@ABV0@@Z';surface='?Uninitialise@CSurface@@QAEXXZ'
specs=[(attach,0xa10e60,35,2,0),(attachZ,0xa10e90,35,2,48),(assign,0x9f2e70,76,0,96),(surface,0x9f2e20,32,0,176)]
targets={s:a for s,a,_,_,_ in specs};offsets={a:o for _,a,_,_,o in specs}
(w/'surface_attachment_aliases.h').write_text('\n'.join('#pragma comment(linker,"/alternatename:'+a+'='+b+'")' for a,b in (('@Attach@12',attach),('@AttachZ@12',attachZ),('@Assign@12',assign)))+'\n')
raw=EXE.read_bytes();body=bytearray(208);fixups=[];md=Cs(CS_ARCH_X86,CS_MODE_32);md.detail=True
for s,address,length,_,offset in specs:
 code=read_virtual_data(raw,address,length);body[offset:offset+length]=code
 for i in md.disasm(code,address):
  if i.mnemonic in ('call','jmp') and i.operands[0].type==2:assert i.operands[0].imm in offsets;fixups.append((offset+i.address-address+i.imm_offset,offsets[i.operands[0].imm]))
assert len(fixups)==4
(w/'surface_attachment_retail.h').write_text('static const unsigned char retailCode[]={'+','.join(map(str,body))+'};\nstatic const unsigned retailFixups[][2]={'+','.join('{%d,%d}'%f for f in fixups)+'};\n')
objects=[]
for n,p in enumerate(sources+[fixture]):
 o=w/(str(n)+'.obj');r=V.cl(['/nologo','/c','/O2','/MT','/I'+str(w),'/Fo'+str(o),str(p)],env,w);assert not r.returncode,r.stdout+r.stderr;objects.append(o)
proofs=[check(objects[n],s,a,l,c,targets) for n,(s,a,l,c,_) in zip((0,0,1,2),specs)]
cmd=[str(V.VC/'bin/link.exe'),'/nologo','/out:'+str(w/'fixture.exe'),'kernel32.lib']
for omit in range(3):
 r=subprocess.run(cmd+[str(o) for i,o in enumerate(objects) if i!=omit],env=env,capture_output=True,text=True);assert r.returncode
r=subprocess.run(cmd+list(map(str,objects)),env=env,capture_output=True,text=True);assert not r.returncode,r.stdout+r.stderr
cases=0
with (w/'input.bin').open('wb') as f:
 for base,entry,source_kind,state,mask,mode,same in itertools.product((0,0x800),range(3),range(3),(0,1,2,0xffffffff),range(4),range(4),range(2)):
  a=bytearray([0x95])*4096;dst=base+(0x14 if entry==2 else 4);src=(base+0x100,dst,base+(4 if entry==2 else 0x14))[source_kind]
  struct.pack_into('<3I',a,dst+4,0x24000c00 if mask&1 else 0,0xdeadbeef,0xcafebabe)
  struct.pack_into('<3I',a,src+4,(0x24000c00 if same else 0x24000c20) if mask&2 else 0,state,0x12345678)
  f.write(struct.pack('<4I',base,entry,src,mode));f.write(a);cases+=1
for kind in ('retail','compiled'):
 r=subprocess.run([str(w/'fixture.exe'),str(w/'input.bin'),str(w/(kind+'.bin')),kind],env=env,capture_output=True,text=True,timeout=60);assert not r.returncode,(w,r.returncode,r.stdout,r.stderr);print(r.stdout.strip(),kind,flush=True)
with (w/'retail.bin').open('rb') as f,(w/'compiled.bin').open('rb') as g:
 while True:
  a=f.read(1<<20);b=g.read(1<<20);assert a==b,w
  if not a:break
size=(w/'compiled.bin').stat().st_size;assert size%4096==0;snapshots=size//4096
report=dict(cases=cases,snapshots=snapshots,omission_checks=3,providers=proofs,source_sha256={str(p.relative_to(ROOT)):hashlib.sha256(p.read_bytes()).hexdigest() for p in sources},fixture_sha256=hashlib.sha256(fixture.read_bytes()).hexdigest(),boundary='Actual render-target attachments,actual surface assignment and cleanup. Full4KiB independent event/final model,three entry points,source self/other/separate aliases,allocation states including special2,all pointer-presence combinations,same COM object,two placements,four callback mutation modes. Release/AddRef only external boundaries;checks raw source reloads,conditional allocated-memory copy,post-AddRef mutations and assignment return pointer.')
(ROOT/'rebuild/compile-gate/native-surface-attachment.json').write_text(json.dumps(report,indent=2)+'\n');print('NATIVE_SURFACE_ATTACHMENT_GATE_PASS',snapshots,'snapshots')
