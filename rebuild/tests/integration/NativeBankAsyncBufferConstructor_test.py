"""Independent caller-buffer constructor model; registration and locking interfaces are controlled."""
import hashlib,itertools,json,struct,sys
from pathlib import Path
from unicorn import Uc,UC_ARCH_X86,UC_MODE_32,UC_HOOK_CODE
from unicorn.x86_const import *
R=Path(__file__).resolve().parents[3];sys.path.insert(0,str(R/'tools/decomp_pipeline'))
from pe_oracle import EXE,read_virtual_data
ENTRY=0x9d5d30;BASE=0x99a2f0;LOCK=0xa62920;UNLOCK=0xa629a0;REGISTER=0x9d7cf0
A=0x2000000;N=32768;S=0x3000000;STOP=0x4000000
raw=EXE.read_bytes();root=read_virtual_data(raw,ENTRY,179);assert 'COMPILED_ROOT' in globals() and 0<len(COMPILED_ROOT)<=384;base=read_virtual_data(raw,BASE,9)
assert hashlib.sha256(root).hexdigest()=='03cb53912eee0156a05317e389fac2f68bb61ce9378cb5aa296f5173cc86ed61'
assert hashlib.sha256(base).hexdigest()=='b061da723fd24f053148a6c2e12667f2da7ee30f88c896c3a732f6e8f5495c11'
inputs=(Path(__file__),R/'tools/decomp_pipeline/pe_oracle.py')
def hashes():return {str(p.relative_to(R)):hashlib.sha256(p.read_bytes()).hexdigest() for p in inputs}
before=hashes()
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
def callback(m,c,name,receiver,args,events,snaps):
 events.append((name,receiver,tuple(args)));snaps.append(m.snap());d=c['data'];mode=c['mutation']
 if name=='lock':
  if mode==1:m.put(d+40,c['bank2']);m.put(d+36,0x87654321)
 elif name=='register':
  if mode==2:
   for off in (8,12,16,20,28,36,40,64):m.put(d+off,0xdead0000+off)
   m.write(d+61,b'ABC')
  elif mode==3:m.put(d+36,0xffffffff)
 elif name=='unlock':
  if mode==4:m.put(d+36,0x80000000);m.put(d+40,c['bank2'])
 else:raise AssertionError(name)
 return 0xabcdef00
def model(initial,c):
 m=Memory(bytearray(initial));d=c['data'];events=[];snaps=[]
 # Base constructor publication is observed separately from final vtable writes.
 m.put(d,0x129a7c4);snaps.append(m.snap())
 for off,val in ((4,0x129b860),(16,c['length']),(20,0),(40,c['bank']),(0,0x129b8b4),(4,0x129b8a0),(8,c['buffer']),(12,c['buffer']),(24,0),(28,0),(32,0xffffffff),(44,0),(48,0),(52,0),(56,0)):
  m.put(d+off,val)
 m.write(d+60,b'\x01');m.put(d+64,3);m.write(d+68,bytes(4))
 callback(m,c,'lock',0,(m.w(d+40)+0x14c,0),events,snaps)
 callback(m,c,'register',m.w(d+40)+0x128,(d,),events,snaps)
 m.put(d+36,0)
 callback(m,c,'unlock',0,(),events,snaps);snaps.append(m.snap())
 return events,snaps,d
u=Uc(UC_ARCH_X86,UC_MODE_32)
for page in {p&~4095 for p in (ENTRY,BASE,LOCK,UNLOCK,REGISTER)}:u.mem_map(page,4096)
u.mem_map(A,N);u.mem_map(S,65536);u.mem_map(STOP,4096);u.mem_write(ENTRY,root);u.mem_write(BASE,base)
active=[None];u.hook_add(UC_HOOK_CODE,lambda uc,pc,n,_:active[0](uc,pc));cpu=u.context_save();regs=(UC_X86_REG_EBX,UC_X86_REG_EBP,UC_X86_REG_ESI,UC_X86_REG_EDI)
execution_hits={mode:{hex(p):0 for p in (ENTRY,BASE)} for mode in ('original','compiled')};execution_mode=[None]
def run(initial,c):
 u.context_restore(cpu);u.mem_write(A,initial);u.mem_write(S,bytes(65536));m=Memory(u=u);events=[];snaps=[];lock=[];sp=S+0x8000
 u.mem_write(sp,word(STOP)+b''.join(word(c[k]) for k in ('buffer','length','bank')));u.reg_write(UC_X86_REG_ESP,sp);u.reg_write(UC_X86_REG_ECX,c['data'])
 for i,r in enumerate(regs):u.reg_write(r,0x11223300+i)
 def hook(uc,pc):
  if pc in (ENTRY,BASE):execution_hits[execution_mode[0]][hex(pc)]+=1
  if pc==STOP:uc.emu_stop();return
  if pc==BASE+8:snaps.append(m.snap());return
  if pc not in (LOCK,REGISTER,UNLOCK):return
  stack=uc.reg_read(UC_X86_REG_ESP);selfp=uc.reg_read(UC_X86_REG_ECX)
  if pc==LOCK:
   args=struct.unpack('<II',uc.mem_read(stack+4,8));assert args[1]==0;name='lock';receiver=0;cleanup=8;lock.append((selfp,args[0]))
   uc.mem_write(selfp,word(args[0]));uc.mem_write(selfp+4,b'\x01')
  elif pc==REGISTER:
   resultp,valuep=struct.unpack('<II',uc.mem_read(stack+4,8));assert struct.unpack('<I',uc.mem_read(valuep,4))[0]==c['data']
   args=(c['data'],);name='register';receiver=selfp;cleanup=8
   # Constructor ignores both inserted=true and inserted=false results.
   uc.mem_write(resultp,word(0xfeed1234)+bytes([0 if c['mutation']==3 else 1]))
  else:
   assert len(lock)==1 and selfp==lock[0][0] and bytes(uc.mem_read(selfp,4))==word(lock[0][1])
   name='unlock';receiver=0;args=();cleanup=0
  result=callback(m,c,name,receiver,args,events,snaps);ret=struct.unpack('<I',uc.mem_read(stack,4))[0]
  uc.reg_write(UC_X86_REG_EAX,result);uc.reg_write(UC_X86_REG_ECX,0xabcdef01);uc.reg_write(UC_X86_REG_EDX,0x98765432);uc.reg_write(UC_X86_REG_ESP,stack+4+cleanup);uc.reg_write(UC_X86_REG_EIP,ret)
 active[0]=hook;u.emu_start(ENTRY,STOP,count=500)
 assert u.reg_read(UC_X86_REG_EIP)==STOP and u.reg_read(UC_X86_REG_ESP)==sp+16
 assert [u.reg_read(r) for r in regs]==[0x11223300+i for i in range(4)]
 snaps.append(m.snap());return events,snaps,u.reg_read(UC_X86_REG_EAX)
cases=points=0
for length,buffer,place,fill,mutation in itertools.product((0,1,0x7fffffff,0x80000000,0xffffffff),(0,1,0x2006000,0x7fffffff,0x80000000,0xffffffff),range(2),(0,0x95),range(5)):
 initial=bytes([fill])*N;origin=A+place*0x3000;c=dict(data=origin+0x1000,bank=origin+0x200,bank2=origin+0x800,length=length,buffer=buffer,mutation=mutation)
 expected=model(initial,c)
 execution_mode[0]='original';u.mem_write(ENTRY,root);u.ctl_remove_cache(ENTRY,ENTRY+len(root));actual=run(initial,c);assert actual==expected,(c,actual[0],expected[0])
 execution_mode[0]='compiled';u.mem_write(ENTRY,COMPILED_ROOT);u.ctl_remove_cache(ENTRY,ENTRY+len(COMPILED_ROOT));compiled=run(initial,c);assert compiled==expected,(c,compiled[0],expected[0]);cases+=1;points+=len(expected[1])
assert before==hashes()
assert all(count==cases for row in execution_hits.values() for count in row.values()),execution_hits
report=dict(status='ORIGINAL_COMPILED_ASYNC_BUFFER_CONSTRUCTOR_MODEL_PASS',cases=cases,snapshots=points,root_sha256=hashlib.sha256(root).hexdigest(),base_sha256=hashlib.sha256(base).hexdigest(),original_exe_sha256=hashlib.sha256(raw).hexdigest(),input_sha256_before=before,input_sha256_after=hashes(),boundary='Actual original179B caller-buffer constructor and9B base constructor. Lock constructor/destructor and registration9D7CF0 are explicit mutating interfaces, not donor-container implementations. Full32KiB arena including72B object padding observed at base/lock/registration/unlock/final; stack excluded except lock receiver/registration pointer ABI validation. Three rawDWORD args, RET12, callee-save registers and original-this return checked. No actual concurrency, source compile, promotion or new provider credit.')
report.update(execution_hits=execution_hits,executable_cache_invalidated=True)
print(report['status'],cases,points,execution_hits,flush=True)
