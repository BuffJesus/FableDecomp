"""Hires texture memory resizing, signed budget arithmetic and bank reloads."""
import hashlib,itertools,json,struct,subprocess,tempfile
from pathlib import Path
from capstone import Cs,CS_ARCH_X86,CS_MODE_32
import check_one as V
from variable_allocator_parity import check
from gate_game_component_abi import Coff
from gate_render_state_static_data import coff_data
from pe_oracle import EXE,read_virtual_data
ROOT=Path(__file__).resolve().parents[2];w=Path(tempfile.mkdtemp(prefix='hires-pool-selection-'));env=V.env()
sources=[ROOT/'rebuild/src/compiled'/p for p in ('00/b3/CEngineResourceManager_ResizeHiresTextureMemory_00b3b4f0.cpp','01/43/NEngineInternal_GPSurfaceCompositionManager_01436eac.cpp')];fixture=ROOT/'rebuild/tests/integration/NativeHiresPoolSelection_test.cpp'
sources += [ROOT/'rebuild/src/compiled'/p for p in ('00/9f/CGraphicDataBank_SceneDelay_009fd0c0.cpp','00/9f/CResourceList_IncrementFrameCounter_009fc160.cpp')]
sources += [ROOT/'rebuild/src/compiled'/p for p in ('00/a6/CTextureManager_CalculateTextureSize_00a698f0.cpp','00/9e/CPixelFormat_GetColourDepth_009e3820.cpp','00/9e/CPixelFormat_Initialise_009e3830.cpp','01/29/CPixelFormat_Table_0129ba40.cpp')]
sources += [ROOT/'rebuild/src/compiled/00/b2/CEngineSurfaceCompositionManager_ChangeSceneForTextureBank_00b2ad80.cpp']
sources += [ROOT/'rebuild/src/compiled'/p for p in ('00/a6/CTextureManager_ResizePoolTextureCount_00a6abe0.cpp','00/a6/CTextureManager_GetPoolIndex_00a69730.cpp')]
symbol='?ResizeHiresTextureMemory@CEngineResourceManager@@QAE_NJ@Z';global_symbol='?GPSurfaceCompositionManager@NEngineInternal@@3PAVCEngineSurfaceCompositionManager@@A'
calls={'?ChangeScene@CGraphicDataBank@@QAEXXZ':(0x9fd0c0,'@ChangeScene@8'),'?ChangeSceneForTextureBank@CEngineSurfaceCompositionManager@@QAEXXZ':(0xb2ad80,'@Composition@8'),'?Initialise@CPixelFormat@@QAEXW4_D3DFORMAT@@@Z':(0x9e3830,'@PixelInit@12'),'?CalculateTextureSize@CTextureManager@@SIKKK_NABVCPixelFormat@@@Z':(0xa698f0,'@TextureSize@16'),'?SetReduceSizeFrameDelay@CGraphicDataBank@@QAEXK@Z':(0x9fd100,'@SetDelay@12'),'?ResizePoolTextureCount@CTextureManager@@QAE_NKK@Z':(0xa6abe0,'@ResizePool@16')}
increment='?IncrementFrameCounter@CResourceList@@QAEXK@Z';calls[increment]=(0x9fc160,'@UnusedIncrement@12')
index='?GetPoolIndex@CTextureManager@@AAEJK@Z';frame='?FrameEnd@CTextureManager@@QAEXXZ';pool='?ResizePool@CPreallocTexturePool@@QAE_NK@Z';calls.update({index:(0xa69730,'@UnusedIndex@12'),frame:(0xa698b0,'@Frame@8'),pool:(0xa6a670,'@ResizePool@12')})
targets={s:v[0] for s,v in calls.items()};targets[global_symbol]=0x1436eac
table='?GD3DFormats@@3QBVCD3DFormatInfo@@B';depth='?GetColourDepth@CPixelFormat@@QBEKXZ'
targets[table]=0x129ba40;targets[depth]=0x9e3820
aliases={'@ResizeHires@12':symbol,'_SurfaceGlobal':global_symbol};aliases.update({s:v[1] for s,v in calls.items()});aliases['_FormatTable']=table
(w/'hires_pool_selection_aliases.h').write_text('\n'.join('#pragma comment(linker,"/alternatename:'+a+'='+b+'")' for a,b in aliases.items())+'\n')
raw=EXE.read_bytes();body=read_virtual_data(raw,0xb3b4f0,136)+bytes(8)+read_virtual_data(raw,0x9fd0c0,41)+bytes(7)+read_virtual_data(raw,0x9fd100,19)+bytes(13)+read_virtual_data(raw,0x9fc160,15)+bytes(1)+read_virtual_data(raw,0xa698f0,136)+bytes(8)+read_virtual_data(raw,0x9e3820,13)+bytes(3)+read_virtual_data(raw,0x9e3830,50)+bytes(14)+read_virtual_data(raw,0xb2ad80,11)+bytes(5)+read_virtual_data(raw,0xa6abe0,62)+bytes(2)+read_virtual_data(raw,0xa69730,99);md=Cs(CS_ARCH_X86,CS_MODE_32);md.detail=True;fixups=[]
for i in md.disasm(body[:136],0xb3b4f0):
 if i.mnemonic=='call' and i.operands[0].type==2:
  assert i.operands[0].imm in targets.values();fixups.append((i.address-0xb3b4f0+i.imm_offset,i.operands[0].imm,1))
 else:
  for op in i.operands:
   if op.type==3 and op.mem.disp==0x1436eac:fixups.append((i.address-0xb3b4f0+i.disp_offset,op.mem.disp,0))
assert len(fixups)==8
for off in (17,35):
 assert body[144+off-1]==0xe8 and 0x9fd0c0+off+4+struct.unpack_from('<i',body,144+off)[0]==0x9fc160
 fixups.append((144+off,0x9fc160,1))
for base,start,length in ((0xa698f0,240,136),(0x9e3820,384,13),(0x9e3830,400,50)):
 for i in md.disasm(body[start:start+length],base):
  if i.mnemonic=='call' and i.operands[0].type==2:
   assert i.operands[0].imm==0x9e3820;fixups.append((start+i.address-base+i.imm_offset,i.operands[0].imm,1))
  else:
   for op in i.operands:
    if op.type==3 and op.mem.disp in (0x129ba40,0x129ba48):fixups.append((start+i.address-base+i.disp_offset,op.mem.disp,0))
    if op.type==2 and op.imm==0x129ba48:fixups.append((start+i.address-base+i.imm_offset,op.imm,0))
assert len(fixups)==14
assert body[470]==0xe9 and 0xb2ad80+11+struct.unpack_from("<i",body,471)[0]==0x9fd0c0
fixups.append((471,0x9fd0c0,1))
for i in md.disasm(body[480:542],0xa6abe0):
 if i.mnemonic=='call':
  assert i.operands[0].imm in (0xa69730,0xa698b0,0xa6a670);fixups.append((480+i.address-0xa6abe0+i.imm_offset,i.operands[0].imm,1))
assert len(fixups)==18
(w/'hires_pool_selection_retail.h').write_text('static const unsigned char retailCode[]={'+','.join(map(str,body))+'};\nstatic const unsigned retailFixups[][3]={'+','.join('{%d,%d,%d}'%f for f in fixups)+'};\n')
objects=[]
for n,p in enumerate(sources+[fixture]):
 o=w/(str(n)+'.obj');r=V.cl(['/nologo','/c','/O2','/MT','/I'+str(w),'/Fo'+str(o),str(p)],env,w);assert not r.returncode,r.stdout+r.stderr;objects.append(o)
proof=check(objects[0],symbol,0xb3b4f0,136,8,targets);assert coff_data(Coff(objects[1]),global_symbol,4)==read_virtual_data(raw,0x1436eac,4)==bytes(4)
bankproofs=[check(objects[2],'?ChangeScene@CGraphicDataBank@@QAEXXZ',0x9fd0c0,41,2,{increment:0x9fc160}),check(objects[2],'?SetReduceSizeFrameDelay@CGraphicDataBank@@QAEXK@Z',0x9fd100,19,0,{}),check(objects[3],increment,0x9fc160,15,0,{})]
sizeproofs=[check(objects[4],'?CalculateTextureSize@CTextureManager@@SIKKK_NABVCPixelFormat@@@Z',0xa698f0,136,2,{depth:0x9e3820,table:0x129ba40}),check(objects[5],depth,0x9e3820,13,1,{table:0x129ba40}),check(objects[6],'?Initialise@CPixelFormat@@QAEXW4_D3DFORMAT@@@Z',0x9e3830,50,1,{table:0x129ba40})]
assert coff_data(Coff(objects[7]),table,1692)==read_virtual_data(raw,0x129ba40,1692)
compositionproof=check(objects[8],'?ChangeSceneForTextureBank@CEngineSurfaceCompositionManager@@QAEXXZ',0xb2ad80,11,1,{'?ChangeScene@CGraphicDataBank@@QAEXXZ':0x9fd0c0})
poolproofs=[check(objects[9],'?ResizePoolTextureCount@CTextureManager@@QAE_NKK@Z',0xa6abe0,62,3,{index:0xa69730,frame:0xa698b0,pool:0xa6a670}),check(objects[10],index,0xa69730,99,0,{})]
cmd=[str(V.VC/'bin/link.exe'),'/nologo','/out:'+str(w/'fixture.exe'),'kernel32.lib']
for omit in range(11):
 r=subprocess.run(cmd+[str(o) for i,o in enumerate(objects) if i!=omit],env=env,capture_output=True,text=True);assert r.returncode
r=subprocess.run(cmd+list(map(str,objects)),env=env,capture_output=True,text=True);assert not r.returncode,r.stdout+r.stderr
BASE=0x24000000;cases=0
with (w/'input.bin').open('wb') as f:
 for memory,size,result,mode,bank,delay in itertools.product((0,1,-1,1024,0x7fffffff,0x200000,-0x200001),(174776,),(0,1),range(8),range(2),(0,1,0x7fffffff,0xffffffff)):
  a=bytearray([0x95])*16384;struct.pack_into('<2I',a,0x954,BASE+0x1000,BASE+(0x1400 if bank else 0x1800))
  for j,offset in enumerate((0x1400,0x1800,0x1c00)):
   struct.pack_into('<I',a,offset+0x26c,(delay+13*j)&0xffffffff);struct.pack_into('<I',a,offset+0x280,BASE+0x3000)
  poolcount=(0,1,3,16)[mode%4];struct.pack_into('<I',a,0x3008,poolcount)
  for j in range(poolcount):struct.pack_into('<I',a,0x300c+4*j,(174775 if mode==1 else 174777) if poolcount==1 else (j+1)*65536)
  numerator=(memory<<10)&0xffffffff;numerator=numerator if numerator<0x80000000 else numerator-0x100000000;divisor=size if size<0x80000000 else size-0x100000000
  count=abs(numerator)//abs(divisor);count=-count if (numerator<0)!=(divisor<0) else count
  f.write(struct.pack('<7I',memory&0xffffffff,size,result,mode,bank,count&0xffffffff,delay));f.write(a);cases+=1
for kind in ('retail','compiled'):
 r=subprocess.run([str(w/'fixture.exe'),str(w/'input.bin'),str(w/(kind+'.bin')),kind],env=env,capture_output=True,text=True,timeout=60);assert not r.returncode,(w,r.returncode,r.stdout,r.stderr);print(r.stdout.strip(),kind,flush=True)
with (w/'retail.bin').open('rb') as f,(w/'compiled.bin').open('rb') as g:
 while True:
  a=f.read(1<<20);b=g.read(1<<20);assert a==b,w
  if not a:break
size=(w/'compiled.bin').stat().st_size;assert size%16412==0;snapshots=size//16412
report=dict(cases=cases,snapshots=snapshots,omission_checks=11,providers=[proof,compositionproof]+bankproofs+sizeproofs+poolproofs,source_sha256={str(p.relative_to(ROOT)):hashlib.sha256(p.read_bytes()).hexdigest() for p in sources},fixture_sha256=hashlib.sha256(fixture.read_bytes()).hexdigest(),boundary='Actual hires resize,bank/composition notifications,delay/counter,pixel/size/table,and real texture-pool resize wrapper/private selection. Full16KiB arena plus singleton,immutable table. Ten original spans/18fixups,11mandatory sources. Empty/missing/exact-neighbor/multiple pools;independently expected selection,result and signed count. Frame callback changes pool count or bank texture-manager pointer;pool callback changes main bank;cached manager/index and delay restoration remain correct. TextureManager FrameEnd and CPreallocTexturePool ResizePool remain explicit boundaries.')
(ROOT/'rebuild/compile-gate/native-hires-pool-selection.json').write_text(json.dumps(report,indent=2)+'\n');print('NATIVE_HIRES_POOL_SELECTION_GATE_PASS',snapshots,'snapshots')
