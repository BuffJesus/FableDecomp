"""Native CRT layout global, virtual names and actual registered destruction."""
import json,struct,subprocess,tempfile
from pathlib import Path
import check_one as V
from gate_game_component_abi import Coff
from variable_allocator_parity import check
from gate_render_state_static_data import coff_data
import oracle_native_shader_layout_global as oracle
ROOT=Path(__file__).resolve().parents[2];w=Path(tempfile.mkdtemp(prefix='native-layout-global-'));env=V.env()
paths=['rebuild/src/compiled/01/22/Global_2DShaderConstantLayout_01224140.cpp',
       'rebuild/src/compiled/00/98/CVertexShaderConstantLayout_PeekName_0098d6f0.cpp',
       'rebuild/src/compiled/00/98/CVertexShaderConstantLayout_Lifetime_0098d490.cpp',
       'rebuild/src/compiled/00/99/CBaseClass_CBaseClass_0099a2d0.cpp',
       'rebuild/tests/integration/NativeShaderLayoutGlobal_test.cpp']
objects=[];originals=[]
for i,path in enumerate(paths):
 obj=w/('%d.obj'%i);options=['/Dfree=FableLayoutGlobalFree']if i in (0,2)else []
 r=V.cl(['/nologo','/c','/O2','/MT']+options+['/Fo'+str(obj),str(ROOT/path)],env,w);assert not r.returncode,r.stdout+r.stderr;objects.append(obj)
 if i in (0,2):
  original=w/('%d-original.obj'%i);r=V.cl(['/nologo','/c','/O2','/MT','/Fo'+str(original),str(ROOT/path)],env,w);assert not r.returncode,r.stdout+r.stderr
  left,right=Coff(original),Coff(obj)
  for name,(section,offset)in left.symbols.items():
   if not name.startswith('?') and name not in ('_$E1','_$E2','_$S3'):continue
   other_section,other_offset=right.symbols[name];lb,lr=left.sections[section];rb,rr=right.sections[other_section]
   if struct.unpack_from('<I',left.data,20+(section-1)*40+36)[0]&0x80:
    lb=coff_data(left,name,len(lb));rb=coff_data(right,name,len(rb))
   assert offset==other_offset and lb==rb and lr=={p:(n.replace('_FableLayoutGlobalFree','_free'),k)for p,(n,k)in rr.items()},name
  originals.append(original)
global_name='?Fable2DShaderConstantLayout_013ca8a8@@3VCVertexShaderConstantLayoutEmpty@@A'
targets={global_name:0x13ca8a8,'??0CVertexShaderConstantLayout@@QAE@XZ':0x98d490,
         '??1CVertexShaderConstantLayout@@UAE@XZ':0x98d710,'??_7CVertexShaderConstantLayoutEmpty@@6B@':0x129b9b0,
         '_$E2':0x122a820,'_atexit':0x4012bc,'??0CCharString@@QAE@PBDJ@Z':0x99ebf0,'??3@YAXPAX@Z':0xbfe9bc}
for path,address,text in [(originals[0],0x129b9b8,b'LayoutEmpty\0'),(objects[1],0x122d70e,b'\0')]:
 c=Coff(path);constants=[name for name,(section,offset)in c.symbols.items()if name.startswith('??_C')and c.sections[section][0][offset:]==text]
 assert len(constants)==1,constants;targets[constants[0]]=address
reports=[check(originals[0],'_$E1',0x1224140,32,6,targets),check(originals[0],'_$E2',0x122a820,10,2,targets),
         check(originals[0],'?PeekName@CVertexShaderConstantLayoutEmpty@@UAE?AVCCharString@@XZ',0x9e16b0,25,2,targets),
         check(originals[0],'??_GCVertexShaderConstantLayoutEmpty@@UAEPAXI@Z',0x9e1850,30,2,targets),
         check(objects[1],'?PeekName@CVertexShaderConstantLayout@@UAE?AVCCharString@@XZ',0x98d6f0,25,2,targets)]
c=Coff(originals[0]);section,offset=c.symbols[global_name];assert offset==0 and coff_data(c,global_name,332)==bytes(332)
section,offset=c.symbols['??_7CVertexShaderConstantLayoutEmpty@@6B@'];body,rel=c.sections[section]
assert offset==0 and len(body)==8 and rel=={0:('??_ECVertexShaderConstantLayoutEmpty@@UAEPAXI@Z',6),4:('?PeekName@CVertexShaderConstantLayoutEmpty@@UAE?AVCCharString@@XZ',6)}
section,offset=c.symbols['_$S3'];assert c.sections[section][1]=={0:('_$E1',6)}
exe=w/'fixture.exe';cmd=[str(V.VC/'bin/link.exe'),'/nologo','/out:'+str(exe),'kernel32.lib']
for omit in range(4):
 r=subprocess.run(cmd+[str(o)for i,o in enumerate(objects)if i!=omit],env=env,capture_output=True,text=True);assert r.returncode,omit
r=subprocess.run(cmd+list(map(str,objects)),env=env,capture_output=True,text=True);assert not r.returncode,r.stdout+r.stderr
r=subprocess.run([str(exe),str(w/'actual.bin')],env=env,capture_output=True,text=True,timeout=30);assert not r.returncode,(w,r.returncode,r.stdout,r.stderr)
expected,report=oracle.collect();actual=(w/'actual.bin').read_bytes()
if actual!=expected:
 (w/'expected.bin').write_bytes(expected);at=next((i for i,(a,b)in enumerate(zip(actual,expected))if a!=b),min(len(actual),len(expected)));raise AssertionError((w,at,at//356,at%356,len(actual),len(expected)))
report.update(providers=reports,omission_checks=4,global_bytes=332,
              boundary='Actual CRT initialization and atexit teardown, native base/layout lifetimes, virtual name dispatch and Empty scalar deletion flags0/1. CCharString construction and heap deallocation are recorded boundaries. Full global object compared at each observation; only vtable addresses normalized. Compiler CRT registration slot and derived vtable bindings audited.')
(ROOT/'rebuild/compile-gate/native-shader-layout-global.json').write_text(json.dumps(report,indent=2)+'\n');print(r.stdout.strip(),report['snapshots'],'global snapshots',flush=True)
