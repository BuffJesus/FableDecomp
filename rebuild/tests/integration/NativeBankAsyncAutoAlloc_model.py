"""Original/model auto allocation plus actual SetupRead; no root candidate."""
import hashlib,itertools,json,re,struct,sys
from pathlib import Path
from unicorn import Uc,UcError,UC_ARCH_X86,UC_MODE_32,UC_HOOK_CODE,UC_HOOK_INTR,UC_HOOK_MEM_INVALID,UC_MEM_READ_UNMAPPED,UC_ERR_READ_UNMAPPED
from unicorn.x86_const import *
from capstone import Cs,CS_ARCH_X86,CS_MODE_32
R=Path(__file__).resolve().parents[3];sys.path.insert(0,str(R/'tools/decomp_pipeline'))
from pe_oracle import EXE,read_virtual_data
ENTRY=0x9d6100;SETUP=0x9d5970;GETDATA=0x9d52d0;ALIGNMENT=0x9cbb80;ALIGN=0x9cbb90;ALLOC=0xbfea1a;CTOR=0x9d5df0;DELETE=0xbfe9bc;DELETER=0x9a42b0;POSITION=0x9cc270;ADVANCE=0x9cbf60;QUEUE=0x98f350
A=0x2000000;SIZE=32768;S=0x3000000;STOP=0x4000000
raw=EXE.read_bytes();root=read_virtual_data(raw,ENTRY,235);setup=read_virtual_data(raw,SETUP,191);nullprefix=read_virtual_data(raw,GETDATA,19)
assert [hashlib.sha256(b).hexdigest() for b in (root,setup,nullprefix)]==['e42590ffa1b0e1e1bc52d760c95f9c2e0e9373520fac1c8a072c47a9f72d9899','f7d1f779f87fa138fb783d45ae681fd9dba4fe879be9116f95bd419b4103843f','91f6abcd92200b06ae6f6cbab17b9e6f4d856f9353e7858c2f037a71241fc19c']
ALTDELETER=0x5000100
assert list(Cs(CS_ARCH_X86,CS_MODE_32).disasm(root,ENTRY))[-1].op_str=='0x10'
assert list(Cs(CS_ARCH_X86,CS_MODE_32).disasm(setup,SETUP))[-1].op_str=='0x18'
def word(v):return struct.pack('<I',v&0xffffffff)
class Memory:
 def __init__(self,buf=None,u=None):self.buf=buf;self.u=u
 def read(self,p,n):
  if self.buf is not None:assert 0<=n and A<=p<=A+SIZE and p+n<=A+SIZE;return bytes(self.buf[p-A:p-A+n])
  return bytes(self.u.mem_read(p,n))
 def write(self,p,b):
  if self.buf is not None:assert A<=p<=A+SIZE and p+len(b)<=A+SIZE;self.buf[p-A:p-A+len(b)]=b
  else:self.u.mem_write(p,b)
 def w(self,p):return struct.unpack('<I',self.read(p,4))[0]
 def put(self,p,v):self.write(p,word(v))
 def snap(self):return self.read(A,SIZE)
def callback(m,ctx,name,selfp,args,events,snaps):
 events.append((name,selfp,tuple(args)));snaps.append(m.snap());entry=ctx['entry'];bank2=ctx['banks'][1];mode=ctx['mutation']
 if name=='alignment':
  if mode==1:m.put(entry+4,bank2)
  return ctx['alignment']
 if name=='align':
  if mode==2:m.put(entry+4,bank2)
  # Bounded functional bank alignment: raw modular arithmetic, not recovered provider.
  return (args[0]+7)&0xfffffff8
 if name=='allocate':
  if args==(72,):
   if mode==3:m.put(entry+4,bank2)
   return 0 if ctx['allocation']==1 else ctx['data']
  assert args==(12,)
  if mode==4:m.put(entry+4,bank2)
  return 0 if ctx['allocation']==2 else ctx['control']
 if name=='construct':
  if ctx['allocation']==3:return 0
  size,offset,pool,bank=args
  returned=ctx.get('ctor_return',selfp);m.put(returned+12,ctx['buffer']);m.put(returned+40,bank);return returned
 if name=='position':
  if mode==5:m.put(entry+4,bank2)
  return 0xfffffff0
 if name=='queue':
  if ctx['control_mode']:m.put(ctx['control'],(0,2,0xffffffff)[ctx['control_mode']-1])
  if ctx.get('cleanup_mutation'):
   m.put(ctx['control'],0);m.put(ctx['control']+8,ctx['alternate_data']);m.put(ctx['control']+4,ALTDELETER)
  return 0xaabbccdd
 if name=='counted_delete':
  if ctx.get('cleanup_mutation'):
   assert m.w(ctx['output'])==ctx.get('ctor_return',ctx['data']) and m.w(ctx['output']+4)==ctx['control']
   if ctx['cleanup_mutation']==2:m.put(ctx['output'],ctx['alternate_data']);m.put(ctx['control']+8,ctx['data'])
  return 0
 if name=='delete':return 0
 raise AssertionError(name)
def model(initial,ctx):
 m=Memory(bytearray(initial));events=[];snaps=[];entry=ctx['entry'];offset=ctx['offset'];size=ctx['size'];priority=ctx['priority']
 def call(name,selfp,args=()):return callback(m,ctx,name,selfp,args,events,snaps)
 bank=m.w(entry+4)
 if m.read(bank+0x148,1)[0]:
  alignment=call('alignment',bank)
  if not alignment:snaps.append(m.snap());return events,snaps,'alignment_divide',None
  aligned=(offset//alignment)*alignment;remainder=offset-aligned
 else:aligned=offset;remainder=0
 wanted=(remainder+size)&0xffffffff;capacity=call('align',m.w(entry+4),(wanted,));data=call('allocate',0,(72,));control=0
 if data:
  bank=m.w(entry+4);data=call('construct',data,(size,remainder,m.w(bank+0x118),bank))
  if data:
   control=call('allocate',0,(12,))
   if control:m.put(control,1);m.put(control+4,DELETER);m.put(control+8,data)
 # Actual root calls SetupRead even if either allocation failed.
 bank=m.w(entry+4)
 if m.read(bank+0x149,1)[0]:
  assert not data,'sync nonnull path intentionally excluded'
  snaps.append(m.snap());return events,snaps,'sync_null_data',None
 if not data:snaps.append(m.snap());return events,snaps,'async_null_data',None
 info=data+44;m.put(info,m.w(data+12));position=call('position',m.w(entry+4),(m.w(entry+8),));m.put(info+4,position+aligned)
 if m.read(bank+0x148,1)[0]:actual_size=call('align',m.w(entry+4),(wanted,))
 else:actual_size=wanted
 m.put(info+8,actual_size);m.put(info+12,data);m.put(info+20,priority);m.write(info+16,b'\0')
 manager=m.w(m.w(data+40)+0x110);token=call('queue',manager,(info,));m.put(data+24,token)
 m.put(ctx['output'],data);m.put(ctx['output']+4,control)
 if control:
  count=m.w(control);m.put(control,count+1);m.put(control,count)
  if not count:call('counted_delete',m.w(control+8));call('delete',0,(control,))
 snaps.append(m.snap());return events,snaps,None,ctx['output']
u=Uc(UC_ARCH_X86,UC_MODE_32)
for page in {v&~4095 for v in (ENTRY,SETUP,GETDATA,ALIGNMENT,ALIGN,ALLOC,CTOR,DELETE,DELETER,ALTDELETER,POSITION,ADVANCE,QUEUE)}|{(DELETE&~4095)+4096}:u.mem_map(page,4096)
u.mem_map(A,SIZE);u.mem_map(S,65536);u.mem_map(STOP,4096);u.mem_write(ENTRY,root);u.mem_write(SETUP,setup);u.mem_write(GETDATA,nullprefix)
active=[None];intr=[None];invalid=[None]
def dispatch(uc,pc,n,data):active[0](uc,pc)
def interrupt_dispatch(uc,n,data):intr[0](uc,n)
def invalid_dispatch(uc,access,address,size,value,data):return invalid[0](uc,access,address,size)
u.hook_add(UC_HOOK_CODE,dispatch);u.hook_add(UC_HOOK_INTR,interrupt_dispatch);u.hook_add(UC_HOOK_MEM_INVALID,invalid_dispatch);cpu=u.context_save();registers=[UC_X86_REG_EBX,UC_X86_REG_EBP,UC_X86_REG_ESI,UC_X86_REG_EDI]
def run(initial,ctx):
 u.context_restore(cpu);u.mem_write(A,initial);u.mem_write(S,bytes(65536));m=Memory(u=u);events=[];snaps=[];fault=None;sp=S+0x8000
 u.mem_write(sp,word(STOP)+b''.join(word(v) for v in (ctx['output'],ctx['offset'],ctx['size'],ctx['priority'])));u.reg_write(UC_X86_REG_ESP,sp);u.reg_write(UC_X86_REG_ECX,ctx['entry']);u.reg_write(UC_X86_REG_EFLAGS,2)
 for i,r in enumerate(registers):u.reg_write(r,0x11223300+i)
 boundaries={ALIGNMENT:('alignment',0,0),ALIGN:('align',1,4),ALLOC:('allocate',1,0),CTOR:('construct',4,16),POSITION:('position',1,4),QUEUE:('queue',1,4),DELETE:('delete',1,0),DELETER:('counted_delete',0,0),ALTDELETER:('counted_delete',0,0)}
 def hook(uc,pc):
  if pc==STOP:uc.emu_stop();return
  if pc==GETDATA:assert ctx['allocation'] in (1,3),'nonnull sync path excluded'
  if pc not in boundaries:return
  name,nargs,cleanup=boundaries[pc];stack=uc.reg_read(UC_X86_REG_ESP);args=tuple(struct.unpack('<'+'I'*nargs,uc.mem_read(stack+4,4*nargs))) if nargs else ();selfp=0 if name in ('allocate','delete') else uc.reg_read(UC_X86_REG_ECX)
  result=callback(m,ctx,name,selfp,args,events,snaps);ret=struct.unpack('<I',uc.mem_read(stack,4))[0];uc.reg_write(UC_X86_REG_EAX,result);uc.reg_write(UC_X86_REG_ECX,0xabcdef01);uc.reg_write(UC_X86_REG_EDX,0x98765432);uc.reg_write(UC_X86_REG_ESP,stack+4+cleanup);uc.reg_write(UC_X86_REG_EIP,ret)
 def on_interrupt(uc,number):
  nonlocal fault
  assert number==0 and uc.reg_read(UC_X86_REG_EIP)==0x9d6127 and uc.reg_read(UC_X86_REG_ECX)==0
  assert next(Cs(CS_ARCH_X86,CS_MODE_32).disasm(bytes(uc.mem_read(0x9d6127,4)),0x9d6127)).mnemonic=='div'
  fault='alignment_divide';uc.emu_stop()
 def on_invalid(uc,access,address,size):
  nonlocal fault
  pc=uc.reg_read(UC_X86_REG_EIP)
  assert access==UC_MEM_READ_UNMAPPED,(access,hex(pc),address,size)
  assert (pc,address,size) in ((0x9d59c6,12,4),(0x9d52e0,40,4)),(hex(pc),address,size,ctx)
  fault='async_null_data' if pc==0x9d59c6 else 'sync_null_data';return False
 active[0]=hook;intr[0]=on_interrupt;invalid[0]=on_invalid
 try:u.emu_start(ENTRY,STOP,count=1500)
 except UcError as error:assert error.errno==UC_ERR_READ_UNMAPPED and fault in ('async_null_data','sync_null_data'),(error,fault)
 result=None
 if not fault:
  assert u.reg_read(UC_X86_REG_EIP)==STOP and u.reg_read(UC_X86_REG_ESP)==sp+20
  assert [u.reg_read(r) for r in registers]==[0x11223300+i for i in range(4)];result=u.reg_read(UC_X86_REG_EAX);assert result==ctx['output']
 snaps.append(m.snap());return events,snaps,fault,result
cases=snapshots=0;faults={name:0 for name in ('alignment_divide','async_null_data','sync_null_data')}
for offset,size,alignment,pad,sync,allocation,mutation,control_mode,place,fill in itertools.product((0,1,63,64,65,0xffffffff),(0,1,64,0xffffffc0),(0,1,64,0xffffffff),range(2),range(2),range(4),range(6),range(4),range(2),(0,0x95)):
 if sync and allocation not in (1,3):continue
 base=A+place*0x3000;initial=bytearray([fill]*SIZE);m=Memory(initial);entry=base+0x100;banks=(base+0x200,base+0x800);data=base+0x1000;control=base+0x1100;output=base+0x1200
 m.put(entry+4,banks[0]);m.put(entry+8,17)
 for index,bank in enumerate(banks):m.write(bank+0x148,bytes([pad,sync]));m.put(bank+0x118,0x11110000+index);m.put(bank+0x110,0x22220000+index)
 ctx=dict(entry=entry,banks=banks,data=data,control=control,output=output,buffer=base+0x2000,offset=offset,size=size,priority=0xffffffff,alignment=alignment,allocation=allocation,mutation=mutation,control_mode=control_mode)
 expected=model(initial,ctx);actual=run(bytes(initial),ctx);assert actual==expected,(ctx,actual[2],expected[2],actual[0],expected[0])
 cases+=1;snapshots+=len(expected[1])
 if expected[2]:faults[expected[2]]+=1
supplement_cases=supplement_snapshots=0;flag_checks=cleanup_checks=0
for alternate,cleanup,pads,mutation,priority in itertools.product(range(2),range(3),((0,1),(1,0)),(1,2,3,4,5),(0,1,0xffffffff)):
 extra=bytearray(initial);em=Memory(extra);context=dict(ctx,offset=65,size=64,alignment=64,allocation=0,control_mode=0,mutation=mutation,priority=priority,cleanup_mutation=cleanup,alternate_data=base+0x1500)
 if alternate:context['ctor_return']=base+0x1400
 for index,bank in enumerate(banks):em.write(bank+0x148,bytes([pads[index],0]))
 em.put(entry+4,banks[0]);expected=model(extra,context);actual=run(bytes(extra),context);assert actual==expected,(context,actual[0],expected[0])
 if cleanup:
  names=[e[0] for e in expected[0]];assert names[-2:]==['counted_delete','delete'];assert expected[0][-2][1]==context['alternate_data'];assert expected[0][-1][2]==(control,);cleanup_checks+=1
 supplement_cases+=1;supplement_snapshots+=len(expected[1]);flag_checks+=1
inputs=[Path(__file__),R/'tools/decomp_pipeline/pe_oracle.py',R/'tools/decomp_pipeline/crawl/rowtrim.py']
report=dict(status='ORIGINAL_ROOT_ACTUAL_SETUP_BOUNDED_NULL_PREFIX_MODEL_PASS',cases=cases+supplement_cases,snapshots=snapshots+supplement_snapshots,ordinary_cases=cases,ordinary_snapshots=snapshots,faults=faults,supplement_cases=supplement_cases,supplement_snapshots=supplement_snapshots,differing_bankflag_cases=flag_checks,live_control_cleanup_cases=cleanup_checks,input_sha256={str(p.relative_to(R)):hashlib.sha256(p.read_bytes()).hexdigest() for p in inputs},original_exe_sha256=hashlib.sha256(raw).hexdigest(),body_sha256={hex(address):hashlib.sha256(body).hexdigest() for address,body in ((ENTRY,root),(SETUP,setup),(GETDATA,nullprefix))},boundary='Original235B root, complete191B SetupRead, and original19B GetData9D52D0 prefix only to prove sync-null fault before first call. No candidate compilation or compiled-source proof. Alignment/Align/heap/constructor/entryoffset/queue/deletion explicit functional callbacks. Async valid data and async/sync null data; sync nonnull excluded. Actual DIV0 at9D6127, READ_UNMAPPED async9D59C6@0xC/sync9D52E0@0x28. Full32KiB arenas/boundary/final snapshots; stack excluded, hidden counted return/RET10/callee-register ABI on successes. Queue mutates control count/Data/DeleteFunc; publication precedes zero-count deletion and cleanup callbacks may mutate live output. Focused alternate constructor return and differing cached policy/livebank identity cases. No resource-pool/constructor/queue/deletion implementation or source-complete/new recovery claim.')
out=Path(__file__).with_name('NativeBankAsyncAutoAlloc_model_result.json');out.write_text(json.dumps(report,indent=2)+'\n')
print('ASYNC_AUTOALLOC_ORIGINAL_MODEL_PASS',cases+supplement_cases,snapshots+supplement_snapshots,faults,'supplement',supplement_cases,supplement_snapshots,flush=True)
