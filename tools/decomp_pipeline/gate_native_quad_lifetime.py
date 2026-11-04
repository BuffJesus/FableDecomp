"""Native movable-resource and detail-quadtree lifetime, scalar deletion and vtables."""
import hashlib,itertools,json,struct,subprocess,tempfile
from pathlib import Path
from capstone import Cs,CS_ARCH_X86,CS_MODE_32
import check_one as V
from gate_game_component_abi import Coff
from variable_allocator_parity import check
from pe_oracle import EXE,read_virtual_data
ROOT=Path(__file__).resolve().parents[2];w=Path(tempfile.mkdtemp(prefix='native-quad-lifetime-'));env=V.env()
sources=[ROOT/'rebuild/src/compiled'/p for p in ('00/bd/CLocalDetailCacheMap_QuadLifetime_00bddfb0.cpp','00/bd/CLocalDetailCacheMap_CQuadTreeElement_RemoveObjectCollections_00bdda50.cpp','00/be/CEngineStaticMapFileBlockManager_ReleaseRequest_00be0400.cpp','00/be/STLStaticMapBlockTree_find_00be08e0.cpp','01/43/NEngineInternal_MapRenderers_01436ea8.cpp','00/9d/CMovableResource_Verify_009d5100.cpp','00/bd/CLocalDetailCacheMap_QuadRelocation_00bde0c0.cpp','00/bd/CEngineLandscapeMemoryManager_SetVirtualBlockMovableResource_00bd9ab0.cpp')];fixture=ROOT/'rebuild/tests/integration/NativeQuadLifetime_test.cpp'
qd='??1CQuadTreeElement@CLocalDetailCacheMap@@UAE@XZ';qdel='??_GCQuadTreeElement@CLocalDetailCacheMap@@UAEPAXI@Z';bdel='??_GCMovableResource@@UAEPAXI@Z';qcan='?CanRelocateData@CQuadTreeElement@CLocalDetailCacheMap@@UBE_NJ@Z';bcan='?CanRelocateData@CMovableResource@@UBE_NJ@Z';bt='??_7CMovableResource@@6B@';qt='??_7CQuadTreeElement@CLocalDetailCacheMap@@6B@';glob='?GPLandscapeRenderer@NEngineInternal@@3PAVCEngineLandscapeRenderer@@A';release='?ReleaseRequest@CEngineStaticMapFileBlockManager@@QAEXJ@Z';remove='?RemoveObjectCollections@CQuadTreeElement@CLocalDetailCacheMap@@QAEXXZ';verify='?Verify@CMovableResource@@UBEKXZ';relocate='?RelocateData@CQuadTreeElement@CLocalDetailCacheMap@@UAEXJPAX0J@Z';freevirtual='?FreeVirtual@CEngineLandscapeMemoryManager@@QAEXPAX@Z'
aliases={'@Destroy@4':qd,'@DeleteQuad@12':qdel,'@DeleteBase@12':bdel,'@CanQuad@12':qcan,'@CanBase@12':bcan,'@VerifyBase@4':verify,'_Renderer':glob,'_BaseTable':bt,'_QuadTable':qt,freevirtual:'@FreeVirtual@12'}
(w/'quad_lifetime_aliases.h').write_text('\n'.join('#pragma comment(linker,"/alternatename:'+a+'='+b+'")' for a,b in aliases.items())+'\n')
spans=[(0xbddfb0,101,0),(0xbdfa10,38,128),(0x9d5110,31,192),(0xbddd10,17,256),(0x9d50f0,5,288),(0xbdda50,44,320),(0xbe0400,29,384),(0xbe08e0,71,448),(0x9d5100,6,544)]
raw=EXE.read_bytes();code=bytearray(550);fixups=[];md=Cs(CS_ARCH_X86,CS_MODE_32);md.detail=True;allowed={a for a,_,_ in spans}|{0xbd9b90,0xbfe9bc}
for a,n,offset in spans:
 body=read_virtual_data(raw,a,n);code[offset:offset+n]=body
 for i in md.disasm(body,a):
  if i.mnemonic=='call' and i.operands[0].type==2:
   target=i.operands[0].imm;assert target in allowed;fixups.append((offset+i.address-a+1,target,1))
  elif i.mnemonic=='mov':
   for op in i.operands:
    if op.type==2 and op.imm in (0x129b860,0x12a7df0):fixups.append((offset+i.address-a+i.imm_offset,op.imm,0))
    if op.type==3 and op.mem.disp==0x1436ea8:fixups.append((offset+i.address-a+i.disp_offset,op.mem.disp,0))
assert len(fixups)==10
(w/'quad_lifetime_retail.h').write_text('static const unsigned char retailCode[]={'+','.join(map(str,code))+'};\nstatic const unsigned retailFixups[][3]={'+','.join('{%d,%d,%d}'%f for f in fixups)+'};\n')
objects=[]
for i,p in enumerate(sources+[fixture]):
 o=w/(str(i)+'.obj');r=V.cl(['/nologo','/c','/O2','/MT','/I'+str(w),'/Fo'+str(o),str(p)],env,w);assert not r.returncode,r.stdout+r.stderr;objects.append(o)
find=next(s for s in Coff(objects[3]).symbols if s.startswith('?find@?$_Rb_tree@J'))
targets={bt:0x129b860,qt:0x12a7df0,release:0xbe0400,remove:0xbdda50,qd:0xbddfb0,glob:0x1436ea8,freevirtual:0xbd9b90,'??3@YAXPAX@Z':0xbfe9bc,find:0xbe08e0}
proofs=[check(objects[i],s,a,n,num,targets) for i,s,a,n,num in [(0,qd,0xbddfb0,101,4),(0,qdel,0xbdfa10,38,3),(0,bdel,0x9d5110,31,2),(0,qcan,0xbddd10,17,0),(0,bcan,0x9d50f0,5,0),(1,remove,0xbdda50,44,0),(2,release,0xbe0400,29,1),(3,find,0xbe08e0,71,0),(5,verify,0x9d5100,6,0)]]
tables=[];coff=Coff(objects[0]);table_targets={qdel.replace('??_G','??_E'):0xbdfa10,bdel.replace('??_G','??_E'):0x9d5110,'__purecall':0xbfea08,qcan:0xbddd10,bcan:0x9d50f0,relocate:0xbde0c0,verify:0x9d5100}
for sym,a in [(bt,0x129b860),(qt,0x12a7df0)]:
 sec,off=coff.symbols[sym];body,rels=coff.sections[sec];assert off==0 and body==bytes(16) and set(rels)=={0,4,8,12};patched=bytearray(body)
 for at,(target,kind) in rels.items():assert kind==6;struct.pack_into('<I',patched,at,table_targets[target])
 assert patched==read_virtual_data(raw,a,16);tables.append(dict(address=f'{a:08x}',symbol=sym,slots=[rels[i][0] for i in (0,4,8,12)]))
cmd=[str(V.VC/'bin/link.exe'),'/nologo','/out:'+str(w/'fixture.exe'),'kernel32.lib']
for omit in range(8):
 r=subprocess.run(cmd+[str(o) for i,o in enumerate(objects) if i!=omit],env=env,capture_output=True,text=True);assert r.returncode
r=subprocess.run(cmd+list(map(str,objects)),env=env,capture_output=True,text=True);assert not r.returncode,r.stdout+r.stderr
base=0x24000000;cases=0
with (w/'input.bin').open('wb') as f:
 for mode,childmask,collections,loading,locked,kind,flags in itertools.product(range(5),range(16),(0,1,3),(0,8),(0,1,0x80,0xff),range(3),(0,1,3)):
  # Deleting-base paths do not depend on children,collections or loading.
  if kind==2 and (childmask or collections or loading):continue
  if kind==0 and flags:continue
  a=bytearray([0x95])*8192
  def put(off,v):struct.pack_into('<I',a,off,v&0xffffffff)
  put(4,base+0x600 if collections else 0);put(0x28,base+0x800);put(0x34,7);a[0x48]=0x47|loading;a[0x800]=locked;put(0x804,base+0x900)
  for i in range(4):put(8+4*i,base+0x200+i*0x100 if childmask&(1<<i) else 0);put(0x200+i*0x100,base+0x1800)
  for i in range(collections):put(0x600+i*32,base+0x1820);put(0x604+i*32,base+0x600+(i+1)*32 if i+1<collections else 0)
  put(0x908,base+0xa00);put(0x90c,1);put(0xa00,0);put(0xa04,base+0xa40);put(0xa08,base+0xa40);put(0xa0c,base+0xa40);put(0xa40,1);put(0xa44,base+0xa00);put(0xa48,0);put(0xa4c,0);put(0xa50,7);put(0xa54,(0,1,2,0x7fffffff,0xffffffff)[mode]);put(0x16b0,base+0x1c00)
  f.write(struct.pack('<3I',mode,kind,flags));f.write(a);cases+=1
for kind in ('retail','compiled'):
 r=subprocess.run([str(w/'fixture.exe'),str(w/'input.bin'),str(w/(kind+'.bin')),kind],env=env,capture_output=True,text=True,timeout=60);assert not r.returncode,(w,r.returncode,r.stdout,r.stderr);print(r.stdout.strip(),kind,flush=True)
with (w/'retail.bin').open('rb') as r,(w/'compiled.bin').open('rb') as c:
 while True:
  x=r.read(1<<20);y=c.read(1<<20);assert x==y,(w,'quad lifetime mismatch')
  if not x:break
size=(w/'compiled.bin').stat().st_size;assert size%(8192+16)==0;snapshots=size//(8192+16)
headers=[ROOT/'rebuild/include'/p for p in ('engine/CLocalDetailCacheMap.h','engine/CEngineLandscapeRenderer.h','engine/CEngineLandscapeMemoryManager.h','engine/CEngineStaticMapFileBlockManager.h','fable_movable_resource.h','fable_game_text_bank.h','fable_engine_internal.h')]
report=dict(cases=cases,snapshots=snapshots,omission_checks=8,providers=proofs,vtables=tables,source_sha256={str(p.relative_to(ROOT)):hashlib.sha256(p.read_bytes()).hexdigest() for p in sources+headers},fixture_sha256=hashlib.sha256(fixture.read_bytes()).hexdigest(),boundary='Nine original bodies with10 audited fixups and both complete four-slot vtables audited against retail. Original vptr stores target the actual compiled tables without snapshot normalization. Actual quad/base scalar deletion,quad destructor,collection cleanup,request release,tree find and relocation/verify defaults. Child masks0..15,collections0/1/3,loading flag,lock bytes0/1/80/FF,delete flags0/1/3,direct/quad/base paths,callback mutations to later children and collection head. Full8KiB state at virtual child/collection deletion,pool/global delete and return. Child/collection deleting destructors,FreeVirtual and operator delete remain boundaries;RelocateData now links its accepted actual provider and retail-empty owner setter;its behavior is covered separately by gate_native_quad_relocation.py.')
(ROOT/'rebuild/compile-gate/native-quad-lifetime.json').write_text(json.dumps(report,indent=2)+'\n');print('NATIVE_QUAD_LIFETIME_GATE_PASS',snapshots,'snapshots')
