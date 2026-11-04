import hashlib,json,subprocess,tempfile,sys,struct
from pathlib import Path
sys.path.insert(0,str(Path(__file__).resolve().parent))
import check_one as V
from variable_allocator_parity import check
from gate_game_component_abi import Coff
from pe_oracle import EXE,read_virtual_data
ROOT=Path(__file__).resolve().parents[2];w=Path(tempfile.mkdtemp(prefix='native-subprimitive-inplace-stable-'));env=V.env()
specs=[
('SubPrimitiveSort_InplaceStableSort_00b86570.cpp','??$__inplace_stable_sort@',0xb86570,99,0),
('SubPrimitiveSort_MergeWithoutBuffer_00b86140.cpp','??$__merge_without_buffer@',0xb86140,274,112),
('SubPrimitiveSort_LowerBound_00b85240.cpp','??$__lower_bound@',0xb85240,54,400),
('SubPrimitiveSort_UpperBound_00b85280.cpp','??$__upper_bound@',0xb85280,54,464),
('SubPrimitiveSort_Rotate_00b85660.cpp','??$__rotate@',0xb85660,356,528),
('SubPrimitiveSort_SwapRanges_00b85390.cpp','??$swap_ranges@',0xb85390,52,896),
('SubPrimitiveSort_InsertionSort_00b85b30.cpp','??$__insertion_sort@',0xb85b30,54,960),
('SubPrimitiveSort_LinearInsert_00b85870.cpp','??$__linear_insert@',0xb85870,89,1104),
('SubPrimitiveSort_UnguardedLinearInsert_00b84f40.cpp','??$__unguarded_linear_insert@',0xb84f40,51,1200)]

sources=[ROOT/'rebuild/src/compiled/00/b8'/s[0] for s in specs];fixture=ROOT/'rebuild/tests/integration/NativeSubPrimitiveInplaceStable_test.cpp'
raw=EXE.read_bytes();body=bytearray(1251);fixups=[]
import capstone
for name,prefix,a,l,offset in specs:
 b=read_virtual_data(raw,a,l);body[offset:offset+l]=b
 for ins in capstone.Cs(capstone.CS_ARCH_X86,capstone.CS_MODE_32).disasm(b,a):
  if ins.mnemonic=='call' and ins.bytes[0]==0xe8:
   target=ins.address+5+struct.unpack_from('<i',ins.bytes,1)[0];destination=next(s[4] for s in specs if s[2]==target);fixups.append((offset+ins.address-a+1,destination))
assert len(fixups)==11
(w/'subprimitive_inplace_stable_retail.h').write_text('static const unsigned char retailCode[]={'+','.join(map(str,body))+'};\nstatic const unsigned retailFixups[][2]={'+','.join('{%d,%d}'%f for f in fixups)+'};\n')
objects=[];symbols=[]
for n,p in enumerate(sources+[fixture]):
 o=w/(str(n)+'.obj');r=V.cl(['/nologo','/c','/O2','/MT','/I'+str(w),'/Fo'+str(o),str(p)],env,w);assert not r.returncode,r.stdout+r.stderr;objects.append(o)
 if n<9:symbols.append(next(s for s in Coff(o).symbols if s.startswith(specs[n][1])))
targets={symbols[n]:s[2] for n,s in enumerate(specs)}
proofs=[check(objects[n],symbols[n],a,l,len(Coff(objects[n]).sections[Coff(objects[n]).symbols[symbols[n]][0]][1]),targets) for n,(_,_,a,l,_) in enumerate(specs)]
assert all(p['near_match'] for p in proofs)
link=[str(V.VC/'bin/link.exe'),'/nologo','/out:'+str(w/'fixture.exe'),'kernel32.lib']
for omit in range(9):
 r=subprocess.run(link+[str(o) for n,o in enumerate(objects) if n!=omit],env=env,capture_output=True,text=True);assert r.returncode
r=subprocess.run(link+list(map(str,objects)),env=env,capture_output=True,text=True);assert not r.returncode,r.stdout+r.stderr
r=subprocess.run([str(w/'fixture.exe')],env=env,capture_output=True,text=True,timeout=60);assert not r.returncode,(w,r.returncode,r.stdout,r.stderr);print(r.stdout.strip());assert 'cases=3648 snapshots=7296' in r.stdout
report=dict(cases=3648,snapshots=7296,providers=proofs,omission_checks=9,source_sha256={str(p.relative_to(ROOT)):hashlib.sha256(p.read_bytes()).hexdigest() for p in sources},fixture_sha256=hashlib.sha256(fixture.read_bytes()).hexdigest(),result=r.stdout.strip(),boundary='Actual allocation-free stable sorting,recursive merge,binary bounds,cyclic rotation,swapping and insertion chain.3648cases/7296full64KiB snapshots,counts0..1025,8keypatterns,two locations,5range splits,6bound queries,2disjoint swaps. Independent stable fullkey/payload model,linear bounds and direct cyclic permutation. Nine mandatory sources;no helper callbacks. No drawing.')
(ROOT/'rebuild/compile-gate/native-subprimitive-inplace-stable.json').write_text(json.dumps(report,indent=2)+'\n');print('NATIVE_SUBPRIMITIVE_INPLACE_STABLE_GATE_PASS')
