"""Native engine registration list, comparing full state through heap mutations."""
import hashlib,json,subprocess,tempfile
from pathlib import Path
import check_one as V
from gate_game_component_abi import Coff
from variable_allocator_parity import check
import oracle_native_engine_manager_list as oracle
ROOT=Path(__file__).resolve().parents[2];w=Path(tempfile.mkdtemp(prefix='native-engine-manager-list-'));env=V.env()
source=ROOT/'rebuild/src/compiled/00/64/CEngineManager_List_006404d0.cpp';helper=ROOT/'rebuild/src/compiled/00/64/EnginePointerList_Remove_006405b0.cpp';fixture=ROOT/'rebuild/tests/integration/NativeEngineManagerList_test.cpp'
add='?AddEngine@CEngineManager@@QAEXAAVCIEngine@@@Z';remove='?RemoveEngine@CEngineManager@@QAEXAAVCIEngine@@@Z';listremove='?remove@?$list@PAVCIEngine@@V?$allocator@PAVCIEngine@@@_STL@@@_STL@@QAEXABQAVCIEngine@@@Z'
targets={'_malloc':0xbfea0e,'_free':0xbfea14,listremove:0x6405b0}
aliases={'@AddEngine@12':add,'@RemoveEngine@12':remove,'@RemoveEnginePointer@12':listremove}
(w/'engine_manager_list_aliases.h').write_text('\n'.join('#pragma comment(linker,"/alternatename:'+a+'='+b+'")'for a,b in aliases.items())+'\n')
objects=[];originals=[]
for i,path in enumerate([source,helper,fixture]):
 obj=w/(str(i)+'.obj');options=['/Dmalloc=FableEngineListMalloc','/Dfree=FableEngineListFree']if i<2 else []
 r=V.cl(['/nologo','/c','/O2','/MT','/I'+str(w),'/Fo'+str(obj),str(path)]+options,env,w);assert not r.returncode,r.stdout+r.stderr;objects.append(obj)
 if i<2:
  original=w/(str(i)+'-original.obj');r=V.cl(['/nologo','/c','/O2','/MT','/Fo'+str(original),str(path)],env,w);assert not r.returncode,r.stdout+r.stderr;originals.append(original)
  a,b=Coff(original),Coff(obj)
  for symbol,(sec,off)in a.symbols.items():
   if not symbol.startswith('?'):continue
   other,otheroff=b.symbols[symbol];body,rel=a.sections[sec];otherbody,otherrel=b.sections[other];assert off==otheroff and body==otherbody and {p:(t.replace('_malloc','_FableEngineListMalloc').replace('_free','_FableEngineListFree'),k)for p,(t,k)in rel.items()}==otherrel
proof=[check(originals[0],add,0x6404d0,44,1,targets),check(originals[0],remove,0x6404b0,24,1,targets),check(originals[1],listremove,0x6405b0,54,1,targets)]
exe=w/'fixture.exe';cmd=[str(V.VC/'bin/link.exe'),'/nologo','/out:'+str(exe),'kernel32.lib']
for omit in [0,1]:
 r=subprocess.run(cmd+[str(o)for i,o in enumerate(objects)if i!=omit],env=env,capture_output=True,text=True);assert r.returncode
r=subprocess.run(cmd+list(map(str,objects)),env=env,capture_output=True,text=True);assert not r.returncode,r.stdout+r.stderr
inputs,expected,report=oracle.collect();(w/'input.bin').write_bytes(inputs)
r=subprocess.run([str(exe),str(w/'input.bin'),str(w/'actual.bin')],env=env,capture_output=True,text=True,timeout=60);assert not r.returncode,(w,r.returncode,r.stdout,r.stderr)
actual=(w/'actual.bin').read_bytes()
if actual!=expected:
 (w/'expected.bin').write_bytes(expected);difference=next((i for i,(a,b)in enumerate(zip(actual,expected))if a!=b),min(len(actual),len(expected)));raise AssertionError((w,difference,difference//4104,difference%4104,len(actual),len(expected)))
report.update(providers=proof,omission_checks=2,source_sha256=hashlib.sha256(source.read_bytes()).hexdigest(),helper_sha256=hashlib.sha256(helper.read_bytes()).hexdigest(),fixture_sha256=hashlib.sha256(fixture.read_bytes()).hexdigest(),boundary='Actual AddEngine/RemoveEngine and list removal. Full4KiB snapshots with circular lists of0..6 nodes, repeated/missing/null values, aliased comparison value, cached iteration links/end and allocator callbacks changing sentinel or values. Heap boundaries only; no normalized state bytes, no graceful allocation-failure claim.')
(ROOT/'rebuild/compile-gate/native-engine-manager-list.json').write_text(json.dumps(report,indent=2)+'\n');print(r.stdout.strip(),report['snapshots'],'snapshots')
