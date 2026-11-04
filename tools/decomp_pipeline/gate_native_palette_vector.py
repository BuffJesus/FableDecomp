"""Palette vector resize/insert/erase and non-POD helpers against retail."""
import hashlib,itertools,json,struct,subprocess,tempfile
from pathlib import Path
from capstone import Cs,CS_ARCH_X86,CS_MODE_32
import check_one as V
from variable_allocator_parity import check
from gate_game_component_abi import Coff
from pe_oracle import EXE,read_virtual_data
ROOT=Path(__file__).resolve().parents[2];w=Path(tempfile.mkdtemp(prefix='native-palette-vector-'));env=V.env()
source=ROOT/'rebuild/src/compiled/00/bf/CEngineTexturePalette_VectorResize_00bf97a0.cpp';fixture=ROOT/'rebuild/tests/integration/NativePaletteVector_test.cpp'
o=w/'source.obj';r=V.cl(['/nologo','/c','/O2','/MT','/Fo'+str(o),str(source)],env,w);assert not r.returncode,r.stdout+r.stderr;c=Coff(o)
specs=[(next(s for s in c.symbols if s.startswith(pre) and (not suffix or s.endswith(suffix))),a,n,r) for pre,suffix,a,n,r in [('?resize@','QAEXI@Z',0xbf97a0,108,5),('?erase@','',0xbf9420,77,2),('?_M_fill_insert@','',0xbf95d0,280,8),('??$__copy@','',0xbf8e50,61,1),('??$__copy_backward@','',0xbf8e90,64,1),('??$fill@','',0xbf8e20,46,1),('??$uninitialized_fill_n@','',0xbf8ff0,50,1),('?_M_insert_overflow@','',0xbf94d0,249,7),('??$__uninitialized_copy@','',0xbf8f30,54,1),('??$__uninitialized_fill_n@','',0xbf8f70,50,1)]]
targets={'??0CCharString@@QAE@XZ':0x99e4b0,'??0CCharString@@QAE@ABV0@@Z':0x99ec30,'??4CCharString@@QAEAAV0@ABV0@@Z':0x99efb0,'??1CCharString@@QAE@XZ':0x99eae0,'_malloc':0xbfea0e,'_free':0xbfea14};targets.update({s:a for s,a,n,r in specs})
aliases={'@Resize@12':specs[0][0],'@Erase@16':specs[1][0],'@Insert@20':specs[2][0],'??0CCharString@@QAE@XZ':'@Default@4','??0CCharString@@QAE@ABV0@@Z':'@Copy@12','??4CCharString@@QAEAAV0@ABV0@@Z':'@Assign@12','??1CCharString@@QAE@XZ':'@Destroy@4'}
(w/'palette_vector_aliases.h').write_text('\n'.join('#pragma comment(linker,"/alternatename:'+a+'='+b+'")' for a,b in aliases.items())+'\n')
raw=EXE.read_bytes();body=bytearray(1138);fixups=[];md=Cs(CS_ARCH_X86,CS_MODE_32);md.detail=True
offsets=[0,128,224,512,576,640,704,768,1024,1088]
for (s,address,length,rel),offset in zip(specs,offsets):
 span=read_virtual_data(raw,address,length);body[offset:offset+length]=span
 for i in md.disasm(span,address):
  if i.mnemonic=='call' and i.operands[0].type==2:
   a=i.operands[0].imm;assert a in targets.values();fixups.append((offset+i.address-address+1,a))
assert len(fixups)==28
(w/'palette_vector_retail.h').write_text('static const unsigned char retailCode[]={'+','.join(map(str,body))+'};\nstatic const unsigned retailFixups[][2]={'+','.join('{%d,%d}'%f for f in fixups)+'};\n')
objects=[o]
for n,p in enumerate((fixture,source)):
 obj=w/(str(n)+'.obj');r=V.cl(['/nologo','/c','/O2','/MT','/I'+str(w),'/Fo'+str(obj)]+(['/Dmalloc=Allocate','/Dfree=Free'] if n==1 else [])+[str(p)],env,w);assert not r.returncode,r.stdout+r.stderr;objects.append(obj)
proof=[check(objects[0],s,a,n,r,targets) for s,a,n,r in specs]
d=Coff(objects[2])
for s,a,n,r in specs:
 sec,off=c.symbols[s];sec1,off1=d.symbols[s];b,rel=c.sections[sec];b1,rel1=d.sections[sec1];assert off==off1 and b==b1 and {p:({'_malloc':'_Allocate','_free':'_Free'}.get(t,t),k) for p,(t,k) in rel.items()}==rel1
cmd=[str(V.VC/'bin/link.exe'),'/nologo','/out:'+str(w/'fixture.exe'),'kernel32.lib']
r=subprocess.run(cmd+[str(objects[1])],env=env,capture_output=True,text=True);assert r.returncode
r=subprocess.run(cmd+[str(objects[1]),str(objects[2])],env=env,capture_output=True,text=True);assert not r.returncode,r.stdout+r.stderr
cases=0;base=0x24000000
with (w/'input.bin').open('wb') as f:
 for fill,mode,old,spare in itertools.product((0,0x95),range(6),(0,1,4),(0,2,8)):
  ops=[(0,n,0) for n in (0,1,2,4,6)]+[(1,p,n) for p in range(old+1) for n in (0,1,3)]+[(2,p,q) for p in range(old+1) for q in range(p,old+1)]
  for op,x,y in ops:
   a=bytearray([fill])*4096
   def put(off,v):struct.pack_into('<I',a,off,v&0xffffffff)
   put(0,base+0x200);put(4,base+0x200+old*8);put(8,base+0x200+(old+spare)*8)
   for start in (0x200,0x400):
    for j in range(16):put(start+j*8,100+j);put(start+j*8+4,200+j)
   put(0x600,0x3344);put(0x604,0x5566)
   f.write(struct.pack('<4I',mode,op,x,y));f.write(a);cases+=1
for kind in ('retail','compiled'):
 r=subprocess.run([str(w/'fixture.exe'),str(w/'input.bin'),str(w/(kind+'.bin')),kind],env=env,capture_output=True,text=True,timeout=60);assert not r.returncode,(w,r.returncode,r.stdout,r.stderr);print(r.stdout.strip(),kind,flush=True)
with (w/'retail.bin').open('rb') as e,(w/'compiled.bin').open('rb') as f:
 while True:
  x=e.read(1<<20);y=f.read(1<<20);assert x==y,(w,'native mismatch')
  if not x:break
size=(w/'compiled.bin').stat().st_size;assert size%4108==0;snapshots=size//4108
report=dict(cases=cases,snapshots=snapshots,omission_checks=1,providers=proof,source_sha256=hashlib.sha256(source.read_bytes()).hexdigest(),fixture_sha256=hashlib.sha256(fixture.read_bytes()).hexdigest(),boundary='Ten original spans/28audited call fixups. Seven new helpers plus actual existing growth/copy/fill. Full4KiB snapshots;all insert/erase positions,shrink/extend/no-op resize,spare capacity/growth,callback source mutations and default-constructor vector replacement. String lifecycle/malloc/free remain boundaries. Default-string callback initializes otherwise uninitialized companion Index for deterministic temporary comparison. Renamed allocation symbols audited across allten bodies.')
(ROOT/'rebuild/compile-gate/native-palette-vector.json').write_text(json.dumps(report,indent=2)+'\n');print('NATIVE_PALETTE_VECTOR_GATE_PASS',snapshots,'snapshots')
