"""Native complete red-black deletion, extrema and compiler-emitted rotations."""
import hashlib,itertools,json,random,struct,subprocess,tempfile
from pathlib import Path
from capstone import Cs,CS_ARCH_X86,CS_MODE_32
import check_one as V
from gate_game_component_abi import Coff
from variable_allocator_parity import check
from pe_oracle import EXE,read_virtual_data
ROOT=Path(__file__).resolve().parents[2];w=Path(tempfile.mkdtemp(prefix='native-tree-erase-'));env=V.env()
sources=[ROOT/'rebuild/src/compiled'/p for p in ('00/42/STLRbTree_RebalanceForErase_0042a34e.cpp','00/41/STLRbTree_Extrema_0041c7e0.cpp')];fixture=ROOT/'rebuild/tests/integration/NativeTreeErase_test.cpp'
erase='?_Rebalance_for_erase@?$_Rb_global@_N@_STL@@SAPAU_Rb_tree_node_base@2@PAU32@AAPAU32@11@Z';minimum='?_S_minimum@_Rb_tree_node_base@_STL@@SAPAU12@PAU12@@Z';maximum='?_S_maximum@_Rb_tree_node_base@_STL@@SAPAU12@PAU12@@Z'
rotate_left='?_Rotate_left@?$_Rb_global@_N@_STL@@SAXPAU_Rb_tree_node_base@2@AAPAU32@@Z';rotate_right='?_Rotate_right@?$_Rb_global@_N@_STL@@SAXPAU_Rb_tree_node_base@2@AAPAU32@@Z'
targets={minimum:0x41c7e0,maximum:0x41c800,rotate_left:0x42951b,rotate_right:0x42955b}
aliases={'_EraseEntry':erase,'_MinimumEntry':minimum,'_MaximumEntry':maximum}
(w/'tree_erase_aliases.h').write_text('\n'.join('#pragma comment(linker,"/alternatename:'+a+'='+b+'")' for a,b in aliases.items())+'\n')
spans=[(0x42a34e,572,0),(0x41c7e0,26,640),(0x41c800,26,672),(0x42951b,64,704),(0x42955b,64,768)];offsets={a:o for a,_,o in spans};code=bytearray(832);fixups=[];raw=EXE.read_bytes();md=Cs(CS_ARCH_X86,CS_MODE_32);md.detail=True
for address,n,offset in spans:
 body=read_virtual_data(raw,address,n);code[offset:offset+n]=body;ins=list(md.disasm(body,address));assert sum(i.size for i in ins)==n and ins[-1].mnemonic=='ret'
 for i in ins:
  if i.mnemonic=='call':assert i.operands[0].type==2 and i.operands[0].imm in offsets;fixups.append((offset+i.address-address+1,offsets[i.operands[0].imm]))
  for op in i.operands:
   if op.type==3 and op.mem.base==0 and op.mem.index==0:assert op.mem.disp<0x400000
assert len(fixups)==8
(w/'tree_erase_retail.h').write_text('static const unsigned char retailCode[]={'+','.join(str(b) for b in code)+'};\nstatic const unsigned retailFixups[][2]={'+','.join('{%d,%d}'%f for f in fixups)+'};\n')
objects=[]
for i,p in enumerate(sources+[fixture]):
 obj=w/(str(i)+'.obj');r=V.cl(['/nologo','/c','/O2','/MT','/I'+str(w),'/Fo'+str(obj),str(p)],env,w);assert not r.returncode,r.stdout+r.stderr;objects.append(obj)
proofs=[check(objects[i],s,a,n,count,targets) for i,s,a,n,count in [(0,erase,0x42a34e,572,8),(1,minimum,0x41c7e0,26,0),(1,maximum,0x41c800,26,0),(0,rotate_left,0x42951b,64,0),(0,rotate_right,0x42955b,64,0)]]
exe=w/'fixture.exe';cmd=[str(V.VC/'bin/link.exe'),'/nologo','/out:'+str(exe),'kernel32.lib']
for omitted in range(2):
 r=subprocess.run(cmd+[str(p) for i,p in enumerate(objects) if i!=omitted],env=env,capture_output=True,text=True);assert r.returncode
r=subprocess.run(cmd+list(map(str,objects)),env=env,capture_output=True,text=True);assert not r.returncode,r.stdout+r.stderr
class Tree:
 def __init__(self,order):
  self.root=None;self.nodes={}
  for key in order:self.insert(key)
  self.validate()
 def red(self,node):return node is not None and self.nodes[node][3]==0
 def rotate(self,x,side):
  # side0 rotates left, side1 rotates right; child slots are0(left),1(right).
  other=1-side;y=self.nodes[x][other];self.nodes[x][other]=self.nodes[y][side]
  if self.nodes[y][side] is not None:self.nodes[self.nodes[y][side]][2]=x
  self.nodes[y][2]=self.nodes[x][2];parent=self.nodes[x][2]
  if parent is None:self.root=y
  else:self.nodes[parent][int(self.nodes[parent][1]==x)]=y
  self.nodes[y][side]=x;self.nodes[x][2]=y
 def insert(self,z):
  parent=None;x=self.root
  while x is not None:parent=x;x=self.nodes[x][int(z>x)]
  self.nodes[z]=[None,None,parent,0]
  if parent is None:self.root=z
  else:self.nodes[parent][int(z>parent)]=z
  while self.red(self.nodes[z][2]):
   p=self.nodes[z][2];g=self.nodes[p][2];side=int(self.nodes[g][1]==p);uncle=self.nodes[g][1-side]
   if self.red(uncle):self.nodes[p][3]=self.nodes[uncle][3]=1;self.nodes[g][3]=0;z=g
   else:
    if z==self.nodes[p][1-side]:z=p;self.rotate(z,side);p=self.nodes[z][2];g=self.nodes[p][2]
    self.nodes[p][3]=1;self.nodes[g][3]=0;self.rotate(g,1-side)
  self.nodes[self.root][3]=1
 def validate(self):
  assert self.nodes[self.root][2] is None and self.nodes[self.root][3]==1
  def visit(n,parent,lo,hi):
   if n is None:return 1
   l,r,p,color=self.nodes[n];assert lo<n<hi and p==parent
   if color==0:assert not self.red(l) and not self.red(r)
   left=visit(l,n,lo,n);right=visit(r,n,n,hi);assert left==right
   return left+color
  visit(self.root,None,-1,32)
rng=random.Random(0x42a34e);scenarios=[]
for count in range(1,6):
 for order in itertools.permutations(range(count)):
  for reverse in (False,True):scenarios.append((list(order),list(reversed(range(count))) if reverse else list(range(count))))
for i in range(400):
 count=(7,15,31)[i%3];order=list(range(count));rng.shuffle(order);deletion=list(range(count));rng.shuffle(deletion);scenarios.append((order,deletion))
inputs=bytearray();cases=snapshots=0;base=0x24000000
for order,deletion in scenarios:
 tree=Tree(order);count=len(order);header=(0,0x100)[cases%2];arena=bytearray([(0,0x55,0xff)[cases%3]])*4096
 def ptr(n):return base+0x500+32*n if n is not None else 0
 def put(at,value):struct.pack_into('<I',arena,at,value)
 put(header,0);put(header+4,ptr(tree.root));put(header+8,ptr(0));put(header+12,ptr(count-1))
 for key,(left,right,parent,color) in tree.nodes.items():
  at=0x500+32*key;arena[at]=color;put(at+4,ptr(parent) if parent is not None else base+header);put(at+8,ptr(left));put(at+12,ptr(right));put(at+16,key)
 inputs.extend(struct.pack('<2I',header,count));inputs.extend(struct.pack('<31I',*(deletion+[0]*(31-count))));inputs.extend(arena);cases+=1;snapshots+=count
(w/'input.bin').write_bytes(inputs)
for kind in ('retail','compiled'):
 r=subprocess.run([str(exe),str(w/'input.bin'),str(w/(kind+'.bin')),kind],env=env,capture_output=True,text=True,timeout=60);assert not r.returncode,(w,r.returncode,r.stdout,r.stderr);print(r.stdout.strip(),kind,flush=True)
actual=(w/'compiled.bin').read_bytes();expected=(w/'retail.bin').read_bytes();assert actual==expected,(w,'tree erase/extrema');assert len(actual)==snapshots*4108
header=ROOT/'rebuild/include/fable_stlport_tree.h'
report=dict(cases=cases,snapshots=snapshots,omission_checks=2,providers=proofs,source_sha256={str(p.relative_to(ROOT)):hashlib.sha256(p.read_bytes()).hexdigest() for p in sources+[header]},fixture_sha256=hashlib.sha256(fixture.read_bytes()).hexdigest(),boundary='Five original spans execute natively with eight audited calls. Complete572byte rebalance,26byte extrema and both64byte compiler-emitted rotations. All insertion permutations for1..5 nodes with ascending/descending deletion;400seeded mixed-color7/15/31-node trees with shuffled insertion/deletion. Input parent/order/red/black-height invariants checked independently. Full4KiB and returned erased/minimum/maximum node after each deletion until empty. No callback substitutes or snapshot normalization.')
(ROOT/'rebuild/compile-gate/native-tree-erase.json').write_text(json.dumps(report,indent=2)+'\n');print('NATIVE_TREE_ERASE_GATE_PASS',snapshots,'snapshots')
