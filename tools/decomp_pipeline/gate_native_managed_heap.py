"""Real CRT ownership checks independent of graphics device creation."""
import hashlib,json,subprocess,sys,tempfile
from pathlib import Path
ROOT=Path(__file__).resolve().parents[2];sys.path.insert(0,str(ROOT/'tools/decomp_pipeline'))
import check_one as V
from variable_allocator_parity import check
from gate_game_component_abi import Coff
from gate_render_state_static_data import coff_data
from pe_oracle import EXE,read_virtual_data
w=Path(tempfile.mkdtemp(prefix='native-managed-heap-'));env=V.env()
sources=['00/a6/CVolumeTexture_Destructor_00a69590.cpp','00/a6/CManagedTextureData_CreateVolumeTexture_00a699d0.cpp','00/a6/CVolumeTexture_Uninitialise_00a68060.cpp','00/a6/CVolumeTexture_Sizeof_00a67f30.cpp','00/99/CBaseClass_CBaseClass_0099a2d0.cpp','00/a6/CManagedTextureData_Constructor_00a69980.cpp','00/a6/CManagedTextureData_Destructor_00a6b5b0.cpp','00/a6/CManagedTextureData_Initialise_00a69d60.cpp','00/56/CCountedPointer_CTexture_Reset_005669c0.cpp','00/a6/CManagedTextureData_IsBusy_00a69810.cpp','00/9f/CTexture_IsBusy_009f9d50.cpp','00/9f/CTexture_Uninitialise_009f9f70.cpp','00/9f/CResourceList_CResourceList_009fc570.cpp','00/a6/CCountedPointer_CVolumeTexture_Reset_00a6ae60.cpp']
sources+=['00/42/CIVCountedPointeeBase_DeleteCIVCountedPointeeBaseUBEXXZ_0042d930.cpp','00/a6/CArray_ManagedTextureData_DeallocateMemory_00a6be40.cpp']
historical=json.loads((ROOT/'rebuild/compile-gate/native-volume-texture-device.json').read_text())
historical['providers'].append(dict(symbol='?DeleteCIVCountedPointeeBase@CIVCountedPointeeBase@@UBEXXZ',address='0042d930',retail_bytes=11,relocations=0,source=sources[-2]))
for p in json.loads((ROOT/'rebuild/compile-gate/native-managed-texture-array-deallocate.json').read_text())['providers']:
 p=dict(p);p['source']=sources[-1];historical['providers'].append(p)
targets={p['symbol']:int(p['address'],16) for p in historical['providers']};targets['_free']=0xbfea14
targets.update({'??2@YAPAXI@Z':0xbfea1a,'??3@YAXPAX@Z':0xbfe9bc,'??_7CVolumeTexture@@6B@':0x129dc18,'??_7CManagedTextureData@@6B@':0x129dc24,'??_7CResource@@6B@':0x12354a4,'??_7CIVCountedPointeeBase@@6B@':0x12354d0,'??_7CBaseClass@@6B@':0x1231710,'?GFGetSystemManager@@YIPAVCSystemManager@@XZ':0x9a4ec0,'?RemoveResource@CResourceList@@QAEXAAVCResource@@@Z':0x9fc020,'?GetAllocationSize@CMemoryAllocator@@QAEKPAX@Z':0x9f2390,'?g_FableMemoryAllocator_013CAA90@@3PAVCMemoryAllocator@@A':0x13caa90,'?g_FableAllocatorMemoryBaseline_013CAAB0@@3KA':0x13caab0,'?TotalMemory@CVolumeTexture@@2KA':0x13d28e0,'?DeleteData@?$CCountedPointer@VCTexture@@@@CIXPAX@Z':0x41bd80,'?DeleteData@?$CCountedPointer@VCVolumeTexture@@@@CIXPAX@Z':0xa6acb0})
objects=[];coffs=[];proofs=[];paths=[]
for i,rel in enumerate(sources):
 p=ROOT/'rebuild/src/compiled'/rel;obj=w/(str(i)+'.obj');r=V.cl(['/nologo','/c','/O2','/MT','/I'+str(ROOT/'rebuild/include'),'/Fo'+str(obj),str(p)],env,w);assert not r.returncode,r.stdout+r.stderr;c=Coff(obj);objects.append(obj);coffs.append(c);paths.append(p)
 for old in historical['providers']:
  if old['source']!=rel:continue
  proof=check(obj,old['symbol'],int(old['address'],16),old['retail_bytes'],old['relocations'],targets);proof['source']=rel;proofs.append(proof)
for rel,symbol,address,length,count in [('00/56/CCountedPointer_CTexture_Reset_005669c0.cpp','?DeleteData@?$CCountedPointer@VCTexture@@@@CIXPAX@Z',0x41bd80,23,2),('00/a6/CCountedPointer_CVolumeTexture_Reset_00a6ae60.cpp','?DeleteData@?$CCountedPointer@VCVolumeTexture@@@@CIXPAX@Z',0xa6acb0,11,0)]:
 proofs.append(check(objects[sources.index(rel)],symbol,address,length,count,targets))
assert coff_data(coffs[3],'?TotalMemory@CVolumeTexture@@2KA',4)==read_virtual_data(EXE.read_bytes(),0x13d28e0,4)
defined={s for c in coffs for s in c.symbols};undefined={s for c in coffs for data,relocs in c.sections.values() for s,kind in relocs.values() if s not in defined}
aliases={'@TextureDeleteData@4':'?DeleteData@?$CCountedPointer@VCTexture@@@@CIXPAX@Z','@VolumeTextureDeleteData@4':'?DeleteData@?$CCountedPointer@VCVolumeTexture@@@@CIXPAX@Z','@ManagedDataScalar@12':'??_GCManagedTextureData@@UAEPAXI@Z','_ManagedDataVTable':'??_7CManagedTextureData@@6B@','@TextureScalar@12':'??_GCVolumeTexture@@UAEPAXI@Z','_TextureVTable':'??_7CVolumeTexture@@6B@'}
aliases['@ManagedVirtualDelete@8']='?DeleteCIVCountedPointeeBase@CIVCountedPointeeBase@@UBEXXZ'
known={'??2@YAPAXI@Z','??3@YAXPAX@Z','__purecall','??_7type_info@@6B@','_free'}
for s in undefined-known:aliases[s]='_AbortBoundary'
(w/'managed_heap_aliases.h').write_text('\n'.join('#pragma comment(linker,"/alternatename:'+a+'='+b+'")' for a,b in aliases.items())+'\n')
fixture=ROOT/'rebuild/tests/integration/NativeManagedHeap_test.cpp';fo=w/'fixture.obj';r=V.cl(['/nologo','/c','/O2','/MT','/I'+str(w),'/I'+str(ROOT/'rebuild/include'),'/Fo'+str(fo),str(fixture)],env,w);assert not r.returncode,r.stdout+r.stderr
cmd=[str(V.VC/'bin/link.exe'),'/nologo','/out:'+str(w/'fixture.exe'),'kernel32.lib',str(fo)]
for omitted in range(len(objects)):
 r=subprocess.run(cmd+[str(o) for i,o in enumerate(objects) if i!=omitted],env=env,capture_output=True,text=True);assert r.returncode and ('LNK2001' in r.stdout or 'LNK2019' in r.stdout),(omitted,r.stdout,r.stderr)
r=subprocess.run(cmd+list(map(str,objects)),env=env,capture_output=True,text=True);assert not r.returncode,r.stdout+r.stderr
r=subprocess.run([str(w/'fixture.exe')],env=env,capture_output=True,text=True,timeout=30);assert not r.returncode,(w,r.returncode,r.stdout,r.stderr);assert 'cycles=256 array_cycles=256 graphics_calls=0' in r.stdout,r.stdout
headers=[ROOT/'rebuild/include'/p for p in ['fable_volume_types.h','fable_surface_types.h','fable_managed_texture_data.h','fable_counted_pointer.h','fable_resource_list.h','rebuild_abi.h','engine/CPreallocTexturePool.h','fable_carray.h','fable_iv_counted_pointer.h','fable_stlport_vector.h','fable_stlport_algobase.h']]
report=dict(cycles=256,array_cycles=256,graphics_calls=0,providers=proofs,omission_checks=len(objects),result=r.stdout.strip(),source_sha256={str(p.relative_to(ROOT)):hashlib.sha256(p.read_bytes()).hexdigest() for p in paths+headers},fixture_sha256=hashlib.sha256(fixture.read_bytes()).hexdigest(),boundary='Sixteen actual sources,all omissions fail,actual vtable scalar andvirtualdeleter identities. Additional256 realCRT array cycles allocate5 slots/construct3 withtwo shared references tofirstmanagedowner andone tosecond;actual112B arrayrelease plus82B copy/50B destruction helpers drop2->1->0,actual11B virtualdelete dispatches30B managedscalar/113B destructor/CRTdelete forboth heap76B owners withlive ordinary-volume handles,thenCRTfree arraystorage. Onlystill-live empty arrayfields inspectedafterownerlifetimes.256 realCRT allocation/share/replace/drop/free cycles ofordinary andvolume texture handles inactualmanagedData ctor/dtor,half live handles/half explicitResetNULL beforedestruction,controlcount2->1->delete,actual23B ordinarydeleter/11B volume virtualdeleter/43B scalar/85B volumeUninit nullPD branch/base resource ctor33 anddtor30. No allocator/deleter stubs,no post-lifetime reads. Unusedgraphics/rounding/initialization/resource-list-removal/allocator-global imports abortaliases; no active resources orgraphics device/library/window calls andno GPUclaim. Existingheap sequence extractedfromdevicefixture; no newfunction/data coverage.')
(ROOT/'rebuild/compile-gate/native-managed-heap.json').write_text(json.dumps(report,indent=2)+'\n');print(r.stdout.strip());print('NATIVE_MANAGED_HEAP_GATE_PASS')

