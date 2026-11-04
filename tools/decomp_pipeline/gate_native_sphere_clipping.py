"""Native sphere clipping, including unordered comparisons and plane order."""
import hashlib,itertools,json,struct,subprocess,tempfile
from pathlib import Path
from capstone import Cs,CS_ARCH_X86,CS_MODE_32
import check_one as V
from gate_game_component_abi import Coff
from gate_render_state_static_data import coff_data
from variable_allocator_parity import check
from pe_oracle import EXE,read_virtual_data
ROOT=Path(__file__).resolve().parents[2];w=Path(tempfile.mkdtemp(prefix='native-sphere-clipping-'));env=V.env()
sources=[ROOT/'rebuild/src/compiled'/p for p in ('00/b2/CEngine_ClipSphere_00b24150.cpp','01/43/NEngineInternal_GPCamera_01436ea0.cpp')];fixture=ROOT/'rebuild/tests/integration/NativeSphereClipping_test.cpp'
symbol='?ClipSphere@CEngine@@UBE_NABVC3DBoundingSphere@@@Z';global_symbol='?GPCamera@NEngineInternal@@3PAVCEngineCamera@@A';aliases={'@ClipSphere@12':symbol,'_CameraGlobal':global_symbol}
(w/'sphere_clipping_aliases.h').write_text('\n'.join('#pragma comment(linker,"/alternatename:'+a+'='+b+'")'for a,b in aliases.items())+'\n')
raw=EXE.read_bytes();code=read_virtual_data(raw,0xb24150,327);fixups=[];m=Cs(CS_ARCH_X86,CS_MODE_32);m.detail=True
for ins in m.disasm(code,0xb24150):
    for op in ins.operands:
        if op.type==3 and op.mem.disp>=0x1220000:
            assert op.mem.disp==0x1436ea0;fixups.append(ins.address-0xb24150+ins.disp_offset)
assert len(fixups)==1
(w/'sphere_clipping_retail.h').write_text('static const unsigned char retailCode[]={'+','.join(str(b)for b in code)+'};\nstatic const unsigned cameraFixup=%d;\n'%fixups[0])
objects=[]
for i,path in enumerate(sources+[fixture]):
    obj=w/(str(i)+'.obj');r=V.cl(['/nologo','/c','/O2','/MT','/I'+str(w),'/Fo'+str(obj),str(path)],env,w);assert not r.returncode,r.stdout+r.stderr;objects.append(obj)
proof=check(objects[0],symbol,0xb24150,327,1,{global_symbol:0x1436ea0})
assert coff_data(Coff(objects[1]),global_symbol,4)==read_virtual_data(raw,0x1436ea0,4)==bytes(4)
exe=w/'fixture.exe';cmd=[str(V.VC/'bin/link.exe'),'/nologo','/out:'+str(exe),'kernel32.lib']
for omitted in range(2):
    r=subprocess.run(cmd+[str(p)for i,p in enumerate(objects)if i!=omitted],env=env,capture_output=True,text=True);assert r.returncode
r=subprocess.run(cmd+list(map(str,objects)),env=env,capture_output=True,text=True);assert not r.returncode,r.stdout+r.stderr
inputs=bytearray();cases=0
def arena_for(fill,camera):
    arena=bytearray([fill])*4096;struct.pack_into('<3f',arena,camera+0x54,0,0,0);struct.pack_into('<2f',arena,camera+0x6c,0.5,10)
    struct.pack_into('<3f',arena,camera+0x1b4,0,0,1)
    for i,plane in enumerate(((0,1,-0.5,0),(0,-1,-0.5,0),(1,0,-0.5,0),(-1,0,-0.5,0))):struct.pack_into('<4f',arena,camera+0x1c0+16*i,*plane)
    return arena
def add(arena,camera,sphere,control):
    global cases
    inputs.extend(struct.pack('<3I',camera,sphere,control));inputs.extend(arena);cases+=1
for (x,y,z,r),precision,rounding in itertools.product(itertools.product((-2.,-1.,0.,1.,2.,4.),repeat=4),(0,0x10000),(0,0x100,0x200,0x300)):
    arena=arena_for(0x55,0);struct.pack_into('<4f',arena,0xe00,x,y,z,r);add(arena,0,0xe00,precision|rounding)
values=(0,0x80000000,1,0x80000001,0x007fffff,0x00800000,0x3e800000,0x3f000000,0x3f800000,0xbf800000,0xbf800001,0x7f7fffff,0xff7fffff,0x7f800000,0xff800000,0x7fc12345,0x7f812345)
for fill,radius,value,field,precision in itertools.product((0,0x55,0xff),values,values,(0x54,0x6c,0x70,0x1b4,0x1c0,0x1d4,0x1e8,0x1f0),(0,0x10000)):
    camera=0x800 if fill&1 else 0;arena=arena_for(fill,camera);struct.pack_into('<3fI',arena,0xe00,0.5,0.25,2.0,radius);struct.pack_into('<I',arena,camera+field,value);add(arena,camera,0xe00,precision)
for fill,value,offset,precision in itertools.product((0,0x55,0xff),values,(0x54,0x1b4,0x1c0),(0,0x10000)):
    arena=arena_for(fill,0);struct.pack_into('<3fI',arena,offset,0.5,0.25,2.0,value);add(arena,0,offset,precision)
(w/'input.bin').write_bytes(inputs)
for kind in ('retail','compiled'):
    r=subprocess.run([str(exe),str(w/'input.bin'),str(w/(kind+'.bin')),kind],env=env,capture_output=True,text=True,timeout=60);assert not r.returncode,(w,r.returncode,r.stdout,r.stderr);print(r.stdout.strip(),kind,flush=True)
actual=(w/'compiled.bin').read_bytes();expected=(w/'retail.bin').read_bytes();assert actual==expected,(w,'sphere clipping');assert len(actual)==cases*(4096+16)
report=dict(cases=cases,snapshots=cases,omission_checks=2,providers=[proof],source_sha256={str(p.relative_to(ROOT)):hashlib.sha256(p.read_bytes()).hexdigest()for p in sources},fixture_sha256=hashlib.sha256(fixture.read_bytes()).hexdigest(),boundary='Original327byte retail body executes natively after one audited global relocation. Full4KiB arena,boolean result and floating status/control;finite6^4 center/radius grid across53/64bit precision and four rounding modes,17x17 exceptional radius/camera-field pairs across eight camera fields,three fills,two owners and separate/overlapping sphere storage. All near/far/side-plane comparisons execute original ordering and unordered rejection semantics. No callbacks or normalized bytes.')
(ROOT/'rebuild/compile-gate/native-sphere-clipping.json').write_text(json.dumps(report,indent=2)+'\n');print('NATIVE_SPHERE_CLIPPING_GATE_PASS',cases,'snapshots')
