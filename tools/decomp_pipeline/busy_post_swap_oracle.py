"""Execute native/candidate busy-query code with only allocation and COM hooks."""
import struct
from unicorn import Uc,UC_ARCH_X86,UC_MODE_32,UC_HOOK_CODE
from unicorn.x86_const import *
ENTRY=0xa618d0;MALLOC=0xbfea0e;FREE_CALL=0xbfea14
OBJECT=0x2000000;PENDING=OBJECT+0x1000;FREE=OBJECT+0x1100;QUERY=OBJECT+0x4000;VTABLE=OBJECT+0x4400;DEVICE=OBJECT+0x4500;DEVICE_TABLE=OBJECT+0x4600
STACK=0x3000000;CALLBACK=0x2200000;GETDATA=CALLBACK;ISSUE=CALLBACK+16;CREATE=CALLBACK+32;STOP=CALLBACK+0x100

def execute(code,pending_count,free_count,mode,create_result,issue_result,mutation):
 u=Uc(UC_ARCH_X86,UC_MODE_32)
 for a,n in [(0xa61000,4096),(0xbfe000,4096),(OBJECT,0x8000),(STACK,0x10000),(CALLBACK,4096)]:u.mem_map(a,n)
 def put(a,v):u.mem_write(a,struct.pack('<I',v&0xffffffff))
 def get(a):return int.from_bytes(u.mem_read(a,4),'little')
 u.mem_write(ENTRY,bytes(code));u.mem_write(STACK,b'\xcc'*0x10000)
 put(OBJECT,FREE);put(OBJECT+4,PENDING);put(OBJECT+8,10);put(OBJECT+12,77);put(OBJECT+16,DEVICE)
 for h in [FREE,PENDING]:put(h,h);put(h+4,h)
 put(VTABLE+28,GETDATA);put(VTABLE+24,ISSUE);put(DEVICE,DEVICE_TABLE);put(DEVICE_TABLE+0x1d8,CREATE)
 for i in range(40):put(QUERY+i*8,VTABLE);put(QUERY+i*8+4,i)
 serial=0;allocated=set();events=[]
 def alloc():
  nonlocal serial
  a=OBJECT+0x2000+serial*16;serial+=1;assert serial<256;allocated.add(a);u.mem_write(a,b'\xa5'*16);return a
 def add(h,id):
  a=alloc();previous=get(h+4);put(a,h);put(a+4,previous);put(a+8,QUERY+id*8);put(a+12,100+id);put(previous,a);put(h+4,a)
 for i in range(pending_count):add(PENDING,i+1)
 for i in range(free_count):add(FREE,i+20)
 def records(h):
  result=[];a=get(h);previous=h
  while a!=h:
   assert a in allocated and get(a+4)==previous
   id=(get(a+8)-QUERY)//8;assert 0<=id<40
   result.append((id,None if id==39 else get(a+12)));previous=a;a=get(a);assert len(result)<40
  assert get(h+4)==previous;return result
 polls=0
 def hook(u,a,size,data):
  nonlocal polls
  if a not in [MALLOC,FREE_CALL,GETDATA,ISSUE,CREATE]:return
  sp=u.reg_read(UC_X86_REG_ESP);argc=0;value=0
  if a==MALLOC:assert get(sp+4)==16;value=alloc();events.append(('allocate',))
  elif a==FREE_CALL:
   node=get(sp+4);assert node in allocated;allocated.remove(node);u.mem_write(node,b'\xdd'*16);events.append(('free',))
  elif a==GETDATA:
   argc=4;id=(get(sp+4)-QUERY)//8;assert get(sp+8)==0 and get(sp+12)==0 and get(sp+16)==1
   value=0 if mode==0 or (mode==3 and polls%2==0) else 1 if mode in (1,3) else 0x80004005;polls+=1
   if mutation&1:put(OBJECT+8,40+polls)
   events.append(('poll',id,value))
  elif a==CREATE:
   argc=3;assert get(sp+4)==DEVICE and get(sp+8)==8;put(get(sp+12),QUERY+39*8);value=create_result
   if mutation&2:put(OBJECT+8,99)
   events.append(('create',value))
  elif a==ISSUE:
   argc=2;id=(get(sp+4)-QUERY)//8;assert get(sp+8)==1;value=issue_result
   if mutation&4:put(OBJECT+8,201)
   events.append(('issue',id,value))
  u.reg_write(UC_X86_REG_EAX,value&0xffffffff);u.reg_write(UC_X86_REG_ESP,sp+4+argc*4);u.reg_write(UC_X86_REG_EIP,get(sp))
 u.hook_add(UC_HOOK_CODE,hook);sp=STACK+0x8000;put(sp,STOP);u.reg_write(UC_X86_REG_ESP,sp);u.reg_write(UC_X86_REG_ECX,OBJECT)
 u.emu_start(ENTRY,STOP,count=100000)
 assert u.reg_read(UC_X86_REG_EIP)==STOP and u.reg_read(UC_X86_REG_ESP)==sp+4
 result=(records(FREE),records(PENDING),get(OBJECT+8),get(OBJECT+12),get(OBJECT+16),events)
 assert len(allocated)==len(result[0])+len(result[1])
 return result
