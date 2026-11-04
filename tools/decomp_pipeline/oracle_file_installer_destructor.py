"""Full installer retail destructor with recorded dependencies and full-region model."""
import itertools,json,struct
from pathlib import Path
from unicorn import Uc,UC_ARCH_X86,UC_MODE_32,UC_HOOK_CODE
from unicorn.x86_const import UC_X86_REG_ECX,UC_X86_REG_EDX,UC_X86_REG_ESP,UC_X86_REG_EIP,UC_X86_REG_EAX
from pe_oracle import EXE,pe_sections
BASE=0x2004000;WIDTH=0x8000;ROOT=Path(__file__).resolve().parents[2]
SIGNAL=0x200d000;SLEEP=SIGNAL+0x100;CLOSE=SIGNAL+0x200;DELETECS=SIGNAL+0x300
def put(s,a,v):struct.pack_into('<I',s,a-BASE,v&0xffffffff)
def get(s,a):return struct.unpack_from('<I',s,a-BASE)[0]
def initial(waits,first,trees,mode):
 s=bytearray(b'\xa5'*WIDTH)
 for off,value in [(0x1c,BASE+0x6000),(0x20,BASE+0x6010),(0x24,BASE+0x6020),(0x28,0x1111),(0x2c,0x2222),(0x38,BASE+0x1000),(0x3c,BASE+0x2000),(0x40,BASE+0x3000),(0x44,BASE+0x4000),(0x48,trees&1),(0x94,BASE+0x5000),(0x98,(trees>>1)&1)]:put(s,BASE+off,value)
 for i in range(4):put(s,BASE+0x84+i*4,0x8000+i)
 for h in [BASE+i*0x1000 for i in range(1,6)]:put(s,h,h+0x100);put(s,h+4,h+0x200)
 s[0x78]=1 if not waits or first==0 else 0;s[0x79]=1 if not waits or first==1 else 0
 return s
def model(args):
 waits,first,trees,mode=args;s=initial(*args);put(s,BASE,0x129b43c);events=[]
 def event(kind,*values):events.append((kind,tuple(values),bytes(s)))
 event('lockctor',BASE+4,0);event('clear',BASE)
 for off in [0x1c,0x20,0x24,0x28,0x2c]:put(s,BASE+off,0)
 s[0x7a]=1;event('signal',get(s,BASE+0x84))
 if mode==1:put(s,BASE+0x88,0x9999)
 event('signal',get(s,BASE+0x88))
 for i in range(waits):
  event('release');event('sleep',0)
  if i+1==waits:s[0x78]=s[0x79]=1
  event('lock')
 for i in range(4):
  event('close',get(s,BASE+0x84+i*4))
  if mode==1 and i==0:put(s,BASE+0x88,0xaaaa)
 event('lockdtor');event('deletecs',BASE+4)
 for off,size,kind in [(0x94,0x98,'alias'),(0x44,0x48,'completed')]:
  if get(s,BASE+size):
   h=get(s,BASE+off);event(kind,BASE+off,get(s,h+4))
   if mode==2 and kind=='alias':put(s,BASE+off,BASE+0x7000)
   h=get(s,BASE+off);put(s,h+8,h);put(s,h+4,0);put(s,h+12,h);put(s,BASE+size,0)
  h=get(s,BASE+off)
  if h:event('free',h)
 for off,kind in [(0x40,'active'),(0x3c,'counted'),(0x38,'counted')]:
  event(kind,BASE+off);h=get(s,BASE+off);put(s,h,h);put(s,h+4,h);event('free',h)
 event('range',get(s,BASE+0x1c),get(s,BASE+0x20))
 if mode==2:put(s,BASE+0x1c,BASE+0x7100)
 if get(s,BASE+0x1c):event('free',get(s,BASE+0x1c))
 put(s,BASE,0x129b414)
 return events,bytes(s)
def main():
 raw=EXE.read_bytes();uc=Uc(UC_ARCH_X86,UC_MODE_32);uc.mem_map(0x400000,0x1200000)
 for _,va,vs,offset,size in pe_sections(raw):
  if size:uc.mem_write(0x400000+va,raw[offset:offset+size])
 uc.mem_map(0x2000000,0x20000);stack=0x2010000;stop=0x200f000
 def read(a):return struct.unpack('<I',bytes(uc.mem_read(a,4)))[0]
 def word(a,v):uc.mem_write(a,struct.pack('<I',v))
 for a,v in [(0x143fde0,SIGNAL),(0x143fe1c,SLEEP),(0x143fe30,CLOSE),(0x143fde4,DELETECS)]:word(a,v)
 kinds={0xa62920:('lockctor',8),0xa62960:('release',0),0xa62980:('lock',0),0xa629a0:('lockdtor',0),0x9c5110:('clear',0),SIGNAL:('signal',4),SLEEP:('sleep',4),CLOSE:('close',4),DELETECS:('deletecs',4),0x9aa830:('alias',4),0x4aba70:('completed',4),0x9c71a0:('active',0),0x9c6cc0:('counted',0),0x9c6d60:('range',4),0xbfea14:('free',0)}
 expected=[];position=sleeps=signals=closes=snapshots=0;args=()
 def hook(machine,address,size,user):
  nonlocal position,sleeps,signals,closes,snapshots
  if address not in kinds:return
  kind,pop=kinds[address];sp=machine.reg_read(UC_X86_REG_ESP);this=machine.reg_read(UC_X86_REG_ECX)
  values=()
  if kind=='lockctor':values=(read(sp+4),read(sp+8))
  elif kind in ['signal','sleep','close','deletecs','free']:values=(read(sp+4),)
  elif kind in ['alias','completed']:values=(this,read(sp+4))
  elif kind in ['clear','active','counted']:values=(this,)
  elif kind=='range':values=(this,machine.reg_read(UC_X86_REG_EDX))
  state=bytes(machine.mem_read(BASE,WIDTH));assert position<len(expected)and(kind,values,state)==expected[position],(args,position,kind,values,'snapshot mismatch')
  position+=1;snapshots+=1;mode=args[3]
  if kind=='clear':
   for off in [0x1c,0x20,0x24,0x28,0x2c]:word(BASE+off,0)
  if kind=='signal':
   if mode==1 and signals==0:word(BASE+0x88,0x9999)
   signals+=1
  if kind=='sleep':
   sleeps+=1
   if sleeps==args[0]:machine.mem_write(BASE+0x78,b'\1\1')
  if kind=='close':
   if mode==1 and closes==0:word(BASE+0x88,0xaaaa)
   closes+=1
  if kind=='alias'and mode==2:word(BASE+0x94,BASE+0x7000)
  if kind in ['active','counted']:h=read(this);word(h,h);word(h+4,h)
  if kind=='range'and mode==2:word(BASE+0x1c,BASE+0x7100)
  machine.reg_write(UC_X86_REG_EAX,1);machine.reg_write(UC_X86_REG_EIP,read(sp));machine.reg_write(UC_X86_REG_ESP,sp+4+pop)
 uc.hook_add(UC_HOOK_CODE,hook);cases=0
 for args in itertools.product([0,1,3],range(2),range(4),range(3)):
  expected,after=model(args);position=sleeps=signals=closes=0;s=initial(*args);put(s,BASE,0x129b43c);uc.mem_write(BASE,bytes(s));word(stack,stop)
  uc.reg_write(UC_X86_REG_ESP,stack);uc.reg_write(UC_X86_REG_ECX,BASE);uc.emu_start(0x9c5170,stop,count=10000)
  assert uc.reg_read(UC_X86_REG_EIP)==stop and uc.reg_read(UC_X86_REG_ESP)==stack+4,args
  assert position==len(expected)and bytes(uc.mem_read(BASE,WIDTH))==after,args
  snapshots+=1;cases+=1
 report=dict(cases=cases,snapshots=snapshots,full_region_bytes=WIDTH,boundary='Original retail destructor with recorded lock/OS/container/buffer dependencies; independently modeled full memory snapshots. Wait0/1/3, either worker initially finished, tree empty/nonempty, callback handle/head/buffer replacement. No real worker, lock or heap lifecycle claim.')
 (ROOT/'rebuild/compile-gate/installer-destructor-retail-oracle.json').write_text(json.dumps(report,indent=2)+'\n');print('INSTALLER_DESTRUCTOR_RETAIL_OK cases=%u snapshots=%u'%(cases,snapshots))
if __name__=='__main__':main()
