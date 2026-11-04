"""Actual signed-long vector growth, with allocator callbacks only."""
import hashlib,itertools,json,struct,subprocess,tempfile
from pathlib import Path
from capstone import Cs,CS_ARCH_X86,CS_MODE_32
import check_one as V
from gate_game_component_abi import Coff
from variable_allocator_parity import check
from pe_oracle import EXE,read_virtual_data
ROOT=Path(__file__).resolve().parents[2];w=Path(tempfile.mkdtemp(prefix='native-long-vector-'));env=V.env()
source=ROOT/'rebuild/src/compiled/00/44/LongVector_InsertOverflow_00440990.cpp';fixture=ROOT/'rebuild/tests/integration/NativeLongVector_test.cpp'
o=w/'source.obj';r=V.cl(['/nologo','/c','/O2','/MT','/Fo'+str(o),str(source)],env,w);assert not r.returncode,r.stdout+r.stderr;c=Coff(o)
symbols=['?_M_insert_overflow@?$vector@JV?$allocator@J@_STL@@@_STL@@QAEXPAJABJABU__true_type@2@I_N@Z']
targets={'_malloc':0xbfea0e,'_free':0xbfea14,'_memmove':0xbfeae6}
(w/'long_vector_aliases.h').write_text('\n'.join('#pragma comment(linker,"/alternatename:'+a+'='+b+'")' for a,b in zip(['@Grow@28'],symbols))+'\n')
raw=EXE.read_bytes();body=bytearray(216);md=Cs(CS_ARCH_X86,CS_MODE_32);md.detail=True;fixups=[]
for a,offset in [(0x440990,0)]:
 span=read_virtual_data(raw,a,216);body[offset:offset+216]=span
 for i in md.disasm(span,a):
  if i.mnemonic=='call':assert i.operands[0].type==2 and i.operands[0].imm in targets.values();fixups.append((offset+i.address-a+1,i.operands[0].imm))
assert len(fixups)==4
(w/'long_vector_retail.h').write_text('static const unsigned char retailCode[]={'+','.join(map(str,body))+'};\nstatic const unsigned retailFixups[][2]={'+','.join('{%d,%d}'%f for f in fixups)+'};\n')
proof=[check(o,s,a,216,4,targets) for s,a in zip(symbols,(0x440990,))]
objects=[]
for n,p in enumerate((fixture,source)):
 obj=w/(str(n)+'.obj');r=V.cl(['/nologo','/c','/O2','/MT','/I'+str(w),'/Fo'+str(obj)]+(['/Dmalloc=Allocate','/Dfree=Free'] if n==1 else [])+[str(p)],env,w);assert not r.returncode,r.stdout+r.stderr;objects.append(obj)
d=Coff(objects[1])
for s in symbols:
 sec,off=c.symbols[s];sec1,off1=d.symbols[s];b,rel=c.sections[sec];b1,rel1=d.sections[sec1];assert off==off1 and b==b1 and {p:({'_malloc':'_Allocate','_free':'_Free'}.get(t,t),k) for p,(t,k) in rel.items()}==rel1
cmd=[str(V.VC/'bin/link.exe'),'/nologo','/out:'+str(w/'fixture.exe'),'kernel32.lib']
r=subprocess.run(cmd+[str(objects[0])],env=env,capture_output=True,text=True);assert r.returncode
r=subprocess.run(cmd+list(map(str,objects)),env=env,capture_output=True,text=True);assert not r.returncode,r.stdout+r.stderr
cases=0;base=0x24000000
with (w/'input.bin').open('wb') as f:
 for fill,mode,kind,old,count in itertools.product((0,0x95),range(4),(0,),(0,1,3),(0,1,3)):
  for pos in range(old+1):
   for end in ((0,1) if pos==old else (0,)):
    for value in ((0x600,0x200) if old else (0x600,)):
     a=bytearray([fill])*4096
     def put(off,v):struct.pack_into('<I',a,off,v&0xffffffff)
     put(0,base+0x200 if old else 0);put(4,at:=((base+0x200+old*4) if old else 0));put(8,at)
     for j in range(8):put(0x200+j*4,0x100+j)
     put(0x600,0x12345678);f.write(struct.pack('<6I',mode,kind,pos,count,end,value));f.write(a);cases+=1
for kind in ('retail','compiled'):
 r=subprocess.run([str(w/'fixture.exe'),str(w/'input.bin'),str(w/(kind+'.bin')),kind],env=env,capture_output=True,text=True,timeout=60);assert not r.returncode,(w,r.returncode,r.stdout,r.stderr);print(r.stdout.strip(),kind,flush=True)
assert (w/'retail.bin').read_bytes()==(w/'compiled.bin').read_bytes()
size=(w/'compiled.bin').stat().st_size;assert size%4104==0;snapshots=size//4104
report=dict(cases=cases,snapshots=snapshots,omission_checks=1,providers=proof,source_sha256=hashlib.sha256(source.read_bytes()).hexdigest(),fixture_sha256=hashlib.sha256(fixture.read_bytes()).hexdigest(),boundary='One216byte original span/four call fixups. Full4KiB snapshots; signed long values, empty/nonempty,all insertion positions,zero/one/multiple fill,end and middle,fill-value aliasing existing storage,callbacks mutate source/value/vector. Only malloc/free boundaries;real memmove. Allocation renames audited.')
(ROOT/'rebuild/compile-gate/native-long-vector.json').write_text(json.dumps(report,indent=2)+'\n');print('NATIVE_LONG_VECTOR_GATE_PASS',snapshots,'snapshots')
