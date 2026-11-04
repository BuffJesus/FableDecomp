"""Full native simple-pool owner lifetime, including both deleting flags."""
import hashlib,json,subprocess,tempfile
from pathlib import Path
import check_one as V
from gate_game_component_abi import Coff
from variable_allocator_parity import check
import oracle_native_pool_owner as oracle
ROOT=Path(__file__).resolve().parents[2];w=Path(tempfile.mkdtemp(prefix='native-pool-owner-'));env=V.env()
source=ROOT/'rebuild/src/compiled/00/a5/CSimpleMemoryPool_Lifetime_00a5c570.cpp';fixture=ROOT/'rebuild/tests/integration/NativePoolOwner_test.cpp'
ctor='??0CSimpleMemoryPool@@QAE@KK@Z';dtor='??1CSimpleMemoryPool@@UAE@XZ';clear='?ClearMemoryBlocks@CSimpleMemoryPool@@AAEXXZ';scalar='??_GCSimpleMemoryPool@@UAEPAXI@Z'
targets={'??0CBaseClassNonCopyable@@QAE@XZ':0x99a2f0,'??1CBaseClassNonCopyable@@UAE@XZ':0x99a300,'??_7CSimpleMemoryPool@@6B@':0x129a064,clear:0xa5c600,'??3@YAXPAX@Z':0xbfe9bc}
aliases={'@ConstructPool@16':ctor,'@DestroyPool@4':dtor,'@ClearPool@4':clear,'@DeletePool@12':scalar,'_PoolTable':'??_7CSimpleMemoryPool@@6B@','??0CBaseClassNonCopyable@@QAE@XZ':'@BaseCtorBoundary@4','??1CBaseClassNonCopyable@@UAE@XZ':'@BaseDtorBoundary@4'}
(w/'pool_owner_aliases.h').write_text('\n'.join('#pragma comment(linker,"/alternatename:'+a+'='+b+'")'for a,b in aliases.items())+'\n')
objects=[]
for name,path in [('production',source),('fixture',fixture)]:
 obj=w/(name+'.obj');r=V.cl(['/nologo','/c','/O2','/MT','/I'+str(w),'/Fo'+str(obj),str(path)],env,w);assert not r.returncode,r.stdout+r.stderr;objects.append(obj)
c=Coff(objects[0]);proof=[]
for symbol,address,length in [(ctor,0xa5c570,74),(dtor,0x987f10,22),(clear,0xa5c600,54),(scalar,0x987f30,43)]:
 sec,_=c.symbols[symbol];proof.append(check(objects[0],symbol,address,length,len(c.sections[sec][1]),targets))
c.table('??_7CSimpleMemoryPool@@6B@',['??_ECSimpleMemoryPool@@UAEPAXI@Z'])
exe=w/'fixture.exe';cmd=[str(V.VC/'bin/link.exe'),'/nologo','/out:'+str(exe),'kernel32.lib']
r=subprocess.run(cmd+[str(objects[1])],env=env,capture_output=True,text=True);assert r.returncode,'native pool provider required'
r=subprocess.run(cmd+list(map(str,objects)),env=env,capture_output=True,text=True);assert not r.returncode,r.stdout+r.stderr
inputs,expected,report=oracle.collect();(w/'input.bin').write_bytes(inputs)
r=subprocess.run([str(exe),str(w/'input.bin'),str(w/'actual.bin')],env=env,capture_output=True,text=True,timeout=60);assert not r.returncode,(w,r.returncode,r.stdout,r.stderr)
actual=(w/'actual.bin').read_bytes()
if actual!=expected:
 (w/'expected.bin').write_bytes(expected);difference=next((i for i,(a,b)in enumerate(zip(actual,expected))if a!=b),min(len(actual),len(expected)));raise AssertionError((w,difference,difference//(oracle.WIDTH+16),difference%(oracle.WIDTH+16),len(actual),len(expected)))
report.update(providers=proof,omission_checks=1,compared_bytes=len(actual),source_sha256=hashlib.sha256(source.read_bytes()).hexdigest(),fixture_sha256=hashlib.sha256(fixture.read_bytes()).hexdigest(),boundary='Actual constructor, destructor, clear, and scalar deleting body. Full4KiB state with base and virtual block deletion boundaries, wrapping alignment, untouched root status, ownership, cached next pointers, callback mutations, and deleting flags. Only compiled pool vtable identity is mapped to retail.')
(ROOT/'rebuild/compile-gate/native-pool-owner.json').write_text(json.dumps(report,indent=2)+'\n');print(r.stdout.strip(),report['snapshots'],'snapshots')
