import struct,itertools
from unicorn import Uc,UC_ARCH_X86,UC_MODE_32,UC_HOOK_CODE
from unicorn.x86_const import *
THING=0x2000000;ENT=THING+0x200;COMBAT=THING+0x300;DEF=THING+0x500;OBJECTS=(0,THING+0x800,THING+0x900);MANAGER=THING+0x1000;DMGR=THING+0x1100;OUT=THING+0x1300;NAME_DATA=THING+0x1400;VT=THING+0x1600;DELETE=THING+0x1700
STACK=0x3000000;STOP=0x4000000;SINGLETON=0x13b8a1c
CTOR=0x99ebf0;DTOR=0x99eae0;INDEX=0x669640;NAMED=0x669770
def execute(codes,has_component,component,variant,index,lookup,count1,count2):
 u=Uc(UC_ARCH_X86,UC_MODE_32)
 for a,n in [(0x430000,0x7000),(0x4c7000,0x1000),(0x660000,0xa000),(0x99e000,0x2000),(0x125a000,0x1000),(0x13b8000,0x1000),(THING,0x4000),(STACK,0x10000),(STOP,0x1000)]:u.mem_map(a,n)
 def put(a,x):u.mem_write(a,struct.pack('<I',x&0xffffffff))
 def get(a):return struct.unpack('<I',u.mem_read(a,4))[0]
 for a,b in codes.items():u.mem_write(a,bytes(b))
 for a in (CTOR,DTOR,INDEX,NAMED,DELETE):u.mem_write(a,b'\xc3')
 u.mem_write(0x125aba0,b'COMBAT_TYPE_NULL\x00');u.mem_write(THING,b'\x5a'*0x4000)
 # Reset only the modeled fields; sentinel-filled neighbours expose stray writes.
 put(THING+0x24,0x10000 if has_component else 0);put(THING+0x44,ENT);put(THING+0x48,ENT+variant*8);put(THING+0x4c,ENT+0x40)
 keys=((48,),(17,48),(17,48,54))[variant-1] if variant else ()
 for i,k in enumerate(keys):put(ENT+i*8,k);put(ENT+i*8+4,COMBAT)
 put(COMBAT+0x28,OBJECTS[component]);put(THING+0xe0,DEF);put(DEF+0x124,index)
 put(SINGLETON,MANAGER);put(MANAGER+0x20,DMGR);put(VT+4,DELETE)
 for obj,count in zip(OBJECTS[1:],(count1,count2)):put(obj,VT);put(obj+4,count)
 put(OUT,0xabababab)
 before=bytes(u.mem_read(THING,0x1800));trace=[];name_objects=[]
 def hook(uc,a,n,user):
  if a not in (CTOR,DTOR,INDEX,NAMED,DELETE):return
  sp=uc.reg_read(UC_X86_REG_ESP);this=uc.reg_read(UC_X86_REG_ECX);cleanup=4;eax=0xbad0bad0
  if a==CTOR:
   assert get(sp+4)==0x125aba0 and get(sp+8)==0xffffffff
   assert bytes(uc.mem_read(get(sp+4),17))==b'COMBAT_TYPE_NULL\x00'
   put(this,NAME_DATA);name_objects.append(this);trace.append(('ctor',this));cleanup=12;eax=this
  elif a==DTOR:
   assert this==name_objects[-1] and get(this)==NAME_DATA
   put(this,0);trace.append(('dtor',this));cleanup=4
  elif a in (INDEX,NAMED):
   assert this==DMGR
   arg=get(sp+4);result=get(sp+8)
   if a==INDEX:assert arg==index&0xffffffff
   else:assert arg==name_objects[-1] and get(arg)==NAME_DATA
   trace.append(('index' if a==INDEX else 'name',arg))
   target=OBJECTS[lookup] if lookup and (a==NAMED or index>0) else 0
   if target:put(result,target);put(target+4,get(target+4)+1)
   eax=result;cleanup=12
  else:
   trace.append(('delete',this,get(this+4),get(OUT)))
  uc.reg_write(UC_X86_REG_EIP,get(sp));uc.reg_write(UC_X86_REG_ESP,sp+cleanup)
  uc.reg_write(UC_X86_REG_EAX,eax);uc.reg_write(UC_X86_REG_ECX,0xbad1bad1);uc.reg_write(UC_X86_REG_EDX,0xbad2bad2)
 u.hook_add(UC_HOOK_CODE,hook);sp=STACK+0x8000;put(sp,STOP);put(sp+4,OUT)
 u.reg_write(UC_X86_REG_ESP,sp);u.reg_write(UC_X86_REG_ECX,THING)
 regs=(UC_X86_REG_EBX,UC_X86_REG_ESI,UC_X86_REG_EDI,UC_X86_REG_EBP)
 for i,r in enumerate(regs):u.reg_write(r,0x123400+i)
 u.emu_start(0x664d40,STOP,count=2000)
 assert u.reg_read(UC_X86_REG_EIP)==STOP and u.reg_read(UC_X86_REG_ESP)==sp+8 and u.reg_read(UC_X86_REG_EAX)==OUT
 assert all(u.reg_read(r)==0x123400+i for i,r in enumerate(regs))
 from_component=bool(has_component and component)
 expected=OBJECTS[component if from_component else lookup if lookup and (index==0 or index>0) else 0]
 assert get(OUT)==expected,(has_component,component,variant,index,lookup,count1,count2,hex(get(OUT)),hex(expected))
 calls=[t[0] for t in trace if t[0] in ('index','name')]
 assert calls==([] if from_component else ['name' if index==0 else 'index'])
 assert sum(t[0]=='ctor' for t in trace)==int(not from_component and index==0)
 assert sum(t[0]=='dtor' for t in trace)==int(not from_component and index==0)
 assert bytes(u.mem_read(THING,0x800))==before[:0x800]
 assert bytes(u.mem_read(THING+0xa00,0x300))==before[0xa00:0xd00]
 for obj in OBJECTS[1:]:assert bytes(u.mem_read(obj+8,0x20))==before[obj-THING+8:obj-THING+0x28]
 return get(OUT),tuple(get(p+4) for p in OBJECTS[1:]),trace
def compare(candidate,native):
 cases=0
 for has_component,component,variant,index,lookup,count1,count2 in itertools.product(range(2),range(3),range(1,4),(-2,0,3),range(3),(0,1,2,0xffffffff),(0,1,2,0xffffffff)):
  assert execute(candidate,has_component,component,variant,index,lookup,count1,count2)==execute(native,has_component,component,variant,index,lookup,count1,count2)
  cases+=1
 return cases
