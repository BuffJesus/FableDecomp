"""Execute the pair<unsigned long, CDefClassBase::CSubDefInfo> sort / heap / copy_backward / LowerBound family against retail."""
import bisect,random,struct
from unicorn import Uc,UC_ARCH_X86,UC_MODE_32
from unicorn.x86_const import *
ARR=0x2000000;OUT=0x2100000;MAP=0x2200000;KEY=0x2200100;PIVOT=0x2200200;TAG=0x2200300;STACK=0x3000000;STOP=0x4000000
SORT,INTRO,FINAL,INSERT,UNG_AUX,COPY_BACK,_COPY_DEFERRED,UNG_LINEAR,PARTITION,PARTIAL=0x9b84c0,0x9b7dd0,0x9b6830,0x9b5670,0x9b2cb0,0x9b18e0,0x9b1620,0x9b1a30,0x9b3800,0x9b6ad0
PUSH,ADJUST,MAKE,SORT_HEAP,LOWER,LOWER_CONST=0x9b1ca0,0x9b2f30,0x9b3c00,0x9b5af0,0x9b20d0,0x9b2130
def pack(items):return b''.join(struct.pack('<3I',*e) for e in items)
def unpack(raw):return [struct.unpack_from('<3I',raw,i) for i in range(0,len(raw),12)]
def run(codes,entry,ecx,edx,stack,memory):
 u=Uc(UC_ARCH_X86,UC_MODE_32)
 for a,z in [(0x9b1000,0x9000),(ARR,0x300000),(STACK,0x10000),(STOP,0x1000)]:u.mem_map(a,z)
 for a,b in codes.items():u.mem_write(a,bytes(b))
 for a,b in memory.items():u.mem_write(a,b)
 sp=STACK+0x8000;u.mem_write(sp,struct.pack('<I',STOP)+b''.join(stack))
 regs=[UC_X86_REG_EBX,UC_X86_REG_ESI,UC_X86_REG_EDI,UC_X86_REG_EBP]
 for i,r in enumerate(regs):u.reg_write(r,0x778800+i)
 u.reg_write(UC_X86_REG_ECX,ecx);u.reg_write(UC_X86_REG_EDX,edx);u.reg_write(UC_X86_REG_ESP,sp);u.reg_write(UC_X86_REG_EAX,0xdeadbeef)
 u.emu_start(entry,STOP,count=3000000)
 assert u.reg_read(UC_X86_REG_ESP)==sp+4+sum(len(s) for s in stack) and all(u.reg_read(r)==0x778800+i for i,r in enumerate(regs)),hex(entry)
 return u.reg_read(UC_X86_REG_EAX),bytes(u.mem_read(ARR,0x20000)),bytes(u.mem_read(OUT,0x20000))
D=lambda v:struct.pack('<I',v&0xffffffff)
def entries(rng,n,keys):return [(rng.choice(keys),rng.getrandbits(32),rng.getrandbits(32)) for _ in range(n)]
def is_heap(keys):return all(keys[(i-1)//2]>=keys[i] for i in range(1,len(keys)))
def compare(candidate,native):
 rng=random.Random(0x5d3f);cases=0
 def both(entry,ecx,edx,stack,memory):
  c=run(candidate,entry,ecx,edx,stack,memory);assert c==run(native,entry,ecx,edx,stack,memory),hex(entry);return c
 shapes=[(n,ks) for n in (0,1,2,3,15,16,17,33,64,200) for ks in (list(range(4)),list(range(1000)),[0,0xffffffff,0x80000000])]
 for n,ks in shapes:
  items=entries(rng,n,ks);keys=lambda raw,m=n:[e[0] for e in unpack(raw[:12*m])]
  # The final pass's unguarded tail needs the minimum within the first 16
  # (introsort leaves it there).
  guarded=items[:]
  if n:low=min(range(n),key=lambda i:guarded[i][0]);guarded[0],guarded[low]=guarded[low],guarded[0]
  for entry in (SORT,FINAL,INSERT):
   _,mem,_=both(entry,ARR,ARR+12*n,[D(0)],{ARR:pack(guarded if entry==FINAL else items)});out=unpack(mem[:12*n]);assert sorted(out)==sorted(items) and keys(mem)==sorted(e[0] for e in items);cases+=1
  for depth in (0,1,2,40):
   # Runs of at most 16 are left for the final insertion pass; a zero depth
   # heap-sorts the whole range at once.
   _,mem,_=both(INTRO,ARR,ARR+12*n,[D(0),D(depth),D(0)],{ARR:pack(items)});out=unpack(mem[:12*n]);assert sorted(out)==sorted(items)
   if depth==0 and n>16:assert keys(mem)==sorted(e[0] for e in items)
   cases+=1
  if n>=2:
   _,mem,_=both(MAKE,ARR,ARR+12*n,[D(0),D(0),D(0)],{ARR:pack(items)});assert is_heap(keys(mem)) and sorted(unpack(mem[:12*n]))==sorted(items)
   heap=mem[:12*n];_,mem,_=both(SORT_HEAP,ARR,ARR+12*n,[D(0)],{ARR:heap});assert keys(mem)==sorted(e[0] for e in items) and sorted(unpack(mem[:12*n]))==sorted(items);cases+=2
   for middle in sorted({1,n//2,n}):
    _,mem,_=both(PARTIAL,ARR,ARR+12*middle,[D(ARR+12*n),D(0),D(0)],{ARR:pack(items)});k=keys(mem);assert k[:middle]==sorted(e[0] for e in items)[:middle] and sorted(unpack(mem[:12*n]))==sorted(items);cases+=1
   pivot=rng.choice(items);lo=min(items)[0]
   if any(e[0]<=pivot[0] for e in items) and any(e[0]>=pivot[0] for e in items):
    eax,mem,_=both(PARTITION,ARR,ARR+12*n,[pack([pivot]),D(0)],{ARR:pack(items)});cut=(eax-ARR)//12;k=keys(mem)
    assert (eax-ARR)%12==0 and 0<=cut<=n and all(x<=pivot[0] for x in k[:cut]) and all(x>=pivot[0] for x in k[cut:]) and sorted(unpack(mem[:12*n]))==sorted(items);cases+=1
  srt=sorted(items,key=lambda e:e[0])
  if n:
   # Unguarded: the minimum sits before `first` and bounds every backward walk.
   rest=items[:];rest.remove(srt[0]);rng.shuffle(rest)
   _,mem,_=both(UNG_AUX,ARR+12,ARR+12*n,[D(0),D(0)],{ARR:pack([srt[0]]+rest)})
   assert keys(mem)==sorted(e[0] for e in items) and sorted(unpack(mem[:12*n]))==sorted(items);cases+=1
   val=(rng.choice(ks),7,9);base=[e for e in srt if e[0]<=val[0]][:1]
   if base:
    rest=sorted(srt[1:],key=lambda e:e[0]);_,mem,_=both(UNG_LINEAR,ARR+12*(len(rest)+1),0,[pack([val]),D(0)],{ARR:pack([base[0]]+rest)})
    k=keys(mem,len(rest)+2);assert k==sorted([base[0][0]]+[e[0] for e in rest]+[val[0]]) and val in unpack(mem[:12*(len(rest)+2)]);cases+=1
  for off in (0,1):
   eax,mem,out=both(COPY_BACK,ARR,ARR+12*n,[D(OUT+12*(n+off)),D(TAG),D(0)],{ARR:pack(items)});assert eax==OUT+12*off and unpack(out[12*off:12*(off+n)])==items;cases+=1
  if n:
   heap=sorted(items,key=lambda e:-e[0]);hole=n-1;value=(rng.choice(ks),1,2)
   _,mem,_=both(PUSH,ARR,hole,[D(0),pack([value]),D(0)],{ARR:pack(heap[:hole])+pack([(0,0,0)])});assert is_heap(keys(mem)) and sorted(unpack(mem[:12*n]))==sorted(heap[:hole]+[value]);cases+=1
   _,mem,_=both(ADJUST,ARR,0,[D(n),pack([value]),D(0)],{ARR:pack(heap)});k=keys(mem);assert is_heap(k) and sorted(unpack(mem[:12*n]))==sorted(heap[1:]+[value]);cases+=1
  srt=sorted({e[0]:e for e in items}.values())
  for probe in [0,1,2,0x7fffffff,0xffffffff]+[e[0] for e in srt[:5]]:
   for entry in (LOWER,LOWER_CONST):
    memory={ARR:pack(srt),MAP:D(ARR)+D(ARR+12*len(srt))+D(ARR+12*len(srt)),KEY:D(probe)}
    eax,_,_=both(entry,MAP,0,[D(KEY)],memory);assert eax==ARR+12*bisect.bisect_left([e[0] for e in srt],probe);cases+=1
 return cases
