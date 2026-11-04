def fresh_inventory():
 """Fresh canonical five-source definition/parity inventory; no execution."""
 import hashlib,json,re,struct,sys,tempfile
 from pathlib import Path
 from capstone import Cs,CS_ARCH_X86,CS_MODE_32
 R=Path(__file__).resolve().parents[3];sys.path.insert(0,str(R/'tools/decomp_pipeline'))
 import check_one as V
 from gate_game_component_abi import Coff
 from near_match import compare,retail_relocations
 from pe_oracle import EXE,read_virtual_data
 ROOT='?insert_unique@?$_Rb_tree@PAVCBankFileAsyncData@@PAV1@U?$_Identity@PAVCBankFileAsyncData@@@_STL@@U?$less@PAVCBankFileAsyncData@@@3@V?$allocator@PAVCBankFileAsyncData@@@3@@_STL@@QAE?AU?$pair@U?$_Rb_tree_iterator@PAVCBankFileAsyncData@@@_STL@@_N@2@ABQAVCBankFileAsyncData@@@Z'
 INSERT='?_M_insert@?$_Rb_tree@PAVCBankFileAsyncData@@PAV1@U?$_Identity@PAVCBankFileAsyncData@@@_STL@@U?$less@PAVCBankFileAsyncData@@@3@V?$allocator@PAVCBankFileAsyncData@@@3@@_STL@@QAE?AU?$_Rb_tree_iterator@PAVCBankFileAsyncData@@@2@PAU_Rb_tree_node_base@2@0ABQAVCBankFileAsyncData@@0@Z'
 DEC='?_M_decrement@_Rb_tree_base_iterator@_STL@@QAEXXZ'
 REBALANCE='?_Rebalance@?$_Rb_global@_N@_STL@@SAXPAU_Rb_tree_node_base@2@AAPAU32@@Z'
 LEFT='?_Rotate_left@?$_Rb_global@_N@_STL@@SAXPAU_Rb_tree_node_base@2@AAPAU32@@Z'
 RIGHT='?_Rotate_right@?$_Rb_global@_N@_STL@@SAXPAU_Rb_tree_node_base@2@AAPAU32@@Z'
 sources=[R/'rebuild/src/compiled'/p for p in ('00/9d/STLAsyncPointerTree_InsertUnique_009d7cf0.cpp','00/9d/STLAsyncBankTree_Minsert_009d7990.cpp','00/42/STL_Rb_global_Rebalance_0042971d.cpp','00/42/CMemoryAllocatorVariableSize_RotateLeft_0042951b.cpp','00/42/CMemoryAllocatorVariableSize_RotateLeft_0042955b.cpp')]
 targets={INSERT:0x9d7990,DEC:0x6000000,REBALANCE:0x42971d,LEFT:0x42951b,RIGHT:0x42955b,'_malloc':0xbfea0e}
 specs={ROOT:(0x9d7cf0,180),INSERT:(0x9d7990,179),REBALANCE:(0x42971d,190),LEFT:(0x42951b,64),RIGHT:(0x42955b,64)}
 inputs={Path(__file__),*sources};seen=set()
 def headers(p):
  if p in seen:return
  seen.add(p);inputs.add(p)
  for n in re.findall(r'^\s*#\s*include\s*"([^"]+)"',p.read_text(),re.M):
   q=next((q for q in (p.parent/n,R/'rebuild/include'/n) if q.is_file()),None);assert q,(p,n);headers(q)
 for p in sources:headers(p)
 for n in ('check_one.py','gate_game_component_abi.py','pe_oracle.py','near_match.py','verify_and_land.py','variable_allocator_parity.py','crawl/rowtrim.py','crawl/purity.py'):inputs.add(R/'tools/decomp_pipeline'/n)
 inputs.add(R/'tools/artifact_layout.py')
 def hashes():return {str(p.relative_to(R)):hashlib.sha256(p.read_bytes()).hexdigest() for p in sorted(inputs)}
 toolchain=[V.VC/'bin'/n for n in ('cl.exe','c1xx.dll','c2.dll','link.exe','mspdb71.dll')]
 def tools():return {str(p):hashlib.sha256(p.read_bytes()).hexdigest() for p in toolchain}
 before=hashes();beforetools=tools();raw=EXE.read_bytes();exe_sha=hashlib.sha256(raw).hexdigest();build=Path(tempfile.mkdtemp(prefix='native-balanced-tree-inventory-'));flags=['/nologo','/c','/O2','/MT'];objects=[];inventory=[];definitions={};audit=[];md=Cs(CS_ARCH_X86,CS_MODE_32)
 for index,source in enumerate(sources):
  obj=build/(str(index)+'.obj');result=V.cl(flags+['/I'+str(R/'rebuild/include'),'/Fo'+str(obj),str(source)],V.env(),build);assert result.returncode==0,result.stdout+result.stderr
  c=Coff(obj);objects.append((obj,c));rows={}
  for symbol,(section,offset) in c.symbols.items():
   if not symbol.startswith('?'):continue
   b,relocs=c.sections[section];assert offset==0
   ins=list(md.disasm(b,0));assert sum(i.size for i in ins)==len(b)
   fixups=[]
   for site,(name,kind) in relocs.items():
    addend=struct.unpack_from('<I',b,site)[0];assert addend==0 and kind==20,(source,symbol,site,name,kind,addend)
    fixups.append(dict(offset=site,symbol=name,kind=kind,addend=addend,target=hex(targets[name]) if name in targets else 'UNMAPPED_EMITTED_HELPER'))
   rows[symbol]=dict(bytes=len(b),instructions=len(ins),sha256=hashlib.sha256(b).hexdigest(),relocations=fixups,calls=[dict(offset=hex(i.address),target=i.op_str) for i in ins if i.mnemonic=='call'])
   definitions.setdefault(symbol,[]).append(index)
   if symbol in specs:
    address,size=specs[symbol];original=read_virtual_data(raw,address,size);original_ins=list(md.disasm(original,address));assert sum(i.size for i in original_ins)==size and original_ins[-1].mnemonic=='ret'
    grade=compare(b,original,tuple(relocs),retail_relocations('%08x'%address,size)).to_dict();patched=bytearray(b)
    for site,(name,kind) in relocs.items():assert name in targets;struct.pack_into('<I',patched,site,(targets[name]-address-site-4)&0xffffffff)
    audit.append(dict(source_index=index,symbol=symbol,address=hex(address),original_bytes=size,compiled_bytes=len(b),original_instructions=len(original_ins),compiled_instructions=len(ins),grade=grade,relocated_exact=bytes(patched)==original,original_sha256=hashlib.sha256(original).hexdigest(),relocations=fixups,full_disassembly=[dict(address=hex(i.address),mnemonic=i.mnemonic,operands=i.op_str) for i in md.disasm(bytes(patched),address)]))
  inventory.append(dict(source=str(source.relative_to(R)),object=str(obj),object_sha256=hashlib.sha256(obj.read_bytes()).hexdigest(),compile_stdout=result.stdout,compile_stderr=result.stderr,sections=rows))
 duplicates={}
 for symbol,indices in definitions.items():
  if len(indices)<2:continue
  data=[]
  for index in indices:
   c=objects[index][1];s,o=c.symbols[symbol];b,r=c.sections[s];data.append((b,r))
  duplicates[symbol]=dict(source_indices=indices,identical_bytes_and_relocations=all(x==data[0] for x in data[1:]))
 assert hashes()==before and tools()==beforetools and hashlib.sha256(EXE.read_bytes()).hexdigest()==exe_sha
 report=dict(status='FRESH_CANONICAL_BALANCED_CHAIN_INVENTORY_PARITY_ONLY',compile_flags=flags,pragma_policy='Preserve accepted source pragmas; /Os remains active in generic rebalance/rotation TUs',sources=inventory,executed_body_candidates=audit,duplicate_definitions=duplicates,required_function_definitions={s:definitions.get(s,[]) for s in (ROOT,INSERT,DEC,REBALANCE,LEFT,RIGHT)},input_sha256_before=before,input_sha256_after=hashes(),toolchain_sha256_before=beforetools,toolchain_sha256_after=tools(),original_exe_sha256=exe_sha,boundary='Fresh accepted canonical sources only. Inventory/parity audit, not execution/source omission/behavior or new recovery claim. Source-vs-symbol necessity inferred only from definitions; genuine omission link checks still pending. Actual emitted decrement identity/synthetic location separate from original79B clone. Dedicated rotation sources independently audited; duplicate definitions may make their aggregate omission nonmandatory.')
 return locals()

"""Fresh canonical balanced-registration chain and independent RB model."""
import hashlib,importlib.util,itertools,json,re,runpy,struct,subprocess,sys,tempfile
import pefile
from pathlib import Path
from unicorn import Uc,UcError,UC_ARCH_X86,UC_MODE_32,UC_HOOK_CODE,UC_HOOK_MEM_INVALID,UC_MEM_WRITE_UNMAPPED,UC_ERR_WRITE_UNMAPPED
from unicorn.x86_const import *
from capstone import Cs,CS_ARCH_X86,CS_MODE_32
R=Path(__file__).resolve().parents[3];w=R/'rebuild/tests/integration';sys.path.insert(0,str(R/'tools/decomp_pipeline'))
import check_one as V
from pe_oracle import EXE,read_virtual_data
model_path=w/'NativeAsyncBalancedRegistration_model.py';required=w/'NativeAsyncBalancedRegistration_required.cpp'
source_paths=[R/'rebuild/src/compiled'/p for p in ('00/9d/STLAsyncPointerTree_InsertUnique_009d7cf0.cpp','00/9d/STLAsyncBankTree_Minsert_009d7990.cpp','00/42/STL_Rb_global_Rebalance_0042971d.cpp','00/42/CMemoryAllocatorVariableSize_RotateLeft_0042951b.cpp','00/42/CMemoryAllocatorVariableSize_RotateLeft_0042955b.cpp')]
inputs={Path(__file__),model_path,required,R/'tools/decomp_pipeline/gate_native_async_balanced_registration.py',*source_paths};seen=set()
def headers(p):
 if p in seen:return
 seen.add(p);inputs.add(p)
 for name in re.findall(r'^\s*#\s*include\s*"([^"]+)"',p.read_text(),re.M):
  q=next((q for q in (p.parent/name,R/'rebuild/include'/name) if q.is_file()),None);assert q,(p,name);headers(q)
for p in [*source_paths,required]:headers(p)
for name in ('check_one.py','gate_game_component_abi.py','pe_oracle.py','near_match.py','verify_and_land.py','variable_allocator_parity.py','crawl/rowtrim.py','crawl/purity.py'):inputs.add(R/'tools/decomp_pipeline'/name)
inputs.add(R/'tools/artifact_layout.py')
def hashes():return {str(p.relative_to(R)):hashlib.sha256(p.read_bytes()).hexdigest() for p in sorted(inputs)}
toolchain=[V.VC/'bin'/n for n in ('cl.exe','c1xx.dll','c2.dll','link.exe','mspdb71.dll')]
def tools():return {str(p):hashlib.sha256(p.read_bytes()).hexdigest() for p in toolchain}
before=hashes();beforetools=tools();raw=EXE.read_bytes();exe_sha=hashlib.sha256(raw).hexdigest()
pe=pefile.PE(data=raw)
malloc_import=[(entry.dll.decode(),item.name.decode()) for entry in pe.DIRECTORY_ENTRY_IMPORT for item in entry.imports if item.address==0x1440158]
assert len(malloc_import)==1 and malloc_import[0][0].lower()=='msvcr71.dll' and malloc_import[0][1]=='malloc',malloc_import
assert read_virtual_data(raw,0xbfea0e,6)==b'\xff\x25'+struct.pack('<I',0x1440158)
# This executes a FRESH current-source inventory, not old objects/hash receipts.
# Dedicated rotation objects are parity prerequisites only; actual chain uses TU2.
fresh=fresh_inventory();objects=fresh['objects'];audit=fresh['audit'];symbols=fresh['specs'];targets=fresh['targets'];DEC=fresh['DEC'];ROOT=fresh['ROOT'];INSERT=fresh['INSERT'];REBALANCE=fresh['REBALANCE'];LEFT=fresh['LEFT'];RIGHT=fresh['RIGHT']
assert fresh['definitions'][LEFT]==[2,3] and fresh['definitions'][RIGHT]==[2,4]
assert fresh['duplicates'][LEFT]['identical_bytes_and_relocations'] and fresh['duplicates'][RIGHT]['identical_bytes_and_relocations']
for row in audit:
 assert row['grade']['complete_decode'] and row['grade']['opcode_similarity']==1 and row['grade']['instruction_delta']==0
 if row['symbol']!=ROOT:assert row['relocated_exact']
spec=importlib.util.spec_from_file_location('independent_balanced_tree',model_path);model=importlib.util.module_from_spec(spec);spec.loader.exec_module(model)
build=Path(tempfile.mkdtemp(prefix='balanced-chain-omission-'));ro=build/'required.obj'
result=V.cl(['/nologo','/c','/O2','/MT','/I'+str(R/'rebuild/include'),'/Fo'+str(ro),str(required)],V.env(),build);assert result.returncode==0,result.stdout+result.stderr
cmd=[str(V.VC/'bin/link.exe'),'/nologo','/out:'+str(build/'required.exe'),'kernel32.lib',str(ro)];omissions=[]
for omitted,symbol in enumerate((ROOT,INSERT,REBALANCE)):
 missing=subprocess.run(cmd+[str(objects[i][0]) for i in range(3) if i!=omitted],env=V.env(),capture_output=True,text=True);diagnostic=missing.stdout+missing.stderr
 assert missing.returncode and any(symbol in line and ('LNK2001' in line or 'LNK2019' in line) for line in diagnostic.splitlines()),(omitted,diagnostic)
 omissions.append(dict(source=str(source_paths[omitted].relative_to(R)),symbol=symbol,missing_failed=True,missing_returncode=missing.returncode,missing_stdout=missing.stdout,missing_stderr=missing.stderr))
control=subprocess.run(cmd+[str(objects[i][0]) for i in range(3)],env=V.env(),capture_output=True,text=True);assert control.returncode==0,control.stdout+control.stderr
A=0x2000000;N=32768;S=0x3000000;STOP=0x4000000;ENTRY=0x9d7cf0;MINSERT=0x9d7990;MALLOC=0xbfea0e;REBAL=0x42971d;LROT=0x42951b;RROT=0x42955b;DORIG=0x9d74e0;DSYN=0x6000000
md=Cs(CS_ARCH_X86,CS_MODE_32)
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
compiled={};provider_sources={}
chain_targets=dict(targets);chain_targets[DEC]=DSYN
for symbol,index in ((ROOT,0),(INSERT,1),(REBALANCE,2),(LEFT,2),(RIGHT,2)):
 address,size=symbols[symbol];c=objects[index][1];section,offset=c.symbols[symbol];b,rels=c.sections[section];assert offset==0;patched=bytearray(b)
 for site,(target,kind) in rels.items():assert kind==20 and struct.unpack_from('<I',b,site)[0]==0;struct.pack_into('<I',patched,site,(chain_targets[target]-address-site-4)&0xffffffff)
 compiled[address]=bytes(patched);provider_sources[hex(address)]=str(source_paths[index].relative_to(R))
c=objects[0][1];ds,do=c.symbols[DEC];decrement,dr=c.sections[ds];assert do==0 and len(decrement)==78 and not dr;compiled[DSYN]=decrement
original={address:read_virtual_data(raw,address,size) for address,size in symbols.values()};original[DORIG]=read_virtual_data(raw,DORIG,79)
u=Uc(UC_ARCH_X86,UC_MODE_32)
for page in {p&~4095 for p in (*original,*compiled,MALLOC)}|{(MALLOC&~4095)+4096}:u.mem_map(page,4096)
u.mem_map(A,N);u.mem_map(S,65536);u.mem_map(STOP,4096);active=[None];invalid=[None]
u.hook_add(UC_HOOK_CODE,lambda uc,pc,n,_:active[0](uc,pc));u.hook_add(UC_HOOK_MEM_INVALID,lambda uc,access,address,n,value,_:invalid[0](uc,access,address,n));cpu=u.context_save();regs=(UC_X86_REG_EBX,UC_X86_REG_EBP,UC_X86_REG_ESI,UC_X86_REG_EDI)
coverage={kind:dict(registration=0,insert=0,rebalance=0,left=0,right=0,decrement=0,left_uncle_red=0,right_uncle_red=0,root_replacements=0,duplicates=0,malloc_calls=0) for kind in ('original','compiled')}
faults={kind:{hex(p):0 for p in (0x9d79cb,0x9d79f1)} for kind in coverage}
def malloc(m,c,events,snaps):
 events.append(('malloc',(20,)));snaps.append(m.snap());m.put(c['marker'],0xcafebabe)
 return 0 if c['fail'] else c['newnode']
def run(initial,c,kind):
 u.context_restore(cpu)
 bodies=original if kind=='original' else compiled
 for address,b in bodies.items():
  u.mem_write(address,b);u.ctl_remove_cache(address,address+len(b))
 u.mem_write(A,initial);u.mem_write(S,bytes(65536));m=Memory(u=u);sp=S+0x8000;u.mem_write(sp,word(STOP)+word(c['output'])+word(c['value']));u.reg_write(UC_X86_REG_ESP,sp);u.reg_write(UC_X86_REG_ECX,c['tree'])
 for i,r in enumerate(regs):u.reg_write(r,0x11223300+i)
 events=[];snaps=[m.snap()];fault=[None];checkpoints={};decentry=DORIG if kind=='original' else DSYN
 retpcs={entry+i.address:entry for entry in (MINSERT,REBAL,LROT,RROT,decentry) for i in md.disasm(bodies[entry],0) if i.mnemonic=='ret'}
 counters=coverage[kind];rolemap={ENTRY:'registration',MINSERT:'insert',REBAL:'rebalance',LROT:'left',RROT:'right',decentry:'decrement'}
 def hook(uc,pc):
  if pc==STOP:uc.emu_stop();return
  stack=uc.reg_read(UC_X86_REG_ESP)
  if pc in rolemap:
   counters[rolemap[pc]]+=1
   if pc!=ENTRY:checkpoints[pc]=(stack,[uc.reg_read(r) for r in regs],uc.reg_read(UC_X86_REG_ECX))
   if pc==MINSERT:
    assert uc.reg_read(UC_X86_REG_ECX)==c['tree']
    out,x,y,value,warg=struct.unpack('<IIIII',uc.mem_read(stack+4,20));assert S<=out and out+4<=S+65536 and value==c['value'] and warg==0
    assert y==c['parent'] and x==(y if c['minimum'] else 0);checkpoints[pc]+= (out,)
   elif pc in (REBAL,LROT,RROT):
    node,rootref=struct.unpack('<II',uc.mem_read(stack+4,8));assert rootref==c['header']+4
    if pc==REBAL:assert node==c['newnode']
   elif pc==decentry:
    iterator=uc.reg_read(UC_X86_REG_ECX);assert S<=iterator and iterator+4<=S+65536
    node=struct.unpack('<I',uc.mem_read(iterator,4))[0]
    if node==c['header']:pred=c['maximum']
    else:
     key=m.w(node+16);smaller=[k for k in c['nodekeys'] if k<key];assert smaller;pred=c['nodekeys'][max(smaller)]
    checkpoints[pc]+=(iterator,pred)
  if pc in retpcs:
   entry=retpcs[pc];state=checkpoints[entry];assert stack==state[0] and [uc.reg_read(r) for r in regs]==state[1]
   if entry==MINSERT:assert uc.reg_read(UC_X86_REG_EAX)==state[3] and struct.unpack('<I',uc.mem_read(state[3],4))[0]==c['newnode']
   if entry==decentry:assert struct.unpack('<I',uc.mem_read(state[3],4))[0]==state[4]
  if pc in (0x42974f,0x429783):
   uncle=uc.reg_read(UC_X86_REG_ECX)
   if uncle and m.read(uncle,1)==b'\0':counters['left_uncle_red' if pc==0x42974f else 'right_uncle_red']+=1
  if pc!=MALLOC:return
  args=struct.unpack('<I',uc.mem_read(stack+4,4));assert args==(20,);counters['malloc_calls']+=1;result=malloc(m,c,events,snaps);ret=struct.unpack('<I',uc.mem_read(stack,4))[0]
  uc.reg_write(UC_X86_REG_EAX,result);uc.reg_write(UC_X86_REG_ECX,0xabcdef01);uc.reg_write(UC_X86_REG_EDX,0x98765432);uc.reg_write(UC_X86_REG_ESP,stack+4);uc.reg_write(UC_X86_REG_EIP,ret)
 def on_invalid(uc,access,address,n):
  pc=uc.reg_read(UC_X86_REG_EIP);assert c['fail'] and access==UC_MEM_WRITE_UNMAPPED and address==16 and n==4 and pc in (0x9d79cb,0x9d79f1) and uc.reg_read(UC_X86_REG_ECX)==16
  ins=next(md.disasm(bytes(uc.mem_read(pc,4)),pc));assert ins.mnemonic=='mov' and ins.op_str=='dword ptr [ecx], edx';fault[0]=pc;faults[kind][hex(pc)]+=1;return False
 active[0]=hook;invalid[0]=on_invalid
 try:u.emu_start(ENTRY,STOP,count=6000)
 except UcError as error:assert error.errno==UC_ERR_WRITE_UNMAPPED and fault[0] is not None
 result=None
 if fault[0] is None:
  assert u.reg_read(UC_X86_REG_EIP)==STOP and u.reg_read(UC_X86_REG_ESP)==sp+12 and [u.reg_read(r) for r in regs]==[0x11223300+i for i in range(4)]
  result=u.reg_read(UC_X86_REG_EAX);assert result==c['output']
  model.validate_arena(m,c['tree'],c['header'],c['afterkeys'])
  if c['duplicate']:counters['duplicates']+=1;assert not events
  if c['oldroot'] and m.w(c['header']+4)!=c['oldroot']:counters['root_replacements']+=1
 else:assert len(events)==1 and coverage[kind]['rebalance']==c['rebalance_before'][kind]
 snaps.append(m.snap());return events,snaps,result,fault[0]
cases=points=sequence_domains=0
def operation(prefix,key,place,fill,fail):
 global cases,points
 base=A+place*0x3000;initial=bytearray([fill]*N);m=Memory(initial);tree=base+0x100;header=base+0x200;value=base+0x300;output=base+0x400;marker=base+0x500;abstract=model.Tree()
 for previous in prefix:abstract.insert(previous,base+0x1000+len(abstract.nodes)*32)
 abstract.serialize(m,tree,header);model.validate_arena(m,tree,header,abstract.nodes)
 m.put(value,key);m.write(output,word(0xdeadface)+b'\xa7\xc1\xd2\xe3')
 duplicate=abstract.find(key);parent=abstract.parent_for(key);newnode=base+0x1000+len(abstract.nodes)*32
 c=dict(tree=tree,header=header,value=value,output=output,marker=marker,newnode=newnode,fail=fail,parent=parent.address if parent else header,minimum=not abstract.nodes or key<min(abstract.nodes),maximum=abstract.nodes[max(abstract.nodes)].address if abstract.nodes else header,nodekeys={k:n.address for k,n in abstract.nodes.items()},oldroot=abstract.root.address if abstract.root else 0,duplicate=bool(duplicate),rebalance_before={kind:coverage[kind]['rebalance'] for kind in coverage})
 initialbytes=bytes(initial);expectedmem=Memory(bytearray(initial));events=[];snaps=[expectedmem.snap()]
 if duplicate:
  node=duplicate.address;expectedmem.put(output,node);expectedmem.write(output+4,b'\0');result=output;fault=False;c['afterkeys']=set(abstract.nodes)
 else:
  malloc(expectedmem,c,events,snaps)
  if fail:result=None;fault=True;c['afterkeys']=set(abstract.nodes)
  else:
   node,inserted=abstract.insert(key,newnode);assert inserted;abstract.serialize(expectedmem,tree,header);expectedmem.put(output,node.address);expectedmem.write(output+4,b'\1');result=output;fault=False;c['afterkeys']=set(abstract.nodes)
 snaps.append(expectedmem.snap())
 for kind in coverage:
  actual=run(initialbytes,c,kind)
  assert actual[:3]==(events,snaps,result) and bool(actual[3])==fault,(kind,c,actual[0],events,actual[3],fault)
  if not fault:assert actual[3] is None
 cases+=1;points+=len(snaps)
def main():
 global sequence_domains
 sequences=list(itertools.permutations(range(4)))+[(2,1,0),(0,1,2),(2,0,1),(0,2,1),tuple(range(8)),tuple(reversed(range(8))),(4,2,6,1,3,5,7,0),(3,1,5,0,2,4,6,7),(0,7,1,6,2,5,3,4),(7,0,6,1,5,2,4,3)]
 mappings=(tuple(range(8)),(0,1,2,0x7ffffffe,0x7fffffff,0x80000000,0xfffffffe,0xffffffff))
 for sequence,mapping,place,fill,fail in itertools.product(sequences,mappings,range(2),(0,0x95),range(2)):
  keys=[mapping[i] for i in sequence];keys.extend((keys[0],keys[-1],keys[len(keys)//2]));prefix=[];sequence_domains+=1
  for key in keys:operation(prefix,key,place,fill,fail);prefix.append(key)
 for kind,row in coverage.items():
  assert all(row[name]>0 for name in ('left','right','decrement','left_uncle_red','right_uncle_red','root_replacements','duplicates')),(kind,row)
  assert all(count>0 for count in faults[kind].values())
 assert hashes()==before and tools()==beforetools and hashlib.sha256(EXE.read_bytes()).hexdigest()==exe_sha
 report=dict(status='FRESH_BALANCED_REGISTRATION_COMPOSITION_PASS',unique_operations=cases,sequence_domains=sequence_domains,snapshots_per_root_model=points,coverage=coverage,nullmalloc_faults=faults,source_omissions=omissions,link_control_passed=True,mandatory_translation_units=[str(p.relative_to(R)) for p in source_paths[:3]],selected_provider_sources=provider_sources,dedicated_rotations='Audited identical parity prerequisites only; NOT mandatory omission providers',fresh_object_sha256={str(p):hashlib.sha256(p.read_bytes()).hexdigest() for p,c in objects},provider_audit=audit,input_sha256_before=before,input_sha256_after=hashes(),toolchain_sha256_before=beforetools,toolchain_sha256_after=tools(),original_exe_sha256=exe_sha,boundary='Fresh canonical three mandatory TUs supply actual registration/minsert/rebalance/both rotations plus emitted78B decrement synthetic6000000; original chain uses actual79B decrement. Only malloc20 cdecl callback, unrelated marker mutation only. Full32KiB rootentry/malloc/final snapshots and independentRB invariants after successful calls, exactnullWRITE16faults; successful rootpair/childiterator/voidleaf/decrement ABIs checked. Model-generated validinitialtrees; immutablekey/header duringmalloc. No realheap/free/concurrency/lifetime/filequeue or helperrecoverycount/canonicalpromotion.')
 report.update(translation_cache_policy='Explicit ctl_remove_cache after every executable body write',original_malloc_import=dict(dll=malloc_import[0][0],symbol=malloc_import[0][1],iat='0x1440158',thunk='0xbfea0e'),control_returncode=control.returncode,control_stdout=control.stdout,control_stderr=control.stderr,link_executable_executed=False)
 (R/'rebuild/compile-gate/native-async-balanced-registration.json').write_text(json.dumps(report,indent=2)+'\n');print(report['status'],cases,points,faults,coverage,flush=True)
if __name__=='__main__':main()
