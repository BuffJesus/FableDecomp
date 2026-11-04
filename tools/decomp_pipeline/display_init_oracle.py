"""Execute native/candidate Init with observable external ownership boundaries."""
import struct
from unicorn import Uc,UC_ARCH_X86,UC_MODE_32,UC_HOOK_CODE
from unicorn.x86_const import *
ENTRY=0x9c0e50;OBJECT=0x2000000;INIT=OBJECT+0x1000;D3D=OBJECT+0x2000
TIMER=OBJECT+0x3000;OLDTIMER=OBJECT+0x3100;RENDER=OBJECT+0x4000;OLDRENDER=OBJECT+0x8000;OTHER=OBJECT+0xc000
STACK=0x3000000;STOP=0x2200000;CAPS=STOP+0x100;RELEASE=STOP+0x200
TARGETS={'_Direct3DCreate9@4':0xbfefb0,'??2@YAPAXI@Z':0xbfea1a,'??3@YAXPAX@Z':0xbfe9bc,
'??0CVsyncCallbackTimer@@QAE@XZ':0x9f70e0,'??1CVsyncCallbackTimer@@QAE@XZ':0x9f7330,
'?CreateDevice@CDisplayManager@@AAE?AW4ECreateDeviceSuccess@1@ABUCCreateDeviceParameters@1@@Z':0x9bf7e0,
'?ResizeWindowToBackBufferSize@CDisplayManager@@QAEXXZ':0x9be490,
'??4CWideString@@QAEAAV0@ABV0@@Z':0x99b7d0,
'??0CRenderManager2D@@QAE@AAVCDisplayManager@@ABVCWideString@@K@Z':0x9d9a30,
'??_7CRenderManager@@6B@':0x129b400,
'?Init@CRenderManagerCore@@QAEXPAUIDirect3DDevice9@@@Z':0xa0a940,
'?Initialise@CShaderRenderManager@@QAEXPAVCRenderManager@@PAUIDirect3DDevice9@@@Z':0x98acf0,
'?FableShaderRenderManager_013bc470@@3VCShaderRenderManager@@A':0x13bc470,
'?InitVirtualPixels@CDisplayManager@@AAEXXZ':0x9be980,'__CxxThrowException@8':0xbfeb84,
'__TI2?AVCGenericException@@':0x13692f8}

def execute(code,width,height,flags,exclusive,caps_fail,create_result,old,mutation,null_timer):
 u=Uc(UC_ARCH_X86,UC_MODE_32)
 for a,n in [(0x980000,0x290000),(0x129b000,0x1000),(OBJECT,0x10000),(STACK,0x10000),(STOP,0x1000)]:u.mem_map(a,n)
 def put(a,v):u.mem_write(a,struct.pack('<I',v&0xffffffff))
 def get(a):return int.from_bytes(u.mem_read(a,4),'little')
 def byte(a,v):u.mem_write(a,bytes([v]))
 u.mem_write(ENTRY,bytes(code));u.mem_write(OBJECT,b'\xa5'*0x10000);u.mem_write(STACK,b'\xcc'*0x10000)
 put(OBJECT+8,OLDRENDER if old else 0);put(OBJECT+0x294,OLDTIMER if old else 0)
 for r in [RENDER,OLDRENDER,OTHER]:put(r,OBJECT+0xf000)
 put(OBJECT+0xf000,RELEASE);put(0x129b400,RELEASE)
 put(D3D,D3D+0x100);put(D3D+0x138,CAPS)
 for offset,value in [(0,width),(4,height),(8,32),(12,75),(0x14,0x12345678),(0x1c,4),(0x2c,2),(0x34,0x5678),(0x38,0xabcdef)]:put(INIT+offset,value)
 for offset,value in [(0x10,flags&1),(0x11,(flags>>1)&1),(0x18,24),(0x19,2),(0x28,flags&1),(0x29,(flags>>1)&1)]:byte(INIT+offset,value)
 events=[];thrown=False;caps_count=0
 def hook(u,a,size,data):
  nonlocal thrown,caps_count
  if a not in set(TARGETS.values())|{CAPS,RELEASE}:return
  sp=u.reg_read(UC_X86_REG_ESP);this=u.reg_read(UC_X86_REG_ECX);argc=0;result=this
  if a==0xbfefb0:argc=1;assert get(sp+4)==32;events.append(('d3d',));result=D3D
  elif a==0xbfea1a:
   n=get(sp+4);assert n in (20,0x3ea4);events.append(('new',n));result=(0 if null_timer else TIMER) if n==20 else RENDER
  elif a==0x9f70e0:assert this==TIMER;events.append(('timer_ctor',));u.mem_write(this,b'\x77'*20)
  elif a==0x9f7330:assert this==OLDTIMER;assert get(OBJECT+0x294)==(0 if null_timer else TIMER);events.append(('timer_dtor',))
  elif a==0xbfe9bc:assert get(sp+4)==OLDTIMER;events.append(('delete_timer',))
  elif a==CAPS:
   argc=4;caps_count+=1;assert get(sp+4)==D3D and get(sp+8)==0 and get(sp+12)==caps_count and get(sp+16)==OBJECT+0x5c
   events.append(('caps',caps_count));result=0x80004005 if caps_fail else 0
   u.mem_write(OBJECT+0x5c,bytes([caps_count])*0x130)
   if mutation==1:put(OBJECT+0x1b4,0x87654321);put(INIT,31);put(INIT+4,33);byte(OBJECT+0x1a0,1-exclusive)
  elif a==0x9bf7e0:
   argc=1;p=get(sp+4);assert this==OBJECT
   fields=tuple(get(p+i) for i in (0,4,8,12,16,20,24,36,40));booleans=tuple(u.mem_read(p+i,1)[0] for i in (28,32,33))
   assert fields==(get(OBJECT+0x1b4),max(32,31 if mutation==1 else width),max(32,33 if mutation==1 else height),32,24,2,75,2,4)
   assert booleans==(exclusive,flags&1,(flags>>1)&1)
   events.append(('create',fields,booleans));result=create_result;put(OBJECT+0x58,D3D+0x400)
  elif a==0xbfeb84:
   assert get(sp+8)==0x13692f8;events.append(('throw',));thrown=True;u.emu_stop();return
  elif a==0x9be490:
   assert this==OBJECT;events.append(('resize',))
   if mutation==2:put(INIT+0x34,0x1111);byte(OBJECT+0x1b0,1)
  elif a==0x99b7d0:
   argc=1;assert this==OBJECT+0x258 and get(sp+4)==INIT+0x38;put(this,get(INIT+0x38));events.append(('wide',))
   if mutation==3:put(INIT+0x34,0x2222)
  elif a==0x9d9a30:
   argc=3;assert this==RENDER and get(sp+4)==OBJECT and get(sp+8)==OBJECT+0x258 and get(sp+12)==get(INIT+0x34)
   events.append(('renderer_ctor',get(sp+12)));put(this,OBJECT+0xf000)
  elif a==RELEASE:
   argc=1;assert this==OLDRENDER and get(sp+4)==1 and get(OBJECT+8)==RENDER;events.append(('renderer_dtor',))
   if mutation==4:put(OBJECT+8,OTHER);byte(OBJECT+0x1b1,1)
  elif a==0xa0a940:
   argc=1;assert this==get(OBJECT+8) and get(sp+4)==get(OBJECT+0x58);events.append(('renderer_init',this))
   if mutation==5:byte(OBJECT+0x1b1,1);put(OBJECT+0x58,D3D+0x500)
  elif a==0x98acf0:
   argc=2;assert this==0x13bc470 and get(sp+4)==get(OBJECT+8) and get(sp+8)==get(OBJECT+0x58);events.append(('shader',get(sp+4),get(sp+8)))
  elif a==0x9be980:assert this==OBJECT;events.append(('virtual',))
  else:raise AssertionError(hex(a))
  u.reg_write(UC_X86_REG_EAX,result);u.reg_write(UC_X86_REG_ESP,sp+4+argc*4);u.reg_write(UC_X86_REG_EIP,get(sp))
 u.hook_add(UC_HOOK_CODE,hook);sp=STACK+0x8000;put(sp,STOP);put(sp+4,INIT);put(sp+8,exclusive);u.reg_write(UC_X86_REG_ESP,sp);u.reg_write(UC_X86_REG_ECX,OBJECT)
 u.emu_start(ENTRY,STOP,count=10000)
 assert thrown==(create_result!=2)
 if not thrown:assert u.reg_read(UC_X86_REG_EIP)==STOP and u.reg_read(UC_X86_REG_ESP)==sp+12
 return events,bytes(u.mem_read(OBJECT,0x2c8)),bytes(u.mem_read(INIT,0x3c)),bytes(u.mem_read(TIMER,20)),bytes(u.mem_read(RENDER,4))

