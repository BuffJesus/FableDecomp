"""Texture loading composed with actual non-POD palette vector operations."""
import hashlib,itertools,json,struct,subprocess,tempfile
from pathlib import Path
from capstone import Cs,CS_ARCH_X86,CS_MODE_32
import check_one as V
from variable_allocator_parity import check
from gate_game_component_abi import Coff
from pe_oracle import EXE,read_virtual_data
ROOT=Path(__file__).resolve().parents[2];w=Path(tempfile.mkdtemp(prefix='native-texture-palette-load-vector-'));env=V.env()
source=ROOT/'rebuild/src/compiled/00/bf/CEngineTexturePalette_Load_00bf9290.cpp';fixture=ROOT/'rebuild/tests/integration/NativeTexturePaletteLoadVector_test.cpp'
symbol='?Load@CEngineTexturePalette@@QAEXAAVCDataInputStream@@@Z'
targets={'?ReadWithSrcChunkOverflow@CDataInputStream@@QAEXPAXJ@Z':0x993ca0,'?resize@?$vector@UCPaletteEntry@CEngineTexturePalette@@V?$allocator@UCPaletteEntry@CEngineTexturePalette@@@_STL@@@_STL@@QAEXI@Z':0xbf97a0,'??4CCharString@@QAEAAV0@ABV0@@Z':0x99efb0,'??1CCharString@@QAE@XZ':0x99eae0,'?FindIndexBySymbol@CBankFile@@QBEKABVCCharString@@@Z':0x9ccdc0}
vector_source=ROOT/'rebuild/src/compiled/00/bf/CEngineTexturePalette_VectorResize_00bf97a0.cpp'
vo=w/'vector.obj';r=V.cl(['/nologo','/c','/O2','/MT','/Fo'+str(vo),str(vector_source)],env,w);assert not r.returncode,r.stdout+r.stderr;c=Coff(vo)
load_targets=targets.copy()
specs=[(next(s for s in c.symbols if s.startswith(pre) and (not suffix or s.endswith(suffix))),a,n,r) for pre,suffix,a,n,r in [('?resize@','QAEXI@Z',0xbf97a0,108,5),('?erase@','',0xbf9420,77,2),('?_M_fill_insert@','',0xbf95d0,280,8),('??$__copy@','',0xbf8e50,61,1),('??$__copy_backward@','',0xbf8e90,64,1),('??$fill@','',0xbf8e20,46,1),('??$uninitialized_fill_n@','',0xbf8ff0,50,1),('?_M_insert_overflow@','',0xbf94d0,249,7),('??$__uninitialized_copy@','',0xbf8f30,54,1),('??$__uninitialized_fill_n@','',0xbf8f70,50,1)]]
targets={'??0CCharString@@QAE@XZ':0x99e4b0,'??0CCharString@@QAE@ABV0@@Z':0x99ec30,'??4CCharString@@QAEAAV0@ABV0@@Z':0x99efb0,'??1CCharString@@QAE@XZ':0x99eae0,'_malloc':0xbfea0e,'_free':0xbfea14};targets.update({s:a for s,a,n,r in specs})

targets.update(load_targets)
boundary={0x993ca0:'@Overflow@16',0x99efb0:'@Assign@12',0x99eae0:'@Destroy@4',0x9ccdc0:'@Find@12',0x99e4b0:'@Default@4',0x99ec30:'@Copy@12'}
aliases={'@Load@12':symbol};aliases.update({s:boundary[a] for s,a in targets.items() if a in boundary})
(w/'palette_load_vector_aliases.h').write_text('\n'.join('#pragma comment(linker,"/alternatename:'+a+'='+b+'")' for a,b in aliases.items())+'\n')
body=bytearray(1650);raw=EXE.read_bytes();md=Cs(CS_ARCH_X86,CS_MODE_32);md.detail=True;fixups=[]
offsets=[0,128,224,512,576,640,704,768,1024,1088]
for (s,address,length,rel),offset in [( (symbol,0xbf9290,362,9),0)]+list(zip(specs,[512+o for o in offsets])):
 span=read_virtual_data(raw,address,length);body[offset:offset+length]=span
 for i in md.disasm(span,address):
  if i.mnemonic=='call' and i.operands[0].type==2:
   assert i.operands[0].imm in targets.values();fixups.append((offset+i.address-address+1,i.operands[0].imm))
assert len(fixups)==37
(w/'palette_load_vector_retail.h').write_text('static const unsigned char retailCode[]={'+','.join(map(str,body))+'};\nstatic const unsigned retailFixups[][2]={'+','.join('{%d,%d}'%f for f in fixups)+'};\n')
objects=[]
for n,p in enumerate((source,fixture,vector_source)):
 o=w/(str(n)+'.obj');r=V.cl(['/nologo','/c','/O2','/MT','/I'+str(w),'/Fo'+str(o)]+(['/Dmalloc=Allocate','/Dfree=Free'] if n==2 else [])+[str(p)],env,w);assert not r.returncode,r.stdout+r.stderr;objects.append(o)
proof=check(objects[0],symbol,0xbf9290,362,9,targets)
vector_proof=[check(vo,s,a,n,r,targets) for s,a,n,r in specs]
d=Coff(objects[2])
for s,a,n,r in specs:
 sec,off=c.symbols[s];sec1,off1=d.symbols[s];b,rel=c.sections[sec];b1,rel1=d.sections[sec1];assert off==off1 and b==b1 and {p:({'_malloc':'_Allocate','_free':'_Free'}.get(t,t),k) for p,(t,k) in rel.items()}==rel1
cmd=[str(V.VC/'bin/link.exe'),'/nologo','/out:'+str(w/'fixture.exe'),'kernel32.lib']
for missing in (0,2):
 r=subprocess.run(cmd+[str(o) for n,o in enumerate(objects) if n!=missing],env=env,capture_output=True,text=True);assert r.returncode
r=subprocess.run(cmd+list(map(str,objects)),env=env,capture_output=True,text=True);assert not r.returncode,r.stdout+r.stderr
cases=0;base=0x24000000
with (w/'input.bin').open('wb') as f:
 for fill,mode,retail,count,remaining,position,found,capacity in itertools.product((0,0x95),range(7),(0,1,0x80,0xff),(1,2,4),(0,1,4,5),(0,0xffffffff),(0,1,0x80000000,0xffffffff),(1,4)):
  a=bytearray([fill])*4096
  def put(off,v):struct.pack_into('<I',a,off,v&0xffffffff)
  put(0,base+0x400);put(4,base+0x500);put(8,base+0x508);put(12,base+0x500+capacity*8);put(0x10,0x778899aa);a[0x14]=0x95
  put(0x204,position);put(0x208,128);put(0x20c,base+0x300);put(0x214,remaining);a[0x300]=retail;put(0x301,count)
  f.write(struct.pack('<2I',mode,found));f.write(a);cases+=1
for kind in ('retail','compiled'):
 r=subprocess.run([str(w/'fixture.exe'),str(w/'input.bin'),str(w/(kind+'.bin')),kind],env=env,capture_output=True,text=True,timeout=60);assert not r.returncode,(w,r.returncode,r.stdout,r.stderr);print(r.stdout.strip(),kind,flush=True)
with (w/'retail.bin').open('rb') as e,(w/'compiled.bin').open('rb') as f:
 while True:
  x=e.read(1<<20);y=f.read(1<<20);assert x==y,(w,'native mismatch')
  if not x:break
size=(w/'compiled.bin').stat().st_size;assert size%4108==0;snapshots=size//4108
report=dict(cases=cases,snapshots=snapshots,omission_checks=2,provider=proof,vector_providers=vector_proof,vector_sha256=hashlib.sha256(vector_source.read_bytes()).hexdigest(),source_sha256=hashlib.sha256(source.read_bytes()).hexdigest(),fixture_sha256=hashlib.sha256(fixture.read_bytes()).hexdigest(),boundary='Eleven original spans/37 audited fixups. Actual load and all ten vector helpers; two required providers, allocation renames audited. Full4KiB snapshots; spare capacity and allocation growth, flags, stream splits/wrap, lookup values, reentrant vector/bank/default changes. String lifecycle, stream strings/overflow, bank lookup and malloc/free remain boundaries. Default string callback initializes companion temporary Index for deterministic comparison.')
(ROOT/'rebuild/compile-gate/native-texture-palette-load-vector.json').write_text(json.dumps(report,indent=2)+'\n');print('NATIVE_TEXTURE_PALETTE_LOAD_VECTOR_GATE_PASS',snapshots,'snapshots')
