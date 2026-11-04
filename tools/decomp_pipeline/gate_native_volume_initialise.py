import hashlib,json,subprocess,sys,tempfile
from pathlib import Path
ROOT=Path(__file__).resolve().parents[2];sys.path.insert(0,str(ROOT/'tools/decomp_pipeline'))
import check_one as V
from variable_allocator_parity import check
from pe_oracle import EXE,read_virtual_data
from gate_game_component_abi import Coff
from capstone import Cs,CS_ARCH_X86,CS_MODE_32
w=Path(tempfile.mkdtemp(prefix='native-volume-initialise-'));env=V.env()
specs=[('00/a6/CVolume_InitialiseFromVolumeTextureMipmap_00a67770.cpp','?InitialiseFromVolumeTextureMipmap@CVolume@@QAEXPAUIDirect3DVolume9@@@Z',0xa67770,96,0,4)]
targets={'?GetByteLength@CVolume@@QBEKXZ':0xa674e0,'?TotalMemory@CVolume@@2KA':0x13d28dc,'?g_FableAllocatorMemoryBaseline_013CAAB0@@3KA':0x13caab0};offsets={0x13d28dc:0x80000000,0x13caab0:0x80000001,0xa674e0:0x80000002}

objects=[];sources=[];proofs=[];body=bytearray(256);raw=EXE.read_bytes();cs=Cs(CS_ARCH_X86,CS_MODE_32);cs.detail=True;fixups=[]
for rel,symbol,address,size,offset,count in specs:
 source=ROOT/'rebuild/src/compiled'/rel
 if not source.exists():source=ROOT/'work/parity_addtext/volume_initialise_candidate_20261009.cpp'
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
assert len(fixups)==4
from gate_render_state_static_data import coff_data
rootCoff=Coff(objects[0]);assert coff_data(rootCoff,'?TotalMemory@CVolume@@2KA',4)==read_virtual_data(raw,0x13d28dc,4)==bytes(4)
dataSource=ROOT/'rebuild/src/compiled/01/39/MemoryAllocatorData_01396868.cpp';dataObj=w/'data.obj';r=V.cl(['/nologo','/c','/O2','/MT','/I'+str(ROOT/'rebuild/include'),'/Fo'+str(dataObj),str(dataSource)],env,w);assert not r.returncode,r.stdout+r.stderr;objects.append(dataObj);sources.append(dataSource);assert coff_data(Coff(dataObj),'?g_FableAllocatorMemoryBaseline_013CAAB0@@3KA',4)==read_virtual_data(raw,0x13caab0,4)==bytes(4)
(w/'volume_initialise_retail.h').write_text('struct Fixup{unsigned site,target,kind;};\nstatic const unsigned char retailCode[]={'+','.join(map(str,body))+'};\nstatic const Fixup fixups[]={'+','.join('{%u,%u,%u}'%f for f in fixups)+'};\n')
(w/'volume_initialise_aliases.h').write_text('\n'.join('#pragma comment(linker,"/alternatename:'+a+'='+b+'")' for a,b in {'@InitialiseVolume@12':specs[0][1],'?GetByteLength@CVolume@@QBEKXZ':'@VolumeBytes@8'}.items())+'\n')
fixture=ROOT/'rebuild/tests/integration/NativeVolumeInitialise_test.cpp';fo=w/'fixture.obj';r=V.cl(['/nologo','/c','/O2','/MT','/I'+str(w),'/I'+str(ROOT/'rebuild/include'),'/Fo'+str(fo),str(fixture)],env,w);assert not r.returncode,r.stdout+r.stderr
cmd=[str(V.VC/'bin/link.exe'),'/nologo','/out:'+str(w/'fixture.exe'),'kernel32.lib',str(fo)]
for omit in range(2):
 r=subprocess.run(cmd+[str(o) for i,o in enumerate(objects) if i!=omit],env=env,capture_output=True,text=True);assert r.returncode and ('LNK2001' in r.stdout or 'LNK2019' in r.stdout),r.stdout+r.stderr
r=subprocess.run(cmd+list(map(str,objects)),env=env,capture_output=True,text=True);assert not r.returncode,r.stdout+r.stderr
r=subprocess.run([str(w/'fixture.exe')],env=env,capture_output=True,text=True,timeout=30);assert not r.returncode,(w,r.returncode,r.stdout,r.stderr)
assert 'cases=6144 snapshots=52736' in r.stdout
headers=[ROOT/'rebuild/include'/p for p in ('fable_volume_types.h','fable_base_class.h','fable_extents_i.h','fable_surface_types.h','fable_memory_allocator.h','fable_d3d9.h')]
report=dict(cases=6144,snapshots=int(r.stdout.split('snapshots=')[1]),providers=proofs,omission_checks=2,result=r.stdout.strip(),source_sha256={str(p.relative_to(ROOT)):hashlib.sha256(p.read_bytes()).hexdigest() for p in sources+headers},fixture_sha256=hashlib.sha256(fixture.read_bytes()).hexdigest(),data_bytes=8,new_data_addresses=['013d28dc'],boundary='Complete96B original CVolume::InitialiseFromVolumeTextureMipmap plusoriginal4B TotalMemory BSS andexisting4B allocatorcounter;two mandatorysources. OnlyCOMRelease/AddRef andalreadyrecoveredByteLength callback boundaries. Independent/native/compiled full16KiB snapshots plusboth4B counters atcallbacks/final;oldnull/twopointees/newvalidpointee,Release clearthenunconditionalnewstore/AddRef,two independentlyreloaded conditional sizequeries,+12live/+8null accounting,callbackslot/global mutations,unsignedcounter/sizewrap,two placements/fills. PDBoriginalvoid onepointerAPI andstatic TotalMemory/donor1330090/native13D28DC verified;existingallocatorcounter reused.')
(ROOT/'rebuild/compile-gate/native-volume-initialise.json').write_text(json.dumps(report,indent=2)+'\n');print(r.stdout.strip());print('NATIVE_VOLUME_INITIALISE_GATE_PASS')
