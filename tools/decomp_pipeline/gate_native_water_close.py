"""Water close with actual index COM release, static discard and world parent."""
import hashlib,itertools,json,struct,subprocess,tempfile
from pathlib import Path
from capstone import Cs,CS_ARCH_X86,CS_MODE_32
import check_one as V
from variable_allocator_parity import check
from pe_oracle import EXE,read_virtual_data
ROOT=Path(__file__).resolve().parents[2];w=Path(tempfile.mkdtemp(prefix='native-water-close-'));env=V.env()
paths=('00/b6/CEngineWaterRenderer_CloseStaticMap_00b6db80.cpp','00/ac/CIndexBuffer_Uninitialise_00ac1ee0.cpp','00/9d/CBankFileAsyncData_DiscardAsyncData_009d68a0.cpp','00/b4/CEngineWorldMap_CloseStaticMapFile_00b40000.cpp','01/43/NEngineInternal_GPWaterRenderer_01436e54.cpp','00/a6/CVertexBufferWin32_Uninitialise_00a63200.cpp','00/a0/CRenderManagerCore_WaitWhileResourceIsBusy_00a0aa70.cpp')
sources=[ROOT/'rebuild/src/compiled'/p for p in paths];fixture=ROOT/'rebuild/tests/integration/NativeWaterClose_test.cpp'
water='?CloseStaticMap@CEngineWaterRenderer@@QAEXXZ';index='?Uninitialise@CIndexBuffer@@QAEXXZ';vertex='?Uninitialise@CVertexBufferWin32@@QAEXXZ';static='?DiscardAsyncData@CBankFileAsyncData@@SIXAAV?$CCountedPointer@VCBankFileAsyncData@@@@@Z';bank='?DiscardAsyncData@CBankFileAsync@@QAEXAAV?$CCountedPointer@VCBankFileAsyncData@@@@@Z';world='?CloseStaticMapFile@CEngineWorldMap@@QAEXXZ';mapclose='?CloseStaticMap@CEngineMap@@QAEXXZ';glob='?GPWaterRenderer@NEngineInternal@@3PAVCEngineWaterRenderer@@A'
system='?GFGetSystemManager@@YIPAVCSystemManager@@XZ';render='?WaitWhileResourceIsBusy@CRenderManagerCore@@QAEXPAUIDirect3DResource9@@M@Z'
aliases={'@Entry@4':water,'@World@4':world,'@Wait@16':render,'_WaterGlobal':glob,system:'@System@0',bank:'@Discard@12',mapclose:'@MapClose@4'}
(w/'water_close_aliases.h').write_text('\n'.join('#pragma comment(linker,"/alternatename:'+a+'='+b+'")' for a,b in aliases.items())+'\n')
targets={vertex:0xa63200,index:0xac1ee0,static:0x9d68a0,'??3@YAXPAX@Z':0xbfe9bc,bank:0x9d67e0,mapclose:0xb3ef40,water:0xb6db80,glob:0x1436e54}
targets.update({system:0x9a4ec0,render:0xa0aa70})
raw=EXE.read_bytes();code=bytearray(451);fixups=[];md=Cs(CS_ARCH_X86,CS_MODE_32);md.detail=True
for address,n,offset in ((0xb6db80,178,0),(0xac1ee0,25,192),(0x9d68a0,16,224),(0xb40000,108,256),(0xa63200,53,384),(0xa0aa70,3,448)):
 body=read_virtual_data(raw,address,n);code[offset:offset+n]=body
 for i in md.disasm(body,address):
  if i.mnemonic=='call' and i.operands[0].type==2:assert i.operands[0].imm in targets.values();fixups.append((offset+i.address-address+1,i.operands[0].imm))
  for op in i.operands:
   if op.type==3 and op.mem.base==0 and op.mem.index==0 and op.mem.disp>=0x400000:assert op.mem.disp==0x1436e54;fixups.append((offset+i.address-address+i.disp_offset,op.mem.disp))
assert len(fixups)==11
(w/'water_close_retail.h').write_text('static const unsigned char retailCode[]={'+','.join(str(b) for b in code)+'};\nstatic const unsigned retailFixups[][2]={'+','.join('{%d,%d}'%f for f in fixups)+'};\n')
objects=[]
for i,p in enumerate(sources+[fixture]):
 obj=w/(str(i)+'.obj');r=V.cl(['/nologo','/c','/O2','/MT','/I'+str(w),'/Fo'+str(obj),str(p)],env,w);assert not r.returncode,r.stdout+r.stderr;objects.append(obj)
proofs=[check(objects[i],s,a,n,count,targets) for i,s,a,n,count in ((0,water,0xb6db80,178,5),(1,index,0xac1ee0,25,0),(2,static,0x9d68a0,16,1),(3,world,0xb40000,108,3))]
proofs.append(check(objects[5],vertex,0xa63200,53,2,targets))
proofs.append(check(objects[6],render,0xa0aa70,3,0,targets))
exe=w/'fixture.exe';cmd=[str(V.VC/'bin/link.exe'),'/nologo','/out:'+str(exe),'kernel32.lib']
for omit in range(7):
 r=subprocess.run(cmd+[str(o) for i,o in enumerate(objects) if i!=omit],env=env,capture_output=True,text=True);assert r.returncode
r=subprocess.run(cmd+list(map(str,objects)),env=env,capture_output=True,text=True);assert not r.returncode,r.stdout+r.stderr
inputs=bytearray();cases=0;base=0x24000000;owner=0x400
for fill,mode,sea,index_present,vertex_present,loading,datarefs,entryrefs,kind in itertools.product((0,0x55,0xff),range(7),range(2),range(2),range(2),range(2),(None,0,1,2,0xffffffff),(None,0,1,2,0xffffffff),range(2)):
 arena=bytearray([fill])*8192
 def put(off,value):struct.pack_into('<I',arena,off,value)
 put(0x20,base+0x200);put(0x24,base+0x204);put(0x28,base+0x204);put(0x200,0x1badb002);put(0x34,0x25000010);put(0x1a8,1)
 arena[owner+0x276]=sea;put(owner+0x6c,base+0x1000 if index_present else 0);put(0x1000,0x25000000)
 put(owner+0x80,base+0x1100 if vertex_present else 0);put(0x1100,0x25000000);put(0x1080,0x25000000)
 put(0x1c60,base+0x1d00);put(0x1d08,base+0x1e00)
 put(owner+0x5b8,base+0x1200 if loading else 0);put(owner+0x5bc,base+0x1800 if datarefs is not None else 0);put(0x1228,base+0x1400)
 put(owner+0x5b0,base+0x1300);put(owner+0x5b4,base+0x1840 if entryrefs is not None else 0)
 for off,refs,payload in ((0x1800,datarefs,0x1200),(0x1840,entryrefs,0x1300),(0x1880,1,0x1200),(0x18c0,1,0x1300)):put(off,refs or 0);put(off+8,base+payload)
 inputs.extend(struct.pack('<2I',mode,kind));inputs.extend(arena);cases+=1
(w/'input.bin').write_bytes(inputs)
for kind in ('retail','compiled'):
 r=subprocess.run([str(exe),str(w/'input.bin'),str(w/(kind+'.bin')),kind],env=env,capture_output=True,text=True,timeout=60);assert not r.returncode,(w,r.returncode,r.stdout,r.stderr);print(r.stdout.strip(),kind,flush=True)
actual=(w/'compiled.bin').read_bytes();expected=(w/'retail.bin').read_bytes();assert actual==expected,(w,'water shutdown ownership');assert len(actual)%(8192+20)==0
snapshots=len(actual)//(8192+20)
headers=[ROOT/'rebuild/include'/p for p in ('engine/CEngineWaterRenderer.h','engine/CIndexBuffer.h','engine/CEngineWorldMap.h','fable_game_text_bank.h','fable_counted_pointer.h','fable_font_vertex_buffer.h','fable_system_manager_state.h','fable_display_viewport.h','fable_render_state_manager.h')]
report=dict(cases=cases,snapshots=snapshots,omission_checks=7,providers=proofs,source_sha256={str(p.relative_to(ROOT)):hashlib.sha256(p.read_bytes()).hexdigest() for p in sources+headers},fixture_sha256=hashlib.sha256(fixture.read_bytes()).hexdigest(),boundary='Six original spans,eleven audited calls/global operand;actual water,index/vertex COM dispatch,retail-empty WaitWhileResourceIsBusy,static discard and world close compose. Direct/world two-call sequences,three fills,sea/index/vertex/loading branches,both control counts null/0/1/2/ffffffff. System,COM,bank,DeleteFunc/free callbacks alter upcoming loading/control/bank fields. Wait additionally called with varied resource/float bits,including exceptional values. Full8KiB and singleton snapshots. System accessor,COM Release,bank discard,DeleteFunc/free and virtual bank close remain recorded boundaries,no normalization.')
(ROOT/'rebuild/compile-gate/native-water-close.json').write_text(json.dumps(report,indent=2)+'\n');print('NATIVE_WATER_CLOSE_GATE_PASS',snapshots,'snapshots')
