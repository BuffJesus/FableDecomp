"""Typed camera FOV reads, raw x87 results and unchanged surrounding storage."""
import hashlib,itertools,json,struct,subprocess,tempfile
from pathlib import Path
from unicorn import Uc,UC_ARCH_X86,UC_MODE_32
from unicorn.x86_const import *
import check_one as V
from variable_allocator_parity import check
from pe_oracle import EXE,pe_sections,read_virtual_data
ROOT=Path(__file__).resolve().parents[2];w=Path(tempfile.mkdtemp(prefix='native-camera-fov-'));env=V.env()
source=ROOT/'rebuild/src/compiled/00/a0/CCamera_FOVGetters_00a0be80.cpp';fixture=ROOT/'rebuild/tests/integration/NativeCameraFOVGetters_test.cpp'
specs=[('?GetFOVFlags@CCamera@@QBEKXZ',0xa0be80),('?GetHorizontalFOV@CCamera@@QBEMXZ',0xa0be90),('?GetVerticalFOV@CCamera@@QBEMXZ',0xa0bea0)]
raw=EXE.read_bytes()
(w/'camera_fov_retail_oracle.h').write_text('\n'.join('static const unsigned char '+name+'[]={'+','.join(str(b)for b in read_virtual_data(raw,address,4))+'};'for name,(_,address)in zip(('retailFlags','retailHorizontal','retailVertical'),specs))+'\n')
(w/'camera_fov_getters_aliases.h').write_text('\n'.join('#pragma comment(linker,"/alternatename:'+alias+'='+symbol+'")'for alias,(symbol,_)in zip(('@Flags@4','@Horizontal@4','@Vertical@4'),specs))+'\n')
objects=[]
for i,path in enumerate((source,fixture)):
    obj=w/(str(i)+'.obj');r=V.cl(['/nologo','/c','/O2','/MT','/I'+str(w),'/Fo'+str(obj),str(path)],env,w);assert not r.returncode,r.stdout+r.stderr;objects.append(obj)
proof=[check(objects[0],symbol,address,4,0,{})for symbol,address in specs]
exe=w/'fixture.exe';cmd=[str(V.VC/'bin/link.exe'),'/nologo','/out:'+str(exe),'kernel32.lib']
r=subprocess.run(cmd+[str(objects[1])],env=env,capture_output=True,text=True);assert r.returncode
r=subprocess.run(cmd+list(map(str,objects)),env=env,capture_output=True,text=True);assert not r.returncode,r.stdout+r.stderr
BASE=0x24000000;u=Uc(UC_ARCH_X86,UC_MODE_32);u.mem_map(0x400000,0x1200000);u.mem_map(BASE,0x10000);raw=EXE.read_bytes()
for _,va,_,off,n in pe_sections(raw):
    if n:u.mem_write(0x400000+va,raw[off:off+n])
# Test-only return adapter stores native x87 ST0 exactly as the C++ caller does.
u.mem_write(BASE+0xf000,b'\xd9\x1d'+struct.pack('<I',BASE+0xd000)+b'\xc3')
values=(0,0x80000000,1,0x80000001,0x007fffff,0x00800000,0x3f800000,0xbf800000,0x7f7fffff,0x7f800000,0xff800000,0x7fc12345,0x7f812345,0xff812345)
inputs=bytearray();expected=bytearray();cases=0;emulator_cases=[]
for fill,index,control in itertools.product(range(256),range(len(values)),range(2)):
    arena=bytearray([fill])*256;flags=values[(index+5)%len(values)];struct.pack_into('<3I',arena,64+0x28,flags,values[index],values[(index+3)%len(values)])
    inputs.extend(struct.pack('<I',control));inputs.extend(arena);u.mem_write(BASE,bytes(arena))
    for _,address in specs:
        sp=BASE+0xe000;stop=BASE+0xf100
        u.mem_write(sp,struct.pack('<2I',stop if address==0xa0be80 else BASE+0xf000,stop));u.reg_write(UC_X86_REG_ESP,sp);u.reg_write(UC_X86_REG_ECX,BASE+64);u.reg_write(UC_X86_REG_FPCW,0x37f if control else 0x27f);u.reg_write(UC_X86_REG_FPSW,0);u.reg_write(UC_X86_REG_FPTAG,0xffff)
        u.emu_start(address,stop,count=20);assert u.reg_read(UC_X86_REG_ESP)==sp+(4 if address==0xa0be80 else 8)
        expected.extend(struct.pack('<I',u.reg_read(UC_X86_REG_EAX))if address==0xa0be80 else u.mem_read(BASE+0xd000,4))
    expected.extend(u.mem_read(BASE,256));cases+=1
    # Unicorn preserves the signaling bit on FLD/FSTP while native x87 quiets
    # it. Every case is compared against actual retail execution below; only
    # non-signaling inputs receive this additional emulator cross-check.
    def signaling(v):return v&0x7f800000==0x7f800000 and v&0x007fffff and not v&0x00400000
    if not any(signaling(v)for v in (values[index],values[(index+3)%len(values)])):emulator_cases.append(cases-1)
(w/'input.bin').write_bytes(inputs)
r=subprocess.run([str(exe),str(w/'input.bin'),str(w/'retail.bin'),'retail'],env=env,capture_output=True,text=True,timeout=60);assert not r.returncode,(w,r.returncode,r.stdout,r.stderr)
native=(w/'retail.bin').read_bytes();assert len(native)==len(expected)
assert all(native[i*268:(i+1)*268]==expected[i*268:(i+1)*268]for i in emulator_cases),w
r=subprocess.run([str(exe),str(w/'input.bin'),str(w/'actual.bin'),'compiled'],env=env,capture_output=True,text=True,timeout=60);assert not r.returncode,(w,r.returncode,r.stdout,r.stderr)
actual=(w/'actual.bin').read_bytes()
if actual!=native:
    difference=next((i for i,(a,b)in enumerate(zip(actual,native))if a!=b),min(len(actual),len(native)));raise AssertionError((w,difference,difference//268,difference%268))
report=dict(cases=cases,snapshots=cases,emulator_cross_checks=len(emulator_cases),omission_checks=1,providers=proof,source_sha256=hashlib.sha256(source.read_bytes()).hexdigest(),fixture_sha256=hashlib.sha256(fixture.read_bytes()).hexdigest(),boundary='Actual compiled leaf providers versus original retail bytes executed natively; full256-byte arena plus flags and stored float returns. All256fills,53/64-bit x87 precision, signed zero, subnormal, finite extremes, infinities, quiet/signaling NaNs and high flag bits. Non-signaling inputs additionally cross-checked with Unicorn; signaling inputs use native retail execution because Unicorn does not quiet FLD/FSTP signaling NaNs. No application call boundaries or normalized bytes.')
(ROOT/'rebuild/compile-gate/native-camera-fov-getters.json').write_text(json.dumps(report,indent=2)+'\n');print(r.stdout.strip())
