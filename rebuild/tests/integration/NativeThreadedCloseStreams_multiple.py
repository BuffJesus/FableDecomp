"""Multiple request cleanup with exact reconstructed vector and tree helpers."""
from NativeThreadedCloseStreams_model import *

def multiple(code,count,mask,seed):
 u=Uc(UC_ARCH_X86,UC_MODE_32)
 for a,n in ((0x410000,0x10000),(0x980000,0x20000),(0x460000,0x10000),(0x420000,0x10000),(0xbf0000,0x10000),(0x143f000,0x1000),(A,0x10000),(STACK,0x10000),(STOP,0x1000),(API,0x1000)):u.mem_map(a,n)
 u.mem_write(0x98e7d0,code)
 for record in json.loads((build/'helper-code.json').read_text())['tree']:u.mem_write(record['address'],bytes.fromhex(record['patched']))
 u.mem_write(0x464ace,bytes.fromhex(json.loads((build/'helper-code.json').read_text())['vector']['patched']))
 def put(a,v):u.mem_write(a,struct.pack('<I',v&0xffffffff))
 def get(a):return struct.unpack('<I',u.mem_read(a,4))[0]
 u.mem_write(A,bytes((i*37+seed*19)&255 for i in range(0x10000)))
 nodes=[A+0x3000+i*0x20 for i in range(count)]
 objects=[A+0x4000+i*0x40 for i in range(count)]
 overlaps=[A+0x5000+i*0x20 for i in range(count)]
 live=list(range(count));selected=[i for i in live if mask>>i&1]
 put(M+0x13c,H);put(M+0x140,count);put(M+0x16c,seed)
 def assemble(indices,parent,depth=0):
  if not indices:return 0
  k=len(indices)//2;i=indices[k];node=nodes[i]
  put(node,0 if count==2 and depth else 1);put(node+4,parent)
  put(node+8,assemble(indices[:k],node,depth+1));put(node+12,assemble(indices[k+1:],node,depth+1));return node
 put(H,0);put(H+4,assemble(live,H));put(H+8,nodes[0]);put(H+12,nodes[-1])
 for i in live:
  put(nodes[i]+16,17+i*3);put(nodes[i]+20,objects[i]);put(objects[i]+0x1c,4);put(objects[i]+0x28,F if i in selected else F+0x100);put(objects[i]+0x20,overlaps[i])
 trace=[];lock=0;allocations=[]
 api={}
 for i,name in enumerate(('EnterCriticalSection','LeaveCriticalSection','GlobalFree')):
  api[API+i*16]=name;put(reverse[name],API+i*16)
 def ret(pop=0,value=0):
  sp=u.reg_read(UC_X86_REG_ESP);ra=get(sp);u.reg_write(UC_X86_REG_EAX,value);u.reg_write(UC_X86_REG_ESP,sp+4+pop);u.reg_write(UC_X86_REG_EIP,ra)
 def hook(uc,a,size,data):
  nonlocal lock
  sp=u.reg_read(UC_X86_REG_ESP);cx=u.reg_read(UC_X86_REG_ECX)
  if a in api:
   name=api[a]
   if name=='EnterCriticalSection':assert not lock and get(sp+4)==M+0x14c;lock=1;trace.append(('enter',get(M+0x16c)));ret(4)
   elif name=='LeaveCriticalSection':assert lock and get(sp+4)==M+0x14c;lock=0;trace.append(('leave',get(M+0x16c)));ret(4)
   else:
    i=overlaps.index(get(sp+4));assert lock and i in selected;trace.append(('overlap',i));ret(4)
  elif a==0xbfea0e:
   size=get(sp+4);assert lock and size==4*(1<<len(allocations)) and size<=32
   block=A+0x6000+len(allocations)*0x100;allocations.append((block,size));trace.append(('allocate',size));ret(value=block)
  elif a==0xbfeae6:
   dst,src,size=get(sp+4),get(sp+8),get(sp+12);assert lock and size<=32
   u.mem_write(dst,bytes(u.mem_read(src,size)));trace.append(('copy',size));ret(value=dst)
  elif a==0xbfe9bc:
   i=objects.index(get(sp+4));assert lock and i in selected;trace.append(('delete',i));ret()
  elif a==0xbfea14:
   value=get(sp+4);assert (value in nodes and lock) or value in [p for p,n in allocations]
   if value in nodes:live.remove(nodes.index(value))
   trace.append(('free',value));ret()
 u.hook_add(UC_HOOK_CODE,hook)
 sp=STACK+0x8000;put(sp,STOP);put(sp+4,F);u.reg_write(UC_X86_REG_ESP,sp);u.reg_write(UC_X86_REG_ECX,M)
 regs=(UC_X86_REG_EBX,UC_X86_REG_EBP,UC_X86_REG_ESI,UC_X86_REG_EDI)
 for i,reg in enumerate(regs):u.reg_write(reg,0x12340000+i)
 u.emu_start(0x98e7d0,STOP,count=10000)
 assert u.reg_read(UC_X86_REG_EIP)==STOP and u.reg_read(UC_X86_REG_ESP)==sp+8 and not lock
 assert all(u.reg_read(reg)==0x12340000+i for i,reg in enumerate(regs))
 assert get(M+0x16c)==seed and get(M+0x140)==count-len(selected)
 assert [x[1] for x in trace if x[0]=='delete']==selected
 def validate(node,parent,lower,upper):
  if not node:return [],1
  i=nodes.index(node);assert i in live and lower<i<upper and get(node+4)==parent
  left,right=get(node+8),get(node+12)
  if not get(node):assert (not left or get(left)==1) and (not right or get(right)==1)
  ls,lb=validate(left,node,lower,i);rs,rb=validate(right,node,i,upper);assert lb==rb
  return ls+[i]+rs,lb+int(bool(get(node)))
 tree=get(H+4);order,height=validate(tree,H,-1,count);assert order==live
 assert not tree or get(tree)==1
 assert get(H+8)==(nodes[live[0]] if live else H) and get(H+12)==(nodes[live[-1]] if live else H)
 assert bool(allocations)==bool(selected)
 if selected:assert [get(allocations[-1][0]+4*i) for i in range(len(selected))]==[17+i*3 for i in selected]
 assert [x[1] for x in trace if x[0]=='free' and x[1] in [p for p,n in allocations]]==[p for p,n in allocations]
 return bytes(u.mem_read(A,0x10000)),trace

cases=0
for count in (2,3,7):
 for mask in range(1<<count):
  for seed in (0,7):
   assert multiple(bytes(patched),count,mask,seed)==multiple(original,count,mask,seed),(count,mask,seed)
   cases+=1
report=dict(status='PASS',cases=cases,boundaries=['Finished requests only; all file-selection subsets for 2/3/7-node UID trees, stable snapshot and ascending cleanup order.','Full64KiB arena, ABI, locks, counter and survivor topology compared. Exact188B native vector overflow executes, including allocation growth, copies and release; exact572B tree erase, both64B rotations and two26B extrema helpers execute; only heap/Win32 boundaries controlled; no host or promotion claim.'])
out=build/('multiple-native-tree-'+__import__('datetime').datetime.now(__import__('datetime').timezone.utc).strftime('%H%M%S%f')+'.json');out.write_text(json.dumps(report,indent=2)+'\n');print(json.dumps(report,indent=2))
