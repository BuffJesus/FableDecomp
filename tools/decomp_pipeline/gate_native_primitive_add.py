"""Temporary primitive handle lifetime and intrusive release callback ordering."""
import hashlib,itertools,json,struct,subprocess,tempfile
from pathlib import Path
from capstone import Cs,CS_ARCH_X86,CS_MODE_32
import check_one as V
from variable_allocator_parity import check
from gate_game_component_abi import Coff
from pe_oracle import EXE,read_virtual_data
ROOT=Path(__file__).resolve().parents[2];w=Path(tempfile.mkdtemp(prefix='native-primitive-add-'));env=V.env()
sources=[ROOT/'rebuild/src/compiled'/p for p in ('00/b3/CEnginePrimitiveRenderer_AddPrimitive_00b32e90.cpp','00/99/CBaseClass_CBaseClass_0099a2d0.cpp')];fixture=ROOT/'rebuild/tests/integration/NativePrimitiveAdd_test.cpp'
symbol='?AddPrimitive@CEnginePrimitiveRenderer@@QAEXABVCEnginePrimitive@@@Z';ctor='??0CBaseClass@@QAE@XZ';dtor='??1CBaseClass@@UAE@XZ';handle='??_7CEnginePrimitiveHandle@@6B@';base='??_7CBaseClass@@6B@';persistent='?SetPersistentPrimitive@CEnginePrimitiveRenderer@@QAEXAAVCEnginePrimitiveHandle@@ABVCEnginePrimitive@@K_N@Z'
targets={ctor:0x99a2d0,dtor:0x99a2e0,handle:0x122f598,base:0x1231710,persistent:0xb324a0}
aliases={'@AddPrimitive@12':symbol,'_HandleTable':handle,'_BaseTable':base,persistent:'@Persistent@24'}
(w/'primitive_add_aliases.h').write_text('\n'.join('#pragma comment(linker,"/alternatename:'+a+'='+b+'")' for a,b in aliases.items())+'\n')
raw=EXE.read_bytes();body=bytearray(167);fixups=[];md=Cs(CS_ARCH_X86,CS_MODE_32);md.detail=True
specs=[(symbol,0xb32e90,136,5,0),(ctor,0x99a2d0,9,1,144),(dtor,0x99a2e0,7,1,160)]
for sym,address,length,count,offset in specs:
 code=read_virtual_data(raw,address,length);body[offset:offset+length]=code
 for i in md.disasm(code,address):
  if i.mnemonic in ('call','jmp') and i.operands[0].type==2 and not address<=i.operands[0].imm<address+length:assert i.operands[0].imm in targets.values();fixups.append((offset+i.address-address+i.imm_offset,i.operands[0].imm,1))
  else:
   for o in i.operands:
    if o.type==2 and o.imm in (0x122f598,0x1231710):fixups.append((offset+i.address-address+i.imm_offset,o.imm,0))
assert len(fixups)==7
(w/'primitive_add_retail.h').write_text('static const unsigned char retailCode[]={'+','.join(map(str,body))+'};\nstatic const unsigned retailFixups[][3]={'+','.join('{%d,%d,%d}'%f for f in fixups)+'};\n')
objects=[]
for n,p in enumerate(sources+[fixture]):
 o=w/(str(n)+'.obj');r=V.cl(['/nologo','/c','/O2','/MT','/I'+str(w),'/Fo'+str(o),str(p)],env,w);assert not r.returncode,r.stdout+r.stderr;objects.append(o)
proofs=[check(objects[0 if n==0 else 1],sym,address,length,count,targets) for n,(sym,address,length,count,offset) in enumerate(specs)]
c=Coff(objects[0]);sec,off=c.symbols[handle];assert off==0 and len(c.sections[sec][0])==4 and list(c.sections[sec][1].values())==[('??_ECEnginePrimitiveHandle@@UAEPAXI@Z',6)]
cmd=[str(V.VC/'bin/link.exe'),'/nologo','/out:'+str(w/'fixture.exe'),'kernel32.lib']
for omit in range(2):
 r=subprocess.run(cmd+[str(o) for i,o in enumerate(objects) if i!=omit],env=env,capture_output=True,text=True);assert r.returncode
r=subprocess.run(cmd+list(map(str,objects)),env=env,capture_output=True,text=True);assert not r.returncode,r.stdout+r.stderr
cases=0
with (w/'input.bin').open('wb') as f:
 for slot,present,refs,otherrefs,mode,alias in itertools.product(range(2),range(3),(0,1,2,0x7fffffff,0xffffffff),(0,1,2,0xffffffff),range(6),range(3)):
  f.write(struct.pack('<6I',slot,present,refs,otherrefs,mode,alias));f.write(bytes([0x95])*1024);cases+=1
for kind in ('retail','compiled'):
 r=subprocess.run([str(w/'fixture.exe'),str(w/'input.bin'),str(w/(kind+'.bin')),kind],env=env,capture_output=True,text=True,timeout=60);assert not r.returncode,(w,r.returncode,r.stdout,r.stderr);print(r.stdout.strip(),kind,flush=True)
assert (w/'retail.bin').read_bytes()==(w/'compiled.bin').read_bytes(),w
size=(w/'compiled.bin').stat().st_size;assert size%1024==0;snapshots=size//1024
report=dict(cases=cases,snapshots=snapshots,omission_checks=2,providers=proofs,source_sha256={str(p.relative_to(ROOT)):hashlib.sha256(p.read_bytes()).hexdigest() for p in sources},fixture_sha256=hashlib.sha256(fixture.read_bytes()).hexdigest(),boundary='Actual temporary handle destructor and base ctor/dtor. Full1KiB independent state;temporary stack pointer/vtable/zero-data checked per run,not normalized snapshots. Persistent callback sees flags4/nonpersistenttrue;null/two resources,counts0/1/2/MAX_INT/MAX_UINT,notify clears/replaces handle orchangesrefcount,delete callbackchangeshandle. Only notify if originalrefcount1; rereads pointer afterward, decrements thenvirtualdelete ifzero. Underlying creation and pointee virtual operations remaincallbacks. Emitted handle deleting-destructor body not separately promoted.')
(ROOT/'rebuild/compile-gate/native-primitive-add.json').write_text(json.dumps(report,indent=2)+'\n');print('NATIVE_PRIMITIVE_ADD_GATE_PASS',snapshots,'snapshots')
