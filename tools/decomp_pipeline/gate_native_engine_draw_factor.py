"""Primitive draw distance arithmetic and genuine native owner globals."""
import hashlib,itertools,json,struct,subprocess,tempfile
from pathlib import Path
from capstone import Cs,CS_ARCH_X86,CS_MODE_32
import check_one as V
from gate_game_component_abi import Coff
from gate_render_state_static_data import coff_data
from variable_allocator_parity import check
from pe_oracle import EXE,read_virtual_data
ROOT=Path(__file__).resolve().parents[2];w=Path(tempfile.mkdtemp(prefix='native-engine-draw-factor-'));env=V.env()
sources=[ROOT/'rebuild/src/compiled'/p for p in ('00/b2/CEngine_GetPrimitiveDrawDistanceFactor_00b23d00.cpp','01/43/NEngineInternal_GPEngineDef_01436e20.cpp','01/43/NEngineInternal_GPPrimitiveRenderer_01436e80.cpp')];fixture=ROOT/'rebuild/tests/integration/NativeEngineDrawFactor_test.cpp'
symbol='?GetPrimitiveDrawDistanceFactor@CEngine@@UBEMXZ';definition='?GPEngineDef@NEngineInternal@@3PBVCEngineDef@@B';renderer='?GPPrimitiveRenderer@NEngineInternal@@3PAVCEnginePrimitiveRenderer@@A'
targets={definition:0x1436e20,renderer:0x1436e80,'__real@3f800000':0x122ded8};aliases={'@DrawFactor@4':symbol,'_DefinitionGlobal':definition,'_RendererGlobal':renderer}
(w/'engine_draw_factor_aliases.h').write_text('\n'.join('#pragma comment(linker,"/alternatename:'+a+'='+b+'")'for a,b in aliases.items())+'\n')
raw=EXE.read_bytes();code=read_virtual_data(raw,0xb23d00,24);fixups=[];m=Cs(CS_ARCH_X86,CS_MODE_32);m.detail=True
for ins in m.disasm(code,0xb23d00):
    for op in ins.operands:
        if op.type==3 and op.mem.disp>=0x1220000:fixups.append((ins.address-0xb23d00+ins.disp_offset,op.mem.disp))
assert len(fixups)==3 and all(t in targets.values()for _,t in fixups);assert read_virtual_data(raw,0x122ded8,4)==struct.pack('<f',1.0)
(w/'engine_draw_factor_retail.h').write_text('static const unsigned char retailCode[]={'+','.join(str(b)for b in code)+'};\nstatic const unsigned retailFixups[][2]={'+','.join('{%d,%d}'%f for f in fixups)+'};\n')
objects=[]
for i,path in enumerate(sources+[fixture]):
    obj=w/(str(i)+'.obj');r=V.cl(['/nologo','/c','/O2','/MT','/I'+str(w),'/Fo'+str(obj),str(path)],env,w);assert not r.returncode,r.stdout+r.stderr;objects.append(obj)
proof=check(objects[0],symbol,0xb23d00,24,3,targets)
for i,name in ((1,definition),(2,renderer)):assert coff_data(Coff(objects[i]),name,4)==read_virtual_data(raw,targets[name],4)==bytes(4)
exe=w/'fixture.exe';cmd=[str(V.VC/'bin/link.exe'),'/nologo','/out:'+str(exe),'kernel32.lib']
for omitted in range(3):
    r=subprocess.run(cmd+[str(p)for i,p in enumerate(objects)if i!=omitted],env=env,capture_output=True,text=True);assert r.returncode
r=subprocess.run(cmd+list(map(str,objects)),env=env,capture_output=True,text=True);assert not r.returncode,r.stdout+r.stderr
inputs=bytearray();cases=0;values=(0,0x80000000,1,0x80000001,0x007fffff,0x00800000,0x3e800000,0x3f000000,0x3f800000,0xbf800000,0xbf800001,0x7f7fffff,0xff7fffff,0x7f800000,0xff800000,0x7fc12345,0x7f812345)
scenarios=list(itertools.product((0,0x55,0xff),values,values,(0,0x10000),(0,0x100,0x200,0x300)))
scenarios += [(fill,0x3f000000,0x3f800000,0x10000,0)for fill in range(256)]
for fill,ratio,factor,precision,rounding in scenarios:
    definition_offset=0x400 if fill&1 else 0;renderer_offset=0x800 if fill&2 else definition_offset
    arena=bytearray([fill])*4096;struct.pack_into('<I',arena,definition_offset+0x50,ratio);struct.pack_into('<I',arena,renderer_offset+0x7c,factor)
    inputs.extend(struct.pack('<3I',definition_offset,renderer_offset,precision|rounding));inputs.extend(arena);cases+=1
(w/'input.bin').write_bytes(inputs)
for kind in ('retail','compiled'):
    r=subprocess.run([str(exe),str(w/'input.bin'),str(w/(kind+'.bin')),kind],env=env,capture_output=True,text=True,timeout=60);assert not r.returncode,(w,r.returncode,r.stdout,r.stderr);print(r.stdout.strip(),kind,flush=True)
actual=(w/'compiled.bin').read_bytes();expected=(w/'retail.bin').read_bytes();assert actual==expected,(w,'draw factor');assert len(actual)==cases*(4096+24)
report=dict(cases=cases,snapshots=cases,omission_checks=3,providers=[proof],source_sha256={str(p.relative_to(ROOT)):hashlib.sha256(p.read_bytes()).hexdigest()for p in sources},fixture_sha256=hashlib.sha256(fixture.read_bytes()).hexdigest(),boundary='Original24byte retail body executes natively after three audited relocations. Full4KiB arena/two globals,double-stored result,floating status/control;17x17 raw operand pairs,53/64bit precision,four rounding modes,owner alias/distinct placements and all256fills. No callbacks or normalized bytes. This fixture observes the returned value after double storage,not all80 x87 bits.')
(ROOT/'rebuild/compile-gate/native-engine-draw-factor.json').write_text(json.dumps(report,indent=2)+'\n');print('NATIVE_ENGINE_DRAW_FACTOR_GATE_PASS',cases,'snapshots')
