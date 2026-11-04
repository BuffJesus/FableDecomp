"""Execute CDefClassBase's defaults, InternalSaveBinary and UnloadResource against retail."""
import itertools,struct
from unicorn import Uc,UC_ARCH_X86,UC_MODE_32,UC_HOOK_CODE
from unicorn.x86_const import *
DEF=0x2000000;MANAGER=0x2001000;TABLE=0x2002000;INFOS=0x2003000;HELD=0x2004000;STREAM=0x2005000;VT=0x2006000;STACK=0x3000000;STOP=0x4000000
WRITE_UBYTE=0x993e50;DELETE_SLOT=0x5000000
DEFAULTS=[(0x42da10,1),(0x42da20,1),(0x42da30,3),(0x42da40,1),(0x42da50,1),(0x42da60,1),(0x42da70,0),(0x42da80,0),(0x42da90,2),(0x42daa0,0)]
SAVE,UNLOAD=0x9acab0,0x9ace90
def run(codes,entry,args,setup=None,ecx=DEF):
 u=Uc(UC_ARCH_X86,UC_MODE_32)
 for a,z in [(0x42d000,0x1000),(0x993000,0x1000),(0x9ac000,0x1000),(DEF,0x10000),(STACK,0x10000),(STOP,0x1000),(DELETE_SLOT,0x1000)]:u.mem_map(a,z)
 def put(a,v):u.mem_write(a,struct.pack('<I',v&0xffffffff))
 def get(a):return struct.unpack('<I',u.mem_read(a,4))[0]
 for a,b in codes.items():u.mem_write(a,bytes(b))
 u.mem_write(WRITE_UBYTE,b'\xc3');u.mem_write(DELETE_SLOT,b'\xc3')
 u.mem_write(DEF,b'\x3c'*0x6000);put(VT+4,DELETE_SLOT)
 if setup:setup(u,put)
 trace=[]
 def hook(uc,a,s,user):
  if a not in (WRITE_UBYTE,DELETE_SLOT):return
  sp=uc.reg_read(UC_X86_REG_ESP);this=uc.reg_read(UC_X86_REG_ECX)
  if a==WRITE_UBYTE:trace.append(('ubyte',this,get(sp+4)&0xff));pop=4
  else:trace.append(('delete',this));pop=0
  uc.reg_write(UC_X86_REG_EIP,get(sp));uc.reg_write(UC_X86_REG_ESP,sp+4+pop)
 u.hook_add(UC_HOOK_CODE,hook);sp=STACK+0x8000;put(sp,STOP)
 for i,v in enumerate(args):put(sp+4+4*i,v)
 regs=[UC_X86_REG_EBX,UC_X86_REG_ESI,UC_X86_REG_EDI,UC_X86_REG_EBP]
 for i,r in enumerate(regs):u.reg_write(r,0x778800+i)
 u.reg_write(UC_X86_REG_ECX,ecx);u.reg_write(UC_X86_REG_ESP,sp);u.reg_write(UC_X86_REG_EAX,0xdeadbeef)
 u.emu_start(entry,STOP,count=1000)
 assert u.reg_read(UC_X86_REG_ESP)==sp+4+4*len(args) and all(u.reg_read(r)==0x778800+i for i,r in enumerate(regs)),hex(entry)
 return u.reg_read(UC_X86_REG_EAX),trace,bytes(u.mem_read(DEF,0x6000))
def compare(candidate,native):
 cases=0;pristine=b'\x3c'*0x6000
 for entry,argc in DEFAULTS:
  for seed in range(4):
   args=[0x7000+seed*16+i for i in range(argc)]
   if entry==0x42da10:args=[DEF+0x5000+seed*4]
   c=run(candidate,entry,args);assert c==run(native,entry,args)
   eax,trace,mem=c;assert not trace
   if entry==0x42da10:assert eax==args[0] and mem[0x5000+seed*4:0x5004+seed*4]==b'\xff\xff\xff\xff' and mem[:0x5000]==pristine[:0x5000]
   else:
    assert mem==pristine
    if entry in (0x42da50,0x42da60,0x42daa0):assert eax==0
    if entry==0x42da70:assert eax&0xff==0
   cases+=1
 for flags,junk in itertools.product(range(8),(0,0xf8)):
  def setup(u,put,flags=flags,junk=junk):u.mem_write(DEF+0x24,bytes([flags|junk]))
  c=run(candidate,SAVE,[STREAM],setup);assert c==run(native,SAVE,[STREAM],setup)
  assert c[1]==[('ubyte',STREAM,flags&1),('ubyte',STREAM,(flags>>1)&1),('ubyte',STREAM,(flags>>2)&1)];cases+=1
 for slots,count,null in itertools.product((1,3,6),(0,1,2,3,4,0x80000000,0xffffffff),(0,1)):
  for index in range(slots):
   def setup(u,put,slots=slots,count=count,null=null,index=index):
    put(DEF+0x1c,MANAGER);put(DEF+0x20,index);put(MANAGER+0xa4,TABLE);put(MANAGER+0xa8,TABLE+4*slots)
    for i in range(slots):
     put(TABLE+4*i,INFOS+0x10*i);put(INFOS+0x10*i+0xc,0 if (i==index and null) else HELD+0x20*i);put(HELD+0x20*i,VT);put(HELD+0x20*i+4,count if i==index else 7)
   c=run(candidate,UNLOAD,[],setup);assert c==run(native,UNLOAD,[],setup)
   eax,trace,mem=c;release=null or count<=2;g=lambda a:struct.unpack_from('<I',mem,a-DEF)[0]
   assert eax&0xff==int(release)
   if release:
    assert g(INFOS+0x10*index+0xc)==0
    if not null:assert g(HELD+0x20*index+4)==(count-1)&0xffffffff and trace==([('delete',HELD+0x20*index)] if count==1 else [])
   else:assert g(INFOS+0x10*index+0xc)==HELD+0x20*index and g(HELD+0x20*index+4)==count and not trace
   for i in range(slots):
    if i!=index:assert g(INFOS+0x10*i+0xc)==HELD+0x20*i and g(HELD+0x20*i+4)==7
   cases+=1
 return cases
