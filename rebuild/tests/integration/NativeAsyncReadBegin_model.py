"""Independent Begin ownership/output model and immutable native-chain runners."""
import hashlib,itertools,json,re,struct,sys
from pathlib import Path
from unicorn import Uc,UC_ARCH_X86,UC_MODE_32,UC_HOOK_CODE
from unicorn.x86_const import *
R=Path(__file__).resolve().parents[3];sys.path.insert(0,str(R/'tools/decomp_pipeline'))
from pe_oracle import EXE,read_virtual_data
ENTRY=0x9d5440;LOCK=0xa62920;UNLOCK=0xa629a0;SETUP=0x9d52d0
TRY=0x6000000;LEAVE=0x6000100;BAD_ENTER=0x6000200
ITRY=0x143fd30;ILEAVE=0x143fe10;IENTER=0x143fe0c
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
def boundary(m,c,name,receiver,args,events,snaps):
 events.append((name,receiver,tuple(args)));snaps.append(m.snap());d=c['data'];mode=c['mutation']
 if name=='try':
  if mode in (1,3):m.put(d+40,c['bank2'])
  if mode in (2,3):m.put(d+12,0x87654321)
  return c['try_result']
 if name=='setup':
  if mode==2:m.put(d+12,0xffffffff)
  elif mode==3:m.put(d+12,0);m.put(d+40,c['bank2'])
  elif mode==4:m.put(d+12,0xdead1234);m.put(d+40,c['bank2']);m.write(d+68,b'\x95')
  elif mode==5:m.put(c['dst'],0xbad12345);m.put(d+12,0xfeed9876)
  return c['setup_result']
 if name=='leave':
  if mode==5:m.put(c['dst'],0xcafe1234);m.put(d+12,0x87654321);m.put(d+40,c['bank2'])
  return 0xabcdef00
 raise AssertionError(name)
def model(initial,c):
 m=Memory(bytearray(initial));events=[];snaps=[];locks=[];d=c['data'];section=m.w(d+40)+0x14c;pad=bytes([c['stackfill']])*3
 locks.append(word(section)+bytes([c['stackfill']])+pad)
 acquired=boundary(m,c,'try',0,(section,),events,snaps)!=0
 success=False
 if acquired:
  locks.append(word(section)+b'\x01'+pad)
  success=(boundary(m,c,'setup',d,(0,),events,snaps)&255)!=0
  if success:m.put(c['dst'],m.w(d+12))
  locks.append(word(section)+b'\x01'+pad)
  boundary(m,c,'leave',0,(section,),events,snaps)
 locks.append(word(section)+b'\x00'+pad);snaps.append(m.snap())
 return events,snaps,locks,int(success)
def build_runner(bodies, provider_hits, callback_hits):
 # One immutable code set per UC instance. Future compiled integration must
 # construct a second runner, never overwrite mixed code in a cached engine.
 u=Uc(UC_ARCH_X86,UC_MODE_32)
 for page in {x&~4095 for x in list(bodies)+[SETUP,TRY,LEAVE,BAD_ENTER,ITRY,ILEAVE,IENTER]}:u.mem_map(page,4096)
 u.mem_map(A,N);u.mem_map(S,65536);u.mem_map(STOP,4096)
 for address,body in bodies.items():u.mem_write(address,body)
 u.mem_write(ITRY,word(TRY));u.mem_write(ILEAVE,word(LEAVE));u.mem_write(IENTER,word(BAD_ENTER))
 cpu=u.context_save();active=[None];u.hook_add(UC_HOOK_CODE,lambda uc,pc,n,_:active[0](uc,pc))
 regs=(UC_X86_REG_EBX,UC_X86_REG_EBP,UC_X86_REG_ESI,UC_X86_REG_EDI)
 def run(initial,c):
  u.context_restore(cpu);u.mem_write(A,initial);u.mem_write(S,bytes([c['stackfill']])*65536)
  m=Memory(u=u);events=[];snaps=[];locks=[];sp=S+0x8000;lockp=[None];returns=[]
  u.mem_write(sp,word(STOP)+word(c['dst'])+word(c['priority']));u.reg_write(UC_X86_REG_ESP,sp);u.reg_write(UC_X86_REG_ECX,c['data'])
  for i,r in enumerate(regs):u.reg_write(r,0x11223300+i)
  def hook(uc,pc):
   if pc in bodies:provider_hits[hex(pc)]+=1
   if pc==STOP:uc.emu_stop();return
   stack=uc.reg_read(UC_X86_REG_ESP)
   if pc==LOCK:
    section,tryonly=struct.unpack('<II',uc.mem_read(stack+4,8));assert tryonly==1
    lockp[0]=uc.reg_read(UC_X86_REG_ECX);returns.append((struct.unpack('<I',uc.mem_read(stack,4))[0],stack,lockp[0],[uc.reg_read(r) for r in regs]))
   if returns and pc==returns[-1][0]:
    ret,oldstack,receiver,saved=returns.pop();assert uc.reg_read(UC_X86_REG_ESP)==oldstack+12 and uc.reg_read(UC_X86_REG_EAX)==receiver and [uc.reg_read(r) for r in regs]==saved
   if pc==UNLOCK:assert uc.reg_read(UC_X86_REG_ECX)==lockp[0]
   assert pc!=BAD_ENTER,'Outer try-only lock cannot call Enter'
   if pc not in (TRY,SETUP,LEAVE):return
   callback_hits[{TRY:'try',SETUP:'setup',LEAVE:'leave'}[pc]]+=1
   locks.append(bytes(uc.mem_read(lockp[0],8)))
   if pc==SETUP:
    receiver=uc.reg_read(UC_X86_REG_ECX);assert receiver==c['data'];args=(struct.unpack('<I',uc.mem_read(stack+4,4))[0],);assert args==(0,);name='setup'
   else:
    receiver=0;args=(struct.unpack('<I',uc.mem_read(stack+4,4))[0],);name='try' if pc==TRY else 'leave'
   result=boundary(m,c,name,receiver,args,events,snaps);ret=struct.unpack('<I',uc.mem_read(stack,4))[0]
   uc.reg_write(UC_X86_REG_EAX,result&0xffffffff);uc.reg_write(UC_X86_REG_ECX,0xabcdef01);uc.reg_write(UC_X86_REG_EDX,0x98765432);uc.reg_write(UC_X86_REG_ESP,stack+8);uc.reg_write(UC_X86_REG_EIP,ret)
  active[0]=hook;u.emu_start(ENTRY,STOP,count=1000)
  assert u.reg_read(UC_X86_REG_EIP)==STOP and u.reg_read(UC_X86_REG_ESP)==sp+12 and not returns
  assert [u.reg_read(r) for r in regs]==[0x11223300+i for i in range(4)]
  locks.append(bytes(u.mem_read(lockp[0],8)));snaps.append(m.snap())
  return events,snaps,locks,u.reg_read(UC_X86_REG_EAX)&255
 return run
def run_fixture(compiled_bodies):
 raw=EXE.read_bytes();original={ENTRY:read_virtual_data(raw,ENTRY,92),LOCK:read_virtual_data(raw,LOCK,54),UNLOCK:read_virtual_data(raw,UNLOCK,25)}
 assert set(compiled_bodies)==set(original)
 assert all(len(compiled_bodies[a])==len(original[a]) for a in original)
 assert hashlib.sha256(original[ENTRY]).hexdigest()=='7cae38736b407df31f76fff028003022eecb1445cfd452f8259d9ccaad17726a'
 original_provider={hex(a):0 for a in original};compiled_provider={hex(a):0 for a in original}
 original_callbacks={n:0 for n in ('try','setup','leave')};compiled_callbacks=original_callbacks.copy();expected_callbacks=original_callbacks.copy()
 originalrun=build_runner(original,original_provider,original_callbacks);compiledrun=build_runner(compiled_bodies,compiled_provider,compiled_callbacks)
 cases=ap=lp=successes=0
 for tr,sr,buffer,priority,dstmode,mutation,place,fill,stackfill in itertools.product((0,1,0x80000000),(0,1,0x100,0xffffffff),(0,0x12345678),(0,3,0xffffffff),range(3),range(6),range(2),(0,0x95),(0,0x95)):
  origin=A+place*0x3000;initial=bytearray([fill])*N;memory=Memory(initial);d=origin+0x1000;dst=(origin+0x1100,d+12,d+40)[dstmode]
  c=dict(data=d,bank=origin+0x200,bank2=origin+0x800,dst=dst,try_result=tr,setup_result=sr,priority=priority,stackfill=stackfill,mutation=mutation)
  memory.put(d+40,c['bank']);memory.put(d+12,buffer)
  expected=model(initial,c);actualoriginal=originalrun(bytes(initial),c);actualcompiled=compiledrun(bytes(initial),c)
  assert actualoriginal==expected,(c,'original');assert actualcompiled==expected,(c,'compiled')
  for name,receiver,args in expected[0]:expected_callbacks[name]+=1
  cases+=1;ap+=len(expected[1]);lp+=len(expected[2]);successes+=expected[3]
 assert (cases,ap,lp)==(10368,34560,34560)
 assert original_provider==compiled_provider=={hex(a):cases for a in original}
 assert original_callbacks==compiled_callbacks==expected_callbacks
 return dict(status='ORIGINAL_COMPILED_ASYNC_READ_BEGIN_MODEL_PASS',cases=cases,arena_snapshots=ap,lock_snapshots=lp,original_cases=cases,original_arena_snapshots=ap,original_lock_snapshots=lp,compiled_cases=cases,compiled_arena_snapshots=ap,compiled_lock_snapshots=lp,successes=successes,original_provider_hits=original_provider,compiled_provider_hits=compiled_provider,original_callback_hits=original_callbacks,compiled_callback_hits=compiled_callbacks,model_callback_counts=expected_callbacks,separate_immutable_uc_instances=True)
