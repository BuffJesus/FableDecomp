"""Identity reset reaches actual shader state and COM matrix submission."""
import hashlib,json,subprocess,tempfile
from pathlib import Path
from capstone import Cs,CS_ARCH_X86,CS_MODE_32
import check_one as V
from variable_allocator_parity import check
from gate_game_component_abi import Coff
from gate_render_state_static_data import coff_data
from pe_oracle import EXE,read_virtual_data
ROOT=Path(__file__).resolve().parents[2];w=Path(tempfile.mkdtemp(prefix='native-primitive-transform-reset-'));env=V.env()
sources=[ROOT/'rebuild/src/compiled'/p for p in ('00/b3/CEnginePrimitiveRenderer_ResetWorldTransform_00b32100.cpp','00/98/CShaderRenderManager_MatrixSetters_009881f0.cpp','00/a0/CRenderManagerCore_MatrixSetters_00a0b0a0.cpp','01/43/NEngineInternal_GPShaderRenderManager_01436e14.cpp','01/43/NEngineInternal_Globals_01436e18.cpp','00/a5/CMatrix4x4_Transpose_00a5cab0.cpp')]
fixture=ROOT/'rebuild/tests/integration/NativePrimitiveTransformReset_test.cpp'
reset='?ResetWorldTransform@CEnginePrimitiveRenderer@@AAEXXZ';shader='?SetWorldTransform@CShaderRenderManager@@QAEXABVCMatrix3x4@@@Z';core='?SetWorldTransform@CRenderManagerCore@@QAEXABVCMatrix3x4@@@Z';sg='?GPShaderRenderManager@NEngineInternal@@3PAVCShaderRenderManager@@A';rg='?GPRenderManager@NEngineInternal@@3PAVCRenderManager@@A'
targets={sg:0x1436e14,rg:0x1436e18,shader:0x9881f0,core:0xa0b140};aliases={'@Reset@8':reset,'_ShaderGlobal':sg,'_RenderGlobal':rg}
(w/'primitive_transform_reset_aliases.h').write_text('\n'.join('#pragma comment(linker,"/alternatename:'+a+'='+b+'")' for a,b in aliases.items())+'\n')
raw=EXE.read_bytes();body=bytearray(455);fixups=[];md=Cs(CS_ARCH_X86,CS_MODE_32);md.detail=True
specs=[(reset,0xb32100,133,4,0),(shader,0x9881f0,160,0,144),(core,0xa0b140,151,0,304)]
for sym,address,length,count,offset in specs:
 code=read_virtual_data(raw,address,length);body[offset:offset+length]=code
 for i in md.disasm(code,address):
  if i.mnemonic in ('call','jmp') and i.operands[0].type==2 and not address<=i.operands[0].imm<address+length:
   assert i.operands[0].imm in targets.values();fixups.append((offset+i.address-address+i.imm_offset,i.operands[0].imm,1))
  else:
   for o in i.operands:
    if o.type==3 and o.mem.disp in (0x1436e14,0x1436e18):fixups.append((offset+i.address-address+i.disp_offset,o.mem.disp,0))
assert len(fixups)==4
(w/'primitive_transform_reset_retail.h').write_text('static const unsigned char retailCode[]={'+','.join(map(str,body))+'};\nstatic const unsigned retailFixups[][3]={'+','.join('{%d,%d,%d}'%f for f in fixups)+'};\n')
objects=[]
for n,p in enumerate(sources+[fixture]):
 o=w/(str(n)+'.obj');r=V.cl(['/nologo','/c','/O2','/MT','/I'+str(w),'/Fo'+str(o),str(p)],env,w);assert not r.returncode,r.stdout+r.stderr;objects.append(o)
proofs=[check(objects[n],sym,a,l,c,targets) for n,(sym,a,l,c,off) in enumerate(specs)]
for n,sym,address in ((3,sg,0x1436e14),(4,rg,0x1436e18)):assert coff_data(Coff(objects[n]),sym,4)==read_virtual_data(raw,address,4)==bytes(4)
link=[str(V.VC/'bin/link.exe'),'/nologo','/out:'+str(w/'fixture.exe'),'kernel32.lib']
for omit in range(5):
 r=subprocess.run(link+[str(o) for n,o in enumerate(objects) if n!=omit],env=env,capture_output=True,text=True);assert r.returncode
r=subprocess.run(link+list(map(str,objects)),env=env,capture_output=True,text=True);assert not r.returncode,r.stdout+r.stderr
r=subprocess.run([str(w/'fixture.exe')],env=env,capture_output=True,text=True,timeout=60);assert not r.returncode,(w,r.returncode,r.stdout,r.stderr);assert 'NATIVE_PRIMITIVE_TRANSFORM_RESET_PASS cases=384 snapshots=6912' in r.stdout
report=dict(cases=384,snapshots=6912,omission_checks=5,providers=proofs,source_sha256={str(p.relative_to(ROOT)):hashlib.sha256(p.read_bytes()).hexdigest() for p in sources},fixture_sha256=hashlib.sha256(fixture.read_bytes()).hexdigest(),boundary='Actual identity reset,shader world setter,renderer world setter,two singleton globals. Full40KiB plusglobals snapshots and actual64B COM identity matrix,three-reset sequences with pointer/device swaps,flag changes,HRESULT failures. Only COM SetTransform callback. Sixth source is actualTranspose link support for unused projection setter in grouped shader object;five providers mandatory for exercised chain. Existing core setter accepted91.1392% near-match unchanged. No visual frame.')
(ROOT/'rebuild/compile-gate/native-primitive-transform-reset.json').write_text(json.dumps(report,indent=2)+'\n');print(r.stdout.strip());print('NATIVE_PRIMITIVE_TRANSFORM_RESET_GATE_PASS 6912 snapshots')
