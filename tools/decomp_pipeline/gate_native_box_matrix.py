"""Bounding-box matrix calculation and actual engine ClipBox composition."""
import hashlib,itertools,json,struct,subprocess,tempfile
from pathlib import Path
from capstone import Cs,CS_ARCH_X86,CS_MODE_32
import check_one as V
from gate_game_component_abi import Coff
from gate_render_state_static_data import coff_data
from variable_allocator_parity import check
from pe_oracle import EXE,read_virtual_data
ROOT=Path(__file__).resolve().parents[2];w=Path(tempfile.mkdtemp(prefix='native-box-matrix-'));env=V.env()
sources=[ROOT/'rebuild/src/compiled'/p for p in ('00/b2/CEngine_ClipBox_00b242a0.cpp','00/ad/C3DBoundingBox_CalculateMatrix_00adebc0.cpp','01/43/NEngineInternal_GPCamera_01436ea0.cpp','01/29/Fable_DeltaNotionalZero_0129ba3c.cpp')];fixture=ROOT/'rebuild/tests/integration/NativeBoxMatrix_test.cpp'
parent='?ClipBox@CEngine@@UBE_NABVC3DBoundingBox@@@Z';matrix='?CalculateMatrix@C3DBoundingBox@@QBEXAAVCMatrix3x4@@@Z';camera='?GPCamera@NEngineInternal@@3PAVCEngineCamera@@A';delta='?DELTA_NOTIONAL_ZERO@@3MB';boundary='?ClipBox@CEngineCamera@@QBE_NABVCMatrix3x4@@@Z'
targets={camera:0x1436ea0,delta:0x129ba3c,matrix:0xadebc0,boundary:0xb30610,'__real@3f000000':0x122f59c}
aliases={'@Calculate@12':matrix,'@Clip@12':parent,'_CameraGlobal':camera,'_Delta':delta,boundary:'@ClipBoundary@12'}
(w/'box_matrix_aliases.h').write_text('\n'.join('#pragma comment(linker,"/alternatename:'+a+'='+b+'")'for a,b in aliases.items())+'\n')
raw=EXE.read_bytes();code=bytearray(512);fixups=[];m=Cs(CS_ARCH_X86,CS_MODE_32);m.detail=True
for i,(address,size)in enumerate(((0xb242a0,37),(0xadebc0,242))):
    body=read_virtual_data(raw,address,size);code[256*i:256*i+size]=body
    for ins in m.disasm(body,address):
        if ins.mnemonic=='call':fixups.append((256*i+ins.address-address+1,ins.operands[0].imm,1))
        for op in ins.operands:
            if op.type==3 and op.mem.disp>=0x1220000:fixups.append((256*i+ins.address-address+ins.disp_offset,op.mem.disp,0))
assert len(fixups)==15 and all(t in targets.values()for _,t,_ in fixups)
(w/'box_matrix_retail.h').write_text('static const unsigned char retailCode[]={'+','.join(str(b)for b in code)+'};\nstatic const unsigned retailFixups[][3]={'+','.join('{%d,%d,%d}'%f for f in fixups)+'};\n')
objects=[]
for i,path in enumerate(sources+[fixture]):
    obj=w/(str(i)+'.obj');r=V.cl(['/nologo','/c','/O2','/MT','/I'+str(w),'/Fo'+str(obj),str(path)],env,w);assert not r.returncode,r.stdout+r.stderr;objects.append(obj)
proofs=[check(objects[0],parent,0xb242a0,37,3,targets),check(objects[1],matrix,0xadebc0,242,12,targets)]
for i,symbol in ((2,camera),(3,delta)):assert coff_data(Coff(objects[i]),symbol,4)==read_virtual_data(raw,targets[symbol],4)
assert coff_data(Coff(objects[2]),camera,4)==bytes(4)
assert coff_data(Coff(objects[3]),delta,4)==struct.pack('<f',0.0001)
exe=w/'fixture.exe';cmd=[str(V.VC/'bin/link.exe'),'/nologo','/out:'+str(exe),'kernel32.lib']
for omitted in range(4):
    r=subprocess.run(cmd+[str(p)for i,p in enumerate(objects)if i!=omitted],env=env,capture_output=True,text=True);assert r.returncode
r=subprocess.run(cmd+list(map(str,objects)),env=env,capture_output=True,text=True);assert not r.returncode,r.stdout+r.stderr
inputs=bytearray();cases=snapshots=matrices=0;values=(0,0x80000000,1,0x80000001,0x3951b716,0x3951b717,0x3951b718,0x3f800000,0xbf800000,0x40000000,0x7f7fffff,0xff7fffff,0x7f800000,0xff800000,0x7fc12345,0x7f812345,0xff812345)
scenarios=list(itertools.product((0,0x55,0xff),range(7),range(len(values)),range(len(values)),(0,0x10000)))
scenarios += [(0x55,kind,5,7,precision|rounding)for kind,precision,rounding in itertools.product(range(7),(0,0x10000),(0x100,0x200,0x300))]
for fill,kind,a,b,control in scenarios:
    arena=bytearray([fill])*8192
    coords=[values[a],values[(a+3)%len(values)],values[(a+7)%len(values)],values[b],values[(b+3)%len(values)],values[(b+7)%len(values)]]
    struct.pack_into('<6I',arena,0x1000,*coords);dest=(0x1200,0x1000,0x1004,0xffc,0x100c,0,0)[kind]
    inputs.extend(struct.pack('<3I',kind,dest,control));inputs.extend(arena);cases+=1;snapshots+=1+(kind>=5);matrices+=kind>=5
(w/'input.bin').write_bytes(inputs)
for kind in ('retail','compiled'):
    r=subprocess.run([str(exe),str(w/'input.bin'),str(w/(kind+'.bin')),kind],env=env,capture_output=True,text=True,timeout=60);assert not r.returncode,(w,r.returncode,r.stdout,r.stderr);print(r.stdout.strip(),kind,flush=True)
actual=(w/'compiled.bin').read_bytes();expected=(w/'retail.bin').read_bytes()
if actual!=expected:
    diff=next((i for i,(a,b)in enumerate(zip(actual,expected))if a!=b),min(len(actual),len(expected)));raise AssertionError((w,diff,len(actual),len(expected)))
assert len(actual)==snapshots*(8192+16)+matrices*48
report=dict(cases=cases,snapshots=snapshots,matrix_snapshots=matrices,omission_checks=4,providers=proofs,source_sha256={str(p.relative_to(ROOT)):hashlib.sha256(p.read_bytes()).hexdigest()for p in sources},fixture_sha256=hashlib.sha256(fixture.read_bytes()).hexdigest(),boundary='Original retail bodies execute natively after15 audited fixups;direct box-to-matrix calculation and actual engine wrapper composition. Full8KiB arenas,floating status,actual camera global and DELTA constant,five separate/overlapping destinations,17x17 coordinate patterns,three fills,53/64bit precision and extra four-rounding-mode coverage. Engine-camera ClipBox remains recorded:matrix argument,receiver,boolean return and mutating callback observed. No normalized bytes.')
(ROOT/'rebuild/compile-gate/native-box-matrix.json').write_text(json.dumps(report,indent=2)+'\n');print('NATIVE_BOX_MATRIX_GATE_PASS',snapshots,'snapshots',matrices,'matrices')
