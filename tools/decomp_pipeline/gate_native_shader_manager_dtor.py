"""Composed native shader-manager teardown and emitted member helpers."""
import hashlib,json,subprocess,tempfile
from pathlib import Path
import check_one as V
from gate_game_component_abi import Coff
from variable_allocator_parity import check
import oracle_native_shader_manager_dtor as oracle
ROOT=Path(__file__).resolve().parents[2];w=Path(tempfile.mkdtemp(prefix='native-shader-manager-dtor-'));env=V.env()
source=ROOT/'rebuild/src/compiled/00/98/CShaderRenderManager_Destructor_0098abf0.cpp';fixture=ROOT/'rebuild/tests/integration/NativeShaderManagerDtor_test.cpp'
dtor='??1CShaderRenderManager@@UAE@XZ';scalar='??_GCShaderRenderManager@@UAEPAXI@Z';indexed='??1?$CIndexedArray@VCCharString@@@@QAE@XZ';pixel='??_GCPixelShader@@UAEPAXI@Z'
erase='?_M_erase@?$_Rb_tree@VCCharString@@U?$pair@$$CBVCCharString@@J@_STL@@U?$_Select1st@U?$pair@$$CBVCCharString@@J@_STL@@@3@U?$less@VCCharString@@@3@V?$allocator@U?$pair@$$CBVCCharString@@J@_STL@@@3@@_STL@@QAEXPAU?$_Rb_tree_node@U?$pair@$$CBVCCharString@@J@_STL@@@2@@Z'
destroy='??$__destroy_aux@PAV?$CCountedPointer@VCShaderResource@@@@@_STL@@YIXPAV?$CCountedPointer@VCShaderResource@@@@0ABU__false_type@0@@Z'
tables={'Manager':('CShaderRenderManager',0x129a080),'Pixel':('CPixelShader',0x129a054),'Shader':('CShaderBase',0x129a030),'Pool':('CSimpleMemoryPool',0x129a064),'Layout':('CVertexShaderConstantLayout',0x129a06c),'Range':('CShaderConstantRange',0x129a04c),'Base':('CBaseClass',0x1231710),'NonCopy':('CBaseClassNonCopyable',0x129a7c4)}
targets={dtor:0x98abf0,indexed:0x98d9f0,erase:0x43020a,destroy:0x98cee0,'?Uninitialise@CShaderRenderManager@@QAEXXZ':0x98a7a0,'_free':0xbfea14,'??3@YAXPAX@Z':0xbfe9bc,'??1CCharString@@QAE@XZ':0x99eae0,'??1CSimpleMemoryPool@@UAE@XZ':0x987f10,'??1CVertexShaderConstantLayout@@UAE@XZ':0x98d710,'??1CBaseClassNonCopyable@@UAE@XZ':0x99a300}
aliases={'@DeletePixel@12':pixel,'@DestroyManager@4':dtor,'@DeleteManager@12':scalar,'?Uninitialise@CShaderRenderManager@@QAEXXZ':'@UninitialiseBoundary@4',erase:'@EraseTreeBoundary@12','??1CCharString@@QAE@XZ':'@StringDtorBoundary@4'}
for label,(cls,address)in tables.items():
 symbol='??_7'+cls+'@@6B@';aliases['_'+label+'Table']=symbol;targets[symbol]=address
for name in ['?GetDebugName@CShaderBase@@MBE?AVCCharString@@XZ','?CountReferences@CPixelShader@@EAEKXZ','?PeekName@CVertexShaderConstantLayout@@UAE?AVCCharString@@XZ']:aliases[name]='_UnusedShaderManagerDtorBoundary'
(w/'shader_manager_dtor_aliases.h').write_text('\n'.join('#pragma comment(linker,"/alternatename:'+a+'='+b+'")'for a,b in aliases.items())+'\n')
objects=[]
paths=[source,source,ROOT/'rebuild/src/compiled/00/a5/CSimpleMemoryPool_Lifetime_00a5c570.cpp',ROOT/'rebuild/src/compiled/00/99/CBaseClass_CBaseClass_0099a2d0.cpp',fixture]
for i,path in enumerate(paths):
 obj=w/(str(i)+'.obj');r=V.cl(['/nologo','/c','/O2','/MT','/I'+str(w),'/Fo'+str(obj),str(path)]+(['/Dfree=FableManagerFree']if i==1 else []),env,w);assert not r.returncode,r.stdout+r.stderr;objects.append(obj)
a,b=Coff(objects[0]),Coff(objects[1]);proof=[]
for symbol,address,length in [(dtor,0x98abf0,246),(scalar,0x98de90,30),(indexed,0x98d9f0,113),(pixel,0x98c3d0,55)]:
 sec,_=a.symbols[symbol];proof.append(check(objects[0],symbol,address,length,len(a.sections[sec][1]),targets))
for symbol,(sec,off)in a.symbols.items():
 if not symbol.startswith('?'):continue
 other,otheroff=b.symbols[symbol];body,rel=a.sections[sec];otherbody,otherrel=b.sections[other]
 assert body==otherbody and off==otheroff and {p:('_FableManagerFree'if t=='_free'else t,k)for p,(t,k)in rel.items()}==otherrel
a.table('??_7CShaderRenderManager@@6B@',['??_ECShaderRenderManager@@UAEPAXI@Z'])
exe=w/'fixture.exe';cmd=[str(V.VC/'bin/link.exe'),'/nologo','/out:'+str(exe),'kernel32.lib']
for omit in [1,2,3]:
 r=subprocess.run(cmd+[str(o)for i,o in enumerate(objects)if i and i!=omit],env=env,capture_output=True,text=True);assert r.returncode,('required provider',omit)
r=subprocess.run(cmd+list(map(str,objects[1:])),env=env,capture_output=True,text=True);assert not r.returncode,r.stdout+r.stderr
inputs,expected,report=oracle.collect();(w/'input.bin').write_bytes(inputs)
r=subprocess.run([str(exe),str(w/'input.bin'),str(w/'actual.bin')],env=env,capture_output=True,text=True,timeout=60);assert not r.returncode,(w,r.returncode,r.stdout,r.stderr)
actual=(w/'actual.bin').read_bytes()
if actual!=expected:
 (w/'expected.bin').write_bytes(expected);difference=next((i for i,(a,b)in enumerate(zip(actual,expected))if a!=b),min(len(actual),len(expected)));raise AssertionError((w,difference,difference//(oracle.WIDTH+16),difference%(oracle.WIDTH+16),len(actual),len(expected)))
report.update(providers=proof,omission_checks=3,compared_bytes=len(actual),source_sha256=hashlib.sha256(source.read_bytes()).hexdigest(),fixture_sha256=hashlib.sha256(fixture.read_bytes()).hexdigest(),boundary='Actual manager, indexed-array, counted resource vector, pixel value, pool, layout/range and base destruction. Full16KiB snapshots; Uninitialise, map erase, string destruction, virtual deletion and heap are recorded boundaries. Vtable and control callback identities mapped, no masked state bytes.')
(ROOT/'rebuild/compile-gate/native-shader-manager-dtor.json').write_text(json.dumps(report,indent=2)+'\n');print(r.stdout.strip(),report['snapshots'],'snapshots')
