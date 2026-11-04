"""Independent uint32 relocation model and immutable actual-body executor."""
import hashlib,itertools,json,re,struct,sys
from pathlib import Path
from unicorn import Uc,UC_ARCH_X86,UC_MODE_32,UC_HOOK_CODE
from unicorn.x86_const import *
R=Path(__file__).resolve().parents[3];sys.path.insert(0,str(R/'tools/decomp_pipeline'))
from pe_oracle import EXE,read_virtual_data
ENTRY=0x9d5670;LOCK=0xa62920;UNLOCK=0xa629a0;ENTER=0x6000000;LEAVE=0x6000100;BADTRY=0x6000200
IENTER=0x143fe0c;ILEAVE=0x143fe10;ITRY=0x143fd30
A=0x2000000;N=32768;S=0x3000000;STOP=0x4000000
def word(v):return struct.pack('<I',v&0xffffffff)
class Memory:
 def __init__(self,b=None,u=None):self.b=b;self.u=u
 def read(self,p,n):
  assert A<=p and p+n<=A+N
  return bytes(self.b[p-A:p-A+n]) if self.b is not None else bytes(self.u.mem_read(p,n))
 def write(self,p,b):
  assert A<=p and p+len(b)<=A+N
  if self.b is not None:self.b[p-A:p-A+len(b)]=b
  else:self.u.mem_write(p,b)
 def w(self,p):return struct.unpack('<I',self.read(p,4))[0]
 def put(self,p,v):self.write(p,word(v))
 def snap(self):return self.read(A,N)
def boundary(m,c,name,section,events,snaps):
 events.append((name,section));snaps.append(m.snap());d=c['data'];mode=c['mutation']
 if name=='enter':
  if mode==1:m.put(d+40,c['bank2'])
  elif mode==2:m.put(d+12,0);m.put(d+8,0xffffffff)
  elif mode==3:m.put(d+12,0xffffffff);m.put(d+8,0)
  elif mode==4:
   buffer=m.w(d+12);data=m.w(d+8);m.put(d+12,data);m.put(d+8,buffer);m.put(d+40,c['bank2'])
  elif mode==5:m.put(d+12,c['old']);m.put(d+8,c['old']);m.write(d+68,b'\x95')
 elif name=='leave':
  if mode==4:m.put(d+12,0xdead1234);m.put(d+8,0);m.put(d+40,c['bank2'])
  elif mode==5:
   buffer=m.w(d+12);data=m.w(d+8);m.put(d+12,data);m.put(d+8,buffer);m.write(d+70,b'\x80')
 else:raise AssertionError(name)
 return 0xabcdef00
def model(initial,c):
 m=Memory(bytearray(initial));d=c['data'];events=[];snaps=[];locks=[];section=m.w(d+40)+0x14c;pad=bytes([c['stackfill']])*3
 locks.append(word(section)+bytes([c['stackfill']])+pad)
 boundary(m,c,'enter',section,events,snaps)
 buffer=m.w(d+12)
 if buffer:m.put(d+12,(buffer-c['old']+c['new'])&0xffffffff)
 snaps.append(m.snap())
 data=m.w(d+8)
 if data:m.put(d+8,(data-c['old']+c['new'])&0xffffffff)
 snaps.append(m.snap())
 locks.append(word(section)+b'\x01'+pad)
 boundary(m,c,'leave',section,events,snaps)
 locks.append(word(section)+b'\x00'+pad);snaps.append(m.snap())
 return events,snaps,locks
def build_runner(bodies):
 u=Uc(UC_ARCH_X86,UC_MODE_32)
 for page in {x&~4095 for x in list(bodies)+[ENTER,LEAVE,BADTRY,IENTER,ILEAVE,ITRY]}:u.mem_map(page,4096)
 u.mem_map(A,N);u.mem_map(S,65536);u.mem_map(STOP,4096)
 # Immutable machine code per runner; future compiled proof uses separate UC.
 for address,body in bodies.items():u.mem_write(address,body)
 u.mem_write(IENTER,word(ENTER));u.mem_write(ILEAVE,word(LEAVE));u.mem_write(ITRY,word(BADTRY))
 hits={hex(a):0 for a in bodies};phases={name:0 for name in ('enter','buffer_phase','data_phase','leave','final')};cpu=u.context_save();active=[None];u.hook_add(UC_HOOK_CODE,lambda uc,pc,n,_:active[0](uc,pc));regs=(UC_X86_REG_EBX,UC_X86_REG_EBP,UC_X86_REG_ESI,UC_X86_REG_EDI)
 def run(initial,c):
  u.context_restore(cpu);u.mem_write(A,initial);u.mem_write(S,bytes([c['stackfill']])*65536);m=Memory(u=u);events=[];snaps=[];locks=[];sp=S+0x8000;lockp=[None];returns=[]
  u.mem_write(sp,word(STOP)+b''.join(word(c[k]) for k in ('ignored1','old','new','ignored4')))
  u.reg_write(UC_X86_REG_ESP,sp);u.reg_write(UC_X86_REG_ECX,c['data']+4)
  for i,r in enumerate(regs):u.reg_write(r,0x11223300+i)
  def hook(uc,pc):
   if pc==STOP:uc.emu_stop();return
   if hex(pc) in hits:hits[hex(pc)]+=1
   stack=uc.reg_read(UC_X86_REG_ESP)
   if pc in (0x9d56a0,0x9d56ae):
    phases['buffer_phase' if pc==0x9d56a0 else 'data_phase']+=1;snaps.append(m.snap())
   if pc==LOCK:
    section,tryonly=struct.unpack('<II',uc.mem_read(stack+4,8));assert tryonly==0
    lockp[0]=uc.reg_read(UC_X86_REG_ECX);returns.append((struct.unpack('<I',uc.mem_read(stack,4))[0],stack,lockp[0],[uc.reg_read(r) for r in regs]))
   if returns and pc==returns[-1][0]:
    ret,oldstack,receiver,saved=returns.pop();assert uc.reg_read(UC_X86_REG_ESP)==oldstack+12 and uc.reg_read(UC_X86_REG_EAX)==receiver and [uc.reg_read(r) for r in regs]==saved
   if pc==UNLOCK:assert uc.reg_read(UC_X86_REG_ECX)==lockp[0]
   assert pc!=BADTRY,'Blocking relocation cannot call TryEnter'
   if pc not in (ENTER,LEAVE):return
   phases['enter' if pc==ENTER else 'leave']+=1
   locks.append(bytes(uc.mem_read(lockp[0],8)));section=struct.unpack('<I',uc.mem_read(stack+4,4))[0]
   result=boundary(m,c,'enter' if pc==ENTER else 'leave',section,events,snaps);ret=struct.unpack('<I',uc.mem_read(stack,4))[0]
   uc.reg_write(UC_X86_REG_EAX,result);uc.reg_write(UC_X86_REG_ECX,0xabcdef01);uc.reg_write(UC_X86_REG_EDX,0x98765432);uc.reg_write(UC_X86_REG_ESP,stack+8);uc.reg_write(UC_X86_REG_EIP,ret)
  active[0]=hook;u.emu_start(ENTRY,STOP,count=1000)
  assert u.reg_read(UC_X86_REG_EIP)==STOP and u.reg_read(UC_X86_REG_ESP)==sp+20 and not returns
  # Unicorn's end address stops before invoking its code hook. Count the
  # verified completed return here, not an unreachable STOP instruction.
  phases['final']+=1
  assert [u.reg_read(r) for r in regs]==[0x11223300+i for i in range(4)]
  locks.append(bytes(u.mem_read(lockp[0],8)));snaps.append(m.snap());return events,snaps,locks
 run.hits=hits;run.phases=phases
 return run
