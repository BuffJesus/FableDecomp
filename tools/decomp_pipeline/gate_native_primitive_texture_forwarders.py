"""Real engine forwarders and singleton storage; renderer/resource calls explicit."""
import hashlib,itertools,json,struct,subprocess,tempfile
from pathlib import Path
from capstone import Cs,CS_ARCH_X86,CS_MODE_32
import check_one as V
from variable_allocator_parity import check
from gate_game_component_abi import Coff
from gate_render_state_static_data import coff_data
from pe_oracle import EXE,read_virtual_data
ROOT=Path(__file__).resolve().parents[2];w=Path(tempfile.mkdtemp(prefix='native-primitive-texture-'));env=V.env()
sources=[ROOT/'rebuild/src/compiled'/p for p in ('00/b2/CEngine_PrimitiveTextureForwarders_00b23be0.cpp','01/43/NEngineInternal_GPPrimitiveRenderer_01436e80.cpp','01/43/NEngineInternal_GPResourceManager_01436e98.cpp')]
fixture=ROOT/'rebuild/tests/integration/NativePrimitiveTextureForwarders_test.cpp'
specs=[('@Animation@16','?SetPrimitiveAnimation@CEngine@@UAEXAAVCEnginePrimitiveHandle@@ABVCEnginePrimitiveAnimation@@@Z',0xb23be0),('@Position@16','?SetPrimitivePosition@CEngine@@UAEXAAVCEnginePrimitiveHandle@@ABVCEnginePrimitivePosition@@@Z',0xb23bf0),('@Allocate@24','?AllocateTextureFromEngineResourcePools@CEngine@@UAEPAVCTexture@@KABVC2DExtentsI@@JABVCPixelFormat@@@Z',0xb23f40),('@Free@12','?FreeTextureFromEngineResourcePools@CEngine@@UAEXPAVCTexture@@@Z',0xb23f50)]
globals={'?GPPrimitiveRenderer@NEngineInternal@@3PAVCEnginePrimitiveRenderer@@A':0x1436e80,'?GPResourceManager@NEngineInternal@@3PAVCEngineResourceManager@@A':0x1436e98}
calls=[('?SetPrimitiveAnimation@CEnginePrimitiveRenderer@@QAEXAAVCEnginePrimitiveHandle@@ABVCEnginePrimitiveAnimation@@@Z','@SetAnimation@16',0xb31e90),('?SetPrimitivePosition@CEnginePrimitiveRenderer@@QAEXAAVCEnginePrimitiveHandle@@ABVCEnginePrimitivePosition@@@Z','@SetPosition@16',0xb31ec0),('?AllocateExternalTextureFromResourcePools@CEngineResourceManager@@QAEPAVCTexture@@KABVC2DExtentsI@@JABVCPixelFormat@@@Z','@AllocateExternal@24',0xb3b1f0),('?FreeExternalTextureFromResourcePools@CEngineResourceManager@@QAEXPAVCTexture@@@Z','@FreeExternal@12',0xb3b5b0)]
targets=dict(globals);targets.update({s:a for s,_,a in calls});aliases={a:s for a,s,_ in specs};aliases.update({s:a for s,a,_ in calls});aliases.update(dict(zip(('_PrimitiveGlobal','_ResourceGlobal'),globals)))
(w/'primitive_texture_aliases.h').write_text('\n'.join('#pragma comment(linker,"/alternatename:'+a+'='+b+'")' for a,b in aliases.items())+'\n')
body=bytearray(64);fixups=[];raw=EXE.read_bytes();md=Cs(CS_ARCH_X86,CS_MODE_32);md.detail=True
for n,(_,symbol,address) in enumerate(specs):
 code=read_virtual_data(raw,address,11);body[n*16:n*16+11]=code
 for i in md.disasm(code,address):
  if i.mnemonic=='jmp':assert i.operands[0].imm in targets.values();fixups.append((n*16+i.address-address+i.imm_offset,i.operands[0].imm,1))
  else:
   for op in i.operands:
    if op.type==3 and op.mem.disp in globals.values():fixups.append((n*16+i.address-address+i.disp_offset,op.mem.disp,0))
assert len(fixups)==8
(w/'primitive_texture_retail.h').write_text('static const unsigned char retailCode[]={'+','.join(map(str,body))+'};\nstatic const unsigned retailFixups[][3]={'+','.join('{%d,%d,%d}'%f for f in fixups)+'};\n')
objects=[]
for n,p in enumerate(sources+[fixture]):
 o=w/(str(n)+'.obj');r=V.cl(['/nologo','/c','/O2','/MT','/I'+str(w),'/Fo'+str(o),str(p)],env,w);assert not r.returncode,r.stdout+r.stderr;objects.append(o)
proofs=[check(objects[0],s,a,11,2,targets) for _,s,a in specs]
for n,(s,a) in enumerate(globals.items(),1):assert coff_data(Coff(objects[n]),s,4)==read_virtual_data(raw,a,4)==bytes(4)
cmd=[str(V.VC/'bin/link.exe'),'/nologo','/out:'+str(w/'fixture.exe'),'kernel32.lib']
for omit in range(3):
 r=subprocess.run(cmd+[str(p) for i,p in enumerate(objects) if i!=omit],env=env,capture_output=True,text=True);assert r.returncode
r=subprocess.run(cmd+list(map(str,objects)),env=env,capture_output=True,text=True);assert not r.returncode,r.stdout+r.stderr
cases=0
with (w/'input.bin').open('wb') as f:
 for mode,slot,flags,levels,result,alias in itertools.product(range(8),range(2),(0,0xffffffff,0x12345678),(0,0xffffffff,7),(0,0x180,0x100),(0x100,0x120,0x140)):
  f.write(struct.pack('<6I',mode,slot,flags,levels,result,alias));f.write(bytes([0x95])*4096);cases+=1
for kind in ('retail','compiled'):
 r=subprocess.run([str(w/'fixture.exe'),str(w/'input.bin'),str(w/(kind+'.bin')),kind],env=env,capture_output=True,text=True,timeout=60);assert not r.returncode,(w,r.returncode,r.stdout,r.stderr);print(r.stdout.strip(),kind,flush=True)
assert (w/'retail.bin').read_bytes()==(w/'compiled.bin').read_bytes(),w
report=dict(cases=cases,snapshots=cases*5,omission_checks=3,providers=proofs,source_sha256={str(p.relative_to(ROOT)):hashlib.sha256(p.read_bytes()).hexdigest() for p in sources},fixture_sha256=hashlib.sha256(fixture.read_bytes()).hexdigest(),boundary='Four exact tailcalls/eight fixups, two actual singleton globals with zero-storage audits. Full4KiB arena, callbacks and return/global values. Four-call sequences check every argument, null/non-null/alias texture returns, arbitrary unsigned flags/signed levels, argument overlap and callback singleton replacements. Renderer setters and texture pool allocation/free remain explicit boundaries.')
(ROOT/'rebuild/compile-gate/native-primitive-texture-forwarders.json').write_text(json.dumps(report,indent=2)+'\n');print('NATIVE_PRIMITIVE_TEXTURE_FORWARDERS_GATE_PASS',cases*5,'snapshots')
