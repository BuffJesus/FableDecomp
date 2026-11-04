import hashlib,json,subprocess,sys,tempfile
from pathlib import Path
ROOT=Path(__file__).resolve().parents[2];sys.path.insert(0,str(ROOT/'tools/decomp_pipeline'))
import check_one as V
from variable_allocator_parity import check
from pe_oracle import EXE,read_virtual_data
from gate_game_component_abi import Coff
from capstone import Cs,CS_ARCH_X86,CS_MODE_32
w=Path(tempfile.mkdtemp(prefix='native-surface-byte-length-'));env=V.env()
specs=[('00/9f/CSurface_GetByteLength_009f2fc0.cpp','?GetByteLength@CSurface@@QBEKXZ',0x9f2fc0,75,0,2),('00/9e/CPixelFormat_Initialise_009e3830.cpp','?Initialise@CPixelFormat@@QAEXW4_D3DFORMAT@@@Z',0x9e3830,50,128,1),('00/9e/CPixelFormat_GetColourDepth_009e3820.cpp','?GetColourDepth@CPixelFormat@@QBEKXZ',0x9e3820,13,192,1)]

targets={symbol:address for rel,symbol,address,size,offset,count in specs};targets['?GD3DFormats@@3QBVCD3DFormatInfo@@B']=0x129ba40;offsets={address:offset for rel,symbol,address,size,offset,count in specs};offsets.update({0x129ba40:0x80000000,0x129ba48:0x80000008})

objects=[];sources=[];proofs=[];body=bytearray(256);raw=EXE.read_bytes();cs=Cs(CS_ARCH_X86,CS_MODE_32);cs.detail=True;fixups=[]
for rel,symbol,address,size,offset,count in specs:
 source=ROOT/'rebuild/src/compiled'/rel
 if not source.exists():source=ROOT/'work/parity_addtext/surface_byte_length_candidate1_20261009.cpp'
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
assert len(fixups)==4,fixups
tableSource=ROOT/'rebuild/src/compiled/01/29/CPixelFormat_Table_0129ba40.cpp';tableObj=w/'table.obj';r=V.cl(['/nologo','/c','/O2','/MT','/I'+str(ROOT/'rebuild/include'),'/Fo'+str(tableObj),str(tableSource)],env,w);assert not r.returncode,r.stdout+r.stderr;objects.append(tableObj);sources.append(tableSource);table=Coff(tableObj);sec,off=table.symbols['?GD3DFormats@@3QBVCD3DFormatInfo@@B'];tableBytes,tableRelocs=table.sections[sec];originalTable=read_virtual_data(raw,0x129ba40,47*36);assert off==0 and tableBytes==originalTable and not tableRelocs
import struct
tableWords=struct.unpack('<423I',originalTable)
(w/'surface_byte_length_retail.h').write_text('struct Fixup{unsigned site,target,kind;};\nstatic const unsigned char retailCode[]={'+','.join(map(str,body))+'};\nstatic const Fixup fixups[]={'+','.join('{%u,%u,%u}'%f for f in fixups)+'};\nstatic const unsigned retailFormats[]={'+','.join(map(str,tableWords))+'};\n')
(w/'surface_byte_length_aliases.h').write_text('\n'.join('#pragma comment(linker,"/alternatename:'+a+'='+b+'")' for a,b in {'@SurfaceByteLength@8':specs[0][1]}.items())+'\n')
fixture=ROOT/'rebuild/tests/integration/NativeSurfaceByteLength_test.cpp';fo=w/'fixture.obj';r=V.cl(['/nologo','/c','/O2','/MT','/I'+str(w),'/I'+str(ROOT/'rebuild/include'),'/Fo'+str(fo),str(fixture)],env,w);assert not r.returncode,r.stdout+r.stderr
cmd=[str(V.VC/'bin/link.exe'),'/nologo','/out:'+str(w/'fixture.exe'),'kernel32.lib',str(fo)]
for omit in range(4):
 r=subprocess.run(cmd+[str(o) for i,o in enumerate(objects) if i!=omit],env=env,capture_output=True,text=True);assert r.returncode and ('LNK2001' in r.stdout or 'LNK2019' in r.stdout),r.stdout+r.stderr
r=subprocess.run(cmd+list(map(str,objects)),env=env,capture_output=True,text=True);assert not r.returncode,r.stdout+r.stderr
r=subprocess.run([str(w/'fixture.exe')],env=env,capture_output=True,text=True,timeout=30);assert not r.returncode,(w,r.returncode,r.stdout,r.stderr)
assert 'cases=94208 snapshots=565248' in r.stdout
headers=[ROOT/'rebuild/include'/p for p in ('fable_surface_types.h','fable_d3d_format_info.h','fable_d3d9.h')]
report=dict(cases=94208,snapshots=int(r.stdout.split('snapshots=')[1]),providers=proofs,omission_checks=4,result=r.stdout.strip(),source_sha256={str(p.relative_to(ROOT)):hashlib.sha256(p.read_bytes()).hexdigest() for p in sources+headers},fixture_sha256=hashlib.sha256(fixture.read_bytes()).hexdigest(),boundary='Original CSurface::GetByteLength75B/27instructions/2relocations accepted first near, actual PixelFormatInitialise50B/GetColourDepth13B/original1692B table:four mandatory sources,all four omissions fail.Only two COMGetDesc callbacks.94208cases/565248full16KiB original/model/compiled callback/final snapshots,46supported formats,width-height unsigned endpoints/overflow,two HRESULTs/four callback mutations/two placements-fills. First descriptor dimensions and second descriptor format retained; interface reloaded after first callback and first descriptor may be mutated by second callback. HRESULTs ignored with valid descriptor outputs.No unknown-format depth query or realGPUclaim.')
(ROOT/'rebuild/compile-gate/native-surface-byte-length.json').write_text(json.dumps(report,indent=2)+'\n');print(r.stdout.strip());print('NATIVE_SURFACE_BYTE_LENGTH_GATE_PASS')
