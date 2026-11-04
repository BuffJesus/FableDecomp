"""All retail pixel formats through actual texture-size, depth getter and format table."""
import hashlib,itertools,json,struct,subprocess,tempfile
from pathlib import Path
from capstone import Cs,CS_ARCH_X86,CS_MODE_32
import check_one as V
from variable_allocator_parity import check
from gate_game_component_abi import Coff
from gate_render_state_static_data import coff_data
from pe_oracle import EXE,read_virtual_data
ROOT=Path(__file__).resolve().parents[2];w=Path(tempfile.mkdtemp(prefix='texture-size-'));env=V.env()
sources=[ROOT/'rebuild/src/compiled'/p for p in ('00/a6/CTextureManager_CalculateTextureSize_00a698f0.cpp','00/9e/CPixelFormat_GetColourDepth_009e3820.cpp','01/29/CPixelFormat_Table_0129ba40.cpp')];fixture=ROOT/'rebuild/tests/integration/NativeTextureSize_test.cpp'
symbol='?CalculateTextureSize@CTextureManager@@SIKKK_NABVCPixelFormat@@@Z';depth='?GetColourDepth@CPixelFormat@@QBEKXZ';table='?GD3DFormats@@3QBVCD3DFormatInfo@@B'
(w/'texture_size_aliases.h').write_text('#pragma comment(linker,"/alternatename:@TextureSize@16='+symbol+'")\n#pragma comment(linker,"/alternatename:_FormatTable='+table+'")\n')
raw=EXE.read_bytes();body=read_virtual_data(raw,0xa698f0,136)+bytes(8)+read_virtual_data(raw,0x9e3820,13);md=Cs(CS_ARCH_X86,CS_MODE_32);md.detail=True;fixups=[]
for base,start,length in ((0xa698f0,0,136),(0x9e3820,144,13)):
 for i in md.disasm(body[start:start+length],base):
  if i.mnemonic=='call' and i.operands[0].type==2:
   assert i.operands[0].imm==0x9e3820;fixups.append((start+i.address-base+i.imm_offset,i.operands[0].imm,1))
  else:
   for op in i.operands:
    if op.type==3 and op.mem.disp in (0x129ba40,0x129ba48):fixups.append((start+i.address-base+i.disp_offset,op.mem.disp,0))
assert len(fixups)==3
(w/'texture_size_retail.h').write_text('static const unsigned char retailCode[]={'+','.join(map(str,body))+'};\nstatic const unsigned retailFixups[][3]={'+','.join('{%d,%d,%d}'%f for f in fixups)+'};\n')
objects=[]
for n,p in enumerate(sources+[fixture]):
 o=w/(str(n)+'.obj');r=V.cl(['/nologo','/c','/O2','/MT','/I'+str(w),'/Fo'+str(o),str(p)],env,w);assert not r.returncode,r.stdout+r.stderr;objects.append(o)
proofs=[check(objects[0],symbol,0xa698f0,136,2,{depth:0x9e3820,table:0x129ba40}),check(objects[1],depth,0x9e3820,13,1,{table:0x129ba40})]
table_bytes=coff_data(Coff(objects[2]),table,1692);assert table_bytes==read_virtual_data(raw,0x129ba40,1692)
cmd=[str(V.VC/'bin/link.exe'),'/nologo','/out:'+str(w/'fixture.exe'),'kernel32.lib']
for omit in range(3):
 r=subprocess.run(cmd+[str(o) for i,o in enumerate(objects) if i!=omit],env=env,capture_output=True,text=True);assert r.returncode
r=subprocess.run(cmd+list(map(str,objects)),env=env,capture_output=True,text=True);assert not r.returncode,r.stdout+r.stderr
dimensions=(0,1,2,3,4,5,7,8,15,16,31,32,255,512,513,65535,65536,0x7fffffff,0x80000000,0xffffffff);cases=0
with (w/'input.bin').open('wb') as f:
 for index,width,height,mipmaps,placement in itertools.product(range(46),dimensions,dimensions,range(2),(0,32)):
  fmt,_,bits=struct.unpack_from('<3I',table_bytes,index*36);minimum=4 if fmt in (0x31545844,0x33545844,0x35545844) else 1
  if fmt==0x31545844:bits=4
  elif fmt in (0x33545844,0x35545844):bits=8
  levels=max(width.bit_length(),height.bit_length()) if mipmaps else int(bool(width or height))
  pixels=sum(max(width>>level,minimum)*max(height>>level,minimum) for level in range(levels));expected=((pixels*bits)&0xffffffff)>>3
  a=bytearray([0x95])*64;struct.pack_into('<I',a,placement,index);f.write(struct.pack('<6I',width,height,mipmaps,index,expected,placement));f.write(a);cases+=1
for kind in ('retail','compiled'):
 r=subprocess.run([str(w/'fixture.exe'),str(w/'input.bin'),str(w/(kind+'.bin')),kind],env=env,capture_output=True,text=True,timeout=60);assert not r.returncode,(w,r.returncode,r.stdout,r.stderr);print(r.stdout.strip(),kind,flush=True)
assert (w/'retail.bin').read_bytes()==(w/'compiled.bin').read_bytes();assert (w/'compiled.bin').stat().st_size==cases*68
report=dict(cases=cases,snapshots=cases,omission_checks=3,providers=proofs,source_sha256={str(p.relative_to(ROOT)):hashlib.sha256(p.read_bytes()).hexdigest() for p in sources},fixture_sha256=hashlib.sha256(fixture.read_bytes()).hexdigest(),boundary='Actual texture size calls actual colour-depth getter and uses byte-audited1692byte retail table. All46valid formats,20x20unsigned dimensions including zero/extremes,mipmaps off/on,two format locations. Independent bit-length mip count and mathematical sum with32bit wrap;full64byte arena and all table bytes unchanged. Three mandatory sources,two native spans/threefixups,no callbacks.')
(ROOT/'rebuild/compile-gate/native-texture-size.json').write_text(json.dumps(report,indent=2)+'\n');print('NATIVE_TEXTURE_SIZE_GATE_PASS',cases,'snapshots')
