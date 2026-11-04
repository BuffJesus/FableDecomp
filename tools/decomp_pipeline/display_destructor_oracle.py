"""Display destruction with live callback mutations and real list-node cleanup."""
import struct
from unicorn import Uc,UC_ARCH_X86,UC_MODE_32,UC_HOOK_CODE
from unicorn.x86_const import *
ENTRY=0x9c01c0;LIST=0x9c2220;OBJECT=0x2000000;STACK=0x3000000;STOP=0x2200000
RENDER=OBJECT+0x1000;OTHER=OBJECT+0x1100;DEVICE=OBJECT+0x1200;TIMER=OBJECT+0x1300;HEAD=OBJECT+0x2000
DELETE_RENDER=STOP+0x100;RELEASE=STOP+0x200
LIST_SYMBOL='??1?$list@PAVCDeviceResetCallback@@V?$allocator@PAVCDeviceResetCallback@@@_STL@@@_STL@@QAE@XZ'
TARGETS={'??_7CDisplayManager@@6B@':0x129b3f8,'?FableShaderRenderManager_013bc470@@3VCShaderRenderManager@@A':0x13bc470,
'?Uninitialise@CShaderRenderManager@@QAEXXZ':0x98a7a0,'?Uninitialise@CSurface@@QAEXXZ':0x9f2e20,
'??1CBusyResourceManager@@QAE@XZ':0xa61b10,'_free':0xbfea14,'??1CVsyncCallbackTimer@@QAE@XZ':0x9f7330,
'??3@YAXPAX@Z':0xbfe9bc,LIST_SYMBOL:LIST,'??1CWideString@@QAE@XZ':0x99b510,
'?Uninitialise@CTexture@@QAEXXZ':0x9f9f70,'??_7CSurface@@6B@':0x122f84c,'??1CBaseClassNonCopyable@@UAE@XZ':0x99a300}
def execute(code,list_code,mask,count,mutation):
 u=Uc(UC_ARCH_X86,UC_MODE_32)
 for a,n in [(0x980000,0x290000),(OBJECT,0x10000),(STACK,0x10000),(STOP,0x1000)]:u.mem_map(a,n)
 def put(a,v):u.mem_write(a,struct.pack('<I',v&0xffffffff))
 def get(a):return int.from_bytes(u.mem_read(a,4),'little')
 u.mem_write(ENTRY,bytes(code));u.mem_write(LIST,bytes(list_code));u.mem_write(OBJECT,b'\xa5'*0x10000)
 u.mem_write(OBJECT+0x1b1,bytes([mask&1]));put(OBJECT+8,RENDER if mask&2 else 0);put(OBJECT+0x58,DEVICE if mask&4 else 0)
 put(OBJECT+0x294,TIMER if mask&8 else 0);put(OBJECT+0x298,OBJECT+0x1500 if mask&16 else 0);put(OBJECT+0x2a4,OBJECT+0x1600 if mask&32 else 0)
 for r in (RENDER,OTHER):put(r,OBJECT+0x1700)
 put(OBJECT+0x1700,DELETE_RENDER);put(DEVICE,OBJECT+0x1800);put(OBJECT+0x1808,RELEASE)
 nodes=[HEAD]+[HEAD+0x20*(i+1) for i in range(count)];put(OBJECT+0x264,HEAD)
 for i,node in enumerate(nodes):put(node,nodes[(i+1)%len(nodes)]);put(node+4,nodes[(i-1)%len(nodes)]);put(node+8,0xdead0000+i)
 events=[];surfaces=[];frees=[]
 def hook(u,a,size,data):
  if a==LIST:events.append(('list',));return
  if a not in set(TARGETS.values())|{DELETE_RENDER,RELEASE}:return
  sp=u.reg_read(UC_X86_REG_ESP);this=u.reg_read(UC_X86_REG_ECX);argc=0;value=0
  if a==0x98a7a0:
   assert this==0x13bc470;events.append(('shader',))
   if mutation==1:put(OBJECT+8,OTHER)
  elif a==DELETE_RENDER:
   argc=1;assert get(sp+4)==1;events.append(('renderer',this,get(OBJECT+8)))
   if mutation==2 and get(OBJECT+8)==0:put(OBJECT+8,OTHER)
  elif a==0x9f2e20:
   offset=this-OBJECT;surfaces.append(offset);events.append(('surface',offset,get(this)))
   if mutation==3 and offset==0x3c:put(OBJECT+8,OTHER)
   if mutation==4 and offset==0x2c:put(OBJECT+0x58,DEVICE)
   put(this+4,0)
  elif a==RELEASE:argc=1;assert get(sp+4)==DEVICE;events.append(('release',))
  elif a==0xa61b10:
   assert this==OBJECT+0x2b0;events.append(('busy',))
   if mutation==5:put(OBJECT+0x294,TIMER)
  elif a==0xbfea14:
   pointer=get(sp+4);assert pointer not in frees;frees.append(pointer);events.append(('free',pointer))
  elif a==0x9f7330:assert this==TIMER;events.append(('timer',))
  elif a==0xbfe9bc:assert get(sp+4)==TIMER;events.append(('delete_timer',))
  elif a==0x99b510:assert this==OBJECT+0x258;events.append(('wide',))
  elif a==0x9f9f70:assert this==OBJECT+0x4c;events.append(('texture',))
  elif a==0x99a300:assert this==OBJECT;events.append(('base',));put(this,0x129a7c4)
  else:raise AssertionError(hex(a))
  u.reg_write(UC_X86_REG_EAX,value);u.reg_write(UC_X86_REG_ESP,sp+4+argc*4);u.reg_write(UC_X86_REG_EIP,get(sp))
 u.hook_add(UC_HOOK_CODE,hook);sp=STACK+0x8000;put(sp,STOP);u.reg_write(UC_X86_REG_ESP,sp);u.reg_write(UC_X86_REG_ECX,OBJECT)
 u.emu_start(ENTRY,STOP,count=10000);assert u.reg_read(UC_X86_REG_EIP)==STOP and u.reg_read(UC_X86_REG_ESP)==sp+4
 assert surfaces==[0x2c,0x3c,0x3c,0x2c,0x1c,0xc]
 assert frees[-len(nodes):]==nodes[1:]+nodes[:1]
 return events,bytes(u.mem_read(OBJECT,0x2c8)),bytes(u.mem_read(HEAD,12))
