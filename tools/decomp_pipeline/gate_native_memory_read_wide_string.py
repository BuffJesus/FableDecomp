"""Native ABI and callback comparison for the existing memory-stream string reader."""
import hashlib,itertools,json,struct,subprocess,tempfile
from pathlib import Path
from capstone import Cs,CS_ARCH_X86,CS_MODE_32
import check_one as V
from variable_allocator_parity import check
from pe_oracle import EXE,read_virtual_data
ROOT=Path(__file__).resolve().parents[2];w=Path(tempfile.mkdtemp(prefix='native-memory-read-wide-string-'));env=V.env()
source=ROOT/'rebuild/src/compiled/00/99/CMemoryDataInputStream_ReadNullTerminatedWString_00994960.cpp';fixture=ROOT/'rebuild/tests/integration/NativeMemoryReadWideString_test.cpp'
symbol='?ReadNullTerminatedWString@CMemoryDataInputStream@@UAE?AVCWideString@@XZ'
targets={'?GetByteLength@CWideString@@QBEJXZ':0x99b240,'??0CWideString@@QAE@PBG@Z':0x99b6b0,'??0CWideString@@QAE@ABV0@@Z':0x99b720,'??1CWideString@@QAE@XZ':0x99b510}
aliases={'?GetByteLength@CWideString@@QBEJXZ':'@ByteLength@4','@ReadString@12':symbol,'??0CWideString@@QAE@PBG@Z':'@Construct@12','??0CWideString@@QAE@ABV0@@Z':'@Copy@12','??1CWideString@@QAE@XZ':'@Destroy@4'}
(w/'memory_string_aliases.h').write_text('\n'.join('#pragma comment(linker,"/alternatename:'+a+'='+b+'")' for a,b in aliases.items())+'\n')
body=read_virtual_data(EXE.read_bytes(),0x994960,136);md=Cs(CS_ARCH_X86,CS_MODE_32);md.detail=True;fixups=[]
for i in md.disasm(body,0x994960):
 if i.mnemonic=='call' and i.operands[0].type==2:
  assert i.operands[0].imm in targets.values();fixups.append((i.address-0x994960+1,i.operands[0].imm))
assert len(fixups)==4
(w/'memory_string_retail.h').write_text('static const unsigned char retailCode[]={'+','.join(map(str,body))+'};\nstatic const unsigned retailFixups[][2]={'+','.join('{%d,%d}'%f for f in fixups)+'};\n')
objects=[]
for n,p in enumerate((source,fixture)):
 o=w/(str(n)+'.obj');r=V.cl(['/nologo','/c','/O2','/MT','/I'+str(w),'/Fo'+str(o),str(p)],env,w);assert not r.returncode,r.stdout+r.stderr;objects.append(o)
proof=check(objects[0],symbol,0x994960,136,4,targets)
cmd=[str(V.VC/'bin/link.exe'),'/nologo','/out:'+str(w/'fixture.exe'),'kernel32.lib']
r=subprocess.run(cmd+[str(objects[1])],env=env,capture_output=True,text=True);assert r.returncode
r=subprocess.run(cmd+list(map(str,objects)),env=env,capture_output=True,text=True);assert not r.returncode,r.stdout+r.stderr
cases=0;base=0x24000000
with (w/'input.bin').open('wb') as f:
 for fill,mode,first,second,length,null,old in itertools.product((0,0x95),range(6),(0,4,0xffffffff),(0,3,0x7fffffff,0xfffffffe),(0,3,0xffffffff,0x7fffffff),(0,1),(0,2)):
  a=bytearray([fill])*2048
  struct.pack_into('<5I',a,4,old,64,base+0x500,123,24);a[0x400:0x405]=b'name\0';a[0x420:0x425]=b'next\0'
  f.write(struct.pack('<5I',mode,first,second,length,null));f.write(a);cases+=1
for kind in ('retail','compiled'):
 r=subprocess.run([str(w/'fixture.exe'),str(w/'input.bin'),str(w/(kind+'.bin')),kind],env=env,capture_output=True,text=True,timeout=60);assert not r.returncode,(w,r.returncode,r.stdout,r.stderr);print(r.stdout.strip(),kind,flush=True)
with (w/'retail.bin').open('rb') as e,(w/'compiled.bin').open('rb') as f:
 while True:
  x=e.read(1<<20);y=f.read(1<<20);assert x==y,(w,'native mismatch')
  if not x:break
size=(w/'compiled.bin').stat().st_size;assert size%2060==0;snapshots=size//2060
report=dict(cases=cases,snapshots=snapshots,omission_checks=1,provider=proof,source_sha256=hashlib.sha256(source.read_bytes()).hexdigest(),fixture_sha256=hashlib.sha256(fixture.read_bytes()).hexdigest(),boundary='136byte wide reader/four audited fixups. Full2KiB snapshots,hidden return pointer,null/empty/nonempty string data,length signed extremes,distinct virtual position replies,wrapped/guard-rejected advancement,callbacks mutate source/position/current pointer and final fields. String ctor/byte length/copy/dtor and virtual GetPosition remain boundaries. Wide-string constructor and byte length callbacks supply controlled values.')
(ROOT/'rebuild/compile-gate/native-memory-read-wide-string.json').write_text(json.dumps(report,indent=2)+'\n');print('NATIVE_MEMORY_READ_WIDE_STRING_GATE_PASS',snapshots,'snapshots')
