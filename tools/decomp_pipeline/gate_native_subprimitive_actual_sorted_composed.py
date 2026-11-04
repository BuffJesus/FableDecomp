"""Native subprimitive RenderType routing and render state capture."""
import hashlib,json,re,subprocess,tempfile
from pathlib import Path
from capstone import Cs,CS_ARCH_X86,CS_MODE_32
import check_one as V
from variable_allocator_parity import check
from gate_game_component_abi import Coff
from gate_render_state_static_data import coff_data
from pe_oracle import EXE,read_virtual_data
ROOT=Path(__file__).resolve().parents[2];w=Path(tempfile.mkdtemp(prefix='native-subprimitive-actual-sorted-composed-'));env=V.env()
sources=[ROOT/'rebuild/src/compiled'/p for p in ('00/b8/CEngineSubPrimitiveRenderer_RenderType_00b849f0.cpp','01/39/CRenderStateManager_StaticData_01396da8.cpp','01/43/NEngineInternal_Globals_01436e18.cpp','01/39/CRenderStateManager_D3DAlphaFuncModeTable_01396dd8.cpp')]
sources += [ROOT/'rebuild/src/compiled/00/b8'/p for p in ('CEngineSubPrimitiveRenderer_RenderList_00b842d0.cpp','CEngineSubPrimitiveRenderer_RenderListTwoPass_00b84320.cpp','CEngineSubPrimitiveRenderer_DeleteList_00b84400.cpp')]
sources.append(ROOT/'rebuild/src/compiled/00/a0/CRenderStateManager_RestoreCaptureBlock_00a05840.cpp')
sources.append(ROOT/'rebuild/src/compiled/00/b8/CEngineSubPrimitiveRenderer_SortList_00b84990.cpp')
import ast
fulltree=ast.parse((ROOT/'tools/decomp_pipeline/gate_native_subprimitive_full_sort.py').read_text())
fullspecs=next(ast.literal_eval(n.value) for n in fulltree.body if isinstance(n,ast.Assign) and any(isinstance(t,ast.Name) and t.id=='specs' for t in n.targets))
sources += [ROOT/'rebuild/src/compiled/00/b8'/spec[0] for spec in fullspecs]
fixture=ROOT/'rebuild/tests/integration/NativeSubPrimitiveActualSortedComposed_test.cpp';root='?RenderType@CEngineSubPrimitiveRenderer@@QAEXW4ESubPrimitiveType@@_N@Z'
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
for s,a,n,o in specs:
 if a not in (0xb842d0,0xb84320,0xb84400,0xa05840,0xb84990):aliases[n if o is not None else s]=s if o is not None else n
aliases['@Sort@16']=specs[3][0]
aliases['?RadixSort@CEngineSubPrimitiveRenderer@@AAEXAAV?$CArray@VCSortElement@CEngineSubPrimitiveRenderer@@@@@Z']='@RadixBoundary@12'
(w/'subprimitive_actual_sorted_composed_aliases.h').write_text('\n'.join('#pragma comment(linker,"/alternatename:'+a+'='+b+'")' for a,b in aliases.items())+'\n')
raw=EXE.read_bytes();body=read_virtual_data(raw,0xb849f0,301);fixups=[];indices={a+(16 if i==2 else 0):i for i,(s,a,n,o) in enumerate(specs)};md=Cs(CS_ARCH_X86,CS_MODE_32);md.detail=True
for i in md.disasm(body,0xb849f0):
 if i.mnemonic in ('call','jmp') and i.operands[0].type==2 and not 0xb849f0<=i.operands[0].imm<0xb84b1d:
  fixups.append((i.address-0xb849f0+i.imm_offset,indices[i.operands[0].imm],1))
 else:
  for o in i.operands:
   if o.type==3 and o.mem.disp in indices:fixups.append((i.address-0xb849f0+i.disp_offset,indices[o.mem.disp],0))
assert len(fixups)==9
body=bytearray(body)+bytearray(818-len(body))
for a,l,offset in [(0xb842d0,69,304),(0xb84320,100,384),(0xb84400,97,496),(0xa05840,121,608),(0xb84990,82,736)]:body[offset:offset+l]=read_virtual_data(raw,a,l)
(w/'subprimitive_actual_sorted_composed_retail.h').write_text('static const unsigned char retailCode[]={'+','.join(map(str,body))+'};\nstatic const unsigned retailFixups[][3]={'+','.join('{%d,%d,%d}'%f for f in fixups)+'};\n')
sortfix=[]
for i in md.disasm(read_virtual_data(raw,0xb84990,82),0xb84990):
 if i.mnemonic=='call':sortfix.append((i.address-0xb84990+i.imm_offset,{0xb86710:0,0xb868c0:1,0xb847d0:2}[i.operands[0].imm]))
with (w/'subprimitive_actual_sorted_composed_retail.h').open('a') as f:f.write('static const unsigned sortFixups[][2]={'+','.join('{%d,%d}'%v for v in sortfix)+'};\n')
fullbody=bytearray(1235);fullfix=[]
for name,prefix,address,length,offset in fullspecs:
 span=read_virtual_data(raw,address,length);fullbody[offset:offset+length]=span
 for ins in md.disasm(span,address):
  if ins.mnemonic=='call':fullfix.append((offset+ins.address-address+ins.imm_offset,next(s[4] for s in fullspecs if s[2]==ins.operands[0].imm)))
with (w/'subprimitive_actual_sorted_composed_retail.h').open('a') as f:f.write('static const unsigned char fullSortCode[]={'+','.join(map(str,fullbody))+'};\nstatic const unsigned fullSortFixups[][2]={'+','.join('{%d,%d}'%v for v in fullfix)+'};\n')
objects=[]
for n,p in enumerate(sources+[fixture]):
 o=w/(str(n)+'.obj');r=V.cl(['/nologo','/c','/O2','/MT','/I'+str(w),'/Fo'+str(o),str(p)],env,w);assert not r.returncode,r.stdout+r.stderr;objects.append(o)
proof=check(objects[0],root,0xb849f0,301,9,targets)
for s,a,n,o in specs:
 if o is not None:
  size=32 if o==3 else 4
  assert coff_data(Coff(objects[o]),s,size)==read_virtual_data(raw,a,size)
sortsymbol=specs[3][0]
sortsection,sortoffset=Coff(objects[8]).symbols[sortsymbol]
sorttargets={symbol:(0xb86710 if '__stable_sort@' in symbol else 0xb868c0 if '??$sort@' in symbol else 0xb847d0) for symbol,kind in Coff(objects[8]).sections[sortsection][1].values()}
sortproof=check(objects[8],sortsymbol,0xb84990,82,3,sorttargets)
fullsymbols=[]
for n,spec in enumerate(fullspecs):fullsymbols.append(next(symbol for symbol in Coff(objects[9+n]).symbols if symbol.startswith(spec[1])))
fulltargets={symbol:spec[2] for symbol,spec in zip(fullsymbols,fullspecs)}
fullproofs=[check(objects[9+n],symbol,spec[2],spec[3],len(Coff(objects[9+n]).sections[Coff(objects[9+n]).symbols[symbol][0]][1]),fulltargets) for n,(symbol,spec) in enumerate(zip(fullsymbols,fullspecs))]
link=[str(V.VC/'bin/link.exe'),'/nologo','/out:'+str(w/'fixture.exe'),'kernel32.lib']
for omit in range(21):
 r=subprocess.run(link+[str(o) for n,o in enumerate(objects) if n!=omit],env=env,capture_output=True,text=True);assert r.returncode
r=subprocess.run(link+list(map(str,objects)),env=env,capture_output=True,text=True);assert not r.returncode,r.stdout+r.stderr
r=subprocess.run([str(w/'fixture.exe')],env=env,capture_output=True,text=True,timeout=60);assert not r.returncode,(w,r.returncode,r.stdout,r.stderr);m=re.search(r'cases=(\d+) snapshots=(\d+)',r.stdout);assert m and int(m[1])==30720
report=dict(cases=int(m[1]),snapshots=int(m[2]),providers=[proof,sortproof]+fullproofs+[check(objects[n],specs[i][0],a,l,0,{}) for n,i,a,l in [(4,5,0xb842d0,69),(5,4,0xb84320,100),(6,6,0xb84400,97),(7,7,0xa05840,121)]],omission_checks=21,source_sha256={str(p.relative_to(ROOT)):hashlib.sha256(p.read_bytes()).hexdigest() for p in sources},fixture_sha256=hashlib.sha256(fixture.read_bytes()).hexdigest(),boundary='Actual RenderType plus one/two-pass walkers and DeleteList, two actual globals and alpha table;30720cases with full48KiB/global expectations. Actual capture restore also executes, including replacement-manager stacks; Actual SortList and full default-sort chain execute; stable helper and manager rendering/deleting destructor remain callbacks. Counts0..3 here; full-sort gate covers recursion/heap fallback and dispatcher covers radix threshold. Single-node draw chains here; standalone walker gate tests linked chains and destruction mutations. No rendered frame.')
(ROOT/'rebuild/compile-gate/native-subprimitive-actual-sorted-composed.json').write_text(json.dumps(report,indent=2)+'\n');print(r.stdout.strip());print('NATIVE_SUBPRIMITIVE_ACTUAL_SORTED_COMPOSED_GATE_PASS',report['snapshots'],'snapshots')
