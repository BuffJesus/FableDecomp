"""Actual glow/radial/displacement quality and resource-release methods."""
import hashlib,itertools,json,struct,subprocess,tempfile
from pathlib import Path
from capstone import Cs,CS_ARCH_X86,CS_MODE_32
import check_one as V
from variable_allocator_parity import check
from gate_game_component_abi import Coff
from gate_render_state_static_data import coff_data
from pe_oracle import EXE,read_virtual_data
ROOT=Path(__file__).resolve().parents[2];w=Path(tempfile.mkdtemp(prefix='native-effect-quality-'));env=V.env()
specs=[[0, '?SetEffectQuality@CEngineScreenEffectGlowRenderer@@UAE_NABVCEngineVideoOptionsDef@@@Z', 11922944, 114, 5], [0, '?OnReleaseDefaultPoolResources@CEngineScreenEffectGlowRenderer@@UAEXXZ', 11922528, 8, 0], [1, '?SetEffectQuality@CEngineScreenEffectRadialBlurRenderer@@UAE_NABVCEngineVideoOptionsDef@@@Z', 11911120, 113, 5], [1, '?OnReleaseDefaultPoolResources@CEngineScreenEffectRadialBlurRenderer@@UAEXXZ', 11910400, 8, 0], [2, '?SetEffectQuality@CEngineScreenEffectDisplacementRenderer@@UAE_NABVCEngineVideoOptionsDef@@@Z', 12086016, 163, 5], [2, '?OnReleaseDefaultPoolResources@CEngineScreenEffectDisplacementRenderer@@UAEXXZ', 12085616, 8, 0]]
targets={'?GPResourceManager@NEngineInternal@@3PAVCEngineResourceManager@@A': 21196440, '?FreeMemoryPage@CGPUScratchBufferTextureManager@@QAEXJ@Z': 15953968, '?InitialiseTextures@CEngineScreenEffectGlowRenderer@@AAE_NXZ': 11922656, '?InitialiseTextures@CEngineScreenEffectRadialBlurRenderer@@AAE_NXZ': 11910928, '?InitialiseTextures@CEngineScreenEffectDisplacementRenderer@@AAE_NXZ': 12085872}
source_names=['00/b5/CEngineScreenEffectGlowRenderer_Quality_00b5ee00.cpp', '00/b5/CEngineScreenEffectRadialBlurRenderer_Quality_00b5bfd0.cpp', '00/b8/CEngineScreenEffectDisplacementRenderer_Quality_00b86b00.cpp']
sources=[ROOT/'rebuild/src/compiled'/p for p in source_names+['01/43/NEngineInternal_GPResourceManager_01436e98.cpp']]
fixture=ROOT/'rebuild/tests/integration/NativeEffectQuality_test.cpp'
aliases=dict(zip(('@GlowQuality@12','@GlowRelease@8','@RadialQuality@12','@RadialRelease@8','@DisplacementQuality@12','@DisplacementRelease@8'),[s[1] for s in specs]))
global_symbol='?GPResourceManager@NEngineInternal@@3PAVCEngineResourceManager@@A';aliases['_ResourceGlobal']=global_symbol
aliases['?FreeMemoryPage@CGPUScratchBufferTextureManager@@QAEXJ@Z']='@FreePage@12'
for owner,callback in [('CEngineScreenEffectGlowRenderer','InitGlow'),('CEngineScreenEffectRadialBlurRenderer','InitRadial'),('CEngineScreenEffectDisplacementRenderer','InitDisplacement')]:aliases['?InitialiseTextures@'+owner+'@@AAE_NXZ']='@'+callback+'@8'
(w/'effect_quality_aliases.h').write_text('\n'.join('#pragma comment(linker,"/alternatename:'+a+'='+b+'")' for a,b in aliases.items())+'\n')
raw=EXE.read_bytes();body=bytearray(512);fixups=[];md=Cs(CS_ARCH_X86,CS_MODE_32);md.detail=True
for (_,symbol,address,length,_),offset in zip(specs,(0,128,160,288,320,496)):
 code=read_virtual_data(raw,address,length);body[offset:offset+length]=code
 for i in md.disasm(code,address):
  if i.mnemonic=='call' and i.operands[0].type==2:assert i.operands[0].imm in targets.values();fixups.append((offset+i.address-address+i.imm_offset,i.operands[0].imm,1))
  else:
   for op in i.operands:
    if op.type==3 and op.mem.disp==0x1436e98:fixups.append((offset+i.address-address+i.disp_offset,op.mem.disp,0))
assert len(fixups)==15
(w/'effect_quality_retail.h').write_text('static const unsigned char retailCode[]={'+','.join(map(str,body))+'};\nstatic const unsigned retailFixups[][3]={'+','.join('{%d,%d,%d}'%f for f in fixups)+'};\n')
objects=[]
for n,p in enumerate(sources+[fixture]):
 o=w/(str(n)+'.obj');r=V.cl(['/nologo','/c','/O2','/MT','/I'+str(w),'/Fo'+str(o),str(p)],env,w);assert not r.returncode,r.stdout+r.stderr;objects.append(o)
proofs=[check(objects[i],s,a,size,count,targets) for i,s,a,size,count in specs]
assert coff_data(Coff(objects[3]),global_symbol,4)==read_virtual_data(raw,0x1436e98,4)==bytes(4)
cmd=[str(V.VC/'bin/link.exe'),'/nologo','/out:'+str(w/'fixture.exe'),'kernel32.lib']
for omit in range(4):
 r=subprocess.run(cmd+[str(p) for i,p in enumerate(objects) if i!=omit],env=env,capture_output=True,text=True);assert r.returncode
r=subprocess.run(cmd+list(map(str,objects)),env=env,capture_output=True,text=True);assert not r.returncode,r.stdout+r.stderr
cases=0
with (w/'input.bin').open('wb') as f:
 for kind,flags,supported,result,page,mode in itertools.product(range(3),range(16),(0,1,255),(0,1),(-1,0,31,0x7fffffff),range(4)):
  a=bytearray([0x95])*20480;owner=(0x400,0x800,0xc00)[kind];a[owner+(0x21c,0x1c8,0xed)[kind]]=supported;struct.pack_into('<i',a,owner+(0x30,0x1c,0x18)[kind],page)
  for j in range(4):a[0x16c+j]=255 if flags&(1<<j) else 0
  f.write(struct.pack('<6I',kind,flags,supported,result,page&0xffffffff,mode));f.write(a);cases+=1
for kind in ('retail','compiled'):
 r=subprocess.run([str(w/'fixture.exe'),str(w/'input.bin'),str(w/(kind+'.bin')),kind],env=env,capture_output=True,text=True,timeout=60);assert not r.returncode,(w,r.returncode,r.stdout,r.stderr);print(r.stdout.strip(),kind,flush=True)
with (w/'retail.bin').open('rb') as f,(w/'compiled.bin').open('rb') as g:
 while True:
  a=f.read(1<<20);b=g.read(1<<20);assert a==b,w
  if not a:break
size=(w/'compiled.bin').stat().st_size;assert size%20492==0;snapshots=size//20492
report=dict(cases=cases,snapshots=snapshots,omission_checks=4,providers=proofs,source_sha256={str(p.relative_to(ROOT)):hashlib.sha256(p.read_bytes()).hexdigest() for p in sources},fixture_sha256=hashlib.sha256(fixture.read_bytes()).hexdigest(),boundary='Six exact bodies/15fixups and actual resource singleton zero-storage audit. Full20KiB arena plus global/return at each allocation/free/release/quality boundary. All16 requested flag combinations,raw support0/1/255,allocation success/failure,signed page values,callbacks changing page/global/render state. Actual release runs between two quality calls; all returns and release markers independently checked. InitialiseTextures and FreeMemoryPage remain explicit boundaries.')
(ROOT/'rebuild/compile-gate/native-effect-quality.json').write_text(json.dumps(report,indent=2)+'\n');print('NATIVE_EFFECT_QUALITY_GATE_PASS',snapshots,'snapshots')
