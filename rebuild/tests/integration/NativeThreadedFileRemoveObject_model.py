"""Independent expected state/events and immutable original/provider machines."""
import hashlib,itertools,struct,sys
from pathlib import Path
from unicorn import Uc,UC_ARCH_X86,UC_MODE_32,UC_HOOK_CODE
from unicorn.x86_const import *
R=Path(__file__).resolve().parents[3];W=Path(__file__).parent;sys.path.insert(0,str(R/'tools/decomp_pipeline'))
from pe_oracle import EXE,read_virtual_data
"""Independent manager predicate/order draft; bounded child interfaces only."""
def expected_predicate(memory,manager,uid,call,observe):
 # call(name,receiver,args) may mutate full arena at every boundary.
 section=(manager+0x14c)&0xffffffff
 call('enter',0,(section,))
 objects=(manager+0x13c)&0xffffffff
 node=call('find',objects,(uid,))
 # End/header is live after lookup, not cached before its callback.
 if node!=memory.w(objects):
  obj=memory.w(node+0x14)
  if memory.w(obj+0x1c)==0:call('pending_remove',manager,(obj,))
  # State is reloaded after possible pending callback.
  if memory.w(obj+0x1c) in (1,2,3):
   call('leave',0,(section,));observe('final',memory.snap());return 0
  handle=memory.w(obj+0x20)
  if handle:call('global_free',0,(handle,))
  # Cached object and iterator identity persist across GlobalFree/delete.
  call('scalar_delete',0,(obj,))
  call('iterator_erase',objects,(node,))
 call('leave',0,(section,));observe('final',memory.snap());return 1

BOUNDARIES=('enter','find','pending_remove','global_free','scalar_delete','iterator_erase','leave')
# No physical deallocation, live-map algorithm or concurrency
# claim; raw DWORD handle provenance is not inferred from a field name.


raw=EXE.read_bytes();pehash=hashlib.sha256(raw).hexdigest()
WRAPPER=0x98ec90;MANAGER=0x98eb20;M=0x13bc870;G=M&~4095;A=0x2000000;N=32768;S=0x3000000;STOP=0x4000000
ENTER=0x6000000;LEAVE=0x6000100;GLOBALFREE=0x6000200;FIND=0x9902d0;PENDING=0x98e770;DELETE=0xbfe9bc;ERASE=0x990430
providers={a:read_virtual_data(raw,a,n) for a,n in ((WRAPPER,10),(MANAGER,156))}
for a,b in providers.items():assert hashlib.sha256(b).hexdigest()=={"0x98ec90":"a845da43c3bd362bbdbdaf898e2c3ab93928e8fc893caf54756faf6193971661","0x98eb20":"dcde86383edd319974b83771a2ad8121a108514ed1d4462db4a74bbaaa39d9be"}[hex(a)]
inputs={Path(__file__),R/'tools/decomp_pipeline/pe_oracle.py'}
def hashes():return {str(p.relative_to(R)):hashlib.sha256(p.read_bytes()).hexdigest() for p in sorted(inputs)}
before=hashes()
def word(v):return struct.pack('<I',v&0xffffffff)
class Memory:
 def __init__(self,b=None,g=None,u=None):self.b=b;self.g=g;self.u=u
 def region(self,p,n):
  assert n>=0
  if A<=p and p+n<=A+N:return self.b,p-A
  assert G<=p and p+n<=G+4096;return self.g,p-G
 def read(self,p,n):
  buf,off=self.region(p,n);return bytes(buf[off:off+n]) if self.u is None else bytes(self.u.mem_read(p,n))
 def write(self,p,b):
  buf,off=self.region(p,len(b))
  if self.u is None:buf[off:off+len(b)]=b
  else:self.u.mem_write(p,b)
 def w(self,p):return struct.unpack('<I',self.read(p,4))[0]
 def put(self,p,v):self.write(p,word(v))
 def snap(self):return self.read(A,N)+self.read(G,4096)
def callback(m,c,name,selfp,args,events,snaps):
 events.append((name,selfp,tuple(args)));snaps.append(m.snap());mode=c['mutation'];obj=c['object'];other=c['object2']
 if name=='enter' and mode==1:m.put(M+0x13c,c['header2']);m.put(obj+0x1c,0)
 if name=='find':
  assert selfp==M+0x13c and args==(c['uid'],)
  if mode==2:m.put(obj+0x1c,0);m.put(obj+0x20,c['handle2'])
  if mode==3:m.put(c['node']+0x14,other)
  if mode==10:m.put(M+0x13c,c['header2'])
  return c['node'] if c['found'] else m.w(M+0x13c)
 if name=='pending_remove':
  if mode==4:m.put(args[0]+0x1c,2)
  if mode==5:m.put(args[0]+0x1c,0);m.put(args[0]+0x20,c['handle2'])
 if name=='global_free' and mode==6:m.put(obj+0x20,0);m.put(obj+0x1c,1)
 if name=='scalar_delete' and mode==7:m.put(M+0x13c,c['header2']);m.put(c['node']+0x14,other)
 if name=='iterator_erase' and mode==8:m.put(M+0x13c,c['header2']);m.put(c['node']+0x14,other)
 if name=='leave' and mode==9:m.put(obj+0x20,c['handle2']);m.put(obj+0x1c,2);m.put(M+0x13c,c['header2'])
 # Deliberately full DWORD GlobalFree return with nonzero low byte; ignored.
 return c['globalfree_return'] if name=='global_free' else 0xabcdef80
def expected(initial,global_initial,c):
 m=Memory(bytearray(initial),bytearray(global_initial));events=[];snaps=[m.snap()]
 result=expected_predicate(m,M,c['uid'],lambda name,selfp,args:callback(m,c,name,selfp,args,events,snaps),lambda name,snap:snaps.append(snap));return events,snaps,result
class Runner:
 def __init__(self,entry,wrapper_body=None):
  selected=dict(providers)
  if wrapper_body is not None:selected[WRAPPER]=wrapper_body
  self.entry=entry;self.hits={hex(a):0 for a in providers};u=self.u=Uc(UC_ARCH_X86,UC_MODE_32)
  for page in {a&~4095 for a in (*providers,ENTER,LEAVE,GLOBALFREE,FIND,PENDING,DELETE,ERASE,0x143fe0c,0x143fe10,0x143fdf8,G)}:u.mem_map(page,4096)
  u.mem_map(A,N);u.mem_map(S,65536);u.mem_map(STOP,4096)
  for a,b in selected.items():u.mem_write(a,b)
  for slot,target in ((0x143fe0c,ENTER),(0x143fe10,LEAVE),(0x143fdf8,GLOBALFREE)):u.mem_write(slot,word(target))
  self.cpu=u.context_save();self.active=None;u.hook_add(UC_HOOK_CODE,lambda uc,pc,n,_:self.active(uc,pc))
 def run(self,initial,g,c):
  u=self.u;u.context_restore(self.cpu);u.mem_write(A,initial);u.mem_write(G,g);u.mem_write(S,bytes(65536));m=Memory(u=u);sp=S+0x8000;events=[];snaps=[m.snap()]
  u.mem_write(sp,word(STOP)+word(c['uid']));u.reg_write(UC_X86_REG_ESP,sp);u.reg_write(UC_X86_REG_ECX,c['receiver'] if self.entry==WRAPPER else M);regs=(UC_X86_REG_EBX,UC_X86_REG_EBP,UC_X86_REG_ESI,UC_X86_REG_EDI)
  for i,r in enumerate(regs):u.reg_write(r,0x11223300+i)
  def hook(uc,pc):
   if hex(pc) in self.hits:self.hits[hex(pc)]+=1
   if pc==STOP:uc.emu_stop();return
   stack=uc.reg_read(UC_X86_REG_ESP);selfp=uc.reg_read(UC_X86_REG_ECX)
   if pc==MANAGER:assert selfp==M
   if pc not in (ENTER,LEAVE,GLOBALFREE,FIND,PENDING,DELETE,ERASE):return
   if pc==FIND:
    out,uidref=struct.unpack('<II',uc.mem_read(stack+4,8));assert S<=out and out+4<=S+65536 and uidref==sp+4;args=(struct.unpack('<I',uc.mem_read(uidref,4))[0],);node=callback(m,c,'find',selfp,args,events,snaps);uc.mem_write(out,word(node));result=out;cleanup=8
   else:
    value=struct.unpack('<I',uc.mem_read(stack+4,4))[0];name={ENTER:'enter',LEAVE:'leave',GLOBALFREE:'global_free',PENDING:'pending_remove',DELETE:'scalar_delete',ERASE:'iterator_erase'}[pc]
    if pc in (ENTER,LEAVE):assert value==M+0x14c
    if pc==ERASE:assert selfp==M+0x13c and value==c['node']
    result=callback(m,c,name,selfp if pc in (PENDING,ERASE) else 0,(value,),events,snaps);cleanup=0 if pc==DELETE else 4
   ret=struct.unpack('<I',uc.mem_read(stack,4))[0];uc.reg_write(UC_X86_REG_EAX,result);uc.reg_write(UC_X86_REG_ECX,0x98765432);uc.reg_write(UC_X86_REG_EDX,0x87654321);uc.reg_write(UC_X86_REG_ESP,stack+4+cleanup);uc.reg_write(UC_X86_REG_EIP,ret)
  self.active=hook;u.emu_start(self.entry,STOP,count=1000)
  assert u.reg_read(UC_X86_REG_EIP)==STOP and u.reg_read(UC_X86_REG_ESP)==sp+8 and [u.reg_read(r) for r in regs]==[0x11223300+i for i in range(4)]
  result=u.reg_read(UC_X86_REG_EAX)&255;assert result in (0,1);snaps.append(m.snap());return events,snaps,result
def run_domain(wrapper,manager):
 cases=points=0;partitions={};results={0:0,1:0}
 for found,state,handle,returnword,uid,place,fill,mutation in itertools.product(range(2),(0,1,2,3,4,0xffffffff),(0,0x12345678),(0,0xffffffff),(0,0xffffffff),range(2),(0,0x95),range(11)):
  base=A+place*0x3000;b=bytearray([fill])*N;g=bytearray([fill])*4096;m=Memory(b,g);c=dict(found=found,uid=uid,node=base+0x1000,object=base+0x1400,object2=base+0x1800,header=base+0x200,header2=base+0x600,handle2=0x80000000,mutation=mutation,globalfree_return=returnword,receiver=0 if fill else base+0x2100)
  m.put(M+0x13c,c['header']);m.put(c['node']+0x14,c['object']);m.put(c['object']+0x1c,state);m.put(c['object']+0x20,handle);m.put(c['object2']+0x1c,4);m.put(c['object2']+0x20,0xffffffff)
  want=expected(bytes(b),bytes(g),c)
  for runner in (wrapper,manager):assert runner.run(bytes(b),bytes(g),c)==want,c
  cases+=1;points+=len(want[1]);results[want[2]]+=1;partitions[str(mutation)]=partitions.get(str(mutation),0)+1
 assert hashes()==before and hashlib.sha256(EXE.read_bytes()).hexdigest()==pehash
 assert wrapper.hits=={hex(WRAPPER):cases,hex(MANAGER):cases} and manager.hits=={hex(WRAPPER):0,hex(MANAGER):cases}
 report=dict(status='ORIGINAL_WRAPPER_MANAGER_REMOVE_MODEL_PASS',cases=cases,snapshots_per_chain=points,results=results,mutation_partitions=partitions,mode_provider_hits=dict(wrapper=wrapper.hits,manager=manager.hits),input_sha256_before=before,input_sha256_after=hashes(),original_exe_sha256=pehash,boundary='Actual10B wrapper+156B manager, raw32KiB arena+singleton4KiB global snapshot. All typed childinterfaces explicit callbacks; GlobalFree fullDWORDreturn ignored/noautomatichandleclear. Cachedsection/object/iterator, liveheader/state/handle, missingtrue/state0pending/state1-3false, ALboolRET4/registers. No genuine managerlayout/C++source/physicalownership/concurrency/lifetime claim.')
 return report
