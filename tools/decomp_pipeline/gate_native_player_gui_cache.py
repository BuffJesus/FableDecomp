import hashlib,json,struct,subprocess,sys,tempfile
from pathlib import Path
ROOT=Path(__file__).resolve().parents[2];sys.path.insert(0,str(ROOT/'tools/decomp_pipeline'))
import check_one as V
from variable_allocator_parity import check
from pe_oracle import EXE,read_virtual_data
from gate_game_component_abi import Coff
w=Path(tempfile.mkdtemp(prefix='native-player-gui-cache-'));env=V.env();source=ROOT/'rebuild/src/compiled/01/3b/CPlayerGui_PGuiDef_013b878c.cpp'
if '--stage' in sys.argv or not source.exists():source=ROOT/'work/parity_addtext/player_gui_cache_candidate_20261009.cpp'
obj=w/'cache.obj';r=V.cl(['/nologo','/c','/O2','/MT','/I'+str(ROOT/'rebuild/include'),'/Fo'+str(obj),str(source)],env,w);assert not r.returncode,r.stdout+r.stderr
targets={'_$E2':0x1228b70,'_atexit':0x4012bc,'?PGuiDef@CPlayerGui@@0V?$CDefPointer@$$CBVCPlayerGuiDef@@@@A':0x13b878c}
proofs=[check(obj,'_$E1',0x121a360,12,2,targets),check(obj,'_$E2',0x1228b70,40,2,targets)];c=Coff(obj);s,o=c.symbols[list(targets)[2]];at=20+40*(s-1);size,rawptr=struct.unpack_from('<II',c.data,at+16);flags=struct.unpack_from('<I',c.data,at+36)[0];assert o==0 and size==4 and rawptr==0 and flags&0x80
body=bytearray(72);raw=EXE.read_bytes();body[:12]=read_virtual_data(raw,0x121a360,12);body[32:]=read_virtual_data(raw,0x1228b70,40);(w/'player_gui_cache_retail.h').write_text('static const unsigned char retailCode[]={'+','.join(map(str,body))+'};\n')
fixture=ROOT/'rebuild/tests/integration/NativePlayerGuiCache_test.cpp';fo=w/'fixture.obj';r=V.cl(['/nologo','/c','/O2','/MT','/I'+str(w),'/I'+str(ROOT/'rebuild/include'),'/Fo'+str(fo),str(fixture)],env,w);assert not r.returncode,r.stdout+r.stderr
cmd=[str(V.VC/'bin/link.exe'),'/nologo','/out:'+str(w/'fixture.exe'),'kernel32.lib',str(fo)];r=subprocess.run(cmd,env=env,capture_output=True,text=True);assert r.returncode and ('LNK2001' in r.stdout or 'LNK2019' in r.stdout)
r=subprocess.run(cmd+[str(obj)],env=env,capture_output=True,text=True);assert not r.returncode,r.stdout+r.stderr
r=subprocess.run([str(w/'fixture.exe')],env=env,capture_output=True,text=True,timeout=30);assert not r.returncode,(w,r.returncode,r.stdout,r.stderr);assert 'cases=80 snapshots=160' in r.stdout
report=dict(cases=80,snapshots=160,providers=proofs,omission_checks=1,zero_initialized_storage_bytes=4,source_sha256={str(source.relative_to(ROOT)):hashlib.sha256(source.read_bytes()).hexdigest()},fixture_sha256=hashlib.sha256(fixture.read_bytes()).hexdigest(),boundary='Actual CPlayerGui PGuiDef storage and compiler generated registration/destructor. Real compiled CRT initialization must register exactly one destructor and zero cache. Native registration independently tested. Null/live refcounts including wrap and callback cache mutations;full16KiB snapshots;atexit and virtual deletion boundaries.')
(ROOT/'rebuild/compile-gate/native-player-gui-cache.json').write_text(json.dumps(report,indent=2)+'\n');print(r.stdout.strip());print('NATIVE_PLAYER_GUI_CACHE_GATE_PASS')
