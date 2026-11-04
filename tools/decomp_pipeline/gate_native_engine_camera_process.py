"""Camera processing compared with original retail code executing natively."""
import hashlib,itertools,json,struct,subprocess,tempfile
from pathlib import Path
import check_one as V
from gate_game_component_abi import Coff
from gate_render_state_static_data import coff_data
from variable_allocator_parity import check
from pe_oracle import EXE,read_virtual_data
from capstone import Cs,CS_ARCH_X86,CS_MODE_32
ROOT=Path(__file__).resolve().parents[2];w=Path(tempfile.mkdtemp(prefix='native-camera-process-'));env=V.env()
source=ROOT/'rebuild/src/compiled/00/b3/CEngineCamera_Process_00b314e0.cpp';getters=ROOT/'rebuild/src/compiled/00/a0/CCamera_FOVGetters_00a0be80.cpp';normalizer=ROOT/'rebuild/src/compiled/00/a1/C3DVector_AccurateNormalise_00a14440.cpp';settings=ROOT/'rebuild/src/compiled/01/39/EngineCamera_ZBufferStart_01399d44.cpp';fixture=ROOT/'rebuild/tests/integration/NativeEngineCameraProcess_test.cpp'
raw=EXE.read_bytes();entry=0xb314e0;code=read_virtual_data(raw,entry,535);m=Cs(CS_ARCH_X86,CS_MODE_32);m.detail=True;fixups=[]
for ins in m.disasm(code,entry):
    if ins.mnemonic=='call':fixups.append((ins.address-entry+1,ins.operands[0].imm,1))
    for op in ins.operands:
        if op.type==3 and op.mem.disp>=0x1220000:fixups.append((ins.address-entry+ins.disp_offset,op.mem.disp,0))
assert len(fixups)==16,fixups
constants=[struct.unpack('<I',read_virtual_data(raw,a,4))[0]for a in (0x1238020,0x1238e00,0x128f608)]
zstart=struct.unpack('<I',read_virtual_data(raw,0x1399d44,4))[0]
(w/'engine_camera_process_retail.h').write_text('static const unsigned char retailCode[]={'+','.join(str(b)for b in code)+'};\nstatic const unsigned retailFixups[][3]={'+','.join('{%d,%d,%d}'%f for f in fixups)+'};\nstatic const unsigned retailConstants[]={'+','.join(str(v)+'u'for v in constants)+'};\n')
with (w/'engine_camera_process_retail.h').open('a')as out:out.write('static const unsigned retailZStart=%du;\n'%zstart)
aliases={'@ProcessCamera@12':'?Process@CEngineCamera@@QAEX_N@Z','?ProcessDebugScript@CEngineCamera@@AAEXXZ':'@DebugBoundary@4','?SetCameraState@CEngineCamera@@QAEXABUCEngineCameraDesc@@_N1@Z':'@StateBoundary@20','?GFVBoxToABox@@YIXPBVC2DBoxF@@PAV1@@Z':'@BoxBoundary@8','?ZBUFFER_PARTITION_MAIN_SCENE_START@@3MB':'_ZStart','@Flags@4':'?GetFOVFlags@CCamera@@QBEKXZ','@Horizontal@4':'?GetHorizontalFOV@CCamera@@QBEMXZ','@Vertical@4':'?GetVerticalFOV@CCamera@@QBEMXZ','@Normalize@4':'?AccurateNormalise@C3DVector@@QAEAAV1@XZ'}
del aliases['?ZBUFFER_PARTITION_MAIN_SCENE_START@@3MB'];aliases['_ZStart']='?ZBUFFER_PARTITION_MAIN_SCENE_START@@3MB'
(w/'engine_camera_process_aliases.h').write_text('\n'.join('#pragma comment(linker,"/alternatename:'+a+'='+b+'")'for a,b in aliases.items())+'\n')
objects=[]
for i,path in enumerate((source,getters,normalizer,settings,fixture)):
    obj=w/(str(i)+'.obj');r=V.cl(['/nologo','/c','/O2','/MT','/I'+str(w),'/Fo'+str(obj),str(path)],env,w);assert not r.returncode,r.stdout+r.stderr;objects.append(obj)
targets={'?ProcessDebugScript@CEngineCamera@@AAEXXZ':0xb31160,'?SetCameraState@CEngineCamera@@QAEXABUCEngineCameraDesc@@_N1@Z':0xb30b50,'?ZBUFFER_PARTITION_MAIN_SCENE_START@@3MB':0x1399d44,'?GFVBoxToABox@@YIXPBVC2DBoxF@@PAV1@@Z':0x9e1e70,'?GetHorizontalFOV@CCamera@@QBEMXZ':0xa0be90,'?GetVerticalFOV@CCamera@@QBEMXZ':0xa0bea0,'?GetFOVFlags@CCamera@@QBEKXZ':0xa0be80,'?AccurateNormalise@C3DVector@@QAEAAV1@XZ':0xa14440,'__real@43b40000':0x1238020,'__real@3b360b61':0x1238e00,'__real@40c90fdb':0x128f608}
proof=check(objects[0],'?Process@CEngineCamera@@QAEX_N@Z',entry,535,16,targets)
assert coff_data(Coff(objects[3]),'?ZBUFFER_PARTITION_MAIN_SCENE_START@@3MB',4)==read_virtual_data(raw,0x1399d44,4)
exe=w/'fixture.exe';cmd=[str(V.VC/'bin/link.exe'),'/nologo','/out:'+str(exe),'kernel32.lib']
for omitted in range(4):
    r=subprocess.run(cmd+[str(p)for i,p in enumerate(objects)if i!=omitted],env=env,capture_output=True,text=True);assert r.returncode
r=subprocess.run(cmd+list(map(str,objects)),env=env,capture_output=True,text=True);assert not r.returncode,r.stdout+r.stderr
inputs=bytearray();cases=snapshots=descriptors=0
float_values=[0,0x80000000,1,0x3f000000,0x3f800000,0xbf800000,0x7f7fffff,0x7f800000,0x7fc12345,0x7f812345]
for fill,debug,mode,force,control,width in itertools.product((0,0x55,0xff),(0,1),range(4),range(2),range(2),(0xffffffff,0,1,640,0x7fffffff)):
    for index in range(len(float_values)):
        arena=bytearray([fill])*4096;struct.pack_into('<2I',arena,12,0x24000400,0x24000800);struct.pack_into('<2I',arena,0x200,width,(0,480,0xffffffff)[index%3]);arena[0x218]=debug
        for off,shift in ((0x400,0),(0x600,3)):
            vals=[0x3f800000,0x40000000,0xc0400000,0,0x3f800000,0,0,0,0x3f800000]
            if index>=6:vals[3]=float_values[index]
            struct.pack_into('<9I',arena,off,*vals);struct.pack_into('<3I',arena,off+0x28,(0,2,0xffffffff)[index%3],float_values[(index+shift)%len(float_values)],float_values[(index+shift+2)%len(float_values)])
        struct.pack_into('<4f',arena,0x800,1.0,2.0,640.0,480.0)
        inputs.extend(struct.pack('<3I',mode,force,control));inputs.extend(arena);cases+=1;snapshots+=2+(not debug and width in (0xffffffff,0));descriptors+=not debug
(w/'input.bin').write_bytes(inputs)
for kind in ('retail','compiled'):
    r=subprocess.run([str(exe),str(w/'input.bin'),str(w/(kind+'.bin')),kind],env=env,capture_output=True,text=True,timeout=60);assert not r.returncode,(w,r.returncode,r.stdout,r.stderr);print(r.stdout.strip(),kind,flush=True)
actual=(w/'compiled.bin').read_bytes();expected=(w/'retail.bin').read_bytes()
if actual!=expected:
    diff=next((i for i,(a,b)in enumerate(zip(actual,expected))if a!=b),min(len(actual),len(expected)));raise AssertionError((w,diff,len(actual),len(expected)))
report=dict(cases=cases,snapshots=snapshots,descriptor_snapshots=descriptors,providers=[proof],omission_checks=4,source_sha256={str(p.relative_to(ROOT)):hashlib.sha256(p.read_bytes()).hexdigest()for p in (source,getters,normalizer,settings)},fixture_sha256=hashlib.sha256(fixture.read_bytes()).hexdigest(),headers={h:hashlib.sha256((ROOT/'rebuild/include'/h).read_bytes()).hexdigest()for h in ('engine/CEngineCamera.h','fable_engine_camera_desc.h','fable_camera.h')},boundary='Original535-byte retail body executed natively with audited call/data relocations. Actual FOV getters, AccurateNormalise and Z-buffer-start storage execute in both runs. Full4KiB arenas plus102 defined descriptor bytes; six indeterminate stack padding bytes are not sampled. Three fills, debug early-return, width/height signed boundaries, FOV flags, finite/subnormal/infinite/quiet/signalingNaN values,53/64-bit x87 precision, both force flags and mutating callbacks. ProcessDebugScript, GFVBoxToABox and SetCameraState are recorded boundaries. No state bytes normalized.')
(ROOT/'rebuild/compile-gate/native-engine-camera-process.json').write_text(json.dumps(report,indent=2)+'\n');print('NATIVE_ENGINE_CAMERA_PROCESS_GATE_PASS',snapshots,'snapshots',descriptors,'descriptors')
