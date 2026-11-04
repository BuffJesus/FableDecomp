"""Fresh canonical source/static ABI gate; no scratch dependencies."""
import hashlib,json,re,struct,subprocess,sys,tempfile
from pathlib import Path
R=Path(__file__).resolve().parents[3];S=Path(tempfile.mkdtemp(prefix='native_dirty_disc_'))
sys.path.insert(0,str(R/'tools/decomp_pipeline'))
import check_one as V
from gate_game_component_abi import Coff
from pe_oracle import EXE,read_virtual_data
from capstone import Cs,CS_ARCH_X86,CS_MODE_32
from unicorn import Uc,UC_ARCH_X86,UC_MODE_32
from unicorn.x86_const import UC_X86_REG_EAX,UC_X86_REG_EBX,UC_X86_REG_ECX,UC_X86_REG_EDX,UC_X86_REG_ESI,UC_X86_REG_EDI,UC_X86_REG_EBP,UC_X86_REG_ESP,UC_X86_REG_EFLAGS,UC_X86_REG_EIP
src=R/'rebuild/src/compiled/00/98/CThreadedFileManager_ProcessDirtyDiscError_0098e1b0.cpp';required=R/'rebuild/tests/integration/NativeDirtyDisc_required.cpp';obj=S/'root.obj';caller=S/'caller.obj'
inputs={Path(__file__),src,required,R/'tools/decomp_pipeline/gate_native_dirty_disc.py',EXE};resolution={}
def closure(p):
 if p in inputs and p.suffix=='.h':return
 inputs.add(p)
 for n in re.findall(r'^\s*#\s*include\s*"([^"]+)"',p.read_text(),re.M):
  q=next((q for q in (p.parent/n,R/'rebuild/include'/n) if q.is_file()),None);assert q,(p,n)
  resolution[str(p)+':'+n]=str(q);closure(q)
closure(src);closure(required)
for n in ('check_one.py','gate_game_component_abi.py','pe_oracle.py','near_match.py','crawl/rowtrim.py','crawl/purity.py','variable_allocator_parity.py','verify_and_land.py'):inputs.add(R/'tools/decomp_pipeline'/n)
inputs.add(R/'tools/artifact_layout.py')
for p in (src,required):assert not re.search(r'\b(__asm|naked|_emit|__forceinline)\b|#\s*define\s+(private|protected)',p.read_text())
compiler={V.VC/'bin'/n for n in ('cl.exe','c1xx.dll','c2.dll','link.exe','mspdb71.dll')}
def hashes(ps):return {str(p):hashlib.sha256(p.read_bytes()).hexdigest() for p in sorted(ps)}
before=hashes(inputs);vc=hashes(compiler);raw=EXE.read_bytes();logs=[]
for p,o in ((src,obj),(required,caller)):
 result=V.cl(['/nologo','/c','/O2','/MT','/I'+str(R/'rebuild/include'),'/Fo'+str(o),str(p)],V.env(),S);logs.append(dict(source=str(p),returncode=result.returncode,stdout=result.stdout,stderr=result.stderr));assert result.returncode==0,logs[-1]
fresh_before=hashes({obj,caller});c=Coff(obj);cc=Coff(caller);root='?ProcessDirtyDiscError@CThreadedFileManager@@SIXXZ';assert root in c.symbols and root not in cc.symbols
sec,offset=c.symbols[root];body,rels=c.sections[sec];assert offset==0 and body==read_virtual_data(raw,0x98e1b0,1)==b'\xc3' and not rels
md=Cs(CS_ARCH_X86,CS_MODE_32);callerrows=[];references=[]
for section,(b,r) in cc.sections.items():
 for k,(name,kind) in r.items():
  if name==root:
   assert kind==20 and struct.unpack_from('<I',b,k)[0]==0;references.append((section,k))
   decoded=list(md.disasm(b,0));assert sum(i.size for i in decoded)==len(b)
   assert any(i.address==k-1 and i.mnemonic in ('call','jmp') for i in decoded)
   callerrows.append(dict(section=section,raw=b.hex(),instructions=[dict(offset=i.address,op=i.mnemonic,args=i.op_str,bytes=i.bytes.hex()) for i in decoded],relocations={str(k):v for k,v in r.items()}))
assert references
def inventory(coff):
 rows=[]
 for section,(b,r) in coff.sections.items():
  at=20+40*(section-1);header=coff.data[at:at+40];flags=struct.unpack_from('<I',header,36)[0];ins=list(md.disasm(b,0)) if flags&0x20 else []
  assert not flags&0x20 or sum(i.size for i in ins)==len(b)
  rows.append(dict(section=section,header_raw=header.hex(),flags=flags,raw_pointer=struct.unpack_from('<I',header,20)[0],raw_size=struct.unpack_from('<I',header,16)[0],raw=b.hex(),symbols={n:o for n,(sec,o) in coff.symbols.items() if sec==section},relocations=[dict(offset=k,symbol=n,kind=t,addend=struct.unpack_from('<I',b,k)[0]) for k,(n,t) in r.items()],code_disassembly=[dict(offset=i.address,bytes=i.bytes.hex(),op=i.mnemonic,args=i.op_str) for i in ins]))
 return rows
root_inventory=inventory(c);caller_inventory=inventory(cc)
assert len(root_inventory)==struct.unpack_from('<H',c.data,2)[0] and len(caller_inventory)==struct.unpack_from('<H',cc.data,2)[0]
rs,ro=cc.symbols['?RequireDirtyDisc@@YAXXZ'];rb,rr=cc.sections[rs];ri=list(md.disasm(rb[ro:],0));assert len(ri)==1 and ri[0].mnemonic=='jmp' and ri[0].size==5 and rr=={1:(root,20)},'Typed wrapper must only tailcall: no ECX setup or stack arguments'
def link(objects,name):return subprocess.run([str(V.VC/'bin/link.exe'),'/nologo','/out:'+str(S/name),*[str(p) for p in objects]],cwd=str(S),env=V.env(),capture_output=True,text=True)
missing=link([caller],'dirty_disc_missing.exe');diag=missing.stdout+missing.stderr;assert missing.returncode!=0 and any(root in line and re.search('LNK(?:2001|2019)',line) for line in diag.splitlines())
control=link([caller,obj],'dirty_disc_control.exe');assert control.returncode==0,control.stdout+control.stderr
linked_before=hashes({obj,caller,S/'dirty_disc_control.exe'})
regs=[UC_X86_REG_EAX,UC_X86_REG_EBX,UC_X86_REG_ECX,UC_X86_REG_EDX,UC_X86_REG_ESI,UC_X86_REG_EDI,UC_X86_REG_EBP];receipts=[]
for mode,b in [('original',read_virtual_data(raw,0x98e1b0,1)),('compiled',body)]:
 for case in range(32):
  u=Uc(UC_ARCH_X86,UC_MODE_32);u.mem_map(0x100000,0x1000);u.mem_map(0x200000,0x9000);u.mem_write(0x100000,b);arena=bytes((j*17+case)&255 for j in range(0x8000));u.mem_write(0x200000,arena);sp=0x207000;stop=0x100100;u.mem_write(sp,struct.pack('<I',stop));values=[((case+1)*0x9e3779b9+i*0x87654321)&0xffffffff for i in range(len(regs))]
  for r,v in zip(regs,values):u.reg_write(r,v)
  u.reg_write(UC_X86_REG_ESP,sp);u.reg_write(UC_X86_REG_EFLAGS,0x202|((case&1)*0x41));flags=u.reg_read(UC_X86_REG_EFLAGS);snapshot=bytes(u.mem_read(0x200000,0x8000));u.emu_start(0x100000,stop,count=1)
  assert u.reg_read(UC_X86_REG_EIP)==stop and [u.reg_read(r) for r in regs]==values and u.reg_read(UC_X86_REG_ESP)==sp+4 and u.reg_read(UC_X86_REG_EFLAGS)==flags and bytes(u.mem_read(0x200000,0x8000))==snapshot
 receipts.append(dict(mode=mode,cases=32,arena_bytes=32768,ret_stack_bytes=4,all_registers_and_flags_unchanged=True))
assert before==hashes(inputs) and vc==hashes(compiler) and hashlib.sha256(raw).hexdigest()==hashlib.sha256(EXE.read_bytes()).hexdigest()
assert fresh_before==hashes({obj,caller})
assert linked_before==hashes({obj,caller,S/'dirty_disc_control.exe'})
report=dict(root_coff_raw_hex=c.data.hex(),caller_coff_raw_hex=cc.data.hex(),root_contributions=root_inventory,caller_contributions=caller_inventory,fresh_objects_before=fresh_before,fresh_objects_after=hashes({obj,caller}),status='NATIVE_DIRTY_DISC_PASS',grade='MATCH',root_symbol=root,root_bytes=body.hex(),root_relocations=[],caller_sections=callerrows,abi=receipts,compile_logs=logs,missing_diagnostic=diag,control_diagnostic=control.stdout+control.stderr,input_before=before,input_after=hashes(inputs),compiler_before=vc,compiler_after=hashes(compiler),quoted_header_resolution=resolution,object_sha256=hashlib.sha256(obj.read_bytes()).hexdigest(),caller_object_sha256=hashlib.sha256(caller.read_bytes()).hexdigest(),control_exe_sha256=hashlib.sha256((S/'dirty_disc_control.exe').read_bytes()).hexdigest(),boundary='Only Unicorn executes original/compiled C3; link EXEs never executed. Void EAX unchanged, no invented return. Existing facade replacement, no new recovery credit or closure claim.')
out=R/'rebuild/compile-gate/native-dirty-disc.json';out.parent.mkdir(parents=True,exist_ok=True);out.write_text(json.dumps(report,indent=2)+'\n');print(report['status'])
