import struct,itertools
from unicorn import Uc,UC_ARCH_X86,UC_MODE_32,UC_HOOK_CODE
from unicorn.x86_const import *
MEM=0x2000000;DEST=MEM;SOURCE=MEM+0x20;INFO=(0,MEM+0x100,MEM+0x200,MEM+0x300);DATA=(0,MEM+0x500,MEM+0x540,MEM+0x580);CALLBACK=MEM+0x800;STACK=0x3000000;STOP=0x4000000;FREE=0xbfe9bc
SPECS=(0x419108,0x419134,0x4194ba)
def execute(codes,address,old,new,old_data,new_data,c1,c2,mode,alias):
 u=Uc(UC_ARCH_X86,UC_MODE_32);u.mem_map(0x419000,0x1000);u.mem_map(0xbfe000,0x1000);u.mem_map(MEM,0x1000);u.mem_map(STACK,0x10000);u.mem_map(STOP,0x1000)
 def put(a,v):u.mem_write(a,struct.pack('<I',v&0xffffffff))
 def get(a):return struct.unpack('<I',u.mem_read(a,4))[0]
 for a,b in codes.items():u.mem_write(a,bytes(b))
 for a in (FREE,CALLBACK):u.mem_write(a,b'\xc3')
 u.mem_write(MEM,b'\xa5'*0x1000)
 for i,c in ((1,c1),(2,c2),(3,17)):put(INFO[i],c);put(INFO[i]+4,CALLBACK);put(INFO[i]+8,DATA[i])
 put(DEST,DATA[old_data]);put(DEST+4,INFO[old]);put(SOURCE,DATA[new_data]);put(SOURCE+4,INFO[new])
 if alias:put(DEST,DATA[old_data]);put(DEST+4,INFO[old])
 source=DEST if alias else SOURCE;requested_info=INFO[old] if alias and address==0x4194ba else INFO[new]
 requested_data=DATA[old_data] if alias and address==0x4194ba else DATA[new_data]
 expected=bytearray(u.mem_read(MEM,0x900));trace=[]
 def exp_put(a,v):struct.pack_into('<I',expected,a-MEM,v&0xffffffff)
 def exp_get(a):return struct.unpack_from('<I',expected,a-MEM)[0]
 def exp_release():
  if old:
   count=(exp_get(INFO[old])-1)&0xffffffff;exp_put(INFO[old],count)
   if count==0:
    trace.append(('callback',DATA[old],old))
    if mode==1:exp_put(source,DATA[3]);exp_put(source+4,INFO[3])
    if mode==2:exp_put(DEST,DATA[3]);exp_put(DEST+4,INFO[3])
    trace.append(('free',exp_get(DEST+4)))
  exp_put(DEST,0);exp_put(DEST+4,0)
 if address==0x419108:exp_release()
 elif INFO[old]!=requested_info:
  exp_release();exp_put(DEST,requested_data);exp_put(DEST+4,requested_info)
  if requested_info:exp_put(requested_info,exp_get(requested_info)+1)
 observed=[]
 def hook(uc,a,n,user):
  if a not in (CALLBACK,FREE):return
  sp=uc.reg_read(UC_X86_REG_ESP)
  if a==CALLBACK:
   value=uc.reg_read(UC_X86_REG_ECX);observed.append(('callback',value,old))
   if mode==1:put(source,DATA[3]);put(source+4,INFO[3])
   if mode==2:put(DEST,DATA[3]);put(DEST+4,INFO[3])
  else:observed.append(('free',get(sp+4)))
  uc.reg_write(UC_X86_REG_EIP,get(sp));uc.reg_write(UC_X86_REG_ESP,sp+4)
  uc.reg_write(UC_X86_REG_EAX,0xbad0bad0);uc.reg_write(UC_X86_REG_ECX,0xbad1bad1);uc.reg_write(UC_X86_REG_EDX,0xbad2bad2)
 u.hook_add(UC_HOOK_CODE,hook);sp=STACK+0x8000;put(sp,STOP)
 if address==0x419134:put(sp+4,requested_data);put(sp+8,requested_info)
 if address==0x4194ba:put(sp+4,source)
 u.reg_write(UC_X86_REG_ESP,sp);u.reg_write(UC_X86_REG_ECX,DEST)
 regs=(UC_X86_REG_EBX,UC_X86_REG_ESI,UC_X86_REG_EDI,UC_X86_REG_EBP)
 for i,r in enumerate(regs):u.reg_write(r,0x123400+i)
 u.emu_start(address,STOP,count=300)
 cleanup={0x419108:4,0x419134:12,0x4194ba:8}[address]
 assert u.reg_read(UC_X86_REG_EIP)==STOP and u.reg_read(UC_X86_REG_ESP)==sp+cleanup
 assert all(u.reg_read(r)==0x123400+i for i,r in enumerate(regs))
 if address==0x4194ba:assert u.reg_read(UC_X86_REG_EAX)==DEST
 assert observed==trace,(address,old,new,old_data,new_data,c1,c2,mode,alias,observed,trace)
 assert bytes(u.mem_read(MEM,0x900))==bytes(expected),(address,old,new,old_data,new_data,c1,c2,mode,alias)
 return get(DEST),get(DEST+4),tuple(get(x) for x in INFO[1:]),observed
def compare(candidate,native):
 cases=0
 for address in SPECS:
  for old,new,od,nd,c1,c2,mode,alias in itertools.product(range(3),range(3),range(2),range(2),(0,1,2,0xffffffff),(0,1,2,0xffffffff),range(3),range(2)):
   assert execute(candidate,address,old,new,od,nd,c1,c2,mode,alias)==execute(native,address,old,new,od,nd,c1,c2,mode,alias)
   cases+=1
 return cases
