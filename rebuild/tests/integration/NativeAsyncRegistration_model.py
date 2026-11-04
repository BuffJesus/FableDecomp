"""Original registration model: prepared for review; run only after authorization."""
import hashlib,itertools,json,struct,sys
from pathlib import Path
from unicorn import Uc,UC_ARCH_X86,UC_MODE_32,UC_HOOK_CODE
from unicorn.x86_const import *
R=Path(__file__).resolve().parents[3];sys.path.insert(0,str(R/'tools/decomp_pipeline'))
from pe_oracle import EXE,read_virtual_data
ENTRY=0x9d7cf0;DECREMENT=0x9d74e0;INSERT=0x9d7990;A=0x2000000;N=32768;S=0x3000000;STOP=0x4000000
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
def child(m,c,x,y,valuep,w,events,snaps):
 assert valuep==c['valuep'] and w==0
 events.append((c['tree'],x,y,valuep,w,m.w(valuep)));snaps.append(m.snap())
 if c['mutation']==1:m.put(valuep,0xffffffff)
 elif c['mutation']==2:m.put(c['tree'],c['alternate_header']);m.put(c['tree']+4,0x80000000)
 elif c['mutation']==3:m.put(c['output']+4,0x12345678)
 return c['returned_node']
def predecessor(m,node):
 # Independent in-order predecessor, original code executed separately.
 parent=m.w(node+4)
 if m.read(node,1)==b'\x00' and m.w(parent+4)==node:return m.w(node+12)
 left=m.w(node+8)
 if left:
  while m.w(left+12):left=m.w(left+12)
  return left
 while node==m.w(parent+8):node,parent=parent,m.w(parent+4)
 return parent
def model(initial,c):
 m=Memory(bytearray(initial));events=[];snaps=[m.snap()];h=m.w(c['tree']);parent=h;node=m.w(h+4);less=True
 while node:
  parent=node;less=m.w(c['valuep'])<m.w(node+16);node=m.w(node+(8 if less else 12))
 candidate=parent
 if less and candidate==m.w(h+8):
  result=child(m,c,parent,parent,c['valuep'],0,events,snaps);success=1
 else:
  if less:candidate=predecessor(m,candidate)
  if m.w(candidate+16)<m.w(c['valuep']):result=child(m,c,node,parent,c['valuep'],0,events,snaps);success=1
  else:result=candidate;success=0
 m.put(c['output'],result);m.write(c['output']+4,bytes([success]));snaps.append(m.snap());return events,snaps,c['output']
def main():
 assert RUN_AUTHORIZED, 'Gate-controlled model execution'
 raw=EXE.read_bytes();root=read_virtual_data(raw,ENTRY,180);decrement=read_virtual_data(raw,DECREMENT,79)
 assert hashlib.sha256(root).hexdigest()=='a0c0f8df766b3700bc25c26fcfa924bb2c3af3986011bb3a233610a313de9002'
 assert hashlib.sha256(decrement).hexdigest()=='2444342210bbf0539e77602ef04cadcf5b615c598b15c6879531efafdbeef3c8'
 inputs=(Path(__file__),R/'tools/decomp_pipeline/pe_oracle.py',R/'rebuild/tests/integration/NativeAsyncRegistration_test.py')
 def hashes():return {str(p.relative_to(R)):hashlib.sha256(p.read_bytes()).hexdigest() for p in inputs}
 before=hashes();u=Uc(UC_ARCH_X86,UC_MODE_32)
 for page in {v&~4095 for v in (ENTRY,DECREMENT,INSERT)}:u.mem_map(page,4096)
 u.mem_map(A,N);u.mem_map(S,65536);u.mem_map(STOP,4096);u.mem_write(ENTRY,root);u.mem_write(DECREMENT,decrement)
 active=[None];u.hook_add(UC_HOOK_CODE,lambda uc,pc,n,_:active[0](uc,pc));cpu=u.context_save();coverage={};regs=(UC_X86_REG_EBX,UC_X86_REG_EBP,UC_X86_REG_ESI,UC_X86_REG_EDI)
 def run(initial,c):
  u.context_restore(cpu);u.mem_write(A,initial);u.mem_write(S,bytes(65536));m=Memory(u=u);events=[];snaps=[m.snap()];sp=S+0x8000
  u.mem_write(sp,word(STOP)+word(c['output'])+word(c['valuep']));u.reg_write(UC_X86_REG_ESP,sp);u.reg_write(UC_X86_REG_ECX,c['tree'])
  for i,r in enumerate(regs):u.reg_write(r,0x11223300+i)
  def hook(uc,pc):
   coverage[hex(pc)]=coverage.get(hex(pc),0)+1
   if pc==STOP:uc.emu_stop();return
   if pc!=INSERT:return
   stack=uc.reg_read(UC_X86_REG_ESP);resultp,x,y,valuep,w=struct.unpack('<IIIII',uc.mem_read(stack+4,20));assert uc.reg_read(UC_X86_REG_ECX)==c['tree']
   node=child(m,c,x,y,valuep,w,events,snaps);uc.mem_write(resultp,word(node));ret=struct.unpack('<I',uc.mem_read(stack,4))[0]
   # Actual ABI returns hidden iterator result pointer, never alternate EAX.
   uc.reg_write(UC_X86_REG_EAX,resultp);uc.reg_write(UC_X86_REG_ECX,0xabcdef01);uc.reg_write(UC_X86_REG_EDX,0x98765432);uc.reg_write(UC_X86_REG_ESP,stack+24);uc.reg_write(UC_X86_REG_EIP,ret)
  active[0]=hook;u.emu_start(ENTRY,STOP,count=2000)
  assert u.reg_read(UC_X86_REG_EIP)==STOP and u.reg_read(UC_X86_REG_ESP)==sp+12
  assert [u.reg_read(r) for r in regs]==[0x11223300+i for i in range(4)]
  snaps.append(m.snap());return events,snaps,u.reg_read(UC_X86_REG_EAX)
 keys=(0,1,0x7fffffff,0x80000000,0xfffffffe,0xffffffff)
 shapes=((),(2,),(2,0,4,1,3,5),(0,1,2,3,4,5),(5,4,3,2,1,0))
 cases=points=0
 for order,incoming,place,fill,mutation in itertools.product(shapes,keys,range(2),(0,0x95),range(4)):
  origin=A+place*0x3000;initial=bytearray([fill])*N;m=Memory(initial);tree=origin+0x100;header=origin+0x200
  m.put(tree,header);m.put(tree+4,len(order));m.write(header,b'\x00');m.put(header+4,0);m.put(header+8,header);m.put(header+12,header)
  for i,keyindex in enumerate(order):
   new=origin+0x400+i*0x20;m.write(new,b'\x01');m.put(new+8,0);m.put(new+12,0);m.put(new+16,keys[keyindex]);p=header;n=m.w(header+4)
   while n:p=n;n=m.w(n+(8 if keys[keyindex]<m.w(n+16) else 12))
   m.put(new+4,p)
   if p==header:m.put(header+4,new)
   else:m.put(p+(8 if keys[keyindex]<m.w(p+16) else 12),new)
   nodes=[origin+0x400+j*0x20 for j in range(i+1)];m.put(header+8,min(nodes,key=lambda n:m.w(n+16)));m.put(header+12,max(nodes,key=lambda n:m.w(n+16)))
  c=dict(tree=tree,valuep=origin+0x1000,output=origin+0x1100,alternate_header=origin+0x1800,returned_node=origin+0x1900,mutation=mutation);m.put(c['valuep'],incoming)
  expected=model(initial,c);actual=run(bytes(initial),c);assert actual==expected,(order,incoming,c,actual[0],expected[0]);cases+=1;points+=len(expected[1])
 assert before==hashes()
 report=dict(status='ORIGINAL_REGISTRATION_MODEL_PASS',cases=cases,snapshots=points,coverage=coverage,input_sha256_before=before,input_sha256_after=hashes(),original_exe_sha256=hashlib.sha256(raw).hexdigest(),boundary='Actual180B registration and79B iterator decrement. _M_insert explicit five-stack-DWORD RET20 interface including hidden result; no allocator/rebalance implementation claim. Fullarena snapshots/pairpadding and unsigned rawpointer model, ABI checked. No compiledsource/promotion/credit.')
 Path(__file__).with_name('NativeAsyncRegistration_model_result.json').write_text(json.dumps(report,indent=2)+'\n');print(report['status'],cases,points,flush=True)
if __name__=='__main__':main()
