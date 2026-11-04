"""Independent expected state/events and immutable native root machines."""
import hashlib,itertools,struct,sys
from pathlib import Path
from unicorn import Uc,UC_ARCH_X86,UC_MODE_32,UC_HOOK_CODE
from unicorn.x86_const import *
R=Path(__file__).resolve().parents[3];sys.path.insert(0,str(R/'tools/decomp_pipeline'))
from pe_oracle import EXE,read_virtual_data
ROOT=0x98ec70;CHILD=0x98e7d0;MANAGER=0x13bc870;G=MANAGER&~4095
A=0x2000000;N=32768;S=0x3000000;STOP=0x4000000
raw=EXE.read_bytes();pehash=hashlib.sha256(raw).hexdigest();original=read_virtual_data(raw,ROOT,25)
assert original.hex()=='568bf1837e08ff740b56b970c83b01e84cfbffff8b46085ec3'
def word(n):return struct.pack('<I',n&0xffffffff)
def mutate(arena,globalarena,case):
 at=case['file']-A;arena[at+8:at+12]=word(case['replacement']);arena[at+0x14:at+0x18]=word(0x87654321)
 globalarena[0x100:0x104]=word(case['replacement']^0xaaaaaaaa)
def expected(initial,global_initial,case):
 arena=bytearray(initial);globalarena=bytearray(global_initial);snap=lambda:bytes(arena)+bytes(globalarena);snaps=[snap()];events=[]
 handle=struct.unpack_from('<I',arena,case['file']-A+8)[0]
 if handle!=0xffffffff:
  events.append(('CloseAllStreamsForFile',MANAGER,case['file']));snaps.append(snap());mutate(arena,globalarena,case)
 result=struct.unpack_from('<I',arena,case['file']-A+8)[0];snaps.append(snap());return events,snaps,result
class Runner:
 def __init__(self,body):
  u=self.u=Uc(UC_ARCH_X86,UC_MODE_32)
  for page in {ROOT&~4095,CHILD&~4095,G}:u.mem_map(page,4096)
  u.mem_map(A,N);u.mem_map(S,4096);u.mem_map(STOP,4096);u.mem_write(ROOT,body)
  self.cpu=u.context_save();self.active=None;self.hits={hex(ROOT):0,hex(CHILD):0};u.hook_add(UC_HOOK_CODE,lambda uc,pc,size,_:self.active(uc,pc))
 def run(self,initial,global_initial,case):
  u=self.u;u.context_restore(self.cpu);u.mem_write(A,initial);u.mem_write(G,global_initial);u.mem_write(S,bytes(4096));sp=S+0x800;u.mem_write(sp,word(STOP));u.reg_write(UC_X86_REG_ESP,sp);u.reg_write(UC_X86_REG_ECX,case['file'])
  regs=(UC_X86_REG_EBX,UC_X86_REG_EBP,UC_X86_REG_ESI,UC_X86_REG_EDI)
  for i,r in enumerate(regs):u.reg_write(r,0x11223300+i)
  snapshot=lambda:bytes(u.mem_read(A,N))+bytes(u.mem_read(G,4096));events=[];snaps=[snapshot()]
  def hook(uc,pc):
   if hex(pc) in self.hits:self.hits[hex(pc)]+=1
   if pc==STOP:uc.emu_stop();return
   if pc!=CHILD:return
   stack=uc.reg_read(UC_X86_REG_ESP);ret,file=struct.unpack('<II',uc.mem_read(stack,8));assert file==case['file'] and uc.reg_read(UC_X86_REG_ECX)==MANAGER
   events.append(('CloseAllStreamsForFile',MANAGER,file));snaps.append(snapshot());arena=bytearray(uc.mem_read(A,N));globalarena=bytearray(uc.mem_read(G,4096));mutate(arena,globalarena,case);uc.mem_write(A,bytes(arena));uc.mem_write(G,bytes(globalarena))
   # Actual child interface is thiscall void(pointer): callee RET4; its EAX
   # residue is arbitrary and must not become the wrapper's return value.
   uc.reg_write(UC_X86_REG_EAX,case['residue']);uc.reg_write(UC_X86_REG_ECX,0xabcdef01);uc.reg_write(UC_X86_REG_EDX,0xabcdef02);uc.reg_write(UC_X86_REG_ESP,stack+8);uc.reg_write(UC_X86_REG_EIP,ret)
  self.active=hook;u.emu_start(ROOT,STOP,count=100)
  assert u.reg_read(UC_X86_REG_EIP)==STOP and u.reg_read(UC_X86_REG_ESP)==sp+4
  assert [u.reg_read(r) for r in regs]==[0x11223300+i for i in range(4)]
  snaps.append(snapshot());return events,snaps,u.reg_read(UC_X86_REG_EAX)

def run_domain(machines):
 cases=points=callbacks=0
 for handle,replacement,place,fill,residue in itertools.product((0,1,0x7fffffff,0x80000000,0xffffffff),(0,1,0x7fffffff,0x80000000,0xffffffff),range(2),(0,0x95),(0,0xffffffff)):
  case=dict(file=A+0x1000+place*0x3000,replacement=replacement,residue=residue);arena=bytearray([fill])*N;globalarena=bytes([fill])*4096;struct.pack_into('<I',arena,case['file']-A+8,handle);want=expected(bytes(arena),globalarena,case)
  for machine in machines.values():assert machine.run(bytes(arena),globalarena,case)==want,case
  cases+=1;points+=len(want[1]);callbacks+=len(want[0])
 assert (cases,points,callbacks)==(200,560,160)
 assert all(m.hits=={hex(ROOT):200,hex(CHILD):160} for m in machines.values())
 return dict(cases=cases,snapshots_per_mode=points,typed_child_callbacks_per_mode=callbacks,mode_hits={name:m.hits for name,m in machines.items()})
