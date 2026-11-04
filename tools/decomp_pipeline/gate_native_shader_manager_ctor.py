"""Native full-size shader-manager construction and its inline pixel-shader value."""
import hashlib,json,subprocess,tempfile
from pathlib import Path
import check_one as V
from gate_game_component_abi import Coff
from variable_allocator_parity import check
import oracle_native_shader_manager_ctor as oracle
ROOT=Path(__file__).resolve().parents[2];w=Path(tempfile.mkdtemp(prefix='native-shader-manager-ctor-'));env=V.env()
source=ROOT/'rebuild/src/compiled/00/98/CShaderRenderManager_Constructor_0098ab00.cpp';fixture=ROOT/'rebuild/tests/integration/NativeShaderManagerCtor_test.cpp'
ctor='??0CShaderRenderManager@@QAE@XZ';pixel='??0CPixelShader@@QAE@XZ'
targets={'??0CBaseClassNonCopyable@@QAE@XZ':0x99a2f0,'??_7CShaderRenderManager@@6B@':0x129a080,'??0CVertexShaderConstantLayout@@QAE@XZ':0x98d490,'??0CSimpleMemoryPool@@QAE@KK@Z':0xa5c570,'??_7CPixelShader@@6B@':0x129a054,'_malloc':0xbfea0e}
aliases={'@ConstructShaderManager@4':ctor,'@ConstructPixelShader@4':pixel,'_ManagerTable':'??_7CShaderRenderManager@@6B@','_PixelTable':'??_7CPixelShader@@6B@','??0CBaseClassNonCopyable@@QAE@XZ':'@BaseCtorBoundary@4','??0CVertexShaderConstantLayout@@QAE@XZ':'@LayoutCtorBoundary@4','??0CSimpleMemoryPool@@QAE@KK@Z':'@PoolCtorBoundary@16'}
for name in ['??1CBaseClass@@UAE@XZ','??1CCharString@@QAE@XZ','?_M_erase@?$_Rb_tree@VCCharString@@U?$pair@$$CBVCCharString@@J@_STL@@U?$_Select1st@U?$pair@$$CBVCCharString@@J@_STL@@@3@U?$less@VCCharString@@@3@V?$allocator@U?$pair@$$CBVCCharString@@J@_STL@@@3@@_STL@@QAEXPAU?$_Rb_tree_node@U?$pair@$$CBVCCharString@@J@_STL@@@2@@Z','??1CShaderRenderManager@@UAE@XZ','?GetDebugName@CShaderBase@@MBE?AVCCharString@@XZ','?CountReferences@CPixelShader@@EAEKXZ']:aliases[name]='_UnusedShaderManagerCtorBoundary'
(w/'shader_manager_ctor_aliases.h').write_text('\n'.join('#pragma comment(linker,"/alternatename:'+a+'='+b+'")'for a,b in aliases.items())+'\n')
objects=[]
for name,path,extra in [('production',source,[]),('replay',source,['/Dmalloc=FableManagerMalloc']),('fixture',fixture,[])]:
 obj=w/(name+'.obj');r=V.cl(['/nologo','/c','/O2','/MT','/I'+str(w),'/Fo'+str(obj),str(path)]+extra,env,w);assert not r.returncode,r.stdout+r.stderr;objects.append(obj)
proof=[check(objects[0],ctor,0x98ab00,232,6,targets),check(objects[0],pixel,0xa5ec00,16,1,targets)]
a,b=Coff(objects[0]),Coff(objects[1])
for name in [ctor,pixel]:
 sec,off=a.symbols[name];other,otheroff=b.symbols[name];body,rel=a.sections[sec];otherbody,otherrel=b.sections[other]
 assert body==otherbody and off==otheroff and {p:('_FableManagerMalloc'if t=='_malloc'else t,k)for p,(t,k)in rel.items()}==otherrel
a.table('??_7CShaderRenderManager@@6B@',['??_ECShaderRenderManager@@UAEPAXI@Z'])
a.table('??_7CPixelShader@@6B@',['??_ECPixelShader@@UAEPAXI@Z','?GetDebugName@CShaderBase@@MBE?AVCCharString@@XZ','?CountReferences@CPixelShader@@EAEKXZ'])
exe=w/'fixture.exe';cmd=[str(V.VC/'bin/link.exe'),'/nologo','/out:'+str(exe),'kernel32.lib']
r=subprocess.run(cmd+[str(objects[2])],env=env,capture_output=True,text=True);assert r.returncode,'native construction required'
r=subprocess.run(cmd+list(map(str,objects[1:])),env=env,capture_output=True,text=True);assert not r.returncode,r.stdout+r.stderr
inputs,expected,report=oracle.collect();(w/'input.bin').write_bytes(inputs)
r=subprocess.run([str(exe),str(w/'input.bin'),str(w/'actual.bin')],env=env,capture_output=True,text=True,timeout=60);assert not r.returncode,(w,r.returncode,r.stdout,r.stderr)
actual=(w/'actual.bin').read_bytes()
if actual!=expected:
 (w/'expected.bin').write_bytes(expected);difference=next((i for i,(a,b)in enumerate(zip(actual,expected))if a!=b),min(len(actual),len(expected)));raise AssertionError((w,difference,difference//(oracle.WIDTH+16),difference%(oracle.WIDTH+16),len(actual),len(expected)))
report.update(providers=proof,omission_checks=1,compared_bytes=len(actual),source_sha256=hashlib.sha256(source.read_bytes()).hexdigest(),fixture_sha256=hashlib.sha256(fixture.read_bytes()).hexdigest(),boundary='Full1000-byte manager with typed member lifetimes and actual pixel-shader construction. Full4KiB including map header allocation across256fills and constructor/allocator mutations; only compiled vtable identities mapped to retail addresses. Layout and pool constructors plus base and malloc are recorded boundaries. Bone-matrix vector tail is verified. No allocation-failure recovery or global startup claim.')
(ROOT/'rebuild/compile-gate/native-shader-manager-ctor.json').write_text(json.dumps(report,indent=2)+'\n');print(r.stdout.strip(),report['snapshots'],'snapshots')
