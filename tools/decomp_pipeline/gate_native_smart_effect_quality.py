"""Native smart video-memory restore retry policy and actual effect singletons."""
import hashlib,itertools,json,struct,subprocess,tempfile
from pathlib import Path
from capstone import Cs,CS_ARCH_X86,CS_MODE_32
import check_one as V
from variable_allocator_parity import check
from gate_game_component_abi import Coff
from gate_render_state_static_data import coff_data
from pe_oracle import EXE,read_virtual_data
ROOT=Path(__file__).resolve().parents[2];w=Path(tempfile.mkdtemp(prefix='native-smart-effect-quality-'));env=V.env()
sources=[ROOT/'rebuild/src/compiled'/p for p in ('00/b2/CEngine_SmartRestoreComponentVideoMemory_00b24500.cpp','01/43/NEngineInternal_ScreenEffectGlobals_01436e30.cpp','01/43/NEngineInternal_EffectGlobals_01436e40.cpp')]
sources.extend(ROOT/'rebuild/src/compiled'/p for p in ['00/b5/CEngineScreenEffectGlowRenderer_Quality_00b5ee00.cpp', '00/b5/CEngineScreenEffectRadialBlurRenderer_Quality_00b5bfd0.cpp', '00/b8/CEngineScreenEffectDisplacementRenderer_Quality_00b86b00.cpp', '01/43/NEngineInternal_GPResourceManager_01436e98.cpp'])
quality_specs=[[0, '?SetEffectQuality@CEngineScreenEffectGlowRenderer@@UAE_NABVCEngineVideoOptionsDef@@@Z', 11922944, 114, 5], [1, '?SetEffectQuality@CEngineScreenEffectRadialBlurRenderer@@UAE_NABVCEngineVideoOptionsDef@@@Z', 11911120, 113, 5], [2, '?SetEffectQuality@CEngineScreenEffectDisplacementRenderer@@UAE_NABVCEngineVideoOptionsDef@@@Z', 12086016, 163, 5]]
quality_targets={'?GPResourceManager@NEngineInternal@@3PAVCEngineResourceManager@@A': 21196440, '?FreeMemoryPage@CGPUScratchBufferTextureManager@@QAEXJ@Z': 15953968, '?InitialiseTextures@CEngineScreenEffectGlowRenderer@@AAE_NXZ': 11922656, '?InitialiseTextures@CEngineScreenEffectRadialBlurRenderer@@AAE_NXZ': 11910928, '?InitialiseTextures@CEngineScreenEffectDisplacementRenderer@@AAE_NXZ': 12085872}
fixture=ROOT/'rebuild/tests/integration/NativeSmartEffectQuality_test.cpp'
symbol='?SmartRestoreComponentVideoMemory@CEngine@@AAE?AW4ESmartRestoreComponentVideoMemoryResult@1@XZ';shadow='?ChangeVideoOptionShadowQuality@CEngineShadowRenderer@@QAE_NJM@Z'
globals={'?GPShadowRenderer@NEngineInternal@@3PAVCEngineShadowRenderer@@A':0x1436e60,'?GPScreenEffectDisplacementRenderer@NEngineInternal@@3PAVCEngineScreenEffectDisplacementRenderer@@A':0x1436e38,'?GPScreenEffectGlowRenderer@NEngineInternal@@3PAVCEngineScreenEffectGlowRenderer@@A':0x1436e34,'?GPScreenEffectRadialBlurRenderer@NEngineInternal@@3PAVCEngineScreenEffectRadialBlurRenderer@@A':0x1436e30}
targets=dict(globals);targets[shadow]=0xb4fc10
aliases={'@SmartRestore@8':symbol,shadow:'@Shadow@16'};aliases.update(dict(zip(('_ShadowGlobal','_DisplacementGlobal','_GlowGlobal','_RadialGlobal'),globals)))
aliases['_ResourceGlobal']='?GPResourceManager@NEngineInternal@@3PAVCEngineResourceManager@@A'
for (obj,symbol_,address,length,count),alias in zip(quality_specs,('@GlowQuality@12','@RadialQuality@12','@DisplacementQuality@12')):aliases[alias]=symbol_
for owner,callback in [('CEngineScreenEffectGlowRenderer','InitGlow'),('CEngineScreenEffectRadialBlurRenderer','InitRadial'),('CEngineScreenEffectDisplacementRenderer','InitDisplacement')]:aliases['?InitialiseTextures@'+owner+'@@AAE_NXZ']='@'+callback+'@8'
aliases['?FreeMemoryPage@CGPUScratchBufferTextureManager@@QAEXJ@Z']='@FreePage@12'
(w/'smart_effect_quality_aliases.h').write_text('\n'.join('#pragma comment(linker,"/alternatename:'+a+'='+b+'")' for a,b in aliases.items())+'\n')
raw=EXE.read_bytes();body=bytearray(read_virtual_data(raw,0xb24500,399))+bytearray(545);md=Cs(CS_ARCH_X86,CS_MODE_32);md.detail=True;fixups=[]
for i in md.disasm(body[:399],0xb24500):
 if i.mnemonic=='call' and i.operands[0].type==2:assert i.operands[0].imm==0xb4fc10;fixups.append((i.address-0xb24500+i.imm_offset,i.operands[0].imm,1))
 else:
  for op in i.operands:
   if op.type==3 and op.mem.disp in globals.values():fixups.append((i.address-0xb24500+i.disp_offset,op.mem.disp,0))
assert len(fixups)==5
for (_,s_,address,length,_),offset in zip(quality_specs,(512,640,768)):
 code=read_virtual_data(raw,address,length);body[offset:offset+length]=code
 for i in md.disasm(code,address):
  if i.mnemonic=='call' and i.operands[0].type==2:assert i.operands[0].imm in quality_targets.values();fixups.append((offset+i.address-address+i.imm_offset,i.operands[0].imm,1))
  else:
   for op in i.operands:
    if op.type==3 and op.mem.disp==0x1436e98:fixups.append((offset+i.address-address+i.disp_offset,op.mem.disp,0))
assert len(fixups)==20
(w/'smart_effect_quality_retail.h').write_text('static const unsigned char retailCode[]={'+','.join(map(str,body))+'};\nstatic const unsigned retailFixups[][3]={'+','.join('{%d,%d,%d}'%f for f in fixups)+'};\n')
objects=[]
for n,p in enumerate(sources+[fixture]):
 o=w/(str(n)+'.obj');r=V.cl(['/nologo','/c','/O2','/MT','/I'+str(w),'/Fo'+str(o),str(p)],env,w);assert not r.returncode,r.stdout+r.stderr;objects.append(o)
proof=check(objects[0],symbol,0xb24500,399,5,targets)
quality_proofs=[check(objects[3+i],s,a,n,c,quality_targets) for i,s,a,n,c in quality_specs]
assert coff_data(Coff(objects[6]),aliases["_ResourceGlobal"],4)==read_virtual_data(raw,0x1436e98,4)==bytes(4)
for s,a in globals.items():assert coff_data(Coff(objects[2 if a==0x1436e60 else 1]),s,4)==read_virtual_data(raw,a,4)==bytes(4)
cmd=[str(V.VC/'bin/link.exe'),'/nologo','/out:'+str(w/'fixture.exe'),'kernel32.lib']
for omit in range(7):
 r=subprocess.run(cmd+[str(p) for i,p in enumerate(objects) if i!=omit],env=env,capture_output=True,text=True);assert r.returncode
r=subprocess.run(cmd+list(map(str,objects)),env=env,capture_output=True,text=True);assert not r.returncode,r.stdout+r.stderr
def expected(shadow,effects,mask,failure,mode,count):
 reduced=False
 for iteration in range(10):
  start=1 if mode==3 and count else 0
  if (failure==6 and count>start) or (failure==7 and start<=1<count):return 2
  if mask&1 and mode==1:mask&=3
  failed=(bool(mask&1) and (failure==5 or (failure==1 and shadow>1)))
  failed|=bool(mask&2) and ((failure==5 and effects>0) or (failure==2 and effects>0))
  failed|=bool(mask&4) and ((failure==5 and effects>0) or (failure==3 and effects>1))
  failed|=bool(mask&8) and ((failure==5 and effects>0) or (failure==4 and effects>0))
  if not failed:return int(reduced)
  reduced=True
  if shadow>1:shadow-=1
  elif effects>0:effects-=1
  elif shadow>0:shadow-=1
  else:return 2
 raise AssertionError('unexpected retry loop')
cases=0;BASE=0x24000000
with (w/'input.bin').open('wb') as f:
 for shadow,effects,mask,failure in itertools.product(range(4),range(4),range(16),range(8)):
  mode=(shadow+effects+mask+failure)%5;count=(shadow+2*effects+mask+failure)%4;a=bytearray([0x95])*32768
  struct.pack_into('<2I',a,0xe8,shadow,effects);struct.pack_into('<3I',a,0x168,BASE+0x200,BASE+0x200+count*4,BASE+0x20c)
  for j in range(3):struct.pack_into('<I',a,0x200+4*j,BASE+0x300+32*j)
  for j in range(4):struct.pack_into('<I',a,0x120+4*j,BASE+0x400+128*j);struct.pack_into('<2I',a,0x464+128*j,1000+j,0x3f800000+j)
  for j in range(4):a[0x46c+128*j:0x470+128*j]=bytes([255 if j else 0])*4
  for owner,support,page in ((0x1000,0xed,0x18),(0x1400,0x21c,0x30),(0x1800,0x1c8,0x1c),(0x1c00,0x1c8,0x1c)):
   a[owner+support]=1;struct.pack_into('<i',a,owner+page,31 if mask&1 else -1)
  result=expected(shadow,effects,mask,failure,mode,count);f.write(struct.pack('<7I',shadow,effects,mask,failure,mode,count,result));f.write(a);cases+=1
for kind in ('retail','compiled'):
 r=subprocess.run([str(w/'fixture.exe'),str(w/'input.bin'),str(w/(kind+'.bin')),kind],env=env,capture_output=True,text=True,timeout=60);assert not r.returncode,(w,r.returncode,r.stdout,r.stderr);print(r.stdout.strip(),kind,flush=True)
with (w/'retail.bin').open('rb') as f,(w/'compiled.bin').open('rb') as g:
 while True:
  a=f.read(1<<20);b=g.read(1<<20);assert a==b,w
  if not a:break
size=(w/'compiled.bin').stat().st_size;assert size%32796==0;snapshots=size//32796
report=dict(cases=cases,snapshots=snapshots,omission_checks=7,providers=[proof]+quality_proofs,source_sha256={str(p.relative_to(ROOT)):hashlib.sha256(p.read_bytes()).hexdigest() for p in sources},fixture_sha256=hashlib.sha256(fixture.read_bytes()).hexdigest(),boundary='Composed actual smart retry policy and three effect quality bodies;four original spans/20fixups,seven mandatory providers. Full32KiB and five actual singleton words. All16 quality pairs/presence masks/eight failure policies with independent final outcomes. Real effect enabled/disabled checks,render flags,page cleanup,glow ordering,displacement cached enable flags. Common dispatch vtables avoid pointer masking. Component callbacks,shadow allocator,InitialiseTextures and FreeMemoryPage remain explicit boundaries.')
(ROOT/'rebuild/compile-gate/native-smart-effect-quality.json').write_text(json.dumps(report,indent=2)+'\n');print('NATIVE_SMART_EFFECT_QUALITY_GATE_PASS',snapshots,'snapshots')
