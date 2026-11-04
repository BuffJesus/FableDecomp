"""Real D3D9 volume resource smoke check; OS/device setup is a harness boundary."""
import hashlib,json,subprocess,sys
from pathlib import Path
ROOT=Path(__file__).resolve().parents[2];sys.path.insert(0,str(ROOT/'tools/decomp_pipeline'))
import check_one as V
from variable_allocator_parity import check
from gate_game_component_abi import Coff
from gate_render_state_static_data import coff_data
from pe_oracle import EXE,read_virtual_data
work=ROOT/'work/codex-native-startup/native-volume-texture-device-20261009';work.mkdir(parents=True,exist_ok=True)
sources=[
 '00/a6/CVolumeTexture_CreateTexture_00a68250.cpp',
 '00/a6/CVolumeTexture_Sizeof_00a67f30.cpp',
 '00/a6/CVolumeTexture_Uninitialise_00a68060.cpp',
 '00/a6/CManagedTextureData_CreateVolumeTexture_00a699d0.cpp',
 '00/a6/CVolumeTexture_Destructor_00a69590.cpp',
 '00/a6/CVolumeTexture_CalcByteLength_00a67ff0.cpp',
 '00/a6/CVolumeTexture_GetNoLevels_00a67e90.cpp',
 '00/a6/CVolumeTexture_GetDimensions_00a680f0.cpp',
 '00/a6/CVolumeTexture_GetPixelFormat_00a680c0.cpp',
 '00/a6/CVolumeTexture_GetLevelByteLength_00a681f0.cpp',
 '00/a6/CVolumeTexture_Unlock_00a67e40.cpp',
 '00/a6/CVolumeTexture_SimpleLeaves_00a67e10.cpp',
 '00/a6/CVolumeTexture_GetPriority_00a67e80.cpp',
 '00/a6/CVolumeTexture_SetPriority_00a67e60.cpp',
 '00/9f/CVolume_Destructor_009fccf0.cpp',
 '00/a6/CVolume_CopyConstructor_00a673e0.cpp',
 '00/a6/CVolume_Uninitialise_00a67460.cpp',
 '00/99/CBaseClass_CBaseClass_0099a2d0.cpp',
 '00/a6/CVolume_InitialiseFromVolumeTextureMipmap_00a67770.cpp',
 '00/a6/CVolume_GetByteLength_00a674e0.cpp',
 '00/9e/CPixelFormat_Initialise_009e3830.cpp',
 '00/9e/CPixelFormat_GetByteColourDepth_009e3800.cpp',
 '01/29/CPixelFormat_Table_0129ba40.cpp',
 '01/39/MemoryAllocatorData_01396868.cpp',
 '01/3b/GFMainSingletonCodeData_013b7d58.cpp',
 '00/a0/CRenderManagerCore_WaitWhileResourceIsBusy_00a0aa70.cpp',
 '00/a6/CVolumeTexture_Lock_00a685e0.cpp',
 '00/a6/CVolume_CopyFromVolume_00a67670.cpp',
 '00/a6/CVolumeTexture_CopyFromVolumeTexture_00a68130.cpp',
 '00/a6/CManagedTextureData_Initialise_00a69d60.cpp',
 '00/56/CCountedPointer_CTexture_Reset_005669c0.cpp',
 '00/a6/CManagedTextureData_IsBusy_00a69810.cpp',
 '00/9f/CTexture_IsBusy_009f9d50.cpp',
 '00/9f/CTexture_Uninitialise_009f9f70.cpp',
 '00/a6/CManagedTextureData_Constructor_00a69980.cpp',
 '00/a6/CManagedTextureData_Destructor_00a6b5b0.cpp',
 '00/9f/CResourceList_CResourceList_009fc570.cpp',
 '00/a6/CCountedPointer_CVolumeTexture_Reset_00a6ae60.cpp']
reports=['create','preallocated','constructor','scalar-destructor','calc-data','dimensions','pixel-format','byte-length','unlock','simple-leaves','priority','set-priority','lock','copy-levels']
specs={}
for name in reports:
 for proof in json.loads((ROOT/f'rebuild/compile-gate/native-volume-texture-{name}.json').read_text())['providers']:
  specs[proof['symbol']]=proof
for report_path in ['native-managed-texture-data-initialise.json','native-counted-texture-reset.json','native-counted-volume-texture-reset.json','native-managed-texture-busy.json','native-managed-texture-constructor.json','native-managed-texture-data-destructor.json']:
 for proof in json.loads((ROOT/'rebuild/compile-gate'/report_path).read_text())['providers']:
  specs[proof['symbol']]=proof
for proof in json.loads((ROOT/'rebuild/compile-gate/native-texture-bindings.json').read_text())['providers']:
 if proof['symbol']=='?Uninitialise@CTexture@@QAEXXZ':specs[proof['symbol']]=proof
targets={symbol:int(proof['address'],16) for symbol,proof in specs.items()}
targets.update({'?GFGetSystemManager@@YIPAVCSystemManager@@XZ':0x9a4ec0,'_D3DXCreateVolumeTexture@36':0xbfeff2,'??2@YAPAXI@Z':0xbfea1a,'?DeleteData@?$CCountedPointer@VCTexture@@@@CIXPAX@Z':0x41bd80,
 '?GetAllocationSize@CMemoryAllocator@@QAEKPAX@Z':0x9f2390,'?g_FableMemoryAllocator_013CAA90@@3PAVCMemoryAllocator@@A':0x13caa90,
 '?g_FableAllocatorMemoryBaseline_013CAAB0@@3KA':0x13caab0,'?TotalMemory@CVolume@@2KA':0x13d28dc,
 '?TotalMemory@CVolumeTexture@@2KA':0x13d28e0,'?GD3DFormats@@3QBVCD3DFormatInfo@@B':0x129ba40,
 '??_7CVolume@@6B@':0x129c840,'??_7CVolumeTexture@@6B@':0x129dc18,'??_7CBaseClass@@6B@':0x1231710,'??3@YAXPAX@Z':0xbfe9bc})
targets['?WaitWhileResourceIsBusy@CRenderManagerCore@@QAEXPAUIDirect3DResource9@@M@Z']=0xa0aa70
targets['_D3DXLoadVolumeFromVolume@32']=0xbfefec
targets['?DeleteData@?$CCountedPointer@VCVolumeTexture@@@@CIXPAX@Z']=0xa6acb0
targets.update({'??_7CManagedTextureData@@6B@':0x129dc24,'??0CResource@@QAE@XZ':0x9fbec0,'??1CResource@@UAE@XZ':0x9fc550,'??_7CResource@@6B@':0x12354a4,'??_7CIVCountedPointeeBase@@6B@':0x12354d0,'?RemoveResource@CResourceList@@QAEXAAVCResource@@@Z':0x9fc020})
specs['??0CResource@@QAE@XZ']=dict(address='009fbec0',retail_bytes=33,relocations=1)
specs['??1CResource@@UAE@XZ']=dict(address='009fc550',retail_bytes=30,relocations=3)
specs['?WaitWhileResourceIsBusy@CRenderManagerCore@@QAEXPAUIDirect3DResource9@@M@Z']=dict(address='00a0aa70',retail_bytes=3,relocations=0)
env=V.env();objects=[];coffs=[];proofs=[];paths=[]
for i,source in enumerate(sources):
 path=ROOT/'rebuild/src/compiled'/source;obj=work/f'{i}.obj';result=V.cl(['/nologo','/c','/O2','/MT','/I'+str(ROOT/'rebuild/include'),'/Fo'+str(obj),str(path)],env,work);assert not result.returncode,result.stdout+result.stderr
 objects.append(obj);coff=Coff(obj);coffs.append(coff);paths.append(path)
 for symbol,proof in specs.items():
  if symbol in coff.symbols and not (symbol=="??_GCVolumeTexture@@UAEPAXI@Z" and source!="00/a6/CVolumeTexture_Destructor_00a69590.cpp"):
   if symbol=="??_GCManagedTextureData@@UAEPAXI@Z" and source!="00/a6/CManagedTextureData_Constructor_00a69980.cpp":continue
   checked=check(obj,symbol,int(proof['address'],16),proof['retail_bytes'],proof['relocations'],targets);checked['source']=source;proofs.append(checked)
assert set(specs)=={proof['symbol'] for proof in proofs},set(specs)-{proof['symbol'] for proof in proofs}
supporting_deleter_proof=check(objects[sources.index('00/56/CCountedPointer_CTexture_Reset_005669c0.cpp')],'?DeleteData@?$CCountedPointer@VCTexture@@@@CIXPAX@Z',0x41bd80,23,2,targets)
supporting_volume_deleter_proof=check(objects[sources.index('00/a6/CCountedPointer_CVolumeTexture_Reset_00a6ae60.cpp')],'?DeleteData@?$CCountedPointer@VCVolumeTexture@@@@CIXPAX@Z',0xa6acb0,11,0,targets)
raw=EXE.read_bytes();data_specs=[(1,'?TotalMemory@CVolumeTexture@@2KA',0x13d28e0,4),(18,'?TotalMemory@CVolume@@2KA',0x13d28dc,4),(22,'?GD3DFormats@@3QBVCD3DFormatInfo@@B',0x129ba40,1692),(23,'?g_FableAllocatorMemoryBaseline_013CAAB0@@3KA',0x13caab0,4),(24,'?g_FableMemoryAllocator_013CAA90@@3PAVCMemoryAllocator@@A',0x13caa90,4)]
for index,symbol,address,size in data_specs:assert coff_data(coffs[index],symbol,size)==read_virtual_data(raw,address,size)
defined={symbol for coff in coffs for symbol in coff.symbols};undefined={symbol for coff in coffs for data,relocs in coff.sections.values() for symbol,kind in relocs.values() if symbol not in defined}
aliases={'@VolumeTextureDeleteData@4':'?DeleteData@?$CCountedPointer@VCVolumeTexture@@@@CIXPAX@Z','@ManagedDataScalar@12':'??_GCManagedTextureData@@UAEPAXI@Z','_ManagedDataVTable':'??_7CManagedTextureData@@6B@','@TextureDeleteData@4':'?DeleteData@?$CCountedPointer@VCTexture@@@@CIXPAX@Z','@TextureLock@20':'?Lock@CVolumeTexture@@QAE?AVCVolumeLockedBox@@KK@Z','@TextureDefaultCtor@8':'??0CVolumeTexture@@QAE@XZ','@TextureCreate@32':'?CreateTexture@CVolumeTexture@@IAE_NABVC3DExtentsI@@IKABVCPixelFormat@@W4EVolumePool@@_N@Z',
 '@TexturePreallocated@12':'?InitialiseFromPreallocatedVolumeTexture@CVolumeTexture@@QAE_NPAUIDirect3DVolumeTexture9@@@Z','@TextureScalar@12':'??_GCVolumeTexture@@UAEPAXI@Z','_TextureVTable':'??_7CVolumeTexture@@6B@'}
known={'?GFGetSystemManager@@YIPAVCSystemManager@@XZ','_D3DXCreateVolumeTexture@36','_D3DXLoadVolumeFromVolume@32','??2@YAPAXI@Z','??3@YAXPAX@Z','__purecall','??_7type_info@@6B@'}
for symbol in undefined-known:aliases[symbol]='_AbortBoundary'
(work/'volume_texture_device_aliases.h').write_text('\n'.join('#pragma comment(linker,"/alternatename:'+a+'='+b+'")' for a,b in aliases.items())+'\n')
fixture=ROOT/'rebuild/tests/integration/NativeVolumeTextureDevice_test.cpp';fixtureObj=work/'fixture.obj';result=V.cl(['/nologo','/c','/O2','/MT','/I'+str(work),'/I'+str(ROOT/'rebuild/include'),'/Fo'+str(fixtureObj),str(fixture)],env,work);assert not result.returncode,result.stdout+result.stderr
user32=Path('C:/Program Files (x86)/Windows Kits/10/Lib/10.0.26100.0/um/x86/user32.lib');assert user32.is_file()
link_objects=[objects[4]]+objects[:4]+objects[5:]
cmd=[str(V.VC/'bin/link.exe'),'/nologo','/out:'+str(work/'fixture.exe'),'kernel32.lib',str(user32),str(fixtureObj)]
for omit in range(len(objects)):
 result=subprocess.run(cmd+[str(obj) for obj in link_objects if obj!=objects[omit]],env=env,capture_output=True,text=True);assert result.returncode and ('LNK2001' in result.stdout or 'LNK2019' in result.stdout),(omit,result.stdout,result.stderr)
result=subprocess.run(cmd+list(map(str,link_objects)),env=env,capture_output=True,text=True);assert not result.returncode,result.stdout+result.stderr
readback=work/'volume-readback.bin';result=subprocess.run([str(work/'fixture.exe'),str(readback)],env=env,capture_output=True,text=True,timeout=45);assert not result.returncode,(result.returncode,result.stdout,result.stderr)
assert 'NATIVE_VOLUME_TEXTURE_DEVICE_PASS dimensions=8x4x2 levels=4 texture_bytes=300 voxels=64 readback_bytes=256' in result.stdout
assert readback.stat().st_size==256
assert 'cropped_voxels=8' in result.stdout and 'creation_calls=2 copy_calls=5' in result.stdout
assert 'managed_heap_cycles=256' in result.stdout
headers=[ROOT/'rebuild/include'/name for name in ['fable_volume_types.h','fable_base_class.h','fable_extents_i.h','fable_surface_types.h','fable_d3d_format_info.h','fable_d3d9.h','fable_display_viewport.h','fable_gfuninitialise.h','fable_memory_allocator.h','fable_render_state_manager.h','fable_d3dx9_texture.h','fable_volume_box.h','fable_display_geometry.h','fable_managed_texture_data.h','fable_counted_pointer.h','fable_resource_list.h']]
report=dict(providers=proofs,supporting_deleter_proof=supporting_deleter_proof,supporting_volume_deleter_proof=supporting_volume_deleter_proof,managed_heap_cycles=256,omission_checks=len(objects),result=result.stdout.strip(),dimensions=[8,4,2],levels=4,texture_bytes=300,voxels=64,readback_bytes=256,readback=str(readback),readback_sha256=hashlib.sha256(readback.read_bytes()).hexdigest(),source_sha256={str(path.relative_to(ROOT)):hashlib.sha256(path.read_bytes()).hexdigest() for path in paths+headers+[fixture]},boundary='Real hidden-window HAL Direct3D9 device and original d3dx9_25 platform function. Actual recovered default constructor/create/preallocated attachment/full mip-calculation/getter/volume initialization/byte and format queries/priority/preload/unlock/shutdown plus actual retail no-op resource wait. System getter supplies typed proxy display/device; D3DX thunks forward real DLL exports; actual151B volume-copy and178B texture-copy providers copy allfour mip levels to a second realtexture with filter1, destination level0 reads back256B byte-exact; a further cropped4x2x1 region copy moves eight knownvoxels fromz1 source toz0 destination and allother destinationvoxels remainunchanged; unused bank/allocator imports abort. Upload/read both use actual167B/171B near recovered CVolumeTexture::Lock and actualUnlock; real3B Wait body executes in both locks. Separately deferred CVolume::Lock is not used. Actual pitched64-voxel upload and byte-exact256B readback, four mip dimensions/byte counts sum300 andSizeof304, original memory counters348/304/652 beforecleanup, default preservesByteLength and shutdown clearsfields. All38 source omissions fail; full original provider parity/data verification repeated. Actualmanaged initializer/reset/busy plus34B textureUninitialise and23B specializeddeleter execute256 real CRT heap cycles: share controlblock count2, replace decrementto1, replace count1destroy, constructboth managed objects with actual67B constructor and33B resourcebase constructor; actual113B managed destructor and30B resourcebase destructor execute on both objects, both ordinary andvolume handles shared/replaced with actual countedvolume Reset103B/private virtualdeleter11B andverified43B volume scalar/85B shutdown; halfcycles with live texture andvolume handles and halfafter explicit Reset(NULL); heapdata zero/count1/actualdeleter identity/busyfalse checked, real CRT allocator/delete; resource-list removal is an unused abort boundary because the real base constructor leaves ResourceListNULL; noallocator ordeleter stubs forheap part, no postlifetime reads. Bounded real volume-resource smoke check, no game/frontend frame or additional function/data coverage.')
(ROOT/'rebuild/compile-gate/native-volume-texture-device.json').write_text(json.dumps(report,indent=2)+'\n');print(result.stdout.strip());print('NATIVE_VOLUME_TEXTURE_DEVICE_GATE_PASS')
