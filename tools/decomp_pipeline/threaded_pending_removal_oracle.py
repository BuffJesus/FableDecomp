"""Actual pending tree removal with cursor, storage and ownership invariants."""
from pathlib import Path
import sys,json,struct,itertools,hashlib
R=Path(__file__).resolve().parents[2]
import threaded_submission_tree_oracle as T
import gate_native_threaded_file_manager_remove as G
from movable_pool_construct_oracle import compile_codes
from pe_oracle import EXE,read_virtual_data
from unicorn.x86_const import *
roots=[r for r in G.ROOTS if r['name'].startswith('pending_')]
for name,source,symbol,va,size in [('erase_balance','00/42/STLRbTree_RebalanceForErase_0042a34e.cpp','?_Rebalance_for_erase@?$_Rb_global@_N@_STL@@SAPAU_Rb_tree_node_base@2@PAU32@AAPAU32@11@Z',0x42a34e,572),('minimum','00/41/STLRbTree_Extrema_0041c7e0.cpp','?_S_minimum@_Rb_tree_node_base@_STL@@SAPAU12@PAU12@@Z',0x41c7e0,26),('maximum','00/41/STLRbTree_Extrema_0041c7e0.cpp','?_S_maximum@_Rb_tree_node_base@_STL@@SAPAU12@PAU12@@Z',0x41c800,26)]:roots.append(dict(name=name,source=R/'rebuild/src/compiled'/source,symbol=symbol,address=va,size=size))
roots.append(dict(name='remove_pending',source=R/'rebuild/src/compiled/00/98/CThreadedFileManager_RemoveObjectFromPendingMap_0098e770.cpp',symbol='?RemoveObjectFromPendingMap@CThreadedFileManager@@AAEXPAVCFileObject@1@@Z',address=0x98e770,size=91))
def build_codes(B):
 original,rebuilt,audit,objs=T.build_codes(B)
 addresses=dict(G.ADDRESS);addresses.update({r['symbol']:r['address'] for r in roots});addresses.update({r['symbol']:r['address'] for r in audit})
 extra=compile_codes(roots,addresses,B);raw=EXE.read_bytes()
 for r in roots:
  code=read_virtual_data(raw,r['address'],r['size'])
  if r['name']!='remove_pending':assert extra[r['address']]==code,(r['name'],len(extra[r['address']]))
  original[r['address']]=code;rebuilt[r['address']]=extra[r['address']]
 return original,rebuilt,audit,objs,addresses

class RemovalMachine(T.Machine):
 def __init__(self,codes,pattern,device,priority):
  common={a:b for a,b in codes.items() if 0x98f000<=a<0x992000 or 0x429000<=a<0x42a000};self.freed=[]
  super().__init__(common,True,pattern);self.codes=codes
  pages=set()
  for a,body in codes.items():
   if a not in common:
    for page in range(a&~4095,(a+len(body)+4095)&~4095,4096):pages.add(page)
  for page in sorted(pages):self.u.mem_map(page,4096);self.u.mem_write(page,b'\xcc'*4096)
  for a,body in codes.items():
   if a not in common:self.u.mem_write(a,body)
  self.manager=T.TREE-4-(device*7+priority)*16
  for n in range(4):self.put(T.FILES+n*32+4,device)
  for n in range(32):self.put(T.OBJECTS+n*64+20,priority)
  self.put(T.TREE+12,T.HDR);self.initial=bytes(self.u.mem_read(T.AR,0x10000))
 def hook(self,u,a,size,_):
  if a!=0xbfea14:return super().hook(u,a,size,_)
  sp=u.reg_read(UC_X86_REG_ESP);block=self.get(sp+4);assert block in self.blocks and block not in self.freed
  self.blocks.remove(block);self.freed.append(block);self.trace.append(('free',block));u.mem_write(block,b'\xdd'*24)
  u.reg_write(UC_X86_REG_EAX,0);u.reg_write(UC_X86_REG_ECX,0xdeadbeef);u.reg_write(UC_X86_REG_EDX,0x87654321);u.reg_write(UC_X86_REG_EIP,self.get(sp));u.reg_write(UC_X86_REG_ESP,sp+4)
 def remove(self,n,selection):
  key=self.keys[n];nodes=self.ordered();node=next(p for p in nodes if self.get(p+16)==key);index=nodes.index(node)
  cursor=node if selection==0 else T.HDR if selection==1 else nodes[0] if selection==2 else nodes[-1]
  expected_cursor=(nodes[index+1] if index+1<len(nodes) else T.HDR) if cursor==node else cursor;self.put(T.TREE+12,cursor)
  before=bytes(self.u.mem_read(T.AR,0x10000));old_frees=len(self.freed);sp=T.STACK+0x8000;self.put(sp,T.STOP);self.put(sp+4,key)
  saved=(UC_X86_REG_EBX,UC_X86_REG_ESI,UC_X86_REG_EDI,UC_X86_REG_EBP)
  for i,reg in enumerate(saved):self.u.reg_write(reg,0x55660000+i)
  self.u.reg_write(UC_X86_REG_ESP,sp);self.u.reg_write(UC_X86_REG_ECX,self.manager);self.u.emu_start(0x98e770,T.STOP,count=20000)
  assert self.u.reg_read(UC_X86_REG_EIP)==T.STOP and self.u.reg_read(UC_X86_REG_ESP)==sp+8
  assert all(self.u.reg_read(reg)==0x55660000+i for i,reg in enumerate(saved))
  assert self.get(T.TREE+12)==expected_cursor and len(self.freed)==old_frees+1 and self.freed[-1]==node
  del self.values[key];self.ordered();state=bytes(self.u.mem_read(T.AR,0x10000));model=bytearray(before)
  def copy(a,size):model[a-T.AR:a-T.AR+size]=state[a-T.AR:a-T.AR+size]
  copy(T.TREE+4,4);copy(T.TREE+12,4);copy(T.HDR+4,12)
  for block in self.blocks:copy(block,1);copy(block+4,20)
  model[node-T.AR:node-T.AR+24]=b'\xdd'*24;assert bytes(model)==state
  return hashlib.sha256(state).hexdigest(),list(self.trace),expected_cursor

def scenario(codes,pattern,device,priority,count,variant):
 m=RemovalMachine(codes,pattern,device,priority);indices=list(range(count))
 if pattern==1:indices.reverse()
 elif pattern>=2:indices=indices[::2]+indices[1::2][::-1]
 for step,n in enumerate(indices):m.call(n,(step+variant)%3,(step+variant)%4)
 removal=indices if variant==0 else indices[::-1] if variant==1 else indices[len(indices)//2:]+indices[:len(indices)//2]
 snapshots=[m.remove(n,(step+variant)%4) for step,n in enumerate(removal)];assert not m.blocks and not m.values and len(m.freed)==count
 return snapshots,m.seen
def compare_removal(original,rebuilt):
 cases=checks=0;witness={}
 for args in itertools.product(range(4),range(2),(1,3,6),(1,2,3,7,15),range(3)):
  a,seen=scenario(original,*args);b,builtseen=scenario(rebuilt,*args);assert a==b,args;cases+=1;checks+=len(a)
  for address in builtseen:witness.setdefault(address,args)
  if cases%120==0:print('remove model progress',cases,flush=True)
 controls=[]
 for r in roots:
  address=r['address'];assert address in witness,(r['name'],'not exercised');damaged=dict(rebuilt);damaged[address]=b'\xcc';detected=False
  try:scenario(damaged,*witness[address])
  except Exception:detected=True
  assert detected;controls.append(r['name'])
 report=dict(status='PASS',cases=cases,removals=checks,controls=controls,executed=[hex(a) for a in sorted(witness)],boundary='Real insertion/find/erase/rebalance/extrema, malloc/free controlled; predecessor remains existing functional-only variant. No new helper acceptance.')
 return report
