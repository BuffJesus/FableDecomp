import hashlib,json,subprocess,sys,tempfile
from pathlib import Path
ROOT=Path(__file__).resolve().parents[2];sys.path.insert(0,str(ROOT/'tools/decomp_pipeline'))
import check_one as V
from variable_allocator_parity import check
from pe_oracle import EXE,read_virtual_data
from gate_game_component_abi import Coff
from capstone import Cs,CS_ARCH_X86,CS_MODE_32
w=Path(tempfile.mkdtemp(prefix='native-volume-texture-release-'));env=V.env()
specs=[('00/a6/CVolumeTexture_Sizeof_00a67f30.cpp','?ReleaseTexture@CVolumeTexture@@IAEXXZ',0xa67f60,132,0,6)]
targets={symbol:address for rel,symbol,address,size,offset,count in specs};targets.update({'?g_FableMemoryAllocator_013CAA90@@3PAVCMemoryAllocator@@A':0x13caa90,'?GetAllocationSize@CMemoryAllocator@@QAEKPAX@Z':0x9f2390,'?g_FableAllocatorMemoryBaseline_013CAAB0@@3KA':0x13caab0,'?TotalMemory@CVolumeTexture@@2KA':0x13d28e0});offsets={address:offset for rel,symbol,address,size,offset,count in specs};offsets.update({0x13caa90:0,0x9f2390:1,0x13caab0:2,0x13d28e0:3})

objects=[];sources=[];proofs=[];body=bytearray(256);raw=EXE.read_bytes();cs=Cs(CS_ARCH_X86,CS_MODE_32);cs.detail=True;fixups=[]
for rel,symbol,address,size,offset,count in specs:
 source=ROOT/'rebuild/src/compiled'/rel
 if not source.exists() or 'CVolumeTexture::ReleaseTexture' not in source.read_text():source=ROOT/'work/parity_addtext/volume_texture_release_candidate_20261009.cpp'
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
assert len(fixups)==6,fixups
from gate_render_state_static_data import coff_data
dataSource=ROOT/'rebuild/src/compiled/01/3b/GFMainSingletonCodeData_013b7d58.cpp';dataObj=w/'globals.obj';run=V.cl(['/nologo','/c','/O2','/MT','/I'+str(ROOT/'rebuild/include'),'/Fo'+str(dataObj),str(dataSource)],env,w);assert not run.returncode,run.stdout+run.stderr
assert coff_data(Coff(dataObj),'?g_FableMemoryAllocator_013CAA90@@3PAVCMemoryAllocator@@A',4)==read_virtual_data(raw,0x13caa90,4)==bytes(4)
objects.append(dataObj);sources.append(dataSource)
assert coff_data(Coff(objects[0]),'?TotalMemory@CVolumeTexture@@2KA',4)==read_virtual_data(raw,0x13d28e0,4)==bytes(4)
baselineSource=ROOT/'rebuild/src/compiled/01/39/MemoryAllocatorData_01396868.cpp';baselineObj=w/'baseline.obj';run=V.cl(['/nologo','/c','/O2','/MT','/I'+str(ROOT/'rebuild/include'),'/Fo'+str(baselineObj),str(baselineSource)],env,w);assert not run.returncode,run.stdout+run.stderr
assert coff_data(Coff(baselineObj),'?g_FableAllocatorMemoryBaseline_013CAAB0@@3KA',4)==read_virtual_data(raw,0x13caab0,4)==bytes(4)
objects.append(baselineObj);sources.append(baselineSource)
proofs.append(check(objects[0],'?Sizeof@CVolumeTexture@@QBEKXZ',0xa67f30,39,2,targets))
(w/'volume_texture_release_retail.h').write_text('struct Fixup{unsigned site,target,kind;};\nstatic const unsigned char retailCode[]={'+','.join(map(str,body))+'};\nstatic const Fixup fixups[]={'+','.join('{%u,%u,%u}'%f for f in fixups)+'};\n')
(w/'volume_texture_release_aliases.h').write_text('\n'.join('#pragma comment(linker,"/alternatename:'+a+'='+b+'")' for a,b in {'@VolumeTextureRelease@8':specs[0][1],'?GetAllocationSize@CMemoryAllocator@@QAEKPAX@Z':'@AllocationSize@12','?CalcByteLength@CVolumeTexture@@IAEXXZ':'_AbortBoundary','?Uninitialise@CVolumeTexture@@QAEXXZ':'_AbortBoundary'}.items())+'\n')
fixture=ROOT/'rebuild/tests/integration/NativeVolumeTextureRelease_test.cpp';fo=w/'fixture.obj';r=V.cl(['/nologo','/c','/O2','/MT','/I'+str(w),'/I'+str(ROOT/'rebuild/include'),'/Fo'+str(fo),str(fixture)],env,w);assert not r.returncode,r.stdout+r.stderr
cmd=[str(V.VC/'bin/link.exe'),'/nologo','/out:'+str(w/'fixture.exe'),'kernel32.lib',str(fo)]
for omit in range(3):
 r=subprocess.run(cmd+[str(o) for i,o in enumerate(objects) if i!=omit],env=env,capture_output=True,text=True);assert r.returncode and ('LNK2001' in r.stdout or 'LNK2019' in r.stdout),r.stdout+r.stderr
r=subprocess.run(cmd+list(map(str,objects)),env=env,capture_output=True,text=True);assert not r.returncode,r.stdout+r.stderr
r=subprocess.run([str(w/'fixture.exe')],env=env,capture_output=True,text=True,timeout=30);assert not r.returncode,(w,r.returncode,r.stdout,r.stderr)
assert 'cases=13824 snapshots=85248' in r.stdout
headers=[ROOT/'rebuild/include'/p for p in ('fable_volume_types.h','fable_base_class.h','fable_extents_i.h','fable_surface_types.h','fable_d3d_format_info.h','fable_d3d9.h','fable_memory_allocator.h')]
report=dict(cases=13824,snapshots=int(r.stdout.split('snapshots=')[1]),providers=proofs,omission_checks=3,result=r.stdout.strip(),source_sha256={str(p.relative_to(ROOT)):hashlib.sha256(p.read_bytes()).hexdigest() for p in sources+headers},fixture_sha256=hashlib.sha256(fixture.read_bytes()).hexdigest(),boundary='Complete132B/49instructions/6relocs original protected noarg CVolumeTexture::ReleaseTexture, genuine normal Sizeof39B inlines twice into sameTU parent while standalone remains exact. Three mandatory sources including original singleton and allocatorbaseline data;new original public static TotalMemory4B at13D28E0, donor1330094, COFFBSS/native zero bytes verified. NullPD no changes;live AddRef then independentlyreloaded Release, only full32bit ReleaseResult==2 subtracts two independentlyqueried Sizeof values from baseline then TotalMemory,final PD reload/Release then clear. Only COMAddRef/Release and allocatorGetAllocationSize boundaries;full16KiB+12B globals snapshots,callback owner/global/interface mutations,independent accountingqueries/counterwrap,pointee replacements/nullslot/pitches unused/two placements-fills. No standaloneSizeof/singleton/baseline additional coverage.')
(ROOT/'rebuild/compile-gate/native-volume-texture-release.json').write_text(json.dumps(report,indent=2)+'\n');print(r.stdout.strip());print('NATIVE_VOLUME_TEXTURE_RELEASE_GATE_PASS')
