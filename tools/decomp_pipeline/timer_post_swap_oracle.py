"""Timer post-swap comparison with list, COM, sleep and clock boundaries."""
import struct
from unicorn import Uc,UC_ARCH_X86,UC_MODE_32,UC_HOOK_CODE
from unicorn.x86_const import *
ENTRY=0x9f73c0
SIZE=0x9f7dc0;CONSTRUCT=0x41c780;DECREMENT=0x41c7b0;POP=0x9f7c50;BACK=0x9f7ec0;FRONT=0x9f7e40;CLEAR=0x9f7250;CLOCK=0x9e1bc0
OBJECT=0x2000000;PENDING=OBJECT+0x1000;FREE=OBJECT+0x1100;QUERY=OBJECT+0x4000;VTABLE=OBJECT+0x4400;DEVICE=OBJECT+0x4500;DEVICE_TABLE=OBJECT+0x4600;TIME_VALUE=OBJECT+0x4900
STACK=0x3000000;CALLBACK=0x2200000;SLEEP=CALLBACK;GETDATA=CALLBACK+16;ISSUE=CALLBACK+32;CREATE=CALLBACK+48;STOP=CALLBACK+0x100
GLOBALS=0x13cb4b0;INDEX=0x13cb4f4

def execute(code,clock_code,pending_count,free_count,mode,create_result,issue_result,start_index,mutation):
 u=Uc(UC_ARCH_X86,UC_MODE_32)
 for a,n in [(0x9f7000,4096),(0x41c000,4096),(0x9e1000,4096),(0x143f000,4096),(0x13cb000,4096),(OBJECT,0x5000),(STACK,0x10000),(CALLBACK,4096)]:u.mem_map(a,n)
 def put(a,v):u.mem_write(a,struct.pack('<I',v&0xffffffff))
 def get(a):return int.from_bytes(u.mem_read(a,4),'little')
 def double(a,v):u.mem_write(a,struct.pack('<d',v))
 u.mem_write(ENTRY,bytes(code));u.mem_write(CLOCK,bytes(clock_code));u.mem_write(STACK,b'\xcc'*0x10000)
 put(OBJECT,DEVICE);put(OBJECT+4,FREE);put(OBJECT+8,PENDING);put(OBJECT+12,10);put(OBJECT+16,77)
 for head in [PENDING,FREE]:put(head,head);put(head+4,head)
 put(VTABLE+24,ISSUE);put(VTABLE+28,GETDATA);put(DEVICE,DEVICE_TABLE);put(DEVICE_TABLE+0x1d8,CREATE);put(0x143fe1c,SLEEP)
 for i in range(32):put(QUERY+i*8,VTABLE);put(QUERY+i*8+4,i)
 for i in range(2):double(GLOBALS+i*24,100.125+i*100);double(GLOBALS+i*24+8,-3.5-i);put(GLOBALS+i*24+16,100+i*100);put(GLOBALS+i*24+20,0x44+i)
 double(0x13cb4e0,30.75);double(0x13cb4e8,-7.0);put(0x13cb4f0,23);put(INDEX,start_index)
 serial=0
 def insert(head,payload,front=False):
  nonlocal serial
  node=OBJECT+0x2000+serial*24;serial+=1;assert serial<32
  following=get(head) if front else head;previous=get(following+4);put(node,following);put(node+4,previous);u.mem_write(node+8,payload);put(previous,node);put(following+4,node)
 def payload(id):return struct.pack('<IId',QUERY+id*8,100+id,id+0.25)
 def normalized(raw):
  id=(int.from_bytes(raw[:4],'little')-QUERY)//8
  # Frame/TimeStamp of newly created records are uninitialized in retail.
  return raw[:4] if id==30 else raw
 def records(head):
  result=[];node=get(head);previous=head
  while node!=head:
   assert get(node+4)==previous;result.append(normalized(bytes(u.mem_read(node+8,16))));previous=node;node=get(node);assert len(result)<32
  assert get(head+4)==previous;return result
 def pop(head):
  node=get(head+4);assert node!=head;previous=get(node+4);following=get(node);put(previous,following);put(following+4,previous);u.mem_write(node,b'\xdd'*24)
 for i in range(pending_count):insert(PENDING,payload(i+1))
 for i in range(free_count):insert(FREE,payload(i+11))
 polls=[0]*32;events=[];times=0
 def hook(u,a,size,data):
  nonlocal times
  if a==CLOCK:
   if times==0 and mutation&1:put(INDEX,get(INDEX)^1)
   if times==1 and mutation&2:put(0x13cb4f0,70);double(0x13cb4e0,99.0)
   value=10.25 if times==0 else 20.5;times+=1;double(TIME_VALUE,value);events.append(('clock',value));return
  if a not in [SIZE,CONSTRUCT,DECREMENT,POP,BACK,FRONT,CLEAR,SLEEP,GETDATA,ISSUE,CREATE]:return
  sp=u.reg_read(UC_X86_REG_ESP);this=u.reg_read(UC_X86_REG_ECX);argc=0;value=0
  if a==SIZE:
   assert this in [OBJECT+4,OBJECT+8];head=get(this);value=len(records(head));events.append(('size',head,value))
  elif a==CONSTRUCT:
   argc=1;head=get(sp+4);assert head in [PENDING,FREE];put(this,head);value=this;events.append(('construct',head))
  elif a==DECREMENT:
   node=get(get(this)+4);put(this,node);value=this;events.append(('decrement',node))
  elif a==POP:
   head=get(this);assert this in [OBJECT+4,OBJECT+8];events.append(('pop',head));pop(head)
  elif a in [BACK,FRONT]:
   argc=1;assert this in [OBJECT+4,OBJECT+8];head=get(this);raw=bytes(u.mem_read(get(sp+4),16));events.append(('front' if a==FRONT else 'back',head,normalized(raw)));insert(head,raw,a==FRONT)
  elif a==CLEAR:
   assert this==OBJECT;events.append(('clear',))
   for head in [PENDING,FREE]:
    while get(head)!=head:pop(head)
  elif a==SLEEP:
   argc=1;assert get(sp+4)==0;events.append(('sleep',))
  elif a==GETDATA:
   argc=4;query=get(sp+4);id=get(query+4);assert 1<=id<=pending_count and get(get(PENDING+4)+8)==query
   assert [get(sp+8),get(sp+12),get(sp+16)]==[0,0,1]
   attempt=polls[id];polls[id]+=1;value=0x80004005 if mode==3 else int(attempt<mode);events.append(('poll',id,value))
  elif a==ISSUE:
   argc=2;assert get(sp+8)==1;id=get(get(sp+4)+4);value=0x80004005 if issue_result else 0;events.append(('issue',id))
  else:
   argc=3;assert get(sp+4)==DEVICE and get(sp+8)==8;put(get(sp+12),QUERY+30*8);value=[0,1,0x80004005][create_result];events.append(('create',value))
  ret=get(sp);u.reg_write(UC_X86_REG_EAX,value);u.reg_write(UC_X86_REG_ECX,0xdead0001);u.reg_write(UC_X86_REG_EDX,0xdead0002);u.reg_write(UC_X86_REG_ESP,sp+4+argc*4);u.reg_write(UC_X86_REG_EIP,ret)
 u.hook_add(UC_HOOK_CODE,hook);sp=STACK+0x8000;put(sp,STOP);u.reg_write(UC_X86_REG_ESP,sp);u.reg_write(UC_X86_REG_ECX,OBJECT);u.reg_write(UC_X86_REG_FPCW,0x37f)
 preserved=[UC_X86_REG_EBX,UC_X86_REG_EBP,UC_X86_REG_ESI,UC_X86_REG_EDI]
 for i,r in enumerate(preserved):u.reg_write(r,0xabcd0000+i)
 u.emu_start(ENTRY,STOP,count=10000)
 assert u.reg_read(UC_X86_REG_EIP)==STOP and u.reg_read(UC_X86_REG_ESP)==sp+4 and times==2
 for i,r in enumerate(preserved):assert u.reg_read(r)==0xabcd0000+i
 return events,bytes(u.mem_read(OBJECT,20)),records(PENDING),records(FREE),bytes(u.mem_read(GLOBALS,72))
