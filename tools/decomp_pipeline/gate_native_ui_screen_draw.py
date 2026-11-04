"""Gate native UI map traversal, including callback mutation and real iterator."""
import json
import subprocess
import tempfile
from pathlib import Path
import check_one as V
import oracle_native_ui_screen_draw as oracle
from variable_allocator_parity import check

ROOT = Path(__file__).resolve().parents[2]
w = Path(tempfile.mkdtemp(prefix='native-ui-screen-draw-')); env=V.env(); objects=[]
for i, source in enumerate(['rebuild/src/compiled/00/59/CFrontEndManager_Draw_00595222.cpp',
                            'rebuild/tests/integration/NativeUIScreenDraw_test.cpp']):
    obj=w/('%d.obj'%i)
    r=V.cl(['/nologo','/c','/O2','/MT','/Fo'+str(obj),str(ROOT/source)],env,w)
    assert r.returncode == 0, r.stdout+r.stderr
    objects.append(obj)
iterator='?_M_increment@?$_Rb_global@_N@_STL@@SAPAU_Rb_tree_node_base@2@PAU32@@Z'
reports=[check(objects[0],'?Draw@CFrontEndManager@NUISystem@@QAEXAAVCIEngine@@PAVCEnginePrimitiveHandle@@@Z',
               0x595222,59,1,{iterator:0x4292c0}),check(objects[0],iterator,0x4292c0,44,0,{})]
exe=w/'fixture.exe'; cmd=[str(V.VC/'bin/link.exe'),'/nologo','/out:'+str(exe),'kernel32.lib']
r=subprocess.run(cmd+[str(objects[1])],env=env,capture_output=True,text=True);assert r.returncode != 0
r=subprocess.run(cmd+list(map(str,objects)),env=env,capture_output=True,text=True);assert r.returncode == 0,r.stdout+r.stderr
inputs,expected,report=oracle.collect();(w/'input.bin').write_bytes(inputs)
r=subprocess.run([str(exe),str(w/'input.bin'),str(w/'actual.bin')],env=env,capture_output=True,text=True,timeout=30)
assert r.returncode == 0,(r.returncode,r.stdout,r.stderr,w)
actual=(w/'actual.bin').read_bytes()
assert actual == expected,(w,len(actual),len(expected))
report.update(providers=reports,omission_checks=1,boundary='Actual native UI screen map traversal and STLport increment; component virtual Draw callbacks recorded. Null entries, three tree shapes, changing next component and changing sentinel checked. No displayed frame.')
(ROOT/'rebuild/compile-gate/native-ui-screen-draw.json').write_text(json.dumps(report,indent=2)+'\n')
print(r.stdout.strip(),report['snapshots'],'full-state snapshots')
