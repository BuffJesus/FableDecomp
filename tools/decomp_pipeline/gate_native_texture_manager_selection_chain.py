import hashlib,json,subprocess,sys,tempfile
from pathlib import Path
ROOT=Path(__file__).resolve().parents[2];sys.path.insert(0,str(ROOT/'tools/decomp_pipeline'))
import check_one as V
from variable_allocator_parity import check
from gate_game_component_abi import Coff
from gate_render_state_static_data import coff_data
from pe_oracle import EXE,read_virtual_data
from capstone import Cs,CS_ARCH_X86,CS_MODE_32
w=Path(tempfile.mkdtemp(prefix='native-texture-manager-selection-chain-'));env=V.env()
specs=[('00/a6/CTextureManager_Allocate_00a69b70.cpp','?AllocateTexture@CTextureManager@@QAE_NAAVCManagedTexture@@KKIABVCPixelFormat@@PAVCManagedTextureOwner@@@Z',0xa69b70,176,0,4),('00/a6/CTextureManager_Allocate_00a69b70.cpp','?AllocateVolumeTexture@CTextureManager@@QAE_NAAVCManagedTexture@@KKKIABVCPixelFormat@@PAVCManagedTextureOwner@@@Z',0xa69c20,186,256,4),('00/a6/CTextureManager_CalculateTextureSize_00a698f0.cpp','?CalculateTextureSize@CTextureManager@@SIKKK_NABVCPixelFormat@@@Z',0xa698f0,136,512,2),('00/9e/CPixelFormat_GetColourDepth_009e3820.cpp','?GetColourDepth@CPixelFormat@@QBEKXZ',0x9e3820,13,672,1),('00/a6/CTextureManager_GetPoolIndex_00a69730.cpp','?GetPoolIndex@CTextureManager@@AAEJK@Z',0xa69730,99,704,0),('00/a6/CPreallocTexturePool_Allocate_00a69830.cpp','?Allocate@CPreallocTexturePool@@QAEPAVCManagedTextureData@@XZ',0xa69830,125,832,3),('00/9f/CResource_SetAsMRU_009fc4a0.cpp','?SetAsMRU@CResource@@QAEXXZ',0x9fc4a0,80,960,0),('00/9f/CTexture_IsBusy_009f9d50.cpp','?IsBusy@CTexture@@QBE_NXZ',0x9f9d50,3,1088,0)]
table='?GD3DFormats@@3QBVCD3DFormatInfo@@B';reuse='?ReuseBusyTextures@CTextureManager@@0_NA';create='?CreateTexture@CManagedTextureData@@QAE_NKKIABVCPixelFormat@@@Z';create_volume='?CreateVolumeTexture@CManagedTextureData@@QAE_NKKKIABVCPixelFormat@@@Z'
targets={sym:address for rel,sym,address,size,offset,count in specs};targets.update({table:0x129ba40,reuse:0x13d28e8,create:0xa697a0,create_volume:0xa699d0});offsets={address:offset for rel,sym,address,size,offset,count in specs};offsets.update({0x129ba40:0x80000000,0x129ba48:0x80000001,0x13d28e8:0x80000002,0xa697a0:0x80000003,0xa699d0:0x80000004})
objects=[];sources=[];compiled={};proofs=[];body=bytearray(1152);fixups=[];raw=EXE.read_bytes();cs=Cs(CS_ARCH_X86,CS_MODE_32);cs.detail=True
for rel,symbol,address,size,offset,count in specs:
 source=ROOT/'rebuild/src/compiled'/rel;obj=compiled.get(source)
 if obj is None:
  obj=w/f'{address:x}.obj';r=V.cl(['/nologo','/c','/O2','/MT','/I'+str(ROOT/'rebuild/include'),'/Fo'+str(obj),str(source)],env,w);assert not r.returncode,r.stdout+r.stderr;objects.append(obj);sources.append(source);compiled[source]=obj
 proofs.append(check(obj,symbol,address,size,count,targets));b=read_virtual_data(raw,address,size);body[offset:offset+size]=b
 for ins in cs.disasm(b,address):
  if ins.mnemonic in ('call','jmp') and ins.bytes[0] in (0xe8,0xe9):
   target=int(ins.op_str,16);assert target in offsets;fixups.append((offset+ins.address-address+1,offsets[target],20))
  if ins.disp_size==4 and ins.disp in offsets:fixups.append((offset+ins.address-address+ins.disp_offset,offsets[ins.disp],6))
assert len(fixups)==14,fixups
for rel,symbol,address,size in [('01/29/CPixelFormat_Table_0129ba40.cpp',table,0x129ba40,1692),('01/3d/CTextureManager_ReuseBusyTextures_013d28e8.cpp',reuse,0x13d28e8,1)]:
 source=ROOT/'rebuild/src/compiled'/rel;obj=w/f'{address:x}.obj';r=V.cl(['/nologo','/c','/O2','/MT','/I'+str(ROOT/'rebuild/include'),'/Fo'+str(obj),str(source)],env,w);assert not r.returncode,r.stdout+r.stderr;assert coff_data(Coff(obj),symbol,size)==read_virtual_data(raw,address,size);objects.append(obj);sources.append(source)
assert len(objects)==9
(w/'texture_manager_selection_chain_retail.h').write_text('struct Fixup{unsigned site,target,kind;};\nstatic const unsigned char retailCode[]={'+','.join(map(str,body))+'};\nstatic const Fixup fixups[]={'+','.join('{%u,%u,%u}'%f for f in fixups)+'};\n')
aliases={'@AllocateTextureRoot@32':specs[0][1],'@AllocateVolumeRoot@36':specs[1][1],'_FormatTable':table,'_ReuseBusyByte':reuse,create:'@TextureCreateBoundary@24',create_volume:'@VolumeCreateBoundary@28'}
(w/'texture_manager_selection_chain_aliases.h').write_text('\n'.join('#pragma comment(linker,"/alternatename:'+a+'='+b+'")' for a,b in aliases.items())+'\n')
fixture=ROOT/'rebuild/tests/integration/NativeTextureManagerSelectionChain_test.cpp';fo=w/'fixture.obj';r=V.cl(['/nologo','/c','/O2','/MT','/I'+str(w),'/Fo'+str(fo),str(fixture)],env,w);assert not r.returncode,r.stdout+r.stderr
cmd=[str(V.VC/'bin/link.exe'),'/nologo','/out:'+str(w/'fixture.exe'),'kernel32.lib',str(fo)]
for omit in objects:
 r=subprocess.run(cmd+[str(o) for o in objects if o!=omit],env=env,capture_output=True,text=True);assert r.returncode and ('LNK2001' in r.stdout or 'LNK2019' in r.stdout),r.stdout+r.stderr
r=subprocess.run(cmd+list(map(str,objects)),env=env,capture_output=True,text=True);assert not r.returncode,r.stdout+r.stderr
r=subprocess.run([str(w/'fixture.exe')],env=env,capture_output=True,text=True,timeout=60);assert not r.returncode,(w,r.returncode,r.stdout,r.stderr)
assert 'cases=108288 snapshots=' in r.stdout,(w,r.stdout)
headers=[ROOT/'rebuild/include'/p for p in ('engine/CTextureManager.h','engine/CPreallocTexturePool.h','fable_managed_texture.h','fable_managed_texture_data.h','fable_iv_counted_pointer.h','fable_resource_list.h','fable_surface_types.h','fable_d3d_format_info.h','rebuild_abi.h')]
report=dict(cases=108288,snapshots=int(r.stdout.split('snapshots=')[1]),providers=proofs,omission_checks=9,result=r.stdout.strip(),data=[dict(address='0129ba40',bytes=1692),dict(address='013d28e8',bytes=1)],source_sha256={str(p.relative_to(ROOT)):hashlib.sha256(p.read_bytes()).hexdigest() for p in sources+headers},fixture_sha256=hashlib.sha256(fixture.read_bytes()).hexdigest(),boundary='Nine mandatory actualsources compose bothmanager roots, original mip-size136B/GetColourDepth13B,99B binarypoollookup,125B poolAllocate/80B MRU/3B PCbusy, full1692B formatstable and1B reuseflag. Original/model/compiled full16KiB callback/final snapshots over47 rows(46valid plus sentinel),zero/thin/rect/extreme unsigneddimensions,depth0/1/3/ffffffff/multiplywrap,true/false mip-sizing,0/4/16 pools/three sortedcapacity profiles,empty/single/three resourcelists/fourbusy-lock-allocation profiles/owner release-leave-next-release-countmutation,creation false/true,old NULL/same/other/refcounts0/1/2/ffffffff/failedcountwrap/live mutations,two placements-fills. Onlymanagedtexture/volume creator bodies,owner release andoldvirtualdelete remain functionalboundaries; actualsize calculation/private lookup/selection/MRU/PCbusy execute throughcomplete recoveredproviders, no algorithm stubs forselection. Sourceomissions9 allfail; no additional function/data coverage or realGPU creation claimed bythis Arena gate.')
(ROOT/'rebuild/compile-gate/native-texture-manager-selection-chain.json').write_text(json.dumps(report,indent=2)+'\n');print(r.stdout.strip());print('NATIVE_TEXTURE_MANAGER_SELECTION_CHAIN_GATE_PASS')
