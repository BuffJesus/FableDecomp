"""Color-filtered quad routing with actual capture restoration and raw float forwarding."""
import hashlib,itertools,json,struct,subprocess,tempfile
from pathlib import Path
from capstone import Cs,CS_ARCH_X86,CS_MODE_32
import check_one as V
from variable_allocator_parity import check
from pe_oracle import EXE,read_virtual_data
ROOT=Path(__file__).resolve().parents[2];w=Path(tempfile.mkdtemp(prefix='native-colour-quad-'));env=V.env()
sources=[ROOT/'rebuild/src/compiled'/p for p in ('00/b2/CEngine_RenderColorFilteredQuad_00b25c00.cpp','00/a0/CRenderStateManager_RestoreCaptureBlock_00a05840.cpp')];fixture=ROOT/'rebuild/tests/integration/NativeColourQuad_test.cpp'
symbol='?RenderColorFilteredQuad@CEngine@@UAEXMMABVC4DVector@@MMPAVCTexture@@@Z';restore='?RestoreCaptureBlock@CRenderStateManager@@QAEXXZ'
targets={'?FableCurrentRenderStateManager_013cb508@@3PAVCRenderStateManager@@A':0x13cb508,'?FableShaderRenderManager_013bc470@@3VCShaderRenderManager@@A':0x13bc470,'?GPScreenEffectColourFilter@NEngineInternal@@3PAVCEngineScreenEffectColourFilter@@A':0x1436e40,'?SetDefaultRenderStates@CEngine@@AAEXXZ':0xb24850,'?SetVertexShaderConstantLayout@CShaderRenderManager@@QAEXJ@Z':0x98b5e0,restore:0xa05840,'?RenderColorFilteredQuad@CEngineScreenEffectColourFilter@@QAEXMMABVC4DVector@@MMPAVCTexture@@@Z':0xb5af80}
callbacks={0x13cb508:'_StateGlobal',0x13bc470:'_Shader',0x1436e40:'_EffectGlobal',0xb24850:'@Defaults@8',0x98b5e0:'@SetLayout@12',0xb5af80:'@DrawQuad@32'}
aliases={k:callbacks[v] for k,v in targets.items() if v in callbacks};aliases['@RenderQuad@32']=symbol
(w/'colour_quad_aliases.h').write_text('\n'.join('#pragma comment(linker,"/alternatename:'+a+'='+b+'")' for a,b in aliases.items())+'\n')
raw=EXE.read_bytes();body=bytearray(249);fixups=[];md=Cs(CS_ARCH_X86,CS_MODE_32);md.detail=True
for address,length,offset in ((0xb25c00,124,0),(0xa05840,121,128)):
 code=read_virtual_data(raw,address,length);body[offset:offset+length]=code
 for i in md.disasm(code,address):
  if i.mnemonic in ('call','jmp') and i.operands[0].type==2 and not address<=i.operands[0].imm<address+length:assert i.operands[0].imm in targets.values();fixups.append((offset+i.address-address+i.imm_offset,i.operands[0].imm,1))
  else:
   for o in i.operands:
    if o.type==3 and o.mem.disp in (0x13cb508,0x13bc478,0x1436e40):fixups.append((offset+i.address-address+i.disp_offset,o.mem.disp,0))
    elif o.type==2 and o.imm==0x13bc470:fixups.append((offset+i.address-address+i.imm_offset,o.imm,0))
assert len(fixups)==11
(w/'colour_quad_retail.h').write_text('static const unsigned char retailCode[]={'+','.join(map(str,body))+'};\nstatic const unsigned retailFixups[][3]={'+','.join('{%d,%d,%d}'%f for f in fixups)+'};\n')
objects=[]
for n,p in enumerate(sources+[fixture]):
 o=w/(str(n)+'.obj');r=V.cl(['/nologo','/c','/O2','/MT','/I'+str(w),'/Fo'+str(o),str(p)],env,w);assert not r.returncode,r.stdout+r.stderr;objects.append(o)
proofs=[check(objects[0],symbol,0xb25c00,124,11,targets),check(objects[1],restore,0xa05840,121,0,targets)]
cmd=[str(V.VC/'bin/link.exe'),'/nologo','/out:'+str(w/'fixture.exe'),'kernel32.lib']
for omit in range(2):
 r=subprocess.run(cmd+[str(o) for i,o in enumerate(objects) if i!=omit],env=env,capture_output=True,text=True);assert r.returncode
r=subprocess.run(cmd+list(map(str,objects)),env=env,capture_output=True,text=True);assert not r.returncode,r.stdout+r.stderr
cases=0
with (w/'input.bin').open('wb') as f:
 for slot,count,mask,layout,mode,bits in itertools.product(range(2),range(4),(1,0x40000000,0x80000000),(0,3,0xffffffff),range(4),(0,0x80000000,0x3f800000,0x7f800000,0x7fc12345,0x7f812345)):
  f.write(struct.pack('<6I',slot,count,mask,layout,mode,bits));cases+=1
for kind in ('retail','compiled'):
 r=subprocess.run([str(w/'fixture.exe'),str(w/'input.bin'),str(w/(kind+'.bin')),kind],env=env,capture_output=True,text=True,timeout=120);assert not r.returncode,(w,r.returncode,r.stdout,r.stderr);print(r.stdout.strip(),kind,flush=True)
with (w/'retail.bin').open('rb') as f,(w/'compiled.bin').open('rb') as g:
 while True:
  a=f.read(1<<20);b=g.read(1<<20);assert a==b,w
  if not a:break
size=(w/'compiled.bin').stat().st_size;assert size%66544==0;snapshots=size//66544
report=dict(cases=cases,snapshots=snapshots,omission_checks=2,providers=proofs,source_sha256={str(p.relative_to(ROOT)):hashlib.sha256(p.read_bytes()).hexdigest() for p in sources},fixture_sha256=hashlib.sha256(fixture.read_bytes()).hexdigest(),boundary='Actual quad wrapper and capture restoration. Full64KiB arena,1000B fixture shader object,two globals;independent capture stack/bookmark/dirty-list model. Raw float bits including NaNs forwarded without arithmetic;callbacks mutate shader layout,effect singleton,colour data and current state-manager. Final restore reloads manager and restores cached pre-layout index. Default-state setup,shader setter,effect drawing and global storage remain explicit fixture boundaries. No rasterized frame.')
(ROOT/'rebuild/compile-gate/native-colour-quad.json').write_text(json.dumps(report,indent=2)+'\n');print('NATIVE_COLOUR_QUAD_GATE_PASS',snapshots,'snapshots')
