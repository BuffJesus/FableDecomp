"""Shader world/view/projection copies and actual matrix transpose composition."""
import hashlib,itertools,json,struct,subprocess,tempfile
from pathlib import Path
from unicorn import Uc,UC_ARCH_X86,UC_MODE_32
from unicorn.x86_const import *
import check_one as V
from variable_allocator_parity import check
from pe_oracle import EXE,pe_sections
ROOT=Path(__file__).resolve().parents[2];w=Path(tempfile.mkdtemp(prefix='native-shader-matrices-'));env=V.env()
sources=[ROOT/'rebuild/src/compiled'/p for p in ('00/98/CShaderRenderManager_MatrixSetters_009881f0.cpp','00/a5/CMatrix4x4_Transpose_00a5cab0.cpp')]
fixture=ROOT/'rebuild/tests/integration/NativeShaderMatrices_test.cpp'
specs=[('?SetWorldTransform@CShaderRenderManager@@QAEXABVCMatrix3x4@@@Z',0x9881f0,160,0),('?SetViewTransform@CShaderRenderManager@@QAEXABVCMatrix3x4@@@Z',0x988350,154,0),('?SetProjectionTransform@CShaderRenderManager@@QAEXABVCMatrix4x4@@@Z',0x988540,54,1),('?Transpose@CMatrix4x4@@QAEXXZ',0xa5cab0,74,0)]
(w/'shader_matrices_aliases.h').write_text('\n'.join('#pragma comment(linker,"/alternatename:'+name+'='+s[0]+'")'for name,s in zip(('@World@12','@View@12','@Projection@12','@Transpose@4'),specs))+'\n')
objects=[]
for i,path in enumerate(sources+[fixture]):
    obj=w/(str(i)+'.obj');r=V.cl(['/nologo','/c','/O2','/MT','/I'+str(w),'/Fo'+str(obj),str(path)],env,w);assert not r.returncode,r.stdout+r.stderr;objects.append(obj)
proof=[check(objects[int(i==3)],symbol,address,size,count,{specs[3][0]:0xa5cab0})for i,(symbol,address,size,count)in enumerate(specs)]
exe=w/'fixture.exe';cmd=[str(V.VC/'bin/link.exe'),'/nologo','/out:'+str(exe),'kernel32.lib']
for omit in range(2):
    r=subprocess.run(cmd+[str(p)for i,p in enumerate(objects)if i!=omit],env=env,capture_output=True,text=True);assert r.returncode
r=subprocess.run(cmd+list(map(str,objects)),env=env,capture_output=True,text=True);assert not r.returncode,r.stdout+r.stderr
BASE=0x24000000;u=Uc(UC_ARCH_X86,UC_MODE_32);u.mem_map(0x400000,0x1200000);u.mem_map(BASE,0x10000);raw=EXE.read_bytes()
for _,va,_,off,n in pe_sections(raw):
    if n:u.mem_write(0x400000+va,raw[off:off+n])
inputs=bytearray();expected=bytearray();cases=snapshots=0
values=(0,0x80000000,1,0x3f800000,0xbf800000,0x7f800000,0xff800000,0x7fc12345,0x7f812345,0xff812345,0xdeadbeef)
for fill,op,offset,flags in itertools.product(range(256),range(4),(0,0x1ec,0x1f0,0x230,0x26c,0x270,0x274,0x380,0x3d8,0x600),(0,0xc990,0x12345678,0xffffffff)):
    arena=bytearray([fill])*2048;struct.pack_into('<I',arena,0x3d8,flags)
    struct.pack_into('<16I',arena,offset,*[values[(fill+i)%len(values)]for i in range(16)])
    inputs.extend(struct.pack('<2I',op,offset));inputs.extend(arena);u.mem_write(BASE,bytes(arena))
    for repeat in range(2 if op==3 else 1):
        sp=BASE+0xe000+(fill%4)*4;stop=BASE+0xf000;u.mem_write(sp,struct.pack('<2I',stop,BASE+offset));u.reg_write(UC_X86_REG_ESP,sp);u.reg_write(UC_X86_REG_ECX,BASE+offset if op==3 else BASE)
        saved=[(UC_X86_REG_EBX,0x11223344),(UC_X86_REG_ESI,0x55667788),(UC_X86_REG_EDI,0xaabbccdd),(UC_X86_REG_EBP,0x87654321)]
        for reg,value in saved:u.reg_write(reg,value)
        u.emu_start(specs[op][1],stop,count=300);assert u.reg_read(UC_X86_REG_ESP)==sp+(4 if op==3 else 8);assert all(u.reg_read(reg)==value for reg,value in saved)
        expected.extend(u.mem_read(BASE,2048));snapshots+=1
    cases+=1
(w/'input.bin').write_bytes(inputs)
r=subprocess.run([str(exe),str(w/'input.bin'),str(w/'actual.bin')],env=env,capture_output=True,text=True,timeout=60);assert not r.returncode,(w,r.returncode,r.stdout,r.stderr)
actual=(w/'actual.bin').read_bytes()
if actual!=expected:
    diff=next((i for i,(a,b)in enumerate(zip(actual,expected))if a!=b),min(len(actual),len(expected)));raise AssertionError((w,diff,diff//2048,diff%2048))
report=dict(cases=cases,snapshots=snapshots,omission_checks=2,providers=proof,source_sha256={str(p.relative_to(ROOT)):hashlib.sha256(p.read_bytes()).hexdigest()for p in sources},fixture_sha256=hashlib.sha256(fixture.read_bytes()).hexdigest(),boundary='Full2KiB shader/argument state, all256fills,ten source placements including each matrix destination and flag storage aliases,four update masks,raw float bit patterns including signalingNaNs. Direct transpose and double-transpose exercise aligned local copy with four incoming stack alignments. Projection executes actual transpose; no callbacks or normalized bytes; retail callee-saved registers and stack cleanup checked.')
(ROOT/'rebuild/compile-gate/native-shader-matrices.json').write_text(json.dumps(report,indent=2)+'\n');print(r.stdout.strip(),snapshots,'snapshots')
