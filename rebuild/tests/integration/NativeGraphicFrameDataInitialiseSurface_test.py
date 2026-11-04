"""Scratch independent ownership/dimension differential proof."""
import itertools,struct,sys,subprocess
from pathlib import Path
from unicorn import Uc,UC_ARCH_X86,UC_MODE_32,UC_HOOK_CODE
from unicorn.x86_const import *
R=Path(__file__).resolve().parents[3];sys.path.insert(0,str(R/'tools/decomp_pipeline'))
import check_one as V
from gate_game_component_abi import Coff
from pe_oracle import EXE,read_virtual_data
ENTRY=0x9fc950;FREE=0xbfe9bc;DIM=0x9f9db0
A=0x2000000;S=0x3000000;STOP=0x4000000;DELETE=STOP+0x100;SIZE=16384
def w(v):return struct.pack('<I',v&0xffffffff)
SOURCE=R/'rebuild/src/compiled/00/9f/CGraphicFrameData_InitialiseAsSurface_009fc950.cpp'
import tempfile
BUILD=Path(tempfile.mkdtemp(prefix='native-surface-init-')) if len(sys.argv)==1 else Path(sys.argv[2]).parent
OBJ=BUILD/'root.obj'
if len(sys.argv)==1:
 q=V.cl(['/nologo','/c','/O2','/Oy','/GS','/MT','/I'+str(R/'rebuild/include'),'/Fo'+str(OBJ),str(SOURCE)],V.env(),SOURCE.parent)
 assert not q.returncode,q.stdout+q.stderr
 for first in range(0,256,32):
  q=subprocess.run([sys.executable,__file__,str(first),str(OBJ)],capture_output=True,text=True)
  assert not q.returncode,q.stdout+q.stderr
  print(q.stdout.strip(),flush=True)
 c=Coff(OBJ);symbol=next(k for k in c.symbols if k.startswith('?InitialiseAsSurface@CGraphicFrameData@@'))
 compiled,rel=c.sections[c.symbols[symbol][0]]
 from surface_initialise_gate_support import finish
 finish(R,SOURCE,OBJ,symbol,[target for target,kind in rel.values()],ENTRY,read_virtual_data(EXE.read_bytes(),ENTRY,230),compiled,rel,18432,47104,'native-graphic-frame-data-initialise-surface.json',Path(__file__))
 print('NATIVE_GRAPHIC_FRAME_DATA_INITIALISE_SURFACE_GATE_PASS',18432,47104)
 sys.exit(0)
c=Coff(OBJ);symbol=next(k for k in c.symbols if k.startswith('?InitialiseAsSurface@CGraphicFrameData@@'))
body,rel=c.sections[c.symbols[symbol][0]];compiled=bytearray(body)
for at,(symbol,kind) in rel.items():
 assert kind==20
 target=FREE if symbol=='??3@YAXPAX@Z' else DIM
 assert symbol=='??3@YAXPAX@Z' or symbol=='?GetDimensions@CTexture@@QBE?AVC2DExtentsI@@K@Z'
 compiled[at:at+4]=w(target-(ENTRY+at+4))
native=read_virtual_data(EXE.read_bytes(),ENTRY,230)
class Mem:
 def __init__(self,initial,u=None):self.u=u;self.b=bytearray(initial)
 def read(self,p,n):return bytes(self.u.mem_read(p,n)) if self.u else bytes(self.b[p-A:p-A+n])
 def write(self,p,b):
  if self.u:self.u.mem_write(p,bytes(b))
  else:self.b[p-A:p-A+len(b)]=b
 def get(self,p):return struct.unpack('<I',self.read(p,4))[0]
 def put(self,p,v):self.write(p,w(v))
 def snap(self):return self.read(A,SIZE)
machines=[];current=[None]
def dispatch(u,pc,size,data):
 if pc not in (DELETE,FREE,DIM):return
 case=current[0];m=case['m'];sp=u.reg_read(UC_X86_REG_ESP);ecx=u.reg_read(UC_X86_REG_ECX)
 if pc==DELETE:
  boundary(case,pc,ecx);cleanup=0;ret=0
 elif pc==FREE:
  boundary(case,pc,m.get(sp+4));cleanup=0;ret=0
 else:
  output=m.get(sp+4);assert m.get(sp+8)==0
  x,y=boundary(case,pc,ecx);u.mem_write(output,w(x)+w(y));cleanup=8;ret=output
 u.reg_write(UC_X86_REG_EAX,ret);u.reg_write(UC_X86_REG_ECX,0xabcddcba);u.reg_write(UC_X86_REG_EDX,0x87654321)
 destination=m.get(sp);u.reg_write(UC_X86_REG_ESP,sp+4+cleanup);u.reg_write(UC_X86_REG_EIP,destination)
for code in (native,bytes(compiled)):
 u=Uc(UC_ARCH_X86,UC_MODE_32)
 for address,length in [(ENTRY&~4095,4096),(FREE&~4095,4096),(DIM&~4095,4096),(A,SIZE),(S,65536),(STOP,4096)]:u.mem_map(address,length)
 u.mem_write(ENTRY,code);u.hook_add(UC_HOOK_CODE,dispatch);machines.append(u)
def boundary(case,pc,value):
 m=case['m'];events=case['events'];events.append((pc,value,m.snap()))
 f,arg,old,new,third=case['ptrs'];profile=case['profile']
 if pc==DELETE and profile:
  m.put(f+12,third);m.put(arg,case['tex'][1]);m.write(f+26,bytes([0xb8]))
 if pc==DIM:
  call=case['dims'];case['dims']+=1
  if profile==1:
   m.put(arg,case['tex'][1]);m.write(f+20,struct.pack('<HH',7,9))
  if profile==2:m.write(f+26,bytes([0xd7]))
  return case['answers'][call]
 return 0
count=0;snaps=0
first=int(sys.argv[1])
for flag,state,profile,dimension in itertools.product(range(first,first+32),range(8),range(3),range(3)):
 base=A+(0x1800 if flag&1 else 0);f=base+0x100;arg=base+0x200;old=base+0x400;new=base+0x440;third=base+0x480
 tex=[base+0x600,base+0x640];initial=bytes([0xa9 if flag&2 else 0])*SIZE;m=Mem(initial)
 source_info=[0,new,0,old,new,old,new,new][state];dest_info=0 if state<2 else old
 if state==5:arg=f+8
 m.put(f+8,tex[0]);m.put(f+12,dest_info);m.put(arg,tex[1]);m.put(arg+4,source_info);m.write(f+26,bytes([flag]))
 for i,p in enumerate((old,new,third)):
  m.put(p,[0,1,2,0xffffffff][(state+i)%4]);m.put(p+4,DELETE);m.put(p+8,tex[i%2])
 width=[0,65543,0xffffffff][dimension];height=[0,9,65535][dimension]
 answers=[(width&65535,0x55),(height&65535,0xaa)]
 if dimension==2:answers[0]=(1,65535)
 args=(arg,width,height,[0,0x80,0xff][dimension],[0xff,1,0x80][dimension])
 initial=m.snap()
 def case(memory):return dict(m=memory,events=[],ptrs=(f,arg,old,new,third),profile=profile,tex=tex,answers=answers,dims=0)
 model=case(Mem(initial));m=model['m'];m.write(f+26,bytes([m.read(f+26,1)[0]|4]))
 cached_data=m.get(arg);cached_info=m.get(arg+4)
 if m.get(f+12)!=cached_info:
  info=m.get(f+12)
  if info:
   m.put(info,m.get(info)-1)
   if m.get(m.get(f+12))==0:
    info=m.get(f+12);boundary(model,DELETE,m.get(info+8));boundary(model,FREE,m.get(f+12))
  m.put(f+8,cached_data);m.put(f+12,cached_info)
  if cached_info:m.put(cached_info,m.get(cached_info)+1)
 m.write(f+20,struct.pack('<HHH',width&65535,height&65535,width&65535))
 bits=m.read(f+26,1)[0];bits=(bits&0xf4)|(args[3]&1)|((args[4]&1)<<1)|16
 m.write(f+26,bytes([bits]));m.write(f+16,bytes(4))
 x,y=boundary(model,DIM,m.get(arg));different=struct.unpack('<H',m.read(f+20,2))[0]!=x
 if not different:
  x,y=boundary(model,DIM,m.get(arg));different=struct.unpack('<H',m.read(f+22,2))[0]!=x
 bits=m.read(f+26,1)[0];m.write(f+26,bytes([(bits&~32)|(32 if different else 0)]))
 expected=m.snap()
 for u in machines:
  u.mem_write(A,initial);u.mem_write(S,bytes(65536));sp=S+0x8000;u.mem_write(sp,w(STOP)+b''.join(w(x) for x in args))
  u.reg_write(UC_X86_REG_ESP,sp);u.reg_write(UC_X86_REG_ECX,f);u.reg_write(UC_X86_REG_EFLAGS,2)
  regs=[UC_X86_REG_EBX,UC_X86_REG_EBP,UC_X86_REG_ESI,UC_X86_REG_EDI]
  for i,r in enumerate(regs):u.reg_write(r,0x11223300+i)
  actual=case(Mem(initial,u));current[0]=actual;u.emu_start(ENTRY,STOP,count=500)
  assert actual['events']==model['events'] and actual['m'].snap()==expected,(flag,state,profile,dimension)
  assert u.reg_read(UC_X86_REG_EAX)&255==1 and u.reg_read(UC_X86_REG_ESP)==sp+24
  assert [u.reg_read(r) for r in regs]==[0x11223300+i for i in range(4)]
 count+=1;snaps+=len(model['events'])+1
print('GRAPHIC_FRAME_DATA_INITIALISE_SURFACE_BATCH_PASS',count,snaps)
