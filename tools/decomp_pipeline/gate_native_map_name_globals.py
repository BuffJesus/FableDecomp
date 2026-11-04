"""Three genuine static-map names with exact CRT initialization/exit and native lifetime trace."""
import hashlib,json,subprocess,tempfile
from pathlib import Path
import check_one as V
from gate_game_component_abi import Coff
from gate_render_state_static_data import coff_data
from variable_allocator_parity import check
from pe_oracle import EXE,read_virtual_data
import oracle_native_map_name_globals as oracle
ROOT=Path(__file__).resolve().parents[2];w=Path(tempfile.mkdtemp(prefix='native-map-name-globals-'));env=V.env()
specs=[('SeaStaticMapBankFileName',0x1224680,0x122acd0,0x1436ec4,0x12a2058),('WaterStaticMapBankFileName',0x12246a0,0x122ace0,0x1436ec8,0x12a207c),('CommonStaticMapHeaderEntryName',0x12246c0,0x122acf0,0x1436ecc,0x12a20a4)]
paths=['rebuild/src/compiled/01/22/Global_'+name+'_%08x.cpp'%init for name,init,_,_,_ in specs]+['rebuild/tests/integration/NativeMapNameGlobals_test.cpp'];objects=[]
for i,path in enumerate(paths):
 obj=w/(str(i)+'.obj');r=V.cl(['/nologo','/c','/O2','/MT','/Fo'+str(obj),str(ROOT/path)],env,w);assert not r.returncode,r.stdout+r.stderr;objects.append(obj)
proof=[];raw=EXE.read_bytes()
for i,(name,init,fini,address,literal) in enumerate(specs):
 c=Coff(objects[i]);symbol='?'+name+'@@3VCCharString@@A';literals=[s for s in c.symbols if s.startswith('??_C@')];assert len(literals)==1;text=literals[0]
 targets={symbol:address,'_$E2':fini,'_atexit':0x4012bc,'??0CCharString@@QAE@PBDJ@Z':0x99ebf0,'??1CCharString@@QAE@XZ':0x99eae0,text:literal}
 proof.extend([check(objects[i],'_$E1',init,29,5,targets),check(objects[i],'_$E2',fini,10,2,targets)])
 assert coff_data(c,symbol,4)==bytes(4) and read_virtual_data(raw,address,4)==bytes(4)
 sec,off=c.symbols['_$S3'];assert c.sections[sec][1]=={0:('_$E1',6)}
 sec,off=c.symbols[text];expected=oracle.NAMES[i]+b'\0';assert c.sections[sec][0][off:off+len(expected)]==expected==read_virtual_data(raw,literal,len(expected))
exe=w/'fixture.exe';cmd=[str(V.VC/'bin/link.exe'),'/nologo','/out:'+str(exe),'kernel32.lib']
for omit in range(3):
 r=subprocess.run(cmd+[str(o) for i,o in enumerate(objects) if i!=omit],env=env,capture_output=True,text=True);assert r.returncode
r=subprocess.run(cmd+list(map(str,objects)),env=env,capture_output=True,text=True);assert not r.returncode,r.stdout+r.stderr
r=subprocess.run([str(exe),str(w/'actual.bin')],env=env,capture_output=True,text=True,timeout=30);assert not r.returncode,(w,r.returncode,r.stdout,r.stderr);print(r.stdout.strip())
expected,trace=oracle.collect();actual=(w/'actual.bin').read_bytes();assert actual==expected,(w,len(actual),len(expected))
report=dict(providers=proof,data_providers=3,snapshots=trace['snapshots'],omission_checks=3,source_sha256={p:hashlib.sha256((ROOT/p).read_bytes()).hexdigest() for p in paths},boundary='Three real globals, zero-initialized storage, exact literal contents, CRT registration and reverse-order destruction. Six original workers/21fixups. Actual compiled startup/exit compared to bounded original Unicorn execution: all three global words and full1KiB callback arena at each event. String construction/destruction are explicit validated callbacks, not a string allocator/lifetime claim.')
(ROOT/'rebuild/compile-gate/native-map-name-globals.json').write_text(json.dumps(report,indent=2)+'\n');print('NATIVE_MAP_NAME_GLOBALS_GATE_PASS',trace['snapshots'],'snapshots')
