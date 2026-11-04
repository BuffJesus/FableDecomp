"""Draft actual original range+all native children; REVIEW BEFORE RUN."""
import hashlib,importlib.util,itertools,json,struct,sys
from pathlib import Path
from unicorn import Uc,UC_ARCH_X86,UC_MODE_32,UC_HOOK_CODE
from unicorn.x86_const import *
R=Path(__file__).resolve().parents[3];W=Path(__file__).parent;sys.path.insert(0,str(R/'tools/decomp_pipeline'))
from pe_oracle import EXE,read_virtual_data
"""Independent abstract red-black insertion, no binary/source imports."""
import struct
class Node:
 def __init__(self,key,address):self.key=key;self.address=address;self.black=False;self.parent=None;self.left=None;self.right=None
class Tree:
 def __init__(self):self.root=None;self.nodes={}
 def find(self,key):return self.nodes.get(key)
 def parent_for(self,key):
  p=None;n=self.root
  while n:p=n;n=n.left if key<n.key else n.right
  return p
 def rotate(self,p,left):
  q=p.right if left else p.left;assert q is not None
  middle=q.left if left else q.right
  if left:p.right=middle
  else:p.left=middle
  if middle:middle.parent=p
  q.parent=p.parent
  if p.parent is None:self.root=q
  elif p is p.parent.left:p.parent.left=q
  else:p.parent.right=q
  if left:q.left=p
  else:q.right=p
  p.parent=q
 def insert(self,key,address):
  assert 0<=key<=0xffffffff
  if key in self.nodes:return self.nodes[key],False
  z=Node(key,address);p=self.parent_for(key);z.parent=p
  if p is None:self.root=z
  elif key<p.key:p.left=z
  else:p.right=z
  self.nodes[key]=z;n=z
  while n.parent is not None and not n.parent.black:
   p=n.parent;g=p.parent;assert g is not None;left=p is g.left;uncle=g.right if left else g.left
   if uncle is not None and not uncle.black:
    p.black=True;uncle.black=True;g.black=False;n=g
   else:
    inner=(n is p.right) if left else (n is p.left)
    if inner:
     n=p;self.rotate(n,left);p=n.parent;g=p.parent
    p.black=True;g.black=False;self.rotate(g,not left)
  self.root.black=True;return z,True
 def validate(self):
  if self.root is None:assert not self.nodes;return
  assert self.root.black and self.root.parent is None
  visited=set()
  def walk(n,parent,lo,hi):
   if n is None:return 1
   assert n.address not in visited and n.parent is parent and lo<n.key<hi
   visited.add(n.address)
   if not n.black:assert (n.left is None or n.left.black) and (n.right is None or n.right.black)
   left=walk(n.left,n,lo,n.key);right=walk(n.right,n,n.key,hi);assert left==right
   return left+int(n.black)
  walk(self.root,None,-1,0x100000000);assert visited=={n.address for n in self.nodes.values()}
 def serialize(self,m,tree,header):
  self.validate();m.put(tree,header);m.put(tree+4,len(self.nodes));m.write(header,b'\0')
  m.put(header+4,self.root.address if self.root else 0)
  m.put(header+8,self.nodes[min(self.nodes)].address if self.nodes else header)
  m.put(header+12,self.nodes[max(self.nodes)].address if self.nodes else header)
  for n in self.nodes.values():
   m.write(n.address,bytes([int(n.black)]));m.put(n.address+4,n.parent.address if n.parent else header)
   m.put(n.address+8,n.left.address if n.left else 0);m.put(n.address+12,n.right.address if n.right else 0);m.put(n.address+16,n.key)
def validate_arena(m,tree,header,keys):
 assert m.w(tree)==header and m.read(header,1)==b'\0' and m.w(tree+4)==len(keys)
 root=m.w(header+4);seen=set();ordered=[]
 def visit(p,parent,lo,hi):
  if not p:return 1
  assert p not in seen and m.w(p+4)==parent;seen.add(p);key=m.w(p+16);assert lo<key<hi
  color=m.read(p,1)[0];assert color in (0,1)
  l=m.w(p+8);r=m.w(p+12)
  if color==0:assert (not l or m.read(l,1)==b'\1') and (not r or m.read(r,1)==b'\1')
  left=visit(l,p,lo,key);ordered.append((key,p));right=visit(r,p,key,hi);assert left==right
  return left+color
 if root:assert m.read(root,1)==b'\1';visit(root,header,-1,0x100000000)
 else:assert not keys
 assert [k for k,p in ordered]==sorted(keys)
 assert m.w(header+8)==(ordered[0][1] if ordered else header) and m.w(header+12)==(ordered[-1][1] if ordered else header)
 assert len(seen)==len(keys)


"""Independent CLRS physical-node deletion; no native/source imports."""
def remove(tree,key):
 z=tree.nodes[key];old_color=z.black;y=z;x=None;parent=None
 def black(n):return n is None or n.black
 def transplant(a,b):
  if a.parent is None:tree.root=b
  elif a is a.parent.left:a.parent.left=b
  else:a.parent.right=b
  if b:b.parent=a.parent
 if z.left is None:
  x=z.right;parent=z.parent;transplant(z,x)
 elif z.right is None:
  x=z.left;parent=z.parent;transplant(z,x)
 else:
  y=z.right
  while y.left:y=y.left
  old_color=y.black;x=y.right
  if y.parent is z:
   parent=y
   if x:x.parent=y
  else:
   parent=y.parent;transplant(y,x);y.right=z.right;y.right.parent=y
  transplant(z,y);y.left=z.left;y.left.parent=y;y.black=z.black
  # Native swap leaves the removed physical node with successor's old color.
  z.black=old_color
 if old_color:
  while x is not tree.root and black(x):
   assert parent is not None
   left=x is parent.left;sibling=parent.right if left else parent.left;assert sibling
   if not sibling.black:
    sibling.black=True;parent.black=False;tree.rotate(parent,left);sibling=parent.right if left else parent.left
   near=sibling.left if left else sibling.right;far=sibling.right if left else sibling.left
   if black(near) and black(far):
    sibling.black=False;x=parent;parent=x.parent
   else:
    if black(far):
     if near:near.black=True
     sibling.black=False;tree.rotate(sibling,not left);sibling=parent.right if left else parent.left
    sibling.black=parent.black;parent.black=True
    far=sibling.right if left else sibling.left
    if far:far.black=True
    tree.rotate(parent,left);x=tree.root;break
  if x:x.black=True
 del tree.nodes[key];tree.validate();return z


A=0x2000000;N=32768;S=0x3000000;STOP=0x4000000;ENTRY=0x9d7760;FREE=0xbfea14
raw=EXE.read_bytes();pehash=hashlib.sha256(raw).hexdigest()
body_specs={"0x9d7760":[189,"c2376d2c44e94162c4e75464c804d2f51d2cb0959f4a2a994f66435dc60a7121"],"0x9d7430":[51,"0b7d9f8da519dda9d15ad9d0fc9a9793024110d30bbb8445a517b88b46ca232b"],"0x42a34e":[572,"70282448d15fc629d1d0f095ba566a159287c524ef047bed39c5c5f5a135e165"],"0x41c7e0":[26,"ea57a9b278610718140d605034574268c88ed19a3f81e95078850bae76f21237"],"0x41c800":[26,"b02bc0e564a1cbd9f764212e08b65819569d8585b885c6b403bd6978fea9ed5a"],"0x42951b":[64,"c44a29a3cd7e7ffa4ccb2dd5100a3b75d5577bf570984ee18c82149b9a7ac65b"],"0x42955b":[64,"5da67519fdaf57f135fac9f30778b434763f172765a7f2b0f56ae0c675831015"]}
bodies={int(a,16):read_virtual_data(raw,int(a,16),n) for a,(n,digest) in body_specs.items()}
for a,(n,digest) in body_specs.items():assert hashlib.sha256(bodies[int(a,16)]).hexdigest()==digest
inputs=(Path(__file__),R/'tools/decomp_pipeline/pe_oracle.py')
def hashes():return {str(p.relative_to(R)):hashlib.sha256(p.read_bytes()).hexdigest() for p in inputs}
before=hashes()
def word(v):return struct.pack('<I',v&0xffffffff)
class Memory:
 def __init__(self,b=None,u=None):self.b=b;self.u=u
 def read(self,p,n):
  assert n>=0 and A<=p and p+n<=A+N
  return bytes(self.b[p-A:p-A+n]) if self.b is not None else bytes(self.u.mem_read(p,n))
 def write(self,p,b):
  assert A<=p and p+len(b)<=A+N
  if self.b is not None:self.b[p-A:p-A+len(b)]=b
  else:self.u.mem_write(p,b)
 def put(self,p,v):self.write(p,word(v))
 def w(self,p):return struct.unpack('<I',self.read(p,4))[0]
 def snap(self):return self.read(A,N)
def free(m,c,p,events,snaps):
 events.append(('free',p));snaps.append(m.snap());m.put(c['marker'],m.w(c['marker'])+1)
 mode=c.get('mutation',0)
 if mode==1:m.put(c['tree']+4,m.w(c['tree']+4)+7)
 if mode==2 and len(events)==len(c['erasekeys']):m.put(c['tree'],c['alternate_header'])
 if mode==3:m.put(p+8,0xdeadbeef) # Only detached/freed node; cached left must survive.
def expected(initial,c,tree):
 m=Memory(bytearray(initial));events=[];snaps=[m.snap()];header=c['header']
 if c['first']==m.w(header+8) and c['last']==header:
  if m.w(c['tree']+4):
   def clear(n):
    while n:
     clear(n.right);left=n.left;free(m,c,n.address,events,snaps);n=left
   clear(tree.root);live=m.w(c['tree']);m.put(live+8,live);m.put(m.w(c['tree'])+4,0);live=m.w(c['tree']);m.put(live+12,live);m.put(c['tree']+4,0)
  tree.nodes={};tree.root=None
 else:
  # Keys chosen before execution encode the cached end/successor valid domain.
  for key in c['erasekeys']:
   count=m.w(c['tree']+4);z=remove(tree,key)
   tree.serialize(m,c['tree'],header);m.put(c['tree']+4,count)
   # Removed physical node's links/key persist; only its color may be swapped.
   m.write(z.address,bytes([int(z.black)]));free(m,c,z.address,events,snaps);m.put(c['tree']+4,m.w(c['tree']+4)-1)
 snaps.append(m.snap());return events,snaps
class Runner:
 def __init__(self,selected_bodies=None):
  selected_bodies=bodies if selected_bodies is None else selected_bodies
  self.u=Uc(UC_ARCH_X86,UC_MODE_32);u=self.u
  for page in {a&~4095 for a in (*selected_bodies,FREE)}|{(FREE&~4095)+4096}:u.mem_map(page,4096)
  u.mem_map(A,N);u.mem_map(S,65536);u.mem_map(STOP,4096)
  for a,b in selected_bodies.items():u.mem_write(a,b)
  self.cpu=u.context_save();self.active=None;self.hits={hex(a):0 for a in selected_bodies};u.hook_add(UC_HOOK_CODE,lambda uc,pc,n,_:self.active(uc,pc))
 def run(self,initial,c):
  u=self.u;u.context_restore(self.cpu);u.mem_write(A,initial);u.mem_write(S,bytes(65536));m=Memory(u=u);sp=S+0x8000;events=[];snaps=[m.snap()]
  u.mem_write(sp,word(STOP)+word(c['first'])+word(c['last']));u.reg_write(UC_X86_REG_ESP,sp);u.reg_write(UC_X86_REG_ECX,c['tree']);regs=(UC_X86_REG_EBX,UC_X86_REG_EBP,UC_X86_REG_ESI,UC_X86_REG_EDI)
  for i,r in enumerate(regs):u.reg_write(r,0x11223300+i)
  def hook(uc,pc):
   if hex(pc) in self.hits:self.hits[hex(pc)]+=1
   if pc==STOP:uc.emu_stop();return
   if pc!=FREE:return
   stack=uc.reg_read(UC_X86_REG_ESP);p=struct.unpack('<I',uc.mem_read(stack+4,4))[0];assert p in c['nodeaddresses'];free(m,c,p,events,snaps)
   ret=struct.unpack('<I',uc.mem_read(stack,4))[0];uc.reg_write(UC_X86_REG_EAX,0xabcdef00);uc.reg_write(UC_X86_REG_ECX,0x98765432);uc.reg_write(UC_X86_REG_EDX,0x87654321);uc.reg_write(UC_X86_REG_ESP,stack+4);uc.reg_write(UC_X86_REG_EIP,ret)
  self.active=hook;u.emu_start(ENTRY,STOP,count=20000)
  assert u.reg_read(UC_X86_REG_EIP)==STOP and u.reg_read(UC_X86_REG_ESP)==sp+12 and [u.reg_read(r) for r in regs]==[0x11223300+i for i in range(4)]
  class StructuralView:
   def w(self,p):return len(c['remaining']) if p==c['tree']+4 else m.w(p)
   def read(self,p,n):return m.read(p,n)
  # Count-mutating free callbacks deliberately leave cardinality inconsistent;
  # structural invariants remain independently checked, raw count is compared
  # by full arena, never normalized in snapshots.
  validate_arena(StructuralView(),c['tree'],m.w(c['tree']),c['remaining']);snaps.append(m.snap());return events,snaps
def run_baseline(runner):
 cases=points=0
 sequences=[(),(0,),(0,1),(1,0),*itertools.permutations(range(4)),tuple(range(8)),tuple(reversed(range(8))),(4,2,6,1,3,5,7,0),(0,7,1,6,2,5,3,4)]
 for sequence,mapping,place,fill in itertools.product(sequences,(tuple(range(8)),(0,1,2,0x7ffffffe,0x7fffffff,0x80000000,0xfffffffe,0xffffffff)),range(2),(0,0x95)):
  keys=[mapping[i] for i in sequence];sortedkeys=sorted(keys)
  for start in range(len(keys)+1):
   for end in range(start,len(keys)+1):
    base=A+place*0x3000;initial=bytearray([fill])*N;m=Memory(initial);tree=Tree()
    for key in keys:tree.insert(key,base+0x1000+len(tree.nodes)*32)
    header=base+0x200;owner=base+0x100;tree.serialize(m,owner,header)
    c=dict(tree=owner,header=header,marker=base+0x500,first=tree.nodes[sortedkeys[start]].address if start<len(keys) else header,last=tree.nodes[sortedkeys[end]].address if end<len(keys) else header,erasekeys=sortedkeys[start:end],nodeaddresses={n.address for n in tree.nodes.values()},remaining=set(sortedkeys[:start]+sortedkeys[end:]))
    want=expected(bytes(initial),c,tree);got=runner.run(bytes(initial),c);assert got==want,(keys,start,end,place,fill,got[0],want[0]);cases+=1;points+=len(want[1])
 assert hashes()==before and hashlib.sha256(EXE.read_bytes()).hexdigest()==pehash
 report=dict(status='ORIGINAL_RANGE_ERASE_ACTUAL_CHILDREN_MODEL_PASS',cases=cases,snapshots=points,provider_hits=runner.hits,input_sha256_before=before,input_sha256_after=hashes(),original_exe_sha256=pehash,boundary='Original range189B plus actualrecursiveclear51B/rebalance572B/extrema26B/rotations64B; onlycdeclfree interface. Independentlyinserted validRB trees/CLRSphysicaldeletion, full32KiB entry/free/final arenas, pointeesuntouched, validranges/cachedend and rootRET8/registers. No compiler/sourceacceptance/concurrency/heaplifetime or adversarialfreeheader/countmutation claim.')
 return report


def run_supplement(runner):
 cases=points=0;partitions={};driverhash=hashlib.sha256(Path(__file__).read_bytes()).hexdigest()
 for keys in ((4,2,6,1,3,5,7,0),(0,7,1,6,2,5,3,4)):
  for high in (False,True):
   values=[k+0x80000000 if high else k for k in keys];ordered=sorted(values)
   for place in range(2):
    for fill in (0,0x95):
     for label,start,end,mutation in (('count_live_interior',1,6,1),('count_live_fullclear',0,8,1),('late_header_fullclear',0,8,2),('cached_left_fullclear',0,8,3),('cached_successor_interior',1,6,3)):
      base=A+place*0x3000;initial=bytearray([fill])*N;m=Memory(initial);tree=Tree()
      for key in values:tree.insert(key,base+0x1000+len(tree.nodes)*32)
      owner=base+0x100;header=base+0x200;alternate=base+0x600;tree.serialize(m,owner,header)
      # Alternate header preinitialized distinct sentinels; fullclear must
      # overwrite each field using the late live owner reference.
      m.write(alternate,b'\0');m.put(alternate+4,0x11111111);m.put(alternate+8,0x22222222);m.put(alternate+12,0x33333333)
      case=dict(tree=owner,header=header,alternate_header=alternate,marker=base+0x500,first=tree.nodes[ordered[start]].address,last=tree.nodes[ordered[end]].address if end<8 else header,erasekeys=ordered[start:end],nodeaddresses={n.address for n in tree.nodes.values()},remaining=set(ordered[:start]+ordered[end:]),mutation=mutation)
      want=expected(bytes(initial),case,tree);got=runner.run(bytes(initial),case);assert got==want,(label,values,place,fill,got[0],want[0]);cases+=1;points+=len(want[1]);partitions[label]=partitions.get(label,0)+1
 assert hashes()==before and hashlib.sha256(EXE.read_bytes()).hexdigest()==pehash and hashlib.sha256(Path(__file__).read_bytes()).hexdigest()==driverhash
 report=dict(status='ORIGINAL_RANGE_ERASE_CALLBACK_SUPPLEMENT_PASS',supplement_cases=cases,snapshots=points,partitions=partitions,provider_hits=runner.hits,input_sha256=before,driver_sha256=driverhash,boundary='Distinct focused cases, not baseline recount. Only original actual chain/free callback. Count callback +7 eachfree proves postfree decrement; structuralRB checks independent of deliberately inconsistent rawcount, which fullarenas retain. Fullclear lateheader swap only on finalfree observes liveheader resets and oldheader untouched. Freed-node left poison proves cachedleft/successor unaffected. No live active-link mutation, physicalfree/lifetime/concurrency claim.')
 return report
