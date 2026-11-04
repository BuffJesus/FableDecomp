"""Retail machine-code cleanup versus an independent full-region event model."""
import itertools,json,struct
from pathlib import Path
from unicorn import Uc,UC_ARCH_X86,UC_MODE_32,UC_HOOK_CODE
from unicorn.x86_const import UC_X86_REG_ECX,UC_X86_REG_ESP,UC_X86_REG_EIP
from pe_oracle import EXE,pe_sections
BASE=0x2004000;WIDTH=0x8000;HEAD=BASE+0x100;ALT=BASE+0x200;CALLBACK=0x200d000
ROOT=Path(__file__).resolve().parents[2]
def node(i):return BASE+0x1000+i*0x100
def info(i):return BASE+0x2000+i*0x100
def replacement(i):return BASE+0x3000+i*0x100
def put(state,address,value):struct.pack_into('<I',state,address-BASE,value&0xffffffff)
def get(state,address):return struct.unpack_from('<I',state,address-BASE)[0]
def initial(count,refcase,mode):
 s=bytearray(b'\xa5'*WIDTH);put(s,BASE,HEAD);put(s,HEAD,node(0)if count else HEAD);put(s,HEAD+4,node(count-1)if count else HEAD)
 put(s,ALT,0);put(s,ALT+4,0)
 for i in range(count):
  for address,value in [(node(i),node(i+1)if i+1<count else HEAD),(node(i)+4,node(i-1)if i else HEAD),(node(i)+8,0x5000+i*16),(node(i)+12,info(i)if refcase else 0),(info(i),[0,0,1,2,0xffffffff][refcase]),(info(i)+4,CALLBACK),(info(i)+8,0x4000+i*16),(replacement(i),77),(replacement(i)+4,CALLBACK),(replacement(i)+8,0x8888)]:put(s,address,value)
 return s
def model(count,refcase,mode):
 s=initial(count,refcase,mode);events=[]
 for i in range(count):
  if refcase:
   remaining=(get(s,info(i))-1)&0xffffffff;put(s,info(i),remaining)
   if remaining==0:
    events.append((1,0x4000+i*16,bytes(s)))
    if mode==1:put(s,node(i)+12,replacement(i))
    if mode==2:put(s,node(i),0)
    events.append((2,replacement(i)if mode==1 else info(i),bytes(s)))
    if mode==3:put(s,node(i)+8,0x9999);put(s,node(i)+12,0)
  put(s,node(i)+8,0);put(s,node(i)+12,0);events.append((3,node(i),bytes(s)))
  if mode==4 and i==0 and count>1:put(s,BASE,ALT);put(s,node(count-1),ALT)
 head=get(s,BASE);put(s,head,head);put(s,head+4,head)
 return events,bytes(s)
def main():
 raw=EXE.read_bytes();uc=Uc(UC_ARCH_X86,UC_MODE_32);uc.mem_map(0x400000,0x1200000)
 for _,va,vs,offset,size in pe_sections(raw):
  if size:uc.mem_write(0x400000+va,raw[offset:offset+size])
 uc.mem_map(0x2000000,0x20000);stack=0x2010000;stop=0x200f000
 expected=[];position=0;freed=0;args=();snapshots=0
 def read(a):return struct.unpack('<I',bytes(uc.mem_read(a,4)))[0]
 def word(a,v):uc.mem_write(a,struct.pack('<I',v))
 def hook(machine,address,size,user):
  nonlocal position,freed,snapshots
  if address not in (CALLBACK,0xbfe9bc,0xbfea14):return
  kind={CALLBACK:1,0xbfe9bc:2,0xbfea14:3}[address];sp=machine.reg_read(UC_X86_REG_ESP)
  arg=machine.reg_read(UC_X86_REG_ECX)if kind==1 else read(sp+4)
  state=bytes(machine.mem_read(BASE,WIDTH));assert position<len(expected),(args,kind,arg,'extra boundary')
  assert (kind,arg,state)==expected[position],(args,position,kind,hex(arg),'snapshot or boundary mismatch')
  position+=1;snapshots+=1;count,refcase,mode=args;i=freed
  if kind==1:
   if mode==1:word(node(i)+12,replacement(i))
   if mode==2:word(node(i),0)
  if kind==2 and mode==3:word(node(i)+8,0x9999);word(node(i)+12,0)
  if kind==3:
   if mode==4 and i==0 and count>1:word(BASE,ALT);word(node(count-1),ALT)
   freed+=1
  machine.reg_write(UC_X86_REG_EIP,read(sp));machine.reg_write(UC_X86_REG_ESP,sp+4)
 uc.hook_add(UC_HOOK_CODE,hook);cases=0
 for args in itertools.product(range(4),range(5),range(5)):
  expected,after=model(*args);position=freed=0;uc.mem_write(BASE,bytes(initial(*args)));word(stack,stop)
  uc.reg_write(UC_X86_REG_ESP,stack);uc.reg_write(UC_X86_REG_ECX,BASE)
  uc.emu_start(0x9c6cc0,stop,count=10000)
  assert uc.reg_read(UC_X86_REG_EIP)==stop and uc.reg_read(UC_X86_REG_ESP)==stack+4,args
  assert position==len(expected) and freed==args[0] and bytes(uc.mem_read(BASE,WIDTH))==after,args
  snapshots+=1;cases+=1
 report=dict(cases=cases,full_region_bytes=WIDTH,snapshots=snapshots,boundary='retail EXE instructions execute in Unicorn; object/control/node release recorded without reclaiming memory; independently predicted callbacks and full-region states; five mutation modes, counts0/1/2/ffffffff and nodecount0..3')
 (ROOT/'rebuild/compile-gate/installer-counted-list-retail-oracle.json').write_text(json.dumps(report,indent=2)+'\n')
 print('INSTALLER_COUNTED_LIST_RETAIL_OK cases='+str(cases)+' snapshots='+str(snapshots))
if __name__=='__main__':main()
