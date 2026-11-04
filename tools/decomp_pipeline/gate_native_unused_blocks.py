"""Native unused-block traversal/ownership with actual reconstructed tree erase."""
import hashlib,itertools,json,struct,subprocess,tempfile
from pathlib import Path
from capstone import Cs,CS_ARCH_X86,CS_MODE_32
import check_one as V
from gate_game_component_abi import Coff
from variable_allocator_parity import check
from pe_oracle import EXE,read_virtual_data
ROOT=Path(__file__).resolve().parents[2];w=Path(tempfile.mkdtemp(prefix='native-unused-blocks-'));env=V.env()
source=ROOT/'rebuild/src/compiled/00/be/CEngineStaticMapFileBlockManager_DeleteUnusedBlocks_00be04f0.cpp';discard_source=ROOT/'rebuild/src/compiled/00/9d/CBankFileAsyncData_DiscardAsyncData_009d68a0.cpp';fixture=ROOT/'rebuild/tests/integration/NativeUnusedBlocks_test.cpp'
tree_source=ROOT/'rebuild/src/compiled/00/42/STLRbTree_RebalanceForErase_0042a34e.cpp';extrema_source=ROOT/'rebuild/src/compiled/00/41/STLRbTree_Extrema_0041c7e0.cpp'
symbol='?DeleteUnusedBlocks@CEngineStaticMapFileBlockManager@@QAEXXZ';discard='?DiscardAsyncData@CBankFileAsyncData@@SIXAAV?$CCountedPointer@VCBankFileAsyncData@@@@@Z';bank='?DiscardAsyncData@CBankFileAsync@@QAEXAAV?$CCountedPointer@VCBankFileAsyncData@@@@@Z';finished='?IsFinished@CBankFileAsyncData@@QBE_NXZ';erase='?_Rebalance_for_erase@?$_Rb_global@_N@_STL@@SAPAU_Rb_tree_node_base@2@PAU32@AAPAU32@11@Z'
targets={finished:0x9d54e0,discard:0x9d68a0,erase:0x42a34e,'??3@YAXPAX@Z':0xbfe9bc,'_free':0xbfea14,bank:0x9d67e0}
aliases={'@Entry@4':symbol,finished:'@Finished@4',bank:'@BankDiscard@12'}
(w/'unused_blocks_aliases.h').write_text('\n'.join('#pragma comment(linker,"/alternatename:'+a+'='+b+'")' for a,b in aliases.items())+'\n')
spans=[(0xbe04f0,210,0),(0x9d68a0,16,256),(0x42a34e,572,512),(0x41c7e0,26,1152),(0x41c800,26,1184),(0x42951b,64,1216),(0x42955b,64,1280)]
code=bytearray(1344);fixups=[];raw=EXE.read_bytes();md=Cs(CS_ARCH_X86,CS_MODE_32);md.detail=True
allowed=set(targets.values())|{a for a,_,_ in spans}
for address,n,offset in spans:
 body=read_virtual_data(raw,address,n);code[offset:offset+n]=body;ins=list(md.disasm(body,address));assert sum(i.size for i in ins)==n and ins[-1].mnemonic=='ret'
 for i in ins:
  if i.mnemonic=='call' and i.operands[0].type==2:
   target=i.operands[0].imm;assert target in allowed and body[i.address-address]==0xe8;fixups.append((offset+i.address-address+1,target))
  for op in i.operands:
   if op.type==3 and op.mem.base==0 and op.mem.index==0:assert op.mem.disp<0x400000,(i.address,i.op_str)
assert len(fixups)==14
(w/'unused_blocks_retail.h').write_text('static const unsigned char retailCode[]={'+','.join(str(b) for b in code)+'};\nstatic const unsigned retailFixups[][2]={'+','.join('{%d,%d}'%f for f in fixups)+'};\n')
objects=[]
for i,(p,extra) in enumerate([(source,[]),(source,['/Dfree=BlockFree']),(discard_source,[]),(fixture,[]),(tree_source,[]),(extrema_source,[])]):
 obj=w/(str(i)+'.obj');r=V.cl(['/nologo','/c','/O2','/MT','/I'+str(w),'/Fo'+str(obj)]+extra+[str(p)],env,w);assert not r.returncode,r.stdout+r.stderr;objects.append(obj)
proofs=[check(objects[0],symbol,0xbe04f0,210,5,targets),check(objects[2],discard,0x9d68a0,16,1,targets)]
minimum='?_S_minimum@_Rb_tree_node_base@_STL@@SAPAU12@PAU12@@Z';maximum='?_S_maximum@_Rb_tree_node_base@_STL@@SAPAU12@PAU12@@Z'
left='?_Rotate_left@?$_Rb_global@_N@_STL@@SAXPAU_Rb_tree_node_base@2@AAPAU32@@Z';right='?_Rotate_right@?$_Rb_global@_N@_STL@@SAXPAU_Rb_tree_node_base@2@AAPAU32@@Z'
targets.update({minimum:0x41c7e0,maximum:0x41c800,left:0x42951b,right:0x42955b})
proofs.extend(check(objects[i],s,a,n,count,targets) for i,s,a,n,count in [(4,erase,0x42a34e,572,8),(5,minimum,0x41c7e0,26,0),(5,maximum,0x41c800,26,0),(4,left,0x42951b,64,0),(4,right,0x42955b,64,0)])
original,renamed=Coff(objects[0]),Coff(objects[1]);sec,off=original.symbols[symbol];other,otheroff=renamed.symbols[symbol];left,lr=original.sections[sec];right,rr=renamed.sections[other]
assert left==right and off==otheroff and {p:('_BlockFree' if t=='_free' else t,k) for p,(t,k) in lr.items()}==rr
exe=w/'fixture.exe';cmd=[str(V.VC/'bin/link.exe'),'/nologo','/out:'+str(exe),'kernel32.lib']
for omitted in (1,2,4,5):
 r=subprocess.run(cmd+[str(p) for i,p in enumerate(objects) if i!=0 and i!=omitted],env=env,capture_output=True,text=True);assert r.returncode
r=subprocess.run(cmd+list(map(str,objects[1:])),env=env,capture_output=True,text=True);assert not r.returncode,r.stdout+r.stderr
inputs=bytearray();cases=0;base=0x24000000
for fill,mode,owner,count,info_count in itertools.product((0,0x55,0xff),range(4),(0,0x100),(0,1,3,7),(None,0,1,2)):
 masks=range(1<<count) if count<7 else (0,127,1,8,64,0x55,0x2a,0x7e)
 for mask in masks:
  arena=bytearray([fill])*16384
  def put(at,value):struct.pack_into('<I',arena,at,value&0xffffffff)
  def build(indices,parent):
   if not indices:return 0
   mid=len(indices)//2;idx=indices[mid];at=0x500+64*idx;put(at,1);put(at+4,parent)
   put(at+8,build(indices[:mid],base+at));put(at+12,build(indices[mid+1:],base+at));return base+at
  put(owner+8,base+0x400);put(owner+12,count);put(0x400,0);put(0x404,build(list(range(count)),base+0x400));put(0x408,base+0x500 if count else base+0x400);put(0x40c,base+0x500+64*(count-1) if count else base+0x400)
  for i in range(count):
   node=0x500+64*i;data=0x2000+128*i;info=0x3000+32*i
   put(node+16,i);put(node+20,0 if mask&(1<<i) else 1);put(node+28,base+data);put(node+32,base+info if info_count is not None else 0)
   put(data+4,base+node);put(data+40,base+0x1000);put(info,info_count or 0);put(info+8,base+0x3800+64*i)
   put(0x3800+64*i,base+node)
  put(0x3e00,7);put(0x3e08,base+0x3f00)
  inputs.extend(struct.pack('<2I',mode,owner));inputs.extend(arena);cases+=1
(w/'input.bin').write_bytes(inputs)
for kind in ('retail','compiled'):
 r=subprocess.run([str(exe),str(w/'input.bin'),str(w/(kind+'.bin')),kind],env=env,capture_output=True,text=True,timeout=60);assert not r.returncode,(w,r.returncode,r.stdout,r.stderr);print(r.stdout.strip(),kind,flush=True)
actual=(w/'compiled.bin').read_bytes();expected=(w/'retail.bin').read_bytes();assert actual==expected,(w,'unused-block state');assert len(actual)%(16384+12)==0
snapshots=len(actual)//(16384+12)
headers=[ROOT/'rebuild/include'/p for p in ('engine/CEngineStaticMapFileBlockManager.h','fable_stlport_tree.h','fable_stlport_tree_erase.h','fable_game_text_bank.h','fable_counted_pointer.h')]
report=dict(cases=cases,snapshots=snapshots,omission_checks=4,providers=proofs,source_sha256={str(p.relative_to(ROOT)):hashlib.sha256(p.read_bytes()).hexdigest() for p in [source,discard_source,tree_source,extrema_source]+headers},fixture_sha256=hashlib.sha256(fixture.read_bytes()).hexdigest(),boundary='Seven original spans execute natively with14 audited fixups. Actual reconstructed cleanup/static discard/rebalance/extrema/rotations compose in compiled run. Free-renamed production body/relocations identical. Full16KiB at status/bank discard,DeleteFunc,operator delete,free and return. Valid all-black trees0/1/3/7 nodes,unused masks,Info null/count0/1/2,callbacks alter node refcount/Info/owner count. IsFinished,bank discard and allocation cleanup remain recorded boundaries;no snapshot normalization.')
(ROOT/'rebuild/compile-gate/native-unused-blocks.json').write_text(json.dumps(report,indent=2)+'\n');print('NATIVE_UNUSED_BLOCKS_GATE_PASS',snapshots,'snapshots')
