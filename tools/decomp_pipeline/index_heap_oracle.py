"""Execute the pair<unsigned long, long> heap helpers and __partial_sort against retail."""
import random,struct
from unicorn import Uc,UC_ARCH_X86,UC_MODE_32
from unicorn.x86_const import *
ARR=0x2000000;STACK=0x3000000;STOP=0x4000000
PARTIAL,MAKE,SORT_HEAP,ADJUST,PUSH=0x9b6b70,0x9b3c70,0x9b5bb0,0x9b2ff0,0x9b1d10
def pack(items):return b''.join(struct.pack('<Ii',*e) for e in items)
def unpack(raw):return [struct.unpack_from('<Ii',raw,i) for i in range(0,len(raw),8)]
def run(codes,entry,ecx,edx,stack,memory):
 u=Uc(UC_ARCH_X86,UC_MODE_32)
 for a,z in [(0x9b1000,0x9000),(ARR,0x100000),(STACK,0x10000),(STOP,0x1000)]:u.mem_map(a,z)
 for a,b in codes.items():u.mem_write(a,bytes(b))
 for a,b in memory.items():u.mem_write(a,b)
 sp=STACK+0x8000;u.mem_write(sp,struct.pack('<I',STOP)+b''.join(stack))
 regs=[UC_X86_REG_EBX,UC_X86_REG_ESI,UC_X86_REG_EDI,UC_X86_REG_EBP]
 for i,r in enumerate(regs):u.reg_write(r,0x778800+i)
 u.reg_write(UC_X86_REG_ECX,ecx);u.reg_write(UC_X86_REG_EDX,edx);u.reg_write(UC_X86_REG_ESP,sp);u.reg_write(UC_X86_REG_EAX,0xdeadbeef)
 u.emu_start(entry,STOP,count=3000000)
 assert u.reg_read(UC_X86_REG_ESP)==sp+4+sum(len(s) for s in stack) and all(u.reg_read(r)==0x778800+i for i,r in enumerate(regs)),hex(entry)
 return bytes(u.mem_read(ARR,0x10000))
D=lambda v:struct.pack('<I',v&0xffffffff)
def is_heap(keys):return all(keys[(i-1)//2]>=keys[i] for i in range(1,len(keys)))
def compare(candidate,native):
 rng=random.Random(0x1d3f);cases=0
 def both(entry,ecx,edx,stack,memory):
  c=run(candidate,entry,ecx,edx,stack,memory);assert c==run(native,entry,ecx,edx,stack,memory),hex(entry);return c
 for n in (2,3,15,16,17,33,64,200):
  for ks in (list(range(4)),list(range(1000)),[0,0xffffffff,0x80000000]):
   items=[(rng.choice(ks),i) for i in range(n)];keys=lambda raw,m=n:[e[0] for e in unpack(raw[:8*m])];want=sorted(e[0] for e in items)
   mem=both(MAKE,ARR,ARR+8*n,[D(0),D(0),D(0)],{ARR:pack(items)});assert is_heap(keys(mem)) and sorted(unpack(mem[:8*n]))==sorted(items)
   mem=both(SORT_HEAP,ARR,ARR+8*n,[D(0)],{ARR:mem[:8*n]});assert keys(mem)==want and sorted(unpack(mem[:8*n]))==sorted(items);cases+=2
   for middle in sorted({1,n//2,n}):
    mem=both(PARTIAL,ARR,ARR+8*middle,[D(ARR+8*n),D(0),D(0)],{ARR:pack(items)});assert keys(mem)[:middle]==want[:middle] and sorted(unpack(mem[:8*n]))==sorted(items);cases+=1
   heap=sorted(items,key=lambda e:-e[0]);value=(rng.choice(ks),-7)
   mem=both(PUSH,ARR,n-1,[D(0),pack([value]),D(0)],{ARR:pack(heap[:n-1])+pack([(0,0)])});assert is_heap(keys(mem)) and sorted(unpack(mem[:8*n]))==sorted(heap[:n-1]+[value])
   mem=both(ADJUST,ARR,0,[D(n),pack([value]),D(0)],{ARR:pack(heap)});assert is_heap(keys(mem)) and sorted(unpack(mem[:8*n]))==sorted(heap[1:]+[value]);cases+=2
 return cases
