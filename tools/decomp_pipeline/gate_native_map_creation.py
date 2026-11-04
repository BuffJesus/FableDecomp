"""Composed world/map/renderer construction against original native execution."""
import hashlib,itertools,json,struct,subprocess,tempfile
from pathlib import Path
from capstone import Cs,CS_ARCH_X86,CS_MODE_32
import check_one as V
from variable_allocator_parity import check
from gate_game_component_abi import Coff
from pe_oracle import EXE,read_virtual_data
ROOT=Path(__file__).resolve().parents[2];w=Path(tempfile.mkdtemp(prefix='native-map-creation-'));env=V.env()
sources=[ROOT/'rebuild/src/compiled'/p for p in ('00/b4/CEngineWorldMap_AddMap_00b42d10.cpp','00/b4/CEngineMap_Constructor_00b429e0.cpp','00/b6/MapRenderers_CreateMap_00b67140.cpp','00/bd/CEngineLandscapeMap_Constructor_00bdd2f0.cpp','00/bd/CLocalDetailCacheMap_Constructors_00bdefc0.cpp','00/bf/CEngineTexturePalette_Constructor_00bf90a0.cpp','00/bd/CEngineLandscapeMap_MovableLifetime_00bdd210.cpp','00/bd/CEngineLandscapeMap_RelocateData_00bdc130.cpp','00/bd/CLocalDetailCacheMap_QuadLifetime_00bddfb0.cpp','00/bd/CLocalDetailCacheMap_CQuadTreeElement_RemoveObjectCollections_00bdda50.cpp','00/be/CEngineStaticMapFileBlockManager_ReleaseRequest_00be0400.cpp','00/be/STLStaticMapBlockTree_find_00be08e0.cpp','01/43/NEngineInternal_MapRenderers_01436ea8.cpp','00/9d/CMovableResource_Verify_009d5100.cpp','00/bd/CLocalDetailCacheMap_QuadRelocation_00bde0c0.cpp','00/bd/CEngineLandscapeMemoryManager_SetVirtualBlockMovableResource_00bd9ab0.cpp','01/43/NEngineInternal_GPEngineDef_01436e20.cpp','01/43/NEngineInternal_GPResourceManager_01436e98.cpp')];fixture=ROOT/'rebuild/tests/integration/NativeMapCreation_test.cpp'
add='?AddMap@CEngineWorldMap@@QAEXJJ@Z';ctor='??0CEngineMap@@QAE@PBVCIDrawMap@@JJJJJVCCharString@@_N@Z'
targets={'?GPLandscapeRenderer@NEngineInternal@@3PAVCEngineLandscapeRenderer@@A':0x1436ea8,'?GPLocalDetailGenerator@NEngineInternal@@3PAVCEngineLocalDetailGenerator@@A':0x1436e88,'??0CCharString@@QAE@ABV0@@Z':0x99ec30,'??1CCharString@@QAE@XZ':0x99eae0,'?OnMapVisibilityChanged@CEngineLandscapeMap@@QAEX_N@Z':0xbdc3c0,'?OnMapVisibilityChanged@CLocalDetailCacheMap@@QAEX_N@Z':0xbde160,'??0CEngineMap@@QAE@PBVCIDrawMap@@JJJJJVCCharString@@_N@Z':0xb429e0,'_malloc':0xbfea0e,'??2@YAPAXI@Z':0xbfea1a,'??3@YAXPAX@Z':0xbfeb1c,'?CreateMap@CEngineLocalDetailGenerator@@QAEXPAVCEngineMap@@@Z':0xb59800,'?CreateMap@CEngineLandscapeRenderer@@QAEXPAVCEngineMap@@J@Z':0xb67140,'?DeleteMap@CEngineWorldMap@@QAEXJ@Z':0xb42b10}
targets.update({'??0CCharString@@QAE@XZ':0x99e4b0,'??4CCharString@@QAEAAV0@ABV0@@Z':0x99efb0,'?resize@?$vector@PAVCEngineMap@@V?$allocator@PAVCEngineMap@@@_STL@@@_STL@@QAEXIABQAVCEngineMap@@@Z':0xb44170})
aliases={'@Add@16':add,'@DefaultText@4':'??0CCharString@@QAE@XZ','@CopyText@12':'??0CCharString@@QAE@ABV0@@Z','@AssignText@12':'??4CCharString@@QAEAAV0@ABV0@@Z','@DestroyText@4':'??1CCharString@@QAE@XZ','@DeleteOne@12':'?DeleteMap@CEngineWorldMap@@QAEXJ@Z','@LandscapeCreate@16':'?CreateMap@CEngineLandscapeRenderer@@QAEXPAVCEngineMap@@J@Z','@DetailCreate@12':'?CreateMap@CEngineLocalDetailGenerator@@QAEXPAVCEngineMap@@@Z','?OnMapVisibilityChanged@CEngineLandscapeMap@@QAEX_N@Z':'@LandscapeVisible@12','?OnMapVisibilityChanged@CLocalDetailCacheMap@@QAEX_N@Z':'@DetailVisible@12'}
landctor='??0CEngineLandscapeMap@@QAE@PBVCEngineMap@@J@Z';detailctor='??0CLocalDetailCacheMap@@QAE@PAVCEngineMap@@@Z';quadctor='??0CQuadTreeElement@CLocalDetailCacheMap@@QAE@PAV1@JJJJ@Z';palettector='??0CEngineTexturePalette@@QAE@PBVCGraphicDataBank@@_NK@Z';lt='??_7CEngineLandscapeMap@@6B@';qt='??_7CQuadTreeElement@CLocalDetailCacheMap@@6B@'
targets.update({landctor:0xbdd2f0,detailctor:0xbdefc0,quadctor:0xbddf30,palettector:0xbf90a0,lt:0x12a7d6c,qt:0x12a7df0,'?GPEngineDef@NEngineInternal@@3PBVCEngineDef@@B':0x1436e20,'?GPResourceManager@NEngineInternal@@3PAVCEngineResourceManager@@A':0x1436e98,'_free':0xbfea14})
aliases.update({'_LandscapeTable':lt,'_QuadTable':qt,'_EngineDef':'?GPEngineDef@NEngineInternal@@3PBVCEngineDef@@B','_Resources':'?GPResourceManager@NEngineInternal@@3PAVCEngineResourceManager@@A','?FreeVirtual@CEngineLandscapeMemoryManager@@QAEXPAX@Z':'@FreeVirtual@12'})
(w/'map_creation_aliases.h').write_text('\n'.join('#pragma comment(linker,"/alternatename:'+a+'='+b+'")' for a,b in aliases.items())+'\n')
code=bytearray(1842);fixups=[];md=Cs(CS_ARCH_X86,CS_MODE_32);md.detail=True;raw=EXE.read_bytes()
for address,size,offset in [(0xb42d10,349,0),(0xb429e0,172,512),(0xb67140,49,704),(0xb59800,50,768),(0xbdd2f0,169,832),(0xbdefc0,66,1024),(0xbddf30,119,1120),(0xbf90a0,171,1280),(0xbf94d0,249,1472),(0xbf8f30,54,1728),(0xbf8f70,50,1792)]:
 body=read_virtual_data(raw,address,size);code[offset:offset+size]=body
 for i in md.disasm(body,address):
  if i.mnemonic=='call' and i.operands[0].type==2:
   assert i.operands[0].imm in set(targets.values())|{0xbf94d0,0xbf8f30,0xbf8f70,0xbfe9bc};fixups.append((offset+i.address-address+1,i.operands[0].imm,1))
  else:
   for op in i.operands:
    if op.type==3 and op.mem.disp in (0x1436ea8,0x1436e88,0x1436e20,0x1436e98):fixups.append((offset+i.address-address+i.disp_offset,op.mem.disp,0))
    elif op.type==2 and op.imm in (0x12a7d6c,0x12a7df0):fixups.append((offset+i.address-address+i.imm_offset,op.imm,0))
assert len(fixups)==43
(w/'map_creation_retail.h').write_text('static const unsigned char retailCode[]={'+','.join(map(str,code))+'};\nstatic const unsigned retailFixups[][3]={'+','.join('{%d,%d,%d}'%f for f in fixups)+'};\n')
objects=[]
for i,p in enumerate(sources+[fixture]+[sources[i] for i in (0,1,5)]):
 o=w/(str(i)+'.obj');r=V.cl(['/nologo','/c','/O2','/MT','/I'+str(w),'/Fo'+str(o)]+(['/Dmalloc=MapAllocate','/Dfree=CreationFree'] if i>=19 else [])+[str(p)],env,w);assert not r.returncode,r.stdout+r.stderr;objects.append(o)
c=Coff(objects[5]);copy=next(s for s in c.symbols if s.startswith('??$__uninitialized_copy'));fill=next(s for s in c.symbols if s.startswith('??$__uninitialized_fill_n'));grow=next(s for s in c.symbols if s.startswith('?_M_insert_overflow'));targets.update({copy:0xbf8f30,fill:0xbf8f70,grow:0xbf94d0})
proofs=[check(objects[i],sym,a,n,num,targets) for i,sym,a,n,num in [(0,add,0xb42d10,349,14),(1,ctor,0xb429e0,172,4),(2,'?CreateMap@CEngineLandscapeRenderer@@QAEXPAVCEngineMap@@J@Z',0xb67140,49,2),(2,'?CreateMap@CEngineLocalDetailGenerator@@QAEXPAVCEngineMap@@@Z',0xb59800,50,2),(3,landctor,0xbdd2f0,169,5),(4,detailctor,0xbdefc0,66,1),(4,quadctor,0xbddf30,119,1),(5,palettector,0xbf90a0,171,5),(5,grow,0xbf94d0,249,7),(5,copy,0xbf8f30,54,1),(5,fill,0xbf8f70,50,1)]]
for idx,renamed,symbols in [(0,19,(add,)),(1,20,(ctor,)),(5,21,(palettector,grow,copy,fill))]:
 c,d=Coff(objects[idx]),Coff(objects[renamed])
 for symbol in symbols:
  sec,off=c.symbols[symbol];sec1,off1=d.symbols[symbol];body,rel=c.sections[sec];body1,rel1=d.sections[sec1];assert off==off1 and body==body1 and {p:({'_malloc':'_MapAllocate','_free':'_CreationFree'}.get(t,t),k) for p,(t,k) in rel.items()}==rel1
slot_targets={'??_ECEngineLandscapeMap@@UAEPAXI@Z':0xbdd640,'?RelocateData@CEngineLandscapeMap@@EAEXJPAX0J@Z':0xbdc130,'?CanRelocateData@CEngineLandscapeMap@@EBE_NJ@Z':0xbdc140,'??_ECQuadTreeElement@CLocalDetailCacheMap@@UAEPAXI@Z':0xbdfa10,'?RelocateData@CQuadTreeElement@CLocalDetailCacheMap@@UAEXJPAX0J@Z':0xbde0c0,'?CanRelocateData@CQuadTreeElement@CLocalDetailCacheMap@@UBE_NJ@Z':0xbddd10,'?Verify@CMovableResource@@UBEKXZ':0x9d5100}
for idx,symbol,a in [(3,lt,0x12a7d6c),(4,qt,0x12a7df0)]:
 c=Coff(objects[idx]);sec,off=c.symbols[symbol];body,rel=c.sections[sec];assert off==0 and body==bytes(16) and set(rel)=={0,4,8,12};patched=bytearray(body)
 for at,(target,kind) in rel.items():assert kind==6;struct.pack_into('<I',patched,at,slot_targets[target])
 assert patched==read_virtual_data(raw,a,16)
cmd=[str(V.VC/'bin/link.exe'),'/nologo','/out:'+str(w/'fixture.exe'),'kernel32.lib'];linked=[objects[18]]+[o for i,o in enumerate(objects) if i not in (0,1,5,18)]
for omit in range(1,len(linked)):
 r=subprocess.run(cmd+[str(o) for i,o in enumerate(linked) if i!=omit],env=env,capture_output=True,text=True);assert r.returncode,('omission',omit)
r=subprocess.run(cmd+list(map(str,linked)),env=env,capture_output=True,text=True);assert not r.returncode,r.stdout+r.stderr
cases=0;base=0x24000000
with (w/'input.bin').open('wb') as f:
 for fill,mode,id,existing,flags,size,children in itertools.product((0,0x95),range(7),(0,3),(0,1,2),(0,1,2,0xffffffff),(0,4),range(4)):
  a=bytearray([fill])*8192
  def put(off,v):struct.pack_into('<I',a,off,v&0xffffffff)
  put(0xc,base+0x500 if fill else 0);put(0x20,base+0x700);put(0x24,base+0x700+size*4);put(0x28,base+0x710);put(0x2c,base+0x900);put(0x19c,base+0x620);put(0x620,base+0x600)
  for j in range(4):put(0x700+j*4,base+0xb00 if j==id and existing and size else 0)
  for h in (0x900,0x920):put(h,base+h);put(h+4,base+h)
  for m in (0xb00,0x1100):put(m,0);put(m+4,0);put(m+0x18,0x1234 if existing==1 or m==0x1100 else 0x4321);put(m+0x58,base+0x900);a[m+0x64]=0
  a[0x1800:0x1820]=bytes(32);a[0x1800]=1;put(0x1804,base+0x1c00);put(0x1808,0x99887766);put(0x180c,65535);put(0x1810,0xffffffff);put(0x1814,0x1234);put(0x1818,33);put(0x181c,65536)
  f.write(struct.pack('<4I',mode,id,flags,children));f.write(a);cases+=1
for kind in ('retail','compiled'):
 r=subprocess.run([str(w/'fixture.exe'),str(w/'input.bin'),str(w/(kind+'.bin')),kind],env=env,capture_output=True,text=True,timeout=60);assert not r.returncode,(w,r.returncode,r.stdout,r.stderr);print(r.stdout.strip(),kind,flush=True)
with (w/'retail.bin').open('rb') as e,(w/'compiled.bin').open('rb') as f:
 while True:
  x=e.read(1<<20);y=f.read(1<<20);assert x==y,(w,'constructor native mismatch')
  if not x:break
size=(w/'compiled.bin').stat().st_size;assert size%8212==0;snapshots=size//8212
headers=[ROOT/'rebuild/include'/p for p in ('engine/CEngineMap.h','engine/CEngineStaticMapFileBlockManager.h','fable_2d_vector.h','fable_stlport_tree.h','fable_engine_map_list.h','engine/CEngineWorldMap.h','engine/CEngineLandscapeRenderer.h','engine/CEngineLocalDetailGenerator.h')]
report=dict(cases=cases,snapshots=snapshots,omission_checks=len(linked)-1,providers=proofs,source_sha256={str(p.relative_to(ROOT)):hashlib.sha256(p.read_bytes()).hexdigest() for p in sources+headers},fixture_sha256=hashlib.sha256(fixture.read_bytes()).hexdigest(),boundary='Eleven original spans with43audited fixups:world AddMap,map constructor,both renderer creators,landscape/detail/quad constructors,texture-palette constructor and real vector grow/copy/fill. Actual definition/resource/renderer globals and audited complete landscape/quad vtables. Full8KiB snapshots through two AddMap calls. Existing UID paths,vector growth,flags,child allocation failures;callback vector/list replacement and map-dimension mutation during palette allocation. Draw info,DeleteMap,vector resize,string lifecycle,allocation and visibility remain boundaries. Six emitted bodies have malloc/free symbol renaming audited;all18native sources mandatory to link.')
(ROOT/'rebuild/compile-gate/native-map-creation.json').write_text(json.dumps(report,indent=2)+'\n');print('NATIVE_MAP_CREATION_GATE_PASS',snapshots,'snapshots')
