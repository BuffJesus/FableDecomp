"""Native world-map opening, ownership and callback traversal comparisons."""
import hashlib,itertools,json,struct,subprocess,tempfile
from pathlib import Path
from capstone import Cs,CS_ARCH_X86,CS_MODE_32
import check_one as V
from variable_allocator_parity import check
from pe_oracle import EXE,read_virtual_data
ROOT=Path(__file__).resolve().parents[2];w=Path(tempfile.mkdtemp(prefix='native-bank-entry-pointer-'));env=V.env()
source=ROOT/'rebuild/src/compiled/00/9d/CBankFileAsyncEntry_CountedLifetime_009d69a0.cpp';fixture=ROOT/'rebuild/tests/integration/NativeBankEntryPointer_test.cpp'
copy='??0?$CCountedPointer@VCBankFileAsyncEntry@@@@QAE@ABV0@@Z';destroy='??1?$CCountedPointer@VCBankFileAsyncEntry@@@@QAE@XZ';assign='??4?$CCountedPointer@VCBankFileAsyncEntry@@@@QAEAAV0@ABV0@@Z'
targets={'??3@YAXPAX@Z':0xbfe9bc}
aliases={'@Copy@12':copy,'@Assign@12':assign,'@Destroy@4':destroy,'_Delete':'??3@YAXPAX@Z'}
(w/'bank_entry_aliases.h').write_text('\n'.join('#pragma comment(linker,"/alternatename:'+a+'='+b+'")' for a,b in aliases.items())+'\n')
body=bytearray(200);md=Cs(CS_ARCH_X86,CS_MODE_32);md.detail=True;fixups=[]
for a,n,off in [(0x9d69a0,25,0),(0x9c9fc0,53,64),(0xa5f6d0,72,128)]:
 span=read_virtual_data(EXE.read_bytes(),a,n);body[off:off+n]=span
 for i in md.disasm(span,a):
  if i.mnemonic=='call' and i.operands[0].type==2:
   assert i.operands[0].imm==0xbfe9bc;fixups.append(off+i.address-a+1)
assert len(fixups)==2
(w/'bank_entry_retail.h').write_text('static const unsigned char retailCode[]={'+','.join(map(str,body))+'};\nstatic const unsigned retailFixups[]={'+','.join(map(str,fixups))+'};\n')
objects=[]
for n,p in enumerate((source,fixture)):
 o=w/(str(n)+'.obj');r=V.cl(['/nologo','/c','/O2','/MT','/I'+str(w),'/Fo'+str(o),str(p)],env,w);assert not r.returncode,r.stdout+r.stderr;objects.append(o)
proof=[check(objects[0],s,a,n,r,targets) for s,a,n,r in [(copy,0x9d69a0,25,0),(destroy,0x9c9fc0,53,1),(assign,0xa5f6d0,72,1)]]
cmd=[str(V.VC/'bin/link.exe'),'/nologo','/out:'+str(w/'fixture.exe'),'kernel32.lib']
r=subprocess.run(cmd+[str(objects[1])],env=env,capture_output=True,text=True);assert r.returncode
r=subprocess.run(cmd+list(map(str,objects)),env=env,capture_output=True,text=True);assert not r.returncode,r.stdout+r.stderr
cases=0;base=0x24000000
with (w/'input.bin').open('wb') as f:
 for fill,mode,old,new,count1,count2,op,rhs in itertools.product((0,0x95),range(5),(0,0x300),(0,0x300,0x320),(0,1,2,0xffffffff),(0,1,2,0xffffffff),range(3),(0,0x100)):
  a=bytearray([fill])*2048
  def put(off,v):struct.pack_into('<I',a,off,v&0xffffffff)
  put(0,base+0x600);put(4,base+old if old else 0);put(0x100,base+0x500);put(0x104,base+new if new else 0);put(0x300,count1);put(0x320,count2);put(0x340,2)
  f.write(struct.pack('<3I',mode,op,rhs));f.write(a);cases+=1
for kind in ('retail','compiled'):
 r=subprocess.run([str(w/'fixture.exe'),str(w/'input.bin'),str(w/(kind+'.bin')),kind],env=env,capture_output=True,text=True,timeout=60);assert not r.returncode,(w,r.returncode,r.stdout,r.stderr);print(r.stdout.strip(),kind,flush=True)
with (w/'retail.bin').open('rb') as e,(w/'compiled.bin').open('rb') as f:
 while True:
  x=e.read(1<<20);y=f.read(1<<20);assert x==y,(w,'native mismatch')
  if not x:break
size=(w/'compiled.bin').stat().st_size;assert size%2056==0;snapshots=size//2056
report=dict(cases=cases,snapshots=snapshots,omission_checks=1,provider=proof,source_sha256=hashlib.sha256(source.read_bytes()).hexdigest(),fixture_sha256=hashlib.sha256(fixture.read_bytes()).hexdigest(),boundary='Three original spans/two audited delete fixups. Full2KiB snapshots;null/same/distinct owners,zero/one/two/overflow reference counts,release callbacks replace old Info or change incoming count. Only ownership DeleteFunc and global delete are boundaries.')
(ROOT/'rebuild/compile-gate/native-bank-entry-pointer.json').write_text(json.dumps(report,indent=2)+'\n');print('NATIVE_BANK_ENTRY_POINTER_GATE_PASS',snapshots,'snapshots')
