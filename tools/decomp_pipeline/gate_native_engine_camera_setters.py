"""Compile, compare and replay native camera forwarding and actual leaf setters."""
import hashlib,json,subprocess,tempfile
from pathlib import Path
import check_one as V
from gate_game_component_abi import Coff
from gate_render_state_static_data import coff_data
from variable_allocator_parity import check
from pe_oracle import EXE,read_virtual_data
import oracle_native_engine_camera_setters as oracle
ROOT=Path(__file__).resolve().parents[2]
w=Path(tempfile.mkdtemp(prefix='native-engine-camera-setters-'));env=V.env()
sources=[ROOT/'rebuild/src/compiled'/p for p in ['00/b2/CEngine_CameraSetters_00b23b50.cpp','00/b2/CEngineCamera_Setters_00b2fbf0.cpp','01/43/NEngineInternal_GPCamera_01436ea0.cpp']]
fixture=ROOT/'rebuild/tests/integration/NativeEngineCameraSetters_test.cpp'
global_symbol='?GPCamera@NEngineInternal@@3PAVCEngineCamera@@A'
camera='?SetGameCamera@CEngineCamera@@QAEXPBVCCamera@@@Z'
window='?SetGameWindow@CEngineCamera@@QAEXPBVC2DBoxF@@@Z'
targets={global_symbol:0x1436ea0,camera:0xb2fbf0,window:0xb2fc00,'?Process@CEngineCamera@@QAEX_N@Z':0xb314e0}
specs=[(0,'?SetCamera@CEngine@@UAEXPBVCCamera@@@Z',0xb23b50,41,4),(0,'?SetWindow@CEngine@@UAEXPBVC2DBoxF@@@Z',0xb23b80,11,2),(1,camera,0xb2fbf0,10,0),(1,window,0xb2fc00,10,0)]
objects=[]
for i,path in enumerate(sources):
    obj=w/(str(i)+'.obj');r=V.cl(['/nologo','/c','/O2','/MT','/Fo'+str(obj),str(path)],env,w);assert not r.returncode,r.stdout+r.stderr;objects.append(obj)
proof=[check(objects[i],symbol,address,size,count,targets)for i,symbol,address,size,count in specs]
c=Coff(objects[2]);section,offset=c.symbols[global_symbol];body,rel=c.sections[section]
assert offset==0 and coff_data(c,global_symbol,4)==bytes(4) and not rel
assert not any(name.startswith('_$E')or name.startswith('_$S')for name in c.symbols)
assert read_virtual_data(EXE.read_bytes(),0x1436ea0,4)==bytes(4)
reports=[];omissions=0
for composed in (False,True):
    aliases={'@SetCamera@12':specs[0][1],'@SetWindow@12':specs[1][1],'_CameraGlobal':global_symbol,'?Process@CEngineCamera@@QAEX_N@Z':'@ProcessBoundary@12','@SetGameCamera@12':camera,'@SetGameWindow@12':window}
    if not composed:aliases.update({camera:'@CameraBoundary@12',window:'@WindowBoundary@12'})
    (w/'engine_camera_setters_aliases.h').write_text('\n'.join('#pragma comment(linker,"/alternatename:'+a+'='+b+'")'for a,b in aliases.items())+'\n')
    obj=w/('fixture%d.obj'%composed);r=V.cl(['/nologo','/c','/O2','/MT','/I'+str(w),'/Fo'+str(obj)]+(['/DCOMPOSED']if composed else[])+[str(fixture)],env,w);assert not r.returncode,r.stdout+r.stderr
    required=objects if composed else [objects[0],objects[2]]
    exe=w/('fixture%d.exe'%composed);cmd=[str(V.VC/'bin/link.exe'),'/nologo','/out:'+str(exe),'kernel32.lib',str(obj)]
    for omitted in required:
        r=subprocess.run(cmd+[str(p)for p in required if p!=omitted],env=env,capture_output=True,text=True);assert r.returncode,omitted;omissions+=1
    r=subprocess.run(cmd+list(map(str,required)),env=env,capture_output=True,text=True);assert not r.returncode,r.stdout+r.stderr
    inputs,expected,report=oracle.collect(composed);(w/'input.bin').write_bytes(inputs)
    r=subprocess.run([str(exe),str(w/'input.bin'),str(w/'actual.bin')],env=env,capture_output=True,text=True,timeout=60);assert not r.returncode,(w,r.returncode,r.stdout,r.stderr)
    actual=(w/'actual.bin').read_bytes()
    if actual!=expected:
        (w/'expected.bin').write_bytes(expected);difference=next((i for i,(a,b)in enumerate(zip(actual,expected))if a!=b),min(len(actual),len(expected)));raise AssertionError((w,composed,difference,difference//2064,difference%2064,len(actual),len(expected)))
    report['composed']=composed;reports.append(report);print(r.stdout.strip(),report['snapshots'],'snapshots',flush=True)
report=dict(providers=proof,replays=reports,omission_checks=omissions,source_sha256={str(p.relative_to(ROOT)):hashlib.sha256(p.read_bytes()).hexdigest()for p in sources},fixture_sha256=hashlib.sha256(fixture.read_bytes()).hexdigest(),boundary='Full2048-byte arena and actual global pointer. All256fills, nullable/opaque arguments, engine/camera aliasing, global replacement by setter and Process callbacks, post-callback CurrentCamera store, direct and composed leaf execution. Process remains a recorded boundary. No normalized state bytes; retail stack cleanup and callee-saved registers checked.')
(ROOT/'rebuild/compile-gate/native-engine-camera-setters.json').write_text(json.dumps(report,indent=2)+'\n')
