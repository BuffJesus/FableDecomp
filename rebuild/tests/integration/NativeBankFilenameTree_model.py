"""Native multimap insertion, balancing, stable duplicates and full arena replay."""
import struct,hashlib
from unicorn import Uc,UC_ARCH_X86,UC_MODE_32,UC_HOOK_CODE
from unicorn.x86_const import *
TREE=0x2000000;HEADER=TREE+0x200;OUTPUT=TREE+0x300;QUERY=TREE+0x400;REFS=TREE+0x1000;TEXT=TREE+0x2000;HEAP=0x2100000;STOP=0x2200000;STACK=0x3000000
TEXTS=[None,b'\x80',b'\xff',b'',b'a',b'ab',b'b',b'z']
def execute(codes,count,pattern,rotation,alias):
 u=Uc(UC_ARCH_X86,UC_MODE_32)
 for a,n in [(0x420000,0x100000),(0x99e000,0x1000),(0x9d3000,0x1000),(0xbfe000,0x1000),(TREE,0x10000),(HEAP,0x10000),(STOP,0x1000),(STACK,0x10000)]:u.mem_map(a,n)
 def put(a,v):u.mem_write(a,struct.pack('<I',v&0xffffffff))
 def get(a):return int.from_bytes(u.mem_read(a,4),'little')
 for a,b in codes.items():u.mem_write(a,bytes(b))
 for a in (0x99ec30,0xbfea0e):u.mem_write(a,b'\xc3')
 u.mem_write(HEAP,b'\x6b'*0x10000);put(TREE,HEADER);put(HEADER+8,HEADER);put(HEADER+12,HEADER);put(OUTPUT+4,0x12345678)
 for i in range(16):put(REFS+i*32,TEXT+(i%8)*32);put(REFS+i*32+13,1)
 for i,text in enumerate(TEXTS):u.mem_write(TEXT+i*32,(text or b'')+b'\0')
 allocated=[];events=[];records=[];results=[]
 def hook(u,a,z,data):
  if a not in (0x99ec30,0xbfea0e):return
  sp=u.reg_read(UC_X86_REG_ESP);arg=get(sp+4)
  if a==0xbfea0e:
   assert arg==24;result=HEAP+32*len(allocated);allocated.append(result);u.mem_write(result,b'\xcc'*24);pop=0;events.append(('malloc',24))
  else:
   result=u.reg_read(UC_X86_REG_ECX);value=get(arg);put(result,value)
   if value:assert REFS<=value<REFS+16*32 and (value-REFS)%32==0;put(value+13,get(value+13)+1)
   pop=4;events.append(('copy',value))
  u.reg_write(UC_X86_REG_EAX,result);u.reg_write(UC_X86_REG_ESP,sp+4+pop);u.reg_write(UC_X86_REG_EIP,get(sp))
 u.hook_add(UC_HOOK_CODE,hook);sp=STACK+0x8000;regs=[UC_X86_REG_EBX,UC_X86_REG_ESI,UC_X86_REG_EDI,UC_X86_REG_EBP]
 def key(node):return (get(node+16)-REFS)//32%8 if get(node+16) else 0
 def walk(node,parent,visited):
  if not node:return [],1
  assert node in allocated and node not in visited;visited.add(node);assert get(node+4)==parent;left=get(node+8);right=get(node+12);color=u.mem_read(node,1)[0];assert color in (0,1)
  if not color:assert (not left or u.mem_read(left,1)[0]) and (not right or u.mem_read(right,1)[0])
  lo,lb=walk(left,node,visited);hi,hb=walk(right,node,visited);assert lb==hb;return lo+[(key(node),get(node+20),node)]+hi,lb+color
 zig=[0,7,1,6,2,5,3,4];mix=[3,1,6,0,7,2,5,4]
 for i in range(count):
  k=([i%8,7-i%8,zig[i%8],mix[i%8]][pattern]+rotation)%8;value=REFS+(k+(8 if alias and i&1 else 0))*32 if k else 0;payload=0xffffffff-i
  put(QUERY,value);put(QUERY+4,payload);put(sp,STOP);put(sp+4,OUTPUT);put(sp+8,QUERY);u.reg_write(UC_X86_REG_ESP,sp);u.reg_write(UC_X86_REG_ECX,TREE)
  for j,r in enumerate(regs):u.reg_write(r,0x667700+j)
  u.emu_start(0x9d34e0,STOP,count=10000)
  assert u.reg_read(UC_X86_REG_EIP)==STOP and u.reg_read(UC_X86_REG_ESP)==sp+12 and all(u.reg_read(r)==0x667700+j for j,r in enumerate(regs))
  node=get(OUTPUT);assert node==allocated[-1] and key(node)==k and get(node+20)==payload and get(OUTPUT+4)==0x12345678 and len(allocated)==i+1
  records.append((k,payload,node,value));assert get(TREE+4)==len(records);root=get(HEADER+4);assert root and u.mem_read(root,1)[0]==1
  order,bh=walk(root,HEADER,set());expected=sorted(records,key=lambda x:x[0]);assert order==[(k,p,n) for k,p,n,v in expected]
  assert get(HEADER+8)==expected[0][2] and get(HEADER+12)==expected[-1][2];results.append((node,payload))
 expected=[1]*16
 for _,_,node,value in records:
  if value:expected[(value-REFS)//32]+=1
  assert bytes(u.mem_read(node+24,8))==b'\x6b'*8
 assert [get(REFS+i*32+13) for i in range(16)]==expected
 return dict(results=results,events=events,tree_sha256=hashlib.sha256(u.mem_read(TREE,0x10000)).hexdigest(),heap_sha256=hashlib.sha256(u.mem_read(HEAP,0x10000)).hexdigest())
def compare(candidate,original):
 cases=0
 for count in range(25):
  for pattern in range(4):
   for rotation in range(8):
    for alias in range(2):assert execute(candidate,count,pattern,rotation,alias)==execute(original,count,pattern,rotation,alias);cases+=1
 return cases
