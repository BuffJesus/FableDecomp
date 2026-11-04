"""Display readiness countdown and floating draw-distance state update."""
import hashlib,json,subprocess,tempfile
from pathlib import Path
import check_one as V
from gate_game_component_abi import Coff
from gate_render_state_static_data import coff_data
from variable_allocator_parity import check
from pe_oracle import EXE,pe_sections,va_to_off,read_virtual_data
import oracle_native_display_engine_update as oracle
ROOT=Path(__file__).resolve().parents[2];w=Path(tempfile.mkdtemp(prefix='native-display-engine-update-'));env=V.env()
source=ROOT/'rebuild/src/compiled/00/43/CDisplayEngine_WorldUpdate_00434f60.cpp';fixture=ROOT/'rebuild/tests/integration/NativeDisplayEngineUpdate_test.cpp'
symbol='?WorldUpdate@CDisplayEngine@@QAEXXZ'
targets={'?StopProgress@NProgressDisplay@@YIX_N0@Z':0x9e9f20,'??_C@_11LOCGONAA@?$AA?$AA@':0x122d70c,'?GResetProgressTipText@@3VCWideString@@A':0x13b89d8,'?GResetProgressTipTitleText@@3VCWideString@@A':0x13b89dc,'??4CWideString@@QAEABV0@PBG@Z':0x99b800,'?SetAboutToRun@CDisplayEngine@@QAEXXZ':0x434cd0,'__real@3f800000':0x122ded8,'?GamePrimitiveDefaultFadeRangeRatio@CDisplayEngine@@2MA':0x13b8774}
aliases={'@UpdateDisplayEngine@4':symbol,'?StopProgress@NProgressDisplay@@YIX_N0@Z':'@StopProgressBoundary@8','?GResetProgressTipText@@3VCWideString@@A':'_TipText','?GResetProgressTipTitleText@@3VCWideString@@A':'_TipTitle','??4CWideString@@QAEABV0@PBG@Z':'@AssignWideBoundary@12','?SetAboutToRun@CDisplayEngine@@QAEXXZ':'@AboutToRunBoundary@4'}
(w/'display_engine_update_aliases.h').write_text('\n'.join('#pragma comment(linker,"/alternatename:'+a+'='+b+'")'for a,b in aliases.items())+'\n')
objects=[]
settings=ROOT/'rebuild/src/compiled/01/37/DisplayEngine_FadeSettings_01375cdc.cpp'
for i,path in enumerate([source,fixture,settings]):
 obj=w/(str(i)+'.obj');r=V.cl(['/nologo','/c','/O2','/MT','/I'+str(w),'/Fo'+str(obj),str(path)],env,w);assert not r.returncode,r.stdout+r.stderr;objects.append(obj)
proof=check(objects[0],symbol,0x434f60,104,10,targets);c=Coff(objects[0]);raw=EXE.read_bytes();sections=pe_sections(raw)
for name,value in [('??_C@_11LOCGONAA@?$AA?$AA@',bytes(2)),('__real@3f800000',b'\0\0\x80\x3f')]:
 sec,off=c.symbols[name];assert c.sections[sec][0][off:off+len(value)]==value;o=va_to_off(sections,targets[name]);assert raw[o:o+len(value)]==value
data=Coff(objects[2]);data_checks=[]
for address,name,value in [(0x1375cdc,'?GInitialFadeAllowedToBeActive@@3_NA',b'\x01'),(0x13b8774,'?GamePrimitiveDefaultFadeRangeRatio@CDisplayEngine@@2MA',bytes(4))]:
 assert coff_data(data,name,len(value))==value;assert read_virtual_data(raw,address,len(value))==value;data_checks.append(dict(address='%08x'%address,symbol=name,size=len(value)))
assert not any(name.startswith('_$E') or name.startswith('_$S') for name in data.symbols)
exe=w/'fixture.exe';cmd=[str(V.VC/'bin/link.exe'),'/nologo','/out:'+str(exe),'kernel32.lib']
r=subprocess.run(cmd+[str(objects[1])],env=env,capture_output=True,text=True);assert r.returncode
r=subprocess.run(cmd+list(map(str,objects[:2])),env=env,capture_output=True,text=True);assert r.returncode
r=subprocess.run(cmd+list(map(str,objects)),env=env,capture_output=True,text=True);assert not r.returncode,r.stdout+r.stderr
inputs,expected,report=oracle.collect();(w/'input.bin').write_bytes(inputs)
r=subprocess.run([str(exe),str(w/'input.bin'),str(w/'actual.bin')],env=env,capture_output=True,text=True,timeout=60);assert not r.returncode,(w,r.returncode,r.stdout,r.stderr)
actual=(w/'actual.bin').read_bytes()
if actual!=expected:
 (w/'expected.bin').write_bytes(expected);difference=next((i for i,(a,b)in enumerate(zip(actual,expected))if a!=b),min(len(actual),len(expected)));raise AssertionError((w,difference,difference//4124,difference%4124,len(actual),len(expected)))
report.update(data_symbols=data_checks,providers=[proof],omission_checks=2,settings_sha256=hashlib.sha256(settings.read_bytes()).hexdigest(),source_sha256=hashlib.sha256(source.read_bytes()).hexdigest(),fixture_sha256=hashlib.sha256(fixture.read_bytes()).hexdigest(),boundary='Actual signed readiness countdown, progress-tip clearing and live engine virtual getter. Full4KiB plus three global words; counter limits, null/live/replaced engine, callback mutations and floating zeros/finite/infinity/NaN/subnormal values at53/64-bit x87 precision. Progress stop, wide assignment, SetAboutToRun and draw-distance virtual are recorded boundaries. No normalized state bytes.')
(ROOT/'rebuild/compile-gate/native-display-engine-update.json').write_text(json.dumps(report,indent=2)+'\n');print(r.stdout.strip(),report['snapshots'],'snapshots')
