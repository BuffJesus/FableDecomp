"""Native bank discard lock/order/ownership and actual static wrapper."""
import hashlib,itertools,json,struct,subprocess,tempfile
from pathlib import Path
from capstone import Cs,CS_ARCH_X86,CS_MODE_32
import check_one as V
from gate_game_component_abi import Coff
from variable_allocator_parity import check
from pe_oracle import EXE,read_virtual_data
ROOT=Path(__file__).resolve().parents[2];w=Path(tempfile.mkdtemp(prefix='native-bank-discard-'));env=V.env()
sources=[ROOT/'rebuild/src/compiled/00/9d'/p for p in ('CBankFileAsync_DiscardAsyncData_009d67e0.cpp','CBankFileAsyncData_DiscardAsyncData_009d68a0.cpp')];fixture=ROOT/'rebuild/tests/integration/NativeBankDiscard_test.cpp'
push_source=ROOT/'rebuild/src/compiled/00/9d/STLAsyncDisposalList_push_back_009d7b00.cpp'
process_source=ROOT/'rebuild/src/compiled/00/9d/CBankFileAsync_ProcessDisposalList_009d6730.cpp'
lock_sources=[ROOT/'rebuild/src/compiled/00/a6'/p for p in ('CCriticalSectionLock_Constructor_00a62920.cpp','CCriticalSectionLock_Destructor_00a629a0.cpp')]
bank='?DiscardAsyncData@CBankFileAsync@@QAEXAAV?$CCountedPointer@VCBankFileAsyncData@@@@@Z';static='?DiscardAsyncData@CBankFileAsyncData@@SIXAAV?$CCountedPointer@VCBankFileAsyncData@@@@@Z';lock='??0CCriticalSectionLock@@QAE@AAU_RTL_CRITICAL_SECTION@@_N@Z';unlock='??1CCriticalSectionLock@@QAE@XZ';abort='?Abort@CBankFileAsyncData@@QAE_NXZ';process='?ProcessDisposalList@CBankFileAsync@@QAEXXZ'
objects=[]
for i,p in enumerate(sources):
 obj=w/(str(i)+'.obj');r=V.cl(['/nologo','/c','/O2','/MT','/Fo'+str(obj),str(p)],env,w);assert not r.returncode,r.stdout+r.stderr;objects.append(obj)
c=Coff(objects[0]);sec,_=c.symbols[bank];push=next(t for t,k in c.sections[sec][1].values() if t.startswith('?push_back'))
targets={lock:0xa62920,unlock:0xa629a0,abort:0x9d6350,process:0x9d6730,push:0x9d7b00,'??3@YAXPAX@Z':0xbfe9bc,bank:0x9d67e0,'_malloc':0xbfea0e}
targets.update({'_free':0xbfea14,'__imp__EnterCriticalSection@4':0x143fe0c,'__imp__TryEnterCriticalSection@4':0x143fd30,'__imp__LeaveCriticalSection@4':0x143fe10})
aliases={'@Entry@12':bank,'@StaticEntry@4':static,abort:'@Abort@4','__imp__BankEnter@4':'_EnterIAT','__imp__BankTry@4':'_TryIAT','__imp__BankLeave@4':'_LeaveIAT'}
(w/'bank_discard_aliases.h').write_text('\n'.join('#pragma comment(linker,"/alternatename:'+a+'='+b+'")' for a,b in aliases.items())+'\n')
raw=EXE.read_bytes();code=bytearray(896);fixups=[];md=Cs(CS_ARCH_X86,CS_MODE_32);md.detail=True
for address,n,offset in ((0x9d67e0,180,0),(0x9d68a0,16,256),(0x9d7b00,59,320),(0x9d6730,164,512),(0xa62920,54,768),(0xa629a0,25,832)):
 body=read_virtual_data(raw,address,n);code[offset:offset+n]=body
 for i in md.disasm(body,address):
  if i.mnemonic=='call' and i.operands[0].type==2:assert i.operands[0].imm in targets.values();fixups.append((offset+i.address-address+1,i.operands[0].imm,1))
  for op in i.operands:
   if op.type==3 and op.mem.base==0 and op.mem.index==0 and op.mem.disp>=0x400000:assert op.mem.disp in targets.values();fixups.append((offset+i.address-address+i.disp_offset,op.mem.disp,0))
assert len(fixups)==19
(w/'bank_discard_retail.h').write_text('static const unsigned char retailCode[]={'+','.join(str(b) for b in code)+'};\nstatic const unsigned retailFixups[][3]={'+','.join('{%d,%d,%d}'%f for f in fixups)+'};\n')
obj=w/'fixture.obj';r=V.cl(['/nologo','/c','/O2','/MT','/I'+str(w),'/Fo'+str(obj),str(fixture)],env,w);assert not r.returncode,r.stdout+r.stderr;objects.append(obj)
proofs=[check(objects[0],bank,0x9d67e0,180,9,targets),check(objects[1],static,0x9d68a0,16,1,targets)]
for i,extra in enumerate(([],['/Dmalloc=PushMalloc'])):
 obj=w/('push%d.obj'%i);r=V.cl(['/nologo','/c','/O2','/MT','/Fo'+str(obj)]+extra+[str(push_source)],env,w);assert not r.returncode,r.stdout+r.stderr;objects.append(obj)
proofs.append(check(objects[3],push,0x9d7b00,59,1,targets))
original,renamed=Coff(objects[3]),Coff(objects[4]);a,off=original.symbols[push];b,otheroff=renamed.symbols[push];left,lr=original.sections[a];right,rr=renamed.sections[b]
assert off==otheroff and left==right and {p:('_PushMalloc' if t=='_malloc' else t,k) for p,(t,k) in lr.items()}==rr
rename={'_free':'_DisposalFree','__imp__EnterCriticalSection@4':'__imp__BankEnter@4','__imp__TryEnterCriticalSection@4':'__imp__BankTry@4','__imp__LeaveCriticalSection@4':'__imp__BankLeave@4'}
for p,extra in [(process_source,['/Dfree=DisposalFree'])]+[(p,['/DEnterCriticalSection=BankEnter','/DTryEnterCriticalSection=BankTry','/DLeaveCriticalSection=BankLeave']) for p in lock_sources]:
 for options in ([],extra):
  obj=w/('dep%d.obj'%len(objects));r=V.cl(['/nologo','/c','/O2','/MT','/Fo'+str(obj)]+options+[str(p)],env,w);assert not r.returncode,r.stdout+r.stderr;objects.append(obj)
for index,symbol,address,size,count in [(5,process,0x9d6730,164,5),(7,lock,0xa62920,54,2),(9,unlock,0xa629a0,25,1)]:
 proofs.append(check(objects[index],symbol,address,size,count,targets))
 original,renamed=Coff(objects[index]),Coff(objects[index+1]);a,off=original.symbols[symbol];b,otheroff=renamed.symbols[symbol];left,lr=original.sections[a];right,rr=renamed.sections[b]
 assert off==otheroff and left==right and {p:(rename.get(t,t),k) for p,(t,k) in lr.items()}==rr
exe=w/'fixture.exe';cmd=[str(V.VC/'bin/link.exe'),'/nologo','/out:'+str(exe),'kernel32.lib']
linked=(0,1,2,4,6,8,10)
for omitted in (0,1,4,6,8,10):
 r=subprocess.run(cmd+[str(objects[i]) for i in linked if i!=omitted],env=env,capture_output=True,text=True);assert r.returncode
r=subprocess.run(cmd+[str(objects[i]) for i in linked],env=env,capture_output=True,text=True);assert not r.returncode,r.stdout+r.stderr
inputs=bytearray();cases=0;base=0x24000000
for fill,mode,owner,present,finished,abort_result,refs,kind,pending in itertools.product((0,0x55,0xff),range(4),(0,0x400),range(2),range(2),range(2),(None,0,1,2,0xffffffff),range(2),range(3)):
 arena=bytearray([fill])*8192
 def put(at,value):struct.pack_into('<I',arena,at,value)
 put(0x1800,base+0x1000 if present else 0);put(0x1804,base+0x1900 if refs is not None else 0)
 put(0x1028,base+owner);put(0x1128,base+0x800);arena[0x1044]=finished;arena[0x1144]=1-finished
 put(0x1900,refs or 0);put(0x1908,base+0x1a00);put(0x1940,1);put(0x1948,base+0x1a40)
 for bank_offset,head in ((0,0x1b00),(0x400,0x1b40),(0x800,0x1b80)):
  put(bank_offset+0x144,base+head);put(head,base+head);put(head+4,base+head)
  if bank_offset==owner and pending:
   nodes=[0x1c40+64*i for i in range(pending)];chain=[head]+nodes
   for i,node in enumerate(chain):put(node,base+chain[(i+1)%len(chain)]);put(node+4,base+chain[(i-1)%len(chain)])
   for i,node in enumerate(nodes):
    put(node+8,base+0x1200+256*i);put(node+12,base+0x19a0+32*i);put(0x19a0+32*i,1);put(0x19a8+32*i,base+0x1a80+64*i)
 inputs.extend(struct.pack('<4I',mode,abort_result,owner,kind));inputs.extend(arena);cases+=1
(w/'input.bin').write_bytes(inputs)
for kind in ('retail','compiled'):
 r=subprocess.run([str(exe),str(w/'input.bin'),str(w/(kind+'.bin')),kind],env=env,capture_output=True,text=True,timeout=60);assert not r.returncode,(w,r.returncode,r.stdout,r.stderr);print(r.stdout.strip(),kind,flush=True)
actual=(w/'compiled.bin').read_bytes();expected=(w/'retail.bin').read_bytes();assert actual==expected,(w,'bank discard');assert len(actual)%(8192+16)==0
snapshots=len(actual)//(8192+16)
headers=[ROOT/'rebuild/include'/p for p in ('fable_game_text_bank.h','fable_counted_pointer.h','fable_critical_section_lock.h','fable_stlport_list.h')]
report=dict(cases=cases,snapshots=snapshots,omission_checks=6,providers=proofs,source_sha256={str(p.relative_to(ROOT)):hashlib.sha256(p.read_bytes()).hexdigest() for p in sources+[push_source,process_source]+lock_sources+headers},fixture_sha256=hashlib.sha256(fixture.read_bytes()).hexdigest(),boundary='Six original bodies natively executed with19 audited calls/import operands;actual owner/static wrapper/list insertion/disposal processing/lock construction/destruction compose. Allocator and Win32-import renames preserve complete bodies/relocations. Full8KiB at Win32 enter/leave,abort,malloc,DeleteFunc,operator delete,free and return. Null/finished/abort branches,two owners,zero/one/two pending nodes,Info null/count0/1/2/ffffffff,callbacks swap Data/Info and change Finished between locks. Critical-section addresses/depth recorded;stack lock addresses/padding not test output. Only Win32 critical-section operations,Abort and allocation callbacks remain boundaries;no arena normalization.')
(ROOT/'rebuild/compile-gate/native-bank-discard.json').write_text(json.dumps(report,indent=2)+'\n');print('NATIVE_BANK_DISCARD_GATE_PASS',snapshots,'snapshots')
