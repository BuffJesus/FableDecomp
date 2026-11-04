"""Prepared standalone original79/emitted78 branch fixture; not registration credit."""
import hashlib,importlib.util,itertools,json,struct,sys
from pathlib import Path
from unicorn import Uc,UC_ARCH_X86,UC_MODE_32
from unicorn.x86_const import *
R=Path(__file__).resolve().parents[3];sys.path.insert(0,str(R/'tools/decomp_pipeline'))
from gate_game_component_abi import Coff
from pe_oracle import EXE,read_virtual_data
parent=Path(__file__).with_name('NativeAsyncRegistration_model.py')
spec=importlib.util.spec_from_file_location('registration_model',parent);module=importlib.util.module_from_spec(spec);spec.loader.exec_module(module)
A=module.A;N=module.N;S=module.S;STOP=module.STOP
def main():
 assert RUN_AUTHORIZED, 'Gate-controlled standalone execution'
 obj=FRESH_OBJECT
 c=Coff(obj);symbol='?_M_decrement@_Rb_tree_base_iterator@_STL@@QAEXXZ';s,o=c.symbols[symbol];compiled,rels=c.sections[s];assert o==0 and len(compiled)==78 and not rels
 original=read_virtual_data(EXE.read_bytes(),0x9d74e0,79);assert hashlib.sha256(original).hexdigest()=='2444342210bbf0539e77602ef04cadcf5b615c598b15c6879531efafdbeef3c8'
 inputs=(Path(__file__),parent,obj,R/'tools/decomp_pipeline/pe_oracle.py',R/'tools/decomp_pipeline/gate_game_component_abi.py')
 def hashes():return {str(q.relative_to(R)) if q.is_relative_to(R) else str(q):hashlib.sha256(q.read_bytes()).hexdigest() for q in inputs}
 before=hashes();u=Uc(UC_ARCH_X86,UC_MODE_32);u.mem_map(0x9d7000,4096);u.mem_map(0x6000000,4096);u.mem_map(A,N);u.mem_map(S,65536);u.mem_map(STOP,4096);u.mem_write(0x9d74e0,original);u.mem_write(0x6000000,compiled);cpu=u.context_save()
 regs=(UC_X86_REG_EBX,UC_X86_REG_EBP,UC_X86_REG_ESI,UC_X86_REG_EDI)
 def run(initial,iterator,entry):
  u.context_restore(cpu);u.mem_write(A,initial);u.mem_write(S,bytes(65536));sp=S+0x8000;u.mem_write(sp,module.word(STOP));u.reg_write(UC_X86_REG_ESP,sp);u.reg_write(UC_X86_REG_ECX,iterator)
  for i,r in enumerate(regs):u.reg_write(r,0x12345600+i)
  u.emu_start(entry,STOP,count=500)
  assert u.reg_read(UC_X86_REG_EIP)==STOP and u.reg_read(UC_X86_REG_ESP)==sp+4
  assert [u.reg_read(r) for r in regs]==[0x12345600+i for i in range(4)]
  return bytes(u.mem_read(A,N))
 cases=0;branches={}
 for shape,place,fill in itertools.product(('header','left_leaf','left_right_chain','parent_direct','parent_climb'),range(2),(0,0x95)):
  initial=bytearray([fill])*N;m=module.Memory(initial);base=A+place*0x3000;h=base+0x200;n=base+0x400;l=base+0x500;r=base+0x600;top=base+0x700;iterator=base+0x1000
  for node in (h,n,l,r,top):m.write(node,b'\x01');m.put(node+4,h);m.put(node+8,0);m.put(node+12,0)
  m.write(h,b'\x00');m.put(h+4,top);m.put(top+4,h);m.put(h+12,r)
  if shape=='header':start=h
  elif shape.startswith('left'):
   start=n;m.put(n+4,top);m.put(n+8,l);m.put(l+4,n)
   if shape=='left_right_chain':m.put(l+12,r);m.put(r+4,l)
  elif shape=='parent_direct':start=n;m.put(n+4,top);m.put(top+12,n)
  else:start=n;m.put(n+4,l);m.put(l+8,n);m.put(l+4,top);m.put(top+8,l);m.put(top+4,r);m.put(r+12,top)
  m.put(iterator,start);expected=module.Memory(bytearray(initial));expected.put(iterator,module.predecessor(expected,start));target=expected.snap()
  assert run(bytes(initial),iterator,0x9d74e0)==target,(shape,'original')
  assert run(bytes(initial),iterator,0x6000000)==target,(shape,'compiled')
  cases+=1;branches[shape]=branches.get(shape,0)+1
 assert before==hashes()
 report=dict(status='STANDALONE_DECREMENT_ORIGINAL_COMPILED_MODEL_PASS',cases=cases,fullarena_final_comparisons=cases*2,scenarios=branches,input_sha256_before=before,input_sha256_after=hashes(),original_sha256=hashlib.sha256(original).hexdigest(),compiled_sha256=hashlib.sha256(compiled).hexdigest(),boundary='Standalone original79B and actual emitted78B iterator methods vs independent predecessor model. Synthetic compiled6000000, no helper retail mapping/parity/provider credit or registration branch claim. Void thiscall RET0/callee-save/fullarena effects, all paths bounded; no concurrency/invalidpointer guarantees.')
 Path(__file__).with_name('NativeAsyncRegistration_decrement_result.json').write_text(json.dumps(report,indent=2)+'\n');print(report['status'],cases,flush=True)
if __name__=='__main__':main()
