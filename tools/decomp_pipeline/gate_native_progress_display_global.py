"""Real CRT-owned engine singleton, engine registration and registered cleanup."""
import json,struct,subprocess,tempfile
from pathlib import Path
import check_one as V
from gate_game_component_abi import Coff
from gate_render_state_static_data import coff_data
from variable_allocator_parity import check
import oracle_native_progress_display_global as oracle
ROOT=Path(__file__).resolve().parents[2];w=Path(tempfile.mkdtemp(prefix='native-progress-display-global-'));env=V.env()
glob='?PProgressDisplay@NProgressDisplay@@3V?$CCountedPointer@VCAProgressDisplay@@@@A'
paths=['rebuild/src/compiled/01/22/Global_ProgressDisplay_012241b0.cpp','rebuild/src/compiled/00/9e/NProgressDisplay_StopProgress_009e9f20.cpp','rebuild/tests/integration/NativeProgressDisplayGlobal_test.cpp']
objects=[]
for i,path in enumerate(paths):
 obj=w/(str(i)+'.obj');r=V.cl(['/nologo','/c','/O2','/MT','/Fo'+str(obj),str(ROOT/path)],env,w);assert not r.returncode,r.stdout+r.stderr;objects.append(obj)
originals=objects
targets={glob:0x13caa38,'_$E2':0x122a890,'_atexit':0x4012bc,'??3@YAXPAX@Z':0xbfe9bc}
proof=[check(originals[0],'_$E1',0x12241b0,12,2,targets),check(originals[0],'_$E2',0x122a890,57,6,targets)]
c=Coff(originals[0]);sec,off=c.symbols[glob];assert off==0 and c.section_metadata[sec]['raw_size']==8 and coff_data(c,glob,8)==bytes(8);sec,off=c.symbols['_$S3'];assert c.sections[sec][1]=={0:('_$E1',6)}
exe=w/'fixture.exe';cmd=[str(V.VC/'bin/link.exe'),'/nologo','/out:'+str(exe),'kernel32.lib']
for omit in range(2):
 r=subprocess.run(cmd+[str(o)for i,o in enumerate(objects)if i!=omit],env=env,capture_output=True,text=True);assert r.returncode
r=subprocess.run(cmd+list(map(str,objects)),env=env,capture_output=True,text=True);assert not r.returncode,r.stdout+r.stderr
reports=[]
for mode in range(6):
 r=subprocess.run([str(exe),str(w/'actual.bin'),str(mode)],env=env,capture_output=True,text=True,timeout=30);assert not r.returncode,(w,r.returncode,r.stdout,r.stderr)
 expected,report=oracle.collect(mode);actual=(w/'actual.bin').read_bytes()
 if actual!=expected:
  (w/'expected.bin').write_bytes(expected);difference=next((i for i,(a,b)in enumerate(zip(actual,expected))if a!=b),min(len(actual),len(expected)));raise AssertionError((w,mode,difference,difference//4116,difference%4116,len(actual),len(expected)))
 reports.append(report)
report=dict(providers=proof,snapshots=sum(r['snapshots']for r in reports),modes=6,omission_checks=2,global_bytes=8,boundary='Real CRT owner and native StopProgress; boot ABI alias identity checked. Full4KiB plus owner; null/refcount0/1/2/FFFFFFFF, virtual forwarding and owner mutation during registered destruction. One deletion callback identity mapped; no masked state bytes.')
(ROOT/'rebuild/compile-gate/native-progress-display-global.json').write_text(json.dumps(report,indent=2)+'\n');print('NATIVE_PROGRESS_DISPLAY_GLOBAL_PASS',report['snapshots'],'snapshots')
