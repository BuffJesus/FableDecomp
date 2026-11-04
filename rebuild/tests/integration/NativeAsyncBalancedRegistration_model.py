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
