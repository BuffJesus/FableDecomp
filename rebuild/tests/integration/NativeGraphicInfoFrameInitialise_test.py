"""Original/COFF/independent ownership glue proof; explicit helper boundaries."""
import hashlib,itertools,json,struct,subprocess,sys
from pathlib import Path
from unicorn import Uc,UcError,UC_ARCH_X86,UC_MODE_32,UC_HOOK_CODE,UC_ERR_READ_UNMAPPED
from unicorn.x86_const import *
R=Path(__file__).resolve().parents[3];sys.path.insert(0,str(R/'tools/decomp_pipeline'))
import check_one as V
from gate_game_component_abi import Coff
from pe_oracle import EXE,read_virtual_data
from variable_allocator_parity import check
SOURCE=R/'rebuild/src/compiled/00/9f/CGraphicInfoFrame_InitialiseFrame_009fe550.cpp'
B=R/'work/codex-native-startup/native-graphic-info-frame-initialise-20261010';B.mkdir(parents=True,exist_ok=True)
OBJ=B/'root.obj';ENTRY=0x9fe550;A=0x2000000;S=0x3000000;STOP=0x4000000;SIZE=16384
SYMBOL='?InitialiseFrame@CGraphicInfoFrame@@AAEXAAVCGraphicFrame@@AAVCManagedTexture@@C@Z'
targets={'??2@YAPAXI@Z':0xbfea1a,'??0CFrameData@CGraphicInfoFrame@@QAE@PBV1@@Z':0x9fe380,'?Uninitialise@CGraphicFrameData@@QAEXXZ':0x9fcba0,'??3@YAXPAX@Z':0xbfe9bc,'??4CManagedTexture@@QAEABV0@ABV0@@Z':0xa69b20,'?Release@CManagedTexture@@QAE_NXZ':0xa69ad0}
ALLOC,CTOR,UNINIT,DELETE,ASSIGN,RELEASE=[targets[k] for k in targets]
IVDELETE=0x5000100;COUNTDELETE=0x5000200;VT=0x5000800
def word(n):return struct.pack('<I',n&0xffffffff)
q=V.cl(['/nologo','/c','/O2','/MT','/Fo'+str(OBJ),str(SOURCE)],V.env(),B);assert not q.returncode,q.stdout+q.stderr
c=Coff(OBJ);section,offset=c.symbols[SYMBOL];built,rel=c.sections[section];assert offset==0 and len(built)==208 and len(rel)==7
native=read_virtual_data(EXE.read_bytes(),ENTRY,208);patched=bytearray(built)
for site,(name,kind) in rel.items():
 assert name in targets and kind==20 and built[site:site+4]==word(0),(site,name,kind,built[site:site+4]);patched[site:site+4]=word(targets[name]-(ENTRY+site+4))
assert bytes(patched)==native
proof=check(OBJ,SYMBOL,ENTRY,208,7,targets);assert proof['grade']=='MATCH'

# Separate required caller preserves canonical private method declaration.
registers=[UC_X86_REG_EBX,UC_X86_REG_EBP,UC_X86_REG_ESI,UC_X86_REG_EDI]
machines={};active=[None]
def dispatch(u,pc,n,data):active[0](u,pc,n,data)
for name,body in [('retail',native),('compiled',bytes(patched))]:
 u=Uc(UC_ARCH_X86,UC_MODE_32)
 pages={ENTRY&~4095,IVDELETE&~4095}|{v&~4095 for v in targets.values()}
 for p in pages:u.mem_map(p,4096)
 u.mem_map(A,SIZE);u.mem_map(S,65536);u.mem_map(STOP,4096);u.mem_write(ENTRY,body);u.mem_write(VT,word(0)+word(IVDELETE));u.hook_add(UC_HOOK_CODE,dispatch);machines[name]=u

class Memory:
 def __init__(self,buf=None,u=None):self.buf=buf;self.u=u
 def read(self,p,n):return bytes(self.buf[p-A:p-A+n]) if self.buf is not None else bytes(self.u.mem_read(p,n))
 def write(self,p,b):
  if self.buf is not None:self.buf[p-A:p-A+len(b)]=b
  else:self.u.mem_write(p,b)
 def w(self,p):return struct.unpack('<I',self.read(p,4))[0]
 def put(self,p,v):self.write(p,word(v))
 def snap(self):return self.read(A,SIZE)

def callback(m,ctx,name,selfp,args,events,snaps):
 frame,owner,fresh,alternate,old,incoming,control,replacement,src,managed=ctx['addresses'];mode=ctx['mode']
 events.append((name,selfp,tuple(args)));snaps.append(m.snap())
 ret=0
 if name=='allocate':
  assert args==(68,);ret=0 if mode==11 else fresh
  if mode==10:m.put(frame+8,owner)
 elif name=='construct':
  assert selfp==fresh and args==(frame,);ret=fresh
  # Boundary deliberately populates authentic fields, ctor behavior proved separately.
  m.put(fresh+4,1);m.put(fresh+28,0);m.put(fresh+36,incoming);m.write(fresh+56,b'\xff')
  if mode in (2,10):m.put(frame+8,owner)
 elif name=='intrusive_delete':
  assert selfp==owner and not args
  if mode==2:m.put(frame+8,alternate)
 elif name=='data_uninitialise':
  assert selfp==old and not args;m.write(old+26,bytes([m.read(old+26,1)[0]&0xef]))
  if mode in (3,7):m.put(frame+8,alternate);m.put(src,incoming)
  if mode==8:m.put(old+12,replacement)
  if mode==14:m.put(old+12,0)
 elif name=='counted_delete':
  assert selfp in (incoming,old) and not args
  if mode in (4,8):m.put(old+12,replacement);m.put(frame+8,alternate);m.put(src,incoming)
  if mode==15:m.put(old+12,0)
 elif name=='heap_delete':
  assert selfp==0 and len(args)==1 and args[0] in (0,control,replacement,old)
  if mode==6:m.put(frame+8,alternate);m.put(src,incoming)
 elif name=='managed_assign':
  assert args==(managed,) and selfp in (owner+32,fresh+32,alternate+32)
  # Functional interface effects expose pointer timing; NOT recovered helper ownership claim.
  m.put(selfp+4,m.w(managed+4));ret=selfp
  if mode in (5,9):m.put(frame+8,alternate)
 elif name=='managed_release':
  assert selfp==managed and not args;m.put(managed+4,0);ret=1
  if mode in (7,9):m.put(frame+8,alternate)
 else:raise AssertionError(name)
 return ret

def model(initial,ctx):
 m=Memory(bytearray(initial));events=[];snaps=[]
 frame,owner,fresh,alternate,old,incoming,control,replacement,src,managed=ctx['addresses']
 def call(name,selfp=0,args=()):return callback(m,ctx,name,selfp,args,events,snaps)
 if not m.w(frame+8):
  value=call('allocate',0,(68,))
  if value:value=call('construct',value,(frame,))
  previous=m.w(frame+8)
  if previous:
   count=(m.w(previous+4)-1)&0xffffffff;m.put(previous+4,count)
   if not count:call('intrusive_delete',previous)
   m.put(frame+8,0)
  m.put(frame+8,value)
 if not m.w(frame+8):snaps.append(m.snap());return events,snaps,'null_owner_read'
 slot=m.w(frame+8)+28;previous=m.w(slot)
 if previous:
  count=(m.w(previous)-1)&0xffffffff;m.put(previous,count)
  if not count:
   if m.read(previous+26,1)[0]&0x10:call('data_uninitialise',previous)
   info=m.w(previous+12)
   if info:
    count=(m.w(info)-1)&0xffffffff;m.put(info,count)
    info=m.w(previous+12)
    if not m.w(info):
     call('counted_delete',m.w(info+8));call('heap_delete',0,(m.w(previous+12),))
   m.put(previous+8,0);m.put(previous+12,0);call('heap_delete',0,(previous,))
 value=m.w(src);m.put(slot,value)
 if value:m.put(value,m.w(value)+1)
 call('managed_assign',m.w(frame+8)+32,(managed,));call('managed_release',managed)
 m.write(m.w(frame+8)+56,bytes([ctx['mip']]))
 snaps.append(m.snap());return events,snaps,None

def run(kind,initial,ctx):
 u=machines[kind];u.mem_write(A,initial);u.mem_write(S,bytes(65536));sp=S+0x8000
 frame,owner,fresh,alternate,old,incoming,control,replacement,src,managed=ctx['addresses']
 u.mem_write(sp,word(STOP)+word(src)+word(managed)+word(ctx['mip']));u.reg_write(UC_X86_REG_ESP,sp);u.reg_write(UC_X86_REG_ECX,frame);u.reg_write(UC_X86_REG_EFLAGS,2)
 for i,r in enumerate(registers):u.reg_write(r,0x11223300+i)
 events=[];snaps=[];m=Memory(u=u)
 boundary={ALLOC:('allocate',1,0),CTOR:('construct',1,4),UNINIT:('data_uninitialise',0,0),DELETE:('heap_delete',1,0),ASSIGN:('managed_assign',1,4),RELEASE:('managed_release',0,0),IVDELETE:('intrusive_delete',0,0),COUNTDELETE:('counted_delete',0,0)}
 def hook(uc,pc,n,data):
  if pc==STOP:uc.emu_stop();return
  if pc not in boundary:return
  name,nargs,cleanup=boundary[pc];stack=uc.reg_read(UC_X86_REG_ESP)
  args=tuple(struct.unpack('<'+'I'*nargs,uc.mem_read(stack+4,4*nargs))) if nargs else ()
  selfp=0 if name in ('allocate','heap_delete') else uc.reg_read(UC_X86_REG_ECX)
  result=callback(m,ctx,name,selfp,args,events,snaps)
  destination=m.w(stack) if A<=stack<A+SIZE else struct.unpack('<I',uc.mem_read(stack,4))[0]
  uc.reg_write(UC_X86_REG_EAX,result);uc.reg_write(UC_X86_REG_ECX,0xabcdef01);uc.reg_write(UC_X86_REG_EDX,0x98765432);uc.reg_write(UC_X86_REG_ESP,stack+4+cleanup);uc.reg_write(UC_X86_REG_EIP,destination)
 active[0]=hook;fault=None
 try:u.emu_start(ENTRY,STOP,count=500)
 except UcError as e:
  assert ctx['mode']==11 and e.errno==UC_ERR_READ_UNMAPPED and u.reg_read(UC_X86_REG_EIP)==0x9fe596,(ctx,e,u.reg_read(UC_X86_REG_EIP));fault='null_owner_read'
 if not fault:
  assert u.reg_read(UC_X86_REG_EIP)==STOP and u.reg_read(UC_X86_REG_ESP)==sp+16
  assert [u.reg_read(r) for r in registers]==[0x11223300+i for i in range(4)]
 snaps.append(m.snap());return events,snaps,fault

if len(sys.argv)==1:
 raise SystemExit('Run through gate_native_graphic_info_frame_initialise.py')
def case_initial(mip,mode,count,place,fill):
 base=A+place*0x1800
 addresses=tuple(base+x for x in (0x100,0x200,0x300,0x400,0x500,0x600,0x700,0x800,0x900,0xa00))
 frame,owner,fresh,alternate,old,incoming,control,replacement,src,managed=addresses
 ctx=dict(addresses=addresses,mode=mode,mip=mip)
 initial=bytearray([0x95 if fill else 0]*SIZE);m=Memory(initial)
 m.put(frame+8,0 if mode in (1,2,10,11) else owner)
 for block in (owner,fresh,alternate):m.put(block,VT);m.put(block+4,count);m.put(block+28,old if block==owner and mode!=0 else 0);m.put(block+36,incoming)
 m.put(old,count);m.put(old+8,incoming);m.put(old+12,control);m.write(old+26,bytes([mip]))
 for info in (control,replacement):m.put(info,1 if info==replacement else count);m.put(info+4,COUNTDELETE);m.put(info+8,incoming)
 m.put(incoming,2);m.put(base+0xb00,2);m.put(src,base+0xb00);m.put(managed+4,incoming)
 if mode==9:ctx['addresses']=addresses[:-1]+(owner+32,)
 return initial,ctx

def verify(initial,ctx):
 expected=model(initial,ctx)
 for kind in ('retail','compiled'):
  actual=run(kind,bytes(initial),ctx);assert actual==expected,(ctx,kind,[e[0] for e in actual[0]],[e[0] for e in expected[0]])
 return len(expected[1]),bool(expected[2])

if sys.argv[1]=='supplement':
 cases=snaps_count=0;groups={n:dict(cases=0,snapshots=0) for n in ('null_incoming','null_texture_info','self_frame_count2','self_frame_countFFFFFFFF','managed_alias_null_incoming')}
 for mip,group,place,fill in itertools.product(range(256),groups,range(2),range(2)):
  mode=9 if group=='managed_alias_null_incoming' else 0 if group.startswith('self_frame') else 5
  count=0xffffffff if group=='self_frame_countFFFFFFFF' else 2 if group.startswith('self_frame') or mode==9 else 1
  initial,ctx=case_initial(mip,mode,count,place,fill);m=Memory(initial)
  frame,owner,fresh,alternate,old,incoming,control,replacement,src,managed=ctx['addresses']
  if 'null_incoming' in group:m.put(src,0)
  if group=='null_texture_info':m.put(old+12,0)
  if group.startswith('self_frame'):
   m.put(owner+28,old);ctx['addresses']=ctx['addresses'][:8]+(owner+28,managed)
  snapshots,fault=verify(initial,ctx);assert not fault
  cases+=1;snaps_count+=snapshots;groups[group]['cases']+=1;groups[group]['snapshots']+=snapshots
 print('INITIALISE_FRAME_SUPPLEMENT_PASS',cases,snaps_count,json.dumps(groups,sort_keys=True),flush=True);sys.exit(0)

if sys.argv[1]=='expanded':
 cases=snaps_count=null_heap_calls=uninit_clears=0
 groups={n:dict(cases=0,snapshots=0) for n in ('old_count1_info_count0','old_count1_info_count2','old_count1_info_countFFFFFFFF','uninitialise_clears_info','counted_delete_clears_info')}
 for mip,group,place,fill in itertools.product(range(256),groups,range(2),range(2)):
  mode=14 if group=='uninitialise_clears_info' else 15 if group=='counted_delete_clears_info' else 5
  initial,ctx=case_initial(mip,mode,1,place,fill);m=Memory(initial)
  frame,owner,fresh,alternate,old,incoming,control,replacement,src,managed=ctx['addresses']
  # Independent outer/frame/control counters: oldgraphic must actually hit itszero branch.
  m.put(owner+4,2);m.put(old,1)
  info_count={'old_count1_info_count0':0,'old_count1_info_count2':2,'old_count1_info_countFFFFFFFF':0xffffffff,'uninitialise_clears_info':2,'counted_delete_clears_info':1}[group]
  m.put(control,info_count)
  events,expected_snaps,fault=model(initial,ctx);assert not fault
  snapshots,got_fault=verify(initial,ctx);assert not got_fault
  if group.startswith('old_count1_info_count'):
   assert not any(e[0]=='counted_delete' for e in events)
   assert [e[2][0] for e in events if e[0]=='heap_delete']==[old]
   assert struct.unpack('<I',expected_snaps[-1][control-A:control-A+4])[0]==(info_count-1)&0xffffffff
  if group=='uninitialise_clears_info' and mip&0x10:
   assert not any(e[0]=='counted_delete' for e in events)
   assert struct.unpack('<I',expected_snaps[-1][control-A:control-A+4])[0]==info_count
   uninit_clears+=1
  if group=='counted_delete_clears_info':
   assert [e[2][0] for e in events if e[0]=='heap_delete']==[0,old]
   null_heap_calls+=1
  cases+=1;snaps_count+=snapshots;groups[group]['cases']+=1;groups[group]['snapshots']+=snapshots
 print('INITIALISE_FRAME_EXPANDED_PASS',cases,snaps_count,null_heap_calls,uninit_clears,json.dumps(groups,sort_keys=True),flush=True);sys.exit(0)

start=int(sys.argv[1]);cases=snaps_count=faults=0
for mip,mode,count,place,fill in itertools.product(range(start,start+16),range(12),(0,1,2,0xffffffff),range(2),range(2)):
 initial,ctx=case_initial(mip,mode,count,place,fill);snapshots,fault=verify(initial,ctx)
 cases+=1;snaps_count+=snapshots;faults+=fault
print('INITIALISE_FRAME_BATCH_PASS',start,cases,snaps_count,faults,flush=True)
