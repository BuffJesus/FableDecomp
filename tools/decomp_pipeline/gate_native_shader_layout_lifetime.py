"""Shader-layout lifetimes with actual range/base destruction and heap boundaries."""
import json,subprocess,tempfile
from pathlib import Path
import check_one as V
from gate_game_component_abi import Coff
from variable_allocator_parity import check
import oracle_native_shader_layout_lifetime as oracle
ROOT=Path(__file__).resolve().parents[2];w=Path(tempfile.mkdtemp(prefix='native-shader-layout-lifetime-'));env=V.env()
source=ROOT/'rebuild/src/compiled/00/98/CVertexShaderConstantLayout_Lifetime_0098d490.cpp'
paths=[source,source,ROOT/'rebuild/src/compiled/00/99/CBaseClass_CBaseClass_0099a2d0.cpp',ROOT/'rebuild/tests/integration/NativeShaderLayoutLifetime_test.cpp'];objects=[]
for i,path in enumerate(paths):
 obj=w/('%d.obj'%i);options=['/Dfree=FableLayoutFixtureFree']if i==1 else []
 r=V.cl(['/nologo','/c','/O2','/MT']+options+['/Fo'+str(obj),str(path)],env,w);assert not r.returncode,r.stdout+r.stderr;objects.append(obj)
targets={'??0CBaseClass@@QAE@XZ':0x99a2d0,'??1CBaseClass@@UAE@XZ':0x99a2e0,
         '??_7CVertexShaderConstantLayout@@6B@':0x129a06c,'??_7CShaderConstantRange@@6B@':0x129a04c,
         '??1CVertexShaderConstantLayout@@UAE@XZ':0x98d710,'??3@YAXPAX@Z':0xbfe9bc,'_free':0xbfea14}
specs=[('??0CVertexShaderConstantLayout@@QAE@XZ',0x98d490,603,48),('??1CVertexShaderConstantLayout@@UAE@XZ',0x98d710,270,25),
       ('??_GCVertexShaderConstantLayout@@UAEPAXI@Z',0x98d990,30,2),('??_GCShaderConstantRange@@UAEPAXI@Z',0x98c390,30,2)]
reports=[check(objects[0],symbol,address,length,count,targets)for symbol,address,length,count in specs]
original,renamed=Coff(objects[0]),Coff(objects[1]);audited=[]
for symbol,(section,offset)in original.symbols.items():
 if not symbol.startswith('?'):continue
 other_section,other_offset=renamed.symbols[symbol];left,lr=original.sections[section];right,rr=renamed.sections[other_section]
 assert offset==other_offset and left==right
 assert {p:(n.replace('_FableLayoutFixtureFree','_free'),k)for p,(n,k)in rr.items()}==lr
 audited.append(symbol)
exe=w/'fixture.exe';cmd=[str(V.VC/'bin/link.exe'),'/nologo','/out:'+str(exe),'kernel32.lib']
for omit in (1,2):
 r=subprocess.run(cmd+[str(o)for i,o in enumerate(objects)if i and i!=omit],env=env,capture_output=True,text=True);assert r.returncode,omit
r=subprocess.run(cmd+list(map(str,objects[1:])),env=env,capture_output=True,text=True);assert not r.returncode,r.stdout+r.stderr
inputs,expected,report=oracle.collect();(w/'input.bin').write_bytes(inputs)
r=subprocess.run([str(exe),str(w/'input.bin'),str(w/'actual.bin')],env=env,capture_output=True,text=True,timeout=60);assert not r.returncode,(w,r.returncode,r.stdout,r.stderr)
actual=(w/'actual.bin').read_bytes()
if actual!=expected:
 (w/'expected.bin').write_bytes(expected);at=next((i for i,(a,b)in enumerate(zip(actual,expected))if a!=b),min(len(actual),len(expected)))
 raise AssertionError((w,at,at//4104,at%4104))
report.update(providers=reports,omission_checks=2,allocator_rename_audited_symbols=audited,
              boundary='Actual native constructor, compiler-generated layout/range destructors and scalar deleting wrappers, with native CBaseClass lifetime. Full4KiB arena at each heap callback and return. Vtable addresses normalized only in known polymorphic subobjects. Empty/nonempty presets, deleting flags0/1, all26 range subobjects and mutating free callbacks. Virtual PeekName is not invoked.')
(ROOT/'rebuild/compile-gate/native-shader-layout-lifetime.json').write_text(json.dumps(report,indent=2)+'\n');print(r.stdout.strip(),report['snapshots'],'full-state snapshots',flush=True)
