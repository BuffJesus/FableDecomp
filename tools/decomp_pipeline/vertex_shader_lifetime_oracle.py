"""Native vertex-shader ownership, including compiler deleting destructors."""
import struct
from unicorn import Uc,UC_ARCH_X86,UC_MODE_32,UC_HOOK_CODE
from unicorn.x86_const import *
CTOR=0xa62fb0;COUNT=0x9be210;DATA_CTOR=0xa62fc0;DATA_DTOR=0xa62ff0;SCALAR=0x9c1ae0;DATA_SCALAR=0xa630d0
OBJECT=0x2000000;DATA=OBJECT+0x100;SHADER=OBJECT+0x200;DECL=OBJECT+0x300;OTHER=OBJECT+0x400
STACK=0x3000000;STOP=0x2200000;SHADER_RELEASE=STOP+0x100;DECL_RELEASE=STOP+0x200
TARGETS={'??_7CVertexShader@@6B@':0x129b3d0,'??_7CShaderBase@@6B@':0x129a030,
'??_7CVertexShaderData@@6B@':0x129da50,'??0CBaseClassNonCopyable@@QAE@XZ':0x99a2f0,
'??1CBaseClassNonCopyable@@UAE@XZ':0x99a300,'??3@YAXPAX@Z':0xbfe9bc}
def execute(codes,entry,fill,mask,refs,flags,mutation,attached):
 u=Uc(UC_ARCH_X86,UC_MODE_32)
 for a,n in [(0x990000,0xe0000),(0xbfe000,0x1000),(0x129a000,0x4000),(OBJECT,0x10000),(STACK,0x10000),(STOP,0x1000)]:u.mem_map(a,n)
 def put(a,v):u.mem_write(a,struct.pack('<I',v&0xffffffff))
 def get(a):return int.from_bytes(u.mem_read(a,4),'little')
 for a,code in codes.items():u.mem_write(a,bytes(code))
 u.mem_write(OBJECT,bytes([fill])*0x1000);put(OBJECT,0x129b3d0);put(OBJECT+4,DATA if attached else 0)
 put(DATA,0x129da50);put(DATA+4,DECL if mask&1 else 0);put(DATA+8,SHADER if mask&2 else 0);put(DATA+12,0x12345678);put(DATA+16,refs)
 put(0x129b3d0,SCALAR);put(0x129da50,DATA_SCALAR)
 put(SHADER,OBJECT+0x600);put(OBJECT+0x608,SHADER_RELEASE)
 for d in (DECL,OTHER):put(d,OBJECT+0x700)
 put(OBJECT+0x708,DECL_RELEASE);events=[]
 def hook(u,a,size,data):
  if a not in [0x99a2f0,0x99a300,0xbfe9bc,SHADER_RELEASE,DECL_RELEASE]:return
  sp=u.reg_read(UC_X86_REG_ESP);this=u.reg_read(UC_X86_REG_ECX);argc=0;value=this
  if a==0x99a2f0:assert this==DATA;events.append(('base_ctor',));put(this,0x129a7c4)
  elif a==0x99a300:assert this==DATA;events.append(('base_dtor',));put(this,0x1231710)
  elif a==0xbfe9bc:assert get(sp+4) in (OBJECT,DATA);events.append(('delete',get(sp+4)));value=0
  elif a==SHADER_RELEASE:
   argc=1;assert get(sp+4)==SHADER;events.append(('shader',));value=123
   if mutation==1:put(DATA+4,OTHER)
   elif mutation==2:put(DATA+4,0)
  elif a==DECL_RELEASE:argc=1;assert get(sp+4) in (DECL,OTHER);events.append(('declaration',get(sp+4)));value=456
  u.reg_write(UC_X86_REG_EAX,value);u.reg_write(UC_X86_REG_ESP,sp+4+argc*4);u.reg_write(UC_X86_REG_EIP,get(sp))
 u.hook_add(UC_HOOK_CODE,hook);sp=STACK+0x8000;put(sp,STOP);argc=0;this=DATA if entry in (DATA_CTOR,DATA_DTOR,DATA_SCALAR) else OBJECT
 if entry==DATA_CTOR:
  for i,v in enumerate([DECL if mask&1 else 0,SHADER if mask&2 else 0,0x87654321,OBJECT+0x800,OBJECT+0x900]):put(sp+4+i*4,v)
  argc=5
 elif entry in (SCALAR,DATA_SCALAR):put(sp+4,flags);argc=1
 u.reg_write(UC_X86_REG_ECX,this);u.reg_write(UC_X86_REG_ESP,sp);u.emu_start(entry,STOP,count=1000)
 assert u.reg_read(UC_X86_REG_EIP)==STOP and u.reg_read(UC_X86_REG_ESP)==sp+4+argc*4
 if entry==COUNT:assert u.reg_read(UC_X86_REG_EAX)==refs
 elif entry!=DATA_DTOR:assert u.reg_read(UC_X86_REG_EAX)==this
 return events,bytes(u.mem_read(OBJECT,8)),bytes(u.mem_read(DATA,20))

def scenarios():
 for fill in range(0,256,51):
  yield (CTOR,fill,0,1,0,0,0)
  for refs in [0,1,2,17,0xffffffff]:yield (COUNT,fill,0,refs,0,0,1)
  for mask in range(4):
   yield (DATA_CTOR,fill,mask,1,0,0,0)
   for mutation in range(3):
    yield (DATA_DTOR,fill,mask,1,0,mutation,0)
    for flags in range(4):
     yield (DATA_SCALAR,fill,mask,1,flags,mutation,0)
     for refs in [0,1,2,17,0xffffffff]:
      for attached in range(2):yield (SCALAR,fill,mask,refs,flags,mutation,attached)
