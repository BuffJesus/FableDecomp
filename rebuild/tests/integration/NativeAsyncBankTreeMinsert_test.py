"""Independent original179B child; malloc/rebalance functional interfaces."""
import hashlib,itertools,json,struct,sys,re,subprocess,tempfile
from pathlib import Path
from unicorn import Uc,UcError,UC_ARCH_X86,UC_MODE_32,UC_HOOK_CODE,UC_HOOK_MEM_INVALID,UC_MEM_WRITE_UNMAPPED,UC_ERR_WRITE_UNMAPPED
from unicorn.x86_const import *
from capstone import Cs,CS_ARCH_X86,CS_MODE_32
R=Path(__file__).resolve().parents[3];sys.path.insert(0,str(R/'tools/decomp_pipeline'))
from pe_oracle import EXE,read_virtual_data
from gate_game_component_abi import Coff
from near_match import compare,retail_relocations
import check_one as V
ENTRY=0x9d7990;MALLOC=0xbfea0e;REBALANCE=0x42971d;A=0x2000000;N=16384;S=0x3000000;STOP=0x4000000
raw=EXE.read_bytes();body=read_virtual_data(raw,ENTRY,179)
assert hashlib.sha256(body).hexdigest()=='cb4c9f46eb1999b08c6deae98a3874a1962a1215db4571bc67093d37ae4aeb35'
source=R/'rebuild/src/compiled/00/9d/STLAsyncBankTree_Minsert_009d7990.cpp'
required=Path(__file__).with_name('NativeAsyncBankTreeMinsert_required.cpp')
assert len(sys.argv)==3,'Usage: fixture OBJECT REPORT'
obj=Path(sys.argv[1]);report_path=Path(sys.argv[2])
symbol='?_M_insert@?$_Rb_tree@PAVCBankFileAsyncData@@PAV1@U?$_Identity@PAVCBankFileAsyncData@@@_STL@@U?$less@PAVCBankFileAsyncData@@@3@V?$allocator@PAVCBankFileAsyncData@@@3@@_STL@@QAE?AU?$_Rb_tree_iterator@PAVCBankFileAsyncData@@@2@PAU_Rb_tree_node_base@2@0ABQAVCBankFileAsyncData@@0@Z'
rebalance='?_Rebalance@?$_Rb_global@_N@_STL@@SAXPAU_Rb_tree_node_base@2@AAPAU32@@Z'
c=Coff(obj);s,o=c.symbols[symbol];b,rels=c.sections[s];assert o==0 and len(b)==179
assert rels=={43:('_malloc',20),81:('_malloc',20),153:(rebalance,20)}
targets={'_malloc':0xbfea0e,rebalance:0x42971d};patched=bytearray(b);audit=[]
for site,(name,kind) in rels.items():
 assert struct.unpack_from('<I',b,site)[0]==0
 struct.pack_into('<I',patched,site,(targets[name]-ENTRY-site-4)&0xffffffff);audit.append(dict(offset=site,symbol=name,kind=kind,addend=0,target=hex(targets[name])))
assert bytes(patched)==body
grade=compare(b,body,tuple(rels),retail_relocations('009d7990',179)).to_dict()
assert grade['complete_decode'] and grade['opcode_similarity']==1.0 and grade['instruction_delta']==0
helpers={}
for name,(section,offset) in c.symbols.items():
 if not name.startswith('?'):continue
 bb,rr=c.sections[section];assert offset==0
 for site,(target,kind) in rr.items():assert kind==20 and target in targets and struct.unpack_from('<I',bb,site)[0]==0,(name,site,target,kind)
 helpers[name]={'bytes':len(bb),'sha256':hashlib.sha256(bb).hexdigest(),'relocations':[(site,target,kind,0) for site,(target,kind) in rr.items()]}
assert len(helpers)==11
inputs={Path(__file__),source,required,obj,R/'tools/decomp_pipeline/gate_native_async_bank_tree_minsert.py'}
seen=set()
def headers(path):
 if path in seen:return
 seen.add(path);inputs.add(path)
 for name in re.findall(r'^\s*#\s*include\s*"([^"]+)"',path.read_text(errors='replace'),re.M):
  q=next((q for q in (path.parent/name,R/'rebuild/include'/name) if q.is_file()),None);assert q,(path,name);headers(q)
headers(source);headers(required)
for n in ('check_one.py','gate_game_component_abi.py','pe_oracle.py','variable_allocator_parity.py','near_match.py','verify_and_land.py','crawl/rowtrim.py','crawl/purity.py'):inputs.add(R/'tools/decomp_pipeline'/n)
inputs.add(R/'tools/artifact_layout.py')
build=Path(tempfile.mkdtemp(prefix='native-pointer-tree-required-'));ro=build/'required.obj'
result=V.cl(['/nologo','/c','/O2','/MT','/I'+str(R/'rebuild/include'),'/Fo'+str(ro),str(required)],V.env(),build);assert result.returncode==0,result.stdout+result.stderr
cmd=[str(V.VC/'bin/link.exe'),'/nologo','/out:'+str(build/'required.exe'),'kernel32.lib',str(ro)]
missing=subprocess.run(cmd,env=V.env(),capture_output=True,text=True);assert missing.returncode and any(symbol in line and ('LNK2001' in line or 'LNK2019' in line) for line in (missing.stdout+missing.stderr).splitlines()),missing.stdout+missing.stderr
control=subprocess.run(cmd+[str(obj)],env=V.env(),capture_output=True,text=True);assert control.returncode==0,control.stdout+control.stderr
def hashes():return {(str(p.relative_to(R)) if p.is_relative_to(R) else str(p)):hashlib.sha256(p.read_bytes()).hexdigest() for p in inputs}
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
def callback(m,c,name,args,events,snaps):
 events.append((name,tuple(args)));snaps.append(m.snap());tree=c['tree'];mode=c['malloc_mutation'];node=c['node'];header2=c['header2']
 if name=='malloc':
  assert args==(20,)
  if mode in (1,4):m.put(c['value'],0xffffffff)
  if mode in (2,4):m.put(tree,header2);m.put(tree+4,0xfffffffe)
  if mode in (3,4):
   m.put(c['y']+8,c['other']);m.put(c['y']+12,c['other']);m.put(header2+8,c['y']);m.put(header2+12,c['y'])
  return 0 if c['nullmalloc'] else node
 assert name=='rebalance' and args==(node,m.w(tree)+4)
 mode=c['rebalance_mutation']
 if mode==1:m.put(tree+4,0xffffffff)
 if mode==2:m.put(tree,header2);m.put(tree+4,0)
 if mode==3:m.put(args[1],c['other']);m.write(node,b'\1');m.put(node+8,c['other']);m.put(node+16,0xcafebabe)
 if mode==4:m.put(c['output'],0xdeadbeef);m.put(c['output']+4,0xbaadf00d)
 return 0xabcdef01
def model(initial,c):
 m=Memory(bytearray(initial));events=[];snaps=[];tree=c['tree'];y=c['y'];v=c['value'];header=m.w(tree)
 left=y==header or (not c['w'] and (c['x']!=0 or m.w(v)<m.w(y+16)))
 node=callback(m,c,'malloc',(20,),events,snaps)
 if not node:snaps.append(m.snap());return events,snaps,None,0x9d79f1 if left else 0x9d79cb
 # Actual placement construction reads live referenced value AFTER allocation.
 m.put(node+16,m.w(v))
 if left:
  m.put(y+8,node);header=m.w(tree)
  if y==header:m.put(header+4,node);m.put(m.w(tree)+12,node)
  elif y==m.w(header+8):m.put(header+8,node)
 else:
  m.put(y+12,node);header=m.w(tree)
  if y==m.w(header+12):m.put(header+12,node)
 m.put(node+4,y);m.put(node+8,0);m.put(node+12,0)
 callback(m,c,'rebalance',(node,m.w(tree)+4),events,snaps);m.put(tree+4,m.w(tree+4)+1);m.put(c['output'],node)
 snaps.append(m.snap());return events,snaps,c['output'],None
u=Uc(UC_ARCH_X86,UC_MODE_32)
for page in {p&~4095 for p in (ENTRY,MALLOC,REBALANCE)}|{(MALLOC&~4095)+4096}:u.mem_map(page,4096)
u.mem_map(A,N);u.mem_map(S,65536);u.mem_map(STOP,4096);u.mem_write(ENTRY,body)
active=[None];invalid=[None];u.hook_add(UC_HOOK_CODE,lambda uc,pc,n,_:active[0](uc,pc));u.hook_add(UC_HOOK_MEM_INVALID,lambda uc,access,address,n,value,_:invalid[0](uc,access,address,n));cpu=u.context_save();regs=(UC_X86_REG_EBX,UC_X86_REG_EBP,UC_X86_REG_ESI,UC_X86_REG_EDI)
execution_hits={mode:0 for mode in ('original','compiled')};execution_mode=[None]
def run(initial,c):
 u.context_restore(cpu);u.mem_write(A,initial);u.mem_write(S,bytes(65536));m=Memory(u=u);events=[];snaps=[];fault=[None];sp=S+0x8000
 u.mem_write(sp,word(STOP)+b''.join(word(c[k]) for k in ('output','x','y','value','w')));u.reg_write(UC_X86_REG_ESP,sp);u.reg_write(UC_X86_REG_ECX,c['tree'])
 for i,r in enumerate(regs):u.reg_write(r,0x11223300+i)
 def hook(uc,pc):
  if pc==ENTRY:execution_hits[execution_mode[0]]+=1
  if pc==STOP:uc.emu_stop();return
  if pc not in (MALLOC,REBALANCE):return
  stack=uc.reg_read(UC_X86_REG_ESP);n=1 if pc==MALLOC else 2;args=struct.unpack('<'+'I'*n,uc.mem_read(stack+4,n*4));result=callback(m,c,'malloc' if pc==MALLOC else 'rebalance',args,events,snaps);ret=struct.unpack('<I',uc.mem_read(stack,4))[0]
  uc.reg_write(UC_X86_REG_EAX,result);uc.reg_write(UC_X86_REG_ECX,0xabcdef01);uc.reg_write(UC_X86_REG_EDX,0x98765432);uc.reg_write(UC_X86_REG_ESP,stack+4);uc.reg_write(UC_X86_REG_EIP,ret)
 def on_invalid(uc,access,address,n):
  pc=uc.reg_read(UC_X86_REG_EIP);assert access==UC_MEM_WRITE_UNMAPPED and address==16 and n==4 and pc in (0x9d79cb,0x9d79f1) and c['nullmalloc']
  instruction=next(Cs(CS_ARCH_X86,CS_MODE_32).disasm(bytes(uc.mem_read(pc,4)),pc));assert instruction.mnemonic=='mov' and instruction.op_str=='dword ptr [ecx], edx' and uc.reg_read(UC_X86_REG_ECX)==16
  fault[0]=pc;return False
 active[0]=hook;invalid[0]=on_invalid
 try:u.emu_start(ENTRY,STOP,count=1000)
 except UcError as error:assert error.errno==UC_ERR_WRITE_UNMAPPED and fault[0] is not None
 result=None
 if fault[0] is None:
  assert u.reg_read(UC_X86_REG_EIP)==STOP and u.reg_read(UC_X86_REG_ESP)==sp+24
  assert [u.reg_read(r) for r in regs]==[0x11223300+i for i in range(4)];result=u.reg_read(UC_X86_REG_EAX);assert result==c['output']
 snaps.append(m.snap());return events,snaps,result,fault[0]
original_run=run
def run(initial,c):
 execution_mode[0]='original';u.mem_write(ENTRY,body);u.ctl_remove_cache(ENTRY,ENTRY+len(body));retail=original_run(initial,c)
 execution_mode[0]='compiled';u.mem_write(ENTRY,bytes(patched));u.ctl_remove_cache(ENTRY,ENTRY+len(patched));compiled=original_run(initial,c)
 assert compiled==retail,(c,compiled[0],retail[0],compiled[2:],retail[2:])
 return compiled
cases=points=0;faults={hex(p):0 for p in (0x9d79cb,0x9d79f1)}
def case(partition,key,parentkey,count,nullmalloc,place,fill,malloc_mutation=0,rebalance_mutation=0):
 global cases,points
 base=A+place*0x2000;initial=bytearray([fill]*N);m=Memory(initial);tree=base+0x100;header=base+0x200;header2=base+0x400;y=header if partition==0 else base+0x600;other=base+0x800;node=base+0xa00;value=base+0xc00;output=base+0xe00
 m.put(tree,header);m.put(tree+4,count);m.put(header+4,y);m.put(header+8,y if partition in (0,4) else other);m.put(header+12,y if partition in (0,5) else other)
 m.put(header2+4,other);m.put(header2+8,y if partition==4 else other);m.put(header2+12,y if partition==5 else other);m.put(y+16,parentkey);m.put(value,key)
 c=dict(tree=tree,header=header,header2=header2,y=y,other=other,node=node,value=value,output=output,x=other if partition in (1,2,4) else 0,w=other if partition in (2,5) else 0,nullmalloc=nullmalloc,malloc_mutation=malloc_mutation,rebalance_mutation=rebalance_mutation)
 expected=model(initial,c);actual=run(bytes(initial),c);assert actual==expected,(c,actual[0],expected[0],actual[2:],expected[2:]);cases+=1;points+=len(expected[1])
 if expected[3] is not None:faults[hex(expected[3])]+=1
def main():
 bits=(0,1,0x7fffffff,0x80000000,0xffffffff)
 for args in itertools.product(range(6),bits,bits,(0,1,0xffffffff),range(2),range(2),(0,0x95)):case(*args)
 baseline=cases
 for partition,nullmalloc,place,fill,mm,rm in itertools.product(range(6),range(2),range(2),(0,0x95),range(5),range(5)):case(partition,0x80000000,0x7fffffff,0xffffffff,nullmalloc,place,fill,mm,rm)
 assert before==hashes()
 assert execution_hits=={'original':cases,'compiled':cases},execution_hits
 report=dict(status='NATIVE_ASYNC_BANK_TREE_MINSERT_MODEL_COMPILED_PASS',cases=cases,snapshots=points,baseline_cases=baseline,supplement_cases=cases-baseline,null_malloc_write16_faults=faults,body_sha256=hashlib.sha256(body).hexdigest(),input_sha256_before=before,input_sha256_after=hashes(),original_exe_sha256=hashlib.sha256(raw).hexdigest(),boundary='Original179B root only. malloc20/cdecl and rebalance(node,rootref)/cdecl explicit callbacks; no actual balancing/heap lifetime. Full16KiB arena boundary/final snapshots, stack excluded. Cached initialbranch/parent vs live postmalloc value/header/links; postrebalance livecount wrap and output publication. Nullmalloc exactWRITE_UNMAPPED16 at9D79CB/right or9D79F1/left; fault paths no successful ABI claim. Normal EAXhiddeniterator/RET14/callee-regs checked. Freshnonnull boundednode only, exoticallocationFFFFFFF0 excluded. No candidate behavior/sourceacceptance/promotion.')
 report.update(execution_hits=execution_hits,executable_cache_invalidated=True,compiled_root_sha256=hashlib.sha256(patched).hexdigest(),compiled_grade=grade,relocation_audit=audit,emitted_sections=helpers,executed_outlined_helpers=0,source_omission={'symbol':symbol,'missing_failed':True,'provider_link_passed':True})
 report['boundary']='Original and fresh compiled exact179B roots against independent model; malloc20/rebalance explicit cdecl callbacks. All11 emitted sections audited, only root executed; zero extra provider credit. Full16KiB snapshots per root/model; unique4800cases, exactnullWRITE16 faults, successful hiddeniterator/EAX/RET14/registerABI. No real balancing/heap/tree lifetime or registration closure claim.'
 assert (cases,points)==(4800,12000) and faults=={'0x9d79cb':1080,'0x9d79f1':1320}
 report_path.write_text(json.dumps(report,indent=2)+'\n');print('NATIVE_ASYNC_BANK_TREE_MINSERT_BEHAVIOR_PASS',cases,points,faults,flush=True)
if __name__=='__main__':main()
