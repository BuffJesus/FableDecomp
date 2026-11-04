import hashlib,json,subprocess,tempfile,sys
from pathlib import Path
sys.path.insert(0,str(Path(__file__).resolve().parent))
import check_one as V
from variable_allocator_parity import check
from capstone import Cs,CS_ARCH_X86,CS_MODE_32
from pe_oracle import EXE,read_virtual_data
ROOT=Path(__file__).resolve().parents[2];w=Path(tempfile.mkdtemp(prefix='native-lighting-disable-'));env=V.env()
sources=[ROOT/'rebuild/src/compiled'/n for n in ('00/98/CShaderRenderManager_ResetLight_00988640.cpp','00/b4/CEngineLightingManager_DisablePointLights_00b44d50.cpp')]+[ROOT/'rebuild/src/compiled/01/43/NEngineInternal_GPShaderRenderManager_01436e14.cpp']
fixture=ROOT/'rebuild/tests/integration/NativeLightingDisable_test.cpp'
reset='?ResetLight@CShaderRenderManager@@QAEXK@Z';disable='?DisablePointLights@CEngineLightingManager@@QAEXXZ';sg='?GPShaderRenderManager@NEngineInternal@@3PAVCShaderRenderManager@@A'
targets={sg:0x1436e14,reset:0x988640}
aliases={'@ResetLight@12':reset,'@DisablePoints@8':disable,'_ShaderGlobal':sg}
(w/'lighting_disable_aliases.h').write_text('\n'.join('#pragma comment(linker,"/alternatename:'+a+'='+b+'")' for a,b in aliases.items())+'\n')
raw=EXE.read_bytes();body=bytearray(120);fixups=[];md=Cs(CS_ARCH_X86,CS_MODE_32);md.detail=True
specs=[(reset,0x988640,62,0,0),(disable,0xb44d50,56,2,64)]
for sym,address,length,count,offset in specs:
 code=read_virtual_data(raw,address,length);body[offset:offset+length]=code
 for i in md.disasm(code,address):
  if i.mnemonic in ('call','jmp') and i.operands[0].type==2 and not address<=i.operands[0].imm<address+length:
   assert i.operands[0].imm in targets.values();fixups.append((offset+i.address-address+i.imm_offset,i.operands[0].imm,1))
  else:
   for o in i.operands:
    if o.type==3 and o.mem.disp==0x1436e14:fixups.append((offset+i.address-address+i.disp_offset,o.mem.disp,0))
assert len(fixups)==2
(w/'lighting_disable_retail.h').write_text('static const unsigned char retailCode[]={'+','.join(map(str,body))+'};\nstatic const unsigned retailFixups[][3]={'+','.join('{%d,%d,%d}'%f for f in fixups)+'};\n')
objects=[]
for n,p in enumerate(sources+[fixture]):
 o=w/(str(n)+'.obj');r=V.cl(['/nologo','/c','/O2','/MT','/I'+str(w),'/Fo'+str(o),str(p)],env,w);assert not r.returncode,r.stdout+r.stderr;objects.append(o)
proofs=[check(objects[n],sym,a,l,c,targets) for n,(sym,a,l,c,off) in enumerate(specs)]
link=[str(V.VC/'bin/link.exe'),'/nologo','/out:'+str(w/'fixture.exe'),'kernel32.lib']
for omit in range(3):
 r=subprocess.run(link+[str(o) for n,o in enumerate(objects) if n!=omit],env=env,capture_output=True,text=True);assert r.returncode
r=subprocess.run(link+list(map(str,objects)),env=env,capture_output=True,text=True);assert not r.returncode,r.stdout+r.stderr
r=subprocess.run([str(w/'fixture.exe')],env=env,capture_output=True,text=True,timeout=60);assert not r.returncode,(w,r.returncode,r.stdout,r.stderr);print(r.stdout.strip())
assert 'cases=1152 snapshots=18432' in r.stdout
report=dict(cases=1152,snapshots=18432,providers=proofs,omission_checks=3,source_sha256={str(p.relative_to(ROOT)):hashlib.sha256(p.read_bytes()).hexdigest() for p in sources},fixture_sha256=hashlib.sha256(fixture.read_bytes()).hexdigest(),result=r.stdout.strip(),boundary='Native and compiled actual light reset and signed manager loop each compared to independent full16KiB model; actual shader global. No callbacks or rendering.')
(ROOT/'rebuild/compile-gate/native-lighting-disable.json').write_text(json.dumps(report,indent=2)+'\n')

print('NATIVE_LIGHTING_DISABLE_GATE_PASS 18432 snapshots')
