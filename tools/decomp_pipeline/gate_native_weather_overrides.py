"""Rain/snow enable,clamp and disable transitions against native retail bodies."""
import hashlib,itertools,json,struct,subprocess,tempfile
from pathlib import Path
from capstone import Cs,CS_ARCH_X86,CS_MODE_32
import check_one as V
from gate_game_component_abi import Coff
from gate_render_state_static_data import coff_data
from variable_allocator_parity import check
from pe_oracle import EXE,read_virtual_data
ROOT=Path(__file__).resolve().parents[2];w=Path(tempfile.mkdtemp(prefix='native-weather-overrides-'));env=V.env()
rels=['00/b2/CEngine_WeatherOverrides_00b24010.cpp','00/b5/CEngineWeatherRenderer_Overrides_00b50ee0.cpp','01/43/NEngineInternal_GPWeatherRenderer_01436e44.cpp'];sources=[ROOT/'rebuild/src/compiled'/p for p in rels];fixture=ROOT/'rebuild/tests/integration/NativeWeatherOverrides_test.cpp'
specs=[(0,'?OverrideRain@CEngine@@UAEXM@Z',0xb24010,11,2,'EngineRain'),(0,'?OverrideSnow@CEngine@@UAEXM@Z',0xb24020,11,2,'EngineSnow'),(1,'?OverrideRain@CEngineWeatherRenderer@@QAEXM@Z',0xb50ee0,86,1,'OwnerRain'),(1,'?OverrideSnow@CEngineWeatherRenderer@@QAEXM@Z',0xb50f40,86,1,'OwnerSnow')]
global_symbol='?GPWeatherRenderer@NEngineInternal@@3PAVCEngineWeatherRenderer@@A'
targets={s:a for _,s,a,_,_,_ in specs};targets.update({global_symbol:0x1436e44,'__real@00000000':0x122dedc})
aliases={'@'+name+'@12':symbol for _,symbol,_,_,_,name in specs};aliases['_WeatherGlobal']=global_symbol
(w/'weather_overrides_aliases.h').write_text('\n'.join('#pragma comment(linker,"/alternatename:'+a+'='+b+'")'for a,b in aliases.items())+'\n')
raw=EXE.read_bytes();code=bytearray(1024);fixups=[];m=Cs(CS_ARCH_X86,CS_MODE_32);m.detail=True
for i,(_,_,address,size,_,_)in enumerate(specs):
    body=read_virtual_data(raw,address,size);code[256*i:256*i+size]=body
    for ins in m.disasm(body,address):
        if ins.mnemonic in ('call','jmp'):fixups.append((256*i+ins.address-address+1,ins.operands[0].imm,1))
        for op in ins.operands:
            if op.type==3 and op.mem.disp>=0x1220000:fixups.append((256*i+ins.address-address+ins.disp_offset,op.mem.disp,0))
assert len(fixups)==6 and all(t in targets.values()for _,t,_ in fixups)
assert read_virtual_data(raw,0x122dedc,4)==bytes(4)
(w/'weather_overrides_retail.h').write_text('static const unsigned char retailCode[]={'+','.join(str(b)for b in code)+'};\nstatic const unsigned retailFixups[][3]={'+','.join('{%d,%d,%d}'%f for f in fixups)+'};\n')
objects=[]
for i,path in enumerate(sources+[fixture]):
    obj=w/(str(i)+'.obj');r=V.cl(['/nologo','/c','/O2','/MT','/I'+str(w),'/Fo'+str(obj),str(path)],env,w);assert not r.returncode,r.stdout+r.stderr;objects.append(obj)
proofs=[check(objects[i],symbol,address,size,n,targets)for i,symbol,address,size,n,_ in specs]
assert coff_data(Coff(objects[2]),global_symbol,4)==read_virtual_data(raw,targets[global_symbol],4)==bytes(4)
exe=w/'fixture.exe';cmd=[str(V.VC/'bin/link.exe'),'/nologo','/out:'+str(exe),'kernel32.lib']
for omitted in range(3):
    r=subprocess.run(cmd+[str(p)for i,p in enumerate(objects)if i!=omitted],env=env,capture_output=True,text=True);assert r.returncode
r=subprocess.run(cmd+list(map(str,objects)),env=env,capture_output=True,text=True);assert not r.returncode,r.stdout+r.stderr
inputs=bytearray();cases=0;values=(0,0x80000000,1,0x80000001,0x007fffff,0x00800000,0x3e800000,0x3f000000,0x3f800000,0xbf800000,0x40000000,0x7f7fffff,0x7f800000,0xff800000,0x7fc12345,0x7f812345,0xff812345)
for fill,kind,owner,vi,maximum in itertools.product((0,0x55,0xaa,0xff),range(4),(0,0x800),range(len(values)),values):
    arena=bytearray([fill])*8192;struct.pack_into('<I',arena,owner+0x12f8,maximum)
    inputs.extend(struct.pack('<4I',kind,owner,values[vi],values[(vi+7)%len(values)]));inputs.extend(arena);cases+=1
(w/'input.bin').write_bytes(inputs)
for kind in ('retail','compiled'):
    r=subprocess.run([str(exe),str(w/'input.bin'),str(w/(kind+'.bin')),kind],env=env,capture_output=True,text=True,timeout=60);assert not r.returncode,(w,r.returncode,r.stdout,r.stderr);print(r.stdout.strip(),kind,flush=True)
actual=(w/'compiled.bin').read_bytes();expected=(w/'retail.bin').read_bytes();assert actual==expected,(w,'weather overrides');assert len(actual)==cases*2*(8192+8)
report=dict(cases=cases,snapshots=cases*2,omission_checks=3,providers=proofs,source_sha256={str(p.relative_to(ROOT)):hashlib.sha256(p.read_bytes()).hexdigest()for p in sources},fixture_sha256=hashlib.sha256(fixture.read_bytes()).hexdigest(),boundary='Original bodies execute natively after six audited relocations;actual engine/owner composition and direct owner calls. Full8KiB arenas/global plus floating status after each of two sequential calls;four fills,two owners,all17x17 strength/maximum bit pairs including signed zero,subnormal,infinity,quiet/signalingNaN. Raw argument bits cross test ABI unchanged. Disable preserves stored strength;NaN input disables. No callbacks or normalized bytes.')
(ROOT/'rebuild/compile-gate/native-weather-overrides.json').write_text(json.dumps(report,indent=2)+'\n');print('NATIVE_WEATHER_OVERRIDES_GATE_PASS',cases*2,'snapshots')
