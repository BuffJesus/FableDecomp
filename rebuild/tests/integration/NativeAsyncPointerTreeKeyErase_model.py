"""Independent key erase corpus with canonical RB deletion model reuse."""
import importlib.util,itertools,struct,sys
from pathlib import Path
from unicorn.x86_const import *
R=next(p for p in Path(__file__).resolve().parents if (p/'rebuild').is_dir())
MODEL=R/'rebuild/tests/integration/NativeAsyncPointerTreeRangeErase_model.py'
spec=importlib.util.spec_from_file_location('async_key_independent_rb_model',MODEL)
f=importlib.util.module_from_spec(spec);spec.loader.exec_module(f)
KEYROOT=0x9d7f40;BOUNDS=0x9d7c80;DISTANCE=0x9d7550
class Runner(f.Runner):
 def run(self,initial,c):
  u=self.u;u.context_restore(self.cpu);u.mem_write(f.A,initial);u.mem_write(f.S,bytes(65536));m=f.Memory(u=u);sp=f.S+0x8000;events=[];snaps=[m.snap()]
  u.mem_write(sp,f.word(f.STOP)+f.word(c['keyp']));u.reg_write(UC_X86_REG_ESP,sp);u.reg_write(UC_X86_REG_ECX,c['tree']);regs=(UC_X86_REG_EBX,UC_X86_REG_EBP,UC_X86_REG_ESI,UC_X86_REG_EDI)
  for i,r in enumerate(regs):u.reg_write(r,0x11223300+i)
  def hook(uc,pc):
   if hex(pc) in self.hits:self.hits[hex(pc)]+=1
   if pc==f.STOP:uc.emu_stop();return
   if pc!=f.FREE:return
   stack=uc.reg_read(UC_X86_REG_ESP);p=struct.unpack('<I',uc.mem_read(stack+4,4))[0];assert p in c['nodeaddresses'];f.free(m,c,p,events,snaps)
   ret=struct.unpack('<I',uc.mem_read(stack,4))[0];uc.reg_write(UC_X86_REG_EAX,0xabcdef00);uc.reg_write(UC_X86_REG_ECX,0x98765432);uc.reg_write(UC_X86_REG_EDX,0x87654321);uc.reg_write(UC_X86_REG_ESP,stack+4);uc.reg_write(UC_X86_REG_EIP,ret)
  self.active=hook;u.emu_start(KEYROOT,f.STOP,count=20000)
  assert u.reg_read(UC_X86_REG_EIP)==f.STOP and u.reg_read(UC_X86_REG_ESP)==sp+8
  assert [u.reg_read(r) for r in regs]==[0x11223300+i for i in range(4)]
  assert u.reg_read(UC_X86_REG_EAX)==len(c['erasekeys'])
  class StructuralView:
   def w(self,p):return len(c['remaining']) if p==c['tree']+4 else m.w(p)
   def read(self,p,n):return m.read(p,n)
  f.validate_arena(StructuralView(),c['tree'],m.w(c['tree']),c['remaining']);snaps.append(m.snap());return events,snaps

def run_domain(original,compiled):
 runners={'original':Runner(original),'compiled':Runner(compiled)}
 sequences=[(),(0,),(0,1),(1,0),*itertools.permutations(range(4)),tuple(range(8)),tuple(reversed(range(8))),(4,2,6,1,3,5,7,0)]
 cases=points=matched=absent=0;partitions={}
 for sequence,high,place,fill,mutation in itertools.product(sequences,(False,True),range(2),(0,0x95),(0,1,3)):
  values=[i+(0x80000000 if high else 0) for i in sequence]
  queries=set(values)|{0,0xffffffff,0x80000000,0x7fffffff,9+(0x80000000 if high else 0)}
  for key in sorted(queries):
   base=f.A+place*0x3000;initial=bytearray([fill])*f.N;m=f.Memory(initial);tree=f.Tree()
   for value in values:tree.insert(value,base+0x1000+len(tree.nodes)*32)
   owner=base+0x100;header=base+0x200;tree.serialize(m,owner,header);keyp=base+0x700;m.put(keyp,key)
   ordered=sorted(tree.nodes);lower=next((tree.nodes[k].address for k in ordered if k>=key),header);upper=next((tree.nodes[k].address for k in ordered if k>key),header)
   erasekeys=[key] if key in tree.nodes else []
   c=dict(tree=owner,header=header,marker=base+0x500,keyp=keyp,first=lower,last=upper,erasekeys=erasekeys,nodeaddresses={n.address for n in tree.nodes.values()},remaining=set(ordered)-set(erasekeys),mutation=mutation)
   snapshot=bytes(initial);wanted=f.expected(snapshot,c,tree)
   for name,runner in runners.items():assert runner.run(snapshot,c)==wanted,(name,sequence,key,mutation)
   cases+=1;points+=len(wanted[1]);matched+=bool(erasekeys);absent+=not bool(erasekeys);partitions[mutation]=partitions.get(mutation,0)+1
 assert all(r.hits[hex(KEYROOT)]==cases and r.hits[hex(BOUNDS)]==cases and r.hits[hex(DISTANCE)]==cases and r.hits[hex(f.ENTRY)]==cases for r in runners.values())
 assert runners['original'].hits==runners['compiled'].hits

 return dict(cases_per_mode=cases,snapshots_per_mode=points,matched=matched,absent=absent,mutation_partitions=partitions,provider_hits={name:r.hits for name,r in runners.items()})

class EqualRangeRunner(f.Runner):
 def run_equal(self,initial,owner,keyp,out,stackfill):
  u=self.u;u.context_restore(self.cpu);u.mem_write(f.A,initial);u.mem_write(f.S,bytes([stackfill])*65536);sp=f.S+0x8000
  u.mem_write(sp,f.word(f.STOP)+f.word(out)+f.word(keyp));u.reg_write(UC_X86_REG_ESP,sp);u.reg_write(UC_X86_REG_ECX,owner)
  regs=(UC_X86_REG_EBX,UC_X86_REG_EBP,UC_X86_REG_ESI,UC_X86_REG_EDI)
  for i,r in enumerate(regs):u.reg_write(r,0x11223300+i)
  def hook(uc,pc):
   if hex(pc) in self.hits:self.hits[hex(pc)]+=1
   if pc==f.STOP:uc.emu_stop()
   else:assert pc!=f.FREE
  self.active=hook;u.emu_start(BOUNDS,f.STOP,count=2000)
  assert u.reg_read(UC_X86_REG_EIP)==f.STOP and u.reg_read(UC_X86_REG_ESP)==sp+12
  assert u.reg_read(UC_X86_REG_EAX)==out and [u.reg_read(r) for r in regs]==[0x11223300+i for i in range(4)]
  return bytes(u.mem_read(f.A,f.N))

def run_equal_domain(original,compiled):
 runners={'original':EqualRangeRunner({BOUNDS:original[BOUNDS]}),'compiled':EqualRangeRunner({BOUNDS:compiled[BOUNDS]})}
 cases=0;aliases={};sequences=[(),(0,),(0,1,2,3),(3,2,1,0),(2,0,3,1),(0,0x7fffffff,0x80000000,0xffffffff)]
 for sequence,place,fill,stackfill,alias in itertools.product(sequences,range(2),(0,0x95),(0,0xa3),('separate','key','tree','header')):
  for key in sorted(set(sequence)|{0,1,2,4,0x7fffffff,0x80000000,0xffffffff}):
   base=f.A+place*0x3000;initial=bytearray([fill])*f.N;m=f.Memory(initial);tree=f.Tree()
   for value in sequence:tree.insert(value,base+0x1000+len(tree.nodes)*32)
   owner=base+0x100;header=base+0x200;keyp=base+0x700;tree.serialize(m,owner,header);m.put(keyp,key)
   out={'separate':base+0x800,'key':keyp,'tree':owner,'header':header}[alias]
   ordered=sorted(tree.nodes)
   lower=next((tree.nodes[k].address for k in ordered if k>=key),header)
   upper=next((tree.nodes[k].address for k in ordered if k>key),header)
   want=bytearray(initial);want[out-f.A:out-f.A+8]=f.word(lower)+f.word(upper)
   for name,runner in runners.items():assert runner.run_equal(bytes(initial),owner,keyp,out,stackfill)==bytes(want),(name,sequence,key,alias)
   cases+=1;aliases[alias]=aliases.get(alias,0)+1
 assert runners['original'].hits==runners['compiled'].hits=={hex(BOUNDS):cases}
 return dict(cases_per_mode=cases,full_arena_snapshots_per_mode=cases,alias_partitions=aliases,provider_hits={name:r.hits for name,r in runners.items()})
