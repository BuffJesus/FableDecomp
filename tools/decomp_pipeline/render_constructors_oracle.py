"""Native renderer construction/destruction with callback-visible object bytes."""
import struct
from unicorn import Uc,UC_ARCH_X86,UC_MODE_32,UC_HOOK_CODE
from unicorn.x86_const import *
STATE=0xa05970;DEVICE=0xa079b0;CORE=0xa0afe0;DESTROY=0xa0b060;STATE_DESTROY=0xa044b0
OBJECT=0x2000000;DISPLAY=OBJECT+0x8000;STACK=0x3000000;STOP=0x2200000;CURRENT=0x13cb508
TARGETS={'??0CBaseClassNonCopyable@@QAE@XZ':0x99a2f0,'??_7CRenderStateManager@@6B@':0x129c978,
'??0CDeviceState@CRenderStateManager@@QAE@XZ':DEVICE,'??0CRenderStateManager@@QAE@K@Z':STATE,
'??_7CRenderManagerCore@@6B@':0x129c9a4,'??0CInitBaseClass@@QAE@XZ':0x99a310,
'??_7CSystemFont@@6B@':0x129c998,'?IsValid@CInitBaseClass@@QBE_NXZ':0x99a330,
'?Uninitialise@CSystemFont@@QAEXXZ':0xa6c120,'??1CInitBaseClass@@UAE@XZ':0x99a410,
'??1CRenderStateManager@@UAE@XZ':STATE_DESTROY}
def execute(codes,state_destructor,entry,fill,flags,mutation,valid):
 u=Uc(UC_ARCH_X86,UC_MODE_32)
 for a,n in [(0x990000,0xe0000),(0x13cb000,0x1000),(OBJECT,0x10000),(STACK,0x10000),(STOP,0x1000)]:u.mem_map(a,n)
 def put(a,v):u.mem_write(a,struct.pack('<I',v&0xffffffff))
 def get(a):return int.from_bytes(u.mem_read(a,4),'little')
 def byte(a,v):u.mem_write(a,bytes([v]))
 for a,code in codes.items():u.mem_write(a,bytes(code))
 u.mem_write(STATE_DESTROY,bytes(state_destructor));u.mem_write(OBJECT,bytes([fill])*0x3d20);put(CURRENT,OBJECT+0x9000)
 if entry==DESTROY:byte(OBJECT+0x3a50,valid)
 events=[]
 def hook(u,a,size,data):
  if a in [STATE,DEVICE,STATE_DESTROY]:events.append(('enter',a,u.reg_read(UC_X86_REG_ECX)));return
  if a not in [0x99a2f0,0x99a310,0x99a330,0xa6c120,0x99a410,0x99a300]:return
  sp=u.reg_read(UC_X86_REG_ESP);this=u.reg_read(UC_X86_REG_ECX);value=this
  if a==0x99a2f0:
   assert this==OBJECT;events.append(('base_ctor',));put(this,0x129a7c4)
   if mutation==1:put(OBJECT+4,0x12345678);put(OBJECT+0x2824,0x10203040);byte(OBJECT+0x3a36,0x81)
  elif a==0x99a310:
   assert this==OBJECT+0x3a4c and get(OBJECT+0x3a3c)==DISPLAY and get(OBJECT+0x3a48)==0xffffffff;events.append(('font_base_ctor',));put(this,0x129a7cc);byte(this+4,0)
   if mutation==2:put(OBJECT+0x3a3c,DISPLAY+4);put(OBJECT+0x3d00,0x444400)
  elif a==0x99a330:
   assert this==OBJECT+0x3a4c;value=u.mem_read(this+4,1)[0];events.append(('valid',value))
   if mutation==1:byte(this+4,0)
  elif a==0xa6c120:
   assert this==OBJECT+0x3a4c;events.append(('font_uninit',))
   if mutation==2:put(OBJECT+0x3cf0,0x334400)
  elif a==0x99a410:assert this==OBJECT+0x3a4c;events.append(('font_base_dtor',));put(this,0x1231710)
  elif a==0x99a300:assert this==OBJECT and get(CURRENT)==0 and get(OBJECT+4)==0;events.append(('base_dtor',));put(this,0x1231710)
  u.reg_write(UC_X86_REG_EAX,value);u.reg_write(UC_X86_REG_ESP,sp+4);u.reg_write(UC_X86_REG_EIP,get(sp))
 u.hook_add(UC_HOOK_CODE,hook);sp=STACK+0x8000;put(sp,STOP);u.reg_write(UC_X86_REG_ESP,sp);u.reg_write(UC_X86_REG_ECX,OBJECT)
 argc=0
 if entry==STATE:put(sp+4,flags);argc=1
 elif entry==CORE:put(sp+4,DISPLAY);put(sp+8,flags);argc=2
 u.emu_start(entry,STOP,count=20000);assert u.reg_read(UC_X86_REG_EIP)==STOP and u.reg_read(UC_X86_REG_ESP)==sp+4+argc*4
 if entry!=DESTROY:assert u.reg_read(UC_X86_REG_EAX)==OBJECT
 return events,bytes(u.mem_read(OBJECT,0x3d20)),get(CURRENT)
