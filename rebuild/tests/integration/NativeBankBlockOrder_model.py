"""CBankFile::InitBlockOrder and its STLport sort closure: original/rebuilt emulation plus an
independent Python STLport 4.x sort (exact tie order) and a McIlroy adversary for the heap path."""
import struct,hashlib,random
from unicorn import Uc,UC_ARCH_X86,UC_MODE_32,UC_HOOK_CODE
from unicorn.x86_const import *
ROOT=0x9d0630;MALLOC=0xbfea0e;FREE=0xbfea14;MEMMOVE=0xbfeae6
# Retail bodies the closure runs: root, comparator, sort helpers, packed Get, mask table and the
# landed vector<unsigned long>::_M_insert_overflow (00464ACE, 188B).
RETAIL=((0x9cc640,94),(0x9d48c0,136),(0x9d2e40,132),(0x9d2ed0,234),(0x9d4620,168),(0x9d3cf0,77),(0x9d3750,186),(0x9d3040,186),(0x9d42b0,80),(0x9d3ed0,75),(0x9d3b90,217),(0x9d2fc0,127),(0xa629f0,101),(0x13979e8,132),(0x464ace,188))
BANK=0x2000000;DATA=0x2100000;PACK=0x2200000;HEAP=0x2400000;STOP=0x2600000;STACK=0x3000000
def bodies(image,read,root=None):
 out={a:read(image,a,n) for a,n in RETAIL};out[ROOT]=root if root is not None else read(image,ROOT,192);return out
# Independent STLport 4.x sort over entry indices (stl/_algo.c, threshold 16).
def stl_sort(items,lt):
 A=list(items)
 def median(a,b,c):
  if lt(a,b):return b if lt(b,c) else c if lt(a,c) else a
  return a if lt(a,c) else c if lt(b,c) else b
 def partition(f,l,p):
  while True:
   while lt(A[f],p):f+=1
   l-=1
   while lt(p,A[l]):l-=1
   if not f<l:return f
   A[f],A[l]=A[l],A[f];f+=1
 def push(f,hole,top,v):
  parent=(hole-1)//2
  while hole>top and lt(A[f+parent],v):A[f+hole]=A[f+parent];hole=parent;parent=(hole-1)//2
  A[f+hole]=v
 def adjust(f,hole,n,v):
  top=hole;child=2*hole+2
  while child<n:
   if lt(A[f+child],A[f+child-1]):child-=1
   A[f+hole]=A[f+child];hole=child;child=2*(child+1)
  if child==n:A[f+hole]=A[f+child-1];hole=child-1
  push(f,hole,top,v)
 def heapsort(f,l):
  n=l-f
  if n>=2:
   parent=(n-2)//2
   while True:
    adjust(f,parent,n,A[f+parent])
    if parent==0:break
    parent-=1
  while l-f>1:
   v=A[l-1];A[l-1]=A[f];adjust(f,0,l-1-f,v);l-=1
 stats={'heap':0}
 def intro(f,l,depth):
  while l-f>16:
   if depth==0:stats['heap']+=1;heapsort(f,l);return
   depth-=1;cut=partition(f,l,median(A[f],A[f+(l-f)//2],A[l-1]));intro(cut,l,depth);l=cut
 def linear(f,l,v):
  if lt(v,A[f]):A[f+1:l+1]=A[f:l];A[f]=v
  else:unguarded(l,v)
 def unguarded(l,v):
  n=l-1
  while lt(v,A[n]):A[l]=A[n];l=n;n-=1
  A[l]=v
 if A:
  intro(0,len(A),2*(len(A).bit_length()-1))
  if len(A)>16:
   for i in range(1,16):linear(0,i,A[i])
   for i in range(16,len(A)):unguarded(i,A[i])
  else:
   for i in range(1,len(A)):linear(0,i,A[i])
 return A,stats['heap']
def adversary(n):
 """McIlroy's antiqsort against stl_sort: keys that drive the introsort to its heap fallback."""
 gas=n;key=[gas]*n;state={'nsolid':0,'candidate':0}
 def lt(a,b):
  if key[a]==gas and key[b]==gas:
   if a==state['candidate']:key[a]=state['nsolid']
   else:key[b]=state['nsolid']
   state['nsolid']+=1
  if key[a]==gas:state['candidate']=a
  elif key[b]==gas:state['candidate']=b
  return key[a]<key[b]
 stl_sort(range(n),lt)
 return [k if k!=gas else n for k in key]
def scenario(n,valid,offsets,packed,prefill,seed):
 """Entry records for one case: (valid,size,offset) per entry, plus packed-offset choice."""
 rng=random.Random(seed*7919+n);rows=[]
 # Packed banks never list entry 0, so their adversary covers entries 1..n-1.
 base=(([0]+adversary(n-1)) if packed else adversary(n)) if offsets=='killer' else None
 for i in range(n):
  ok={'all':True,'mixed':rng.random()<0.7,'none':False,'zero':True}[valid] and not (packed and i==0)
  size=0 if (valid=='zero' and i%3==0) or (valid=='mixed' and i%5==0) else (i*37)%900+1
  off={'ascending':i*16,'descending':(n-i)*16,'random':rng.randrange(0,1<<20),'equal':0x4000,'few':rng.randrange(0,4)*0x100,'killer':base[i]*8 if base else 0}[offsets]
  rows.append((ok,size,off))
 return rows
def pack(values):
 bias=min(values) if values else 0;combined=0
 for v in values:combined|=v-bias
 bits=combined.bit_length();stream=0
 for i,v in enumerate(values):stream|=(v-bias)<<(i*bits)
 words=(bits*len(values)+31)//32
 return bias,bits,[(stream>>(32*i))&0xffffffff for i in range(words)]
def expected(rows,packed):
 order=[i for i,(ok,size,off) in enumerate(rows) if ok and size>0]
 offset=lambda i:rows[i][2]
 result,heap=stl_sort(order,lambda a,b:offset(a)<offset(b))
 return result,heap
def execute(codes,n,valid,offsets,packed,prefill,seed):
 u=Uc(UC_ARCH_X86,UC_MODE_32)
 for a,size in ((0x464000,0x1000),(0x9cc000,0x1000),(0x9d0000,0x5000),(0xa62000,0x1000),(0x1397000,0x1000),(0xbfe000,0x1000),(BANK,0x1000),(DATA,0x10000),(PACK,0x10000),(HEAP,0x100000),(STOP,0x1000),(STACK,0x10000)):u.mem_map(a,size)
 def get(a):return int.from_bytes(u.mem_read(a,4),'little')
 def put(a,v):u.mem_write(a,struct.pack('<I',v&0xffffffff))
 for a,body in codes.items():u.mem_write(a,body)
 for a in (MALLOC,FREE,MEMMOVE):u.mem_write(a,b'\xc3')
 u.mem_write(BANK,b'\x6b'*0x1000);u.mem_write(HEAP,b'\x7d'*0x100000);u.mem_write(PACK,b'\x5a'*0x10000)
 rows=scenario(n,valid,offsets,packed,prefill,seed);allocated={};freed=set();events=[];cursor=[HEAP+16]
 def allocate(size):
  assert 0<size<=0x8000;p=cursor[0];cursor[0]+=(size+31)&~15;allocated[p]=size;events.append(('malloc',size,p-HEAP));return p
 def hook(u,a,size,user):
  if a not in (MALLOC,FREE,MEMMOVE):return
  sp=u.reg_read(UC_X86_REG_ESP);ret=get(sp);result=0
  if a==MALLOC:result=allocate(get(sp+4))
  elif a==FREE:
   p=get(sp+4);events.append(('free',p-HEAP if p else 0))
   if p:
    assert p in allocated and p not in freed;assert bytes(u.mem_read(p-16,16))==b'\x7d'*16 and bytes(u.mem_read(p+allocated[p],16))==b'\x7d'*16;freed.add(p)
  else:
   dst,src,count=get(sp+4),get(sp+8),get(sp+12);assert count<0x8000;u.mem_write(dst,bytes(u.mem_read(src,count)));result=dst;events.append(('memmove',count))
  u.reg_write(UC_X86_REG_EAX,result);u.reg_write(UC_X86_REG_ESP,sp+4);u.reg_write(UC_X86_REG_EIP,ret)
 u.hook_add(UC_HOOK_CODE,hook)
 for i,(ok,size,off) in enumerate(rows):u.mem_write(DATA+12*i,struct.pack('<IIBB',off,size,0x33,1 if ok else 0)+b'\xee\xee')
 put(BANK+4,n);put(BANK+0x20,DATA);put(BANK+0x24,DATA+12*n);put(BANK+0x28,DATA+12*n)
 if packed:
  bias,bits,words=pack([r[2] for r in rows[1:]]);put(BANK+0x38,PACK if words else 0);put(BANK+0x3c,n-1);u.mem_write(BANK+0x40,bytes([bits]));put(BANK+0x44,bias)
  for i,w in enumerate(words):put(PACK+4*i,w)
  # Packed tables replace the runtime offsets (PackRuntimeData): poison them.
  for i in range(n):put(DATA+12*i,0xdeadbeef)
 else:put(BANK+0x38,0);put(BANK+0x3c,0);u.mem_write(BANK+0x40,b'\0');put(BANK+0x44,0)
 if prefill:
  p=allocate(12);put(p,7);put(p+4,3);put(BANK+0xe0,p);put(BANK+0xe4,p+8);put(BANK+0xe8,p+12)
 else:put(BANK+0xe0,0);put(BANK+0xe4,0);put(BANK+0xe8,0)
 before=bytes(u.mem_read(BANK,0x1000));data=bytes(u.mem_read(DATA,0x10000));packbytes=bytes(u.mem_read(PACK,0x10000))
 registers=(UC_X86_REG_EBX,UC_X86_REG_ESI,UC_X86_REG_EDI,UC_X86_REG_EBP);sp=STACK+0xf000;put(sp,STOP)
 for j,r in enumerate(registers):u.reg_write(r,0x889900+j)
 u.reg_write(UC_X86_REG_ECX,BANK);u.reg_write(UC_X86_REG_ESP,sp);u.emu_start(ROOT,STOP,count=20000000)
 assert u.reg_read(UC_X86_REG_EIP)==STOP and u.reg_read(UC_X86_REG_ESP)==sp+4 and all(u.reg_read(r)==0x889900+j for j,r in enumerate(registers))
 after=bytes(u.mem_read(BANK,0x1000));assert after[:0xe0]==before[:0xe0] and after[0xec:]==before[0xec:]
 assert bytes(u.mem_read(DATA,0x10000))==data and bytes(u.mem_read(PACK,0x10000))==packbytes
 begin,end,cap=get(BANK+0xe0),get(BANK+0xe4),get(BANK+0xe8);order=[get(begin+4*i) for i in range((end-begin)//4)] if begin else []
 if prefill:assert after==before and order==[7,3] and not [e for e in events[1:]];heap=0
 else:
  want,heap=expected(rows,packed);assert order==want,(order[:8],want[:8])
  assert (begin==0)==(not want) and cap>=end and (not begin or (begin in allocated and allocated[begin]==cap-begin and begin not in freed))
  assert set(allocated)-freed==({begin} if begin else set())
 return dict(order=order,heap=heap,events=events,bank=hashlib.sha256(after).hexdigest(),arena=hashlib.sha256(u.mem_read(HEAP,cursor[0]-HEAP)).hexdigest())
def cases():
 out=[]
 for n in (0,1,2,3,5,16,17,18,40,200):
  for valid in ('all','mixed','none','zero'):
   for offsets in ('ascending','descending','random','equal','few'):
    for packed in (0,1):out.append((n,valid,offsets,packed,0,len(out)))
 for n in (60,200,500):
  for packed in (0,1):out.append((n,'all','killer',packed,0,len(out)))
 for packed in (0,1):out.append((40,'all','random',packed,1,len(out)))
 return out
def compare(candidate,original):
 total=0;heap=0
 for args in cases():
  a=execute(candidate,*args);z=execute(original,*args);assert a==z,args;total+=1;heap+=a['heap']>0
 assert heap==6,heap
 return total,heap
def write_cases(path):
 """Fixture input: inputs plus the independent STLport order for every case."""
 out=[len(cases())]
 for n,valid,offsets,packed,prefill,seed in cases():
  rows=scenario(n,valid,offsets,packed,prefill,seed);bias,bits,words=pack([r[2] for r in rows[1:]]) if packed else (0,0,[])
  want,heap=expected(rows,packed);out+=[n,packed,prefill,1 if heap and not prefill else 0,bias,bits,len(words),*words]
  for ok,size,off in rows:out+=[off,size,1 if ok else 0]
  out+=[len(want),*want]
 import struct as S
 open(path,'wb').write(S.pack('<%dI'%len(out),*out));return len(out)
