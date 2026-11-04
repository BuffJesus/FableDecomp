import hashlib,json,subprocess,tempfile,sys,struct
from pathlib import Path
sys.path.insert(0,str(Path(__file__).resolve().parent))
import check_one as V
from variable_allocator_parity import check
from gate_game_component_abi import Coff
from pe_oracle import EXE,read_virtual_data
ROOT=Path(__file__).resolve().parents[2];w=Path(tempfile.mkdtemp(prefix='native-subprimitive-insertion-'));env=V.env()
specs=[('SubPrimitiveSort_InsertionSort_00b85b30.cpp','??$__insertion_sort@',0xb85b30,54,0),('SubPrimitiveSort_UnguardedInsertionSort_00b851d0.cpp','??$__unguarded_insertion_sort_aux@',0xb851d0,73,64),('SubPrimitiveSort_LinearInsert_00b85870.cpp','??$__linear_insert@',0xb85870,89,144),('SubPrimitiveSort_UnguardedLinearInsert_00b84f40.cpp','??$__unguarded_linear_insert@',0xb84f40,51,240)]
sources=[ROOT/'rebuild/src/compiled/00/b8'/s[0] for s in specs];fixture=ROOT/'rebuild/tests/integration/NativeSubPrimitiveInsertion_test.cpp'
raw=EXE.read_bytes();body=bytearray(291);fixups=[]
import capstone
for name,prefix,a,l,offset in specs:
 b=read_virtual_data(raw,a,l);body[offset:offset+l]=b
 for ins in capstone.Cs(capstone.CS_ARCH_X86,capstone.CS_MODE_32).disasm(b,a):
  if ins.mnemonic=='call' and ins.bytes[0]==0xe8:
   target=ins.address+5+struct.unpack_from('<i',ins.bytes,1)[0];destination=next(s[4] for s in specs if s[2]==target);fixups.append((offset+ins.address-a+1,destination))
assert len(fixups)==2
(w/'subprimitive_insertion_retail.h').write_text('static const unsigned char retailCode[]={'+','.join(map(str,body))+'};\nstatic const unsigned retailFixups[][2]={'+','.join('{%d,%d}'%f for f in fixups)+'};\n')
objects=[];symbols=[]
for n,p in enumerate(sources+[fixture]):
 o=w/(str(n)+'.obj');r=V.cl(['/nologo','/c','/O2','/MT','/I'+str(w),'/Fo'+str(o),str(p)],env,w);assert not r.returncode,r.stdout+r.stderr;objects.append(o)
 if n<4:symbols.append(next(s for s in Coff(o).symbols if s.startswith(specs[n][1])))
targets={symbols[n]:s[2] for n,s in enumerate(specs)}
proofs=[check(objects[n],symbols[n],a,l,1 if n in (0,2) else 0,targets) for n,(_,_,a,l,_) in enumerate(specs)]
assert all(p['grade']=='MATCH' for p in proofs)
link=[str(V.VC/'bin/link.exe'),'/nologo','/out:'+str(w/'fixture.exe'),'kernel32.lib']
for omit in range(4):
 r=subprocess.run(link+[str(o) for n,o in enumerate(objects) if n!=omit],env=env,capture_output=True,text=True);assert r.returncode
r=subprocess.run(link+list(map(str,objects)),env=env,capture_output=True,text=True);assert not r.returncode,r.stdout+r.stderr
r=subprocess.run([str(w/'fixture.exe')],env=env,capture_output=True,text=True,timeout=60);assert not r.returncode,(w,r.returncode,r.stdout,r.stderr);print(r.stdout.strip());assert 'cases=672 snapshots=1344' in r.stdout
report=dict(cases=672,snapshots=1344,providers=proofs,omission_checks=4,source_sha256={str(p.relative_to(ROOT)):hashlib.sha256(p.read_bytes()).hexdigest() for p in sources},fixture_sha256=hashlib.sha256(fixture.read_bytes()).hexdigest(),result=r.stdout.strip(),boundary='Four actual insertion routines;independent stable full32-bit insertion model and original native code,672cases/1344full64KiB snapshots,counts0..257,8patterns,two locations,sorted sentinel prefixes and isolated value insertion. Entire production chain with no callback helpers. No drawing.')
(ROOT/'rebuild/compile-gate/native-subprimitive-insertion.json').write_text(json.dumps(report,indent=2)+'\n');print('NATIVE_SUBPRIMITIVE_INSERTION_GATE_PASS')
