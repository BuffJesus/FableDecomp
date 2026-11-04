"""Independent bounded proof; allocator and data initializer remain boundaries."""
import itertools,struct,sys,json,subprocess
from pathlib import Path
from unicorn import Uc,UC_ARCH_X86,UC_MODE_32,UC_HOOK_CODE
from unicorn.x86_const import *
R=Path(__file__).resolve().parents[3];sys.path.insert(0,str(R/'tools/decomp_pipeline'))
import check_one as V
from gate_game_component_abi import Coff
from pe_oracle import EXE,read_virtual_data
SOURCE=R/'rebuild/src/compiled/00/9f/CGraphicFrame_InitialiseAsSurface_009fcb20.cpp'
import tempfile
BUILD=Path(tempfile.mkdtemp(prefix='native-surface-init-')) if len(sys.argv)==1 else Path(sys.argv[2]).parent
OBJ=BUILD/'root.obj';ENTRY=0x9fcb20;ALLOC=0xbfea1a;INIT=0x9fc950
A=0x2000000;S=0x3000000;STOP=0x4000000;SIZE=16384
def word(v):return struct.pack('<I',v&0xffffffff)
res=V.cl(['/nologo','/c','/O2','/Oy','/GS','/MT','/I'+str(R/'rebuild/include'),'/Fo'+str(OBJ),str(SOURCE)],V.env(),SOURCE.parent)
assert not res.returncode,res.stdout+res.stderr
c=Coff(OBJ);symbol=next(k for k in c.symbols if k.startswith('?InitialiseAsSurface@CGraphicFrame@@'))
section,offset=c.symbols[symbol];built,rel=c.sections[section];assert offset==0
native=read_virtual_data(EXE.read_bytes(),ENTRY,57);patched=bytearray(built)
assert set(rel)=={6,41,53}
for site,(target,kind) in rel.items():
 assert kind==20
 address=ALLOC if target=='??2@YAPAXI@Z' else INIT
 assert target=='??2@YAPAXI@Z' or target.startswith('?InitialiseAsSurface@CGraphicFrameData@@')
 patched[site:site+4]=word(address-(ENTRY+site+4))
assert bytes(patched)==native,'Not exact after audited relocation resolution'
registers=[UC_X86_REG_EBX,UC_X86_REG_EBP,UC_X86_REG_ESI,UC_X86_REG_EDI]
machines={}
active_hook=[None]
def dispatch(uc,pc,size,data):active_hook[0](uc,pc,size,data)
for body in (native,bytes(patched)):
 u=Uc(UC_ARCH_X86,UC_MODE_32)
 for address,length in [(ENTRY&~4095,4096),(ALLOC&~4095,4096),(A,SIZE),(S,65536),(STOP,4096)]:u.mem_map(address,length)
 u.mem_write(ENTRY,body);u.hook_add(UC_HOOK_CODE,dispatch);machines[body]=u
def run(body,initial,frame,args,allocated,result):
 u=machines[body];u.mem_write(A,initial);sp=S+0x8000
 u.mem_write(S,bytes(65536))
 for r in (UC_X86_REG_EAX,UC_X86_REG_EDX):u.reg_write(r,0)
 u.reg_write(UC_X86_REG_EFLAGS,2)
 u.mem_write(sp,word(STOP)+b''.join(word(x) for x in args));u.reg_write(UC_X86_REG_ESP,sp);u.reg_write(UC_X86_REG_ECX,frame)
 for i,r in enumerate(registers):u.reg_write(r,0x11223300+i)
 snapshots=[];events=[]
 def hook(uc,pc,size,data):
  if pc==STOP:uc.emu_stop();return
  if pc not in (ALLOC,INIT):return
  stack=uc.reg_read(UC_X86_REG_ESP);selfp=uc.reg_read(UC_X86_REG_ECX)
  if pc==ALLOC:
   n=struct.unpack('<I',uc.mem_read(stack+4,4))[0];assert n==27
   events.append(('allocate',n));snapshots.append(bytes(uc.mem_read(A,SIZE)));ret=allocated;cleanup=0
  else:
   values=struct.unpack('<IIIII',uc.mem_read(stack+4,20));assert values==args
   events.append(('initialize',selfp,values));snapshots.append(bytes(uc.mem_read(A,SIZE)));ret=result;cleanup=20
   # Observable boundary writes expose owner reload timing without inventing its implementation.
   uc.mem_write(frame+4,word(selfp));uc.mem_write(frame+8,word(result))
  uc.reg_write(UC_X86_REG_EAX,ret);uc.reg_write(UC_X86_REG_ECX,0xabcdef01);uc.reg_write(UC_X86_REG_EDX,0x98765432)
  destination=struct.unpack('<I',uc.mem_read(stack,4))[0];uc.reg_write(UC_X86_REG_ESP,stack+4+cleanup);uc.reg_write(UC_X86_REG_EIP,destination)
 active_hook[0]=hook;u.emu_start(ENTRY,STOP,count=200)
 assert u.reg_read(UC_X86_REG_EIP)==STOP
 assert u.reg_read(UC_X86_REG_ESP)==sp+24
 assert [u.reg_read(r) for r in registers]==[0x11223300+i for i in range(4)]
 assert u.reg_read(UC_X86_REG_EAX)==result
 snapshots.append(bytes(u.mem_read(A,SIZE)));return events,snapshots
if len(sys.argv)==1:
 for start in range(0,256,32):
  p=subprocess.run([sys.executable,__file__,str(start),str(OBJ)],capture_output=True,text=True)
  assert not p.returncode,p.stdout+p.stderr
  assert 'GRAPHIC_FRAME_INITIALISE_SURFACE_BATCH_PASS 4608 13824' in p.stdout,p.stdout
  print('batch',start,'pass',flush=True)
 from surface_initialise_gate_support import finish
 finish(R,SOURCE,OBJ,symbol,[target for target,kind in rel.values()],ENTRY,native,built,rel,36864,110592,'native-graphic-frame-initialise-surface.json',Path(__file__))
 print('NATIVE_GRAPHIC_FRAME_INITIALISE_SURFACE_GATE_PASS',36864,110592)
 sys.exit(0)
start=int(sys.argv[1]);count=0;snapshot_count=0
for flag,mode,place,fill,values,result in itertools.product(range(start,start+32),range(3),range(2),range(2),range(3),[0,1,0x80,0xff]):
 base=A+place*0x1800;frame=base+0x100;block=base+0x200
 allocated=[0,block,frame][mode]
 initial=bytearray([0xa9 if fill else 0]*SIZE)
 def put(buf,addr,value):buf[addr-A:addr-A+4]=word(value)
 put(initial,frame,base+0x400)
 initial[(allocated or block)-A+26]=flag
 args=(base+0x600,[0,0x80000000,0xffffffff][values],[0x7fffffff,65535,1][values],[0,0x80,0xff][values],[1,0xff,0x80][values])
 expected=bytearray(initial);snaps=[bytes(expected)]
 if allocated:
  put(expected,allocated,1);put(expected,allocated+8,0);put(expected,allocated+12,0)
  expected[allocated-A+26]&=0xef
 put(expected,frame,allocated);snaps.append(bytes(expected))
 put(expected,frame+4,allocated);put(expected,frame+8,result);snaps.append(bytes(expected))
 events=[('allocate',27),('initialize',allocated,args)]
 for body in (native,bytes(patched)):
  got_events,got_snaps=run(body,bytes(initial),frame,args,allocated,result)
  assert got_events==events and got_snaps==snaps,(flag,mode,place,fill,values,result)
 count+=1;snapshot_count+=3
 if count%10000==0:print('progress',count,flush=True)
print('GRAPHIC_FRAME_INITIALISE_SURFACE_BATCH_PASS',count,snapshot_count)
