import hashlib,json,subprocess,sys,tempfile
from pathlib import Path
ROOT=Path(__file__).resolve().parents[2];sys.path.insert(0,str(ROOT/'tools/decomp_pipeline'))
import check_one as V
from variable_allocator_parity import check
from pe_oracle import EXE,read_virtual_data
from gate_game_component_abi import Coff
from capstone import Cs,CS_ARCH_X86,CS_MODE_32
w=Path(tempfile.mkdtemp(prefix='native-pixel-format-channel-masks-'));env=V.env()
specs=[('GetGMask',0x9e40d0,31,0,2),('GetBMask',0x9e40f0,21,32,1),('GetRScaleShift',0x9e4110,20,64,1),('GetGScaleShift',0x9e4130,20,96,1),('GetBScaleShift',0x9e4150,20,128,1),('GetAScaleShift',0x9e4170,20,160,1)]
targets={'?GD3DFormats@@3QBVCD3DFormatInfo@@B':0x129ba40}
offsets={0x129ba58:0x80000018,0x129ba50:0x80000010,0x129ba54:0x80000014,0x129ba4c:0x8000000c}

objects=[];sources=[];proofs=[];body=bytearray(256);raw=EXE.read_bytes();cs=Cs(CS_ARCH_X86,CS_MODE_32);cs.detail=True;fixups=[]
source=ROOT/'rebuild/src/compiled/00/9e/CPixelFormat_ChannelMasks_009e40d0.cpp'
if not source.exists():source=ROOT/'work/parity_addtext/pixel_format_channel_masks_verified_20261009.cpp'
obj=w/'channel.obj';r=V.cl(['/nologo','/c','/O2','/MT','/I'+str(ROOT/'rebuild/include'),'/Fo'+str(obj),str(source)],env,w);assert not r.returncode,r.stdout+r.stderr
sources.append(source);objects.append(obj)
for name,address,size,offset,count in specs:
 symbol='?'+name+'@CPixelFormat@@QBEKXZ'
 proofs.append(check(obj,symbol,address,size,count,targets));b=read_virtual_data(raw,address,size);body[offset:offset+size]=b
 for ins in cs.disasm(b,address):
  if ins.disp_size==4 and ins.disp in offsets:fixups.append((offset+ins.address-address+ins.disp_offset,offsets[ins.disp],6))
assert len(fixups)==7,fixups
tableSource=ROOT/'rebuild/src/compiled/01/29/CPixelFormat_Table_0129ba40.cpp';tableObj=w/'table.obj';r=V.cl(['/nologo','/c','/O2','/MT','/I'+str(ROOT/'rebuild/include'),'/Fo'+str(tableObj),str(tableSource)],env,w);assert not r.returncode,r.stdout+r.stderr;objects.append(tableObj);sources.append(tableSource);table=Coff(tableObj);sec,off=table.symbols['?GD3DFormats@@3QBVCD3DFormatInfo@@B'];tableBytes,tableRelocs=table.sections[sec];originalTable=read_virtual_data(raw,0x129ba40,47*36);assert off==0 and tableBytes==originalTable and not tableRelocs
import struct
tableWords=struct.unpack('<423I',originalTable)
(w/'pixel_format_channel_masks_retail.h').write_text('struct Fixup{unsigned site,target,kind;};\nstatic const unsigned char retailCode[]={'+','.join(map(str,body))+'};\nstatic const Fixup fixups[]={'+','.join('{%u,%u,%u}'%f for f in fixups)+'};\nstatic const unsigned retailFormats[]={'+','.join(map(str,tableWords))+'};\n')
(w/'pixel_format_channel_masks_aliases.h').write_text('\n'.join('#pragma comment(linker,"/alternatename:'+a+'='+b+'")' for a,b in {'@Channel%d@8'%i:'?'+x[0]+'@CPixelFormat@@QBEKXZ' for i,x in enumerate(specs)}.items())+'\n')
fixture=ROOT/'rebuild/tests/integration/NativePixelFormatChannelMasks_test.cpp';fo=w/'fixture.obj';r=V.cl(['/nologo','/c','/O2','/MT','/I'+str(w),'/I'+str(ROOT/'rebuild/include'),'/Fo'+str(fo),str(fixture)],env,w);assert not r.returncode,r.stdout+r.stderr
cmd=[str(V.VC/'bin/link.exe'),'/nologo','/out:'+str(w/'fixture.exe'),'kernel32.lib',str(fo)]
for omit in range(2):
 r=subprocess.run(cmd+[str(o) for i,o in enumerate(objects) if i!=omit],env=env,capture_output=True,text=True);assert r.returncode and ('LNK2001' in r.stdout or 'LNK2019' in r.stdout),r.stdout+r.stderr
r=subprocess.run(cmd+list(map(str,objects)),env=env,capture_output=True,text=True);assert not r.returncode,r.stdout+r.stderr
r=subprocess.run([str(w/'fixture.exe')],env=env,capture_output=True,text=True,timeout=30);assert not r.returncode,(w,r.returncode,r.stdout,r.stderr)
assert 'cases=9024 snapshots=18048' in r.stdout
headers=[ROOT/'rebuild/include'/p for p in ('fable_volume_types.h','fable_base_class.h','fable_extents_i.h','fable_surface_types.h','fable_d3d_format_info.h','fable_d3d9.h')]
report=dict(cases=9024,snapshots=int(r.stdout.split('snapshots=')[1]),providers=proofs,omission_checks=2,result=r.stdout.strip(),source_sha256={str(p.relative_to(ROOT)):hashlib.sha256(p.read_bytes()).hexdigest() for p in sources+headers},fixture_sha256=hashlib.sha256(fixture.read_bytes()).hexdigest(),boundary='Six original const unsigned-long channel mask/scale getters complete byte MATCH first candidate. Two mandatory actual sources include original1692B format table; all47rows including sentinel, six operations, eight surrounding-field patterns,two placements/fills,full16KiB original/model/compiled comparisons, originaltable unchanged. No functional call boundaries and no new table-data coverage')
(ROOT/'rebuild/compile-gate/native-pixel-format-channel-masks.json').write_text(json.dumps(report,indent=2)+'\n');print(r.stdout.strip());print('NATIVE_PIXEL_FORMAT_CHANNEL_MASKS_GATE_PASS')
