"""Engine singleton getter and real owner/list lifetimes, with heap boundaries."""
import hashlib,json,subprocess,tempfile
from pathlib import Path
import check_one as V
from gate_game_component_abi import Coff
from variable_allocator_parity import check
import oracle_native_engine_manager_owner as oracle
ROOT=Path(__file__).resolve().parents[2];w=Path(tempfile.mkdtemp(prefix='native-engine-manager-owner-'));env=V.env()
source=ROOT/'rebuild/src/compiled/00/43/CEngineManager_Get_00436fb0.cpp';owner=ROOT/'rebuild/src/compiled/00/43/CEngineManager_Ownership_004366b0.cpp';helper=ROOT/'rebuild/src/compiled/00/43/EnginePointerList_Destructor_00436900.cpp';fixture=ROOT/'rebuild/tests/integration/NativeEngineManagerOwner_test.cpp'
get='?Get@?$CTBaseSingleton@VCEngineManager@@@@SIAAVCEngineManager@@XZ';reset='?Reset@?$CCountedPointer@VCEngineManager@@@@QAEXPAVCEngineManager@@@Z';delete='?DeleteData@?$CCountedPointer@VCEngineManager@@@@CIXPAX@Z';dtor='??1CEngineManager@@UAE@XZ';scalar='??_GCEngineManager@@UAEPAXI@Z';listdtor='??1?$list@PAVCIEngine@@V?$allocator@PAVCIEngine@@@_STL@@@_STL@@QAE@XZ';glob='?PClass@?$CTBaseSingleton@VCEngineManager@@@@2V?$CCountedPointer@VCEngineManager@@@@A'
targets={glob:0x13ba854,reset:0x4366b0,delete:0x4361e0,dtor:0x437040,listdtor:0x436900,'??_7CEngineManager@@6B@':0x1231584,'??_7?$CTBaseSingleton@VCEngineManager@@@@6B@':0x123156c,'??2@YAPAXI@Z':0xbfea1a,'??3@YAXPAX@Z':0xbfe9bc,'_malloc':0xbfea0e,'_free':0xbfea14}
aliases={'@GetEngineManager@0':get,'@ResetEngineManager@12':reset,'@DeleteEngineManager@12':scalar,'@DestroyEngineList@4':listdtor,'@DeleteManagerData@4':delete,'_ManagerTable':'??_7CEngineManager@@6B@','_SingletonTable':'??_7?$CTBaseSingleton@VCEngineManager@@@@6B@',glob:'_EngineManagerOwner'}
(w/'engine_manager_owner_aliases.h').write_text('\n'.join('#pragma comment(linker,"/alternatename:'+a+'='+b+'")'for a,b in aliases.items())+'\n')
objects=[];originals=[]
for i,path in enumerate([source,owner,helper,fixture]):
 obj=w/(str(i)+'.obj');options=['/Dmalloc=FableEngineOwnerMalloc','/Dfree=FableEngineOwnerFree']if i<3 else []
 r=V.cl(['/nologo','/c','/O2','/MT','/I'+str(w),'/Fo'+str(obj),str(path)]+options,env,w);assert not r.returncode,r.stdout+r.stderr;objects.append(obj)
 if i<3:
  original=w/(str(i)+'-original.obj');r=V.cl(['/nologo','/c','/O2','/MT','/Fo'+str(original),str(path)],env,w);assert not r.returncode,r.stdout+r.stderr;originals.append(original)
  a,b=Coff(original),Coff(obj)
  for symbol,(sec,off)in a.symbols.items():
   if not symbol.startswith('?'):continue
   other,otheroff=b.symbols[symbol];body,rel=a.sections[sec];otherbody,otherrel=b.sections[other];assert off==otheroff and body==otherbody and {p:(t.replace('_malloc','_FableEngineOwnerMalloc').replace('_free','_FableEngineOwnerFree'),k)for p,(t,k)in rel.items()}==otherrel
proof=[check(originals[i],symbol,address,length,n,targets)for i,symbol,address,length,n in [(0,get,0x436fb0,105,10),(0,dtor,0x437040,35,3),(0,scalar,0x437020,30,2),(1,reset,0x4366b0,103,3),(1,delete,0x4361e0,11,0),(2,listdtor,0x436900,62,2)]]
Coff(originals[0]).table('??_7CEngineManager@@6B@',['??_ECEngineManager@@UAEPAXI@Z'])
exe=w/'fixture.exe';cmd=[str(V.VC/'bin/link.exe'),'/nologo','/out:'+str(exe),'kernel32.lib']
for omit in range(3):
 r=subprocess.run(cmd+[str(o)for i,o in enumerate(objects)if i!=omit],env=env,capture_output=True,text=True);assert r.returncode
r=subprocess.run(cmd+list(map(str,objects)),env=env,capture_output=True,text=True);assert not r.returncode,r.stdout+r.stderr
inputs,expected,report=oracle.collect();(w/'input.bin').write_bytes(inputs)
r=subprocess.run([str(exe),str(w/'input.bin'),str(w/'actual.bin')],env=env,capture_output=True,text=True,timeout=60);assert not r.returncode,(w,r.returncode,r.stdout,r.stderr)
actual=(w/'actual.bin').read_bytes()
if actual!=expected:
 (w/'expected.bin').write_bytes(expected);difference=next((i for i,(a,b)in enumerate(zip(actual,expected))if a!=b),min(len(actual),len(expected)));raise AssertionError((w,difference,difference//4116,difference%4116,len(actual),len(expected)))
report.update(providers=proof,omission_checks=3,source_sha256=hashlib.sha256(source.read_bytes()).hexdigest(),owner_sha256=hashlib.sha256(owner.read_bytes()).hexdigest(),helper_sha256=hashlib.sha256(helper.read_bytes()).hexdigest(),fixture_sha256=hashlib.sha256(fixture.read_bytes()).hexdigest(),boundary='Actual singleton getter, counted Reset/DeleteData, manager destructor/scalar delete and list destructor. Full4KiB plus8-byte owner snapshots. Existing/null global values, refs0/1/2/FFFFFFFF, object/control allocation failures,0..6 list nodes, flags0/1/2/FFFFFFFF and callback mutations. Only heap boundaries; callback/vtable identities mapped. Fixture owns global storage; real CRT global is tested separately.')
(ROOT/'rebuild/compile-gate/native-engine-manager-owner.json').write_text(json.dumps(report,indent=2)+'\n');print(r.stdout.strip(),report['snapshots'],'snapshots')
