"""Engine and lighting owner setters with actual vector normalization."""
import hashlib,itertools,json,struct,subprocess,tempfile
from pathlib import Path
from capstone import Cs,CS_ARCH_X86,CS_MODE_32
import check_one as V
from gate_game_component_abi import Coff
from gate_render_state_static_data import coff_data
from variable_allocator_parity import check
from pe_oracle import EXE,read_virtual_data
ROOT=Path(__file__).resolve().parents[2];w=Path(tempfile.mkdtemp(prefix='native-lighting-setters-'));env=V.env()
rels=['00/b2/CEngine_LightingSetters_00b23ea0.cpp','00/b4/CEngineLightingManager_SetGlobalLightingParameters_00b47280.cpp','00/b4/CEnginePrimitiveRenderer2D_SetLightingParameters_00b4a5f0.cpp','01/43/NEngineInternal_GPLightingManager_01436e9c.cpp','01/43/NEngineInternal_GPPrimitiveRenderer2D_01436e7c.cpp','00/a1/C3DVector_AccurateNormalise_00a14440.cpp']
sources=[ROOT/'rebuild/src/compiled'/p for p in rels];fixture=ROOT/'rebuild/tests/integration/NativeLightingSetters_test.cpp'
suffix='@@QAEXABVC3DVector@@ABVCRGBFloatColour@@11@Z'
specs=[(0,'?SetGlobalLightingParameters@CEngine@@UAEXABVC3DVector@@ABVCRGBFloatColour@@11@Z',0xb23ea0,11,2,'EngineGlobal'),(0,'?Set2DPrimitiveLightingParameters@CEngine@@UAEXABVC3DVector@@ABVCRGBFloatColour@@11@Z',0xb23eb0,11,2,'Engine2D'),(1,'?SetGlobalLightingParameters@CEngineLightingManager'+suffix,0xb47280,143,1,'OwnerGlobal'),(2,'?SetLightingParameters@CEnginePrimitiveRenderer2D'+suffix,0xb4a5f0,122,1,'Owner2D')]
light='?GPLightingManager@NEngineInternal@@3PAVCEngineLightingManager@@A';renderer='?GPPrimitiveRenderer2D@NEngineInternal@@3PAVCEnginePrimitiveRenderer2D@@A';normalizer='?AccurateNormalise@C3DVector@@QAEAAV1@XZ'
targets={s:a for _,s,a,_,_,_ in specs};targets.update({light:0x1436e9c,renderer:0x1436e7c,normalizer:0xa14440})
aliases={'@'+name+'@24':symbol for _,symbol,_,_,_,name in specs};aliases.update({'_LightingGlobal':light,'_RendererGlobal':renderer,'@Normalize@4':normalizer})
(w/'lighting_setters_aliases.h').write_text('\n'.join('#pragma comment(linker,"/alternatename:'+a+'='+b+'")'for a,b in aliases.items())+'\n')
raw=EXE.read_bytes();code=bytearray(1024);fixups=[];m=Cs(CS_ARCH_X86,CS_MODE_32);m.detail=True
for i,(_,_,address,size,_,_)in enumerate(specs):
    body=read_virtual_data(raw,address,size);code[256*i:256*i+size]=body
    for ins in m.disasm(body,address):
        if ins.mnemonic in ('call','jmp'):fixups.append((256*i+ins.address-address+1,ins.operands[0].imm,1))
        for op in ins.operands:
            if op.type==3 and op.mem.disp>=0x1220000:fixups.append((256*i+ins.address-address+ins.disp_offset,op.mem.disp,0))
assert len(fixups)==6 and all(t in targets.values()for _,t,_ in fixups)
(w/'lighting_setters_retail.h').write_text('static const unsigned char retailCode[]={'+','.join(str(b)for b in code)+'};\nstatic const unsigned retailFixups[][3]={'+','.join('{%d,%d,%d}'%f for f in fixups)+'};\n')
objects=[]
for i,path in enumerate(sources+[fixture]):
    obj=w/(str(i)+'.obj');r=V.cl(['/nologo','/c','/O2','/MT','/I'+str(w),'/Fo'+str(obj),str(path)],env,w);assert not r.returncode,r.stdout+r.stderr;objects.append(obj)
proofs=[check(objects[i],symbol,address,size,n,targets)for i,symbol,address,size,n,_ in specs]
for i,symbol in ((3,light),(4,renderer)):assert coff_data(Coff(objects[i]),symbol,4)==read_virtual_data(raw,targets[symbol],4)==bytes(4)
exe=w/'fixture.exe';cmd=[str(V.VC/'bin/link.exe'),'/nologo','/out:'+str(exe),'kernel32.lib']
for omitted in range(6):
    r=subprocess.run(cmd+[str(p)for i,p in enumerate(objects)if i!=omitted],env=env,capture_output=True,text=True);assert r.returncode
r=subprocess.run(cmd+list(map(str,objects)),env=env,capture_output=True,text=True);assert not r.returncode,r.stdout+r.stderr
inputs=bytearray();cases=0;values=(0,0x80000000,1,0x3f800000,0xbf800000,0x7f800000,0x7fc12345,0x7f812345)
scenarios=list(itertools.product((0,0x55,0xaa,0xff),range(4),range(8),range(6),range(2)))
scenarios += [(fill,kind,3,0,0)for fill,kind in itertools.product(range(256),range(4))]
for fill,kind,vi,placement,precision in scenarios:
    owner=0x5000 if (fill+vi)&1 else 0;theme=0xa000;direction=owner+(0x30 if kind<2 else 0x7c);colour=theme if kind<2 else owner+0x4c
    args=[(0xa200,0xa220,0xa240,0xa260),(direction,colour+16,colour,colour+32),(colour,colour+4,colour+12,colour+20),(direction-4,direction,colour+4,colour+12),(0xa200,)*4,(owner+0xe4,owner+0xe4,colour,colour+16)][placement]
    arena=bytearray([fill])*49152
    for offset in args:struct.pack_into('<4I',arena,offset,*[values[(vi+j)%len(values)]for j in range(4)])
    if kind<2:struct.pack_into('<I',arena,owner+0x3c,0x24000000+theme)
    inputs.extend(struct.pack('<7I',kind,owner,*args,precision));inputs.extend(arena);cases+=1
(w/'input.bin').write_bytes(inputs)
for kind in ('retail','compiled'):
    r=subprocess.run([str(exe),str(w/'input.bin'),str(w/(kind+'.bin')),kind],env=env,capture_output=True,text=True,timeout=60);assert not r.returncode,(w,r.returncode,r.stdout,r.stderr);print(r.stdout.strip(),kind,flush=True)
actual=(w/'compiled.bin').read_bytes();expected=(w/'retail.bin').read_bytes()
if actual!=expected:
    diff=next((i for i,(a,b)in enumerate(zip(actual,expected))if a!=b),min(len(actual),len(expected)));raise AssertionError((w,diff,len(actual),len(expected)))
assert len(actual)==cases*(49152+8)
report=dict(cases=cases,snapshots=cases,omission_checks=6,providers=proofs,source_sha256={str(p.relative_to(ROOT)):hashlib.sha256(p.read_bytes()).hexdigest()for p in sources},fixture_sha256=hashlib.sha256(fixture.read_bytes()).hexdigest(),boundary='Original retail bodies execute natively after six audited relocations; direct owners and actual engine-to-owner composition both execute actual AccurateNormalise. Full48KiB arenas/two globals,all256fills plus exceptional float payloads,six argument alias/overlap patterns,two owner placements,53/64-bit precision. No callbacks or normalized bytes.')
(ROOT/'rebuild/compile-gate/native-lighting-setters.json').write_text(json.dumps(report,indent=2)+'\n');print('NATIVE_LIGHTING_SETTERS_GATE_PASS',cases,'snapshots')
