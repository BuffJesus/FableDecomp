import hashlib,json,subprocess,sys,tempfile
from pathlib import Path
ROOT=Path(__file__).resolve().parents[2];sys.path.insert(0,str(ROOT/'tools/decomp_pipeline'))
import check_one as V
from variable_allocator_parity import check
from pe_oracle import EXE,read_virtual_data
w=Path(tempfile.mkdtemp(prefix='native-vertex-format-'));env=V.env()
source=ROOT/'rebuild/src/compiled/00/a0/CRenderManagerCore_SetVertexFormat_00a0aa30.cpp';fixture=ROOT/'rebuild/tests/integration/NativeVertexFormat_test.cpp'
symbol='?SetVertexFormat@CRenderManagerCore@@QAEXK@Z';body=read_virtual_data(EXE.read_bytes(),0xa0aa30,23)
(w/'vertex_format_retail.h').write_text('static const unsigned char retailCode[]={'+','.join(map(str,body))+'};\n')
(w/'vertex_format_aliases.h').write_text('#pragma comment(linker,"/alternatename:@SetFormat@12='+symbol+'")\n')
objects=[]
for i,path in enumerate((source,fixture)):
    obj=w/f'{i}.obj';r=V.cl(['/nologo','/c','/O2','/MT','/I'+str(w),'/Fo'+str(obj),str(path)],env,w);assert not r.returncode,r.stdout+r.stderr;objects.append(obj)
proof=check(objects[0],symbol,0xa0aa30,23,0,{})
cmd=[str(V.VC/'bin/link.exe'),'/nologo','/out:'+str(w/'fixture.exe'),'kernel32.lib']
r=subprocess.run(cmd+[str(objects[1])],env=env,capture_output=True,text=True);assert r.returncode
r=subprocess.run(cmd+list(map(str,objects)),env=env,capture_output=True,text=True);assert not r.returncode,r.stdout+r.stderr
r=subprocess.run([str(w/'fixture.exe')],env=env,capture_output=True,text=True,timeout=30);assert not r.returncode,(w,r.returncode,r.stdout,r.stderr);assert 'cases=1024 snapshots=4096' in r.stdout
report=dict(cases=1024,snapshots=4096,providers=[proof],omission_checks=1,source_sha256={str(source.relative_to(ROOT)):hashlib.sha256(source.read_bytes()).hexdigest()},fixture_sha256=hashlib.sha256(fixture.read_bytes()).hexdigest(),boundary='Canonical core SetVertexFormat,actual COMslot89.128FVF patterns,two objects,success/failure,device/index-pointer callback mutations. Full32KiB callback and final images compared to independent expected state and native execution;only COM boundary.')
(ROOT/'rebuild/compile-gate/native-vertex-format.json').write_text(json.dumps(report,indent=2)+'\n');print(r.stdout.strip());print('NATIVE_VERTEX_FORMAT_GATE_PASS')
