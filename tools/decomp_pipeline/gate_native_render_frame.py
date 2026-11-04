import sys,tempfile,subprocess,json
from pathlib import Path
ROOT=Path(__file__).resolve().parents[2];sys.path.insert(0,str(ROOT/'tools/decomp_pipeline'))
import check_one
from variable_allocator_parity import check
w=Path(tempfile.mkdtemp(prefix='native-render-frame-'));env=check_one.env();objects=[]
for i,source in enumerate(['rebuild/src/compiled/00/9b/CDisplayManager_NativeRender_009bef20.cpp','rebuild/src/compiled/00/a0/CRenderManagerCore_Frame_00a0b480.cpp','rebuild/tests/integration/NativeRenderFrame_test.cpp']):
    obj=w/('%d.obj'%i);r=check_one.cl(['/nologo','/c','/O2','/MT','/Fo'+str(obj),str(ROOT/source)],env,w)
    assert r.returncode==0,r.stdout+r.stderr;objects.append(obj)
targets={'?BeginRender@CRenderManagerCore@@QAE_NXZ':0xa0b480,'?EndRender@CRenderManagerCore@@QAEXXZ':0xa0b520,'?GFGetTime@@YINXZ':0x9e1bc0,'__ftol2':0xbfea70}
specs=[(0,'?BeginRender@CDisplayManager@@QAE_NXZ',0x9bef20,41,1),(0,'?EndRender@CDisplayManager@@QAEXXZ',0x9bef50,43,1),(1,'?BeginRender@CRenderManagerCore@@QAE_NXZ',0xa0b480,146,1),(1,'?EndRender@CRenderManagerCore@@QAEXXZ',0xa0b520,63,2)]
reports=[check(objects[i],s,a,n,c,targets)for i,s,a,n,c in specs]
exe=w/'fixture.exe';cmd=[str(check_one.VC/'bin/link.exe'),'/nologo','/out:'+str(exe)]
for omit in (0,1):
    r=subprocess.run(cmd+[str(o)for i,o in enumerate(objects)if i!=omit],env=env,capture_output=True,text=True);assert r.returncode!=0
r=subprocess.run(cmd+list(map(str,objects)),env=env,capture_output=True,text=True);assert r.returncode==0,r.stdout+r.stderr
r=subprocess.run([str(exe)],env=env,capture_output=True,text=True,timeout=30);assert r.returncode==0 and 'NATIVE_RENDER_FRAME_PASS cases=180'in r.stdout,(r.returncode,r.stdout,r.stderr,w)
(ROOT/'rebuild/compile-gate/native-render-frame.json').write_text(json.dumps(dict(providers=reports,cases=180,omission_checks=2,boundary='Actual display/core begin/end composition; COM calls and clock recorded. Full core image, success/failure, stream mutation, reentrant device replacement, signed/zero duration and x87 integer conversion checked.'),indent=2)+'\n');print(r.stdout.strip())
