"""Native InitEntry through filename multimap; native path conversion, lowercase, copy-on-write and last release."""
import struct,hashlib
from unicorn import Uc,UC_ARCH_X86,UC_MODE_32,UC_HOOK_CODE
from unicorn.x86_const import *
TREE=0x2000000;HEADER=TREE+0x200;OUTPUT=TREE+0x300;QUERY=TREE+0x400;REFS=TREE+0x1000;TEXT=TREE+0x2000;HEAP=0x2100000;STOP=0x2200000;STACK=0x3000000
TEXTS=[None,b'\x80',b'\xff',b'',b'a',b'ab.big',b'b',b'z']
BOUNDARIES=(0xbfed4c,0xbfe9f9,0xbfea14,0xbfe9bc,0xbfea0e,0xbfea1a,0xbfeae6,0xbfeaf2,0xbfeaec,0xbfeb1c,0xbfeb22,0xbfed10)
def execute(codes,count,pattern,rotation,alias):
 u=Uc(UC_ARCH_X86,UC_MODE_32)
 for a,n in ((0x420000,0x100000),(0x990000,0x50000),(0xbf0000,0x10000),(0x1240000,0x10000),(0x1290000,0x10000),(0x1390000,0x10000),(0x13b0000,0x10000),(TREE,0x10000),(HEAP,0x80000),(STACK,0x10000),(STOP,0x1000)):u.mem_map(a,n)
 def get(a):return int.from_bytes(u.mem_read(a,4),'little')
 def put(a,v):u.mem_write(a,struct.pack('<I',v&0xffffffff))
 for a,code in codes.items():u.mem_write(a,code)
 for a in BOUNDARIES:u.mem_write(a,b'\xc3')
 put(0x139c8a8,0x5a5a1234);put(0x13bd800,16)
 bank=TREE+0x3000;tree=bank+0xc4;put(bank+0x78,128);u.mem_write(HEAP,b'\x6b'*0x10000+b'\x7d'*0x70000);put(tree,HEADER);put(HEADER+8,HEADER);put(HEADER+12,HEADER);put(OUTPUT+4,0x12345678)
 inputs=[]
 for i,text in enumerate([b'', b'C:\\DIR\\\x80', b'/root/\xff', b'', b'D:A', b'folder\\aB.BIG', b'\\\\server\\share\\B', b'/root/Z', b'', b'C:\\DIR\\\x80', b'/ROOT/\xff', b'', b'D:a', b'FOLDER\\ab.big', b'\\\\server\\share\\b', b'/ROOT/z']):
  inputs.append(text);put(REFS+i*32,TEXT+i*128);put(REFS+i*32+4,len(text));put(REFS+i*32+8,0x80000000|len(text)+1);u.mem_write(REFS+i*32+12,b'\x01');put(REFS+i*32+13,1);u.mem_write(TEXT+i*128,text+b'\0')
 allocations={};events=[];frees=set();allocated=[];records=[];results=[]
 def read_string(a,wide=False):
  step=2 if wide else 1;out=bytearray()
  for i in range(1024):
   v=bytes(u.mem_read(a+i*step,step))
   if v==b'\0'*step:return bytes(out)
   out.extend(v)
  raise AssertionError('unterminated')
 def hook(u,a,size,data):
  if a not in BOUNDARIES:return
  sp=u.reg_read(UC_X86_REG_ESP);arg=get(sp+4);result=0
  if a==0xbfea0e and 0x42b587<=get(sp)<0x42b5a8:
   assert arg==24;result=HEAP+32*len(allocated);allocated.append(result);u.mem_write(result,b'\xcc'*24);events.append(('node',24))
  elif a in (0xbfea0e,0xbfea1a,0xbfeb22):
   assert 0<arg<992;result=HEAP+0x10000+len(allocations)*1024+16;assert result+arg<HEAP+0x80000;allocations[result]=(arg,a);events.append(('allocate',a,arg,result))
  elif a in (0xbfea14,0xbfe9bc,0xbfeb1c):
   if arg:
    if arg in [REFS+i*32 for i in range(16)]+[TEXT+i*128 for i in range(16)]:assert arg not in frees
    else:
     assert arg in allocations and arg not in frees
     n,kind=allocations[arg];assert (kind,a) in ((0xbfea0e,0xbfea14),(0xbfea1a,0xbfe9bc),(0xbfeb22,0xbfeb1c))
     assert bytes(u.mem_read(arg-16,16))==b'\x7d'*16 and bytes(u.mem_read(arg+n,16))==b'\x7d'*16
    frees.add(arg);events.append(('free',a,arg))
  elif a==0xbfeae6:
   source=get(sp+8);length=get(sp+12);assert length<1024;u.mem_write(arg,bytes(u.mem_read(source,length)));result=arg;events.append(('memmove',arg,source,length))
  elif a==0xbfed10:
   result=arg+32 if 65<=arg<=90 else arg;events.append(('tolower',arg,result))
  elif a==0xbfe9f9:assert u.reg_read(UC_X86_REG_ECX)==0x5a5a1234
  elif a==0xbfed4c:
   raw=read_string(arg,True);text=raw.decode('utf-16le');drive=text[:2] if len(text)>1 and text[1]==':' else '';rest=text[len(drive):];split=max(rest.rfind('/'),rest.rfind('\\'));directory=rest[:split+1];leaf=rest[split+1:];dot=leaf.rfind('.');name=leaf[:dot] if dot>=0 else leaf;ext=leaf[dot:] if dot>=0 else ''
   for i,value in enumerate((drive,directory,name,ext)):
    dest=get(sp+8+i*4)
    if dest:u.mem_write(dest,value.encode('utf-16le')+b'\0\0')
   events.append(('split',raw,drive,directory,name,ext))
  else:raise AssertionError(('unexpected failure boundary',hex(a)))
  u.reg_write(UC_X86_REG_EAX,result);u.reg_write(UC_X86_REG_ESP,sp+4);u.reg_write(UC_X86_REG_EIP,get(sp))
 u.hook_add(UC_HOOK_CODE,hook)
 sp=STACK+0x8000;regs=[UC_X86_REG_EBX,UC_X86_REG_ESI,UC_X86_REG_EDI,UC_X86_REG_EBP]
 def key(node):
  ref=get(node+16)
  if not ref:return 0
  text=get(ref);raw=bytes(u.mem_read(text,8)).split(b'\0',1)[0];assert raw in TEXTS;return TEXTS.index(raw)

 def walk(node,parent,visited):
  if not node:return [],1
  assert node in allocated and node not in visited;visited.add(node);assert get(node+4)==parent;left=get(node+8);right=get(node+12);color=u.mem_read(node,1)[0];assert color in (0,1)
  if not color:assert (not left or u.mem_read(left,1)[0]) and (not right or u.mem_read(right,1)[0])
  lo,lb=walk(left,node,visited);hi,hb=walk(right,node,visited);assert lb==hb;return lo+[(key(node),get(node+20),node)]+hi,lb+color
 zig=[0,7,1,6,2,5,3,4];mix=[3,1,6,0,7,2,5,4]
 for i in range(count):
  k=([i%8,7-i%8,zig[i%8],mix[i%8]][pattern]+rotation)%8;value=REFS+(k+(8 if alias and i&1 else 0))*32 if k else 0;payload=0xffffffff-i
  put(QUERY,value);put(QUERY+4,payload);put(sp,STOP);put(sp+4,payload);put(sp+8,QUERY);put(sp+12,QUERY);u.reg_write(UC_X86_REG_ESP,sp);u.reg_write(UC_X86_REG_ECX,bank)
  for j,r in enumerate(regs):u.reg_write(r,0x667700+j)
  u.emu_start(0x9ce050,STOP,count=100000)
  assert u.reg_read(UC_X86_REG_EIP)==STOP and u.reg_read(UC_X86_REG_ESP)==sp+16 and all(u.reg_read(r)==0x667700+j for j,r in enumerate(regs))
  if k in (0,3):
   assert len(allocated)==len(records);continue
  assert len(allocated)==len(records)+1
  node=allocated[-1];assert key(node)==k and get(node+20)==payload and get(OUTPUT+4)==0x12345678
  records.append((k,payload,node,value));assert get(tree+4)==len(records);root=get(HEADER+4);assert root and u.mem_read(root,1)[0]==1
  order,bh=walk(root,HEADER,set());expected=sorted(records,key=lambda x:x[0]);assert order==[(k,p,n) for k,p,n,v in expected]
  assert get(HEADER+8)==expected[0][2] and get(HEADER+12)==expected[-1][2];results.append((node,payload))
 expected=[1]*16
 for _,_,node,value in records:
  assert get(get(node+16)+13)==1
  assert bytes(u.mem_read(node+24,8))==b'\x6b'*8
 assert [get(REFS+i*32+13) for i in range(16)]==expected
 assert get(0x13bd800)==16+len(records) and get(0x13bca20)==0
 for i,text in enumerate(inputs):assert bytes(u.mem_read(TEXT+i*128,len(text)+1))==text+b"\0"
 for pointer,anchor in [(node+16,None) for _,_,node,_ in records]+[(QUERY,k) for k in range(16)]:
  if anchor is not None:put(QUERY,REFS+anchor*32)
  put(sp,STOP);u.reg_write(UC_X86_REG_ECX,pointer);u.reg_write(UC_X86_REG_ESP,sp);u.emu_start(0x99eae0,STOP,count=1000);assert get(pointer)==0 and u.reg_read(UC_X86_REG_ESP)==sp+4
 assert get(0x13bd800)==get(0x13bca20)==0 and set(allocations).issubset(frees)
 assert sum(p in frees for p in [REFS+i*32 for i in range(16)]+[TEXT+i*128 for i in range(16)])==32
 return dict(results=results,events=events,tree_sha256=hashlib.sha256(u.mem_read(TREE,0x10000)).hexdigest(),heap_sha256=hashlib.sha256(u.mem_read(HEAP,0x80000)).hexdigest())
def compare(candidate,original):
 cases=0
 for count in range(25):
  for pattern in range(4):
   for rotation in range(8):
    for alias in range(2):assert execute(candidate,count,pattern,rotation,alias)==execute(original,count,pattern,rotation,alias);cases+=1
 return cases
