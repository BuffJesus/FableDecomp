"""Map constructor against original native execution."""
import hashlib,itertools,json,struct,subprocess,tempfile
from pathlib import Path
from capstone import Cs,CS_ARCH_X86,CS_MODE_32
import check_one as V
from variable_allocator_parity import check
from gate_game_component_abi import Coff
from pe_oracle import EXE,read_virtual_data
ROOT=Path(__file__).resolve().parents[2];w=Path(tempfile.mkdtemp(prefix='native-map-constructor-'));env=V.env()
source=ROOT/'rebuild/src/compiled/00/b4/CEngineMap_Constructor_00b429e0.cpp';fixture=ROOT/'rebuild/tests/integration/NativeMapConstructor_test.cpp'
symbol='??0CEngineMap@@QAE@PBVCIDrawMap@@JJJJJVCCharString@@_N@Z';targets={'??0CCharString@@QAE@XZ':0x99e4b0,'??1CCharString@@QAE@XZ':0x99eae0,'_malloc':0xbfea0e,'??4CCharString@@QAEAAV0@ABV0@@Z':0x99efb0}
code=read_virtual_data(EXE.read_bytes(),0xb429e0,172);fixups=[];md=Cs(CS_ARCH_X86,CS_MODE_32);md.detail=True
for i in md.disasm(code,0xb429e0):
 if i.mnemonic=='call':
  assert i.operands[0].type==2 and i.operands[0].imm in targets.values();fixups.append((i.address-0xb429e0+1,i.operands[0].imm))
assert len(fixups)==4
(w/'map_constructor_retail.h').write_text('static const unsigned char retailCode[]={'+','.join(map(str,code))+'};\nstatic const unsigned retailFixups[][2]={'+','.join('{%d,%d}'%f for f in fixups)+'};\n')
objects=[]
for i,p in enumerate((source,fixture,source)):
 o=w/(str(i)+'.obj');r=V.cl(['/nologo','/c','/O2','/MT','/I'+str(w),'/Fo'+str(o)]+(['/Dmalloc=MapAllocate'] if i==2 else [])+[str(p)],env,w);assert not r.returncode,r.stdout+r.stderr;objects.append(o)
proof=check(objects[0],symbol,0xb429e0,172,4,targets);assert proof['grade']=='MATCH'
c,d=Coff(objects[0]),Coff(objects[2]);sec,off=c.symbols[symbol];sec1,off1=d.symbols[symbol];body,rel=c.sections[sec];body1,rel1=d.sections[sec1];assert off==off1 and body==body1 and {p:('_MapAllocate' if t=='_malloc' else t,k) for p,(t,k) in rel.items()}==rel1
cmd=[str(V.VC/'bin/link.exe'),'/nologo','/out:'+str(w/'fixture.exe'),'kernel32.lib']
r=subprocess.run(cmd+[str(objects[1])],env=env,capture_output=True,text=True);assert r.returncode
r=subprocess.run(cmd+list(map(str,objects[1:])),env=env,capture_output=True,text=True);assert not r.returncode,r.stdout+r.stderr
cases=0
with (w/'input.bin').open('wb') as f:
 for fill,mode,game,xy,dims,uid,name,flag in itertools.product((0,0x95),range(4),(0,0x24000700),((0,0),(31,32),(65535,65536),(-1,-32768),(0x7fffffff,-0x80000000)),((0,0),(1,33),(65535,65536),(-1,-2)),(0,0xffffffff),(0,0x99887766),(0,1)):
  p=(mode,game,*xy,*dims,uid,name,flag);f.write(struct.pack('<9I',*(v&0xffffffff for v in p)));f.write(bytes([fill])*2048);cases+=1
for kind in ('retail','compiled'):
 r=subprocess.run([str(w/'fixture.exe'),str(w/'input.bin'),str(w/(kind+'.bin')),kind],env=env,capture_output=True,text=True,timeout=60);assert not r.returncode,(w,r.returncode,r.stdout,r.stderr);print(r.stdout.strip(),kind,flush=True)
with (w/'retail.bin').open('rb') as e,(w/'compiled.bin').open('rb') as f:
 while True:
  x=e.read(1<<20);y=f.read(1<<20);assert x==y,(w,'constructor native mismatch')
  if not x:break
snapshots=(w/'compiled.bin').stat().st_size//2056;assert snapshots==cases*5
headers=[ROOT/'rebuild/include'/p for p in ('engine/CEngineMap.h','engine/CEngineStaticMapFileBlockManager.h','fable_2d_vector.h','fable_stlport_tree.h')]
report=dict(cases=cases,snapshots=snapshots,omission_checks=1,providers=[proof],source_sha256={str(p.relative_to(ROOT)):hashlib.sha256(p.read_bytes()).hexdigest() for p in [source]+headers},fixture_sha256=hashlib.sha256(fixture.read_bytes()).hexdigest(),boundary='Original172bytes/fouraudited fixups versus compiled constructor. Full2KiB snapshots including map and allocated tree sentinel. Zero/poison fills;null/live game map;coordinate signed extremes and truncation;dimension0/1/33/65535/65536/negative;UID/namezero/nonzero;ignored bool both values. String constructor,assignment,by-value destructor and malloc remain boundaries;callbacks overwrite object/name/visibility before later stores. Malloc-renamed fixture body/relocations audited unchanged;actual typed member constructors build counted pointers and red-black tree sentinel.')
(ROOT/'rebuild/compile-gate/native-map-constructor.json').write_text(json.dumps(report,indent=2)+'\n');print('NATIVE_MAP_CONSTRUCTOR_GATE_PASS',snapshots,'snapshots')
