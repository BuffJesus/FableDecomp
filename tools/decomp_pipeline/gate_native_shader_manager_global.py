"""Native shader manager global: real CRT construction and registered teardown."""
import json,struct,subprocess,tempfile
from pathlib import Path
import check_one as V
from gate_game_component_abi import Coff
from gate_render_state_static_data import coff_data
from variable_allocator_parity import check
import oracle_native_shader_manager_global as oracle
ROOT=Path(__file__).resolve().parents[2];w=Path(tempfile.mkdtemp(prefix='native-shader-manager-global-'));env=V.env()
global_name='?FableShaderRenderManager_013bc470@@3VCShaderRenderManager@@A'
aliases={'_ManagerGlobal':global_name}
for label,cls in [('Manager','CShaderRenderManager'),('Pixel','CPixelShader'),('Shader','CShaderBase'),('Pool','CSimpleMemoryPool'),('Layout','CVertexShaderConstantLayout'),('Range','CShaderConstantRange'),('Base','CBaseClass'),('NonCopy','CBaseClassNonCopyable')]:aliases['_'+label+'Table']='??_7'+cls+'@@6B@'
for name in ['?GetDebugName@CShaderBase@@MBE?AVCCharString@@XZ','?CountReferences@CPixelShader@@EAEKXZ','?PeekName@CVertexShaderConstantLayout@@UAE?AVCCharString@@XZ','??1CCharString@@QAE@XZ','?Uninitialise@CShaderResource@@AAEXXZ','?_M_erase@?$_Rb_tree@VCCharString@@U?$pair@$$CBVCCharString@@J@_STL@@U?$_Select1st@U?$pair@$$CBVCCharString@@J@_STL@@@3@U?$less@VCCharString@@@3@V?$allocator@U?$pair@$$CBVCCharString@@J@_STL@@@3@@_STL@@QAEXPAU?$_Rb_tree_node@U?$pair@$$CBVCCharString@@J@_STL@@@2@@Z']:aliases[name]='_UnusedShaderManagerGlobalBoundary'
(w/'shader_manager_global_aliases.h').write_text('\n'.join('#pragma comment(linker,"/alternatename:'+a+'='+b+'")'for a,b in aliases.items())+'\n')
paths=['rebuild/src/compiled/01/22/Global_ShaderRenderManager_01223d00.cpp','rebuild/src/compiled/00/98/CShaderRenderManager_Constructor_0098ab00.cpp','rebuild/src/compiled/00/98/CShaderRenderManager_Destructor_0098abf0.cpp','rebuild/src/compiled/00/a5/CSimpleMemoryPool_Lifetime_00a5c570.cpp','rebuild/src/compiled/00/98/CVertexShaderConstantLayout_Lifetime_0098d490.cpp','rebuild/src/compiled/00/99/CBaseClass_CBaseClass_0099a2d0.cpp','rebuild/src/compiled/00/98/CShaderRenderManager_Uninitialise_0098a7a0.cpp','rebuild/tests/integration/NativeShaderManagerGlobal_test.cpp']
objects=[];originals=[]
for i,path in enumerate(paths):
 obj=w/(str(i)+'.obj');options=['/Dfree=FableGlobalFree','/Dmalloc=FableGlobalMalloc']if i<7 else []
 r=V.cl(['/nologo','/c','/O2','/MT','/I'+str(w),'/Fo'+str(obj),str(ROOT/path)]+options,env,w);assert not r.returncode,r.stdout+r.stderr;objects.append(obj)
 if i<7:
  original=w/(str(i)+'-original.obj');r=V.cl(['/nologo','/c','/O2','/MT','/Fo'+str(original),str(ROOT/path)],env,w);assert not r.returncode,r.stdout+r.stderr;originals.append(original)
  a,b=Coff(original),Coff(obj)
  for symbol,(sec,off)in a.symbols.items():
   if not symbol.startswith('?')and symbol not in ['_$E1','_$E2','_$S3']:continue
   other,otheroff=b.symbols[symbol];body,rel=a.sections[sec];otherbody,otherrel=b.sections[other]
   if struct.unpack_from('<I',a.data,20+(sec-1)*40+36)[0]&0x80:body=coff_data(a,symbol,len(body));otherbody=coff_data(b,symbol,len(otherbody))
   assert body==otherbody and off==otheroff and {p:(t.replace('_free','_FableGlobalFree').replace('_malloc','_FableGlobalMalloc'),k)for p,(t,k)in rel.items()}==otherrel,symbol
targets={global_name:0x13bc470,'??0CShaderRenderManager@@QAE@XZ':0x98ab00,'??1CShaderRenderManager@@UAE@XZ':0x98abf0,'_$E2':0x122a4c0,'_atexit':0x4012bc}
proof=[check(originals[0],'_$E1',0x1223d00,22,4,targets),check(originals[0],'_$E2',0x122a4c0,10,2,targets)]
c=Coff(originals[0]);sec,off=c.symbols[global_name];assert off==0 and c.section_metadata[sec]['raw_size']==1000 and coff_data(c,global_name,1000)==bytes(1000)
sec,off=c.symbols['_$S3'];assert c.sections[sec][1]=={0:('_$E1',6)}
exe=w/'fixture.exe';cmd=[str(V.VC/'bin/link.exe'),'/nologo','/out:'+str(exe),'kernel32.lib']
for omit in range(7):
 r=subprocess.run(cmd+[str(o)for i,o in enumerate(objects)if i!=omit],env=env,capture_output=True,text=True);assert r.returncode,('required provider',omit)
r=subprocess.run(cmd+list(map(str,objects)),env=env,capture_output=True,text=True);assert not r.returncode,r.stdout+r.stderr
r=subprocess.run([str(exe),str(w/'actual.bin')],env=env,capture_output=True,text=True,timeout=30);assert not r.returncode,(w,r.returncode,r.stdout,r.stderr)
expected,report=oracle.collect();actual=(w/'actual.bin').read_bytes()
if actual!=expected:
 (w/'expected.bin').write_bytes(expected);difference=next((i for i,(a,b)in enumerate(zip(actual,expected))if a!=b),min(len(actual),len(expected)));raise AssertionError((w,difference,difference//1032,difference%1032,len(actual),len(expected)))
report.update(providers=proof,global_bytes=1000,omission_checks=7,boundary='Real C++ global, CRT initialization and atexit destruction. Actual manager constructor, destructor, empty shutdown, layout, pool and base lifetimes. Full1000-byte object and24-byte map header compared before allocation, before main, before exit, at each free and after registered teardown. Compiled vtable identities mapped; heap boundaries only. Nonempty resource shutdown is covered by the separate shutdown gate.')
(ROOT/'rebuild/compile-gate/native-shader-manager-global.json').write_text(json.dumps(report,indent=2)+'\n');print(r.stdout.strip(),report['snapshots'],'global snapshots')
