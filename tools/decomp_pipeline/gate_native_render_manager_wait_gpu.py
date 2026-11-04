import hashlib,json,subprocess,sys,tempfile
from pathlib import Path
ROOT=Path(__file__).resolve().parents[2];sys.path.insert(0,str(ROOT/'tools/decomp_pipeline'))
import check_one as V
from variable_allocator_parity import check
from gate_game_component_abi import Coff
from pe_oracle import EXE,read_virtual_data
w=Path(tempfile.mkdtemp(prefix='native-render-wait-gpu-'));env=V.env();source=ROOT/'rebuild/src/compiled/00/a0/CRenderManagerCore_WaitWhileGPUIsBusy_00a0af80.cpp'
if not source.exists():source=ROOT/'work/parity_addtext/render_manager_wait_gpu_candidate_20261009.cpp'
obj=w/'wait.obj';r=V.cl(['/nologo','/c','/O2','/MT','/I'+str(ROOT/'rebuild/include'),'/Fo'+str(obj),str(source)],env,w);assert not r.returncode,r.stdout+r.stderr
symbol='?WaitWhileGPUIsBusy@CRenderManagerCore@@QAEXXZ';proof=check(obj,symbol,0xa0af80,1,0,{});assert read_virtual_data(EXE.read_bytes(),0xa0af80,1)==b'\xc3'
(w/'wait_gpu_aliases.h').write_text('#pragma comment(linker,"/alternatename:@WaitRoot@8='+symbol+'")\n')
fixture=ROOT/'rebuild/tests/integration/NativeRenderManagerWaitGPU_test.cpp';fo=w/'fixture.obj';r=V.cl(['/nologo','/c','/O2','/MT','/I'+str(w),'/Fo'+str(fo),str(fixture)],env,w);assert not r.returncode,r.stdout+r.stderr
cmd=[str(V.VC/'bin/link.exe'),'/nologo','/out:'+str(w/'fixture.exe'),'kernel32.lib',str(fo)];r=subprocess.run(cmd,env=env,capture_output=True,text=True);assert r.returncode and ('LNK2001' in r.stdout or 'LNK2019' in r.stdout),r.stdout+r.stderr
r=subprocess.run(cmd+[str(obj)],env=env,capture_output=True,text=True);assert not r.returncode,r.stdout+r.stderr;r=subprocess.run([str(w/'fixture.exe')],env=env,capture_output=True,text=True,timeout=30);assert not r.returncode,(w,r.returncode,r.stdout,r.stderr);assert 'cases=24 snapshots=48' in r.stdout,r.stdout
header=ROOT/'rebuild/include/fable_render_state_manager.h';report=dict(cases=24,snapshots=48,providers=[proof],omission_checks=1,result=r.stdout.strip(),source_sha256={str(p.relative_to(ROOT)):hashlib.sha256(p.read_bytes()).hexdigest() for p in [source,header]},fixture_sha256=hashlib.sha256(fixture.read_bytes()).hexdigest(),boundary='Original public void noargs WaitWhileGPUIsBusy donor774540/1byte,retailA0AF80 RET1. Actualcompiledsource/noomissionfallback,rawNULL-invalid-valid receiver words,two placements-fills/full16KiB unchangedmemory. Canonicalidentity correction ofalreadycounted E2/C00A0AF80 placeholder,zero newcoverage. PCno-op preserved,no GPU synchronization invented.')
(ROOT/'rebuild/compile-gate/native-render-manager-wait-gpu.json').write_text(json.dumps(report,indent=2)+'\n');print(r.stdout.strip());print('NATIVE_RENDER_MANAGER_WAIT_GPU_GATE_PASS')
