"""Native renderer matrix submission and perspective arithmetic against retail."""
import hashlib,itertools,json,struct,subprocess,tempfile
from pathlib import Path
import check_one as V
from variable_allocator_parity import check
from pe_oracle import EXE,read_virtual_data
ROOT=Path(__file__).resolve().parents[2];w=Path(tempfile.mkdtemp(prefix='native-renderer-matrices-'));env=V.env()
source=ROOT/'rebuild/src/compiled/00/a0/CRenderManagerCore_MatrixSetters_00a0b0a0.cpp';fixture=ROOT/'rebuild/tests/integration/NativeRendererMatrices_test.cpp'
specs=[('?SetViewTransform@CRenderManagerCore@@QAEXABVCMatrix3x4@@@Z',0xa0b0a0,148),('?SetWorldTransform@CRenderManagerCore@@QAEXABVCMatrix3x4@@@Z',0xa0b140,151),('?SetPerspectiveTransform@CRenderManagerCore@@QAEXMMMM@Z',0xa0b320,186)]
(w/'renderer_matrices_aliases.h').write_text('\n'.join('#pragma comment(linker,"/alternatename:'+name+'='+s[0]+'")'for name,s in zip(('@SetView@12','@SetWorld@12','@SetPerspective@24'),specs))+'\n')
raw=EXE.read_bytes();(w/'renderer_matrices_retail.h').write_text('\n'.join('static const unsigned char '+name+'[]={'+','.join(str(b)for b in read_virtual_data(raw,address,size))+'};'for name,(_,address,size)in zip(('retailView','retailWorld','retailPerspective'),specs))+'\n')
objects=[]
for i,path in enumerate((source,fixture)):
    obj=w/(str(i)+'.obj');r=V.cl(['/nologo','/c','/O2','/MT','/I'+str(w),'/Fo'+str(obj),str(path)],env,w);assert not r.returncode,r.stdout+r.stderr;objects.append(obj)
proof=[check(objects[0],symbol,address,size,0,{})for symbol,address,size in specs]
exe=w/'fixture.exe';cmd=[str(V.VC/'bin/link.exe'),'/nologo','/out:'+str(exe),'kernel32.lib']
r=subprocess.run(cmd+[str(objects[1])],env=env,capture_output=True,text=True);assert r.returncode
r=subprocess.run(cmd+list(map(str,objects)),env=env,capture_output=True,text=True);assert not r.returncode,r.stdout+r.stderr
values=(0,0x80000000,1,0x3dcccccd,0x457a0000,0xbf800000,0x7f800000,0x7fc12345,0x7f812345)
scenarios=[]
for fill,op,offset,mode,result in itertools.product((0,1,0x55,0xff),range(2),(0x100,0x200,0x3cc0,0x4500),range(4),(0,0x80004005,0xffffffff)):
    scenarios.append((fill,op,offset,mode,result,fill%2,(0,0,0,0)))
for i,j,pair,control,mode in itertools.product(range(len(values)),range(len(values)),range(2),range(2),(0,3)):
    scenarios.append(((i+j)%256,2,0x4800,mode,(0,0x80004005,0xffffffff)[i%3],control,(values[i],values[j],0x3dcccccd if pair else 0,0x3f7d70a4 if pair else 0x3f800000)))
inputs=bytearray()
for fill,op,offset,mode,result,control,params in scenarios:
    arena=bytearray([fill])*0x5000
    struct.pack_into('<12I',arena,offset,*[values[(fill+i)%len(values)]for i in range(12)])
    struct.pack_into('<4I',arena,0x4800,*params)
    inputs.extend(struct.pack('<5I',op,offset,result,mode,control));inputs.extend(arena)
(w/'input.bin').write_bytes(inputs)
for kind in ('retail','compiled'):
    r=subprocess.run([str(exe),str(w/'input.bin'),str(w/(kind+'.bin')),kind],env=env,capture_output=True,text=True,timeout=60);assert not r.returncode,(w,r.returncode,r.stdout,r.stderr);print(r.stdout.strip(),kind,flush=True)
actual=(w/'compiled.bin').read_bytes();expected=(w/'retail.bin').read_bytes()
if actual!=expected:
    diff=next((i for i,(a,b)in enumerate(zip(actual,expected))if a!=b),min(len(actual),len(expected)));raise AssertionError((w,diff,diff//(3*(0x5000+12)+128),diff%(3*(0x5000+12)+128)))
report=dict(cases=len(scenarios),snapshots=len(scenarios)*3,omission_checks=1,providers=proof,source_sha256=hashlib.sha256(source.read_bytes()).hexdigest(),fixture_sha256=hashlib.sha256(fixture.read_bytes()).hexdigest(),boundary='Original retail bodies execute natively without address fixups. Full20KiB state plus submitted64-byte matrices; view/world raw bits and identity column, perspective near/far signed-zero/subnormal/infinity/quiet/signalingNaN pairs, two Z ranges,53/64-bit x87 precision, three HRESULT returns and mutating SetTransform callbacks. Only COM SetTransform is a boundary; no state normalized.')
(ROOT/'rebuild/compile-gate/native-renderer-matrices.json').write_text(json.dumps(report,indent=2)+'\n');print('NATIVE_RENDERER_MATRICES_GATE_PASS',report['snapshots'],'snapshots')
