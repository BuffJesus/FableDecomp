import hashlib,json,subprocess,sys,tempfile
from pathlib import Path
ROOT=Path(__file__).resolve().parents[2];sys.path.insert(0,str(ROOT/'tools/decomp_pipeline'))
import check_one as V
from variable_allocator_parity import check
from pe_oracle import EXE,read_virtual_data
from gate_game_component_abi import Coff
from capstone import Cs,CS_ARCH_X86,CS_MODE_32
w=Path(tempfile.mkdtemp(prefix='native-volume-initialise-bytes-'));env=V.env()
specs=[('00/a6/CVolume_InitialiseFromVolumeTextureMipmap_00a67770.cpp','?InitialiseFromVolumeTextureMipmap@CVolume@@QAEXPAUIDirect3DVolume9@@@Z',0xa67770,96,0,4),('00/a6/CVolume_GetByteLength_00a674e0.cpp','?GetByteLength@CVolume@@QBEKXZ',0xa674e0,77,128,2),('00/9e/CPixelFormat_Initialise_009e3830.cpp','?Initialise@CPixelFormat@@QAEXW4_D3DFORMAT@@@Z',0x9e3830,50,240,1),('00/9e/CPixelFormat_GetByteColourDepth_009e3800.cpp','?GetByteColourDepth@CPixelFormat@@QBEKXZ',0x9e3800,22,320,1)]
targets={symbol:address for rel,symbol,address,size,offset,count in specs};targets.update({'?TotalMemory@CVolume@@2KA':0x13d28dc,'?g_FableAllocatorMemoryBaseline_013CAAB0@@3KA':0x13caab0,'?GD3DFormats@@3QBVCD3DFormatInfo@@B':0x129ba40});offsets={address:offset for rel,symbol,address,size,offset,count in specs};offsets.update({0x13d28dc:0x80000000,0x13caab0:0x80000001,0x129ba40:0x80000002,0x129ba48:0x80000003})

objects=[];sources=[];proofs=[];body=bytearray(384);raw=EXE.read_bytes();cs=Cs(CS_ARCH_X86,CS_MODE_32);cs.detail=True;fixups=[]
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
assert len(fixups)==8
from gate_render_state_static_data import coff_data
rootCoff=Coff(objects[0]);assert coff_data(rootCoff,'?TotalMemory@CVolume@@2KA',4)==read_virtual_data(raw,0x13d28dc,4)==bytes(4)
dataSource=ROOT/'rebuild/src/compiled/01/39/MemoryAllocatorData_01396868.cpp';dataObj=w/'data.obj';r=V.cl(['/nologo','/c','/O2','/MT','/I'+str(ROOT/'rebuild/include'),'/Fo'+str(dataObj),str(dataSource)],env,w);assert not r.returncode,r.stdout+r.stderr;objects.append(dataObj);sources.append(dataSource);assert coff_data(Coff(dataObj),'?g_FableAllocatorMemoryBaseline_013CAAB0@@3KA',4)==read_virtual_data(raw,0x13caab0,4)==bytes(4)
tableSource=ROOT/'rebuild/src/compiled/01/29/CPixelFormat_Table_0129ba40.cpp';tableObj=w/'table.obj';r=V.cl(['/nologo','/c','/O2','/MT','/I'+str(ROOT/'rebuild/include'),'/Fo'+str(tableObj),str(tableSource)],env,w);assert not r.returncode,r.stdout+r.stderr;objects.append(tableObj);sources.append(tableSource);table=read_virtual_data(raw,0x129ba40,47*36);assert coff_data(Coff(tableObj),'?GD3DFormats@@3QBVCD3DFormatInfo@@B',47*36)==table
import struct
(w/'volume_initialise_retail.h').write_text('struct Fixup{unsigned site,target,kind;};\nstatic const unsigned char retailCode[]={'+','.join(map(str,body))+'};\nstatic const Fixup fixups[]={'+','.join('{%u,%u,%u}'%f for f in fixups)+'};\nstatic const unsigned retailFormats[]={'+','.join(map(str,struct.unpack('<423I',table)))+'};\n')
(w/'volume_initialise_aliases.h').write_text('\n'.join('#pragma comment(linker,"/alternatename:'+a+'='+b+'")' for a,b in {'@InitialiseVolume@12':specs[0][1]}.items())+'\n')
fixture=ROOT/'rebuild/tests/integration/NativeVolumeInitialiseBytes_test.cpp';fo=w/'fixture.obj';r=V.cl(['/nologo','/c','/O2','/MT','/I'+str(w),'/I'+str(ROOT/'rebuild/include'),'/Fo'+str(fo),str(fixture)],env,w);assert not r.returncode,r.stdout+r.stderr
cmd=[str(V.VC/'bin/link.exe'),'/nologo','/out:'+str(w/'fixture.exe'),'kernel32.lib',str(fo)]
for omit in range(6):
 r=subprocess.run(cmd+[str(o) for i,o in enumerate(objects) if i!=omit],env=env,capture_output=True,text=True);assert r.returncode and ('LNK2001' in r.stdout or 'LNK2019' in r.stdout),r.stdout+r.stderr
r=subprocess.run(cmd+list(map(str,objects)),env=env,capture_output=True,text=True);assert not r.returncode,r.stdout+r.stderr
r=subprocess.run([str(w/'fixture.exe')],env=env,capture_output=True,text=True,timeout=60);assert not r.returncode,(w,r.returncode,r.stdout,r.stderr)
assert 'cases=282624 snapshots=3344384' in r.stdout
headers=[ROOT/'rebuild/include'/p for p in ('fable_volume_types.h','fable_base_class.h','fable_extents_i.h','fable_surface_types.h','fable_memory_allocator.h','fable_d3d_format_info.h','fable_d3d9.h')]
report=dict(cases=282624,snapshots=int(r.stdout.split('snapshots=')[1]),providers=proofs,omission_checks=6,result=r.stdout.strip(),source_sha256={str(p.relative_to(ROOT)):hashlib.sha256(p.read_bytes()).hexdigest() for p in sources+headers},fixture_sha256=hashlib.sha256(fixture.read_bytes()).hexdigest(),data_bytes=8,new_data_addresses=['013d28dc'],boundary='Six actualsources:96B VolumeInitialise/77B VolumeByteLength/50B PixelFormatInitialise/22B ByteDepth/original1692B format table/existingallocatorcounterdata;root includesoriginalTotalMemory4B BSS. OnlyCOMRelease/AddRef/GetDesc boundaries;4descriptorqueries onfullylivebranch,firstdimensions/secondformat perByteLength call,PD reloads andtwo parentconditional sizequeries,+12live/+8null,counter32bitwrap. Independent/native/compiled full16KiB snapshots plusboth4B counters atcallbacks/final;46supportedformats/rawdimensions/HRESULTignored/slotandglobalmutations,two placements/fills. All6omissionsfail;noadditionalhelper/table/data coverage.')
(ROOT/'rebuild/compile-gate/native-volume-initialise-bytes.json').write_text(json.dumps(report,indent=2)+'\n');print(r.stdout.strip());print('NATIVE_VOLUME_INITIALISE_BYTES_GATE_PASS')
