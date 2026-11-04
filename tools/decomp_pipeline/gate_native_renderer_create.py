"""Renderer map creation against original native execution."""
import hashlib,itertools,json,struct,subprocess,tempfile
from pathlib import Path
from capstone import Cs,CS_ARCH_X86,CS_MODE_32
import check_one as V
from variable_allocator_parity import check
from gate_game_component_abi import Coff
from pe_oracle import EXE,read_virtual_data
ROOT=Path(__file__).resolve().parents[2];w=Path(tempfile.mkdtemp(prefix='native-renderer-create-'));env=V.env()
source=ROOT/'rebuild/src/compiled/00/b6/MapRenderers_CreateMap_00b67140.cpp';fixture=ROOT/'rebuild/tests/integration/NativeRendererCreate_test.cpp'
land='?CreateMap@CEngineLandscapeRenderer@@QAEXPAVCEngineMap@@J@Z';detail='?CreateMap@CEngineLocalDetailGenerator@@QAEXPAVCEngineMap@@@Z';targets={'??2@YAPAXI@Z':0xbfea1a,'??0CEngineLandscapeMap@@QAE@PBVCEngineMap@@J@Z':0xbdd2f0,'??0CLocalDetailCacheMap@@QAE@PAVCEngineMap@@@Z':0xbdefc0}
code=bytearray(114);fixups=[];md=Cs(CS_ARCH_X86,CS_MODE_32);md.detail=True;raw=EXE.read_bytes()
for address,size,offset in [(0xb67140,49,0),(0xb59800,50,64)]:
 body=read_virtual_data(raw,address,size);code[offset:offset+size]=body
 for i in md.disasm(body,address):
  if i.mnemonic=='call':
   assert i.operands[0].type==2 and i.operands[0].imm in targets.values();fixups.append((offset+i.address-address+1,i.operands[0].imm))
assert len(fixups)==4
(w/'renderer_create_retail.h').write_text('static const unsigned char retailCode[]={'+','.join(map(str,code))+'};\nstatic const unsigned retailFixups[][2]={'+','.join('{%d,%d}'%f for f in fixups)+'};\n')
objects=[]
for i,p in enumerate((source,fixture)):
 o=w/(str(i)+'.obj');r=V.cl(['/nologo','/c','/O2','/MT','/I'+str(w),'/Fo'+str(o)]+[str(p)],env,w);assert not r.returncode,r.stdout+r.stderr;objects.append(o)
proofs=[check(objects[0],land,0xb67140,49,2,targets),check(objects[0],detail,0xb59800,50,2,targets)];assert all(p['grade']=='MATCH' for p in proofs)
cmd=[str(V.VC/'bin/link.exe'),'/nologo','/out:'+str(w/'fixture.exe'),'kernel32.lib']
r=subprocess.run(cmd+[str(objects[1])],env=env,capture_output=True,text=True);assert r.returncode
r=subprocess.run(cmd+list(map(str,objects)),env=env,capture_output=True,text=True);assert not r.returncode,r.stdout+r.stderr
cases=0
with (w/'input.bin').open('wb') as f:
 for fill,mode,nullmask,initial,flags,renderer,order in itertools.product((0,0x95),range(4),range(4),range(4),(0,1,0xffffffff,0x80000000),(0,0x24000600),(0,1)):
  a=bytearray([fill])*2048;struct.pack_into('<2I',a,0,0x24000700 if initial&1 else 0,0x24000780 if initial&2 else 0)
  f.write(struct.pack('<5I',mode,nullmask,flags,renderer,order));f.write(a);cases+=1
for kind in ('retail','compiled'):
 r=subprocess.run([str(w/'fixture.exe'),str(w/'input.bin'),str(w/(kind+'.bin')),kind],env=env,capture_output=True,text=True,timeout=60);assert not r.returncode,(w,r.returncode,r.stdout,r.stderr);print(r.stdout.strip(),kind,flush=True)
with (w/'retail.bin').open('rb') as e,(w/'compiled.bin').open('rb') as f:
 while True:
  x=e.read(1<<20);y=f.read(1<<20);assert x==y,(w,'constructor native mismatch')
  if not x:break
size=(w/'compiled.bin').stat().st_size;assert size%2056==0;snapshots=size//2056
headers=[ROOT/'rebuild/include'/p for p in ('engine/CEngineMap.h','engine/CEngineLandscapeRenderer.h','engine/CEngineLocalDetailGenerator.h','engine/CLocalDetailCacheMap.h','candidates/landscape_background_lod_compiled.h')]
report=dict(cases=cases,snapshots=snapshots,omission_checks=1,providers=proofs,source_sha256={str(p.relative_to(ROOT)):hashlib.sha256(p.read_bytes()).hexdigest() for p in [source]+headers},fixture_sha256=hashlib.sha256(fixture.read_bytes()).hexdigest(),boundary='Original49/50byte renderer creation spans,fouraudited fixups. Full2KiB snapshots for map and allocated children;two creation rounds in both orders. Existing child masks,null-allocation masks,flags0/1/ffffffff/80000000,ignored renderer this null/live,zero/poison fills. Allocator/constructor callbacks mutate child pointers;native caller overwrite and conditional detail creation verified. Operator new and child constructors remain boundaries;allocation sizes60/70 checked.')
(ROOT/'rebuild/compile-gate/native-renderer-create.json').write_text(json.dumps(report,indent=2)+'\n');print('NATIVE_RENDERER_CREATE_GATE_PASS',snapshots,'snapshots')
