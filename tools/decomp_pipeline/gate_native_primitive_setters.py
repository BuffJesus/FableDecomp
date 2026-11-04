"""Primitive animation/position dispatch through real engine and renderer methods."""
import hashlib,itertools,json,struct,subprocess,tempfile
from pathlib import Path
from capstone import Cs,CS_ARCH_X86,CS_MODE_32
import check_one as V
from variable_allocator_parity import check
from gate_game_component_abi import Coff
from gate_render_state_static_data import coff_data
from pe_oracle import EXE,read_virtual_data
ROOT=Path(__file__).resolve().parents[2];w=Path(tempfile.mkdtemp(prefix='native-primitive-setters-'));env=V.env()
sources=[ROOT/'rebuild/src/compiled'/p for p in ('00/b3/CEnginePrimitiveRenderer_PrimitiveSetters_00b31e90.cpp','00/b2/CEngine_PrimitiveTextureForwarders_00b23be0.cpp','01/43/NEngineInternal_GPPrimitiveRenderer_01436e80.cpp','01/43/NEngineInternal_GPResourceManager_01436e98.cpp')];fixture=ROOT/'rebuild/tests/integration/NativePrimitiveSetters_test.cpp'
animation='?SetPrimitiveAnimation@CEnginePrimitiveRenderer@@QAEXAAVCEnginePrimitiveHandle@@ABVCEnginePrimitiveAnimation@@@Z';position='?SetPrimitivePosition@CEnginePrimitiveRenderer@@QAEXAAVCEnginePrimitiveHandle@@ABVCEnginePrimitivePosition@@@Z';parentA='?SetPrimitiveAnimation@CEngine@@UAEXAAVCEnginePrimitiveHandle@@ABVCEnginePrimitiveAnimation@@@Z';parentP='?SetPrimitivePosition@CEngine@@UAEXAAVCEnginePrimitiveHandle@@ABVCEnginePrimitivePosition@@@Z';update='?UpdatePosition@CEngineScene@@SIXAAVCEngineInternalPrimitiveBase@@K@Z';pg='?GPPrimitiveRenderer@NEngineInternal@@3PAVCEnginePrimitiveRenderer@@A';rg='?GPResourceManager@NEngineInternal@@3PAVCEngineResourceManager@@A'
targets={animation:0xb31e90,position:0xb31ec0,update:0xb91840,pg:0x1436e80}
aliases={'@Animation@16':parentA,'@Position@16':parentP,'@DirectAnimation@16':animation,'@DirectPosition@16':position,update:'@Update@8','_PrimitiveGlobal':pg,'_ResourceGlobal':rg,'?AllocateExternalTextureFromResourcePools@CEngineResourceManager@@QAEPAVCTexture@@KABVC2DExtentsI@@JABVCPixelFormat@@@Z':'@AllocateExternal@24','?FreeExternalTextureFromResourcePools@CEngineResourceManager@@QAEXPAVCTexture@@@Z':'@FreeExternal@12'}
(w/'primitive_setters_aliases.h').write_text('\n'.join('#pragma comment(linker,"/alternatename:'+a+'='+b+'")' for a,b in aliases.items())+'\n')
raw=EXE.read_bytes();body=bytearray(128);fixups=[];md=Cs(CS_ARCH_X86,CS_MODE_32);md.detail=True
specs=[(1,parentA,0xb23be0,11,2,0),(1,parentP,0xb23bf0,11,2,16),(0,animation,0xb31e90,45,1,32),(0,position,0xb31ec0,45,1,80)]
for obj,sym,address,length,count,offset in specs:
 code=read_virtual_data(raw,address,length);body[offset:offset+length]=code
 for i in md.disasm(code,address):
  if i.mnemonic in ('call','jmp') and i.operands[0].type==2:fixups.append((offset+i.address-address+i.imm_offset,i.operands[0].imm,1))
  else:
   for o in i.operands:
    if o.type==3 and o.mem.disp==0x1436e80:fixups.append((offset+i.address-address+i.disp_offset,o.mem.disp,0))
assert len(fixups)==6
(w/'primitive_setters_retail.h').write_text('static const unsigned char retailCode[]={'+','.join(map(str,body))+'};\nstatic const unsigned retailFixups[][3]={'+','.join('{%d,%d,%d}'%f for f in fixups)+'};\n')
objects=[]
for n,p in enumerate(sources+[fixture]):
 o=w/(str(n)+'.obj');r=V.cl(['/nologo','/c','/O2','/MT','/I'+str(w),'/Fo'+str(o),str(p)],env,w);assert not r.returncode,r.stdout+r.stderr;objects.append(o)
proofs=[check(objects[n],s,a,l,c,targets) for n,s,a,l,c,off in specs]
assert coff_data(Coff(objects[2]),pg,4)==coff_data(Coff(objects[3]),rg,4)==bytes(4)
cmd=[str(V.VC/'bin/link.exe'),'/nologo','/out:'+str(w/'fixture.exe'),'kernel32.lib']
for omit in range(4):
 r=subprocess.run(cmd+[str(o) for i,o in enumerate(objects) if i!=omit],env=env,capture_output=True,text=True);assert r.returncode
r=subprocess.run(cmd+list(map(str,objects)),env=env,capture_output=True,text=True);assert not r.returncode,r.stdout+r.stderr
cases=0
with (w/'input.bin').open('wb') as f:
 for base,primitive,level,mode,arg,direct in itertools.product((0,0x400),(0,0xa00,0xb00),range(256),range(4),(0xc00,0x804,0xa0c,0xa24),range(2)):
  flags=(level*0x01010101)^0xa5a55a5a;f.write(struct.pack('<7I',base,primitive,level,mode,flags,arg,direct));cases+=1
for kind in ('retail','compiled'):
 r=subprocess.run([str(w/'fixture.exe'),str(w/'input.bin'),str(w/(kind+'.bin')),kind],env=env,capture_output=True,text=True,timeout=120);assert not r.returncode,(w,r.returncode,r.stdout,r.stderr);print(r.stdout.strip(),kind,flush=True)
with (w/'retail.bin').open('rb') as f,(w/'compiled.bin').open('rb') as g:
 while True:
  a=f.read(1<<20);b=g.read(1<<20);assert a==b,w
  if not a:break
size=(w/'compiled.bin').stat().st_size;assert size%4104==0;snapshots=size//4104
headers=[ROOT/'rebuild/include'/p for p in ('fable_engine_primitive_handle.h','fable_iv_counted_pointer.h','fable_game_text_bank.h','engine/CEngineInternalPrimitiveBase.h','engine/CEngineScene.h')]
report=dict(cases=cases,snapshots=snapshots,omission_checks=4,providers=proofs,source_sha256={str(p.relative_to(ROOT)):hashlib.sha256(p.read_bytes()).hexdigest() for p in sources},header_sha256={str(p.relative_to(ROOT)):hashlib.sha256(p.read_bytes()).hexdigest() for p in headers},fixture_sha256=hashlib.sha256(fixture.read_bytes()).hexdigest(),boundary='Actual engine forwarders/renderer setters with existing singleton storage. Four mandatory sources/four native spans/six fixups. Two-call sequences,all256grid-level bytes,null/two primitive pointers,two receiver placements,aliases in arguments,four mutation modes,both direct and engine entry. Full4KiB and both globals independently checked at virtual/scene callbacks and return. Original primitive cached across handle mutation;post-set level/flags reread;negative signed grid level suppresses scene update. Virtual slots23/24 and Scene::UpdatePosition are boundaries;unused sibling resource calls trap.')
(ROOT/'rebuild/compile-gate/native-primitive-setters.json').write_text(json.dumps(report,indent=2)+'\n');print('NATIVE_PRIMITIVE_SETTERS_GATE_PASS',snapshots,'snapshots')
