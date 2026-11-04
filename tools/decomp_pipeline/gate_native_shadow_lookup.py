"""Shadow lookup pixel-generation loop with exact unlock and surface cleanup."""
import hashlib,itertools,json,struct,subprocess,tempfile
from pathlib import Path
from capstone import Cs,CS_ARCH_X86,CS_MODE_32
import check_one as V
from variable_allocator_parity import check
from pe_oracle import EXE,read_virtual_data
ROOT=Path(__file__).resolve().parents[2];w=Path(tempfile.mkdtemp(prefix='native-shadow-lookup-'));env=V.env()
sources=[ROOT/'rebuild/src/compiled'/p for p in ('00/b4/CEngineShadowRenderer_BuildDepthPackingLookupTexture_00b4f420.cpp','00/9f/CSurface_Unlock_009f2dc0.cpp','00/9f/CSurface_Uninitialise_009f2e20.cpp')];fixture=ROOT/'rebuild/tests/integration/NativeShadowLookup_test.cpp'
symbol='?BuildDepthPackingLookupTexture@CEngineShadowRenderer@@AAEXXZ'
targets={'??_7CSurface@@6B@':0x122f84c,'?GetSurfaceLevel@CTexture@@QBE?AVCSurface@@J@Z':0x9f9e00,'?Initialise@CPixelFormat@@QAEXJJJJ_N@Z':0x9e3790,'?Initialise@CTexture@@QAE_NABVC2DExtentsI@@JABVCPixelFormat@@KW4ESurfacePool@@_N3@Z':0x9fa280,'?Lock@CSurface@@QAE?AVCSurfaceLockedRect@@K@Z':0x9f33e0,'?SetPixel@CSurface@@QAEXJJABVCRGBColour@@ABVCSurfaceLockedRect@@@Z':0x9f31c0,'?Uninitialise@CSurface@@QAEXXZ':0x9f2e20,'?Unlock@CSurface@@QBEXXZ':0x9f2dc0}

callbacks={0x9e3790:'@InitFormat@28',0x9fa280:'@TextureInit@36',0x9f9e00:'@GetLevel@16',0x9f33e0:'@Lock@16',0x9f31c0:'@SetPixel@24'}
aliases={'@Build@8':symbol,'_SurfaceVtable':'??_7CSurface@@6B@'};aliases.update({s:callbacks[a] for s,a in targets.items() if a in callbacks})
(w/'shadow_lookup_aliases.h').write_text('\n'.join('#pragma comment(linker,"/alternatename:'+a+'='+b+'")' for a,b in aliases.items())+'\n')
raw=EXE.read_bytes();body=bytearray(288);fixups=[];md=Cs(CS_ARCH_X86,CS_MODE_32);md.detail=True
for address,length,offset in ((0xb4f420,231,0),(0x9f2dc0,10,240),(0x9f2e20,32,256)):
 code=read_virtual_data(raw,address,length);body[offset:offset+length]=code
 for i in md.disasm(code,address):
  if i.mnemonic=='call' and i.operands[0].type==2:assert i.operands[0].imm in targets.values();fixups.append((offset+i.address-address+i.imm_offset,i.operands[0].imm,1))
  else:
   for o in i.operands:
    if o.type==2 and o.imm==0x122f84c:fixups.append((offset+i.address-address+i.imm_offset,o.imm,0))
assert len(fixups)==8
(w/'shadow_lookup_retail.h').write_text('static const unsigned char retailCode[]={'+','.join(map(str,body))+'};\nstatic const unsigned retailFixups[][3]={'+','.join('{%d,%d,%d}'%f for f in fixups)+'};\n')
objects=[]
for n,p in enumerate(sources+[fixture]):
 o=w/(str(n)+'.obj');r=V.cl(['/nologo','/c','/O2','/MT','/I'+str(w),'/Fo'+str(o),str(p)],env,w);assert not r.returncode,r.stdout+r.stderr;objects.append(o)
proofs=[check(objects[0],symbol,0xb4f420,231,8,targets),check(objects[1],'?Unlock@CSurface@@QBEXXZ',0x9f2dc0,10,0,{}),check(objects[2],'?Uninitialise@CSurface@@QAEXXZ',0x9f2e20,32,0,{})]
cmd=[str(V.VC/'bin/link.exe'),'/nologo','/out:'+str(w/'fixture.exe'),'kernel32.lib']
for omit in range(3):
 r=subprocess.run(cmd+[str(o) for i,o in enumerate(objects) if i!=omit],env=env,capture_output=True,text=True);assert r.returncode
r=subprocess.run(cmd+list(map(str,objects)),env=env,capture_output=True,text=True);assert not r.returncode,r.stdout+r.stderr
cases=0
with (w/'input.bin').open('wb') as f:
 for p in itertools.product((0,0x800),(1024,1040),range(2),range(4)):f.write(struct.pack('<4I',*p));cases+=1
for kind in ('retail','compiled'):
 r=subprocess.run([str(w/'fixture.exe'),str(w/'input.bin'),str(w/(kind+'.bin')),kind],env=env,capture_output=True,text=True,timeout=120);assert not r.returncode,(w,r.returncode,r.stdout,r.stderr);print(r.stdout.strip(),kind,flush=True)
with (w/'retail.bin').open('rb') as f,(w/'compiled.bin').open('rb') as g:
 while True:
  a=f.read(1<<20);b=g.read(1<<20);assert a==b,w
  if not a:break
snapshots=cases*7;assert (w/'compiled.bin').stat().st_size==snapshots*0xc0000
report=dict(cases=cases,pixel_calls=cases*65536,snapshots=snapshots,omission_checks=3,providers=proofs,source_sha256={str(p.relative_to(ROOT)):hashlib.sha256(p.read_bytes()).hexdigest() for p in sources},fixture_sha256=hashlib.sha256(fixture.read_bytes()).hexdigest(),boundary='Actual lookup builder,Unlock and surface cleanup. Every65536pixel call independently checked for x/y/order/BGRA value;32cases=2097152pixel calls. Seven full768KiB arena snapshots/case preserve both buffers/padding. Two object placements/pitches/allocation results,four mutation modes (output buffer change,pitch change,colour temporary overwrite). Within-run surface/rect/colour identities checked;raw stack pointers excluded. Texture creation/format/get-surface/lock/set-pixel and COM unlock/release remain boundaries. Ignored texture-init/Unlock failures covered;outputs still supplied.')
(ROOT/'rebuild/compile-gate/native-shadow-lookup.json').write_text(json.dumps(report,indent=2)+'\n');print('NATIVE_SHADOW_LOOKUP_GATE_PASS',snapshots,'snapshots')
