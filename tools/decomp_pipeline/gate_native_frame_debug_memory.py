import hashlib,json,struct,subprocess,sys,tempfile
from pathlib import Path
ROOT=Path(__file__).resolve().parents[2];sys.path.insert(0,str(ROOT/'tools/decomp_pipeline'))
import check_one as V
from variable_allocator_parity import check
from pe_oracle import EXE,read_virtual_data
from gate_game_component_abi import Coff
from capstone import Cs,CS_ARCH_X86,CS_MODE_32
w=Path(tempfile.mkdtemp(prefix='native-frame-debug-memory-'));env=V.env()
specs=[('00/9f/CGraphicInfoFrame_CFrameData_GetDebugMemoryInUse_009fdcb0.cpp','?GetDebugMemoryInUse@CFrameData@CGraphicInfoFrame@@UBEKXZ',0x9fdcb0,226,0,7),('00/9f/CTexture_CopyFromTexture_009fa4e0.cpp','?GetNoLevels@CTexture@@QBEJXZ',0x9f9df0,9,256,0),('00/9f/CTexture_GetSurfaceLevel_009f9e00.cpp','?GetSurfaceLevel@CTexture@@QBE?AVCSurface@@J@Z',0x9f9e00,96,288,5),('00/9f/CSurface_CopyConstructor_009f2d60.cpp','??0CSurface@@QAE@ABV0@@Z',0x9f2d60,55,384,1),('00/9f/CSurface_InitialiseFromTextureMipmap_009f2f10.cpp','?InitialiseFromTextureMipmap@CSurface@@QAEXPAUIDirect3DSurface9@@@Z',0x9f2f10,66,448,0),('00/9f/CSurface_Uninitialise_009f2e20.cpp','?Uninitialise@CSurface@@QAEXXZ',0x9f2e20,32,528,0),('00/9f/CSurface_GetByteLength_009f2fc0.cpp','?GetByteLength@CSurface@@QBEKXZ',0x9f2fc0,75,576,2),('00/9e/CPixelFormat_Initialise_009e3830.cpp','?Initialise@CPixelFormat@@QAEXW4_D3DFORMAT@@@Z',0x9e3830,50,672,1),('00/9e/CPixelFormat_GetColourDepth_009e3820.cpp','?GetColourDepth@CPixelFormat@@QBEKXZ',0x9e3820,13,736,1)]
targets={symbol:address for rel,symbol,address,size,offset,count in specs};targets.update({'?Sizeof@CGraphicFrameData@@QBEKXZ':0x9fcbf0,'?Sizeof@CManagedTexture@@QBEKXZ':0xa69e60,'??_7CSurface@@6B@':0x122f84c,'?GD3DFormats@@3QBVCD3DFormatInfo@@B':0x129ba40})
offsets={address:offset for rel,symbol,address,size,offset,count in specs};offsets.update({0x9fcbf0:0x10000000,0xa69e60:0x10000001,0x122f84c:0x40000000,0x129ba40:0x80000000,0x129ba48:0x80000008})
objects=[];sources=[];proofs=[];body=bytearray(768);raw=EXE.read_bytes();cs=Cs(CS_ARCH_X86,CS_MODE_32);cs.detail=True;fixups=[]
for rel,symbol,address,size,offset,count in specs:
 source=ROOT/'rebuild/src/compiled'/rel
 if not source.exists():source=ROOT/'work/parity_addtext/frame_data_debug_memory_candidate2_20261009.cpp'
 obj=w/f'{address:x}.obj';r=V.cl(['/nologo','/c','/O2','/MT','/I'+str(ROOT/'rebuild/include'),'/Fo'+str(obj),str(source)],env,w);assert not r.returncode,r.stdout+r.stderr
 sources.append(source);objects.append(obj);proofs.append(check(obj,symbol,address,size,count,targets));b=read_virtual_data(raw,address,size);body[offset:offset+size]=b
 for ins in cs.disasm(b,address):
  if ins.mnemonic in ('call','jmp') and ins.bytes[0] in (0xe8,0xe9):
   target=int(ins.op_str,16)
   if not address<=target<address+size:assert target in offsets,(hex(target),rel);fixups.append((offset+ins.address-address+1,offsets[target],20))
  if ins.disp_size==4 and ins.disp in offsets:fixups.append((offset+ins.address-address+ins.disp_offset,offsets[ins.disp],6))
  if ins.mnemonic not in ('call','jmp') and ins.imm_size==4:
   for operand in ins.operands:
    if operand.type==2 and operand.imm in offsets:fixups.append((offset+ins.address-address+ins.imm_offset,offsets[operand.imm],6))
assert len(fixups)==17,fixups
source=ROOT/'rebuild/src/compiled/01/29/CPixelFormat_Table_0129ba40.cpp';obj=w/'table.obj';r=V.cl(['/nologo','/c','/O2','/MT','/Fo'+str(obj),str(source)],env,w);assert not r.returncode,r.stdout+r.stderr;sources.append(source);objects.append(obj);table=Coff(obj);sec,off=table.symbols['?GD3DFormats@@3QBVCD3DFormatInfo@@B'];tableBytes,tableRelocs=table.sections[sec];original=read_virtual_data(raw,0x129ba40,1692);assert off==0 and tableBytes==original and not tableRelocs
(w/'frame_debug_memory_retail.h').write_text('struct Fixup{unsigned site,target,kind;};\nstatic const unsigned char retailCode[]={'+','.join(map(str,body))+'};\nstatic const Fixup fixups[]={'+','.join('{%u,%u,%u}'%f for f in fixups)+'};\nstatic const unsigned retailFormats[]={'+','.join(map(str,struct.unpack('<423I',original)))+'};\n')
aliases={'@FrameDebug@8':specs[0][1],'_SurfaceVtable':'??_7CSurface@@6B@','?Sizeof@CGraphicFrameData@@QBEKXZ':'@FrameSize@8','?Sizeof@CManagedTexture@@QBEKXZ':'@ManagedSize@8','?CopyFromSurface@CSurface@@QAEXPBVC2DBoxI@@ABV1@0K@Z':'_UnusedBoundary','??_ECSurface@@UAEPAXI@Z':'_UnusedBoundary'}
(w/'frame_debug_memory_aliases.h').write_text(''.join('#pragma comment(linker,"/alternatename:'+a+'='+b+'")\n' for a,b in aliases.items()))
fixture=ROOT/'rebuild/tests/integration/NativeFrameDebugMemory_test.cpp';fo=w/'fixture.obj';r=V.cl(['/nologo','/c','/O2','/MT','/I'+str(w),'/I'+str(ROOT/'rebuild/include'),'/Fo'+str(fo),str(fixture)],env,w);assert not r.returncode,r.stdout+r.stderr
cmd=[str(V.VC/'bin/link.exe'),'/nologo','/out:'+str(w/'fixture.exe'),'kernel32.lib',str(fo)]
for omit in range(len(objects)):
 r=subprocess.run(cmd+[str(o) for i,o in enumerate(objects) if i!=omit],env=env,capture_output=True,text=True);assert r.returncode and ('LNK2001' in r.stdout or 'LNK2019' in r.stdout),(omit,r.stdout+r.stderr)
r=subprocess.run(cmd+list(map(str,objects)),env=env,capture_output=True,text=True);assert not r.returncode,r.stdout+r.stderr
r=subprocess.run([str(w/'fixture.exe')],env=env,capture_output=True,text=True,timeout=60);assert not r.returncode,(w,r.returncode,r.stdout,r.stderr);assert 'cases=61440 snapshots=427520' in r.stdout,r.stdout
headers=[ROOT/'rebuild/include'/p for p in ['fable_graphic_info.h','fable_graphic_info_frame.h','fable_graphic_frame.h','fable_graphic_frame_data.h','fable_surface_types.h','fable_d3d_format_info.h']]
report=dict(cases=61440,snapshots=427520,providers=proofs,omission_checks=10,result=r.stdout.strip(),source_sha256={str(p.relative_to(ROOT)):hashlib.sha256(p.read_bytes()).hexdigest() for p in sources+headers},fixture_sha256=hashlib.sha256(fixture.read_bytes()).hexdigest(),boundary='Actual212B frame debug memory root secondcandidate91.6129percent, stop tuning. Ten required sources execute actuallevelcount9B/mipmapgetter96B/copy55B/mipmapinit66B/surfacecleanup32B/byteLength75B/formatInit50B/depth13B/original1692B table. FrameDataSizeof/ManagedTextureSizeof remain explicit functional boundaries; COMGetLevelCount/GetSurfaceLevel/AddRef/Release/GetDesc functional boundaries; unusedcopy parent/scalar imports abort. Independent/native/compiled full16KiB event/final comparisons,signed mipmap sentinel/clamps,nullableframe/optionalmanaged owner,unsignedsizewrap and original signed divide4 on nonnegative actualsurface sizes,callbackliveowner/pointer/minimum/firstdescriptor mutations,representative8formats/twoHRESULTs/placements/fills. No actual deferredSizeof/GPUclaim or additional oldcallee coverage.')
(ROOT/'rebuild/compile-gate/native-frame-debug-memory.json').write_text(json.dumps(report,indent=2)+'\n');print(r.stdout.strip());print('NATIVE_FRAME_DEBUG_MEMORY_GATE_PASS')
