"""Native global lighting queries, including hidden return-buffer overlap."""
import hashlib,itertools,json,struct,subprocess,tempfile
from pathlib import Path
from unicorn import Uc,UC_ARCH_X86,UC_MODE_32
from unicorn.x86_const import *
import check_one as V
from gate_game_component_abi import Coff
from gate_render_state_static_data import coff_data
from variable_allocator_parity import check
from pe_oracle import EXE,pe_sections,read_virtual_data
ROOT=Path(__file__).resolve().parents[2];w=Path(tempfile.mkdtemp(prefix='native-engine-colour-peeks-'));env=V.env()
source=ROOT/'rebuild/src/compiled/00/b2/CEngine_GlobalColourPeeks_00b25d20.cpp'
settings=ROOT/'rebuild/src/compiled/01/43/NEngineInternal_GPLightingManager_01436e9c.cpp'
fixture=ROOT/'rebuild/tests/integration/NativeEngineColourPeeks_test.cpp'
global_symbol='?GPLightingManager@NEngineInternal@@3PAVCEngineLightingManager@@A'
specs=[('Diffuse',0xb25d20,41),('Ambient',0xb25d50,46)]
aliases={'_LightingGlobal':global_symbol}
for name,_,_ in specs:aliases['@'+name+'@12']='?PeekGlobal'+name+'Colour@CEngine@@UBE?BVCRGBFloatColour@@XZ'
(w/'engine_colour_peeks_aliases.h').write_text('\n'.join('#pragma comment(linker,"/alternatename:'+a+'='+b+'")'for a,b in aliases.items())+'\n')
objects=[]
for i,path in enumerate((source,settings,fixture)):
    obj=w/(str(i)+'.obj');r=V.cl(['/nologo','/c','/O2','/MT','/I'+str(w),'/Fo'+str(obj),str(path)],env,w);assert not r.returncode,r.stdout+r.stderr;objects.append(obj)
proofs=[check(objects[0],aliases['@'+name+'@12'],address,size,1,{global_symbol:0x1436e9c})for name,address,size in specs]
raw=EXE.read_bytes();assert coff_data(Coff(objects[1]),global_symbol,4)==read_virtual_data(raw,0x1436e9c,4)==bytes(4)
exe=w/'fixture.exe';cmd=[str(V.VC/'bin/link.exe'),'/nologo','/out:'+str(exe),'kernel32.lib']
for omitted in range(2):
    r=subprocess.run(cmd+[str(p)for i,p in enumerate(objects)if i!=omitted],env=env,capture_output=True,text=True);assert r.returncode
r=subprocess.run(cmd+list(map(str,objects)),env=env,capture_output=True,text=True);assert not r.returncode,r.stdout+r.stderr
BASE=0x24000000;u=Uc(UC_ARCH_X86,UC_MODE_32);u.mem_map(0x400000,0x1200000);u.mem_map(BASE,0x10000)
for _,va,_,off,n in pe_sections(raw):
    if n:u.mem_write(0x400000+va,raw[off:off+n])
inputs=bytearray();expected=bytearray();cases=0
values=(0,0x80000000,1,0x3f800000,0xbf800000,0x7f800000,0xff800000,0x7fc12345,0x7f812345)
for fill,kind,theme,placement in itertools.product(range(256),range(2),(0x4800,0x4900),range(8)):
    dest=(0x100,0x3c,theme-4,theme,theme+4,theme+12,theme+16,theme+20)[placement]
    arena=bytearray([fill])*20480;struct.pack_into('<I',arena,0x3c,BASE+theme)
    struct.pack_into('<8I',arena,theme,*[values[(fill+i)%len(values)]for i in range(8)])
    inputs.extend(struct.pack('<2I',kind,dest));inputs.extend(arena)
    u.mem_write(BASE,bytes(arena));u.mem_write(0x1436e9c,struct.pack('<I',BASE));sp=BASE+0xe000;stop=BASE+0xf000
    u.mem_write(sp,struct.pack('<2I',stop,BASE+dest));u.reg_write(UC_X86_REG_ESP,sp);u.reg_write(UC_X86_REG_ECX,0x1badb002)
    saved={UC_X86_REG_EBX:0x13572468,UC_X86_REG_ESI:0x23456789,UC_X86_REG_EDI:0x3456789a,UC_X86_REG_EBP:0x456789ab}
    for reg,value in saved.items():u.reg_write(reg,value)
    u.emu_start(specs[kind][1],stop,count=40)
    assert u.reg_read(UC_X86_REG_ESP)==sp+8 and all(u.reg_read(reg)==value for reg,value in saved.items())
    assert u.reg_read(UC_X86_REG_EAX)==BASE+dest
    expected.extend(struct.pack('<I',u.reg_read(UC_X86_REG_EAX)));expected.extend(u.mem_read(BASE,20480));cases+=1
(w/'input.bin').write_bytes(inputs)
r=subprocess.run([str(exe),str(w/'input.bin'),str(w/'actual.bin')],env=env,capture_output=True,text=True,timeout=60);assert not r.returncode,(w,r.returncode,r.stdout,r.stderr)
assert (w/'actual.bin').read_bytes()==expected,w
report=dict(cases=cases,snapshots=cases,omission_checks=2,providers=proofs,source_sha256={str(p.relative_to(ROOT)):hashlib.sha256(p.read_bytes()).hexdigest()for p in (source,settings)},fixture_sha256=hashlib.sha256(fixture.read_bytes()).hexdigest(),boundary='Full20KiB arena and hidden return pointer;256fills,two theme placements,eight return-buffer positions including partial/exact overlap and manager array-pointer aliasing,raw exceptional float payloads. No this dereference,callbacks or normalized bytes. Retail stack cleanup and all callee-saved registers checked. Native compiled bodies compared with original retail integer instructions under Unicorn. Actual BSS global is required and verified.')
(ROOT/'rebuild/compile-gate/native-engine-colour-peeks.json').write_text(json.dumps(report,indent=2)+'\n');print(r.stdout.strip())
