"""Native two-pass world switching with mutable map providers and descriptor lifetime."""
import hashlib,itertools,json,struct,subprocess,tempfile
from pathlib import Path
from capstone import Cs,CS_ARCH_X86,CS_MODE_32
import check_one as V
from variable_allocator_parity import check
from pe_oracle import EXE,read_virtual_data
ROOT=Path(__file__).resolve().parents[2];w=Path(tempfile.mkdtemp(prefix='native-world-switch-'));env=V.env()
sources=[ROOT/'rebuild/src/compiled'/p for p in ('00/b4/CEngineWorldMap_SetWorld_00b42ed0.cpp','00/bd/CEngineLandscapeMap_GameMapNotifications_00bdbfe0.cpp','01/43/NEngineInternal_GPEngineWorldMap_01436e8c.cpp','00/b4/CEngineWorldMap_DeleteAllMaps_00b42c70.cpp','00/b4/CEngineWorldMap_AutoBuildWorldMapGrid_00b40070.cpp','00/b3/CEngineWorldMap_LinkMapToWorld_00b3edf0.cpp')];fixture=ROOT/'rebuild/tests/integration/NativeWorldSwitch_test.cpp'
symbol='?SetWorld@CEngineWorldMap@@QAEXPBVCIDrawWorld@@@Z';loaded_symbol='?OnGameMapLoaded@CEngineLandscapeMap@@QAEXXZ';glob='?GPEngineWorldMap@NEngineInternal@@3PAVCEngineWorldMap@@A'
targets={'?DeleteMap@CEngineWorldMap@@AAEXXZ':0xb42c70,'?resize@?$vector@PAVCEngineMap@@V?$allocator@PAVCEngineMap@@@_STL@@@_STL@@QAEXIABQAVCEngineMap@@@Z':0xb44170,'?AddMap@CEngineWorldMap@@QAEXJJ@Z':0xb42d10,'??1CCharString@@QAE@XZ':0x99eae0,glob:0x1436e8c,loaded_symbol:0xbdbfe0,'?AutoBuildWorldMapGrid@CEngineWorldMap@@AAEXXZ':0xb40070}
delete_targets={'?DeleteMap@CEngineWorldMap@@QAEXJ@Z':0xb42b10,'_memmove':0xbfeae6,'?_M_fill_insert@?$vector@PAVCEngineMap@@V?$allocator@PAVCEngineMap@@@_STL@@@_STL@@QAEXPAPAVCEngineMap@@IABQAV3@@Z':0xb43f80,'??3@YAXPAX@Z':0xbfe9bc}
boundaries=[None,'@Resize@16','@AddMap@16','@DestroyString@4',None,None,None];aliases={'@Entry@12':symbol,'_MapGlobal':glob,'@DeleteAll@4':'?DeleteMap@CEngineWorldMap@@AAEXXZ','?DeleteMap@CEngineWorldMap@@QAEXJ@Z':'@DeleteOne@12',list(delete_targets)[2]:'@Fill@20'};aliases.update({s:b for s,b in zip(targets,boundaries) if b})
(w/'world_switch_aliases.h').write_text('\n'.join('#pragma comment(linker,"/alternatename:'+a+'='+b+'")' for a,b in aliases.items())+'\n')
raw=EXE.read_bytes();body=read_virtual_data(raw,0xb42ed0,276);code=bytearray(1027);code[:276]=body;code[288:289]=read_virtual_data(raw,0xbdbfe0,1);code[304:458]=read_virtual_data(raw,0xb42c70,154);code[512:873]=read_virtual_data(raw,0xb40070,361);code[896:]=read_virtual_data(raw,0xb3edf0,131);fixups=[];md=Cs(CS_ARCH_X86,CS_MODE_32);md.detail=True
for i in md.disasm(body,0xb42ed0):
 if i.mnemonic=='call' and i.operands[0].type==2:
  target=i.operands[0].imm;assert target in targets.values();fixups.append((i.address-0xb42ed0+1,target,1))
 elif i.mnemonic=='mov' and any(o.type==3 and o.mem.disp==0x1436e8c for o in i.operands):fixups.append((i.address-0xb42ed0+i.disp_offset,0x1436e8c,0))
assert len(fixups)==9
for i in md.disasm(code[304:458],0xb42c70):
 if i.mnemonic=='call' and i.operands[0].type==2:
  target=i.operands[0].imm;assert target in delete_targets.values();fixups.append((304+i.address-0xb42c70+1,target,1))
assert len(fixups)==13
for off,target in [(264,0xbfeb22),(335,0xb3edf0)]:
 assert code[512+off-1]==0xe8 and 0xb40070+off+4+struct.unpack_from('<i',code,512+off)[0]==target
 fixups.append((512+off,target,1))
(w/'world_switch_retail.h').write_text('static const unsigned char retailCode[]={'+','.join(map(str,code))+'};\nstatic const unsigned retailFixups[][3]={'+','.join('{%d,%d,%d}'%f for f in fixups)+'};\n')
objects=[]
for i,p in enumerate(sources+[fixture]):
 obj=w/(str(i)+'.obj');r=V.cl(['/nologo','/c','/O2','/MT','/I'+str(w),'/Fo'+str(obj),str(p)],env,w);assert not r.returncode,r.stdout+r.stderr;objects.append(obj)
proofs=[check(objects[0],symbol,0xb42ed0,276,9,targets),check(objects[1],loaded_symbol,0xbdbfe0,1,0,{}),check(objects[3],"?DeleteMap@CEngineWorldMap@@AAEXXZ",0xb42c70,154,4,delete_targets),check(objects[4],"?AutoBuildWorldMapGrid@CEngineWorldMap@@AAEXXZ",0xb40070,361,2,{"??_U@YAPAXI@Z":0xbfeb22,"?LinkMapToWorld@CEngineWorldMap@@AAEXPAVCEngineMap@@J@Z":0xb3edf0}),check(objects[5],"?LinkMapToWorld@CEngineWorldMap@@AAEXPAVCEngineMap@@J@Z",0xb3edf0,131,0,{})]
exe=w/'fixture.exe';cmd=[str(V.VC/'bin/link.exe'),'/nologo','/out:'+str(exe),'kernel32.lib']
for omit in range(6):
 r=subprocess.run(cmd+[str(o) for i,o in enumerate(objects) if i!=omit],env=env,capture_output=True,text=True);assert r.returncode
r=subprocess.run(cmd+list(map(str,[objects[-1]]+objects[:-1])),env=env,capture_output=True,text=True);assert not r.returncode,r.stdout+r.stderr
base=0x24000000;cases=snapshots=0
with (w/'input.bin').open('wb') as f:
 for fill,mode,count,valid,loaded,same,global_owner in itertools.product((0,0x95),range(9),(0,1,2,5),(0,0x1e,0x0a),(0,0x1e,0x14),(0,1),(0,0x200)):
  a=bytearray([fill])*8192
  def put(off,v):struct.pack_into('<I',a,off,v)
  put(0x0c,base+0x1900 if fill else 0);put(0x198,base+0x500 if same else 0);put(0x19c,base+0x600);put(0x39c,base+0x620);put(0x500,base+0x1700);put(0x600,base+0x1720);put(0x620,base+0x1720)
  for off in (0x20,0x220):put(off,base+0x700);put(off+4,base+0x714);put(off+8,base+0x714)
  for i in range(5):put(0x700+4*i,base+0x800+i*0x100);put(0x740+4*i,base+0xd00+i*0x100);put(0x800+i*0x100,base+0x1400);put(0xd00+i*0x100,base+0x1400)
  for i in range(5):
   for off in (0x800,0xd00):struct.pack_into('<4H',a,off+i*0x100+0x5c,32+i,32,i*32,i*32)
  f.write(struct.pack('<5I',mode,count,valid,loaded,global_owner));f.write(a);cases+=1

for kind in ('retail','compiled'):
 r=subprocess.run([str(exe),str(w/'input.bin'),str(w/(kind+'.bin')),kind],env=env,capture_output=True,text=True,timeout=60);assert not r.returncode,(w,r.returncode,r.stdout,r.stderr);print(r.stdout.strip(),kind,flush=True)
# Compare in bounded chunks; no addresses or object bytes are normalized.
with (w/'retail.bin').open('rb') as r,(w/'compiled.bin').open('rb') as c:
 while True:
  x=r.read(1<<20);y=c.read(1<<20);assert x==y,(w,'world switching state/event mismatch')
  if not x:break
# Each destructor appends the complete initialized descriptor in addition to its snapshot.
observed=0
with (w/'compiled.bin').open('rb') as f:
 while True:
  h=f.read(16)
  if not h:break
  assert len(h)==16 and len(f.read(8192))==8192
  if struct.unpack('<4I',h)[0]==4:assert len(f.read(32))==32
  observed+=1
snapshots=observed
assert cases==2592 and snapshots>20000
headers=[ROOT/'rebuild/include'/p for p in ('engine/CEngineWorldMap.h','engine/CEngineMap.h','engine/CIDrawWorldMap.h','fable_world.h','fable_carray.h','fable_stlport_vector.h','fable_stlport_vector_impl.h','candidates/landscape_background_lod_compiled.h')]
report=dict(cases=cases,snapshots=snapshots,omission_checks=6,fixture_template_boundaries="Explicit inline resize/fill specializations linked first; address-taken to retain them. Native comparison verifies events from both boundaries, avoiding implicit template definitions displacing weak aliases.",providers=proofs,source_sha256={str(p.relative_to(ROOT)):hashlib.sha256(p.read_bytes()).hexdigest() for p in sources+headers},fixture_sha256=hashlib.sha256(fixture.read_bytes()).hexdigest(),boundary='Exact276byte world switch plus actual154byte DeleteMap(all),361byte grid builder,131byte linker,retail-empty landscape hook and real singleton. Fifteen audited fixups. Two world calls and standalone cleanup per case;identity no-op;counts0/1/2/5;valid and loaded masks;two singleton owners;callback mutations to current world-map,global owner,array and cached map. Full8KiB state at every event plus full32byte descriptor at string destruction. Deletion callbacks shrink or replace list storage;null/non-null signed-short grid allocation;operator delete can replace owner grid before final zero. Virtual draw interfaces,single-map deletion/creation,vector resize/zero fill,string destruction,allocation and deallocation remain explicit boundaries;grid writes execute actual production code.')
(ROOT/'rebuild/compile-gate/native-world-switch.json').write_text(json.dumps(report,indent=2)+'\n');print('NATIVE_WORLD_SWITCH_GATE_PASS',snapshots,'snapshots')
