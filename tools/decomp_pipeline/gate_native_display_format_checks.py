"""Actual adapter texture/depth format queries with retail format table."""
import hashlib,itertools,json,struct,subprocess,tempfile
from pathlib import Path
from capstone import Cs,CS_ARCH_X86,CS_MODE_32
import check_one as V
from variable_allocator_parity import check
from gate_game_component_abi import Coff
from gate_render_state_static_data import coff_data
from pe_oracle import EXE,read_virtual_data
ROOT=Path(__file__).resolve().parents[2];w=Path(tempfile.mkdtemp(prefix='native-display-format-checks-'));env=V.env()
sources=[ROOT/'rebuild/src/compiled'/p for p in ('00/9b/CDisplayManager_FormatChecks_009be8f0.cpp','01/29/CPixelFormat_Table_0129ba40.cpp')];fixture=ROOT/'rebuild/tests/integration/NativeDisplayFormatChecks_test.cpp'
texture='?IsValidTextureFormat@CDisplayManager@@QAE_NABVCPixelFormat@@@Z';target='?IsValidRenderTargetFormat@CDisplayManager@@QAE_NABVCPixelFormat@@0@Z';table='?GD3DFormats@@3QBVCD3DFormatInfo@@B'
(w/'display_format_checks_aliases.h').write_text('\n'.join('#pragma comment(linker,"/alternatename:'+a+'='+b+'")' for a,b in (('@TextureFormat@12',texture),('@TargetFormat@16',target),('_FormatTable',table)))+'\n')
raw=EXE.read_bytes();body=bytearray(138);fixups=[];md=Cs(CS_ARCH_X86,CS_MODE_32);md.detail=True
for address,length,offset in ((0x9be8f0,59,0),(0x9be930,74,64)):
 code=read_virtual_data(raw,address,length);body[offset:offset+length]=code
 for i in md.disasm(code,address):
  for o in i.operands:
   if o.type==3 and o.mem.disp==0x129ba40:fixups.append(offset+i.address-address+i.disp_offset)
assert len(fixups)==3
(w/'display_format_checks_retail.h').write_text('static const unsigned char retailCode[]={'+','.join(map(str,body))+'};\nstatic const unsigned retailFixups[]={'+','.join(map(str,fixups))+'};\n')
objects=[]
for n,p in enumerate(sources+[fixture]):
 o=w/(str(n)+'.obj');r=V.cl(['/nologo','/c','/O2','/MT','/I'+str(w),'/Fo'+str(o),str(p)],env,w);assert not r.returncode,r.stdout+r.stderr;objects.append(o)
proofs=[check(objects[0],s,a,l,c,{table:0x129ba40}) for s,a,l,c in ((texture,0x9be8f0,59,1),(target,0x9be930,74,2))]
assert coff_data(Coff(objects[1]),table,1692)==read_virtual_data(raw,0x129ba40,1692)
cmd=[str(V.VC/'bin/link.exe'),'/nologo','/out:'+str(w/'fixture.exe'),'kernel32.lib']
for omit in range(2):
 r=subprocess.run(cmd+[str(o) for i,o in enumerate(objects) if i!=omit],env=env,capture_output=True,text=True);assert r.returncode
r=subprocess.run(cmd+list(map(str,objects)),env=env,capture_output=True,text=True);assert not r.returncode,r.stdout+r.stderr
cases=0
with (w/'input.bin').open('wb') as f:
 for base,colour,depth,status in itertools.product((0,0x400),range(46),range(46),(0,1,0x7fffffff,0x80000000,0x80004005,0xffffffff)):
  a=bytearray([0x95])*2048;kind=(colour+2*depth)%3;mode=(colour+depth+status)%3;cp,dp=(0x700,0x704) if kind==0 else (0x700,0x700) if kind==1 else (base+0x1c4,base+0x1c8)
  struct.pack_into('<I',a,base+0x54,0x24000600);struct.pack_into('<2I',a,base+0x5c,0x12345678,0x87654321);struct.pack_into('<I',a,base+0x1c4,23)
  struct.pack_into('<I',a,cp,colour);struct.pack_into('<I',a,dp,depth)
  f.write(struct.pack('<5I',base,status,cp,dp,mode));f.write(a);cases+=1
for kind in ('retail','compiled'):
 r=subprocess.run([str(w/'fixture.exe'),str(w/'input.bin'),str(w/(kind+'.bin')),kind],env=env,capture_output=True,text=True,timeout=60);assert not r.returncode,(w,r.returncode,r.stdout,r.stderr);print(r.stdout.strip(),kind,flush=True)
with (w/'retail.bin').open('rb') as f,(w/'compiled.bin').open('rb') as g:
 while True:
  a=f.read(1<<20);b=g.read(1<<20);assert a==b,w
  if not a:break
snapshots=cases*4;assert (w/'compiled.bin').stat().st_size==snapshots*2048
report=dict(cases=cases,snapshots=snapshots,omission_checks=2,providers=proofs,source_sha256={str(p.relative_to(ROOT)):hashlib.sha256(p.read_bytes()).hexdigest() for p in sources},fixture_sha256=hashlib.sha256(fixture.read_bytes()).hexdigest(),boundary='Actual texture/render-target format methods and1692byte table. Full2KiB independently expected event/final snapshots,all46x46valid format pairs,six signed HRESULT values,two placements,rotating aliases and callback mutations. COM slot10/12 receiver/argument checks,sign-only return check,table unchanged. Callbacks can swap D3D object or mutate format indices/adapter before second query. Invalid negative/out-of-range table indices excluded as undefined C++ inputs.')
(ROOT/'rebuild/compile-gate/native-display-format-checks.json').write_text(json.dumps(report,indent=2)+'\n');print('NATIVE_DISPLAY_FORMAT_CHECKS_GATE_PASS',snapshots,'snapshots')
