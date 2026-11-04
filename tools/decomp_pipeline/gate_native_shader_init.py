"""Full-body parity and compiled retail replay for shader-manager initialization."""
import hashlib,json,subprocess,tempfile
from pathlib import Path
import check_one as V
from gate_game_component_abi import Coff
from variable_allocator_parity import check
from pe_oracle import EXE,pe_sections,va_to_off
import oracle_native_shader_init as oracle
ROOT=Path(__file__).resolve().parents[2];w=Path(tempfile.mkdtemp(prefix='native-shader-init-'));env=V.env();objects=[]
source=ROOT/'rebuild/src/compiled/00/98/CShaderRenderManager_Initialise_0098acf0.cpp';fixture=ROOT/'rebuild/tests/integration/NativeShaderInit_test.cpp'
for i,path in enumerate([source,fixture]):
 obj=w/('%d.obj'%i);r=V.cl(['/nologo','/c','/O2','/MT','/Fo'+str(obj),str(path)],env,w);assert not r.returncode,r.stdout+r.stderr;objects.append(obj)
targets={'?resize@?$vector@PBVCVertexShaderConstantLayout@@V?$allocator@PBVCVertexShaderConstantLayout@@@_STL@@@_STL@@QAEXIABQBVCVertexShaderConstantLayout@@@Z':0x98d870,'?Initialise@CShaderResource@@AAEXAAVCShaderRenderManager@@@Z':0xa5d4d0,'??0CCharString@@QAE@PBDJ@Z':0x99ebf0,'?push_back@?$CIndexedArray@VCCharString@@@@QAEXABVCCharString@@@Z':0x98db20,'??1CCharString@@QAE@XZ':0x99eae0,'?erase@?$vector@VC4DVector@@V?$allocator@VC4DVector@@@_STL@@@_STL@@QAEPAVC4DVector@@PAV3@0@Z':0x98cf70,'?_M_fill_insert@?$vector@VC4DVector@@V?$allocator@VC4DVector@@@_STL@@@_STL@@QAEXPAVC4DVector@@IABV3@@Z':0x98d360}
c=Coff(objects[0]);raw=EXE.read_bytes();sections=pe_sections(raw);names=set()
for name,(sec,offset)in c.symbols.items():
 if not name.startswith('??_C@'):continue
 value=c.sections[sec][0][offset:].split(b'\0')[0];index=oracle.NAMES.index(value.decode());address=oracle.NAME_ADDRESSES[index];at=va_to_off(sections,address);assert raw[at:at+len(value)+1]==value+b'\0';targets[name]=address;names.add(index)
assert names==set(range(9))
proof=check(objects[0],'?Initialise@CShaderRenderManager@@QAEXPAVCRenderManager@@PAUIDirect3DDevice9@@@Z',0x98acf0,1333,40,targets)
exe=w/'fixture.exe';cmd=[str(V.VC/'bin/link.exe'),'/nologo','/out:'+str(exe),'kernel32.lib']
r=subprocess.run(cmd+[str(objects[1])],env=env,capture_output=True,text=True);assert r.returncode,'parent provider required'
r=subprocess.run(cmd+list(map(str,objects)),env=env,capture_output=True,text=True);assert not r.returncode,r.stdout+r.stderr
inputs,expected,report=oracle.collect();(w/'input.bin').write_bytes(inputs)
r=subprocess.run([str(exe),str(w/'input.bin'),str(w/'actual.bin')],env=env,capture_output=True,text=True,timeout=60);assert not r.returncode,(w,r.returncode,r.stdout,r.stderr)
actual=(w/'actual.bin').read_bytes()
if actual!=expected:
 (w/'expected.bin').write_bytes(expected);difference=next((i for i,(a,b)in enumerate(zip(actual,expected))if a!=b),min(len(actual),len(expected)));raise AssertionError((w,difference,difference//(oracle.WIDTH+16),difference%(oracle.WIDTH+16),len(actual),len(expected)))
report.update(providers=[proof],omission_checks=1,compared_bytes=len(actual),source_sha256=hashlib.sha256(source.read_bytes()).hexdigest(),fixture_sha256=hashlib.sha256(fixture.read_bytes()).hexdigest(),boundary='Full1333-byte manager initialization. Full16KiB at every recorded dependency and return, no masked state bytes; literal names mapped to validated indices, temporary string values captured. Layout/resource/name/table boundaries, existing names retained, table counts0/3/9/10/12, live resource list mutation and constant table replacement. Matrix defaults and untouched transforms verified in snapshots. No shader execution or GPU initialization claim.')
(ROOT/'rebuild/compile-gate/native-shader-init.json').write_text(json.dumps(report,indent=2)+'\n');print(r.stdout.strip(),report['snapshots'],'snapshots')
