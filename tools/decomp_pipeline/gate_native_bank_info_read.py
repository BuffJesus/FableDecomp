"""Bank metadata reads, including the retail wrapped-position guard."""
import hashlib,itertools,json,struct,subprocess,tempfile
from pathlib import Path
import check_one as V
from variable_allocator_parity import check
from pe_oracle import EXE,read_virtual_data
ROOT=Path(__file__).resolve().parents[2];w=Path(tempfile.mkdtemp(prefix='native-bank-info-read-'));env=V.env()
source=ROOT/'rebuild/src/compiled/00/9c/CBankFileEntryUpdateData_ReadInfo_009cbe90.cpp';fixture=ROOT/'rebuild/tests/integration/NativeBankInfoRead_test.cpp';symbol='?ReadInfo@CBankFileEntryUpdateData@@QBEXKPAXK@Z'
(w/'bank_info_read_aliases.h').write_text('#pragma comment(linker,"/alternatename:@Read@20='+symbol+'")\n')
body=read_virtual_data(EXE.read_bytes(),0x9cbe90,67)
(w/'bank_info_read_retail.h').write_text('static const unsigned char retailCode[]={'+','.join(map(str,body))+'};\n')
objects=[]
for i,p in enumerate((source,fixture)):
 o=w/(str(i)+'.obj');r=V.cl(['/nologo','/c','/O2','/MT','/I'+str(w),'/Fo'+str(o),str(p)],env,w);assert not r.returncode,r.stdout+r.stderr;objects.append(o)
proof=check(objects[0],symbol,0x9cbe90,67,0,{})
cmd=[str(V.VC/'bin/link.exe'),'/nologo','/out:'+str(w/'fixture.exe'),'kernel32.lib']
r=subprocess.run(cmd+[str(objects[1])],env=env,capture_output=True,text=True);assert r.returncode
r=subprocess.run(cmd+list(map(str,objects)),env=env,capture_output=True,text=True);assert not r.returncode,r.stdout+r.stderr
expected=bytearray();cases=0
with (w/'input.bin').open('wb') as f:
 for fill,offset,size,alignment in itertools.product((0,0x95),(0,1,15,0x7ffffff0,0x7fffffff,0x80000000,0xfffffff0,0xffffffff),(0,1,3,4,15,16,31,64,127),range(4)):
  a=bytearray([fill])*2048;a[0x200:0x300]=bytes(range(256));struct.pack_into('<I',a,8,(0x24000200+alignment-offset)&0xffffffff)
  f.write(struct.pack('<2I',offset,size));f.write(a)
  if (offset+size)&0xffffffff<=0x7fffffff:a[0x400:0x400+size]=a[0x200+alignment:0x200+alignment+size]
  expected+=a;cases+=1
for kind in ('retail','compiled'):
 r=subprocess.run([str(w/'fixture.exe'),str(w/'input.bin'),str(w/(kind+'.bin')),kind],env=env,capture_output=True,text=True,timeout=60);assert not r.returncode,(w,r.returncode,r.stdout,r.stderr)
 assert (w/(kind+'.bin')).read_bytes()==expected,(w,kind,'independent expected mismatch');print(r.stdout.strip(),kind,flush=True)
report=dict(cases=cases,snapshots=cases,omission_checks=1,provider=proof,source_sha256=hashlib.sha256(source.read_bytes()).hexdigest(),fixture_sha256=hashlib.sha256(fixture.read_bytes()).hexdigest(),boundary='No call boundaries or native fixups. Original67bytes versus compiled and independent full2KiB expected memory. Unaligned reads,zero lengths,INT_MAX crossing and UINT_MAX wrap with effective source kept in arena;retail truncates the addition before signed-range validation.')
(ROOT/'rebuild/compile-gate/native-bank-info-read.json').write_text(json.dumps(report,indent=2)+'\n');print('NATIVE_BANK_INFO_READ_GATE_PASS',cases,'snapshots')
