"""Typed static async-data discard dispatch and callback-mutated smart pointer."""
import hashlib,itertools,json,struct,subprocess,tempfile
from pathlib import Path
import check_one as V
from variable_allocator_parity import check
from pe_oracle import EXE,read_virtual_data
ROOT=Path(__file__).resolve().parents[2];w=Path(tempfile.mkdtemp(prefix='native-async-data-discard-'));env=V.env()
source=ROOT/'rebuild/src/compiled/00/9d/CBankFileAsyncData_DiscardAsyncData_009d68a0.cpp';fixture=ROOT/'rebuild/tests/integration/NativeAsyncDataDiscard_test.cpp'
symbol='?DiscardAsyncData@CBankFileAsyncData@@SIXAAV?$CCountedPointer@VCBankFileAsyncData@@@@@Z';owner='?DiscardAsyncData@CBankFileAsync@@QAEXAAV?$CCountedPointer@VCBankFileAsyncData@@@@@Z'
aliases={'@Entry@4':symbol,owner:'@Discard@12'}
(w/'async_data_discard_aliases.h').write_text('\n'.join('#pragma comment(linker,"/alternatename:'+a+'='+b+'")' for a,b in aliases.items())+'\n')
body=read_virtual_data(EXE.read_bytes(),0x9d68a0,16);assert body[10]==0xe8 and 0x9d68af+struct.unpack_from('<i',body,11)[0]==0x9d67e0
(w/'async_data_discard_retail.h').write_text('static const unsigned char retailCode[]={'+','.join(str(b) for b in body)+'};\n')
objects=[]
for i,p in enumerate([source,fixture]):
 obj=w/(str(i)+'.obj');r=V.cl(['/nologo','/c','/O2','/MT','/I'+str(w),'/Fo'+str(obj),str(p)],env,w);assert not r.returncode,r.stdout+r.stderr;objects.append(obj)
proof=check(objects[0],symbol,0x9d68a0,16,1,{owner:0x9d67e0})
exe=w/'fixture.exe';cmd=[str(V.VC/'bin/link.exe'),'/nologo','/out:'+str(exe),'kernel32.lib']
r=subprocess.run(cmd+[str(objects[1])],env=env,capture_output=True,text=True);assert r.returncode
r=subprocess.run(cmd+list(map(str,objects)),env=env,capture_output=True,text=True);assert not r.returncode,r.stdout+r.stderr
inputs=bytearray();cases=0;base=0x24000000
for fill,mode,slot,present,owner_kind in itertools.product(range(256),range(2),(0x100,0x200),range(2),range(3)):
 arena=bytearray([fill])*4096
 struct.pack_into('<I',arena,slot,base+0x400 if present else 0)
 struct.pack_into('<I',arena,0x428,base+(0x600,slot,0x400)[owner_kind]);struct.pack_into('<I',arena,0x828,base+0xa00)
 inputs.extend(struct.pack('<2I',mode,slot));inputs.extend(arena);cases+=1
(w/'input.bin').write_bytes(inputs)
for kind in ('retail','compiled'):
 r=subprocess.run([str(exe),str(w/'input.bin'),str(w/(kind+'.bin')),kind],env=env,capture_output=True,text=True,timeout=60);assert not r.returncode,(w,r.returncode,r.stdout,r.stderr);print(r.stdout.strip(),kind,flush=True)
actual=(w/'compiled.bin').read_bytes();expected=(w/'retail.bin').read_bytes();assert actual==expected,(w,'async discard state');assert len(actual)%(4096+12)==0
snapshots=len(actual)//(4096+12);header=ROOT/'rebuild/include/fable_game_text_bank.h'
report=dict(cases=cases,snapshots=snapshots,omission_checks=1,providers=[proof],source_sha256={str(p.relative_to(ROOT)):hashlib.sha256(p.read_bytes()).hexdigest() for p in [source,header]},fixture_sha256=hashlib.sha256(fixture.read_bytes()).hexdigest(),boundary='Original16byte static fastcall body natively executed with one audited bank-owner call relocation. Two-call sequences,all256fills,two pointer slots,null/data branches,owner aliases pointer slot or payload,callback clears/replaces data and writes arbitrary Info. Full4KiB at each bank callback and return. Actual CBankFileAsync owner remains recorded boundary;no normalized bytes.')
(ROOT/'rebuild/compile-gate/native-async-data-discard.json').write_text(json.dumps(report,indent=2)+'\n');print('NATIVE_ASYNC_DATA_DISCARD_GATE_PASS',snapshots,'snapshots')
