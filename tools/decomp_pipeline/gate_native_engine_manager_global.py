"""Real CRT-owned engine singleton, engine registration and registered cleanup."""
import json,struct,subprocess,tempfile
from pathlib import Path
import check_one as V
from gate_game_component_abi import Coff
from gate_render_state_static_data import coff_data
from variable_allocator_parity import check
import oracle_native_engine_manager_global as oracle
ROOT=Path(__file__).resolve().parents[2];w=Path(tempfile.mkdtemp(prefix='native-engine-manager-global-'));env=V.env()
glob='?PClass@?$CTBaseSingleton@VCEngineManager@@@@2V?$CCountedPointer@VCEngineManager@@@@A'
aliases={'_EngineManagerOwner':glob,'_ManagerTable':'??_7CEngineManager@@6B@','_SingletonTable':'??_7?$CTBaseSingleton@VCEngineManager@@@@6B@','@GetEngineManager@0':'?Get@?$CTBaseSingleton@VCEngineManager@@@@SIAAVCEngineManager@@XZ','@DeleteManagerData@4':'?DeleteData@?$CCountedPointer@VCEngineManager@@@@CIXPAX@Z','@AddEngine@12':'?AddEngine@CEngineManager@@QAEXAAVCIEngine@@@Z','@RemoveEngine@12':'?RemoveEngine@CEngineManager@@QAEXAAVCIEngine@@@Z'}
(w/'engine_manager_global_aliases.h').write_text('\n'.join('#pragma comment(linker,"/alternatename:'+a+'='+b+'")'for a,b in aliases.items())+'\n')
paths=['rebuild/src/compiled/01/21/Global_EngineManager_0121df10.cpp','rebuild/src/compiled/00/43/CEngineManager_Get_00436fb0.cpp','rebuild/src/compiled/00/43/CEngineManager_Ownership_004366b0.cpp','rebuild/src/compiled/00/43/EnginePointerList_Destructor_00436900.cpp','rebuild/src/compiled/00/64/CEngineManager_List_006404d0.cpp','rebuild/src/compiled/00/64/EnginePointerList_Remove_006405b0.cpp','rebuild/tests/integration/NativeEngineManagerGlobal_test.cpp']
objects=[];originals=[]
for i,path in enumerate(paths):
 obj=w/(str(i)+'.obj');options=['/Dmalloc=FableEngineGlobalMalloc','/Dfree=FableEngineGlobalFree']if i<6 else []
 r=V.cl(['/nologo','/c','/O2','/MT','/I'+str(w),'/Fo'+str(obj),str(ROOT/path)]+options,env,w);assert not r.returncode,r.stdout+r.stderr;objects.append(obj)
 if i<6:
  original=w/(str(i)+'-original.obj');r=V.cl(['/nologo','/c','/O2','/MT','/Fo'+str(original),str(ROOT/path)],env,w);assert not r.returncode,r.stdout+r.stderr;originals.append(original)
  a,b=Coff(original),Coff(obj)
  for symbol,(sec,off)in a.symbols.items():
   if not symbol.startswith('?')and symbol not in ['_$E1','_$E2','_$S3']:continue
   other,otheroff=b.symbols[symbol];body,rel=a.sections[sec];otherbody,otherrel=b.sections[other]
   if struct.unpack_from('<I',a.data,20+(sec-1)*40+36)[0]&0x80:body=coff_data(a,symbol,len(body));otherbody=coff_data(b,symbol,len(otherbody))
   assert off==otheroff and body==otherbody and {p:(t.replace('_malloc','_FableEngineGlobalMalloc').replace('_free','_FableEngineGlobalFree'),k)for p,(t,k)in rel.items()}==otherrel
targets={glob:0x13ba854,'_$E2':0x1229190,'_atexit':0x4012bc,'??3@YAXPAX@Z':0xbfe9bc}
proof=[check(originals[0],'_$E1',0x121df10,12,2,targets),check(originals[0],'_$E2',0x1229190,57,6,targets)]
c=Coff(originals[0]);sec,off=c.symbols[glob];assert off==0 and c.section_metadata[sec]['raw_size']==8 and coff_data(c,glob,8)==bytes(8);sec,off=c.symbols['_$S3'];assert c.sections[sec][1]=={0:('_$E1',6)}
exe=w/'fixture.exe';cmd=[str(V.VC/'bin/link.exe'),'/nologo','/out:'+str(exe),'kernel32.lib']
for omit in range(6):
 r=subprocess.run(cmd+[str(o)for i,o in enumerate(objects)if i!=omit],env=env,capture_output=True,text=True);assert r.returncode
r=subprocess.run(cmd+list(map(str,objects)),env=env,capture_output=True,text=True);assert not r.returncode,r.stdout+r.stderr
r=subprocess.run([str(exe),str(w/'actual.bin')],env=env,capture_output=True,text=True,timeout=30);assert not r.returncode,(w,r.returncode,r.stdout,r.stderr)
expected,report=oracle.collect();actual=(w/'actual.bin').read_bytes()
if actual!=expected:
 (w/'expected.bin').write_bytes(expected);difference=next((i for i,(a,b)in enumerate(zip(actual,expected))if a!=b),min(len(actual),len(expected)));raise AssertionError((w,difference,difference//4112,difference%4112,len(actual),len(expected)))
report.update(providers=proof,omission_checks=6,global_bytes=8,boundary='Real C++ static owner and CRT registration; native singleton Get, manager construction, owner Reset, AddEngine/RemoveEngine, list removal and full registered teardown. Full4KiB plus8-byte owner snapshots before main work, through allocation/list mutations and after actual atexit destruction. Only heap boundaries. Vtable and deletion callback identities mapped; no masked state bytes.')
(ROOT/'rebuild/compile-gate/native-engine-manager-global.json').write_text(json.dumps(report,indent=2)+'\n');print(r.stdout.strip(),report['snapshots'],'snapshots')
