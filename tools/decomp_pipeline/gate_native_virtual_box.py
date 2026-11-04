import sys,tempfile,subprocess,json
from pathlib import Path
ROOT=Path(__file__).resolve().parents[2];sys.path.insert(0,str(ROOT/'tools/decomp_pipeline'))
import check_one,oracle_native_virtual_box as oracle
from variable_allocator_parity import check
w=Path(tempfile.mkdtemp(prefix='virtual-box-replay-'));env=check_one.env();objects=[]
sources=['rebuild/src/compiled/00/9e/Global_GFVirtualToActualScalars_009e1e10.cpp','rebuild/src/compiled/00/a0/CRenderManagerCore_SetVWindow_00a0b560.cpp','rebuild/src/compiled/00/9b/CDisplayManager_GetRenderTargetDimensions_009bedc0.cpp','rebuild/src/compiled/00/9e/Global_GFACoordToVCoord_009e21d0.cpp','rebuild/tests/integration/NativeVirtualBox_test.cpp']
sources.insert(4,'rebuild/src/compiled/00/9e/Global_GFABoxToVBox_009e2020.cpp')
for i,source in enumerate(sources):
    obj=w/('%d.obj'%i);r=check_one.cl(['/nologo','/c','/O2','/MT','/Fo'+str(obj),str(ROOT/source)],env,w);assert r.returncode==0,r.stdout+r.stderr;objects.append(obj)
targets={'?GFGetSystemManager@@YIPAVCSystemManager@@XZ':0x9a4ec0,'?GetRenderTargetDimensions@CDisplayManager@@QAE?BVC2DExtentsI@@XZ':0x9bedc0,'?g_FableVirtualWidth_013961E8@@3NA':0x13961e8,'?g_FableVirtualHeight_013961F0@@3NA':0x13961f0,'?GFVBoxToABox@@YIXPBVC2DBoxF@@PAV1@@Z':0x9e1e70,'?SetViewport@CDisplayManager@@QAEXABVC2DBoxF@@@Z':0x9bf490}
reports=[check(objects[0],'?GFVBoxToABox@@YIXPBVC2DBoxF@@PAV1@@Z',0x9e1e70,209,12,targets),check(objects[1],'?SetVWindow@CRenderManagerCore@@QAEXABVC2DBoxF@@@Z',0xa0b560,42,2,targets),check(objects[3],'?GFACoordToVCoord@@YIXABVC2DVector@@AAV1@@Z',0x9e21d0,106,6,targets)]
reports.append(check(objects[4],'?GFABoxToVBox@@YIXABVC2DBoxF@@AAV1@@Z',0x9e2020,199,12,targets))
exe=w/'fixture.exe';cmd=[str(check_one.VC/'bin/link.exe'),'/nologo','/out:'+str(exe),'kernel32.lib']
for omit in range(5):
    r=subprocess.run(cmd+[str(o)for i,o in enumerate(objects)if i!=omit],env=env,capture_output=True,text=True);assert r.returncode!=0
r=subprocess.run(cmd+list(map(str,objects)),env=env,capture_output=True,text=True);assert r.returncode==0,r.stdout+r.stderr
inputs,expected,report=oracle.collect();(w/'input.bin').write_bytes(inputs)
r=subprocess.run([str(exe),str(w/'input.bin'),str(w/'actual.bin')],env=env,capture_output=True,text=True,timeout=30);assert r.returncode==0,(r.returncode,r.stdout,r.stderr,w)
actual=(w/'actual.bin').read_bytes()
if actual!=expected:
    (w/'expected.bin').write_bytes(expected);diff=next((i for i,(a,b)in enumerate(zip(actual,expected))if a!=b),min(len(actual),len(expected)))
    raise AssertionError((w,diff,diff//(oracle.WIDTH+16),diff%(oracle.WIDTH+16),len(actual),len(expected)))
report.update(providers=reports,omission_checks=5,boundary='Native actual-to-virtual coordinates and boxes, virtual-to-actual boxes, render target dimensions and SetVWindow. System getter/float viewport endpoint recorded; aliased/overlapping source/output and getter mutation under four x87 rounding modes match retail byte for byte. Vertical actual-to-virtual box numerators retain the retail float-rounding boundary.')
(ROOT/'rebuild/compile-gate/native-virtual-box.json').write_text(json.dumps(report,indent=2)+'\n');print(r.stdout.strip(),report['snapshots'],'full-state snapshots')
