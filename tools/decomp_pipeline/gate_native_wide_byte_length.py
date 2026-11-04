"""Original-byte and independent result checks for CWideString's byte count."""
import hashlib,itertools,json,struct,subprocess,tempfile
from pathlib import Path
import check_one as V
from variable_allocator_parity import check
from pe_oracle import EXE,read_virtual_data
ROOT=Path(__file__).resolve().parents[2];w=Path(tempfile.mkdtemp(prefix='native-wide-byte-length-'));env=V.env()
source=ROOT/'rebuild/src/compiled/00/99/CWideString_GetByteLength_0099b240.cpp';fixture=ROOT/'rebuild/tests/integration/NativeWideByteLength_test.cpp'
symbol='?GetByteLength@CWideString@@QBEJXZ'
(w/'wide_length_aliases.h').write_text('#pragma comment(linker,"/alternatename:@ByteLength@4='+symbol+'")\n')
body=read_virtual_data(EXE.read_bytes(),0x99b240,21)
(w/'wide_length_retail.h').write_text('static const unsigned char retailCode[]={'+','.join(map(str,body))+'};\n')
objects=[]
for n,p in enumerate((source,fixture)):
 o=w/(str(n)+'.obj');r=V.cl(['/nologo','/c','/O2','/MT','/I'+str(w),'/Fo'+str(o),str(p)],env,w);assert not r.returncode,r.stdout+r.stderr;objects.append(o)
proof=check(objects[0],symbol,0x99b240,21,0,{})
cmd=[str(V.VC/'bin/link.exe'),'/nologo','/out:'+str(w/'fixture.exe'),'kernel32.lib']
r=subprocess.run(cmd+[str(objects[1])],env=env,capture_output=True,text=True);assert r.returncode
r=subprocess.run(cmd+list(map(str,objects)),env=env,capture_output=True,text=True);assert not r.returncode,r.stdout+r.stderr
cases=0;base=0x24000000
with (w/'input.bin').open('wb') as f:
 for fill,receiver,storage,start,count,null in itertools.product((0,0x95),(0,32),(64,96),(0,base+512,0xfffffff0),(0,1,3,100,0x3fffffff,0x40000000),(0,1)):
  a=bytearray([fill])*1024;struct.pack_into('<I',a,receiver,0 if null else base+storage);struct.pack_into('<2I',a,storage,start,(start+count*2)&0xffffffff)
  f.write(struct.pack('<2I',receiver,0 if null else (count*2)&0xffffffff));f.write(a);cases+=1
for kind in ('retail','compiled'):
 r=subprocess.run([str(w/'fixture.exe'),str(w/'input.bin'),str(w/(kind+'.bin')),kind],env=env,capture_output=True,text=True,timeout=60);assert not r.returncode,(w,r.returncode,r.stdout,r.stderr);print(r.stdout.strip(),kind,flush=True)
assert (w/'retail.bin').read_bytes()==(w/'compiled.bin').read_bytes()
report=dict(cases=cases,snapshots=cases,omission_checks=1,provider=proof,source_sha256=hashlib.sha256(source.read_bytes()).hexdigest(),fixture_sha256=hashlib.sha256(fixture.read_bytes()).hexdigest(),boundary='Actual21byte body/no fixups or callbacks. Null/non-null storage, different receiver/data locations, zero/short/large byte counts, pointer wrap. Original and compiled results also checked against independent arithmetic; full1KiB memory must remain unchanged. Synthetic extreme pointers are never dereferenced.')
(ROOT/'rebuild/compile-gate/native-wide-byte-length.json').write_text(json.dumps(report,indent=2)+'\n');print('NATIVE_WIDE_BYTE_LENGTH_GATE_PASS',cases,'snapshots')
