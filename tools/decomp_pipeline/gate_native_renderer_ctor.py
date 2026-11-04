"""Audit the full renderer constructor and replay recorded native dependencies."""
import hashlib,json,struct,subprocess,tempfile
from pathlib import Path
import check_one as V
from gate_game_component_abi import Coff
from variable_allocator_parity import check
from renderer_ctor_targets import targets
import oracle_native_renderer_ctor as oracle
ROOT=Path(__file__).resolve().parents[2];w=Path(tempfile.mkdtemp(prefix='native-renderer-ctor-'));env=V.env()
# Explicit calling-convention adapters for recorded dependencies, never parent code.
boundaries={0xa0afe0:('CoreCtor',16),0xa62020:('BankCtor',4),0xa62fb0:('VertexCtor',4),0x99b6b0:('WideCtor',12),0x99be70:('WideAdd',12),0x99bf30:('WideAddLiteral',12),0x9a8840:('OpenRetail',16),0x99b510:('WideDtor',4),0x99ebf0:('CharCtor',16),0x9a80b0:('BankExists',12),0x98b860:('CreateResource',12),0x9de7c0:('AssignResource',12),0xa5d7c0:('OpenShaderBank',12),0xa5d4e0:('SetDeclaration',12),0xa5d4f0:('SetFormat',12),0xa5d5f0:('LookupVertex',16),0x99eae0:('CharDtor',4)}
aliases=[]
for name,address in targets.items():
 if address in boundaries:
  target,count=boundaries[address];aliases.append('#pragma comment(linker,"/alternatename:'+name+'=@'+target+'@'+str(count)+'")')
for name in ['??1CRenderManagerCore@@UAE@XZ','??1CShaderDataBank@@UAE@XZ','?CountReferences@CVertexShader@@EAEKXZ','?GetDebugName@CShaderBase@@MBE?AVCCharString@@XZ','?Uninitialise@CTexture@@QAEXXZ']:
 aliases.append('#pragma comment(linker,"/alternatename:'+name+'=_UnusedRendererCtorBoundary")')
(w/'renderer_ctor_aliases.h').write_text('\n'.join(aliases)+'\n')
objects=[]
for i,source in enumerate(['rebuild/src/compiled/00/9d/CRenderManager2D_Constructor_009d9a30.cpp','rebuild/tests/integration/NativeRendererCtor_test.cpp']):
 obj=w/('%d.obj'%i);r=V.cl(['/nologo','/c','/O2','/MT','/I'+str(w),'/Fo'+str(obj),str(ROOT/source)],env,w);assert not r.returncode,r.stdout+r.stderr;objects.append(obj)
proof=check(objects[0],'??0CRenderManager2D@@QAE@AAVCDisplayManager@@ABVCWideString@@K@Z',0x9d9a30,577,33,targets)
coff=Coff(objects[0]);coff.table('??_7CRenderManager2D@@6B@',['??_ECRenderManager2D@@UAEPAXI@Z'])
# Absolute data symbols only. No retail executable instructions in this fixture.
strings=bytearray();symbols=bytearray();globals_={name:value for name,value in targets.items()if '@@3' in name and 'Fable2DShaderDeclaration' not in name}
for name,value in globals_.items():
 symbols.extend(struct.pack('<2IIhHBB',0,4+len(strings),value,-1,0,2,0));strings.extend(name.encode()+b'\0')
bindings=w/'globals.obj';bindings.write_bytes(struct.pack('<HHIIIHH',0x14c,0,0,20,len(globals_),0,0)+symbols+struct.pack('<I',4+len(strings))+strings)
data_source=ROOT/'rebuild/src/compiled/01/29/Render2DShaderDeclaration_0129d708.cpp'
data_obj=w/'declaration.obj';r=V.cl(['/nologo','/c','/O2','/MT','/Fo'+str(data_obj),str(data_source)],env,w);assert not r.returncode,r.stdout+r.stderr
from gate_render_state_static_data import coff_data
from pe_oracle import EXE,pe_sections,va_to_off
raw=EXE.read_bytes();offset=va_to_off(pe_sections(raw),0x129d708);symbol='?Fable2DShaderDeclaration_0129D708@@3QBU_D3DVERTEXELEMENT9@@B'
assert coff_data(Coff(data_obj),symbol,40)==raw[offset:offset+40]
objects.append(data_obj)
exe=w/'fixture.exe';cmd=[str(V.VC/'bin/link.exe'),'/nologo','/fixed','/out:'+str(exe),'kernel32.lib']
for omit in [0,2]:
 r=subprocess.run(cmd+[str(o)for i,o in enumerate(objects)if i!=omit]+[str(bindings)],env=env,capture_output=True,text=True);assert r.returncode,('required provider',omit)
r=subprocess.run(cmd+list(map(str,objects))+[str(bindings)],env=env,capture_output=True,text=True);assert not r.returncode,r.stdout+r.stderr
inputs,expected,report=oracle.collect();(w/'input.bin').write_bytes(inputs)
r=subprocess.run([str(exe),str(w/'input.bin'),str(w/'actual.bin')],env=env,capture_output=True,text=True,timeout=60);assert not r.returncode,(w,r.returncode,r.stdout,r.stderr)
actual=(w/'actual.bin').read_bytes()
if actual!=expected:
 (w/'expected.bin').write_bytes(expected);difference=next((i for i,(a,b)in enumerate(zip(actual,expected))if a!=b),min(len(actual),len(expected)))
 stride=16+oracle.WIDTH+0x38+0x3dc;raise AssertionError((w,difference,difference//stride,difference%stride,len(actual),len(expected)))
report.update(providers=[proof],omission_checks=2,data_bytes=40,compared_bytes=len(actual),trace_sha256=hashlib.sha256(actual).hexdigest(),boundary='Native parent and actual40-byte vertex declaration. Core/bank/vertex constructors, strings, bank opening, resource create/assign/configure and lookup are recorded boundaries. Callback mutations and inline counted-resource/shader release paths. Full24KiB arena plus bank and shader globals; four vtable words and control callback pointer normalized.')
(ROOT/'rebuild/compile-gate/native-renderer-ctor.json').write_text(json.dumps(report,indent=2)+'\n');print(r.stdout.strip(),report['snapshots'],'snapshots')
