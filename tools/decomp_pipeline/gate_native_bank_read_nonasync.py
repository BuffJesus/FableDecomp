"""Typed bank-entry forwarding with original code and independent callback argument checks."""
import hashlib,itertools,json,struct,subprocess,tempfile
from pathlib import Path
import check_one as V
from variable_allocator_parity import check
from pe_oracle import EXE,read_virtual_data
ROOT=Path(__file__).resolve().parents[2];w=Path(tempfile.mkdtemp(prefix='native-bank-read-nonasync-'));env=V.env()
source=ROOT/'rebuild/src/compiled/00/9d/CBankFileAsyncEntry_ReadDataNonAsync_009d52b0.cpp';fixture=ROOT/'rebuild/tests/integration/NativeBankReadNonAsync_test.cpp'
symbol='?ReadDataNonAsync@CBankFileAsyncEntry@@QAEXKPAXK@Z';target='?ReadEntryData@CBankFile@@QAEXKKPAXK@Z'
aliases={'@ReadNonAsync@20':symbol,target:'@BankRead@24'}
(w/'bank_nonasync_aliases.h').write_text('\n'.join('#pragma comment(linker,"/alternatename:'+a+'='+b+'")' for a,b in aliases.items())+'\n')
body=read_virtual_data(EXE.read_bytes(),0x9d52b0,30);assert body[22]==0xe8 and struct.unpack_from('<i',body,23)[0]+0x9d52b0+27==0x9cc2a0
(w/'bank_nonasync_retail.h').write_text('static const unsigned char retailCode[]={'+','.join(map(str,body))+'};\nstatic const unsigned retailCallOffset=23;\n')
objects=[]
for n,p in enumerate((source,fixture)):
 o=w/(str(n)+'.obj');r=V.cl(['/nologo','/c','/O2','/MT','/I'+str(w),'/Fo'+str(o),str(p)],env,w);assert not r.returncode,r.stdout+r.stderr;objects.append(o)
proof=check(objects[0],symbol,0x9d52b0,30,1,{target:0x9cc2a0})
cmd=[str(V.VC/'bin/link.exe'),'/nologo','/out:'+str(w/'fixture.exe'),'kernel32.lib']
r=subprocess.run(cmd+[str(objects[1])],env=env,capture_output=True,text=True);assert r.returncode
r=subprocess.run(cmd+list(map(str,objects)),env=env,capture_output=True,text=True);assert not r.returncode,r.stdout+r.stderr
cases=0;base=0x24000000
with (w/'input.bin').open('wb') as f:
 for fill,mode,receiver,owner,index,offset,size in itertools.product((0,0x95),range(3),(0,0x100),(0x200,0x300),(0,1,0xffffffff),(0,7,0xffffffff),(0,16,0xffffffff)):
  a=bytearray([fill])*2048;struct.pack_into('<2I',a,receiver+4,base+owner,index)
  f.write(struct.pack('<4I',mode,receiver,offset,size));f.write(a);cases+=1
for kind in ('retail','compiled'):
 r=subprocess.run([str(w/'fixture.exe'),str(w/'input.bin'),str(w/(kind+'.bin')),kind],env=env,capture_output=True,text=True,timeout=60);assert not r.returncode,(w,r.returncode,r.stdout,r.stderr);print(r.stdout.strip(),kind,flush=True)
assert (w/'retail.bin').read_bytes()==(w/'compiled.bin').read_bytes()
report=dict(cases=cases,snapshots=cases*2,omission_checks=1,provider=proof,source_sha256=hashlib.sha256(source.read_bytes()).hexdigest(),fixture_sha256=hashlib.sha256(fixture.read_bytes()).hexdigest(),boundary='Actual30byte wrapper/one audited fixup. Two calls per case; independent expected bank receiver,index,offset,destination,size checks and full2KiB snapshots. Zero/extreme unsigned parameters and callback mutations to entry owner/index/other fields. Bank ReadEntryData remains callback boundary; replaces mislabeled editor shim.')
(ROOT/'rebuild/compile-gate/native-bank-read-nonasync.json').write_text(json.dumps(report,indent=2)+'\n');print('NATIVE_BANK_READ_NONASYNC_GATE_PASS',cases*2,'snapshots')
