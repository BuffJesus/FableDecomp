"""Original immutable-UC CanRelocate + actual RAII vs independent model."""
import hashlib,importlib.util,itertools,json,struct,sys
import pefile
from pathlib import Path
from unicorn import Uc,UC_ARCH_X86,UC_MODE_32,UC_HOOK_CODE
from unicorn.x86_const import *
R=Path(__file__).resolve().parents[3];sys.path[:0]=[str(R/'tools'),str(R/'tools/decomp_pipeline')]
from pe_oracle import EXE,read_virtual_data
from pdb_class_methods import source,children,publics
"""Independent bounded CanRelocate model draft; no execution at import."""
def expected_predicate(memory,movable_this,ignored_long,lock,unlock,observe):
 # Callbacks are explicit thiscall lock constructor/destructor interfaces.
 # Memory supports w(address), read(address,size), snap(); observe captures
 # complete arena and, optionally, the normalized eight-byte stack lock.
 section=(memory.w(movable_this+0x24)+0x14c)&0xffffffff
 observe('before_lock',memory.snap())
 lock(section,False)
 # These volatile bytes are read after locking, not cached before callbacks.
 finished=memory.read(movable_this+0x40,1)[0]
 # Native short circuit never reads BufferLocked when Finished==0.
 can_relocate=bool(finished) and not memory.read(movable_this+0x41,1)[0]
 observe('before_unlock',memory.snap())
 unlock()
 # Destructor mutations are retained, but cannot change the cached predicate.
 observe('final',memory.snap())
 return int(can_relocate)

DOMAIN={
 'raw_bool_bytes':(0,1,0x7f,0x80,0xff),
 'ignored_long':(0,1,0x7fffffff,0x80000000,0xffffffff),
 'callback_partitions':('unchanged','lock_changes_finished','lock_changes_locked',
                        'lock_changes_both','lock_changes_bank','unlock_changes_both'),
 'arena_fills':(0,0x95),
 'object_placements':2,
}


ENTRY=0x9d55d0;LOCK=0xa62920;UNLOCK=0xa629a0;ENTER=0x6000000;LEAVE=0x6000100
A=0x2000000;N=32768;S=0x3000000;STOP=0x4000000
raw=EXE.read_bytes();pehash=hashlib.sha256(raw).hexdigest()
pe=pefile.PE(data=raw)
imports={item.address:(entry.dll.decode(),item.name.decode() if item.name else None) for entry in pe.DIRECTORY_ENTRY_IMPORT for item in entry.imports}
for address,name in ((0x143fe0c,'EnterCriticalSection'),(0x143fe10,'LeaveCriticalSection'),(0x143fd30,'TryEnterCriticalSection')):
 assert imports[address][0].lower()=='kernel32.dll' and imports[address][1]==name
providers={a:read_virtual_data(raw,a,n) for a,n in ((ENTRY,76),(LOCK,54),(UNLOCK,25))}
assert hashlib.sha256(providers[ENTRY]).hexdigest()=='4ec6541b2cc647d4c628bd61fd60b2bda270b6f3e4bf9567357464fc2aa597ae'
assert struct.unpack('<III',read_virtual_data(raw,0x129b8b4,12))==(0x9d8180,0x9d5440,0x9d54a0)
assert struct.unpack('<IIII',read_virtual_data(raw,0x129b8a0,16))==(0x9d80a0,0x9d5670,ENTRY,0x9d5100)
# Fresh DIA, not a stale signature packet.
dia,src=source();src.loadDataFromPdb(str(R/'debug_build/Ego_r.pdb'));g=src.openSession().globalScope
udt=g.findChildren(11,'CBankFileAsyncData',0).Item(0)
methods=[dict(name=x.name,signature=x.undecoratedName,access=x.access,calling_convention=x.type.callingConvention) for x in children(dia,udt,5) if x.name=='CanRelocateData']
assert len(methods)==1 and methods[0]['access']==3 and methods[0]['calling_convention']==11
public=publics('?CanRelocateData@CBankFileAsyncData',str(R/'debug_build/Ego_r.pdb'));assert public
inputs=(Path(__file__),R/'tools/decomp_pipeline/pe_oracle.py',R/'tools/pdb_class_methods.py',R/'debug_build/Ego_r.pdb',R/'rebuild/include/fable_game_text_bank.h',R/'rebuild/include/fable_movable_resource.h')
def hashes():return {str(p.relative_to(R)):hashlib.sha256(p.read_bytes()).hexdigest() for p in inputs}
before=hashes()
def word(v):return struct.pack('<I',v&0xffffffff)
class Memory:
 def __init__(self,b=None,u=None):self.b=b;self.u=u
 def read(self,p,n):
  assert n>=0 and A<=p and p+n<=A+N
  return bytes(self.b[p-A:p-A+n]) if self.b is not None else bytes(self.u.mem_read(p,n))
 def write(self,p,b):
  assert A<=p and p+len(b)<=A+N
  if self.b is not None:self.b[p-A:p-A+len(b)]=b
  else:self.u.mem_write(p,b)
 def w(self,p):return struct.unpack('<I',self.read(p,4))[0]
 def put(self,p,v):self.write(p,word(v))
 def snap(self):return self.read(A,N)
def mutate(m,c,name):
 d=c['data'];mode=c['mutation']
 if name=='enter':
  if mode in (1,3):m.write(d+68,b'\x80')
  if mode in (2,3):m.write(d+69,b'\0')
  if mode==4:m.put(d+40,c['bank2'])
 elif mode==5:m.write(d+68,b'\0');m.write(d+69,b'\xff');m.put(d+40,c['bank2'])
def expected(initial,c):
 m=Memory(bytearray(initial));events=[];snaps=[];locks=[];section=m.w(c['data']+40)+0x14c;pad=bytes([c['stackfill']])*3
 def lock(s,tryonly):
  assert s==section and not tryonly;events.append(('enter',s));locks.append(word(s)+bytes([c['stackfill']])+pad);mutate(m,c,'enter')
 def unlock():events.append(('leave',section));locks.append(word(section)+b'\1'+pad);mutate(m,c,'leave')
 result=expected_predicate(m,c['data']+4,c['argument'],lock,unlock,lambda label,snapshot:snaps.append(snapshot))
 locks.append(word(section)+b'\0'+pad);return events,snaps,locks,result
class Runner:
 # Each future compiled mode must instantiate a separate immutable machine.
 def __init__(self,bodies):
  self.u=Uc(UC_ARCH_X86,UC_MODE_32);u=self.u
  for page in {a&~4095 for a in (*bodies,ENTER,LEAVE,0x143fd30,0x143fe0c,0x143fe10)}:u.mem_map(page,4096)
  u.mem_map(A,N);u.mem_map(S,65536);u.mem_map(STOP,4096)
  for a,b in bodies.items():u.mem_write(a,b)
  for a,target in ((0x143fe0c,ENTER),(0x143fe10,LEAVE),(0x143fd30,ENTER+0x200)):u.mem_write(a,word(target))
  self.cpu=u.context_save();self.active=None;self.hits={hex(a):0 for a in bodies};u.hook_add(UC_HOOK_CODE,lambda uc,pc,n,_:self.active(uc,pc))
 def run(self,initial,c):
  u=self.u;u.context_restore(self.cpu);u.mem_write(A,initial);u.mem_write(S,bytes([c['stackfill']])*65536);m=Memory(u=u);sp=S+0x8000
  u.mem_write(sp,word(STOP)+word(c['argument']));u.reg_write(UC_X86_REG_ESP,sp);u.reg_write(UC_X86_REG_ECX,c['data']+4)
  regs=(UC_X86_REG_EBX,UC_X86_REG_EBP,UC_X86_REG_ESI,UC_X86_REG_EDI)
  for i,r in enumerate(regs):u.reg_write(r,0x11223300+i)
  events=[];snaps=[m.snap()];locks=[];lockp=[None];pending=[]
  def hook(uc,pc):
   if hex(pc) in self.hits:self.hits[hex(pc)]+=1
   if pc==STOP:uc.emu_stop();return
   stack=uc.reg_read(UC_X86_REG_ESP)
   if pc==LOCK:
    section,tryonly=struct.unpack('<II',uc.mem_read(stack+4,8));assert tryonly==0;lockp[0]=uc.reg_read(UC_X86_REG_ECX);pending.append((struct.unpack('<I',uc.mem_read(stack,4))[0],stack,lockp[0],[uc.reg_read(r) for r in regs]))
   if pending and pc==pending[-1][0]:
    ret,oldsp,receiver,saved=pending.pop();assert uc.reg_read(UC_X86_REG_ESP)==oldsp+12 and uc.reg_read(UC_X86_REG_EAX)==receiver and [uc.reg_read(r) for r in regs]==saved
   if pc==UNLOCK:assert uc.reg_read(UC_X86_REG_ECX)==lockp[0];snaps.append(m.snap())
   assert pc!=ENTER+0x200
   if pc not in (ENTER,LEAVE):return
   section=struct.unpack('<I',uc.mem_read(stack+4,4))[0];name='enter' if pc==ENTER else 'leave';events.append((name,section));locks.append(bytes(uc.mem_read(lockp[0],8)));mutate(m,c,name)
   ret=struct.unpack('<I',uc.mem_read(stack,4))[0];uc.reg_write(UC_X86_REG_EAX,0xabcdef80);uc.reg_write(UC_X86_REG_ECX,0x87654321);uc.reg_write(UC_X86_REG_EDX,0x98765432);uc.reg_write(UC_X86_REG_ESP,stack+8);uc.reg_write(UC_X86_REG_EIP,ret)
  self.active=hook;u.emu_start(ENTRY,STOP,count=500)
  assert u.reg_read(UC_X86_REG_EIP)==STOP and u.reg_read(UC_X86_REG_ESP)==sp+8 and not pending
  assert [u.reg_read(r) for r in regs]==[0x11223300+i for i in range(4)]
  locks.append(bytes(u.mem_read(lockp[0],8)));snaps.append(m.snap());result=u.reg_read(UC_X86_REG_EAX)&255;assert result in (0,1)
  return events,snaps,locks,result
