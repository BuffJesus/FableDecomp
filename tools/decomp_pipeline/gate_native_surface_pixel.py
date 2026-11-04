"""Pixel format selection/byte depth and surface pixel address calculation."""
import hashlib,itertools,json,struct,subprocess,tempfile
from pathlib import Path
from capstone import Cs,CS_ARCH_X86,CS_MODE_32
import check_one as V
from variable_allocator_parity import check
from gate_game_component_abi import Coff
from gate_render_state_static_data import coff_data
from pe_oracle import EXE,read_virtual_data
ROOT=Path(__file__).resolve().parents[2];w=Path(tempfile.mkdtemp(prefix='native-surface-pixel-'));env=V.env()
sources=[ROOT/'rebuild/src/compiled'/p for p in ('00/9e/CPixelFormat_GetByteColourDepth_009e3800.cpp','00/9e/CPixelFormat_InitialiseRGBA_009e3790.cpp','00/9f/CSurface_SetPixel_009f31c0.cpp','00/9e/CPixelFormat_Initialise_009e3830.cpp','01/29/CPixelFormat_Table_0129ba40.cpp')]
fixture=ROOT/'rebuild/tests/integration/NativeSurfacePixel_test.cpp'
byte='?GetByteColourDepth@CPixelFormat@@QBEKXZ';rgba='?Initialise@CPixelFormat@@QAEXJJJJ_N@Z';pixel='?SetPixel@CSurface@@QAEXJJABVCRGBColour@@ABVCSurfaceLockedRect@@@Z';init='?Initialise@CPixelFormat@@QAEXW4_D3DFORMAT@@@Z';table='?GD3DFormats@@3QBVCD3DFormatInfo@@B';convert='?ConvertRGBToPFDData@CPixelFormat@@QBEXABVCRGBColour@@PAE@Z'
targets={table:0x129ba40,byte:0x9e3800,init:0x9e3830,convert:0x9e3f60}
aliases={'@ByteDepth@8':byte,'@InitRGBA@28':rgba,'@SetPixel@24':pixel,'_FormatTable':table,convert:'@Convert@16'}
(w/'surface_pixel_aliases.h').write_text('\n'.join('#pragma comment(linker,"/alternatename:'+a+'='+b+'")' for a,b in aliases.items())+'\n')
raw=EXE.read_bytes();body=bytearray(288);fixups=[];md=Cs(CS_ARCH_X86,CS_MODE_32);md.detail=True
specs=[(byte,0x9e3800,22,1,0),(rgba,0x9e3790,87,1,32),(pixel,0x9f31c0,82,3,128),(init,0x9e3830,50,1,224)]
for sym,address,length,count,offset in specs:
 code=read_virtual_data(raw,address,length);body[offset:offset+length]=code
 for i in md.disasm(code,address):
  if i.mnemonic=='call' and i.operands[0].type==2:fixups.append((offset+i.address-address+i.imm_offset,i.operands[0].imm,1))
  else:
   for o in i.operands:
    if o.type==2 and 0x129ba40<=o.imm<0x129c0dc:fixups.append((offset+i.address-address+i.imm_offset,o.imm,0))
    elif o.type==3 and 0x129ba40<=o.mem.disp<0x129c0dc:fixups.append((offset+i.address-address+i.disp_offset,o.mem.disp,0))
assert len(fixups)==6,fixups
(w/'surface_pixel_retail.h').write_text('static const unsigned char retailCode[]={'+','.join(map(str,body))+'};\nstatic const unsigned retailFixups[][3]={'+','.join('{%d,%d,%d}'%f for f in fixups)+'};\n')
objects=[]
for n,p in enumerate(sources+[fixture]):
 o=w/(str(n)+'.obj');r=V.cl(['/nologo','/c','/O2','/MT','/I'+str(w),'/Fo'+str(o),str(p)],env,w);assert not r.returncode,r.stdout+r.stderr;objects.append(o)
proofs=[check(objects[n],s,a,l,c,targets) for n,(s,a,l,c,off) in enumerate(specs)]
assert coff_data(Coff(objects[4]),table,1692)==read_virtual_data(raw,0x129ba40,1692)
cmd=[str(V.VC/'bin/link.exe'),'/nologo','/out:'+str(w/'fixture.exe'),'kernel32.lib']
for omit in range(5):
 r=subprocess.run(cmd+[str(o) for i,o in enumerate(objects) if i!=omit],env=env,capture_output=True,text=True);assert r.returncode
r=subprocess.run(cmd+list(map(str,objects)),env=env,capture_output=True,text=True);assert not r.returncode,r.stdout+r.stderr
cases=snapshots=0
with (w/'input.bin').open('wb') as f:
 def emit(p):
  global cases,snapshots
  f.write(struct.pack('<14I',*(v&0xffffffff for v in p)));cases+=1;snapshots+=3 if p[0]==2 else 1
 for base,idx in itertools.product((0,128),range(47)):emit([0,base,idx]+[0]*11)
 for base,old,bits,sign in itertools.product((0,128),(-1,0,45,0x12345678),itertools.product((-1,0,1,2,4,5,6,8,10,16),repeat=4),range(2)):
  emit([1,base,old,*bits,sign]+[0]*6)
 for base,fmt,x,y,pitch,mode,status in itertools.product((0,128),range(46),(0,3),(-2,0,2),(-16,16),range(3),(0,0x80004005)):
  emit([2,base,0,0,0,0,0,0,fmt,x,y,pitch,mode,status])
for kind in ('retail','compiled'):
 r=subprocess.run([str(w/'fixture.exe'),str(w/'input.bin'),str(w/(kind+'.bin')),kind],env=env,capture_output=True,text=True,timeout=120);assert not r.returncode,(w,r.returncode,r.stdout,r.stderr);print(r.stdout.strip(),kind,flush=True)
with (w/'retail.bin').open('rb') as f,(w/'compiled.bin').open('rb') as g:
 while True:
  a=f.read(1<<20);b=g.read(1<<20);assert a==b,w
  if not a:break
assert (w/'compiled.bin').stat().st_size==snapshots*1024
report=dict(cases=cases,snapshots=snapshots,omission_checks=5,providers=proofs,source_sha256={str(p.relative_to(ROOT)):hashlib.sha256(p.read_bytes()).hexdigest() for p in sources},fixture_sha256=hashlib.sha256(fixture.read_bytes()).hexdigest(),boundary='Actual byte-depth,RGBA initializer,SetPixel,D3DFORMAT initializer and1692byte retail table. Full1KiB independently expected snapshots. RGBA includes10000channel tuples,two signed flags,four previous indices,two placements;no match preserves previous index. Byte depth includes sentinel. Pixel addresses cover46valid inputformats,positive/negative y and pitch,COM failed HRESULT with valid description,rect/buffer/colour/receiver mutations. Convert callback independently checks selected format,address,colour then writes observable result;real pixel conversion and Lock remain unresolved. Signed multiplication stays within range;native low32 unsigned offset wrap covered.')
(ROOT/'rebuild/compile-gate/native-surface-pixel.json').write_text(json.dumps(report,indent=2)+'\n');print('NATIVE_SURFACE_PIXEL_GATE_PASS',snapshots,'snapshots')
