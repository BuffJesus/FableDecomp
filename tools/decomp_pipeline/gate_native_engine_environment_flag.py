"""Native environmental update flag and real environment pointer storage."""
import hashlib,itertools,json,struct,subprocess,tempfile
from pathlib import Path
from unicorn import Uc,UC_ARCH_X86,UC_MODE_32
from unicorn.x86_const import *
import check_one as V
from gate_game_component_abi import Coff
from gate_render_state_static_data import coff_data
from variable_allocator_parity import check
from pe_oracle import EXE,pe_sections,read_virtual_data
ROOT=Path(__file__).resolve().parents[2];w=Path(tempfile.mkdtemp(prefix='native-engine-environment-'));env=V.env()
sources=[ROOT/'rebuild/src/compiled'/p for p in ['00/b2/CEngine_SetEnvironmentalEffectsAlwaysUpdate_00b23b90.cpp','01/43/NEngineInternal_GPEngineEnvironment_01436e48.cpp']]
fixture=ROOT/'rebuild/tests/integration/NativeEngineEnvironmentFlag_test.cpp'
symbol='?SetEnvironmentalEffectsAlwaysUpdate@CEngine@@UAEX_N@Z';global_symbol='?GPEngineEnvironment@NEngineInternal@@3PAVCEngineEnvironment@@A'
aliases={'@SetEnvironmentFlag@12':symbol,'_EnvironmentGlobal':global_symbol}
(w/'engine_environment_flag_aliases.h').write_text('\n'.join('#pragma comment(linker,"/alternatename:'+a+'='+b+'")'for a,b in aliases.items())+'\n')
objects=[]
for i,path in enumerate(sources+[fixture]):
    obj=w/(str(i)+'.obj');r=V.cl(['/nologo','/c','/O2','/MT','/I'+str(w),'/Fo'+str(obj),str(path)],env,w);assert not r.returncode,r.stdout+r.stderr;objects.append(obj)
proof=check(objects[0],symbol,0xb23b90,19,1,{global_symbol:0x1436e48})
c=Coff(objects[1]);raw=EXE.read_bytes()
assert coff_data(c,global_symbol,4)==read_virtual_data(raw,0x1436e48,4)==bytes(4)
assert not any(s.startswith('_$E')or s.startswith('_$S')for s in c.symbols)
exe=w/'fixture.exe';cmd=[str(V.VC/'bin/link.exe'),'/nologo','/out:'+str(exe),'kernel32.lib']
for omitted in (0,1):
    r=subprocess.run(cmd+[str(p)for i,p in enumerate(objects)if i!=omitted],env=env,capture_output=True,text=True);assert r.returncode
r=subprocess.run(cmd+list(map(str,objects)),env=env,capture_output=True,text=True);assert not r.returncode,r.stdout+r.stderr
BASE=0x24000000;GLOBAL=0x1436e48
u=Uc(UC_ARCH_X86,UC_MODE_32);u.mem_map(0x400000,0x1200000);u.mem_map(BASE,0x10000)
for _,va,_,off,n in pe_sections(raw):
    if n:u.mem_write(0x400000+va,raw[off:off+n])
inputs=bytearray();expected=bytearray();cases=0
for fill,self,owner,value in itertools.product(range(256),(0,BASE,BASE+0x800),(BASE,BASE+0x800),(0,1)):
    arena=bytes([fill])*4096;inputs.extend(struct.pack('<3I',self,owner,value));inputs.extend(arena)
    u.mem_write(BASE,arena);u.mem_write(GLOBAL,struct.pack('<I',owner));sp=BASE+0xe000;stop=BASE+0xf000
    u.mem_write(sp,struct.pack('<2I',stop,value));u.reg_write(UC_X86_REG_ECX,self);u.reg_write(UC_X86_REG_ESP,sp)
    u.emu_start(0xb23b90,stop,count=20);assert u.reg_read(UC_X86_REG_ESP)==sp+8
    expected.extend(u.mem_read(GLOBAL,4));expected.extend(u.mem_read(BASE,4096));cases+=1
(w/'input.bin').write_bytes(inputs)
r=subprocess.run([str(exe),str(w/'input.bin'),str(w/'actual.bin')],env=env,capture_output=True,text=True,timeout=60);assert not r.returncode,(w,r.returncode,r.stdout,r.stderr)
assert (w/'actual.bin').read_bytes()==expected,w
report=dict(cases=cases,snapshots=cases,omission_checks=2,providers=[proof],source_sha256={str(p.relative_to(ROOT)):hashlib.sha256(p.read_bytes()).hexdigest()for p in sources},fixture_sha256=hashlib.sha256(fixture.read_bytes()).hexdigest(),boundary='Actual global and full4KiB arena, all256fills, both booleans, two global targets and null/distinct/alias receiver. No callbacks, normalization or masked bytes; exact one-byte store preserves all neighboring state.')
(ROOT/'rebuild/compile-gate/native-engine-environment-flag.json').write_text(json.dumps(report,indent=2)+'\n');print(r.stdout.strip())
