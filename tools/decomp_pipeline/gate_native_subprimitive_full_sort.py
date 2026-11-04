import hashlib,json,subprocess,tempfile,sys,struct
from pathlib import Path
sys.path.insert(0,str(Path(__file__).resolve().parent))
import check_one as V
from variable_allocator_parity import check
from gate_game_component_abi import Coff
from pe_oracle import EXE,read_virtual_data
ROOT=Path(__file__).resolve().parents[2];w=Path(tempfile.mkdtemp(prefix='native-subprimitive-full-sort-'));env=V.env()
specs=[
('SubPrimitiveSort_DefaultSort_00b868c0.cpp','??$sort@',0xb868c0,120,0),
('SubPrimitiveSort_Introsort_00b867a0.cpp','??$__introsort_loop@',0xb867a0,180,128),
('SubPrimitiveSort_Partition_00b85430.cpp','??$__unguarded_partition@',0xb85430,85,320),
('SubPrimitiveSort_PushHeap_00b84fd0.cpp','??$__push_heap@',0xb84fd0,79,416),
('SubPrimitiveSort_AdjustHeap_00b852f0.cpp','??$__adjust_heap@',0xb852f0,98,512),
('SubPrimitiveSort_MakeHeap_00b855f0.cpp','??$__make_heap@',0xb855f0,102,624),
('SubPrimitiveSort_SortHeap_00b85e10.cpp','??$sort_heap@',0xb85e10,92,736),
('SubPrimitiveSort_PartialSort_00b86450.cpp','??$__partial_sort@',0xb86450,99,832),
('SubPrimitiveSort_InsertionSort_00b85b30.cpp','??$__insertion_sort@',0xb85b30,54,944),
('SubPrimitiveSort_UnguardedInsertionSort_00b851d0.cpp','??$__unguarded_insertion_sort_aux@',0xb851d0,73,1008),
('SubPrimitiveSort_LinearInsert_00b85870.cpp','??$__linear_insert@',0xb85870,89,1088),
('SubPrimitiveSort_UnguardedLinearInsert_00b84f40.cpp','??$__unguarded_linear_insert@',0xb84f40,51,1184)]

sources=[ROOT/'rebuild/src/compiled/00/b8'/s[0] for s in specs];fixture=ROOT/'rebuild/tests/integration/NativeSubPrimitiveFullSort_test.cpp'
raw=EXE.read_bytes();body=bytearray(1235);fixups=[]
import capstone
for name,prefix,a,l,offset in specs:
 b=read_virtual_data(raw,a,l);body[offset:offset+l]=b
 for ins in capstone.Cs(capstone.CS_ARCH_X86,capstone.CS_MODE_32).disasm(b,a):
  if ins.mnemonic=='call' and ins.bytes[0]==0xe8:
   target=ins.address+5+struct.unpack_from('<i',ins.bytes,1)[0];destination=next(s[4] for s in specs if s[2]==target);fixups.append((offset+ins.address-a+1,destination))
assert len(fixups)==16
(w/'subprimitive_full_sort_retail.h').write_text('static const unsigned char retailCode[]={'+','.join(map(str,body))+'};\nstatic const unsigned retailFixups[][2]={'+','.join('{%d,%d}'%f for f in fixups)+'};\n')
objects=[];symbols=[]
for n,p in enumerate(sources+[fixture]):
 o=w/(str(n)+'.obj');r=V.cl(['/nologo','/c','/O2','/MT','/I'+str(w),'/Fo'+str(o),str(p)],env,w);assert not r.returncode,r.stdout+r.stderr;objects.append(o)
 if n<12:symbols.append(next(s for s in Coff(o).symbols if s.startswith(specs[n][1])))
targets={symbols[n]:s[2] for n,s in enumerate(specs)}
proofs=[check(objects[n],symbols[n],a,l,len(Coff(objects[n]).sections[Coff(objects[n]).symbols[symbols[n]][0]][1]),targets) for n,(_,_,a,l,_) in enumerate(specs)]
assert all(p['grade']=='MATCH' for p in proofs[1:]) and proofs[0]['near_match']
link=[str(V.VC/'bin/link.exe'),'/nologo','/out:'+str(w/'fixture.exe'),'kernel32.lib']
for omit in range(12):
 r=subprocess.run(link+[str(o) for n,o in enumerate(objects) if n!=omit],env=env,capture_output=True,text=True);assert r.returncode
r=subprocess.run(link+list(map(str,objects)),env=env,capture_output=True,text=True);assert not r.returncode,r.stdout+r.stderr
r=subprocess.run([str(w/'fixture.exe')],env=env,capture_output=True,text=True,timeout=60);assert not r.returncode,(w,r.returncode,r.stdout,r.stderr);print(r.stdout.strip());assert 'cases=1344 snapshots=4608' in r.stdout
report=dict(cases=1344,snapshots=4608,providers=proofs,omission_checks=12,source_sha256={str(p.relative_to(ROOT)):hashlib.sha256(p.read_bytes()).hexdigest() for p in sources},fixture_sha256=hashlib.sha256(fixture.read_bytes()).hexdigest(),result=r.stdout.strip(),boundary='Entire actual default-sort,recursive introsort,partition,heap fallback and insertion chain.1344cases/4608full64KiB native state snapshots,counts0..1025,8keypatterns,depth0/1/2/64,two locations,partial sorting and make/sort heap composition. Independently sorted keys,payload bijection,maxheap checks,and untouched neighbouring bytes. Twelve mandatory sources;no algorithm callbacks. No drawing.')
(ROOT/'rebuild/compile-gate/native-subprimitive-full-sort.json').write_text(json.dumps(report,indent=2)+'\n');print('NATIVE_SUBPRIMITIVE_FULL_SORT_GATE_PASS')
