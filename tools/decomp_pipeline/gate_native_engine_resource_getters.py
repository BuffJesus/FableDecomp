"""Engine pool temporary ownership and scratch-subobject getter against retail."""
import hashlib,itertools,json,struct,subprocess,tempfile
from pathlib import Path
from capstone import Cs,CS_ARCH_X86,CS_MODE_32
import check_one as V
from gate_game_component_abi import Coff
from gate_render_state_static_data import coff_data
from variable_allocator_parity import check
from pe_oracle import EXE,read_virtual_data
ROOT=Path(__file__).resolve().parents[2];w=Path(tempfile.mkdtemp(prefix='native-engine-resource-getters-'));env=V.env()
sources=[ROOT/'rebuild/src/compiled'/p for p in ('00/b2/CEngine_ResourceGetters_00b25c80.cpp','01/43/NEngineInternal_GPResourceManager_01436e98.cpp')];fixture=ROOT/'rebuild/tests/integration/NativeEngineResourceGetters_test.cpp'
specs=[('?GetLoadingMemoryPool@CEngine@@UAEPAVCMovableResourceMemoryPool@@XZ',0xb25c80,55,2,'Loading'),('?GetAnimationMemoryPool@CEngine@@UAEPAVCMovableResourceMemoryPool@@XZ',0xb25cc0,55,2,'Animation'),('?GetScratchBufferTextureManager@CEngine@@UAEPAVCGPUScratchBufferTextureManager@@XZ',0xb23f60,11,1,'Scratch')]
global_symbol='?GPResourceManager@NEngineInternal@@3PAVCEngineResourceManager@@A';targets={global_symbol:0x1436e98,'??3@YAXPAX@Z':0xbfe9bc}
aliases={'@'+name+'@4':symbol for symbol,_,_,_,name in specs};aliases['_ResourceGlobal']=global_symbol
(w/'resource_getters_aliases.h').write_text('\n'.join('#pragma comment(linker,"/alternatename:'+a+'='+b+'")'for a,b in aliases.items())+'\n')
raw=EXE.read_bytes();code=bytearray(768);fixups=[];m=Cs(CS_ARCH_X86,CS_MODE_32);m.detail=True
for i,(_,address,size,_,_)in enumerate(specs):
    body=read_virtual_data(raw,address,size);code[256*i:256*i+size]=body
    for ins in m.disasm(body,address):
        if ins.mnemonic=='call' and ins.operands[0].type==2:fixups.append((256*i+ins.address-address+1,ins.operands[0].imm,1))
        for op in ins.operands:
            if op.type==3 and op.mem.disp>=0x1220000:fixups.append((256*i+ins.address-address+ins.disp_offset,op.mem.disp,0))
assert len(fixups)==5 and all(t in targets.values()for _,t,_ in fixups)
(w/'resource_getters_retail.h').write_text('static const unsigned char retailCode[]={'+','.join(str(b)for b in code)+'};\nstatic const unsigned retailFixups[][3]={'+','.join('{%d,%d,%d}'%f for f in fixups)+'};\n')
objects=[]
for i,path in enumerate(sources+[fixture]):
    obj=w/(str(i)+'.obj');r=V.cl(['/nologo','/c','/O2','/MT','/I'+str(w),'/Fo'+str(obj),str(path)],env,w);assert not r.returncode,r.stdout+r.stderr;objects.append(obj)
proofs=[check(objects[0],symbol,address,size,n,targets)for symbol,address,size,n,_ in specs]
assert coff_data(Coff(objects[1]),global_symbol,4)==read_virtual_data(raw,targets[global_symbol],4)==bytes(4)
exe=w/'fixture.exe';cmd=[str(V.VC/'bin/link.exe'),'/nologo','/out:'+str(exe),'kernel32.lib']
for omitted in range(2):
    r=subprocess.run(cmd+[str(p)for i,p in enumerate(objects)if i!=omitted],env=env,capture_output=True,text=True);assert r.returncode
r=subprocess.run(cmd+list(map(str,objects)),env=env,capture_output=True,text=True);assert not r.returncode,r.stdout+r.stderr
inputs=bytearray();cases=snapshots=0
scenarios=list(itertools.product((0,0x55,0xaa,0xff),range(3),range(4),(0,0x4000),(None,0,1,2,0x7fffffff,0xffffffff)))
scenarios += [(fill,kind,0,0,1)for fill,kind in itertools.product(range(256),range(3))]
for fill,kind,mode,owner,count in scenarios:
    arena=bytearray([fill])*32768;info=0x7800 if count is not None else 0
    data=(0,0x24007c00,0xdeadbeef,0xffffffff)[fill%4]
    for offset in (0x8e0,0x8f0):struct.pack_into('<2I',arena,owner+offset,data,0x24000000+info if info else 0)
    if info:struct.pack_into('<3I',arena,info,count,0,0x24007d00)
    inputs.extend(struct.pack('<4I',kind,mode,owner,info));inputs.extend(arena);cases+=1;snapshots+=1+2*(kind<2 and count==0)
(w/'input.bin').write_bytes(inputs)
for kind in ('retail','compiled'):
    r=subprocess.run([str(exe),str(w/'input.bin'),str(w/(kind+'.bin')),kind],env=env,capture_output=True,text=True,timeout=60);assert not r.returncode,(w,r.returncode,r.stdout,r.stderr);print(r.stdout.strip(),kind,flush=True)
actual=(w/'compiled.bin').read_bytes();expected=(w/'retail.bin').read_bytes();assert actual==expected,(w,'resource getters');assert len(actual)==snapshots*(32768+12)
report=dict(cases=cases,snapshots=snapshots,omission_checks=2,providers=proofs,source_sha256={str(p.relative_to(ROOT)):hashlib.sha256(p.read_bytes()).hexdigest()for p in sources},fixture_sha256=hashlib.sha256(fixture.read_bytes()).hexdigest(),boundary='Original bodies execute natively after five audited relocations;full32KiB arena and global/callback arguments/cached return value. All256fills plus null controls,counts0/1/2/7fffffff/ffffffff,two owners,four callback mutation modes and null/arbitrary data pointers. DeleteFunc and operator delete are recorded boundaries;callbacks change manager/global/control fields and poison freed block. No normalized bytes. Partial shared retail owner view asserts only accessed offsets;does not assert total owner size.')
(ROOT/'rebuild/compile-gate/native-engine-resource-getters.json').write_text(json.dumps(report,indent=2)+'\n');print('NATIVE_ENGINE_RESOURCE_GETTERS_GATE_PASS',snapshots,'snapshots')
