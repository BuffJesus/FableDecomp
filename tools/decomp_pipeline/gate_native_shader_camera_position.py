"""Native shader camera-position copy and update-mask preservation."""
import hashlib,itertools,json,struct,subprocess,tempfile
from pathlib import Path
from unicorn import Uc,UC_ARCH_X86,UC_MODE_32
from unicorn.x86_const import *
import check_one as V
from variable_allocator_parity import check
from pe_oracle import EXE,pe_sections
ROOT=Path(__file__).resolve().parents[2];w=Path(tempfile.mkdtemp(prefix='native-shader-camera-position-'));env=V.env()
source=ROOT/'rebuild/src/compiled/00/98/CShaderRenderManager_SetCameraPos_00988320.cpp';fixture=ROOT/'rebuild/tests/integration/NativeShaderCameraPosition_test.cpp';symbol='?SetCameraPos@CShaderRenderManager@@QAEXABVC3DVector@@@Z'
(w/'shader_camera_position_aliases.h').write_text('#pragma comment(linker,"/alternatename:@SetPosition@12='+symbol+'")\n')
objects=[]
for i,path in enumerate((source,fixture)):
    obj=w/(str(i)+'.obj');r=V.cl(['/nologo','/c','/O2','/MT','/I'+str(w),'/Fo'+str(obj),str(path)],env,w);assert not r.returncode,r.stdout+r.stderr;objects.append(obj)
proof=check(objects[0],symbol,0x988320,38,0,{})
exe=w/'fixture.exe';cmd=[str(V.VC/'bin/link.exe'),'/nologo','/out:'+str(exe),'kernel32.lib']
r=subprocess.run(cmd+[str(objects[1])],env=env,capture_output=True,text=True);assert r.returncode
r=subprocess.run(cmd+list(map(str,objects)),env=env,capture_output=True,text=True);assert not r.returncode,r.stdout+r.stderr
BASE=0x24000000;u=Uc(UC_ARCH_X86,UC_MODE_32);u.mem_map(0x400000,0x1200000);u.mem_map(BASE,0x10000);raw=EXE.read_bytes()
for _,va,_,off,n in pe_sections(raw):
    if n:u.mem_write(0x400000+va,raw[off:off+n])
inputs=bytearray();expected=bytearray();cases=0
values=(0,0x80000000,1,0x3f800000,0xbf800000,0x7f800000,0xff800000,0x7fc12345,0x7f812345)
for fill,offset,flags in itertools.product(range(256),(0,0x200,0x378,0x37c,0x380,0x384,0x388,0x3d0,0x3d8),(0,8,0x12345678,0xffffffff)):
    arena=bytearray([fill])*2048;struct.pack_into('<I',arena,0x3d8,flags)
    struct.pack_into('<3I',arena,offset,*[values[(fill+i)%len(values)]for i in range(3)])
    inputs.extend(struct.pack('<I',offset));inputs.extend(arena);u.mem_write(BASE,bytes(arena));sp=BASE+0xe000;stop=BASE+0xf000
    u.mem_write(sp,struct.pack('<2I',stop,BASE+offset));u.reg_write(UC_X86_REG_ESP,sp);u.reg_write(UC_X86_REG_ECX,BASE);u.reg_write(UC_X86_REG_ESI,0x12345678)
    u.emu_start(0x988320,stop,count=40);assert u.reg_read(UC_X86_REG_ESP)==sp+8 and u.reg_read(UC_X86_REG_ESI)==0x12345678
    expected.extend(u.mem_read(BASE,2048));cases+=1
(w/'input.bin').write_bytes(inputs)
r=subprocess.run([str(exe),str(w/'input.bin'),str(w/'actual.bin')],env=env,capture_output=True,text=True,timeout=60);assert not r.returncode,(w,r.returncode,r.stdout,r.stderr)
assert (w/'actual.bin').read_bytes()==expected,w
report=dict(cases=cases,snapshots=cases,omission_checks=1,providers=[proof],source_sha256=hashlib.sha256(source.read_bytes()).hexdigest(),fixture_sha256=hashlib.sha256(fixture.read_bytes()).hexdigest(),boundary='Full2KiB state,all256fills,nine source placements including exact/partial overlap and source aliasing update mask,four flag patterns,raw float bit patterns incl signalingNaNs. Native retail stack and ESI preserved. No callbacks or normalized bytes.')
(ROOT/'rebuild/compile-gate/native-shader-camera-position.json').write_text(json.dumps(report,indent=2)+'\n');print(r.stdout.strip())
