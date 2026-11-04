"""Actual grouped-list removal composed through scene update and manager routing."""
import hashlib,itertools,json,struct,subprocess,tempfile
from pathlib import Path
from capstone import Cs,CS_ARCH_X86,CS_MODE_32
import check_one as V
from variable_allocator_parity import check
from gate_game_component_abi import Coff
from gate_render_state_static_data import coff_data
from pe_oracle import EXE,read_virtual_data
ROOT=Path(__file__).resolve().parents[2];w=Path(tempfile.mkdtemp(prefix='native-scene-primitive-lists-'));env=V.env()
sources=[ROOT/'rebuild/src/compiled'/p for p in ('00/b8/CEngineInternalPrimitiveBase_Lists_00b8fdf0.cpp','00/b9/CEngineScene_UpdatePosition_00b91840.cpp','00/ba/CEnginePrimitiveManager_AddPrimitive_00bacdd0.cpp','01/43/NEngineInternal_GPPrimitiveTypeInfo_01436e84.cpp')];fixture=ROOT/'rebuild/tests/integration/NativeScenePrimitiveLists_test.cpp'
add='?AddToList@CEngineInternalPrimitiveBase@@QAEXPAPAV1@@Z';remove='?RemoveFromList@CEngineInternalPrimitiveBase@@QAEXXZ';update='?UpdatePosition@CEngineScene@@SIXAAVCEngineInternalPrimitiveBase@@K@Z';manager='?AddPrimitive@CEnginePrimitiveManager@@QAEXAAVCEngineInternalPrimitiveBase@@K@Z';scene='?AddPrimitive@CEngineScene@@QAEXAAVCEngineInternalPrimitiveBase@@K@Z';globalname='?GPPrimitiveTypeInfo@NEngineInternal@@3PAVCEnginePrimitiveTypeInfo@@A'
targets={remove:0xb8fe60,manager:0xbacdd0,scene:0xb94d30,globalname:0x1436e84};aliases={'@Add@12':add,'@Update@8':update,scene:'@SceneAdd@16','_TypeInfoGlobal':globalname}
(w/'scene_primitive_lists_aliases.h').write_text('\n'.join('#pragma comment(linker,"/alternatename:'+a+'='+b+'")' for a,b in aliases.items())+'\n')
raw=EXE.read_bytes();body=bytearray(304);fixups=[];md=Cs(CS_ARCH_X86,CS_MODE_32);md.detail=True
specs=[(0,add,0xb8fdf0,106,0,0),(0,remove,0xb8fe60,116,0,112),(1,update,0xb91840,39,3,240),(2,manager,0xbacdd0,12,1,288)]
for n,sym,address,length,count,offset in specs:
 code=read_virtual_data(raw,address,length);body[offset:offset+length]=code
 for i in md.disasm(code,address):
  if i.mnemonic in ('call','jmp') and i.operands[0].type==2 and i.operands[0].imm in targets.values():fixups.append((offset+i.address-address+i.imm_offset,i.operands[0].imm,1))
  else:
   for o in i.operands:
    if o.type==3 and o.mem.disp==0x1436e84:fixups.append((offset+i.address-address+i.disp_offset,o.mem.disp,0))
assert len(fixups)==4
(w/'scene_primitive_lists_retail.h').write_text('static const unsigned char retailCode[]={'+','.join(map(str,body))+'};\nstatic const unsigned retailFixups[][3]={'+','.join('{%d,%d,%d}'%f for f in fixups)+'};\n')
objects=[]
for n,p in enumerate(sources+[fixture]):
 o=w/(str(n)+'.obj');r=V.cl(['/nologo','/c','/O2','/MT','/I'+str(w),'/Fo'+str(o),str(p)],env,w);assert not r.returncode,r.stdout+r.stderr;objects.append(o)
proofs=[check(objects[n],s,a,l,c,targets) for n,s,a,l,c,off in specs];assert coff_data(Coff(objects[3]),globalname,4)==bytes(4)
cmd=[str(V.VC/'bin/link.exe'),'/nologo','/out:'+str(w/'fixture.exe'),'kernel32.lib']
for omit in range(4):
 r=subprocess.run(cmd+[str(o) for i,o in enumerate(objects) if i!=omit],env=env,capture_output=True,text=True);assert r.returncode
r=subprocess.run(cmd+list(map(str,objects)),env=env,capture_output=True,text=True);assert not r.returncode,r.stdout+r.stderr
cases=snapshots=0
with (w/'input.bin').open('wb') as f:
 for base,counts,mask,flags,cell,route in itertools.product((0,0x800),itertools.product(range(3),repeat=3),(0,1,0x80000000,0xfedcba98),(0,1,2,3,255),range(2),(0,0x40,0x80000000,0xffffffff)):
  for selected in range(sum(counts)+1):
   local=(0,1,0x7fffffff)[(sum(counts)+selected)%3];f.write(struct.pack('<12I',base,*counts,mask,selected,flags,cell,local,0xabcdef01,route,(cases*37)%47));cases+=1;snapshots+=2 if route&0x40 else 3
for kind in ('retail','compiled'):
 r=subprocess.run([str(w/'fixture.exe'),str(w/'input.bin'),str(w/(kind+'.bin')),kind],env=env,capture_output=True,text=True,timeout=120);assert not r.returncode,(w,r.returncode,r.stdout,r.stderr);print(r.stdout.strip(),kind,flush=True)
with (w/'retail.bin').open('rb') as f,(w/'compiled.bin').open('rb') as g:
 while True:
  a=f.read(1<<20);b=g.read(1<<20);assert a==b,w
  if not a:break
assert (w/'compiled.bin').stat().st_size==snapshots*8196
report=dict(cases=cases,snapshots=snapshots,omission_checks=4,providers=proofs,source_sha256={str(p.relative_to(ROOT)):hashlib.sha256(p.read_bytes()).hexdigest() for p in sources},fixture_sha256=hashlib.sha256(fixture.read_bytes()).hexdigest(),boundary='Actual list insertion/removal,Scene::UpdatePosition,manager first-scene routing and TypeInfo global. Four mandatory sources/four native spans/four fixups. Zero-six existing nodes/three layergroups,matching/new mask,all selected nodes,two placements,five nodeflagbytes,optionalcell,count0/1/MAX_INT,four updateflags,rotatingall47types. Independent whole-graph reconstruction checked after insertion,at final Scene::AddPrimitive callback,and return. Skip flag preserves list/cell. Callback verifies routedscene/originalflags and may clearglobal. Full8KiB-plus-global snapshots;only final scene insertion is a callback.')
(ROOT/'rebuild/compile-gate/native-scene-primitive-lists.json').write_text(json.dumps(report,indent=2)+'\n');print('NATIVE_SCENE_PRIMITIVE_LISTS_GATE_PASS',snapshots,'snapshots')
