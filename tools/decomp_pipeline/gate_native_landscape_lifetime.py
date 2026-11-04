"""Native landscape lifetime, ownership cleanup and corrected movable-resource ABI."""
import hashlib,itertools,json,struct,subprocess,tempfile
from pathlib import Path
from capstone import Cs,CS_ARCH_X86,CS_MODE_32
import check_one as V
from variable_allocator_parity import check
from gate_game_component_abi import Coff
from pe_oracle import EXE,read_virtual_data
ROOT=Path(__file__).resolve().parents[2];w=Path(tempfile.mkdtemp(prefix='native-landscape-lifetime-'));env=V.env()
sources=[ROOT/'rebuild/src/compiled'/p for p in ('00/bd/CEngineLandscapeMap_Constructor_00bdd2f0.cpp','00/bd/CEngineLandscapeMap_MovableLifetime_00bdd210.cpp','00/bd/CEngineLandscapeMap_RelocateData_00bdc130.cpp','00/bd/CLocalDetailCacheMap_QuadLifetime_00bddfb0.cpp','00/bd/CLocalDetailCacheMap_CQuadTreeElement_RemoveObjectCollections_00bdda50.cpp','00/be/CEngineStaticMapFileBlockManager_ReleaseRequest_00be0400.cpp','00/be/STLStaticMapBlockTree_find_00be08e0.cpp','01/43/NEngineInternal_MapRenderers_01436ea8.cpp','00/9d/CMovableResource_Verify_009d5100.cpp','00/bd/CLocalDetailCacheMap_QuadRelocation_00bde0c0.cpp','00/bd/CEngineLandscapeMemoryManager_SetVirtualBlockMovableResource_00bd9ab0.cpp','01/43/NEngineInternal_GPEngineDef_01436e20.cpp','01/43/NEngineInternal_GPResourceManager_01436e98.cpp')];fixture=ROOT/'rebuild/tests/integration/NativeLandscapeLifetime_test.cpp'
ctor='??0CEngineLandscapeMap@@QAE@PBVCEngineMap@@J@Z';dtor='??1CEngineLandscapeMap@@UAE@XZ';delete='??_GCEngineLandscapeMap@@UAEPAXI@Z';can='?CanRelocateData@CEngineLandscapeMap@@EBE_NJ@Z';move='?RelocateData@CEngineLandscapeMap@@EAEXJPAX0J@Z';table='??_7CEngineLandscapeMap@@6B@';base='??_7CMovableResource@@6B@';release='?ReleaseRequest@CEngineStaticMapFileBlockManager@@QAEXJ@Z';edef='?GPEngineDef@NEngineInternal@@3PBVCEngineDef@@B';resource='?GPResourceManager@NEngineInternal@@3PAVCEngineResourceManager@@A';palette='??0CEngineTexturePalette@@QAE@PBVCGraphicDataBank@@_NK@Z'
aliases={'@Construct@16':ctor,'@Destroy@4':dtor,'@DeleteMap@12':delete,'@CanRelocate@12':can,'@Relocate@24':move,'_LandscapeTable':table,'_BaseTable':base,'_EngineDef':edef,'_Resources':resource,palette:'@Palette@20','??1CCharString@@QAE@XZ':'@DestroyString@4','?FreeVirtual@CEngineLandscapeMemoryManager@@QAEXPAX@Z':'@FreeVirtual@12'}
(w/'landscape_lifetime_aliases.h').write_text('\n'.join('#pragma comment(linker,"/alternatename:'+a+'='+b+'")' for a,b in aliases.items())+'\n')
spans=[(0xbdd2f0,169,0),(0xbdd210,218,192),(0xbdd640,30,448),(0xbdc140,13,480),(0xbdc130,3,512),(0xbe0400,29,544),(0xbe08e0,71,576)]
raw=EXE.read_bytes();code=bytearray(647);fixups=[];md=Cs(CS_ARCH_X86,CS_MODE_32);md.detail=True;allowed={a for a,_,_ in spans}|{0xbf90a0,0xbfe9bc,0xbfeb1c,0xbfea14,0x99eae0}
for address,size,offset in spans:
 body=read_virtual_data(raw,address,size);code[offset:offset+size]=body
 for i in md.disasm(body,address):
  if i.mnemonic=='call' and i.operands[0].type==2:
   target=i.operands[0].imm;assert target in allowed;fixups.append((offset+i.address-address+1,target,1))
  else:
   for op in i.operands:
    if op.type==3 and op.mem.disp in (0x1436e20,0x1436e98):fixups.append((offset+i.address-address+i.disp_offset,op.mem.disp,0))
    elif op.type==2 and op.imm in (0x12a7d6c,0x129b860):fixups.append((offset+i.address-address+i.imm_offset,op.imm,0))
assert len(fixups)==16
(w/'landscape_lifetime_retail.h').write_text('static const unsigned char retailCode[]={'+','.join(map(str,code))+'};\nstatic const unsigned retailFixups[][3]={'+','.join('{%d,%d,%d}'%f for f in fixups)+'};\n')
objects=[]
for i,p in enumerate(sources+[fixture,sources[1]]):
 o=w/(str(i)+'.obj');r=V.cl(['/nologo','/c','/O2','/MT','/I'+str(w),'/Fo'+str(o)]+(['/Dfree=LandFree'] if i==14 else [])+[str(p)],env,w);assert not r.returncode,r.stdout+r.stderr;objects.append(o)
find=next(s for s in Coff(objects[6]).symbols if s.startswith('?find@?$_Rb_tree@J'))
targets={table:0x12a7d6c,base:0x129b860,release:0xbe0400,edef:0x1436e20,resource:0x1436e98,palette:0xbf90a0,'??3@YAXPAX@Z':0xbfe9bc,'_free':0xbfea14,'??1CCharString@@QAE@XZ':0x99eae0,dtor:0xbdd210,find:0xbe08e0}
proofs=[check(objects[i],sym,a,n,num,targets) for i,sym,a,n,num in [(0,ctor,0xbdd2f0,169,5),(1,dtor,0xbdd210,218,8),(1,delete,0xbdd640,30,2),(1,can,0xbdc140,13,0),(2,move,0xbdc130,3,0),(5,release,0xbe0400,29,1),(6,find,0xbe08e0,71,0)]]
c,d=Coff(objects[1]),Coff(objects[14]);sec,off=c.symbols[dtor];sec1,off1=d.symbols[dtor];body,rel=c.sections[sec];body1,rel1=d.sections[sec1];assert off==off1 and body==body1 and {p:('_LandFree' if t=='_free' else t,k) for p,(t,k) in rel.items()}==rel1
slots={delete.replace('??_G','??_E'):0xbdd640,move:0xbdc130,can:0xbdc140,'?Verify@CMovableResource@@UBEKXZ':0x9d5100}
for idx in (0,1):
 c=Coff(objects[idx]);sec,off=c.symbols[table];body,rel=c.sections[sec];assert off==0 and body==bytes(16) and set(rel)=={0,4,8,12};patched=bytearray(body)
 for at,(target,kind) in rel.items():assert kind==6;struct.pack_into('<I',patched,at,slots[target])
 assert patched==read_virtual_data(raw,0x12a7d6c,16)
cmd=[str(V.VC/'bin/link.exe'),'/nologo','/out:'+str(w/'fixture.exe'),'kernel32.lib'];linked=[o for i,o in enumerate(objects) if i!=1]
for omit in range(len(linked)):
 if omit==12:continue # fixture
 r=subprocess.run(cmd+[str(o) for i,o in enumerate(linked) if i!=omit],env=env,capture_output=True,text=True);assert r.returncode,('omission',omit)
r=subprocess.run(cmd+list(map(str,linked)),env=env,capture_output=True,text=True);assert not r.returncode,r.stdout+r.stderr
cases=0;baseaddr=0x24000000
refs=[(None,None),(0,0),(1,1),(2,2),(0xffffffff,1),(1,0xffffffff)]
shapes=[(None,0,0,0),(0,1,1,1),(1,0,3,1),(3,1,0,0),(3,1,3,1),(None,1,3,1),(1,1,1,0),(0,0,3,0)]
with (w/'input.bin').open('wb') as f:
 for mode,(r1,r2),shape,kind in itertools.product(range(7),refs,shapes,(0,1,2,3)):
  patch,tree,pal,loading=shape;a=bytearray([0x95])*8192
  def put(off,v):struct.pack_into('<I',a,off,v&0xffffffff)
  put(4,baseaddr+0x700 if tree else 0);put(8,baseaddr+0x500 if patch is not None else 0);put(0x4fc,patch or 0);put(0x500,baseaddr+0x1700);put(0x700,baseaddr+0x1720);put(0x740,baseaddr+0x1720)
  put(0x18,baseaddr+0x1800);put(0x1c,baseaddr+0x900 if r1 is not None else 0);put(0x38,baseaddr+0x1820);put(0x3c,baseaddr+0x940 if r2 is not None else 0)
  for off,count,slot in [(0x900,r1 or 0,0x1c),(0x940,r2 or 0,0x3c),(0x980,1,0x3c),(0x9c0,2,0x1c)]:put(off,count);put(off+8,baseaddr+slot)
  put(0x44,baseaddr+0x800 if pal else 0);put(0x48,baseaddr+0x800+pal*8 if pal else 0);put(0x4c,baseaddr+0x818 if pal else 0)
  for j in range(3):put(0x804+j*8,0x11110000+j)
  put(0x864,0x2222);put(0x86c,0x3333);a[0x5c]=(0,1,0x80,0xff)[mode%4];a[0x5e]=loading
  put(0x20,baseaddr+0xa00);put(0x30,7);put(0xa08,baseaddr+0xb00);put(0xa0c,1);put(0xb00,0);put(0xb04,baseaddr+0xb40);put(0xb08,baseaddr+0xb40);put(0xb0c,baseaddr+0xb40);put(0xb40,1);put(0xb44,baseaddr+0xb00);put(0xb48,0);put(0xb4c,0);put(0xb50,7);put(0xb54,mode)
  put(0x1954,baseaddr+0x1e00);put(0x1578,0xffffffff-mode);struct.pack_into('<2H',a,0x145c,(0,15,16,17,65535,32,33)[mode],(65535,0,31,32,33,16,1)[mode])
  f.write(struct.pack('<4I',mode,min(kind,2),kind==3,0x80000000+mode));f.write(a);cases+=1
for kind in ('retail','compiled'):
 r=subprocess.run([str(w/'fixture.exe'),str(w/'input.bin'),str(w/(kind+'.bin')),kind],env=env,capture_output=True,text=True,timeout=60);assert not r.returncode,(w,r.returncode,r.stdout,r.stderr);print(r.stdout.strip(),kind,flush=True)
with (w/'retail.bin').open('rb') as e,(w/'compiled.bin').open('rb') as f:
 while True:
  x=e.read(1<<20);y=f.read(1<<20);assert x==y,(w,'landscape lifetime mismatch')
  if not x:break
size=(w/'compiled.bin').stat().st_size;assert size%8212==0;snapshots=size//8212
headers=[ROOT/'rebuild/include'/p for p in ('candidates/landscape_background_lod_compiled.h','engine/CEngineTexturePalette.h','engine/CEngineResourceManager.h','fable_movable_resource.h','fable_counted_pointer_reset_inline.h')]
report=dict(cases=cases,snapshots=snapshots,omission_checks=len(sources),providers=proofs,source_sha256={str(p.relative_to(ROOT)):hashlib.sha256(p.read_bytes()).hexdigest() for p in sources+headers},fixture_sha256=hashlib.sha256(fixture.read_bytes()).hexdigest(),boundary='Seven original spans with16audited fixups;constructor,destructor,scalar deletion,relocation/can-relocate,actual request release and tree lookup. Both emitted landscape vtables audited against retail;native vptrs target actual tables without normalization. Full8KiB snapshots plus callback args and actual definition/resource globals. Constructor+destructor,direct destructor,scalar flags0/1;patch array missing/zero/1/3,tree null/live,palette0/1/3,loading,counted ownership null/0/1/2/ffffffff. Palette callback injects ownership or mutates map dimensions;resource callbacks replace tree/palette/Info. String,texture-palette construction,virtual child destruction,ownership DeleteFunc,global delete and node free remain boundaries. Free-renamed object body/relocs audited.')
(ROOT/'rebuild/compile-gate/native-landscape-lifetime.json').write_text(json.dumps(report,indent=2)+'\n');print('NATIVE_LANDSCAPE_LIFETIME_GATE_PASS',snapshots,'snapshots')
