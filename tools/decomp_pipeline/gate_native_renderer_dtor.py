"""Implicit renderer teardown and actual vector cleanup; full retail replay."""
import hashlib,json,struct,subprocess,tempfile,sys
from pathlib import Path
import check_one as V
from gate_game_component_abi import Coff
from variable_allocator_parity import check
from renderer_ctor_targets import targets as ctor_targets
import oracle_native_renderer_dtor as oracle
ROOT=Path(__file__).resolve().parents[2];w=Path(tempfile.mkdtemp(prefix='native-renderer-dtor-'));env=V.env()
source=ROOT/'rebuild/src/compiled/00/9d/CRenderManager2D_Constructor_009d9a30.cpp';fixture=ROOT/'rebuild/tests/integration/NativeRendererDtor_test.cpp'
dtor='??1CRenderManager2D@@UAE@XZ';deleting='??_GCRenderManager2D@@UAEPAXI@Z';vector='??1?$vector@VCQuickDrawTriInfo@@V?$allocator@VCQuickDrawTriInfo@@@_STL@@@_STL@@QAE@XZ'
derived='--derived'in sys.argv
if derived:
 from display_init_oracle import TARGETS as ctor_targets
 source=ROOT/'rebuild/src/compiled/00/9c/CDisplayManager_Init_009c0e50.cpp';deleting='??_GCRenderManager@@UAEPAXI@Z'
deleting_address=0x9c3100 if derived else 0x9e16e0
targets={dtor:0x9c2fd0,vector:0x9c2f50,'_free':0xbfea14,'??_7CVertexShader@@6B@':0x129b3d0,'??_7CShaderBase@@6B@':0x129a030,'??3@YAXPAX@Z':0xbfe9bc,'??1CShaderDataBank@@UAE@XZ':0xa620d0,'??1CRenderManagerCore@@UAE@XZ':0xa0b060,'?Uninitialise@CTexture@@QAEXXZ':0x9f9f70,'??1CWideString@@QAE@XZ':0x99b510}
objects=[]
for name,extra in [('production',[]),('replay',['/Dfree=FableRendererFree'])]:
 obj=w/(name+'.obj');r=V.cl(['/nologo','/c','/O2','/MT','/Fo'+str(obj),str(source)]+extra,env,w);assert not r.returncode,r.stdout+r.stderr;objects.append(obj)
proof=[check(objects[0],s,a,n,k,targets)for s,a,n,k in [(dtor,0x9c2fd0,217,11),(vector,0x9c2f50,58,3),(deleting,deleting_address,30,2)]]
a,b=Coff(objects[0]),Coff(objects[1])
for name in [dtor,vector,deleting]:
 sec,off=a.symbols[name];other,otheroff=b.symbols[name];body,rel=a.sections[sec];otherbody,otherrel=b.sections[other]
 assert body==otherbody and off==otheroff and {p:('_FableRendererFree'if t=='_free'else t,k)for p,(t,k)in rel.items()}==otherrel
a.table('??_7CRenderManager@@6B@'if derived else '??_7CRenderManager2D@@6B@',['??_ECRenderManager@@UAEPAXI@Z'if derived else '??_ECRenderManager2D@@UAEPAXI@Z'])
aliases={'@DestroyRenderer@4':dtor,'@DeleteRenderer@12':deleting,'@DestroyRecords@4':vector,'_VertexTable':'??_7CVertexShader@@6B@','_ShaderTable':'??_7CShaderBase@@6B@','?Uninitialise@CTexture@@QAEXXZ':'@TextureBoundary@4','??1CWideString@@QAE@XZ':'@WideBoundary@4','??1CShaderDataBank@@UAE@XZ':'@BankBoundary@4','??1CRenderManagerCore@@UAE@XZ':'@CoreBoundary@4'}
for name in ctor_targets:
 if name not in a.symbols and name not in aliases and not name.startswith('_') and '@@3'not in name and name!='??3@YAXPAX@Z':aliases[name]='_UnusedRendererDtorBoundary'
for name in ['?CountReferences@CVertexShader@@EAEKXZ','?GetDebugName@CShaderBase@@MBE?AVCCharString@@XZ']:aliases[name]='_UnusedRendererDtorBoundary'
if derived:aliases['_Direct3DCreate9@4']='_UnusedRendererDtorBoundary'
(w/'renderer_dtor_aliases.h').write_text('\n'.join('#pragma comment(linker,"/alternatename:'+s+'='+t+'")'for s,t in aliases.items())+'\n')
obj=w/'fixture.obj';r=V.cl(['/nologo','/c','/O2','/MT','/I'+str(w),'/Fo'+str(obj),str(fixture)],env,w);assert not r.returncode,r.stdout+r.stderr
strings=bytearray();symbols=bytearray();globals_={name:value for name,value in ctor_targets.items()if '@@3'in name}
for name,value in globals_.items():symbols.extend(struct.pack('<2IIhHBB',0,4+len(strings),value,-1,0,2,0));strings.extend(name.encode()+b'\0')
bindings=w/'globals.obj';bindings.write_bytes(struct.pack('<HHIIIHH',0x14c,0,0,20,len(globals_),0,0)+symbols+struct.pack('<I',4+len(strings))+strings)
exe=w/'fixture.exe';cmd=[str(V.VC/'bin/link.exe'),'/nologo','/fixed','/out:'+str(exe),'kernel32.lib',str(obj),str(bindings)]
r=subprocess.run(cmd,env=env,capture_output=True,text=True);assert r.returncode,'renderer provider required'
r=subprocess.run(cmd+[str(objects[1])],env=env,capture_output=True,text=True);assert not r.returncode,r.stdout+r.stderr
inputs,expected,report=oracle.collect(deleting_address);(w/'input.bin').write_bytes(inputs)
r=subprocess.run([str(exe),str(w/'input.bin'),str(w/'actual.bin')],env=env,capture_output=True,text=True,timeout=60);assert not r.returncode,(w,r.returncode,r.stdout,r.stderr)
actual=(w/'actual.bin').read_bytes()
if actual!=expected:
 (w/'expected.bin').write_bytes(expected);difference=next((i for i,(a,b)in enumerate(zip(actual,expected))if a!=b),min(len(actual),len(expected)));raise AssertionError((w,difference,difference//(oracle.WIDTH+12),difference%(oracle.WIDTH+12),len(actual),len(expected)))
report.update(providers=proof,omission_checks=1,compared_bytes=len(actual),source_sha256=hashlib.sha256(source.read_bytes()).hexdigest(),fixture_sha256=hashlib.sha256(fixture.read_bytes()).hexdigest(),boundary='Actual217-byte implicit renderer destructor,58-byte record-vector destructor and30-byte deleting destructor. Full24KiB snapshots; vertex/base vtables mapped to retail identities, two callback pointers normalized. Empty/nonempty vectors, shader/control ownership, delete flag and callbacks mutating vector storage/resource ownership. Texture/string cleanup, shader virtual deletion, bank/core teardown and CRT free/delete are recorded boundaries. No GPU teardown claim.')
(ROOT/('rebuild/compile-gate/native-renderer-dtor'+('-derived'if derived else '')+'.json')).write_text(json.dumps(report,indent=2)+'\n');print(r.stdout.strip(),report['snapshots'],'snapshots', 'derived'if derived else '')
