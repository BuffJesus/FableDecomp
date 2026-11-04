"""Native subprimitive RenderType routing and render state capture."""
import hashlib,json,re,subprocess,tempfile
from pathlib import Path
from capstone import Cs,CS_ARCH_X86,CS_MODE_32
import check_one as V
from variable_allocator_parity import check
from gate_game_component_abi import Coff
from gate_render_state_static_data import coff_data
from pe_oracle import EXE,read_virtual_data
ROOT=Path(__file__).resolve().parents[2];w=Path(tempfile.mkdtemp(prefix='native-subprimitive-render-type-'));env=V.env()
sources=[ROOT/'rebuild/src/compiled'/p for p in ('00/b8/CEngineSubPrimitiveRenderer_RenderType_00b849f0.cpp','01/39/CRenderStateManager_StaticData_01396da8.cpp','01/43/NEngineInternal_Globals_01436e18.cpp','01/39/CRenderStateManager_D3DAlphaFuncModeTable_01396dd8.cpp')]
fixture=ROOT/'rebuild/tests/integration/NativeSubPrimitiveRenderType_test.cpp';root='?RenderType@CEngineSubPrimitiveRenderer@@QAEXW4ESubPrimitiveType@@_N@Z'
specs=[
('?FableCurrentRenderStateManager_013cb508@@3PAVCRenderStateManager@@A',0x13cb508,'_StateGlobal',1),
('?GPRenderManager@NEngineInternal@@3PAVCRenderManager@@A',0x1436e18,'_RenderGlobal',2),
('?D3DAlphaFuncModeTable@CRenderStateManager@@0PAKA',0x1396dd8,'_AlphaTable',3),
('?SortList@CEngineSubPrimitiveRenderer@@AAEXAAV?$CArray@VCSortElement@CEngineSubPrimitiveRenderer@@@@_N@Z',0xb84990,'@Sort@16',None),
('?RenderListTwoPass@CEngineSubPrimitiveRenderer@@AAEXABV?$CArray@VCSortElement@CEngineSubPrimitiveRenderer@@@@W4ERenderPassType@@1@Z',0xb84320,'@Two@20',None),
('?RenderList@CEngineSubPrimitiveRenderer@@AAEXABV?$CArray@VCSortElement@CEngineSubPrimitiveRenderer@@@@W4ERenderPassType@@@Z',0xb842d0,'@One@16',None),
('?DeleteList@CEngineSubPrimitiveRenderer@@AAEXAAV?$CArray@VCSortElement@CEngineSubPrimitiveRenderer@@@@@Z',0xb84400,'@Delete@12',None),
('?RestoreCaptureBlock@CRenderStateManager@@QAEXXZ',0xa05840,'@Restore@8',None)]

targets={s:a for s,a,n,o in specs};aliases={'@RenderType@16':root}
for s,a,n,o in specs:aliases[n if o is not None else s]=s if o is not None else n
(w/'subprimitive_render_type_aliases.h').write_text('\n'.join('#pragma comment(linker,"/alternatename:'+a+'='+b+'")' for a,b in aliases.items())+'\n')
raw=EXE.read_bytes();body=read_virtual_data(raw,0xb849f0,301);fixups=[];indices={a+(16 if i==2 else 0):i for i,(s,a,n,o) in enumerate(specs)};md=Cs(CS_ARCH_X86,CS_MODE_32);md.detail=True
for i in md.disasm(body,0xb849f0):
 if i.mnemonic in ('call','jmp') and i.operands[0].type==2 and not 0xb849f0<=i.operands[0].imm<0xb84b1d:
  fixups.append((i.address-0xb849f0+i.imm_offset,indices[i.operands[0].imm],1))
 else:
  for o in i.operands:
   if o.type==3 and o.mem.disp in indices:fixups.append((i.address-0xb849f0+i.disp_offset,indices[o.mem.disp],0))
assert len(fixups)==9
(w/'subprimitive_render_type_retail.h').write_text('static const unsigned char retailCode[]={'+','.join(map(str,body))+'};\nstatic const unsigned retailFixups[][3]={'+','.join('{%d,%d,%d}'%f for f in fixups)+'};\n')
objects=[]
for n,p in enumerate(sources+[fixture]):
 o=w/(str(n)+'.obj');r=V.cl(['/nologo','/c','/O2','/MT','/I'+str(w),'/Fo'+str(o),str(p)],env,w);assert not r.returncode,r.stdout+r.stderr;objects.append(o)
proof=check(objects[0],root,0xb849f0,301,9,targets)
for s,a,n,o in specs:
 if o is not None:
  size=32 if o==3 else 4
  assert coff_data(Coff(objects[o]),s,size)==read_virtual_data(raw,a,size)
link=[str(V.VC/'bin/link.exe'),'/nologo','/out:'+str(w/'fixture.exe'),'kernel32.lib']
for omit in range(4):
 r=subprocess.run(link+[str(o) for n,o in enumerate(objects) if n!=omit],env=env,capture_output=True,text=True);assert r.returncode
r=subprocess.run(link+list(map(str,objects)),env=env,capture_output=True,text=True);assert not r.returncode,r.stdout+r.stderr
r=subprocess.run([str(w/'fixture.exe')],env=env,capture_output=True,text=True,timeout=60);assert not r.returncode,(w,r.returncode,r.stdout,r.stderr);m=re.search(r'cases=(\d+) snapshots=(\d+)',r.stdout);assert m and int(m[1])==30720
report=dict(cases=int(m[1]),snapshots=int(m[2]),providers=[proof],omission_checks=4,source_sha256={str(p.relative_to(ROOT)):hashlib.sha256(p.read_bytes()).hexdigest() for p in sources},fixture_sha256=hashlib.sha256(fixture.read_bytes()).hexdigest(),boundary='Actual RenderType, two globals and alpha comparison table. Independent full48KiB and global expectations cover all15types,raw bool flags,0..3elements,alpha state capture/dirty branches,distinct state/render managers,global swaps,list replacement after sort,and list truncation during bounding callbacks. Sorting,render lists,deletion,restore,and manager bounding-volume execution remain boundaries; no rendered frame.')
(ROOT/'rebuild/compile-gate/native-subprimitive-render-type.json').write_text(json.dumps(report,indent=2)+'\n');print(r.stdout.strip());print('NATIVE_SUBPRIMITIVE_RENDER_TYPE_GATE_PASS',report['snapshots'],'snapshots')
