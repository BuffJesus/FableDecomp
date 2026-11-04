import hashlib,json,subprocess,sys,tempfile
from pathlib import Path
ROOT=Path(__file__).resolve().parents[2];sys.path.insert(0,str(ROOT/'tools/decomp_pipeline'))
import check_one as V
from variable_allocator_parity import check
from pe_oracle import EXE,read_virtual_data
from gate_game_component_abi import Coff
from capstone import Cs,CS_ARCH_X86,CS_MODE_32
w=Path(tempfile.mkdtemp(prefix='native-volume-texture-scalar-destructor-'));env=V.env()
specs=[('00/a6/CVolumeTexture_Destructor_00a69590.cpp','??_GCVolumeTexture@@UAEPAXI@Z',0xa69590,43,0,4),('00/a6/CVolumeTexture_Uninitialise_00a68060.cpp','?Uninitialise@CVolumeTexture@@QAEXXZ',0xa68060,85,64,3),('00/a6/CVolumeTexture_Sizeof_00a67f30.cpp','?ReleaseTexture@CVolumeTexture@@IAEXXZ',0xa67f60,132,192,6),('00/99/CBaseClass_CBaseClass_0099a2d0.cpp','??1CBaseClass@@UAE@XZ',0x99a2e0,7,336,1)]
targets={symbol:address for rel,symbol,address,size,offset,count in specs};targets.update({'?g_FableMemoryAllocator_013CAA90@@3PAVCMemoryAllocator@@A':0x13caa90,'?GetAllocationSize@CMemoryAllocator@@QAEKPAX@Z':0x9f2390,'?g_FableAllocatorMemoryBaseline_013CAAB0@@3KA':0x13caab0,'?TotalMemory@CVolumeTexture@@2KA':0x13d28e0,'?GFGetSystemManager@@YIPAVCSystemManager@@XZ':0x9a4ec0,'??_7CVolumeTexture@@6B@':0x129dc18,'??_7CBaseClass@@6B@':0x1231710,'??3@YAXPAX@Z':0xbfe9bc,'?WaitWhileResourceIsBusy@CRenderManagerCore@@QAEXPAUIDirect3DResource9@@M@Z':0xa0aa70});offsets={address:offset for rel,symbol,address,size,offset,count in specs};offsets.update({0x13caa90:0,0x9f2390:1,0x13caab0:2,0x13d28e0:3,0x9a4ec0:4,0xa0aa70:5,0xa67f60:6,0x129dc18:7,0xa68060:8,0x99a2e0:9,0x1231710:10,0xbfe9bc:11})

objects=[];sources=[];proofs=[];body=bytearray(352);raw=EXE.read_bytes();cs=Cs(CS_ARCH_X86,CS_MODE_32);cs.detail=True;fixups=[]
for rel,symbol,address,size,offset,count in specs:
 source=ROOT/'rebuild/src/compiled'/rel
 if not source.exists():source=ROOT/'work/parity_addtext/volume_texture_scalar_destructor_candidate_20261009.cpp'
 obj=w/f'{address:x}.obj';r=V.cl(['/nologo','/c','/O2','/MT','/I'+str(ROOT/'rebuild/include'),'/Fo'+str(obj),str(source)],env,w);assert not r.returncode,r.stdout+r.stderr
 sources.append(source);proofs.append(check(obj,symbol,address,size,count,targets));b=read_virtual_data(raw,address,size);body[offset:offset+size]=b
 for ins in cs.disasm(b,address):
  if ins.mnemonic in ('call','jmp') and ins.bytes[0] in (0xe8,0xe9):
   target=int(ins.op_str,16)
   if not address<=target<address+size:assert target in offsets;fixups.append((offset+ins.address-address+1,offsets[target],20))
  if ins.disp_size==4 and ins.disp in offsets:fixups.append((offset+ins.address-address+ins.disp_offset,offsets[ins.disp],6))
  if ins.mnemonic not in ('call','jmp') and ins.imm_size==4:
   for operand in ins.operands:
    if operand.type==2 and operand.imm in offsets:fixups.append((offset+ins.address-address+ins.imm_offset,offsets[operand.imm],6))
 objects.append(obj)
assert len(fixups)==14,fixups
from gate_render_state_static_data import coff_data
dataSource=ROOT/'rebuild/src/compiled/01/3b/GFMainSingletonCodeData_013b7d58.cpp';dataObj=w/'globals.obj';run=V.cl(['/nologo','/c','/O2','/MT','/I'+str(ROOT/'rebuild/include'),'/Fo'+str(dataObj),str(dataSource)],env,w);assert not run.returncode,run.stdout+run.stderr
assert coff_data(Coff(dataObj),'?g_FableMemoryAllocator_013CAA90@@3PAVCMemoryAllocator@@A',4)==read_virtual_data(raw,0x13caa90,4)==bytes(4)
objects.append(dataObj);sources.append(dataSource)
assert coff_data(Coff(objects[2]),'?TotalMemory@CVolumeTexture@@2KA',4)==read_virtual_data(raw,0x13d28e0,4)==bytes(4)
baselineSource=ROOT/'rebuild/src/compiled/01/39/MemoryAllocatorData_01396868.cpp';baselineObj=w/'baseline.obj';run=V.cl(['/nologo','/c','/O2','/MT','/I'+str(ROOT/'rebuild/include'),'/Fo'+str(baselineObj),str(baselineSource)],env,w);assert not run.returncode,run.stdout+run.stderr
assert coff_data(Coff(baselineObj),'?g_FableAllocatorMemoryBaseline_013CAAB0@@3KA',4)==read_virtual_data(raw,0x13caab0,4)==bytes(4)
objects.append(baselineObj);sources.append(baselineSource)
proofs.append(check(objects[2],'?Sizeof@CVolumeTexture@@QBEKXZ',0xa67f30,39,2,targets))
(w/'volume_texture_scalar_destructor_retail.h').write_text('struct Fixup{unsigned site,target,kind;};\nstatic const unsigned char retailCode[]={'+','.join(map(str,body))+'};\nstatic const Fixup fixups[]={'+','.join('{%u,%u,%u}'%f for f in fixups)+'};\n')
(w/'volume_texture_scalar_destructor_aliases.h').write_text('\n'.join('#pragma comment(linker,"/alternatename:'+a+'='+b+'")' for a,b in {'@VolumeTextureScalarDestructor@12':specs[0][1],'?GetAllocationSize@CMemoryAllocator@@QAEKPAX@Z':'@AllocationSize@12','?GFGetSystemManager@@YIPAVCSystemManager@@XZ':'@SystemGetter@0','?WaitWhileResourceIsBusy@CRenderManagerCore@@QAEXPAUIDirect3DResource9@@M@Z':'@WaitResource@16'}.items())+'\n')
coffs=[Coff(obj) for obj in objects]
section,off=coffs[0].symbols['??_7CVolumeTexture@@6B@'];data,relocs=coffs[0].sections[section];assert off==0 and len(data)==4 and relocs=={0:('??_ECVolumeTexture@@UAEPAXI@Z',6)} and read_virtual_data(raw,0x129dc18,4)==bytes.fromhex('9095a600')
defined={name for c in coffs for name in c.symbols};undefined={name for c in coffs for data,rels in c.sections.values() for name,kind in rels.values() if name not in defined}
known={'?GetAllocationSize@CMemoryAllocator@@QAEKPAX@Z','?GFGetSystemManager@@YIPAVCSystemManager@@XZ','?WaitWhileResourceIsBusy@CRenderManagerCore@@QAEXPAUIDirect3DResource9@@M@Z','??2@YAPAXI@Z','??3@YAXPAX@Z','__purecall','??_7type_info@@6B@'}
with (w/'volume_texture_scalar_destructor_aliases.h').open('a') as out:
 for name in sorted(undefined-known):out.write('#pragma comment(linker,"/alternatename:'+name+'=_AbortBoundary")\n')
 for name,target in [('_VolumeVTable','??_7CVolumeTexture@@6B@'),('_BaseVTable','??_7CBaseClass@@6B@')]:out.write('#pragma comment(linker,"/alternatename:'+name+'='+target+'")\n')
fixture=ROOT/'rebuild/tests/integration/NativeVolumeTextureScalarDestructor_test.cpp';fo=w/'fixture.obj';r=V.cl(['/nologo','/c','/O2','/MT','/I'+str(w),'/I'+str(ROOT/'rebuild/include'),'/Fo'+str(fo),str(fixture)],env,w);assert not r.returncode,r.stdout+r.stderr
cmd=[str(V.VC/'bin/link.exe'),'/nologo','/out:'+str(w/'fixture.exe'),'kernel32.lib',str(fo)]
for omit in range(6):
 r=subprocess.run(cmd+[str(o) for i,o in enumerate(objects) if i!=omit],env=env,capture_output=True,text=True);assert r.returncode and ('LNK2001' in r.stdout or 'LNK2019' in r.stdout),r.stdout+r.stderr
r=subprocess.run(cmd+list(map(str,objects)),env=env,capture_output=True,text=True);assert not r.returncode,r.stdout+r.stderr
r=subprocess.run([str(w/'fixture.exe')],env=env,capture_output=True,text=True,timeout=60);assert not r.returncode,(w,r.returncode,r.stdout,r.stderr)
assert 'cases=552960 snapshots=' in r.stdout
headers=[ROOT/'rebuild/include'/p for p in ('fable_volume_types.h','fable_base_class.h','fable_extents_i.h','fable_surface_types.h','fable_d3d_format_info.h','fable_d3d9.h','fable_memory_allocator.h','fable_gfuninitialise.h','fable_display_viewport.h')]
report=dict(cases=552960,snapshots=int(r.stdout.split('snapshots=')[1]),providers=proofs,omission_checks=6,result=r.stdout.strip(),source_sha256={str(p.relative_to(ROOT)):hashlib.sha256(p.read_bytes()).hexdigest() for p in sources+headers},fixture_sha256=hashlib.sha256(fixture.read_bytes()).hexdigest(),boundary='Complete43B original scalar deleting destructor generated from genuine virtual C++ destructor, actual85B Uninitialise/132B ReleaseTexture/7B BaseDtor/preserved39B Sizeof, six mandatory sources. Original one-slot vtable confirmed plus runtime scalar fallback identity. Full32bit flags onlybit0 controls heapdelete; cached returnthis after heapcallback overwrites owner. COM/allocator/systemgetter/wait/heapdelete explicit boundaries. Full16KiB and12B globals snapshots; independent model/native/compiled, callback mutation, accountingwrap, nullPD andtwo pointees/placements/fills. Standalone compiler dtor and vtable add no physical function/data coverage.')
(ROOT/'rebuild/compile-gate/native-volume-texture-scalar-destructor.json').write_text(json.dumps(report,indent=2)+'\n');print(r.stdout.strip());print('NATIVE_VOLUME_TEXTURE_SCALAR_DESTRUCTOR_GATE_PASS')
