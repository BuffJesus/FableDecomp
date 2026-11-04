"""Native world-map opening, ownership and callback traversal comparisons."""
import hashlib,itertools,json,struct,subprocess,tempfile
from pathlib import Path
from capstone import Cs,CS_ARCH_X86,CS_MODE_32
import check_one as V
from variable_allocator_parity import check
from pe_oracle import EXE,read_virtual_data
ROOT=Path(__file__).resolve().parents[2];w=Path(tempfile.mkdtemp(prefix='native-bank-pool-'));env=V.env()
source=ROOT/'rebuild/src/compiled/00/b3/CBankFileAsync_SetLoadingMemoryPool_00b3e820.cpp';fixture=ROOT/'rebuild/tests/integration/NativeBankPool_test.cpp'
symbol='?SetLoadingMemoryPool@CBankFileAsync@@QAEXV?$CCountedPointer@VCMovableResourceMemoryPool@@@@@Z'
targets={'??3@YAXPAX@Z':0xbfe9bc}
aliases={'@SetPool@16':symbol,'_Delete':'??3@YAXPAX@Z'}
(w/'bank_pool_aliases.h').write_text('\n'.join('#pragma comment(linker,"/alternatename:'+a+'='+b+'")' for a,b in aliases.items())+'\n')
body=read_virtual_data(EXE.read_bytes(),0xb3e820,106);md=Cs(CS_ARCH_X86,CS_MODE_32);md.detail=True
fixups=[]
for i in md.disasm(body,0xb3e820):
 if i.mnemonic=='call' and i.operands[0].type==2:
  assert i.operands[0].imm==0xbfe9bc;fixups.append(i.address-0xb3e820+1)
assert len(fixups)==2
(w/'bank_pool_retail.h').write_text('static const unsigned char retailCode[]={'+','.join(map(str,body))+'};\nstatic const unsigned retailFixups[]={'+','.join(map(str,fixups))+'};\n')
objects=[]
for n,p in enumerate((source,fixture)):
 o=w/(str(n)+'.obj');r=V.cl(['/nologo','/c','/O2','/MT','/I'+str(w),'/Fo'+str(o),str(p)],env,w);assert not r.returncode,r.stdout+r.stderr;objects.append(o)
proof=check(objects[0],symbol,0xb3e820,106,2,targets)
cmd=[str(V.VC/'bin/link.exe'),'/nologo','/out:'+str(w/'fixture.exe'),'kernel32.lib']
r=subprocess.run(cmd+[str(objects[1])],env=env,capture_output=True,text=True);assert r.returncode
r=subprocess.run(cmd+list(map(str,objects)),env=env,capture_output=True,text=True);assert not r.returncode,r.stdout+r.stderr
cases=0;base=0x24000000
with (w/'input.bin').open('wb') as f:
 for fill,mode,old,new,count1,count2,data in itertools.product((0,0x95),range(5),(0,0x300),(0,0x300,0x320),(0,1,2,0xffffffff),(0,1,2,0xffffffff),(0,base+0x500)):
  a=bytearray([fill])*2048
  def put(off,v):struct.pack_into('<I',a,off,v&0xffffffff)
  put(0x118,base+0x600);put(0x11c,base+old if old else 0);put(0x300,count1);put(0x320,count2);put(0x340,2)
  f.write(struct.pack('<3I',mode,data,base+new if new else 0));f.write(a);cases+=1
for kind in ('retail','compiled'):
 r=subprocess.run([str(w/'fixture.exe'),str(w/'input.bin'),str(w/(kind+'.bin')),kind],env=env,capture_output=True,text=True,timeout=60);assert not r.returncode,(w,r.returncode,r.stdout,r.stderr);print(r.stdout.strip(),kind,flush=True)
with (w/'retail.bin').open('rb') as e,(w/'compiled.bin').open('rb') as f:
 while True:
  x=e.read(1<<20);y=f.read(1<<20);assert x==y,(w,'native mismatch')
  if not x:break
size=(w/'compiled.bin').stat().st_size;assert size%2056==0;snapshots=size//2056
report=dict(cases=cases,snapshots=snapshots,omission_checks=1,provider=proof,source_sha256=hashlib.sha256(source.read_bytes()).hexdigest(),fixture_sha256=hashlib.sha256(fixture.read_bytes()).hexdigest(),boundary='Original106byte body/two audited delete fixups. Full2KiB snapshots;null/same/distinct owners,zero/one/two/overflow reference counts,release callbacks replace old Info or change incoming count. Only ownership DeleteFunc and global delete are boundaries.')
(ROOT/'rebuild/compile-gate/native-bank-pool.json').write_text(json.dumps(report,indent=2)+'\n');print('NATIVE_BANK_POOL_GATE_PASS',snapshots,'snapshots')
