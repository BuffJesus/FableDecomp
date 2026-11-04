"""Actual threaded UID/pending insertion graph; predecessor is explicitly functional-only."""
from pathlib import Path
import sys,struct,json,itertools,hashlib
import check_one as V
from gate_game_component_abi import Coff
from pe_oracle import EXE,read_virtual_data
from near_match import compare,retail_relocations
from capstone import Cs,CS_ARCH_X86,CS_MODE_32,CS_OP_IMM
from unicorn import Uc,UC_ARCH_X86,UC_MODE_32,UC_HOOK_CODE
from unicorn.x86_const import *
R=Path(__file__).resolve().parents[2]
SOURCES=[R/'rebuild/src/compiled'/p for p in ('00/42/STL_Rb_global_Rebalance_0042971d.cpp','00/99/STLThreadedPendingMap_OperatorIndex_00991350.cpp','00/99/STLThreadedUIDMap_OperatorIndex_009913d0.cpp','00/99/STLThreadedMaps_InsertHint_00990dc0.cpp','00/99/STLThreadedMaps_InsertUnique_00990900.cpp','00/99/STLThreadedMaps_InsertNode_00990830.cpp')]
def build_codes(B):
 sources=SOURCES
 objects=[];definitions={}
 for i,src in enumerate(sources):
  obj=B/('closure%d.obj'%i);r=V.cl(['/nologo','/c','/O2','/MT','/I'+str(R/'rebuild/include'),'/Fo'+str(obj),str(src)],V.env(),B);assert r.returncode==0,r.stdout+r.stderr;objects.append(obj);(B/('compile%d.json'%i)).write_text(json.dumps(dict(source=str(src),returncode=r.returncode,stdout=r.stdout,stderr=r.stderr),indent=2)+'\n');c=Coff(obj)
  for sym,(sec,off) in c.symbols.items():
   if sec>0 and sym.startswith('?') and off==0:definitions.setdefault(sym,(i,*c.sections[sec]))
 roots=[]
 def select(prefix,pending,address,size=None):
  found=[(s,body) for s,(index,body,rels) in definitions.items() if s.startswith(prefix) and (pending is None or ('CPendingObjectSortKey' in s)==pending)]
  assert len(found)==1,(prefix,pending,found);sym,body=found[0];roots.append(dict(symbol=sym,address=address,size=size or len(body)))
 select('??A?$map@',True,0x991350,124);select('??A?$map@',False,0x9913d0,115)
 for prefix,yes,no,sy,sn in [('?insert_unique@',0x990dc0,0x991030,610,518),('?_M_insert@',0x990830,0x990a10,201,191)]:
  for pending,addr,size in ((True,yes,sy),(False,no,sn)):
   names=[s for s in definitions if s.startswith(prefix) and ('CPendingObjectSortKey' in s)==pending and (prefix!='?insert_unique@' or '@U32@' in s)]
   assert len(names)==1;roots.append(dict(symbol=names[0],address=addr,size=size))
 for pending,addr,size in ((True,0x990900,263),(False,0x990ad0,180)):
  names=[s for s in definitions if s.startswith('?insert_unique@') and ('CPendingObjectSortKey' in s)==pending and '@U32@' not in s];assert len(names)==1;roots.append(dict(symbol=names[0],address=addr,size=size))
 select('?_M_lower_bound@',True,0x9901d0,89);select('??R?$less@UCPending',None,0x98f570,64)
 select('??E?$_Rb_tree_iterator@',True,0x98f9d0,62);select('??E?$_Rb_tree_iterator@',False,0x98fa70,62)
 select('?_M_decrement@_Rb_tree_base_iterator',None,0x98fdd0,79)
 select('?_Rebalance@',None,0x42971d);select('?_Rotate_left@',None,0x42951b);select('?_Rotate_right@',None,0x42955b)
 addresses={r['symbol']:r['address'] for r in roots};addresses['_malloc']=0xbfea0e
 raw=EXE.read_bytes();assert read_virtual_data(raw,0x98fdd0,79)==read_virtual_data(raw,0x98fe20,79)
 md=Cs(CS_ARCH_X86,CS_MODE_32);md.detail=True
 originals={};rebuilt={};report=[]
 for root in roots:
  sym,va,size=root['symbol'],root['address'],root['size'];index,body,rels=definitions[sym];original=read_virtual_data(raw,va,size);patched=bytearray(body);audit=[]
  for site,(name,kind) in rels.items():
   assert kind==20 and struct.unpack_from('<I',body,site)[0]==0;target=addresses[name]
   if target==0x98fdd0 and sym.startswith('?insert_unique@') and 'CPendingObjectSortKey' not in sym:target=0x98fe20
   struct.pack_into('<I',patched,site,(target-va-site-4)&0xffffffff);audit.append(dict(site=site,symbol=name,target=target))
  calls=[i.operands[0].imm&0xffffffff for i in md.disasm(original,va) if i.mnemonic=='call'];assert sorted(calls)==sorted(x['target'] for x in audit),(sym,calls,audit)
  grade=compare(body,original,tuple(rels),retail_relocations('%08x'%va,size)).to_dict();assert grade['near_match'] or va==0x98fdd0,(sym,grade)
  if len(body)==size and grade['opcode_similarity']==1:assert bytes(patched)==original,sym
  originals[va]=original;rebuilt[va]=bytes(patched);report.append(dict(**root,source=str(sources[index]),index=index,body=body.hex(),bytes=len(body),grade='MATCH' if bytes(patched)==original else 'NEAR_MATCH' if grade['near_match'] else 'FUNCTIONAL_VARIANT',metrics=grade,relocations=audit))
 originals[0x98fe20]=originals[0x98fdd0];rebuilt[0x98fe20]=rebuilt[0x98fdd0]
 (B/'closure-audit.json').write_text(json.dumps(report,indent=2)+'\n')
 (B/'codes.json').write_text(json.dumps({'original':{hex(a):b.hex() for a,b in originals.items()},'rebuilt':{hex(a):b.hex() for a,b in rebuilt.items()}},indent=2)+'\n')
 return originals,rebuilt,report,objects

AR,TREE,HDR,KEY,VALUE,OUT,OBJECTS,FILES,NODES,STACK,STOP=0x2000000,0x2001000,0x2001100,0x2001200,0x2001240,0x2001300,0x2002000,0x2004000,0x2008000,0x3000000,0x4000000
class Machine:
 def __init__(self,codes,pending,pattern):
  self.u=Uc(UC_ARCH_X86,UC_MODE_32);u=self.u;self.pending=pending;self.codes=codes;self.seen=set();self.blocks=[];self.values={};self.trace=[];self.return_records=[]
  for a,n in ((0x98f000,0x3000),(0x429000,0x1000),(0xbfe000,0x1000),(AR,0x10000),(STACK,0x10000),(STOP,0x1000)):
   u.mem_map(a,n);u.mem_write(a,b'\xcc'*n)
  for a,data in codes.items():u.mem_write(a,data)
  u.mem_write(AR,b'\xa5'*0x10000);self.put(TREE,HDR);self.put(TREE+4,0);u.mem_write(HDR,b'\0');self.put(HDR+4,0);self.put(HDR+8,HDR);self.put(HDR+12,HDR)
  self.keys=[];self.rank={}
  for n in range(32):
   obj=OBJECTS+n*64;file=FILES+(n%4)*32;sort=(n%4 if pattern<3 else (n%4)//2);pos=n*3 if pattern<3 else n%3
   self.put(file+20,sort);self.put(obj+40,file);self.put(obj+4,pos)
   key=obj if pending else ((n*0x10000001)&0xffffffff if pattern==4 else n*7)
   self.keys.append(key);self.rank[key]=(sort,pos,obj) if pending else (key,)
  # A file's key is shared; derive ranks from final storage rather than n's loop temporary.
  if pending:
   for key in self.keys:self.rank[key]=(self.get(self.get(key+40)+20),self.get(key+4),key)
  self.initial=bytes(u.mem_read(AR,0x10000));u.hook_add(UC_HOOK_CODE,self.hook)
 def put(self,a,v):self.u.mem_write(a,struct.pack('<I',v&0xffffffff))
 def get(self,a):return struct.unpack('<I',self.u.mem_read(a,4))[0]
 def hook(self,u,a,size,_):
  if a in self.codes:self.seen.add(a)
  if a!=0xbfea0e:return
  sp=u.reg_read(UC_X86_REG_ESP);assert self.get(sp+4)==24
  result=NODES+len(self.blocks)*32;assert len(self.blocks)<32;self.blocks.append(result);self.trace.append(('malloc',result))
  u.reg_write(UC_X86_REG_EAX,result);u.reg_write(UC_X86_REG_ECX,0xdeadbeef);u.reg_write(UC_X86_REG_EDX,0x87654321);u.reg_write(UC_X86_REG_EIP,self.get(sp));u.reg_write(UC_X86_REG_ESP,sp+4)
 def ordered(self):
  visited=set()
  def walk(node,parent):
   if not node:return [],1
   assert node in self.blocks and node not in visited;visited.add(node);assert self.get(node+4)==parent
   color=self.u.mem_read(node,1)[0];assert color in (0,1);left,right=self.get(node+8),self.get(node+12)
   if color==0:
    assert not left or self.u.mem_read(left,1)==b'\1'
    assert not right or self.u.mem_read(right,1)==b'\1'
   lo,lb=walk(left,node);hi,hb=walk(right,node);assert lb==hb
   return lo+[node]+hi,lb+color
  nodes,_=walk(self.get(HDR+4),HDR);assert visited==set(self.blocks)
  if nodes:
   assert self.u.mem_read(self.get(HDR+4),1)==b'\1' and self.get(HDR+8)==nodes[0] and self.get(HDR+12)==nodes[-1]
  else:assert self.get(HDR+8)==HDR and self.get(HDR+12)==HDR
  keys=[self.get(n+16) for n in nodes];assert keys==sorted(self.values,key=self.rank.__getitem__);assert self.get(TREE+4)==len(self.values)
  assert all(self.get(n+20)==self.values[self.get(n+16)] for n in nodes)
  return nodes
 def call(self,n,method,hint_kind):
  key=self.keys[n];old=self.values.get(key);self.put(KEY,key);self.put(VALUE,key);self.put(VALUE+4,0x70000000+n)
  self.u.mem_write(OUT,b'\xa5'*16);nodes=self.ordered();hint=(nodes[0] if hint_kind==0 and nodes else nodes[-1] if hint_kind==1 and nodes else nodes[len(nodes)//2] if hint_kind==2 and nodes else HDR)
  entry=(0x991350 if self.pending else 0x9913d0) if method==0 else (0x990900 if self.pending else 0x990ad0) if method==1 else (0x990dc0 if self.pending else 0x991030)
  args=[KEY] if method==0 else [OUT,VALUE] if method==1 else [OUT,hint,VALUE]
  sp=STACK+0x8000;self.put(sp,STOP)
  for i,v in enumerate(args):self.put(sp+4+4*i,v)
  saved=(UC_X86_REG_EBX,UC_X86_REG_ESI,UC_X86_REG_EDI,UC_X86_REG_EBP)
  for i,reg in enumerate(saved):self.u.reg_write(reg,0x55660000+i)
  self.u.reg_write(UC_X86_REG_ESP,sp);self.u.reg_write(UC_X86_REG_ECX,TREE);self.u.emu_start(entry,STOP,count=10000)
  assert self.u.reg_read(UC_X86_REG_EIP)==STOP and self.u.reg_read(UC_X86_REG_ESP)==sp+4+4*len(args)
  assert all(self.u.reg_read(reg)==0x55660000+i for i,reg in enumerate(saved))
  result=self.u.reg_read(UC_X86_REG_EAX)
  if method==0:node=result-20
  else:
   assert result==OUT;node=self.get(OUT)
   if method==1:assert self.u.mem_read(OUT+4,1)[0]==int(old is None)
  assert node in self.blocks and self.get(node+16)==key
  assert self.get(node+20)==(old if old is not None else 0 if method==0 else 0x70000000+n)
  assert self.get(KEY)==key and self.get(VALUE)==key and self.get(VALUE+4)==0x70000000+n
  self.put(node+20,0x90000000+n);self.values[key]=0x90000000+n;self.ordered()
  state=bytes(self.u.mem_read(AR,0x10000));model=bytearray(self.initial)
  def copy(a,size):model[a-AR:a-AR+size]=state[a-AR:a-AR+size]
  copy(TREE+4,4);copy(HDR+4,12);copy(KEY,4);copy(VALUE,8)
  if method:copy(OUT,4)
  if method==1:copy(OUT+4,1)
  for block in self.blocks:copy(block,1);copy(block+4,20)
  assert bytes(model)==state
  return state,list(self.trace),result

def scenario(codes,pending,pattern,count,variant):
 m=Machine(codes,pending,pattern);indices=list(range(count))
 if pattern==1:indices.reverse()
 elif pattern==2:indices=indices[::2]+indices[1::2][::-1]
 elif pattern==3:indices=sorted(indices,key=lambda n:(n*13)%31)
 sequence=indices+indices[::-1]+indices[::2];snapshots=[]
 for step,n in enumerate(sequence):
  state,trace,result=m.call(n,(step+variant)%3,(step+variant)%4);snapshots.append((hashlib.sha256(state).hexdigest(),trace,result))
 return snapshots,m.seen

def compare_trees(original,rebuilt):
 cases=checkpoints=0;witness={};union=set()
 for args in itertools.product((False,True),range(5),(1,2,3,7,15,31),range(3)):
  a,seen=scenario(original,*args);b,builtseen=scenario(rebuilt,*args);assert a==b,args;cases+=1;checkpoints+=len(a);union|=builtseen
  for address in builtseen:witness.setdefault(address,args)
  if cases%60==0:print('tree model progress',cases,flush=True)
 controls=[]
 for address in sorted(witness):
  damaged=dict(rebuilt);damaged[address]=b'\xcc';detected=False
  try:scenario(damaged,*witness[address])
  except Exception:detected=True
  assert detected;controls.append(hex(address))
 report=dict(status='PASS',cases=cases,checkpoints=checkpoints,executed=[hex(a) for a in sorted(union)],controls=controls,boundaries=['Only malloc24 is controlled; actual lookup/insert/link/rotation/rebalance/iterator code executes. Full64KiB original/rebuilt snapshots and independent ordered keys/values, count, parent links, color/black height, allocation and untouched guards.','Iterator predecessor78B remains a reused FUNCTIONAL_VARIANT, not an accepted near/exact root; two direct source forms deferred. No native or production submission-to-worker claim.'])

 return report
