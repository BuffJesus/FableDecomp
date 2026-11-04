"""Independent field/owner model vs original and supplied native RAII chains."""
import hashlib,itertools,json,struct,sys
from pathlib import Path
from unicorn import Uc,UC_ARCH_X86,UC_MODE_32,UC_HOOK_CODE
from unicorn.x86_const import *
R=Path(__file__).resolve().parents[3];sys.path.insert(0,str(R/'tools/decomp_pipeline'))
from pe_oracle import EXE,read_virtual_data
ENTRY=0x9d54a0;LOCK=0xa62920;UNLOCK=0xa629a0;ENTER=0x6000000;LEAVE=0x6000100
IENTER=0x143fe0c;ILEAVE=0x143fe10;ITRY=0x143fd30;A=0x2000000;N=32768;S=0x3000000;STOP=0x4000000
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
def callback(m,c,name,section,events,snaps):
 events.append((name,section));snaps.append(m.snap());d=c['data'];mode=c['mutation']
 if name=='enter':
  if mode in (1,5):m.put(d+40,c['bank2'])
  if mode in (2,5):m.put(d+32,0xffffffff)
  if mode in (3,6):m.put(d+32,0x80000000)
  if mode in (4,6):m.write(d+68,b'\x95');m.write(d+70,b'\x80');m.put(d+24,0xdeadbeef)
 elif name=='leave':
  if mode in (5,7):m.put(d+40,c['bank2']);m.put(d+32,0x12345678);m.write(d+68,b'\x95');m.write(d+70,b'\x80');m.put(d+24,0xbaadf00d)
 else:raise AssertionError(name)
 return 0xabcdef00
def model(initial,c):
 m=Memory(bytearray(initial));d=c['data'];events=[];snaps=[];locks=[];section=m.w(d+40)+0x14c;pad=bytes([c['stackfill']])*3
 locks.append(word(section)+bytes([c['stackfill']])+pad)
 callback(m,c,'enter',section,events,snaps)
 restart=m.w(d+32)!=0xffffffff
 m.write(d+68,b'\x01');m.put(d+24,0)
 if restart:m.write(d+70,b'\x01')
 locks.append(word(section)+b'\x01'+pad)
 callback(m,c,'leave',section,events,snaps)
 locks.append(word(section)+b'\x00'+pad);snaps.append(m.snap())
 return events,snaps,locks
def run_fixture(compiled_bodies):
 raw=EXE.read_bytes();bodies={ENTRY:read_virtual_data(raw,ENTRY,63),LOCK:read_virtual_data(raw,LOCK,54),UNLOCK:read_virtual_data(raw,UNLOCK,25)}
 assert set(compiled_bodies)==set(bodies) and all(len(compiled_bodies[a])==len(bodies[a]) for a in bodies)
 assert hashlib.sha256(bodies[ENTRY]).hexdigest()=='242f84a472961f1b8e2f6467c70c8409f7ae80921aee3343e7b86c68ae28e5bd'
 inputs={Path(__file__),R/'tools/decomp_pipeline/pe_oracle.py'};visited=set()
 def headers(p):
  if p in visited:return
  visited.add(p);inputs.add(p)
  import re
  for name in re.findall(r'^\s*#\s*include\s*"([^"]+)"',p.read_text(encoding='utf-8'),re.M):
   q=next((q for q in (p.parent/name,R/'rebuild/include'/name) if q.is_file()),None);assert q;headers(q)
 headers(R/'rebuild/include/fable_game_text_bank.h');headers(R/'rebuild/include/fable_critical_section_lock.h')
 def hashes():return {str(p.relative_to(R)):hashlib.sha256(p.read_bytes()).hexdigest() for p in sorted(inputs)}
 before=hashes();u=Uc(UC_ARCH_X86,UC_MODE_32)
 for page in {v&~4095 for v in (ENTRY,LOCK,UNLOCK,ENTER,LEAVE,IENTER,ILEAVE,ITRY)}:u.mem_map(page,4096)
 u.mem_map(A,N);u.mem_map(S,65536);u.mem_map(STOP,4096)
 for address,body in bodies.items():u.mem_write(address,body)
 u.mem_write(IENTER,word(ENTER));u.mem_write(ILEAVE,word(LEAVE));u.mem_write(ITRY,word(ENTER+0x200));cpu=u.context_save();active=[None];u.hook_add(UC_HOOK_CODE,lambda uc,pc,n,_:active[0](uc,pc));regs=(UC_X86_REG_EBX,UC_X86_REG_EBP,UC_X86_REG_ESI,UC_X86_REG_EDI)
 execution_counts={kind:{hex(a):0 for a in bodies} for kind in ('original','compiled')}
 def run(initial,c,selected_bodies,kind):
  u.context_restore(cpu)
  for address,body in selected_bodies.items():
   u.mem_write(address,body);u.ctl_remove_cache(address,address+len(body))
  u.mem_write(A,initial);u.mem_write(S,bytes([c['stackfill']])*65536);m=Memory(u=u);events=[];snaps=[];locks=[];sp=S+0x8000;lockp=[None];lockreturns=[]
  u.mem_write(sp,word(STOP)+word(c['boolarg']));u.reg_write(UC_X86_REG_ESP,sp);u.reg_write(UC_X86_REG_ECX,c['data'])
  for i,r in enumerate(regs):u.reg_write(r,0x11223300+i)
  def hook(uc,pc):
   if pc in selected_bodies:execution_counts[kind][hex(pc)]+=1
   if pc==STOP:uc.emu_stop();return
   stack=uc.reg_read(UC_X86_REG_ESP)
   if pc==LOCK:
    section,tryonly=struct.unpack('<II',uc.mem_read(stack+4,8));assert tryonly==0;lockp[0]=uc.reg_read(UC_X86_REG_ECX);lockreturns.append((struct.unpack('<I',uc.mem_read(stack,4))[0],stack,lockp[0],[uc.reg_read(r) for r in regs]))
   if lockreturns and pc==lockreturns[-1][0]:
    ret,oldstack,receiver,saved=lockreturns.pop();assert uc.reg_read(UC_X86_REG_ESP)==oldstack+12 and uc.reg_read(UC_X86_REG_EAX)==receiver and [uc.reg_read(r) for r in regs]==saved
   if pc==UNLOCK:assert uc.reg_read(UC_X86_REG_ECX)==lockp[0]
   assert pc!=ENTER+0x200,'Blocking ctor must not call TryEnter'
   if pc not in (ENTER,LEAVE):return
   locks.append(bytes(uc.mem_read(lockp[0],8)));section=struct.unpack('<I',uc.mem_read(stack+4,4))[0];result=callback(m,c,'enter' if pc==ENTER else 'leave',section,events,snaps);ret=struct.unpack('<I',uc.mem_read(stack,4))[0]
   uc.reg_write(UC_X86_REG_EAX,result);uc.reg_write(UC_X86_REG_ECX,0xabcdef01);uc.reg_write(UC_X86_REG_EDX,0x98765432);uc.reg_write(UC_X86_REG_ESP,stack+8);uc.reg_write(UC_X86_REG_EIP,ret)
  active[0]=hook;u.emu_start(ENTRY,STOP,count=1000)
  assert u.reg_read(UC_X86_REG_EIP)==STOP and u.reg_read(UC_X86_REG_ESP)==sp+8 and not lockreturns
  assert [u.reg_read(r) for r in regs]==[0x11223300+i for i in range(4)]
  locks.append(bytes(u.mem_read(lockp[0],8)));snaps.append(m.snap());return events,snaps,locks
 cases=points=0
 for handle,boolarg,place,fill,stackfill,mutation in itertools.product((0,1,0x7fffffff,0x80000000,0xffffffff),(0,1,255,0x80000000,0xffffffff),range(2),(0,0x95),(0,0x95),range(8)):
  origin=A+place*0x3000;initial=bytearray([fill])*N;m=Memory(initial);d=origin+0x1000;c=dict(data=d,bank=origin+0x200,bank2=origin+0x800,handle=handle,boolarg=boolarg,stackfill=stackfill,mutation=mutation)
  m.put(d+40,c['bank']);m.put(d+32,handle);m.put(d+24,0x12345678)
  expected=model(initial,c);actual=run(bytes(initial),c,bodies,'original');assert actual==expected,(c,actual[0],expected[0],actual[2],expected[2]);compiled=run(bytes(initial),c,compiled_bodies,'compiled');assert compiled==expected,(c,compiled[0],expected[0],compiled[2],expected[2]);cases+=1;points+=len(expected[1])
 assert before==hashes()
 assert all(count==cases for counts in execution_counts.values() for count in counts.values())
 report=dict(status='ORIGINAL_COMPILED_ASYNC_COMPLETION_ACTUAL_LOCK_MODEL_PASS',cases=cases,snapshots=points,lock_snapshots=cases*3,input_sha256_before=before,input_sha256_after=hashes(),body_sha256={hex(a):hashlib.sha256(b).hexdigest() for a,b in bodies.items()},original_exe_sha256=hashlib.sha256(raw).hexdigest(),boundary='Actual63B completion+54B lockctor+25B destructor. Only Win32Enter/Leave stdcall interfaces; no real concurrency. Full32KiB arena+8B cached lock including untouchedpadding at Enter/Leave/final; livehandle afterEnter, boolargignored RET4/callee saves. Original root does not overwrite callback state afterLeave. Actual fresh compiled chain compared separately with independent model; no concurrency or extra lock-provider recovery credit.')
 report.update(execution_counts=execution_counts,translation_cache_policy='Explicit ctl_remove_cache after every executable body write')
 return report
