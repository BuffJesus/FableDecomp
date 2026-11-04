"""Scene reinsert routing through real primitive-manager scene-array reference."""
import hashlib,itertools,json,struct,subprocess,tempfile
from pathlib import Path
from capstone import Cs,CS_ARCH_X86,CS_MODE_32
import check_one as V
from variable_allocator_parity import check
from gate_game_component_abi import Coff
from gate_render_state_static_data import coff_data
from pe_oracle import EXE,read_virtual_data
ROOT=Path(__file__).resolve().parents[2];w=Path(tempfile.mkdtemp(prefix='native-scene-primitive-update-'));env=V.env()
sources=[ROOT/'rebuild/src/compiled'/p for p in ('00/b9/CEngineScene_UpdatePosition_00b91840.cpp','00/ba/CEnginePrimitiveManager_AddPrimitive_00bacdd0.cpp','01/43/NEngineInternal_GPPrimitiveTypeInfo_01436e84.cpp')];fixture=ROOT/'rebuild/tests/integration/NativeScenePrimitiveUpdate_test.cpp'
update='?UpdatePosition@CEngineScene@@SIXAAVCEngineInternalPrimitiveBase@@K@Z';remove='?RemoveFromList@CEngineInternalPrimitiveBase@@QAEXXZ';manager='?AddPrimitive@CEnginePrimitiveManager@@QAEXAAVCEngineInternalPrimitiveBase@@K@Z';scene='?AddPrimitive@CEngineScene@@QAEXAAVCEngineInternalPrimitiveBase@@K@Z';globalname='?GPPrimitiveTypeInfo@NEngineInternal@@3PAVCEnginePrimitiveTypeInfo@@A'
targets={remove:0xb8fe60,manager:0xbacdd0,scene:0xb94d30,globalname:0x1436e84};aliases={'@Update@8':update,remove:'@Remove@8',scene:'@SceneAdd@16','_TypeInfoGlobal':globalname}
(w/'scene_primitive_update_aliases.h').write_text('\n'.join('#pragma comment(linker,"/alternatename:'+a+'='+b+'")' for a,b in aliases.items())+'\n')
raw=EXE.read_bytes();body=bytearray(64);fixups=[];md=Cs(CS_ARCH_X86,CS_MODE_32);md.detail=True
for address,length,offset in ((0xb91840,39,0),(0xbacdd0,12,48)):
 code=read_virtual_data(raw,address,length);body[offset:offset+length]=code
 for i in md.disasm(code,address):
  if i.mnemonic in ('call','jmp') and i.operands[0].type==2:fixups.append((offset+i.address-address+i.imm_offset,i.operands[0].imm,1))
  else:
   for o in i.operands:
    if o.type==3 and o.mem.disp==0x1436e84:fixups.append((offset+i.address-address+i.disp_offset,o.mem.disp,0))
assert len(fixups)==4
(w/'scene_primitive_update_retail.h').write_text('static const unsigned char retailCode[]={'+','.join(map(str,body))+'};\nstatic const unsigned retailFixups[][3]={'+','.join('{%d,%d,%d}'%f for f in fixups)+'};\n')
objects=[]
for n,p in enumerate(sources+[fixture]):
 o=w/(str(n)+'.obj');r=V.cl(['/nologo','/c','/O2','/MT','/I'+str(w),'/Fo'+str(o),str(p)],env,w);assert not r.returncode,r.stdout+r.stderr;objects.append(o)
proofs=[check(objects[0],update,0xb91840,39,3,targets),check(objects[1],manager,0xbacdd0,12,1,targets)];assert coff_data(Coff(objects[2]),globalname,4)==bytes(4)
cmd=[str(V.VC/'bin/link.exe'),'/nologo','/out:'+str(w/'fixture.exe'),'kernel32.lib']
for omit in range(3):
 r=subprocess.run(cmd+[str(o) for i,o in enumerate(objects) if i!=omit],env=env,capture_output=True,text=True);assert r.returncode
r=subprocess.run(cmd+list(map(str,objects)),env=env,capture_output=True,text=True);assert not r.returncode,r.stdout+r.stderr
cases=0
with (w/'input.bin').open('wb') as f:
 for base,kind,low,mode in itertools.product((0,0x100),range(47),range(256),range(5)):
  flags=low|((kind*0x01010100)&0xffffff00);f.write(struct.pack('<4I',base,kind,flags,mode));cases+=1
for kind in ('retail','compiled'):
 r=subprocess.run([str(w/'fixture.exe'),str(w/'input.bin'),str(w/(kind+'.bin')),kind],env=env,capture_output=True,text=True,timeout=120);assert not r.returncode,(w,r.returncode,r.stdout,r.stderr);print(r.stdout.strip(),kind,flush=True)
with (w/'retail.bin').open('rb') as f,(w/'compiled.bin').open('rb') as g:
 while True:
  a=f.read(1<<20);b=g.read(1<<20);assert a==b,w
  if not a:break
snapshots=cases*2;assert (w/'compiled.bin').stat().st_size==snapshots*4100
report=dict(cases=cases,snapshots=snapshots,omission_checks=3,providers=proofs,source_sha256={str(p.relative_to(ROOT)):hashlib.sha256(p.read_bytes()).hexdigest() for p in sources},fixture_sha256=hashlib.sha256(fixture.read_bytes()).hexdigest(),boundary='Actual Scene::UpdatePosition and PrimitiveManager::AddPrimitive with zero-initialized actual GPPrimitiveTypeInfo. Three mandatory sources/two native spans/four fixups.47types,all256lowflagbytes,highflags,two placements,fivemutations. Flag0x40 skips both calls;Remove callback can change Type/global/primitive flags or install initiallynull global;subsequent manager lookup rereads type/global but forwardsoriginalflags. Real scene-array-reference traversal chooses firstscene. Full4KiB-plus-global expected snapshots. RemoveFromList and Scene::AddPrimitive remain callback boundaries;valid manager typeindices only.')
(ROOT/'rebuild/compile-gate/native-scene-primitive-update.json').write_text(json.dumps(report,indent=2)+'\n');print('NATIVE_SCENE_PRIMITIVE_UPDATE_GATE_PASS',snapshots,'snapshots')
