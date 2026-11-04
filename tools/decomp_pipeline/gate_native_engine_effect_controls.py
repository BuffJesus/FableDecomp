"""Native engine effect controls, real singleton storage and PC shadow-buffer defaults."""
import hashlib,itertools,json,struct,subprocess,tempfile
from pathlib import Path
from capstone import Cs,CS_ARCH_X86,CS_MODE_32
import check_one as V
from variable_allocator_parity import check
from gate_game_component_abi import Coff
from gate_render_state_static_data import coff_data
from pe_oracle import EXE,read_virtual_data
ROOT=Path(__file__).resolve().parents[2];w=Path(tempfile.mkdtemp(prefix='native-engine-effect-controls-'));env=V.env()
specs=[[0, '?SetSpotlightShadowMode@CEngine@@UAEX_N@Z', 11680768, 27, 1], [0, '?SetScreenColourFilter@CEngine@@UAEXABVCEngineColourFilterSettings@@@Z', 11681472, 33, 1], [0, '?SetSeaBodyToRender@CEngine@@UAEXVCCharString@@@Z', 11681536, 57, 5], [1, '?DeallocateShadowBuffer@CEngine@@UAE_NXZ', 11681648, 11, 2], [1, '?ReallocateShadowBuffer@CEngine@@UAEXXZ', 11681664, 11, 2], [2, '?XBoxDeallocateShadowBuffer@CEngineShadowRenderer@@QAE_NXZ', 11843728, 3, 0], [2, '?XBoxReallocateShadowBuffer@CEngineShadowRenderer@@QAEXXZ', 11843744, 1, 0]]
targets={'?GPLightingManager@NEngineInternal@@3PAVCEngineLightingManager@@A': 21196444, '?GPScreenEffectColourFilter@NEngineInternal@@3PAVCEngineScreenEffectColourFilter@@A': 21196352, '?GPWaterRenderer@NEngineInternal@@3PAVCEngineWaterRenderer@@A': 21196372, '?GPShadowRenderer@NEngineInternal@@3PAVCEngineShadowRenderer@@A': 21196384, '??0CCharString@@QAE@ABV0@@Z': 10087472, '??4CCharString@@QAEAAV0@ABV0@@Z': 10088368, '??1CCharString@@QAE@XZ': 10087136, '?XBoxDeallocateShadowBuffer@CEngineShadowRenderer@@QAE_NXZ': 11843728, '?XBoxReallocateShadowBuffer@CEngineShadowRenderer@@QAEXXZ': 11843744}
sources=[ROOT/"rebuild/src/compiled"/p for p in ['00/b2/CEngine_EffectControls_00b23c00.cpp', '00/b2/CEngine_ShadowBufferForwarders_00b23f70.cpp', '00/b4/CEngineShadowRenderer_PCBufferDefaults_00b4b890.cpp', '01/43/NEngineInternal_EffectGlobals_01436e40.cpp', '01/43/NEngineInternal_GPLightingManager_01436e9c.cpp', '01/43/NEngineInternal_GPWaterRenderer_01436e54.cpp']]
fixture=ROOT/'rebuild/tests/integration/NativeEngineEffectControls_test.cpp'
aliases={'@Spotlight@12':specs[0][1],'@Filter@12':specs[1][1],'@Sea@12':specs[2][1],'@Deallocate@4':specs[3][1],'@Reallocate@4':specs[4][1],'??0CCharString@@QAE@ABV0@@Z':'@CopyString@12','??4CCharString@@QAEAAV0@ABV0@@Z':'@AssignString@12','??1CCharString@@QAE@XZ':'@DestroyString@4'}
(w/'engine_effect_aliases.h').write_text('\n'.join('#pragma comment(linker,"/alternatename:'+a+'='+b+'")' for a,b in aliases.items())+'\n')
body=bytearray(288);fixups=[];md=Cs(CS_ARCH_X86,CS_MODE_32);md.detail=True;raw=EXE.read_bytes()
for (obj,symbol,address,length,rel),offset in zip(specs,[0,64,128,208,224,256,272]):
 code=read_virtual_data(raw,address,length);body[offset:offset+length]=code
 for i in md.disasm(code,address):
  if i.mnemonic in ('call','jmp') and i.operands[0].type==2:
   assert i.operands[0].imm in targets.values();fixups.append((offset+i.address-address+i.imm_offset,i.operands[0].imm,1))
  else:
   for op in i.operands:
    if op.type==3 and op.mem.disp in (0x1436e9c,0x1436e40,0x1436e54,0x1436e60):fixups.append((offset+i.address-address+i.disp_offset,op.mem.disp,0))
assert len(fixups)==11
(w/'engine_effect_retail.h').write_text('static const unsigned char retailCode[]={'+','.join(map(str,body))+'};\nstatic const unsigned retailFixups[][3]={'+','.join('{%d,%d,%d}'%f for f in fixups)+'};\n')
objects=[]
for n,p in enumerate(sources+[fixture]):
 o=w/(str(n)+'.obj');r=V.cl(['/nologo','/c','/O2','/MT','/I'+str(w),'/Fo'+str(o),str(p)],env,w);assert not r.returncode,r.stdout+r.stderr;objects.append(o)
proofs=[check(objects[i],symbol,address,length,rel,targets) for i,symbol,address,length,rel in specs]
for i,symbol,address in [(3,'?GPScreenEffectColourFilter@NEngineInternal@@3PAVCEngineScreenEffectColourFilter@@A',0x1436e40),(3,'?GPShadowRenderer@NEngineInternal@@3PAVCEngineShadowRenderer@@A',0x1436e60),(4,'?GPLightingManager@NEngineInternal@@3PAVCEngineLightingManager@@A',0x1436e9c),(5,'?GPWaterRenderer@NEngineInternal@@3PAVCEngineWaterRenderer@@A',0x1436e54)]:
 assert coff_data(Coff(objects[i]),symbol,4)==read_virtual_data(raw,address,4)==bytes(4)
cmd=[str(V.VC/'bin/link.exe'),'/nologo','/out:'+str(w/'fixture.exe'),'kernel32.lib']
for omit in range(6):
 r=subprocess.run(cmd+[str(p) for i,p in enumerate(objects) if i!=omit],env=env,capture_output=True,text=True);assert r.returncode
r=subprocess.run(cmd+list(map(str,objects)),env=env,capture_output=True,text=True);assert not r.returncode,r.stdout+r.stderr
cases=0
with (w/'input.bin').open('wb') as f:
 for fill,mode,slot,value,forced,alias,null in itertools.product((0,0x95),range(5),(0,1),(0,1,255,256),(0,1,255),(0,1,2),(0,1)):
  a=bytearray([fill])*32768;off=0x400 if slot else 0x200;a[off+12]=forced;source=0x40 if alias==0 else off+16 if alias==1 else off+20
  for i in range(8):struct.pack_into('<I',a,source+4*i,0x12340000+i)
  struct.pack_into('<I',a,0x100,3);struct.pack_into('<I',a,0x180,3)
  f.write(struct.pack('<6I',mode,slot,value,source,forced,null));f.write(a);cases+=1
for kind in ('retail','compiled'):
 r=subprocess.run([str(w/'fixture.exe'),str(w/'input.bin'),str(w/(kind+'.bin')),kind],env=env,capture_output=True,text=True,timeout=60);assert not r.returncode,(w,r.returncode,r.stdout,r.stderr);print(r.stdout.strip(),kind,flush=True)
with (w/'retail.bin').open('rb') as e,(w/'compiled.bin').open('rb') as f:
 while True:
  x=e.read(1<<20);y=f.read(1<<20);assert x==y,(w,'native mismatch')
  if not x:break
size=(w/'compiled.bin').stat().st_size;assert size%32792==0;snapshots=size//32792
report=dict(cases=cases,snapshots=snapshots,omission_checks=6,providers=proofs,new_data_providers=2,source_sha256={str(p.relative_to(ROOT)):hashlib.sha256(p.read_bytes()).hexdigest() for p in sources},fixture_sha256=hashlib.sha256(fixture.read_bytes()).hexdigest(),boundary='Seven exact native spans/11fixups;four actual singleton pointers with zero-storage audits,two newly defined. Full32KiB snapshots plus four global values. All byte bool semantics, forced/unforced filter, input alias/overlap, alternative singleton locations and string callbacks replacing renderer/argument. Actual shadow forwarders execute actual PC false/empty backends. Spotlight result and applied filter bytes also independently checked. Narrow-string copy/assignment/destruction remain explicit callbacks.')
(ROOT/'rebuild/compile-gate/native-engine-effect-controls.json').write_text(json.dumps(report,indent=2)+'\n');print('NATIVE_ENGINE_EFFECT_CONTROLS_GATE_PASS',snapshots,'snapshots')
