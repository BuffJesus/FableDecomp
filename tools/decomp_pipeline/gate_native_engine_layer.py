"""Native layer pre-render,render,post-render traversal."""
import hashlib,json,re,subprocess,tempfile
from pathlib import Path
import check_one as V
from variable_allocator_parity import check
from pe_oracle import EXE,read_virtual_data
ROOT=Path(__file__).resolve().parents[2]
w=Path(tempfile.mkdtemp(prefix='native-camera-copy-'));env=V.env()
source=ROOT/'rebuild/src/compiled/00/b2/CEngineLayerRenderer_Render_00b2ab80.cpp'
fixture=ROOT/'rebuild/tests/integration/NativeEngineLayer_test.cpp'
symbol='?Render@CEngineLayerRenderer@@UAEXABVCEngineComponentParams@@@Z'
raw=read_virtual_data(EXE.read_bytes(),0xb2ab80,251)
(w/'engine_layer_retail.h').write_text('static const unsigned char retailCode[]={'+','.join(map(str,raw))+'};\n#pragma comment(linker,"/alternatename:@RenderLayer@12='+symbol+'")\n')
objects=[]
for n,p in enumerate((source,fixture)):
 o=w/(str(n)+'.obj');r=V.cl(['/nologo','/c','/O2','/MT','/I'+str(w),'/Fo'+str(o),str(p)],env,w);assert not r.returncode,r.stdout+r.stderr;objects.append(o)
proof=check(objects[0],symbol,0xb2ab80,251,0,{})
link=[str(V.VC/'bin/link.exe'),'/nologo','/out:'+str(w/'fixture.exe'),'kernel32.lib']
r=subprocess.run(link+[str(objects[1])],env=env,capture_output=True,text=True);assert r.returncode
r=subprocess.run(link+list(map(str,objects)),env=env,capture_output=True,text=True);assert not r.returncode,r.stdout+r.stderr
r=subprocess.run([str(w/'fixture.exe')],env=env,capture_output=True,text=True,timeout=30);assert not r.returncode,(r.returncode,r.stdout,r.stderr)
match=re.search(r'NATIVE_ENGINE_LAYER_PASS cases=(\d+) snapshots=(\d+)',r.stdout);assert match and int(match[1])==2304;snapshots=int(match[2]);rpt=dict(cases=2304,snapshots=snapshots,omission_checks=1,providers=[proof],source_sha256={str(source.relative_to(ROOT)):hashlib.sha256(source.read_bytes()).hexdigest()},fixture_sha256=hashlib.sha256(fixture.read_bytes()).hexdigest(),headers={name:hashlib.sha256((ROOT/'rebuild/include'/name).read_bytes()).hexdigest() for name in ('fable_engine_layer_renderer.h','fable_engine_component.h','fable_engine_component_params.h')},boundary='Actual layer traversal; full1024-byte state at each callback and final,independent three-phase model. Zero-to-three components,outer and per-component scene filters,raw enable bytes,swapped array storage and truncated size,mutable input filters and cached layer/flags. Component virtual calls remain callbacks; no drawing claim.')
(ROOT/'rebuild/compile-gate/native-engine-layer.json').write_text(json.dumps(rpt,indent=2)+'\n')
print(r.stdout.strip());print('NATIVE_ENGINE_LAYER_GATE_PASS',snapshots,'snapshots')
