"""Native map deletion, ownership callbacks and actual grid unlinking."""
import hashlib,itertools,json,struct,subprocess,tempfile
from pathlib import Path
from capstone import Cs,CS_ARCH_X86,CS_MODE_32
import check_one as V
from gate_game_component_abi import Coff
from gate_render_state_static_data import coff_data
from variable_allocator_parity import check
from pe_oracle import EXE,read_virtual_data
ROOT=Path(__file__).resolve().parents[2];w=Path(tempfile.mkdtemp(prefix='native-map-delete-'));env=V.env()
sources=[ROOT/'rebuild/src/compiled'/p for p in ('00/b4/CEngineWorldMap_DeleteMap_00b42b10.cpp','00/bd/CEngineLandscapeMap_GameMapNotifications_00bdbfe0.cpp','00/b3/CEngineWorldMap_UnlinkMapFromWorld_00b3ee80.cpp','01/43/NEngineInternal_MapRenderers_01436ea8.cpp','00/b4/CEngineMap_Destructor_00b42a90.cpp','00/b3/CEngineMap_CloseStaticMap_00b3ef40.cpp','00/be/CEngineStaticMapFileBlockManager_Reset_00be05d0.cpp','00/b4/CEngineStaticMapFileBlockManager_Destructor_00b443b0.cpp','00/b4/StaticMapBlockTree_Erase_00b441f0.cpp','00/42/STLRbTree_RebalanceForErase_0042a34e.cpp','00/41/STLRbTree_Extrema_0041c7e0.cpp','00/b6/MapRenderers_DeleteMap_00b67180.cpp','00/bd/CLocalDetailCacheMap_Destructor_00bded60.cpp','00/bd/CLocalDetailCacheMap_QuadLifetime_00bddfb0.cpp','00/bd/CLocalDetailCacheMap_CQuadTreeElement_RemoveObjectCollections_00bdda50.cpp','00/be/CEngineStaticMapFileBlockManager_ReleaseRequest_00be0400.cpp','00/be/STLStaticMapBlockTree_find_00be08e0.cpp','00/9d/CMovableResource_Verify_009d5100.cpp','00/bd/CLocalDetailCacheMap_QuadRelocation_00bde0c0.cpp','00/bd/CEngineLandscapeMemoryManager_SetVirtualBlockMovableResource_00bd9ab0.cpp')];fixture=ROOT/'rebuild/tests/integration/NativeMapDelete_test.cpp'
symbol='?DeleteMap@CEngineWorldMap@@QAEXJ@Z';unloaded='?OnGameMapUnloaded@CEngineLandscapeMap@@QAEXXZ';unlink='?UnlinkMapFromWorld@CEngineWorldMap@@AAEXPAVCEngineMap@@J@Z';land='?GPLandscapeRenderer@NEngineInternal@@3PAVCEngineLandscapeRenderer@@A';detail='?GPLocalDetailGenerator@NEngineInternal@@3PAVCEngineLocalDetailGenerator@@A'
targets={'?OnGameMapUnloaded@CEngineLandscapeMap@@QAEXXZ':0xbdbff0,'_free':0xbfea14,'?OnMapVisibilityChanged@CEngineLandscapeMap@@QAEX_N@Z':0xbdc3c0,'?OnMapVisibilityChanged@CLocalDetailCacheMap@@QAEX_N@Z':0xbde160,'?UnlinkMapFromWorld@CEngineWorldMap@@AAEXPAVCEngineMap@@J@Z':0xb3ee80,'?GPLandscapeRenderer@NEngineInternal@@3PAVCEngineLandscapeRenderer@@A':0x1436ea8,'?DeleteMap@CEngineLandscapeRenderer@@QAEXPAVCEngineMap@@@Z':0xb67180,'?GPLocalDetailGenerator@NEngineInternal@@3PAVCEngineLocalDetailGenerator@@A':0x1436e88,'?DeleteMap@CEngineLocalDetailGenerator@@QAEXPAVCEngineMap@@@Z':0xb59840,'??1CEngineMap@@QAE@XZ':0xb42a90,'??3@YAXPAX@Z':0xbfe9bc,'_memmove':0xbfeae6,'?_M_fill_insert@?$vector@PAVCEngineMap@@V?$allocator@PAVCEngineMap@@@_STL@@@_STL@@QAEXPAPAVCEngineMap@@IABQAV3@@Z':0xb43f80}
aliases={'@Entry@12':symbol,'_LandscapeGlobal':land,'_DetailGlobal':detail,'?OnMapVisibilityChanged@CEngineLandscapeMap@@QAEX_N@Z':'@LandscapeVisibility@12','?OnMapVisibilityChanged@CLocalDetailCacheMap@@QAEX_N@Z':'@DetailVisibility@12','_BaseTable':'??_7CMovableResource@@6B@','_QuadTable':'??_7CQuadTreeElement@CLocalDetailCacheMap@@6B@','?FreeVirtual@CEngineLandscapeMemoryManager@@QAEXPAX@Z':'@FreeVirtual@12','??1CCharString@@QAE@XZ':'@StringDtor@4','?CloseStaticMap@CEngineLandscapeMap@@QAEXXZ':'@CloseLandscape@4','?CloseStaticMap@CLocalDetailCacheMap@@QAEXXZ':'@CloseDetail@4'}
(w/'map_delete_aliases.h').write_text('\n'.join('#pragma comment(linker,"/alternatename:'+a+'='+b+'")' for a,b in aliases.items())+'\n')
raw=EXE.read_bytes();body=read_virtual_data(raw,0xb42b10,342);code=bytearray(2727);code[:342]=body;code[352:353]=read_virtual_data(raw,0xbdbff0,1);code[384:503]=read_virtual_data(raw,0xb3ee80,119);code[544:665]=read_virtual_data(raw,0xb42a90,121);code[704:795]=read_virtual_data(raw,0xb3ef40,91);fixups=[];md=Cs(CS_ARCH_X86,CS_MODE_32);md.detail=True
for i in md.disasm(body,0xb42b10):
 if i.mnemonic=='call' and i.operands[0].type==2:
  target=i.operands[0].imm;assert target in targets.values();fixups.append((i.address-0xb42b10+1,target,1))
 elif i.mnemonic=='mov':
  for op in i.operands:
   if op.type==3 and op.mem.disp in (0x1436ea8,0x1436e88):fixups.append((i.address-0xb42b10+i.disp_offset,op.mem.disp,0))
assert len(fixups)==13
lifetime_targets={'?CloseStaticMap@CEngineMap@@QAEXXZ':0xb3ef40,'?Reset@CEngineStaticMapFileBlockManager@@QAEXXZ':0xbe05d0,'??1CEngineStaticMapFileBlockManager@@QAE@XZ':0xb443b0,'??1CCharString@@QAE@XZ':0x99eae0,'?CloseStaticMap@CEngineLandscapeMap@@QAEXXZ':0xbdc4f0,'?CloseStaticMap@CLocalDetailCacheMap@@QAEXXZ':0xbddd50};lifetime_targets.update(targets)
for addr,n,offset in [(0xb42a90,121,544),(0xb3ef40,91,704)]:
 for i in md.disasm(code[offset:offset+n],addr):
  if i.mnemonic in ('call','jmp') and i.operands[0].type==2:
   target=i.operands[0].imm
   if addr<=target<addr+n:continue
   assert target in lifetime_targets.values();fixups.append((offset+i.address-addr+1,target,1))
  elif i.mnemonic=='mov':
   for op in i.operands:
    if op.type==3 and op.mem.disp in (0x1436ea8,0x1436e88):fixups.append((offset+i.address-addr+i.disp_offset,op.mem.disp,0))
assert len(fixups)==25
block_spans=[(0xbe05d0,218,832),(0xb443b0,106,1088),(0xb441f0,92,1216),(0x42a34e,572,1344),(0x41c7e0,26,1920),(0x41c800,26,1952),(0x42951b,64,1984),(0x42955b,64,2048)]
allowed={a for a,_,_ in block_spans}|{0xbfe9bc,0xbfea14}
for addr,n,offset in block_spans:
 code[offset:offset+n]=read_virtual_data(raw,addr,n)
 for i in md.disasm(code[offset:offset+n],addr):
  if i.mnemonic=='call' and i.operands[0].type==2:
   target=i.operands[0].imm;assert target in allowed;fixups.append((offset+i.address-addr+1,target,1))
assert len(fixups)==43
code[2176:2203]=read_virtual_data(raw,0xb67180,27);code[2240:2281]=read_virtual_data(raw,0xb59840,41)
for off,target in [(16,0xbded60),(22,0xbfe9bc)]:
 assert code[2240+off-1]==0xe8 and 0xb59840+off+4+struct.unpack_from('<i',code,2240+off)[0]==target
 fixups.append((2240+off,target,1))
code[2304:2348]=read_virtual_data(raw,0xbded60,44)
for off,target in [(12,0xbfea14),(28,0xbfea14),(40,0xbddfb0)]:
 assert code[2304+off-1] in (0xe8,0xe9) and 0xbded60+off+4+struct.unpack_from('<i',code,2304+off)[0]==target
 fixups.append((2304+off,target,1))
assert len(fixups)==48
for addr,n,offset in [(0xbddfb0,101,2400),(0xbdda50,44,2528),(0xbe0400,29,2592),(0xbe08e0,71,2656)]:
 code[offset:offset+n]=read_virtual_data(raw,addr,n)
 for i in md.disasm(code[offset:offset+n],addr):
  if i.mnemonic=='call' and i.operands[0].type==2:
   target=i.operands[0].imm;assert target in {0xbdda50,0xbe0400,0xbe08e0};fixups.append((offset+i.address-addr+1,target,1))
  elif i.mnemonic=='mov':
   for op in i.operands:
    if op.type==2 and op.imm in (0x129b860,0x12a7df0):fixups.append((offset+i.address-addr+i.imm_offset,op.imm,0))
assert len(fixups)==53
(w/'map_delete_retail.h').write_text('static const unsigned char retailCode[]={'+','.join(map(str,code))+'};\nstatic const unsigned retailFixups[][3]={'+','.join('{%d,%d,%d}'%f for f in fixups)+'};\n')
objects=[]
for i,p in enumerate(sources+[fixture,sources[0],sources[6],sources[7],sources[8],sources[12]]):
 o=w/(str(i)+'.obj');r=V.cl(['/nologo','/c','/O2','/MT','/I'+str(w),'/Fo'+str(o)]+(['/Dfree=MapFree'] if i>=21 else [])+[str(p)],env,w);assert not r.returncode,r.stdout+r.stderr;objects.append(o)
proofs=[check(objects[0],symbol,0xb42b10,342,13,targets),check(objects[1],unloaded,0xbdbff0,1,0,{}),check(objects[2],unlink,0xb3ee80,119,0,{}),check(objects[4],"??1CEngineMap@@QAE@XZ",0xb42a90,121,9,lifetime_targets),check(objects[5],"?CloseStaticMap@CEngineMap@@QAEXXZ",0xb3ef40,91,3,lifetime_targets)]
c0,c1=Coff(objects[0]),Coff(objects[21]);sec,off=c0.symbols[symbol];sec1,off1=c1.symbols[symbol];b,r=c0.sections[sec];b1,r1=c1.sections[sec1];assert b==b1 and off==off1 and {p:('_MapFree' if t=='_free' else t,k) for p,(t,k) in r.items()}==r1
for sym,a in [(land,0x1436ea8),(detail,0x1436e88)]:assert coff_data(Coff(objects[3]),sym,4)==read_virtual_data(raw,a,4)==bytes(4)
treeerase=next(s for s in Coff(objects[8]).symbols if s.startswith('?_M_erase'))
rebalance='?_Rebalance_for_erase@?$_Rb_global@_N@_STL@@SAPAU_Rb_tree_node_base@2@PAU32@AAPAU32@11@Z'
minimum='?_S_minimum@_Rb_tree_node_base@_STL@@SAPAU12@PAU12@@Z';maximum='?_S_maximum@_Rb_tree_node_base@_STL@@SAPAU12@PAU12@@Z';left='?_Rotate_left@?$_Rb_global@_N@_STL@@SAXPAU_Rb_tree_node_base@2@AAPAU32@@Z';right='?_Rotate_right@?$_Rb_global@_N@_STL@@SAXPAU_Rb_tree_node_base@2@AAPAU32@@Z'
block_targets={treeerase:0xb441f0,rebalance:0x42a34e,minimum:0x41c7e0,maximum:0x41c800,left:0x42951b,right:0x42955b,'??3@YAXPAX@Z':0xbfe9bc,'_free':0xbfea14}
proofs.extend(check(objects[i],sym,a,n,num,block_targets) for i,sym,a,n,num in [(6,'?Reset@CEngineStaticMapFileBlockManager@@QAEXXZ',0xbe05d0,218,4),(7,'??1CEngineStaticMapFileBlockManager@@QAE@XZ',0xb443b0,106,3),(8,treeerase,0xb441f0,92,3),(9,rebalance,0x42a34e,572,8),(10,minimum,0x41c7e0,26,0),(10,maximum,0x41c800,26,0),(9,left,0x42951b,64,0),(9,right,0x42955b,64,0)])
for i,j,sym in [(6,22,'?Reset@CEngineStaticMapFileBlockManager@@QAEXXZ'),(7,23,'??1CEngineStaticMapFileBlockManager@@QAE@XZ'),(8,24,treeerase),(12,25,"??1CLocalDetailCacheMap@@QAE@XZ")]:
 a,b=Coff(objects[i]),Coff(objects[j]);sec,off=a.symbols[sym];sec1,off1=b.symbols[sym];body,rel=a.sections[sec];body1,rel1=b.sections[sec1];assert off==off1 and body==body1 and {p:('_MapFree' if t=='_free' else t,k) for p,(t,k) in rel.items()}==rel1
proofs.extend([check(objects[11],'?DeleteMap@CEngineLandscapeRenderer@@QAEXPAVCEngineMap@@@Z',0xb67180,27,0,{}),check(objects[11],'?DeleteMap@CEngineLocalDetailGenerator@@QAEXPAVCEngineMap@@@Z',0xb59840,41,2,{'??1CLocalDetailCacheMap@@QAE@XZ':0xbded60,'??3@YAXPAX@Z':0xbfe9bc})])
proofs.append(check(objects[12],'??1CLocalDetailCacheMap@@QAE@XZ',0xbded60,44,3,{'_free':0xbfea14,'??1CQuadTreeElement@CLocalDetailCacheMap@@UAE@XZ':0xbddfb0}))
quadfind=next(s for s in Coff(objects[16]).symbols if s.startswith('?find@?$_Rb_tree@J'))
quadtargets={'??_7CMovableResource@@6B@':0x129b860,'??_7CQuadTreeElement@CLocalDetailCacheMap@@6B@':0x12a7df0,'?ReleaseRequest@CEngineStaticMapFileBlockManager@@QAEXJ@Z':0xbe0400,'?RemoveObjectCollections@CQuadTreeElement@CLocalDetailCacheMap@@QAEXXZ':0xbdda50,quadfind:0xbe08e0}
proofs.extend(check(objects[i],sym,a,n,num,quadtargets) for i,sym,a,n,num in [(13,'??1CQuadTreeElement@CLocalDetailCacheMap@@UAE@XZ',0xbddfb0,101,4),(14,'?RemoveObjectCollections@CQuadTreeElement@CLocalDetailCacheMap@@QAEXXZ',0xbdda50,44,0),(15,'?ReleaseRequest@CEngineStaticMapFileBlockManager@@QAEXJ@Z',0xbe0400,29,1),(16,quadfind,0xbe08e0,71,0),(17,'?Verify@CMovableResource@@UBEKXZ',0x9d5100,6,0)])
cmd=[str(V.VC/'bin/link.exe'),'/nologo','/out:'+str(w/'fixture.exe'),'kernel32.lib'];linked=[objects[i] for i in (1,2,3,4,5,9,10,11,13,14,15,16,17,18,19,20,21,22,23,24,25)]
for omit in tuple(i for i in range(21) if i!=15):
 r=subprocess.run(cmd+[str(o) for i,o in enumerate(linked) if i!=omit],env=env,capture_output=True,text=True);assert r.returncode
r=subprocess.run(cmd+list(map(str,linked)),env=env,capture_output=True,text=True);assert not r.returncode,r.stdout+r.stderr
base=0x24000000;cases=0
with (w/'input.bin').open('wb') as f:
 for fill,mode,index,pattern,children,visible,globals_on,grid in itertools.product((0,0x95),range(8),range(3),range(3),range(4),(0,1),range(4),(0,1)):
  a=bytearray([fill])*8192
  def put(off,v):struct.pack_into('<I',a,off,v&0xffffffff)
  put(12,base+0x1800 if grid else 0);put(16,8);put(20,8);put(24,-32);put(28,-32);put(0x20,base+0x400);put(0x24,base+0x40c);put(0x28,base+0x40c);put(0x2c,base+0x1500)
  for off in (0x1100,0x1200):
   put(off+0x58,base+0x1f00 if fill else 0);put(off+0x64,base+0x1f20 if fill else 0)
  for off in (0x1100,0x1200):
   put(off+4,base+0x600+index*0x100+0x44);put(off+0x10,base+0x1f40 if fill else 0)
   for i in range(4):put(off+0x14+i*4,base+0x1f80 if fill and i==0 else 0)
   put(off+0x34,base+off);put(off+0x40,index);a[off+0x54]=8 if fill else 0
  put(0x1f40,base+0x1e20);put(0x1f44,0);put(0x1f80,base+0x1e40)
  put(0x1000,base+0x1e00)
  mask=(0,1<<index,7)[pattern]
  for i in range(3):
   put(0x400+i*4,base+0x600+i*0x100 if mask&(1<<i) else 0);put(0x420+i*4,base+0x900+i*0x100 if mask&(1<<i) else 0)
   for off in (0x600,0x900):
    m=off+i*0x100;put(m,base+0x1000 if children&1 else 0);put(m+4,base+0x1100 if children&2 else 0);put(m+0x58,base+0x1540);struct.pack_into('<4H',a,m+0x5c,32,32,32*i,32*i);a[m+0x64]=visible
  for i in range(3):
   for off in (0x600,0x900):
    m=off+i*0x100;put(m+0x20,mode%2);put(m+0x3c,base+0x1700);put(m+0x40,base+0x1600 if fill else 0)
  for i in range(6):
   m=(0x600 if i<3 else 0x900)+(i%3)*0x100;head=0x1900+32*i;node=0x1b00+64*i
   put(m+0x44,base+0x1700);put(m+0x48,base+0x1780 if fill else 0);put(m+0x4c,base+head);put(m+0x50,1 if fill else 0)
   put(head,0);put(head+4,base+node if fill else 0);put(head+8,base+node if fill else base+head);put(head+12,base+node if fill else base+head)
   put(node,1);put(node+4,base+head);put(node+8,0);put(node+12,0);put(node+16,i);put(node+20,0);put(node+28,base+0x1700);put(node+32,base+0x1740);put(node+36,0)
  for off in (0x1600,0x1640,0x1680,0x1740,0x1780):put(off,(0,1,2,0xffffffff)[mode%4]);put(off+8,base+0x600+index*0x100)
  put(0x1500,base+0x1540);put(0x1504,base+0x1540);put(0x1540,base+0x1500);put(0x1544,base+0x1500)
  f.write(struct.pack('<4I',mode,index,globals_on&1,globals_on&2));f.write(a);cases+=1
for kind in ('retail','compiled'):
 r=subprocess.run([str(w/'fixture.exe'),str(w/'input.bin'),str(w/(kind+'.bin')),kind],env=env,capture_output=True,text=True,timeout=60);assert not r.returncode,(w,r.returncode,r.stdout,r.stderr);print(r.stdout.strip(),kind,flush=True)
with (w/'retail.bin').open('rb') as r,(w/'compiled.bin').open('rb') as c:
 while True:
  x=r.read(1<<20);y=c.read(1<<20);assert x==y,(w,'map deletion mismatch')
  if not x:break
size=(w/'compiled.bin').stat().st_size;assert size%(8192+20)==0;snapshots=size//(8192+20)
headers=[ROOT/'rebuild/include'/p for p in ('engine/CEngineWorldMap.h','engine/CEngineMap.h','engine/CEngineLandscapeRenderer.h','engine/CEngineLocalDetailGenerator.h','engine/CLocalDetailCacheMap.h','fable_engine_internal.h','fable_stlport_vector_impl.h','engine/CEngineStaticMapFileBlockManager.h','fable_stlport_tree.h','fable_stlport_tree_erase.h','fable_movable_resource.h','candidates/landscape_background_lod_compiled.h')]
report=dict(cases=cases,snapshots=snapshots,omission_checks=20,providers=proofs,data=[{'address':'01436ea8','symbol':land,'bytes':4},{'address':'01436e88','symbol':detail,'bytes':4}],source_sha256={str(p.relative_to(ROOT)):hashlib.sha256(p.read_bytes()).hexdigest() for p in sources+headers},fixture_sha256=hashlib.sha256(fixture.read_bytes()).hexdigest(),boundary='Exact342byte map deletion,actual quad destructor/collection cleanup/request release/tree find and44byte detail destructor and27/41byte renderer deletions,121byte destructor/91byte map close and block Reset/destructor/recursive erase/rebalance/extrema/rotations,empty landscape unload and119byte grid unlink;real zero renderer globals. Fifty-three audited fixups;free boundary renamed only after emitted body/relocation audit. Two calls per case;null/single/full arrays,three indices,child/list/grid/global combinations,eight mutation modes. Full8KiB arena and both globals at callbacks and return. Ownership counts0/1/2/ffffffff and mutating DeleteFunc callbacks exercise actual counted-pointer release. Visibility,landscape virtual deleting destructor,quad child/collection deleting destructors,child close,string destruction,free/operator delete remain explicit boundaries;vector tail cleanup and grid writes are actual production code. Relocation vtable entry uses the separately validated actual relocation/empty setter providers.')
(ROOT/'rebuild/compile-gate/native-map-delete.json').write_text(json.dumps(report,indent=2)+'\n');print('NATIVE_MAP_DELETE_GATE_PASS',snapshots,'snapshots')
