import hashlib,json,subprocess,sys,tempfile
from pathlib import Path
ROOT=Path(__file__).resolve().parents[2];sys.path.insert(0,str(ROOT/'tools/decomp_pipeline'))
import check_one as V
from variable_allocator_parity import check
from pe_oracle import EXE,read_virtual_data
w=Path(tempfile.mkdtemp(prefix='native-display-end-fade-'));env=V.env();source=ROOT/'rebuild/src/compiled/00/43/CDisplayEngine_EndScreenFadeOut_00434c90.cpp'
globalSource=ROOT/'rebuild/src/compiled/01/37/DisplayEngine_FadeSettings_01375cdc.cpp';fixture=ROOT/'rebuild/tests/integration/NativeDisplayEndFade_test.cpp';objects=[]
(w/'display_end_fade_retail.h').write_text('static const unsigned char retailCode[]={'+','.join(map(str,read_virtual_data(EXE.read_bytes(),0x434c90,59)))+'};\n')
for i,path in enumerate([source,globalSource,fixture]):
 obj=w/f'{i}.obj';r=V.cl(['/nologo','/c','/O2','/MT','/I'+str(w),'/I'+str(ROOT/'rebuild/include'),'/Fo'+str(obj),str(path)],env,w);assert not r.returncode,r.stdout+r.stderr;objects.append(obj)
proof=check(objects[0],'?EndScreenFadeOut@CDisplayEngine@@QAEXXZ',0x434c90,59,1,{'?GInitialFadeAllowedToBeActive@@3_NA':0x1375cdc})
cmd=[str(V.VC/'bin/link.exe'),'/nologo','/out:'+str(w/'fixture.exe'),'kernel32.lib']
for omit in range(2):
 r=subprocess.run(cmd+[str(o) for i,o in enumerate(objects) if i!=omit],env=env,capture_output=True,text=True);assert r.returncode and ('LNK2001' in r.stdout or 'LNK2019' in r.stdout),r.stdout+r.stderr
r=subprocess.run(cmd+list(map(str,objects)),env=env,capture_output=True,text=True);assert not r.returncode,r.stdout+r.stderr
r=subprocess.run([str(w/'fixture.exe')],env=env,capture_output=True,text=True,timeout=30);assert not r.returncode,(w,r.returncode,r.stdout,r.stderr);assert 'cases=4096 snapshots=8192' in r.stdout
report=dict(cases=4096,snapshots=8192,providers=[proof],omission_checks=2,source_sha256={str(p.relative_to(ROOT)):hashlib.sha256(p.read_bytes()).hexdigest() for p in [source,globalSource]},fixture_sha256=hashlib.sha256(fixture.read_bytes()).hexdigest(),boundary='Actual end-fade method and settings global;no callbacks.Full8KiB native/compiled/independent-model snapshots;allguard combinations,prior opening/closing/global flags,rawfloatbit patterns,two locations/fills.')
(ROOT/'rebuild/compile-gate/native-display-end-fade.json').write_text(json.dumps(report,indent=2)+'\n');print(r.stdout.strip());print('NATIVE_DISPLAY_END_FADE_GATE_PASS')
