"""Native smart video-memory restore retry policy and actual effect singletons."""
import hashlib,itertools,json,struct,subprocess,tempfile
from pathlib import Path
from capstone import Cs,CS_ARCH_X86,CS_MODE_32
import check_one as V
from variable_allocator_parity import check
from gate_game_component_abi import Coff
from gate_render_state_static_data import coff_data
from pe_oracle import EXE,read_virtual_data
ROOT=Path(__file__).resolve().parents[2];w=Path(tempfile.mkdtemp(prefix='native-device-restore-'));env=V.env()
sources=[ROOT/'rebuild/src/compiled'/p for p in ('00/b2/CEngine_SmartRestoreComponentVideoMemory_00b24500.cpp','01/43/NEngineInternal_ScreenEffectGlobals_01436e30.cpp','01/43/NEngineInternal_EffectGlobals_01436e40.cpp','00/b2/CEngine_DeviceReset_00b25d00.cpp')]
fixture=ROOT/'rebuild/tests/integration/NativeDeviceRestore_test.cpp'
symbol='?SmartRestoreComponentVideoMemory@CEngine@@AAE?AW4ESmartRestoreComponentVideoMemoryResult@1@XZ';shadow='?ChangeVideoOptionShadowQuality@CEngineShadowRenderer@@QAE_NJM@Z'
globals={'?GPShadowRenderer@NEngineInternal@@3PAVCEngineShadowRenderer@@A':0x1436e60,'?GPScreenEffectDisplacementRenderer@NEngineInternal@@3PAVCEngineScreenEffectDisplacementRenderer@@A':0x1436e38,'?GPScreenEffectGlowRenderer@NEngineInternal@@3PAVCEngineScreenEffectGlowRenderer@@A':0x1436e34,'?GPScreenEffectRadialBlurRenderer@NEngineInternal@@3PAVCEngineScreenEffectRadialBlurRenderer@@A':0x1436e30}
targets=dict(globals);targets[shadow]=0xb4fc10
aliases={'@SmartRestore@8':symbol,'@PostReset@8':'?OnPostDeviceReset@CEngine@@UAE_NXZ',shadow:'@Shadow@16'};aliases.update(dict(zip(('_ShadowGlobal','_DisplacementGlobal','_GlowGlobal','_RadialGlobal'),globals)))
(w/'device_restore_aliases.h').write_text('\n'.join('#pragma comment(linker,"/alternatename:'+a+'='+b+'")' for a,b in aliases.items())+'\n')
raw=EXE.read_bytes();body=bytearray(read_virtual_data(raw,0xb24500,399))+bytearray(17)+bytearray(read_virtual_data(raw,0xb25d00,32));md=Cs(CS_ARCH_X86,CS_MODE_32);md.detail=True;fixups=[]
for i in md.disasm(body[:399],0xb24500):
 if i.mnemonic=='call' and i.operands[0].type==2:assert i.operands[0].imm==0xb4fc10;fixups.append((i.address-0xb24500+i.imm_offset,i.operands[0].imm,1))
 else:
  for op in i.operands:
   if op.type==3 and op.mem.disp in globals.values():fixups.append((i.address-0xb24500+i.disp_offset,op.mem.disp,0))
assert len(fixups)==5
assert body[429]==0xe8 and 0xb25d00+18+struct.unpack_from("<i",body,430)[0]==0xb24500
fixups.append((430,0xb24500,1))
(w/'device_restore_retail.h').write_text('static const unsigned char retailCode[]={'+','.join(map(str,body))+'};\nstatic const unsigned retailFixups[][3]={'+','.join('{%d,%d,%d}'%f for f in fixups)+'};\n')
objects=[]
for n,p in enumerate(sources+[fixture]):
 o=w/(str(n)+'.obj');r=V.cl(['/nologo','/c','/O2','/MT','/I'+str(w),'/Fo'+str(o),str(p)],env,w);assert not r.returncode,r.stdout+r.stderr;objects.append(o)
proof=check(objects[0],symbol,0xb24500,399,5,targets)
postproof=check(objects[3],"?OnPostDeviceReset@CEngine@@UAE_NXZ",0xb25d00,32,1,{symbol:0xb24500})
for s,a in globals.items():assert coff_data(Coff(objects[2 if a==0x1436e60 else 1]),s,4)==read_virtual_data(raw,a,4)==bytes(4)
cmd=[str(V.VC/'bin/link.exe'),'/nologo','/out:'+str(w/'fixture.exe'),'kernel32.lib']
for omit in range(4):
 r=subprocess.run(cmd+[str(p) for i,p in enumerate(objects) if i!=omit],env=env,capture_output=True,text=True);assert r.returncode
r=subprocess.run(cmd+list(map(str,objects)),env=env,capture_output=True,text=True);assert not r.returncode,r.stdout+r.stderr
def expected(shadow,effects,mask,failure,mode,count):
 reduced=False
 for iteration in range(10):
  start=1 if mode==3 and count else 0
  if (failure==6 and count>start) or (failure==7 and start<=1<count):return 2
  if mask&1 and mode==1:mask&=3
  failed=(bool(mask&1) and (failure==5 or (failure==1 and shadow>1)))
  failed|=bool(mask&2) and (failure==5 or (failure==2 and effects>0))
  failed|=bool(mask&4) and (failure==5 or (failure==3 and effects>1))
  failed|=bool(mask&8) and (failure==5 or (failure==4 and effects>0))
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
  mode=(shadow+effects+mask+failure)%5;count=(shadow+2*effects+mask+failure)%4;a=bytearray([0x95])*4096
  struct.pack_into('<2I',a,0xe8,shadow,effects);struct.pack_into('<3I',a,0x168,BASE+0x200,BASE+0x200+count*4,BASE+0x20c)
  for j in range(3):struct.pack_into('<I',a,0x200+4*j,BASE+0x300+32*j)
  for j in range(4):struct.pack_into('<I',a,0x120+4*j,BASE+0x400+128*j);struct.pack_into('<2I',a,0x464+128*j,1000+j,0x3f800000+j)
  result=expected(shadow,effects,mask,failure,mode,count)
  for entry,enabled in ((0,0),(1,0),(1,255)):
   f.write(struct.pack('<9I',shadow,effects,mask,failure,mode,count,result,entry,enabled));f.write(a);cases+=1
for kind in ('retail','compiled'):
 r=subprocess.run([str(w/'fixture.exe'),str(w/'input.bin'),str(w/(kind+'.bin')),kind],env=env,capture_output=True,text=True,timeout=60);assert not r.returncode,(w,r.returncode,r.stdout,r.stderr);print(r.stdout.strip(),kind,flush=True)
with (w/'retail.bin').open('rb') as f,(w/'compiled.bin').open('rb') as g:
 while True:
  a=f.read(1<<20);b=g.read(1<<20);assert a==b,w
  if not a:break
size=(w/'compiled.bin').stat().st_size;assert size%4120==0;snapshots=size//4120
report=dict(cases=cases,snapshots=snapshots,omission_checks=4,providers=[proof,postproof],source_sha256={str(p.relative_to(ROOT)):hashlib.sha256(p.read_bytes()).hexdigest() for p in sources},fixture_sha256=hashlib.sha256(fixture.read_bytes()).hexdigest(),boundary='Composed actual secondary-base post-reset callback and smart restore, plus direct entry, disabled/enabled reset. Four mandatory providers,two native spans/sixfixups. Full4KiB arena plus four singleton words at every callback/final return. All16 quality pairs/all16 singleton-presence masks/eight failure policies, independently expected success/reduced/failure outcomes. Sampled zero-to-three component arrays,release callback advances begin,restore changes original settings without changing cached retry state,shadow clears later globals,displacement replaces radial renderer. Actual three new BSS globals and existing shadow global audited. Component release/restore,shadow allocation and effect-quality allocation remain explicit boundaries.')
(ROOT/'rebuild/compile-gate/native-device-restore.json').write_text(json.dumps(report,indent=2)+'\n');print('NATIVE_DEVICE_RESTORE_GATE_PASS',snapshots,'snapshots')
