"""Native nonsorted primitive-manager drawing dispatch."""
import hashlib,json,re,subprocess,tempfile
from pathlib import Path
import check_one as V
from variable_allocator_parity import check
from pe_oracle import EXE,read_virtual_data
ROOT=Path(__file__).resolve().parents[2]
w=Path(tempfile.mkdtemp(prefix='native-camera-copy-'));env=V.env()
source=ROOT/'rebuild/src/compiled/00/b3/CEnginePrimitiveRenderer_RenderNonsortedPrimitives_00b32610.cpp'
fixture=ROOT/'rebuild/tests/integration/NativePrimitiveNonsorted_test.cpp'
symbol='?RenderNonsortedPrimitives@CEnginePrimitiveRenderer@@QAEXXZ'
raw=read_virtual_data(EXE.read_bytes(),0xb32610,69)
(w/'primitive_nonsorted_retail.h').write_text('static const unsigned char retailCode[]={'+','.join(map(str,raw))+'};\n#pragma comment(linker,"/alternatename:@RenderNonsorted@8='+symbol+'")\n')
objects=[]
for n,p in enumerate((source,fixture)):
 o=w/(str(n)+'.obj');r=V.cl(['/nologo','/c','/O2','/MT','/I'+str(w),'/Fo'+str(o),str(p)],env,w);assert not r.returncode,r.stdout+r.stderr;objects.append(o)
proof=check(objects[0],symbol,0xb32610,69,0,{})
link=[str(V.VC/'bin/link.exe'),'/nologo','/out:'+str(w/'fixture.exe'),'kernel32.lib']
r=subprocess.run(link+[str(objects[1])],env=env,capture_output=True,text=True);assert r.returncode
r=subprocess.run(link+list(map(str,objects)),env=env,capture_output=True,text=True);assert not r.returncode,r.stdout+r.stderr
r=subprocess.run([str(w/'fixture.exe')],env=env,capture_output=True,text=True,timeout=30);assert not r.returncode,(r.returncode,r.stdout,r.stderr)
match=re.search(r'NATIVE_PRIMITIVE_NONSORTED_PASS cases=(\d+) snapshots=(\d+)',r.stdout);assert match and int(match[1])==720;snapshots=int(match[2]);rpt=dict(cases=720,snapshots=snapshots,omission_checks=1,providers=[proof],source_sha256={str(source.relative_to(ROOT)):hashlib.sha256(source.read_bytes()).hexdigest()},fixture_sha256=hashlib.sha256(fixture.read_bytes()).hexdigest(),headers={name:hashlib.sha256((ROOT/'rebuild/include'/name).read_bytes()).hexdigest() for name in ('engine/CEnginePrimitiveRenderer.h','fable_engine_primitive_manager.h')},boundary='Actual nonsorted dispatch via recovered manager virtualslot11. Full2048-byte independent state model. Zero-to-four initialentries,storage swaps,truncation,growth tofive duringcallback,Data/Info replacement,repeated managers,enable byte mutations;controlblocks untouched. Concrete manager drawing remains callbacks;no visual frame.')
(ROOT/'rebuild/compile-gate/native-primitive-nonsorted.json').write_text(json.dumps(rpt,indent=2)+'\n')
print(r.stdout.strip());print('NATIVE_PRIMITIVE_NONSORTED_GATE_PASS',snapshots,'snapshots')
