"""Hires texture memory resizing, signed budget arithmetic and bank reloads."""
import hashlib,itertools,json,struct,subprocess,tempfile
from pathlib import Path
from capstone import Cs,CS_ARCH_X86,CS_MODE_32
import check_one as V
from variable_allocator_parity import check
from gate_game_component_abi import Coff
from gate_render_state_static_data import coff_data
from pe_oracle import EXE,read_virtual_data
ROOT=Path(__file__).resolve().parents[2];w=Path(tempfile.mkdtemp(prefix='hires-texture-memory-'));env=V.env()
sources=[ROOT/'rebuild/src/compiled'/p for p in ('00/b3/CEngineResourceManager_ResizeHiresTextureMemory_00b3b4f0.cpp','01/43/NEngineInternal_GPSurfaceCompositionManager_01436eac.cpp')];fixture=ROOT/'rebuild/tests/integration/NativeHiresTextureMemory_test.cpp'
symbol='?ResizeHiresTextureMemory@CEngineResourceManager@@QAE_NJ@Z';global_symbol='?GPSurfaceCompositionManager@NEngineInternal@@3PAVCEngineSurfaceCompositionManager@@A'
calls={'?ChangeScene@CGraphicDataBank@@QAEXXZ':(0x9fd0c0,'@ChangeScene@8'),'?ChangeSceneForTextureBank@CEngineSurfaceCompositionManager@@QAEXXZ':(0xb2ad80,'@Composition@8'),'?Initialise@CPixelFormat@@QAEXW4_D3DFORMAT@@@Z':(0x9e3830,'@PixelInit@12'),'?CalculateTextureSize@CTextureManager@@SIKKK_NABVCPixelFormat@@@Z':(0xa698f0,'@TextureSize@16'),'?SetReduceSizeFrameDelay@CGraphicDataBank@@QAEXK@Z':(0x9fd100,'@SetDelay@12'),'?ResizePoolTextureCount@CTextureManager@@QAE_NKK@Z':(0xa6abe0,'@ResizePool@16')}
targets={s:v[0] for s,v in calls.items()};targets[global_symbol]=0x1436eac
aliases={'@ResizeHires@12':symbol,'_SurfaceGlobal':global_symbol};aliases.update({s:v[1] for s,v in calls.items()})
(w/'hires_texture_memory_aliases.h').write_text('\n'.join('#pragma comment(linker,"/alternatename:'+a+'='+b+'")' for a,b in aliases.items())+'\n')
raw=EXE.read_bytes();body=read_virtual_data(raw,0xb3b4f0,136);md=Cs(CS_ARCH_X86,CS_MODE_32);md.detail=True;fixups=[]
for i in md.disasm(body,0xb3b4f0):
 if i.mnemonic=='call' and i.operands[0].type==2:
  assert i.operands[0].imm in targets.values();fixups.append((i.address-0xb3b4f0+i.imm_offset,i.operands[0].imm,1))
 else:
  for op in i.operands:
   if op.type==3 and op.mem.disp==0x1436eac:fixups.append((i.address-0xb3b4f0+i.disp_offset,op.mem.disp,0))
assert len(fixups)==8
(w/'hires_texture_memory_retail.h').write_text('static const unsigned char retailCode[]={'+','.join(map(str,body))+'};\nstatic const unsigned retailFixups[][3]={'+','.join('{%d,%d,%d}'%f for f in fixups)+'};\n')
objects=[]
for n,p in enumerate(sources+[fixture]):
 o=w/(str(n)+'.obj');r=V.cl(['/nologo','/c','/O2','/MT','/I'+str(w),'/Fo'+str(o),str(p)],env,w);assert not r.returncode,r.stdout+r.stderr;objects.append(o)
proof=check(objects[0],symbol,0xb3b4f0,136,8,targets);assert coff_data(Coff(objects[1]),global_symbol,4)==read_virtual_data(raw,0x1436eac,4)==bytes(4)
cmd=[str(V.VC/'bin/link.exe'),'/nologo','/out:'+str(w/'fixture.exe'),'kernel32.lib']
for omit in (0,1):
 r=subprocess.run(cmd+[str(o) for i,o in enumerate(objects) if i!=omit],env=env,capture_output=True,text=True);assert r.returncode
r=subprocess.run(cmd+list(map(str,objects)),env=env,capture_output=True,text=True);assert not r.returncode,r.stdout+r.stderr
BASE=0x24000000;cases=0
with (w/'input.bin').open('wb') as f:
 for memory,size,result,mode,bank,delay in itertools.product((0,1,-1,1024,0x7fffffff,0x200000,-0x200001),(1,3,174776,0x80000000),(0,1),range(8),range(2),(0,1,0x7fffffff,0xffffffff)):
  a=bytearray([0x95])*16384;struct.pack_into('<2I',a,0x954,BASE+0x1000,BASE+(0x1400 if bank else 0x1800))
  for j,offset in enumerate((0x1400,0x1800,0x1c00)):
   struct.pack_into('<I',a,offset+0x26c,(delay+13*j)&0xffffffff);struct.pack_into('<I',a,offset+0x280,BASE+0x3000)
  numerator=(memory<<10)&0xffffffff;numerator=numerator if numerator<0x80000000 else numerator-0x100000000;divisor=size if size<0x80000000 else size-0x100000000
  count=abs(numerator)//abs(divisor);count=-count if (numerator<0)!=(divisor<0) else count
  f.write(struct.pack('<7I',memory&0xffffffff,size,result,mode,bank,count&0xffffffff,delay));f.write(a);cases+=1
for kind in ('retail','compiled'):
 r=subprocess.run([str(w/'fixture.exe'),str(w/'input.bin'),str(w/(kind+'.bin')),kind],env=env,capture_output=True,text=True,timeout=60);assert not r.returncode,(w,r.returncode,r.stdout,r.stderr);print(r.stdout.strip(),kind,flush=True)
with (w/'retail.bin').open('rb') as f,(w/'compiled.bin').open('rb') as g:
 while True:
  a=f.read(1<<20);b=g.read(1<<20);assert a==b,w
  if not a:break
snapshots=cases*8;assert (w/'compiled.bin').stat().st_size==snapshots*16412
report=dict(cases=cases,snapshots=snapshots,omission_checks=2,providers=[proof],new_data_providers=1,source_sha256={str(p.relative_to(ROOT)):hashlib.sha256(p.read_bytes()).hexdigest() for p in sources},fixture_sha256=hashlib.sha256(fixture.read_bytes()).hexdigest(),boundary='Full16KiB arena plus actual surface-composition singleton. Seven signed memory budgets,four texture sizes including signed-negative denominator,two resize results,eight mutation modes,two initial banks,four delay values. Independently expected wrapped signed quotient,exact callbacks/order/receivers/arguments,original delay restored after bank replacement. Pixel format stack address is verified for identity within each run;only its initialized value is recorded. Underlying bank/composition/pixel/texture methods remain explicit boundaries.')
(ROOT/'rebuild/compile-gate/native-hires-texture-memory.json').write_text(json.dumps(report,indent=2)+'\n');print('NATIVE_HIRES_TEXTURE_MEMORY_GATE_PASS',snapshots,'snapshots')
