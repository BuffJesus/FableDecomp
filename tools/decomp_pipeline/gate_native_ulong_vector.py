"""Five native unsigned-long vector providers with composed CRT boundary replay."""
import hashlib,json,subprocess,tempfile
from pathlib import Path
import check_one as V
from gate_game_component_abi import Coff
from variable_allocator_parity import check
import oracle_native_ulong_vector as oracle
ROOT=Path(__file__).resolve().parents[2];w=Path(tempfile.mkdtemp(prefix='native-ulong-vector-'));env=V.env()
sources=[ROOT/('rebuild/src/compiled/00/46/UlongVector_'+name+'_'+address+'.cpp')for name,address in [('Resize','00464931'),('Storage','0046496f'),('Overflow','00464aab')]]
specs=[(0,'?resize@?$vector@KV?$allocator@K@_STL@@@_STL@@QAEXIABK@Z',0x464931,62,2),(1,'?erase@?$vector@KV?$allocator@K@_STL@@@_STL@@QAEPAKPAK0@Z',0x46496f,52,1),(1,'?_M_fill_insert@?$vector@KV?$allocator@K@_STL@@@_STL@@QAEXPAKIABK@Z',0x4649a3,264,4),(2,'??$__uninitialized_copy@K@_STL@@YIPAKPBK0PAKABU__true_type@0@@Z',0x464aab,35,1),(2,'?_M_insert_overflow@?$vector@KV?$allocator@K@_STL@@@_STL@@QAEXPAKABKABU__true_type@2@I_N@Z',0x464ace,188,4)]
targets={s:a for _,s,a,_,_ in specs};targets.update(_malloc=0xbfea0e,_memmove=0xbfeae6,_free=0xbfea14)
original=[];replay=[]
for i,source in enumerate(sources):
 for kind,objects,extra in [('production',original,[]),('replay',replay,['/Dmalloc=FableUlongMalloc','/Dfree=FableUlongFree','/Dmemmove=FableUlongMemmove'])]:
  obj=w/('%s%d.obj'%(kind,i));r=V.cl(['/nologo','/c','/O2','/MT','/Fo'+str(obj),str(source)]+extra,env,w);assert not r.returncode,r.stdout+r.stderr;objects.append(obj)
proof=[check(original[i],symbol,address,length,relocs,targets)for i,symbol,address,length,relocs in specs]
rename={'_malloc':'_FableUlongMalloc','_free':'_FableUlongFree','_memmove':'_FableUlongMemmove'}
for i,name,_,_,_ in specs:
 a,b=Coff(original[i]),Coff(replay[i]);sec,off=a.symbols[name];other,otheroff=b.symbols[name];body,rel=a.sections[sec];otherbody,otherrel=b.sections[other]
 assert body==otherbody and off==otheroff and {p:(rename.get(t,t),k)for p,(t,k)in rel.items()}==otherrel
fixture=ROOT/'rebuild/tests/integration/NativeUlongVector_test.cpp';obj=w/'fixture.obj'
r=V.cl(['/nologo','/c','/O2','/MT','/Fo'+str(obj),str(fixture)],env,w);assert not r.returncode,r.stdout+r.stderr
exe=w/'fixture.exe';cmd=[str(V.VC/'bin/link.exe'),'/nologo','/out:'+str(exe),'kernel32.lib',str(obj)]
for i in range(3):
 r=subprocess.run(cmd+[str(o)for j,o in enumerate(replay)if j!=i],env=env,capture_output=True,text=True);assert r.returncode,('provider required',i)
r=subprocess.run(cmd+list(map(str,replay)),env=env,capture_output=True,text=True);assert not r.returncode,r.stdout+r.stderr
inputs,expected,report=oracle.collect();(w/'input.bin').write_bytes(inputs)
r=subprocess.run([str(exe),str(w/'input.bin'),str(w/'actual.bin')],env=env,capture_output=True,text=True,timeout=60);assert not r.returncode,(w,r.returncode,r.stdout,r.stderr)
actual=(w/'actual.bin').read_bytes()
if actual!=expected:
 (w/'expected.bin').write_bytes(expected);difference=next((i for i,(a,b)in enumerate(zip(actual,expected))if a!=b),min(len(actual),len(expected)));raise AssertionError((w,difference,difference//(oracle.WIDTH+16),difference%(oracle.WIDTH+16),len(actual),len(expected)))
report.update(providers=proof,omission_checks=3,compared_bytes=len(actual),source_sha256=[hashlib.sha256(s.read_bytes()).hexdigest()for s in sources],fixture_sha256=hashlib.sha256(fixture.read_bytes()).hexdigest(),boundary='Actual resize, erase, fill insertion, uninitialized copy and overflow composed together. Full16KiB at each allocation, memmove, free and return; no masked bytes. Covers empty/spare/full storage, growth/shrink, all bounded insertion and erase positions, fill aliases, overlapping copy and mutating allocation/copy/free callbacks. CRT boundaries use audited renamed relocations. No allocation-failure recovery claim.')
(ROOT/'rebuild/compile-gate/native-ulong-vector.json').write_text(json.dumps(report,indent=2)+'\n');print(r.stdout.strip(),report['snapshots'],'snapshots')
