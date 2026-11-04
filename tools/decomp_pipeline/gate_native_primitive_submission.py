"""Persistent primitive creation/update with independent lifetime/routing model."""
import hashlib,itertools,json,struct,subprocess,tempfile
from pathlib import Path
from capstone import Cs,CS_ARCH_X86,CS_MODE_32
import check_one as V
from variable_allocator_parity import check
from gate_game_component_abi import Coff
from gate_render_state_static_data import coff_data
from pe_oracle import EXE,read_virtual_data
ROOT=Path(__file__).resolve().parents[2];w=Path(tempfile.mkdtemp(prefix='native-primitive-submission-'));env=V.env()
sources=[ROOT/'rebuild/src/compiled'/p for p in ('00/b3/CEnginePrimitiveRenderer_SetPersistentPrimitive_00b324a0.cpp','01/43/NEngineInternal_GPPrimitiveTypeInfo_01436e84.cpp','01/43/NEngineInternal_GPPrimitiveRenderer2D_01436e7c.cpp','00/b3/CEnginePrimitiveRenderer_AddPrimitive_00b32e90.cpp')];fixture=ROOT/'rebuild/tests/integration/NativePrimitiveSubmission_test.cpp'
symbol='?SetPersistentPrimitive@CEnginePrimitiveRenderer@@QAEXAAVCEnginePrimitiveHandle@@ABVCEnginePrimitive@@K_N@Z'
typeinfo='?GPPrimitiveTypeInfo@NEngineInternal@@3PAVCEnginePrimitiveTypeInfo@@A';twoD='?GPPrimitiveRenderer2D@NEngineInternal@@3PAVCEnginePrimitiveRenderer2D@@A'
update='?UpdatePosition@CEngineScene@@SIXAAVCEngineInternalPrimitiveBase@@K@Z';add2D='?AddPrimitive@CEnginePrimitiveRenderer2D@@QAEXAAVCEngineInternalPrimitiveBase@@@Z';add='?AddPrimitive@CEnginePrimitiveManager@@QAEXAAVCEngineInternalPrimitiveBase@@K@Z'
targets={typeinfo:0x1436e84,twoD:0x1436e7c,update:0xb91840,add2D:0xb4a670,add:0xbacdd0}
aliases={'@Persistent@24':symbol,'_TypeGlobal':typeinfo,'_TwoDGlobal':twoD,update:'@SceneUpdate@8',add2D:'@Add2D@12',add:'@ManagerAdd@16'}
addprimitive='?AddPrimitive@CEnginePrimitiveRenderer@@QAEXABVCEnginePrimitive@@@Z';ctor='??0CBaseClass@@QAE@XZ';dtor='??1CBaseClass@@UAE@XZ';handle='??_7CEnginePrimitiveHandle@@6B@'
targets.update({symbol:0xb324a0,ctor:0x99a2d0,dtor:0x99a2e0,handle:0x122f598})
aliases.update({'@AddPrimitive@12':addprimitive,ctor:'@BaseCtor@8',dtor:'@BaseDtor@8','_HandleTable':handle})
(w/'primitive_submission_aliases.h').write_text('\n'.join('#pragma comment(linker,"/alternatename:'+a+'='+b+'")' for a,b in aliases.items())+'\n')
raw=EXE.read_bytes();body=bytearray(504);fixups=[];md=Cs(CS_ARCH_X86,CS_MODE_32);md.detail=True
for address,length,offset in ((0xb324a0,354,0),(0xb32e90,136,368)):
 code=read_virtual_data(raw,address,length);body[offset:offset+length]=code
 for i in md.disasm(code,address):
  if i.mnemonic in ('call','jmp') and i.operands[0].type==2 and not address<=i.operands[0].imm<address+length:assert i.operands[0].imm in targets.values();fixups.append((offset+i.address-address+i.imm_offset,i.operands[0].imm,1))
  else:
   for o in i.operands:
    if o.type==3 and o.mem.disp in (0x1436e84,0x1436e7c):fixups.append((offset+i.address-address+i.disp_offset,o.mem.disp,0))
    elif o.type==2 and o.imm==0x122f598:fixups.append((offset+i.address-address+i.imm_offset,o.imm,0))
assert len(fixups)==10
(w/'primitive_submission_retail.h').write_text('static const unsigned char retailCode[]={'+','.join(map(str,body))+'};\nstatic const unsigned retailFixups[][3]={'+','.join('{%d,%d,%d}'%f for f in fixups)+'};\n')
objects=[]
for n,p in enumerate(sources+[fixture]):
 o=w/(str(n)+'.obj');r=V.cl(['/nologo','/c','/O2','/MT','/I'+str(w),'/Fo'+str(o),str(p)],env,w);assert not r.returncode,r.stdout+r.stderr;objects.append(o)
proofs=[check(objects[0],symbol,0xb324a0,354,5,targets),check(objects[3],addprimitive,0xb32e90,136,5,targets)]
for n,(sym,address) in enumerate(((typeinfo,0x1436e84),(twoD,0x1436e7c)),1):assert coff_data(Coff(objects[n]),sym,4)==read_virtual_data(raw,address,4)==bytes(4)
cmd=[str(V.VC/'bin/link.exe'),'/nologo','/out:'+str(w/'fixture.exe'),'kernel32.lib']
for omit in range(4):
 r=subprocess.run(cmd+[str(o) for i,o in enumerate(objects) if i!=omit],env=env,capture_output=True,text=True);assert r.returncode
r=subprocess.run(cmd+list(map(str,objects)),env=env,capture_output=True,text=True);assert not r.returncode,r.stdout+r.stderr
cases=0
with (w/'input.bin').open('wb') as f:
 for typ,present,mode,refs in itertools.product(range(47),range(2),range(6),(0,1,2,0xffffffff)):
  f.write(struct.pack('<8I',typ,0,present,4,mode,1,refs,0xff));cases+=1
for kind in ('retail','compiled'):
 r=subprocess.run([str(w/'fixture.exe'),str(w/'input.bin'),str(w/(kind+'.bin')),kind],env=env,capture_output=True,text=True,timeout=120);assert not r.returncode,(w,r.returncode,r.stdout,r.stderr);print(r.stdout.strip(),kind,flush=True)
with (w/'retail.bin').open('rb') as f,(w/'compiled.bin').open('rb') as g:
 while True:
  a=f.read(1<<20);b=g.read(1<<20);assert a==b,w
  if not a:break
size=(w/'compiled.bin').stat().st_size;assert size%4108==0;snapshots=size//4108
report=dict(cases=cases,snapshots=snapshots,omission_checks=4,providers=proofs,source_sha256={str(p.relative_to(ROOT)):hashlib.sha256(p.read_bytes()).hexdigest() for p in sources},fixture_sha256=hashlib.sha256(fixture.read_bytes()).hexdigest(),boundary='Actual temporary submission through persistent creation and final handle release; full4KiB+actualglobals+observed temporary Data.47types,managerpresent/null,sixcallbackmodes,fourrefcounts. Independent state-machine checks constructor,creation callbacks,adoption/update/routing,notify/deletion andbase destructor. Stackhandle identity/vtable/zero state validated within eachrun;addresses not compared. Base ctor/dtor are explicit observer boundaries;factory,virtuals anddownstreamrouting remaincallbacks.')
(ROOT/'rebuild/compile-gate/native-primitive-submission.json').write_text(json.dumps(report,indent=2)+'\n');print('NATIVE_PRIMITIVE_SUBMISSION_GATE_PASS',snapshots,'snapshots')
