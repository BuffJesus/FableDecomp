"""Original/rebuilt CBankFile::PackRuntimeData over the real packed-array bodies."""
import struct,hashlib,itertools
from unicorn import Uc,UC_ARCH_X86,UC_MODE_32,UC_HOOK_CODE
from unicorn.x86_const import *
ENTRY=0x9cd740;DEALLOCATE=0x9d3d90;NEW=0xbfeb22;DELETE=0xbfeb1c
HELPERS=((0xa62ae0,130),(0xa62b70,285),(0xa62c90,238),(0xa62d80,152),(0xa629f0,101),(0xa62a60,22),(0xa629d0,29),(0x13979e8,132))
BANK=0x2000000;HEAP=0x2100000;DATA=0x2200000;STOP=0x2300000;STACK=0x3000000;PACKED=(0x38,0x48,0x58,0x68)
def helpers(image,read):return {a:read(image,a,n) for a,n in HELPERS}
def record(i,mode):
 if mode==0:return 0,0,0,0
 if mode==1:return i*16,i,i&3,1
 if mode==2:return (i*0x9e3779b9)^0x5a5a1234,(i*0x85ebca6b)&0xffffffff,(i*37)&0xff,i&1
 if mode==3:return 0x1000,0x200,7,1
 return (0xffffff00+i if i&1 else 0),(0x80000000 if i&1 else 1),(0xff if i&1 else 0),(i%3==0)
def summary(values):
 bias=min(values) if values else 0;combined=0
 for v in values:combined|=v-bias
 bits=combined.bit_length();words=(bits*len(values)+31)//32;packed=0
 for i,v in enumerate(values):packed|=(v-bias)<<(i*bits)
 return bias,bits,words,[(packed>>(32*i))&0xffffffff for i in range(words)]
def execute(codes,count,extra,mode,old):
 u=Uc(UC_ARCH_X86,UC_MODE_32)
 for a,n in ((0x9cd000,0x1000),(0x9d3000,0x1000),(0xa62000,0x1000),(0x1397000,0x1000),(0xbfe000,0x1000),(BANK,0x1000),(HEAP,0x10000),(DATA,0x1000),(STOP,0x1000),(STACK,0x10000)):u.mem_map(a,n)
 def get(a):return int.from_bytes(u.mem_read(a,4),'little')
 def put(a,v):u.mem_write(a,struct.pack('<I',v&0xffffffff))
 for a,body in codes.items():u.mem_write(a,body)
 for a in (NEW,DELETE,DEALLOCATE):u.mem_write(a,b'\xc3')
 u.mem_write(HEAP,b'\x7d'*0x10000);u.mem_write(BANK,b'\x6b'*0x1000);u.mem_write(DATA,b'\x59'*0x1000);allocated={};freed=set();events=[]
 def allocate(n):
  assert 0<n<=1024 and len(allocated)<16;result=HEAP+2048*len(allocated)+16;allocated[result]=n;events.append(('new',n,result-HEAP));return result
 def hook(u,a,size,user):
  if a not in (NEW,DELETE,DEALLOCATE):return
  sp=u.reg_read(UC_X86_REG_ESP);arg=get(sp+4);result=0;pop=4
  if a==NEW:result=allocate(arg)
  elif a==DELETE:
   events.append(('delete',arg-HEAP if arg else 0))
   if arg:
    assert arg in allocated and arg not in freed;n=allocated[arg];assert bytes(u.mem_read(arg-16,16))==bytes(u.mem_read(arg+n,16))==b'\x7d'*16;freed.add(arg)
  else:
   ecx=u.reg_read(UC_X86_REG_ECX);assert ecx==BANK+0x20;events.append(('deallocate-runtime',));put(BANK+0x20,0);put(BANK+0x24,0);put(BANK+0x28,0);pop=0
  u.reg_write(UC_X86_REG_EAX,result);u.reg_write(UC_X86_REG_ESP,sp+4);u.reg_write(UC_X86_REG_EIP,get(sp))
 u.hook_add(UC_HOOK_CODE,hook);capacity=count+extra
 rows=[record(i,mode) for i in range(count)]
 for i,(offset,size,kind,valid) in enumerate(rows):u.mem_write(DATA+12*i,struct.pack('<IIBB',offset&0xffffffff,size&0xffffffff,kind,valid)+b'\xee\xee')
 put(BANK+0x20,DATA if capacity else 0);put(BANK+0x24,DATA+12*count if capacity else 0);put(BANK+0x28,DATA+12*capacity if capacity else 0)
 for j,o in enumerate(PACKED):put(BANK+o,allocate(8) if old else 0);put(BANK+o+4,0x76543210+j);u.mem_write(BANK+o+8,bytes([0xf0+j]));put(BANK+o+12,0xa5a5a5a5)
 before=bytes(u.mem_read(BANK,0x1000));data_before=bytes(u.mem_read(DATA,0x1000))
 registers=(UC_X86_REG_EBX,UC_X86_REG_ESI,UC_X86_REG_EDI,UC_X86_REG_EBP);sp=STACK+0x8000;put(sp,STOP)
 for j,r in enumerate(registers):u.reg_write(r,0x889900+j)
 u.reg_write(UC_X86_REG_ECX,BANK);u.reg_write(UC_X86_REG_ESP,sp);u.emu_start(ENTRY,STOP,count=200000)
 assert u.reg_read(UC_X86_REG_EIP)==STOP and u.reg_read(UC_X86_REG_ESP)==sp+4 and all(u.reg_read(r)==0x889900+j for j,r in enumerate(registers))
 after=bytes(u.mem_read(BANK,0x1000));assert bytes(u.mem_read(DATA,0x1000))==data_before
 assert after[:0x20]==before[:0x20] and after[0x2c:0x38]==before[0x2c:0x38] and after[0x78:]==before[0x78:]
 if count<=1:
  assert after==before and not [e for e in events[4 if old else 0:]];return dict(branch='none',events=events,bank=hashlib.sha256(after).hexdigest())
 fields=[[r[k]&0xffffffff for r in rows[1:]] for k in range(4)]
 total=sum(16+4*summary(values)[2] for values in fields);kept=total<(capacity+1)*12;deallocated=('deallocate-runtime',) in events;assert deallocated==kept
 for j,(o,values) in enumerate(zip(PACKED,fields)):
  if not kept:break
  bias,bits,words,expected=summary(values)
  assert get(BANK+o+4)==count-1 and after[o+8]==bits and get(BANK+o+12)==bias and after[o+9:o+12]==before[o+9:o+12]
  pointer=get(BANK+o)
  if words:
   assert pointer in allocated and allocated[pointer]==4*words
   if bits<32:assert [get(pointer+4*i) for i in range(words)]==expected
  else:assert pointer==0
 if kept:
  assert get(BANK+0x20)==get(BANK+0x24)==get(BANK+0x28)==0
  for j,o in enumerate(PACKED):
   if after[o+8]==32:continue
   for i in range(count-1):
    u.reg_write(UC_X86_REG_ECX,BANK+o);put(sp,STOP);put(sp+4,i);u.reg_write(UC_X86_REG_ESP,sp);u.emu_start(0xa629f0,STOP,count=1000);assert u.reg_read(UC_X86_REG_EAX)==fields[j][i]&0xffffffff
 else:
  assert after[0x20:0x2c]==before[0x20:0x2c]
  for o in PACKED:assert get(BANK+o)==get(BANK+o+4)==get(BANK+o+12)==0 and after[o+8]==0
  assert set(allocated)==freed
 return dict(branch='packed' if kept else 'runtime',total=total,events=events,bank=hashlib.sha256(after).hexdigest(),heap=hashlib.sha256(u.mem_read(HEAP,0x10000)).hexdigest())
def cases():return itertools.product((0,1,2,3,4,9,33,100),(0,1,3,60),range(5),range(2))
def compare(candidate,original):
 total=0;branches={}
 for args in cases():
  a=execute(candidate,*args);z=execute(original,*args);assert a==z,args;total+=1;branches[a['branch']]=branches.get(a['branch'],0)+1
 assert all(branches.get(k) for k in ('none','packed','runtime')),branches
 return total,branches
