"""Execute CInputManager's scans, device loops and ClearEvents (with the real erase) against retail."""
import itertools,struct
from unicorn import Uc,UC_ARCH_X86,UC_MODE_32,UC_HOOK_CODE
from unicorn.x86_const import *
MANAGER=0x2000000;EVENTS=0x2010000;PROCESSED=0x2020000;SLOTS=0x2030000;DEVICES=0x2040000;TABLES=0x2080000;INFO=0x2090000;OUT=0x2090100;STACK=0x3000000;STOP=0x4000000;STUBS=0x5000000
START,NEXT,END,EXCLUSIVE,NUMBER,CLEAR,ERASE=0x9f4ed0,0x9f4f10,0x9f4ac0,0x9f4e70,0x9f5250,0x9f5540,0x9f67e0
CLEARALL={0x9e42c0:'J',0xa66c00:'M',0xa66860:'K'};GETTYPE,DTOR,LISTCLEAR=0xa03b40,0xa03b30,0x9e4710
EVENT=0x34
USED={START,NEXT,END,EXCLUSIVE,NUMBER,CLEAR,ERASE,*CLEARALL}
def run(codes,entry,args,state,index=0):
 u=Uc(UC_ARCH_X86,UC_MODE_32)
 for a,z in [(0x9e4000,0x1000),(0x9f4000,0x3000),(0xa03000,0x1000),(0xa66000,0x1000),(MANAGER,0x100000),(STACK,0x10000),(STOP,0x1000),(STUBS,0x1000)]:u.mem_map(a,z)
 def put(a,v):u.mem_write(a,struct.pack('<I',v&0xffffffff))
 def get(a):return struct.unpack('<I',u.mem_read(a,4))[0]
 for a,b in codes.items():
  if a in USED:u.mem_write(a,bytes(b))
 for a in (GETTYPE,DTOR,LISTCLEAR):u.mem_write(a,b'\xc3')
 stored,types,devices,processed,storeCount,numbers,nulls=state
 u.mem_write(MANAGER,b'\xa5'*0x80);put(MANAGER+0x1c,stored);put(MANAGER+0x20,7)
 for i in range(8):
  u.mem_write(EVENTS+i*EVENT,bytes([0x10+i])*EVENT);put(EVENTS+i*EVENT+0x28,types[i])
  u.mem_write(PROCESSED+i*EVENT,bytes([0x60+i])*EVENT);put(PROCESSED+i*EVENT+0x28,1)
 put(MANAGER+0x10,EVENTS);put(MANAGER+0x14,EVENTS+storeCount*EVENT);put(MANAGER+0x18,EVENTS+8*EVENT)
 put(MANAGER+4,PROCESSED);put(MANAGER+8,PROCESSED+processed*EVENT);put(MANAGER+0xc,PROCESSED+8*EVENT)
 mouse,keyboard,joysticks=devices
 put(MANAGER+0x24,DEVICES if mouse else 0);put(MANAGER+0x28,DEVICES+0x4000 if keyboard else 0)
 for i in range(joysticks):put(SLOTS+8*i,0 if i in nulls else DEVICES+0x8000+i*0x1000);put(SLOTS+8*i+4,0xc0de0000+i)
 put(MANAGER+0x2c,SLOTS);put(MANAGER+0x30,SLOTS+8*joysticks);put(MANAGER+0x34,SLOTS+8*joysticks)
 # Device vtables: every slot a distinct stub, recorded by slot number.
 for d,(base,n) in enumerate([(DEVICES,18),(DEVICES+0x4000,5)]+[(DEVICES+0x8000+i*0x1000,19) for i in range(4)]):
  put(base,TABLES+d*0x100)
  for k in range(n):put(TABLES+d*0x100+4*k,STUBS+d*0x100+4*k)
  u.mem_write(base+4,b'\x3c'*0x40)
 u.mem_write(STUBS,b'\xc3'*0x1000)
 put(INFO,0);put(INFO+4,index)
 trace=[]
 def hook(uc,a,s,user):
  sp=uc.reg_read(UC_X86_REG_ESP);this=uc.reg_read(UC_X86_REG_ECX);result=0;pop=0
  if a==GETTYPE:result=get(this+0x28)
  elif a==DTOR:trace.append(('dtor',this))
  elif a==LISTCLEAR:trace.append(('listclear',this))
  elif STUBS<=a<STUBS+0x1000:
   d,k=(a-STUBS)//0x100,((a-STUBS)%0x100)//4;trace.append(('slot',d,k,this))
   pops={(0,6):4,(1,2):4,(2,12):4,(2,8):0,(2,15):0};pop=pops.get((min(d,2),k),0)
   if (min(d,2),k)==(2,8):result=numbers[d-2]
  else:return
  uc.reg_write(UC_X86_REG_EAX,result);uc.reg_write(UC_X86_REG_EIP,get(sp));uc.reg_write(UC_X86_REG_ESP,sp+4+pop)
 u.hook_add(UC_HOOK_CODE,hook);sp=STACK+0x8000;put(sp,STOP)
 for i,v in enumerate(args):put(sp+4+4*i,v)
 regs=[UC_X86_REG_EBX,UC_X86_REG_ESI,UC_X86_REG_EDI,UC_X86_REG_EBP]
 for i,r in enumerate(regs):u.reg_write(r,0x778800+i)
 u.reg_write(UC_X86_REG_ECX,MANAGER);u.reg_write(UC_X86_REG_ESP,sp);u.emu_start(entry,STOP,count=20000)
 assert u.reg_read(UC_X86_REG_ESP)==sp+4+4*len(args) and all(u.reg_read(r)==0x778800+i for i,r in enumerate(regs)),hex(entry)
 return u.reg_read(UC_X86_REG_EAX)&0xff,trace,bytes(u.mem_read(MANAGER,0x80)),bytes(u.mem_read(INFO,8)),bytes(u.mem_read(OUT,EVENT)),bytes(u.mem_read(SLOTS,0x40))
def compare(candidate,native):
 cases=0
 for stored,pattern in itertools.product(range(7),range(4)):
  types=[0 if (i and (pattern>>(i%2))&1) else 3 for i in range(8)];state=(stored,types,(0,0,0),0,8,(0,)*4,())
  c=run(candidate,START,[INFO,OUT],state);assert c==run(native,START,[INFO,OUT],state)
  assert c[0]==int(stored>0) and c[3][0]==1 and struct.unpack_from('<I',c[2],0x20)[0]==8
  for index in range(-1,7):
   c=run(candidate,NEXT,[INFO,OUT],state,index);assert c==run(native,NEXT,[INFO,OUT],state,index)
   nxt=index+1
   while nxt<stored and types[nxt]==0:nxt+=1
   got=struct.unpack_from('<i',c[3],4)[0]
   if nxt<stored:assert c[0]==1 and got==nxt and c[4]==bytes([0x10+nxt])*0x28+struct.pack('<I',types[nxt])+bytes([0x10+nxt])*8
   else:assert c[0]==0 and got==index
   cases+=1
  c=run(candidate,END,[INFO],state,3);assert c==run(native,END,[INFO],state,3) and c[3][0]==0 and struct.unpack_from('<I',c[2],0x20)[0]==6
  cases+=1
 for mouse,keyboard,joysticks,nullset,on in itertools.product((0,1),(0,1),range(5),((),(1,),(0,2)),(0,1)):
  nulls=tuple(i for i in nullset if i<joysticks);numbers=(10,11+on,12,13);state=(0,[3]*8,(mouse,keyboard,joysticks),(joysticks+on)%6,(joysticks*2+len(nulls))%6,numbers,nulls)
  c=run(candidate,EXCLUSIVE,[on],state);assert c==run(native,EXCLUSIVE,[on],state)
  want=([('slot',0,6,0x2040000)] if mouse else [])+([('slot',1,2,0x2044000)] if keyboard else [])+[('slot',2+i,12,0x2048000+i*0x1000) for i in range(joysticks) if i not in nulls]
  assert c[0]==1 and c[1]==want,(c[1],want)
  c=run(candidate,CLEAR,[],state);assert c==run(native,CLEAR,[],state)
  manager=c[2];g=lambda o:struct.unpack_from('<I',manager,o)[0]
  assert g(0x4)==g(0x8)==PROCESSED and g(0x10)==g(0x14)==EVENTS and g(0x1c)==0
  dt=[t for t in c[1] if t[0]=='dtor'];assert len(dt)==state[3]+state[4]
  lists=[t[1] for t in c[1] if t[0]=='listclear'];want=[0x2048000+i*0x1000+0xd0c for i in range(joysticks) if i not in nulls]+([0x2040000+0x340c] if mouse else [])+([0x2044000+0x340c] if keyboard else [])
  assert lists==want,(lists,want)
  cases+=2
  if not nulls:
   for query in (9,10,11,12,13,14):
    c=run(candidate,NUMBER,[query],state);assert c==run(native,NUMBER,[query],state);assert c[0]==int(query in numbers[:joysticks]);cases+=1
 return cases
