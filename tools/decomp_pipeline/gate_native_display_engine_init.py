"""Native display-engine Init composed with real view-manager ownership."""
import hashlib,json,subprocess,tempfile
from pathlib import Path
import check_one as V
from gate_game_component_abi import Coff
from variable_allocator_parity import check
from pe_oracle import EXE,pe_sections,va_to_off
import oracle_native_display_engine_init as oracle
ROOT=Path(__file__).resolve().parents[2];w=Path(tempfile.mkdtemp(prefix='native-display-engine-init-'));env=V.env()
source=ROOT/'rebuild/src/compiled/00/43/CDisplayEngine_Init_004350d0.cpp';owner=ROOT/'rebuild/src/compiled/00/43/CDisplayViewManager_Ownership_00436cc0.cpp';fixture=ROOT/'rebuild/tests/integration/NativeDisplayEngineInit_test.cpp'
init='?Init@CDisplayEngine@@QAEXXZ';ctor='??0CDisplayViewManager@@QAE@XZ';reset='?Reset@?$CCountedPointer@VCDisplayViewManager@@@@QAEXPAVCDisplayViewManager@@@Z';delete='?DeleteData@?$CCountedPointer@VCDisplayViewManager@@@@CIXPAX@Z'
targets={ctor:0x640040,reset:0x436cc0,delete:0x436ba0,'??0CCharString@@QAE@PBDJ@Z':0x99ebf0,'??1CCharString@@QAE@XZ':0x99eae0,'?DisplayProgress@NProgressDisplay@@YIXABVCCharString@@M_N1@Z':0x9e9f40,'??2@YAPAXI@Z':0xbfea1a,'??3@YAXPAX@Z':0xbfe9bc,'__real@0000000000000000':0x122ed70}
aliases={'@InitialiseDisplayEngine@4':init,'@DeleteView@4':delete,'??0CCharString@@QAE@PBDJ@Z':'@StringCtorBoundary@16','??1CCharString@@QAE@XZ':'@StringDtorBoundary@4','?DisplayProgress@NProgressDisplay@@YIXABVCCharString@@M_N1@Z':'@ProgressBoundary@16'}
(w/'display_engine_init_aliases.h').write_text('\n'.join('#pragma comment(linker,"/alternatename:'+a+'='+b+'")'for a,b in aliases.items())+'\n')
objects=[]
for i,path in enumerate([source,owner,fixture]):
 obj=w/(str(i)+'.obj');r=V.cl(['/nologo','/c','/O2','/MT','/I'+str(w),'/Fo'+str(obj),str(path)],env,w);assert not r.returncode,r.stdout+r.stderr;objects.append(obj)
c=Coff(objects[0]);raw=EXE.read_bytes();sections=pe_sections(raw)
for text,address in [(b'Init Display Engine: Init Graphics\0',0x12315ac),(b'Init Display Engine: Init Engine\0',0x1231588),(bytes(8),0x122ed70)]:
 names=[name for name,(sec,off)in c.symbols.items()if name.startswith(('??_C','__real'))and c.sections[sec][0][off:]==text];assert len(names)==1,names;o=va_to_off(sections,address);assert raw[o:o+len(text)]==text;targets[names[0]]=address
proof=[check(objects[0],init,0x4350d0,180,13,targets),check(objects[1],ctor,0x640040,16,0,targets),check(objects[1],reset,0x436cc0,103,3,targets),check(objects[1],delete,0x436ba0,66,2,targets)]
exe=w/'fixture.exe';cmd=[str(V.VC/'bin/link.exe'),'/nologo','/out:'+str(exe),'kernel32.lib']
for omit in [0,1]:
 r=subprocess.run(cmd+[str(o)for i,o in enumerate(objects)if i!=omit],env=env,capture_output=True,text=True);assert r.returncode
r=subprocess.run(cmd+list(map(str,objects)),env=env,capture_output=True,text=True);assert not r.returncode,r.stdout+r.stderr
inputs,expected,report=oracle.collect();(w/'input.bin').write_bytes(inputs)
r=subprocess.run([str(exe),str(w/'input.bin'),str(w/'actual.bin')],env=env,capture_output=True,text=True,timeout=60);assert not r.returncode,(w,r.returncode,r.stdout,r.stderr)
actual=(w/'actual.bin').read_bytes()
if actual!=expected:
 (w/'expected.bin').write_bytes(expected);difference=next((i for i,(a,b)in enumerate(zip(actual,expected))if a!=b),min(len(actual),len(expected)));raise AssertionError((w,difference,difference//4116,difference%4116,len(actual),len(expected)))
report.update(providers=proof,omission_checks=2,source_sha256=hashlib.sha256(source.read_bytes()).hexdigest(),owner_sha256=hashlib.sha256(owner.read_bytes()).hexdigest(),fixture_sha256=hashlib.sha256(fixture.read_bytes()).hexdigest(),boundary='Actual Init, view constructor, ownership reset and nested deletion helper. Full4KiB snapshots; string/progress, virtual engine initialization, new/delete and nested view deletion are recorded boundaries. Null ownership, refs0/1/2/FFFFFFFF, allocation failure at8/12 bytes, live virtual dispatch and callback mutations. Five actual function-pointer fields map by identity; no masked state bytes. No GPU initialization or frame claim.')
(ROOT/'rebuild/compile-gate/native-display-engine-init.json').write_text(json.dumps(report,indent=2)+'\n');print(r.stdout.strip(),report['snapshots'],'snapshots')
