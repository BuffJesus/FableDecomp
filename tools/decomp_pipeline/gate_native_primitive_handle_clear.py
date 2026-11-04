import hashlib,json,subprocess,sys,tempfile
from pathlib import Path
ROOT=Path(__file__).resolve().parents[2];sys.path.insert(0,str(ROOT/'tools/decomp_pipeline'))
import check_one as V
from variable_allocator_parity import check
from pe_oracle import EXE,read_virtual_data
w=Path(tempfile.mkdtemp(prefix='native-primitive-handle-clear-'));env=V.env();source=ROOT/'rebuild/src/compiled/00/b3/CEnginePrimitiveRenderer_SetPersistentPrimitive_00b324a0.cpp'
symbol='?Clear@CEnginePrimitiveHandle@@QAEXXZ';obj=w/'clear.obj';r=V.cl(['/nologo','/c','/O2','/MT','/I'+str(ROOT/'rebuild/include'),'/Fo'+str(obj),str(source)],env,w);assert not r.returncode,r.stdout+r.stderr;proof=check(obj,symbol,0x41be70,57,0,{})
(w/'primitive_handle_clear_retail.h').write_text('static const unsigned char retailCode[]={'+','.join(map(str,read_virtual_data(EXE.read_bytes(),0x41be70,57)))+'};\n');(w/'primitive_handle_clear_aliases.h').write_text('#pragma comment(linker,"/alternatename:@HandleClear@8='+symbol+'")\n')
fixture=ROOT/'rebuild/tests/integration/NativePrimitiveHandleClear_test.cpp';fo=w/'fixture.obj';r=V.cl(['/nologo','/c','/O2','/MT','/I'+str(w),'/I'+str(ROOT/'rebuild/include'),'/Fo'+str(fo),str(fixture)],env,w);assert not r.returncode,r.stdout+r.stderr
cmd=[str(V.VC/'bin/link.exe'),'/nologo','/out:'+str(w/'fixture.exe'),'kernel32.lib',str(fo)];r=subprocess.run(cmd,env=env,capture_output=True,text=True);assert r.returncode and ('LNK2001' in r.stdout or 'LNK2019' in r.stdout)
r=subprocess.run(cmd+[str(obj)],env=env,capture_output=True,text=True);assert not r.returncode,r.stdout+r.stderr
r=subprocess.run([str(w/'fixture.exe')],env=env,capture_output=True,text=True,timeout=30);assert not r.returncode,(w,r.returncode,r.stdout,r.stderr)
assert 'cases=1152 snapshots=2560' in r.stdout
header=ROOT/'rebuild/include/fable_engine_primitive_handle.h'
report=dict(cases=1152,snapshots=2560,providers=[proof],omission_checks=1,result=r.stdout.strip(),source_sha256={str(p.relative_to(ROOT)):hashlib.sha256(p.read_bytes()).hexdigest() for p in (source,header)},fixture_sha256=hashlib.sha256(fixture.read_bytes()).hexdigest(),boundary='Actual primitivehandleClear inline COMDAT emitted by existing persistent renderer TU;virtual NotifyReleased/Delete callbacks. Independent/native/compiled full16KiB snapshots after callbacks and return. Null/live,unsigned count wrap,callback replacement/clearing/recount and postcallback reloaded pointer;two placements/fills. Three aborting methods and two zero globals only satisfy unused renderer-parent references;executing them fails the fixture.')
(ROOT/'rebuild/compile-gate/native-primitive-handle-clear.json').write_text(json.dumps(report,indent=2)+'\n');print(r.stdout.strip());print('NATIVE_PRIMITIVE_HANDLE_CLEAR_GATE_PASS')
