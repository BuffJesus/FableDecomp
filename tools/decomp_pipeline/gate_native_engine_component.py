"""Shared engine component lifetime, owner links, virtual defaults and console name."""
import hashlib,json,subprocess,tempfile
from pathlib import Path
import check_one as V
from gate_game_component_abi import Coff
from variable_allocator_parity import check
from pe_oracle import EXE,pe_sections,va_to_off
import oracle_native_engine_component as oracle
ROOT=Path(__file__).resolve().parents[2];w=Path(tempfile.mkdtemp(prefix='native-engine-component-'));env=V.env()
source=ROOT/'rebuild/src/compiled/00/b5/CEngineComponent_Lifetime_00b59710.cpp';fixture=ROOT/'rebuild/tests/integration/NativeEngineComponent_test.cpp'
specs=oracle.SPECS
targets={'??_7CEngineComponent@@6B@':0x12a2544,'??_C@_00CNPNBAHC@?$AA@':0x122d70e,'??0CCharString@@QAE@PBDJ@Z':0x99ebf0,'??3@YAXPAX@Z':0xbfe9bc}
aliases={'@ConstructComponent@4':'??0CEngineComponent@@QAE@XZ','@DestroyComponent@4':'??1CEngineComponent@@UAE@XZ','@SetOwner@12':'?SetGlobalComponentPointer@CEngineComponent@@QAEXPAPAX@Z','@DeleteComponent@12':'??_GCEngineComponent@@UAEPAXI@Z','@ConsoleName@12':'?GetConsoleEnableFunctionName@CEngineComponent@@UAE?AVCCharString@@XZ','_ComponentTable':'??_7CEngineComponent@@6B@','??0CCharString@@QAE@PBDJ@Z':'@CharCtorBoundary@16'}
for i,e in enumerate(specs[:10]):aliases['@Default%d@%d'%(i,12 if e['param']else 8)]=e['symbol']
(w/'engine_component_aliases.h').write_text('\n'.join('#pragma comment(linker,"/alternatename:'+a+'='+b+'")'for a,b in aliases.items())+'\n')
objects=[]
for i,path in enumerate([source,fixture]):
 obj=w/(str(i)+'.obj');r=V.cl(['/nologo','/c','/O2','/MT','/I'+str(w),'/Fo'+str(obj),str(path)],env,w);assert not r.returncode,r.stdout+r.stderr;objects.append(obj)
proof=[check(objects[0],e['symbol'],e['address'],e['size'],e['relocations'],targets)for e in specs]
c=Coff(objects[0]);table=json.loads((ROOT/'tools/decomp_pipeline/native_engine_component_vtable.json').read_text())
c.table(table['symbol'],[e['symbol']for e in table['entries']])
from pe_oracle import read_virtual_data
import struct
assert read_virtual_data(EXE.read_bytes(),0x12a2544,52)==struct.pack('<13I',*[int(e['retail_address'],16)for e in table['entries']])
sec,off=c.symbols['??_C@_00CNPNBAHC@?$AA@'];assert c.sections[sec][0][off:off+1]==read_virtual_data(EXE.read_bytes(),0x122d70e,1)==bytes(1)
exe=w/'fixture.exe';cmd=[str(V.VC/'bin/link.exe'),'/nologo','/out:'+str(exe),'kernel32.lib']
r=subprocess.run(cmd+[str(objects[1])],env=env,capture_output=True,text=True);assert r.returncode
r=subprocess.run(cmd+list(map(str,objects)),env=env,capture_output=True,text=True);assert not r.returncode,r.stdout+r.stderr
inputs,expected,report=oracle.collect();(w/'input.bin').write_bytes(inputs)
r=subprocess.run([str(exe),str(w/'input.bin'),str(w/'actual.bin')],env=env,capture_output=True,text=True,timeout=60);assert not r.returncode,(w,r.returncode,r.stdout,r.stderr)
actual=(w/'actual.bin').read_bytes()
if actual!=expected:
 (w/'expected.bin').write_bytes(expected);difference=next((i for i,(a,b)in enumerate(zip(actual,expected))if a!=b),min(len(actual),len(expected)));raise AssertionError((w,difference,difference//1036,difference%1036,len(actual),len(expected)))
report.update(providers=proof,omission_checks=1,source_sha256=hashlib.sha256(source.read_bytes()).hexdigest(),fixture_sha256=hashlib.sha256(fixture.read_bytes()).hexdigest(),boundary='Full1024-byte snapshots. All256fills, null/external/self-alias owner links, constructor padding preservation, setter non-dereference, direct/scalar/combined teardown with flags0/1/2/FFFFFFFF, ten default virtuals and hidden string results.13-slot table matches donor identities and retail targets; only compiled vtable identity mapped. Character construction and heap deletion remain recorded boundaries; no state bytes masked.')
(ROOT/'rebuild/compile-gate/native-engine-component.json').write_text(json.dumps(report,indent=2)+'\n');print(r.stdout.strip(),report['snapshots'],'snapshots')
