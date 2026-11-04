"""Native block-manager reset/destruction with real tree deletion algorithms."""
import hashlib,itertools,json,struct,subprocess,tempfile
from pathlib import Path
from capstone import Cs,CS_ARCH_X86,CS_MODE_32
import check_one as V
from gate_game_component_abi import Coff
from variable_allocator_parity import check
from pe_oracle import EXE,read_virtual_data
ROOT=Path(__file__).resolve().parents[2];w=Path(tempfile.mkdtemp(prefix='native-block-lifetime-'));env=V.env()
sources=[ROOT/'rebuild/src/compiled'/p for p in ('00/be/CEngineStaticMapFileBlockManager_Reset_00be05d0.cpp','00/b4/CEngineStaticMapFileBlockManager_Destructor_00b443b0.cpp','00/b4/StaticMapBlockTree_Erase_00b441f0.cpp','00/42/STLRbTree_RebalanceForErase_0042a34e.cpp','00/41/STLRbTree_Extrema_0041c7e0.cpp')];fixture=ROOT/'rebuild/tests/integration/NativeBlockLifetime_test.cpp'
reset='?Reset@CEngineStaticMapFileBlockManager@@QAEXXZ';dtor='??1CEngineStaticMapFileBlockManager@@QAE@XZ';erase='?_Rebalance_for_erase@?$_Rb_global@_N@_STL@@SAPAU_Rb_tree_node_base@2@PAU32@AAPAU32@11@Z'
minimum='?_S_minimum@_Rb_tree_node_base@_STL@@SAPAU12@PAU12@@Z';maximum='?_S_maximum@_Rb_tree_node_base@_STL@@SAPAU12@PAU12@@Z';left='?_Rotate_left@?$_Rb_global@_N@_STL@@SAXPAU_Rb_tree_node_base@2@AAPAU32@@Z';right='?_Rotate_right@?$_Rb_global@_N@_STL@@SAXPAU_Rb_tree_node_base@2@AAPAU32@@Z'
(w/'block_lifetime_aliases.h').write_text('#pragma comment(linker,"/alternatename:@Entry@4='+reset+'")\n#pragma comment(linker,"/alternatename:@Destroy@4='+dtor+'")\n')
spans=[(0xbe05d0,218,0),(0x42a34e,572,512),(0x41c7e0,26,1152),(0x41c800,26,1184),(0x42951b,64,1216),(0x42955b,64,1280),(0xb443b0,106,1408),(0xb441f0,92,1536)]
code=bytearray(1628);fixups=[];raw=EXE.read_bytes();md=Cs(CS_ARCH_X86,CS_MODE_32);md.detail=True;allowed={a for a,_,_ in spans}|{0xbfe9bc,0xbfea14}
for a,n,offset in spans:
 body=read_virtual_data(raw,a,n);code[offset:offset+n]=body
 for i in md.disasm(body,a):
  if i.mnemonic=='call' and i.operands[0].type==2:
   target=i.operands[0].imm;assert target in allowed;fixups.append((offset+i.address-a+1,target))
assert len(fixups)==18
(w/'block_lifetime_retail.h').write_text('static const unsigned char retailCode[]={'+','.join(map(str,code))+'};\nstatic const unsigned retailFixups[][2]={'+','.join('{%d,%d}'%f for f in fixups)+'};\n')
objects=[]
for i,p in enumerate(sources+[fixture]+sources[:3]):
 o=w/(str(i)+'.obj');r=V.cl(['/nologo','/c','/O2','/MT','/I'+str(w),'/Fo'+str(o)]+(['/Dfree=BlockFree'] if i>=6 else [])+[str(p)],env,w);assert not r.returncode,r.stdout+r.stderr;objects.append(o)
treeerase=next(s for s in Coff(objects[2]).symbols if s.startswith('?_M_erase'))
targets={treeerase:0xb441f0,erase:0x42a34e,minimum:0x41c7e0,maximum:0x41c800,left:0x42951b,right:0x42955b,'??3@YAXPAX@Z':0xbfe9bc,'_free':0xbfea14}
proofs=[check(objects[i],s,a,n,c,targets) for i,s,a,n,c in [(0,reset,0xbe05d0,218,4),(1,dtor,0xb443b0,106,3),(2,treeerase,0xb441f0,92,3),(3,erase,0x42a34e,572,8),(4,minimum,0x41c7e0,26,0),(4,maximum,0x41c800,26,0),(3,left,0x42951b,64,0),(3,right,0x42955b,64,0)]]
for i,s in enumerate((reset,dtor,treeerase)):
 a,b=Coff(objects[i]),Coff(objects[i+6]);sec,off=a.symbols[s];sec1,off1=b.symbols[s];body,rel=a.sections[sec];body1,rel1=b.sections[sec1];assert off==off1 and body==body1 and {p:('_BlockFree' if t=='_free' else t,k) for p,(t,k) in rel.items()}==rel1
cmd=[str(V.VC/'bin/link.exe'),'/nologo','/out:'+str(w/'fixture.exe'),'kernel32.lib'];linked=objects[3:]
for omit in (0,1,3,4,5):
 r=subprocess.run(cmd+[str(o) for i,o in enumerate(linked) if i!=omit],env=env,capture_output=True,text=True);assert r.returncode
r=subprocess.run(cmd+list(map(str,linked)),env=env,capture_output=True,text=True);assert not r.returncode,r.stdout+r.stderr
base=0x24000000;cases=0
with (w/'input.bin').open('wb') as f:
 for fill,mode,owner,count,refs,bankrefs,kind in itertools.product((0,0x95),range(3),(0,0x100),(0,1,3,7),(None,0,1,2,0xffffffff),(None,0,1,2,0xffffffff),(0,1)):
  a=bytearray([fill])*16384
  def put(off,v):struct.pack_into('<I',a,off,v&0xffffffff)
  def build(ids,parent):
   if not ids:return 0
   mid=len(ids)//2;i=ids[mid];off=0x500+64*i;put(off,1);put(off+4,parent);put(off+8,build(ids[:mid],base+off));put(off+12,build(ids[mid+1:],base+off));return base+off
  put(owner,base+0x3d00);put(owner+4,base+0x3c00 if bankrefs is not None else 0);put(0x3c00,bankrefs or 0);put(0x3c08,base+0x3d00)
  put(owner+8,base+0x400);put(owner+12,count);put(0x400,0);put(0x404,build(list(range(count)),base+0x400));put(0x408,base+0x500 if count else base+0x400);put(0x40c,base+0x500+64*(count-1) if count else base+0x400)
  for i in range(count):
   node=0x500+64*i;info=0x3000+32*i;put(node+16,i);put(node+20,i%2);put(node+28,base+0x2000+128*i);put(node+32,base+info if refs is not None else 0);put(info,refs or 0);put(info+8,base+0x3800+64*i);put(0x3800+64*i,base+node)
  put(0x3e00,7);put(0x3e08,base+0x3f00);f.write(struct.pack('<3I',mode,owner,kind));f.write(a);cases+=1
for kind in ('retail','compiled'):
 r=subprocess.run([str(w/'fixture.exe'),str(w/'input.bin'),str(w/(kind+'.bin')),kind],env=env,capture_output=True,text=True,timeout=60);assert not r.returncode,(w,r.returncode,r.stdout,r.stderr);print(r.stdout.strip(),kind,flush=True)
with (w/'retail.bin').open('rb') as r,(w/'compiled.bin').open('rb') as c:
 while True:
  x=r.read(1<<20);y=c.read(1<<20);assert x==y,(w,'block lifetime mismatch')
  if not x:break
size=(w/'compiled.bin').stat().st_size;assert size%(16384+12)==0;snapshots=size//(16384+12)
headers=[ROOT/'rebuild/include'/p for p in ('engine/CEngineStaticMapFileBlockManager.h','fable_stlport_tree.h','fable_stlport_tree_erase.h','fable_counted_pointer.h')]
report=dict(cases=cases,snapshots=snapshots,omission_checks=5,providers=proofs,source_sha256={str(p.relative_to(ROOT)):hashlib.sha256(p.read_bytes()).hexdigest() for p in sources+headers},fixture_sha256=hashlib.sha256(fixture.read_bytes()).hexdigest(),boundary='Eight original spans with18 audited fixups:reset,destructor,recursive tree cleanup,rebalance,extrema and rotations. Actual algorithms compose;free symbol renaming audited against unchanged compiled bodies/relocations. Full16KiB arenas at DeleteFunc/operator delete/free/return. Trees0/1/3/7 nodes,independent data and bank Info null/count0/1/2/ffffffff,two owner offsets,two fills,callback mutations replace Info and alter owner count. Repeated Reset followed by destruction or direct destruction. Allocation cleanup and DeleteFunc remain boundaries.')
(ROOT/'rebuild/compile-gate/native-block-lifetime.json').write_text(json.dumps(report,indent=2)+'\n');print('NATIVE_BLOCK_LIFETIME_GATE_PASS',snapshots,'snapshots')
