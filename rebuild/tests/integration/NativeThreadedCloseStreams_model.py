"""Scratch original/rebuilt cleanup comparison; controlled Win32 and containers."""
from pathlib import Path
import sys,struct,json,itertools,hashlib
R=Path.cwd();sys.path.insert(0,str(R/'tools/decomp_pipeline'))
from gate_game_component_abi import Coff
from pe_oracle import EXE,read_virtual_data
from native_exact_roots import import_names
from unicorn import Uc,UC_ARCH_X86,UC_MODE_32,UC_HOOK_CODE
from unicorn.x86_const import *
root=Path(__file__).resolve().parent
build=Path(sys.argv[1])
c=Coff(build/'candidate.obj');symbol='?CloseAllStreamsForFile@CThreadedFileManager@@QAEXPAVCThreadedFile@@@Z'
s,o=c.symbols[symbol];body,rels=c.sections[s]
imports=import_names(EXE.read_bytes());reverse={v:k for k,v in imports.items()}
addresses={'_free':0xbfea14,'??3@YAXPAX@Z':0xbfe9bc}
for _,(name,_) in rels.items():
 if name.startswith('__imp__'):addresses[name]=reverse[name[7:].split('@')[0]]
 elif '_M_insert_overflow' in name:addresses[name]=0x464ace
 elif '_M_find' in name:addresses[name]=0x98fe70
 elif name.startswith('?find@'):addresses[name]=0x9902d0
 elif '_M_lower_bound' in name:addresses[name]=0x9901d0
 elif '_M_upper_bound' in name:addresses[name]=0x98fca0
 elif '__distance' in name:addresses[name]=0x990690
 elif '?erase@' in name:addresses[name]=0x9905d0
 elif '_Rebalance_for_erase' in name:addresses[name]=0x42a34e
patched=bytearray(body)
for site,(name,kind) in rels.items():
 value=addresses[name]+struct.unpack_from('<i',body,site)[0]-(0x98e7d0+site+4 if kind==20 else 0)
 struct.pack_into('<I',patched,site,value&0xffffffff)
original=read_virtual_data(EXE.read_bytes(),0x98e7d0,840)
A=0x2000000;M=A+0x100;H=A+0x1000;N=A+0x1100;O=A+0x1200;F=A+0x1400;V=A+0x1800
STACK=0x3000000;STOP=0x4000000;API=0x5000000
def execute(code,present,match,status,seed,waits,callback=0,wait_remove=False):
 u=Uc(UC_ARCH_X86,UC_MODE_32)
 for a,n in ((0x980000,0x20000),(0x460000,0x10000),(0x420000,0x10000),(0xbf0000,0x10000),(0x143f000,0x1000),(A,0x4000),(STACK,0x10000),(STOP,0x1000),(API,0x1000)):u.mem_map(a,n)
 u.mem_write(0x98e7d0,code)
 def put(a,v):u.mem_write(a,struct.pack('<I',v&0xffffffff))
 def get(a):return struct.unpack('<I',u.mem_read(a,4))[0]
 u.mem_write(A,bytes((i*37+seed*19)&255 for i in range(0x4000)))
 put(M+0x13c,H);put(M+0x140,int(present));put(M+0x16c,seed);put(M+0x164,0x1234)
 for off,v in ((0,0),(4,N if present else 0),(8,N if present else H),(12,N if present else H)):put(H+off,v)
 for off,v in ((0,1),(4,H),(8,0),(12,0),(16,17),(20,O)):put(N+off,v)
 put(O+0x28,F if match else F+0x100);put(O+0x1c,status);put(O+0x20,A+0x1500)
 put(F+4,0);put(O+0x14,3);put(O+0xc,A+0x1700 if callback else 0)
 put(A+0x1700,A+0x1750);put(A+0x1758,API+0x100)
 PH=A+0x2000;PN=A+0x2100;Q=M+0x34
 put(Q,PH);put(Q+4,1);put(Q+12,PN)
 for off,v in ((0,0),(4,PN),(8,PN),(12,PN)):put(PH+off,v)
 for off,v in ((0,1),(4,PH),(8,0),(12,0),(16,O),(20,O)):put(PN+off,v)
 trace=[];lock=0;sleeps=0;allocated=False
 api={}
 for i,name in enumerate(('EnterCriticalSection','LeaveCriticalSection','SetEvent','SleepEx','GlobalFree')):
  api[API+i*16]=name;put(reverse[name],API+i*16)
 def ret(pop=0,value=0):
  sp=u.reg_read(UC_X86_REG_ESP);ra=get(sp);u.reg_write(UC_X86_REG_EAX,value);u.reg_write(UC_X86_REG_ESP,sp+4+pop);u.reg_write(UC_X86_REG_EIP,ra)
 def hook(uc,a,size,data):
  nonlocal lock,sleeps,allocated
  sp=u.reg_read(UC_X86_REG_ESP);cx=u.reg_read(UC_X86_REG_ECX)
  if a in api:
   name=api[a]
   if name=='EnterCriticalSection':
    assert get(sp+4)==M+0x14c and lock==0;lock=1;trace.append(('enter',get(M+0x16c)));ret(4)
   elif name=='LeaveCriticalSection':
    assert get(sp+4)==M+0x14c and lock==1;lock=0;trace.append(('leave',get(M+0x16c)));ret(4)
   elif name=='SetEvent':assert not lock and get(sp+4)==0x1234;trace.append(('event',));ret(4,1)
   elif name=='SleepEx':
    assert not lock and get(sp+4)==0 and get(sp+8)==1;sleeps+=1;assert sleeps<=waits
    if sleeps==waits:
     put(O+0x1c,4)
     if wait_remove:put(H+4,0);put(H+8,H);put(H+12,H);put(M+0x140,0)
    trace.append(('sleep',));ret(8)
   else:assert lock and get(sp+4)==A+0x1500;trace.append(('overlap-free',));ret(4)
  elif a==0x464ace:
   assert lock and not allocated and get(sp+16)==1 and get(sp+20)==1
   value=get(get(sp+8));assert value==17;put(V,value);put(cx,V);put(cx+4,V+4);put(cx+8,V+4);allocated=True;trace.append(('collect',value));ret(20)
  elif a==0x98fe70:
   assert lock and cx==Q and get(get(sp+4))==O;ret(4,PN)
  elif a in (0x98fca0,0x9901d0):
   assert lock and cx==Q and get(get(sp+4))==O;ret(4,PH if a==0x98fca0 else PN)
  elif a==0x990690:
   assert get(get(sp+4))==PN and get(get(sp+8))==PH;ret(value=1)
  elif a==0x9905d0:
   assert lock and cx==Q and get(sp+4)==PN and get(sp+8)==PH and get(Q+12)==PH
   put(PH+4,0);put(PH+8,PH);put(PH+12,PH);put(Q+4,0);trace.append(('pending-erase',));ret(8)
  elif a==API+0x100:
   assert not lock and cx==A+0x1700 and get(sp+4)==0 and get(M+0x16c)==(seed+1)&0xffffffff
   trace.append(('callback',callback))
   if callback==2:
    put(H+4,0);put(H+8,H);put(H+12,H);put(M+0x140,0)
   ret(4)
  elif a==0x9902d0:
   assert lock and cx==M+0x13c and get(get(sp+8))==17
   out=get(sp+4);put(out,N if get(M+0x140) else H);ret(8,out)
  elif a==0xbfe9bc:
   assert lock and get(sp+4)==O;trace.append(('delete',));ret()
  elif a==0x42a34e:
   assert lock and get(sp+4)==N and get(sp+8)==H+4 and get(sp+12)==H+8 and get(sp+16)==H+12
   put(H+4,0);put(H+8,H);put(H+12,H);trace.append(('erase',));ret(value=N)
  elif a==0xbfea14:
   value=get(sp+4);assert value in (N,V);assert lock==int(value==N);trace.append(('free',value));ret()
 u.hook_add(UC_HOOK_CODE,hook)
 sp=STACK+0x8000;put(sp,STOP);put(sp+4,F)
 u.reg_write(UC_X86_REG_ESP,sp);u.reg_write(UC_X86_REG_ECX,M)
 regs=(UC_X86_REG_EBX,UC_X86_REG_EBP,UC_X86_REG_ESI,UC_X86_REG_EDI)
 for i,reg in enumerate(regs):u.reg_write(reg,0x12340000+i)
 u.emu_start(0x98e7d0,STOP,count=10000)
 assert u.reg_read(UC_X86_REG_EIP)==STOP and u.reg_read(UC_X86_REG_ESP)==sp+8 and not lock
 assert all(u.reg_read(reg)==0x12340000+i for i,reg in enumerate(regs))
 assert get(M+0x16c)==seed and get(M+0x140)==int(present and not match)
 assert allocated==bool(present and match)
 if present and match and status==0:assert get(Q+4)==0 and trace.count(('pending-erase',))==1 and trace.count(('callback',callback))==int(bool(callback))
 if present and match and ((callback==2 and status==0) or (wait_remove and status in (1,2,3))):assert ('delete',) not in trace
 assert sleeps==(waits if present and match and status in (1,2,3) else 0)
 return bytes(u.mem_read(A,0x4000)),trace
def main():
 cases=0
 for present,match,status,seed,waits,callback,wait_remove in itertools.product((False,True),(False,True),(0,1,2,3,4),(0,7,0x7fffffff),(1,3),(0,1,2),(False,True)):
  args=(present,match,status,seed,waits,callback,wait_remove)
  assert execute(bytes(patched),*args)==execute(original,*args),args
  cases+=1
 report=dict(status='PASS',cases=cases,candidate_bytes=len(body),original_bytes=840,source_sha256=hashlib.sha256((build/'candidate.cpp').read_bytes()).hexdigest(),boundaries=['Scratch only; no source promotion.','Empty/nonmatching/single matching request, pending callback retention/removal, finished and three active statuses with one/three controlled waits, including concurrent UID disappearance; full arena, ABI, lock/block counter and release trace compared.','Vector growth, tree erase, Win32 and memory release are controlled. Pending callback may retain or remove its UID; multi-node trees and host execution remain unproved.'])
 (build/('comparison-'+__import__('datetime').datetime.now(__import__('datetime').timezone.utc).strftime('%H%M%S%f')+'.json')).write_text(json.dumps(report,indent=2)+'\n');print(json.dumps(report,indent=2))

if __name__=='__main__':main()
