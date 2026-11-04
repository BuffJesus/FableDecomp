"""Retail active-file cleanup compared with independently predicted memory states."""
import itertools,json,struct
from pathlib import Path
from unicorn import Uc,UC_ARCH_X86,UC_MODE_32,UC_HOOK_CODE
from unicorn.x86_const import UC_X86_REG_ECX,UC_X86_REG_ESP,UC_X86_REG_EIP
from pe_oracle import EXE,pe_sections
BASE=0x2004000;WIDTH=0x8000;HEAD=BASE+0x100;CALLBACK=0x200d000
ROOT=Path(__file__).resolve().parents[2]
def node(i):return BASE+0x1000+i*0x100
def info(i):return BASE+0x2000+i*0x100
def replacement(i):return BASE+0x3000+i*0x100
def transfer(i,j):return BASE+0x4000+i*0x800+j*0x400
def put(s,a,v):struct.pack_into('<I',s,a-BASE,v&0xffffffff)
def get(s,a):return struct.unpack_from('<I',s,a-BASE)[0]
def initial(count,members,refcase,mode):
 s=bytearray(b'\xa5'*WIDTH);put(s,BASE,HEAD);put(s,HEAD,node(0)if count else HEAD);put(s,HEAD+4,node(count-1)if count else HEAD)
 for i in range(count):
  for a,v in [(node(i),node(i+1)if i+1<count else HEAD),(node(i)+4,node(i-1)if i else HEAD),(node(i)+0x24,0x5000+i*16),(node(i)+0x28,info(i)if refcase else 0),(info(i),[0,0,1,2,0xffffffff][refcase]),(info(i)+4,CALLBACK),(info(i)+8,0x4000+i*16),(replacement(i),1),(replacement(i)+4,CALLBACK),(replacement(i)+8,0x8000+i*16)]:put(s,a,v)
  for j,offset in enumerate((0x9c,0x98)):
   h=transfer(i,j);put(s,node(i)+offset,h)
   links=[h]+[h+0x100*(k+1)for k in range(members)]
   for k,a in enumerate(links):put(s,a,links[(k+1)%len(links)]);put(s,a+4,links[k-1])
 return s
def model(count,members,refcase,mode):
 s=initial(count,members,refcase,mode);events=[]
 for i in range(count):
  for j in range(2):
   h=transfer(i,j)
   for k in range(members):events.append((1,h+0x100*(k+1),bytes(s)))
   put(s,h,h);put(s,h+4,h);events.append((1,h,bytes(s)))
  events.append((2,node(i)+0x40,bytes(s)))
  if mode==1:put(s,node(i),0)
  if mode==2:put(s,node(i)+0x28,replacement(i))
  control=get(s,node(i)+0x28)
  if control:
   remaining=(get(s,control)-1)&0xffffffff;put(s,control,remaining)
   if remaining==0:
    events.append((3,get(s,control+8),bytes(s)));events.append((4,control,bytes(s)))
  put(s,node(i)+0x24,0);put(s,node(i)+0x28,0);events.append((1,node(i),bytes(s)))
 put(s,HEAD,HEAD);put(s,HEAD+4,HEAD)
 return events,bytes(s)
def main():
 raw=EXE.read_bytes();uc=Uc(UC_ARCH_X86,UC_MODE_32);uc.mem_map(0x400000,0x1200000)
 for _,va,vs,offset,size in pe_sections(raw):
  if size:uc.mem_write(0x400000+va,raw[offset:offset+size])
 uc.mem_map(0x2000000,0x20000);stack=0x2010000;stop=0x200f000
 expected=[];position=0;args=();snapshots=0
 def read(a):return struct.unpack('<I',bytes(uc.mem_read(a,4)))[0]
 def word(a,v):uc.mem_write(a,struct.pack('<I',v))
 def hook(machine,address,size,user):
  nonlocal position,snapshots
  kinds={0xbfea14:1,0xa3acc0:2,CALLBACK:3,0xbfe9bc:4}
  if address not in kinds:return
  kind=kinds[address];sp=machine.reg_read(UC_X86_REG_ESP)
  arg=machine.reg_read(UC_X86_REG_ECX)if kind in (2,3)else read(sp+4)
  state=bytes(machine.mem_read(BASE,WIDTH));assert position<len(expected),(args,'extra boundary')
  assert (kind,arg,state)==expected[position],(args,position,kind,hex(arg),'snapshot mismatch')
  position+=1;snapshots+=1
  if kind==2:
   n=arg-0x40;i=(n-node(0))//0x100
   if args[3]==1:word(n,0)
   if args[3]==2:word(n+0x28,replacement(i))
  machine.reg_write(UC_X86_REG_EIP,read(sp));machine.reg_write(UC_X86_REG_ESP,sp+4)
 uc.hook_add(UC_HOOK_CODE,hook);cases=0
 for args in itertools.product(range(3),range(3),range(5),range(3)):
  expected,after=model(*args);position=0;uc.mem_write(BASE,bytes(initial(*args)));word(stack,stop)
  uc.reg_write(UC_X86_REG_ESP,stack);uc.reg_write(UC_X86_REG_ECX,BASE)
  uc.emu_start(0x9c71a0,stop,count=10000)
  assert uc.reg_read(UC_X86_REG_EIP)==stop and uc.reg_read(UC_X86_REG_ESP)==stack+4,args
  assert position==len(expected)and bytes(uc.mem_read(BASE,WIDTH))==after,args
  snapshots+=1;cases+=1
 report=dict(cases=cases,full_region_bytes=WIDTH,snapshots=snapshots,boundary='Retail EXE instructions versus independent full-region model. Releases and compressor/object destruction recorded; memory retained. Outer/nested counts0..2, absent/count0/1/2/ffffffff controls, compressor callback clears next or replaces file control. No real compressor lifetime claim.')
 (ROOT/'rebuild/compile-gate/installer-active-file-list-retail-oracle.json').write_text(json.dumps(report,indent=2)+'\n')
 print('INSTALLER_ACTIVE_LIST_RETAIL_OK cases='+str(cases)+' snapshots='+str(snapshots))
if __name__=='__main__':main()
