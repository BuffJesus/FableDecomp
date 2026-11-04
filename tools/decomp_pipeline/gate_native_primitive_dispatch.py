"""Native engine primitive routing, including persistent-call hidden false argument."""
import hashlib,itertools,json,struct,subprocess,tempfile
from pathlib import Path
from capstone import Cs,CS_ARCH_X86,CS_MODE_32
import check_one as V
from variable_allocator_parity import check
from gate_game_component_abi import Coff
from gate_render_state_static_data import coff_data
from pe_oracle import EXE,read_virtual_data
ROOT=Path(__file__).resolve().parents[2];w=Path(tempfile.mkdtemp(prefix='native-primitive-dispatch-'));env=V.env()
sources=[ROOT/'rebuild/src/compiled'/p for p in ('00/b2/CEngine_PrimitiveDispatch_00b23bb0.cpp','01/43/NEngineInternal_GPPrimitiveRenderer_01436e80.cpp')];fixture=ROOT/'rebuild/tests/integration/NativePrimitiveDispatch_test.cpp'
specs=[('Add','AddPrimitive','XABVCEnginePrimitive@@@Z',0xb23bb0,0xb32e90,12),('Persistent','SetPersistentPrimitive','XAAVCEnginePrimitiveHandle@@ABVCEnginePrimitive@@K@Z',0xb23bc0,0xb324a0,20),('AddChild','AddChildPrimitive','KAAVCEnginePrimitiveHandle@@ABVCEnginePrimitive@@K@Z',0xb23c20,0xb31ef0,20),('SetChildHandle','SetChildPrimitive','XAAVCEnginePrimitiveHandle@@K0ABVCEnginePrimitive@@K@Z',0xb23c30,0xb31f20,28),('GetChild','GetChildPrimitiveHandle','XAAVCEnginePrimitiveHandle@@K0@Z',0xb23c40,0xb31f50,20),('SetChild','SetChildPrimitive','XAAVCEnginePrimitiveHandle@@KABVCEnginePrimitive@@K@Z',0xb23c50,0xb31f70,24),('ClearChild','ClearChildPrimitive','XAAVCEnginePrimitiveHandle@@K@Z',0xb23c60,0xb31fa0,16),('ClearAll','ClearAllChildPrimitives','XAAVCEnginePrimitiveHandle@@@Z',0xb23c70,0xb31fc0,12)]
glob='?GPPrimitiveRenderer@NEngineInternal@@3PAVCEnginePrimitiveRenderer@@A';targets={glob:0x1436e80};aliases={'_PrimitiveGlobal':glob};providers=[]
for n,(alias,name,suffix,address,target,argc) in enumerate(specs):
 symbol='?'+name+'@CEngine@@UAE'+suffix;callee='?'+name+'@CEnginePrimitiveRenderer@@QAE'+(suffix[:-2]+'_N@Z' if n==1 else suffix)
 targets[callee]=target;aliases['@'+alias+'@'+str(argc)]=symbol;aliases[callee]='@On'+alias+'@'+str(argc+(4 if n==1 else 0));providers.append((symbol,address,31 if n==1 else 11,n*32))
(w/'primitive_dispatch_aliases.h').write_text('\n'.join('#pragma comment(linker,"/alternatename:'+a+'='+b+'")' for a,b in aliases.items())+'\n')
raw=EXE.read_bytes();body=bytearray(256);fixups=[];md=Cs(CS_ARCH_X86,CS_MODE_32);md.detail=True
for symbol,address,length,offset in providers:
 code=read_virtual_data(raw,address,length);body[offset:offset+length]=code
 for i in md.disasm(code,address):
  if i.mnemonic in ('call','jmp') and i.operands[0].type==2:assert i.operands[0].imm in targets.values();fixups.append((offset+i.address-address+i.imm_offset,i.operands[0].imm,1))
  else:
   for o in i.operands:
    if o.type==3 and o.mem.disp==0x1436e80:fixups.append((offset+i.address-address+i.disp_offset,o.mem.disp,0))
assert len(fixups)==16
(w/'primitive_dispatch_retail.h').write_text('static const unsigned char retailCode[]={'+','.join(map(str,body))+'};\nstatic const unsigned retailFixups[][3]={'+','.join('{%d,%d,%d}'%f for f in fixups)+'};\n')
objects=[]
for n,p in enumerate(sources+[fixture]):
 o=w/(str(n)+'.obj');r=V.cl(['/nologo','/c','/O2','/MT','/I'+str(w),'/Fo'+str(o),str(p)],env,w);assert not r.returncode,r.stdout+r.stderr;objects.append(o)
proofs=[check(objects[0],symbol,address,length,2,targets) for symbol,address,length,offset in providers]
assert coff_data(Coff(objects[1]),glob,4)==read_virtual_data(raw,0x1436e80,4)==bytes(4)
cmd=[str(V.VC/'bin/link.exe'),'/nologo','/out:'+str(w/'fixture.exe'),'kernel32.lib']
for omit in range(2):
 r=subprocess.run(cmd+[str(o) for i,o in enumerate(objects) if i!=omit],env=env,capture_output=True,text=True);assert r.returncode
r=subprocess.run(cmd+list(map(str,objects)),env=env,capture_output=True,text=True);assert not r.returncode,r.stdout+r.stderr
cases=0
with (w/'input.bin').open('wb') as f:
 for slot,index,flags,alias,mode,result in itertools.product(range(2),(0,1,0xffffffff,0x80000000),(0,1,0x40,0xffffffff),range(3),range(4),(0,1,0xffffffff)):
  f.write(struct.pack('<6I',slot,index,flags,alias,mode,result));f.write(bytes([0x95])*1024);cases+=1
for kind in ('retail','compiled'):
 r=subprocess.run([str(w/'fixture.exe'),str(w/'input.bin'),str(w/(kind+'.bin')),kind],env=env,capture_output=True,text=True,timeout=60);assert not r.returncode,(w,r.returncode,r.stdout,r.stderr);print(r.stdout.strip(),kind,flush=True)
assert (w/'retail.bin').read_bytes()==(w/'compiled.bin').read_bytes(),w
size=(w/'compiled.bin').stat().st_size;assert size%(1024+4)==0;snapshots=size//1028
report=dict(cases=cases,snapshots=snapshots,omission_checks=2,providers=proofs,source_sha256={str(p.relative_to(ROOT)):hashlib.sha256(p.read_bytes()).hexdigest() for p in sources},fixture_sha256=hashlib.sha256(fixture.read_bytes()).hexdigest(),boundary='Eight-call sequences;full1KiB arena plus actual renderer singleton. Checks all identities/arguments, persistent false flag, return bits, input aliases, renderer replacement/null after callback, independent model of callback handle changes. Each next call reloads global. Underlying renderer operations remain explicit callbacks.')
(ROOT/'rebuild/compile-gate/native-primitive-dispatch.json').write_text(json.dumps(report,indent=2)+'\n');print('NATIVE_PRIMITIVE_DISPATCH_GATE_PASS',snapshots,'snapshots')
