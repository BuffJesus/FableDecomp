"""Camera-change notification composed with the actual occlusion scene flag."""
import hashlib,itertools,json,struct,subprocess,tempfile
from pathlib import Path
from unicorn import Uc,UC_ARCH_X86,UC_MODE_32
from unicorn.x86_const import *
import check_one as V
from gate_game_component_abi import Coff
from gate_render_state_static_data import coff_data
from variable_allocator_parity import check
from pe_oracle import EXE,pe_sections,read_virtual_data
ROOT=Path(__file__).resolve().parents[2];w=Path(tempfile.mkdtemp(prefix='native-engine-camera-angle-'));env=V.env()
sources=[ROOT/'rebuild/src/compiled'/p for p in ('00/b2/CEngine_ChangeCameraAngle_00b23a50.cpp','00/b2/CEnginePrimitiveOcclusionManager_StartNewScene_00b2f740.cpp','01/43/NEngineInternal_GPPrimitiveOcclusionManager_01436e58.cpp')]
fixture=ROOT/'rebuild/tests/integration/NativeEngineCameraAngle_test.cpp'
parent='?ChangeCameraAngle@CEngine@@UAEXXZ';leaf='?StartNewScene@CEnginePrimitiveOcclusionManager@@QAEXXZ';global_symbol='?GPPrimitiveOcclusionManager@NEngineInternal@@3PAVCEnginePrimitiveOcclusionManager@@A'
aliases={'@ChangeAngle@4':parent,'@StartScene@4':leaf,'_OcclusionGlobal':global_symbol}
(w/'engine_camera_angle_aliases.h').write_text('\n'.join('#pragma comment(linker,"/alternatename:'+a+'='+b+'")'for a,b in aliases.items())+'\n')
objects=[]
for i,path in enumerate(sources+[fixture]):
    obj=w/(str(i)+'.obj');r=V.cl(['/nologo','/c','/O2','/MT','/I'+str(w),'/Fo'+str(obj),str(path)],env,w);assert not r.returncode,r.stdout+r.stderr;objects.append(obj)
proofs=[check(objects[0],parent,0xb23a50,20,2,{global_symbol:0x1436e58,leaf:0xb2f740}),check(objects[1],leaf,0xb2f740,5,0,{})]
raw=EXE.read_bytes();assert coff_data(Coff(objects[2]),global_symbol,4)==read_virtual_data(raw,0x1436e58,4)==bytes(4)
exe=w/'fixture.exe';cmd=[str(V.VC/'bin/link.exe'),'/nologo','/out:'+str(exe),'kernel32.lib']
for omitted in range(3):
    r=subprocess.run(cmd+[str(p)for i,p in enumerate(objects)if i!=omitted],env=env,capture_output=True,text=True);assert r.returncode
r=subprocess.run(cmd+list(map(str,objects)),env=env,capture_output=True,text=True);assert not r.returncode,r.stdout+r.stderr
BASE=0x24000000;u=Uc(UC_ARCH_X86,UC_MODE_32);u.mem_map(0x400000,0x1200000);u.mem_map(BASE,0x10000)
for _,va,_,off,n in pe_sections(raw):
    if n:u.mem_write(0x400000+va,raw[off:off+n])
inputs=bytearray();expected=bytearray();cases=0
for fill,kind,engine,manager in itertools.product(range(256),range(2),(0,0x100,0x200),(0,0x104,0x200,0x800)):
    arena=bytes([fill])*4096;inputs.extend(struct.pack('<3I',kind,engine,manager));inputs.extend(arena)
    u.mem_write(BASE,arena);u.mem_write(0x1436e58,struct.pack('<I',BASE+manager));sp=BASE+0xe000;stop=BASE+0xf000
    u.mem_write(sp,struct.pack('<I',stop));u.reg_write(UC_X86_REG_ESP,sp);u.reg_write(UC_X86_REG_ECX,BASE+(engine if kind else manager))
    saved={UC_X86_REG_EBX:0x13572468,UC_X86_REG_ESI:0x23456789,UC_X86_REG_EDI:0x3456789a,UC_X86_REG_EBP:0x456789ab}
    for reg,value in saved.items():u.reg_write(reg,value)
    u.emu_start(0xb23a50 if kind else 0xb2f740,stop,count=40)
    assert u.reg_read(UC_X86_REG_ESP)==sp+4 and all(u.reg_read(reg)==value for reg,value in saved.items())
    expected.extend(u.mem_read(0x1436e58,4));expected.extend(u.mem_read(BASE,4096));cases+=1
(w/'input.bin').write_bytes(inputs)
r=subprocess.run([str(exe),str(w/'input.bin'),str(w/'actual.bin')],env=env,capture_output=True,text=True,timeout=60);assert not r.returncode,(w,r.returncode,r.stdout,r.stderr)
assert (w/'actual.bin').read_bytes()==expected,w
report=dict(cases=cases,snapshots=cases,omission_checks=3,providers=proofs,source_sha256={str(p.relative_to(ROOT)):hashlib.sha256(p.read_bytes()).hexdigest()for p in sources},fixture_sha256=hashlib.sha256(fixture.read_bytes()).hexdigest(),boundary='Direct leaf and actual parent/leaf composition;all256fills,three engine and four manager placements including exact/partial overlap and shared destination flag. Full4KiB arena and real global storage,retail stack/callee-saved registers. Original retail instructions under Unicorn compared with compiled native execution. No callbacks or normalized bytes.')
(ROOT/'rebuild/compile-gate/native-engine-camera-angle.json').write_text(json.dumps(report,indent=2)+'\n');print(r.stdout.strip())
