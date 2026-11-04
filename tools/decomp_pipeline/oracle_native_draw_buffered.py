"""Buffered-draw parent replay; shader/window/sort/COM are explicit boundaries."""
import itertools,struct
import oracle_native_clear_render_target as clear
from unicorn.x86_const import UC_X86_REG_ECX,UC_X86_REG_EAX,UC_X86_REG_ESP,UC_X86_REG_EIP
BASE=clear.BASE;WIDTH=0x9000;SHADER=clear.SHADER;LAYOUT=clear.LAYOUT;ENTRY=0x9d9c80
targets=dict(clear.targets)
targets.update({'?LibraryStateBlockFunctions@CRenderManager2D@@2PAPAVCStateBlockFunctionBase@@A':0x1396510,
 '?RealiseRenderState@CRenderStateManager@@QAEXXZ':0xa058c0,
 '?AttachTextureToStage@CRenderManagerCore@@QAEXJPBVCTexture@@@Z':0x9a0cf0,
 '?SetAWindow@CRenderManagerCore@@QAEXABVC2DBoxF@@@Z':0xa0aa80,
 '?Uninitialise@CTexture@@QAEXXZ':0x9f9f70,'??4CTexture@@QAEABV0@ABV0@@Z':0x9fa1c0,
 '?erase@?$vector@VCQuickDrawTriInfo@@V?$allocator@VCQuickDrawTriInfo@@@_STL@@@_STL@@QAEPAVCQuickDrawTriInfo@@PAV3@0@Z':0x9e15e0,
 '?D3DTextureMipMapModeTable@CRenderStateManager@@0PAKA':0x1396e64,
 '_qsort':0xbfedca,'_FableCompareQuickDrawInfoDepth_009d8c30':0x9d8c30,'_FableCompareQuickDrawVerticesDepth_009d8cb0':0x9d8cb0})
for a in (0x1396dd0,0x1396e04,0x1396e58,0x1396fa0,0x1396fa4,0x1396fb0):targets[f'?FableRenderDefaultValue_{a:08x}@@3KA']=a
names=dict(clear.names)
names.update({0xa058c0:'Realise',0xa0aa80:'Window',0x99b510:'StringDestroy',0xbfedca:'Sort',
 0x9a4ec0:'SystemRoot',
 BASE+0xc000:'Release',BASE+0xc100:'AddRef',BASE+0xc200:'SetTexture',BASE+0xc300:'DrawUP',BASE+0xc400:'ApplyBlock'})

class Runner(clear.Runner):
 def __init__(self,compose=False):
  self.compose=compose
  super().__init__()
 def event(self,name,args):
  self.events.append((name,args,bytes(self.u.mem_read(BASE,WIDTH)),bytes(self.u.mem_read(SHADER,0x3dc))))
 def hook(self,u,a,n,_):
  if a not in names or a in clear.names:
   return super().hook(u,a,n,_)
  name=names[a];sp=u.reg_read(UC_X86_REG_ESP);receiver=u.reg_read(UC_X86_REG_ECX);count=0;cdecl=False
  if name=='SystemRoot':
   self.event(name,(0,));u.reg_write(UC_X86_REG_EAX,BASE+0x8800)
   u.reg_write(UC_X86_REG_EIP,self.read(sp));u.reg_write(UC_X86_REG_ESP,sp+4);return
  elif name in ('Release','AddRef','SetTexture','DrawUP'):
   receiver=self.read(sp+4);count={'Release':1,'AddRef':1,'SetTexture':3,'DrawUP':5}[name]
   args=tuple(self.read(sp+4+i*4)for i in range(1,count));self.event(name,(receiver,)+args)
   if name=='SetTexture' and self.mutation:self.word(BASE+0x3cb8,8)
  elif name=='Sort':
   args=tuple(self.read(sp+4+i*4)for i in range(4));self.event(name,(0,)+args);cdecl=True
   # Deterministic dependency response: swap complete records. This tests
   # parent consumption of sort output, not the CRT sorting algorithm.
   first,num,size,callback=args
   data=bytes(u.mem_read(first,num*size));u.mem_write(first,b''.join(data[i*size:(i+1)*size]for i in reversed(range(num))))
  elif name=='Window':
   count=1;value=self.read(sp+4);self.event(name,(receiver,bytes(u.mem_read(value,16))))
   if self.mutation:self.word(value,self.read(value)^0x80000000)
  elif name=='StringDestroy':
   self.event(name,(receiver,));self.word(receiver,0)
  else:
   self.event(name,(receiver,))
   if name=='Realise' and self.mutation:
    self.word(BASE+0x4058,BASE+(0x8520 if self.read(BASE+0x4058)==BASE+0x8500 else 0x8500))
    for offset,low,high in ((0x3e58,0x5000,0x6000),(0x3e70,0x7000,0x7800)):
     start=self.read(BASE+offset);length=self.read(BASE+offset+4)-start;new=BASE+(high if start==BASE+low else low)
     self.word(BASE+offset,new);self.word(BASE+offset+4,new+length);self.word(BASE+offset+8,new+0x400)
   elif name=='ApplyBlock' and self.mutation:self.word(SHADER+0x3d8,self.read(SHADER+0x3d8)^0x20487)
  u.reg_write(UC_X86_REG_EAX,0x80004005 if name in ('SetTexture','DrawUP')else 7)
  u.reg_write(UC_X86_REG_EIP,self.read(sp));u.reg_write(UC_X86_REG_ESP,sp+4+(0 if cdecl else count*4))
 def run(self,arena,shader,layout,flags,mutation):
  u=self.u;self.mutation=mutation;self.events=[]
  u.mem_write(BASE,arena);u.mem_write(SHADER,shader);u.mem_write(LAYOUT,layout);self.word(0x13cb508,BASE)
  self.word(BASE+0xa004,BASE+0xc100);self.word(BASE+0xa008,BASE+0xc000)
  self.word(BASE+0xa100+65*4,BASE+0xc200);self.word(BASE+0xa100+83*4,BASE+0xc300);self.word(BASE+0xa400,BASE+0xc400)
  if not self.compose:
   for i in range(7):self.word(0x1396510+i*4,BASE+0x8600+i*16)
  u.mem_write(BASE+0x10000,bytes([0xa5])*0x10000);stack=BASE+0x1e000
  self.word(stack,BASE+0x1f000);u.reg_write(UC_X86_REG_ESP,stack);u.reg_write(UC_X86_REG_ECX,BASE)
  u.emu_start(ENTRY,BASE+0x1f000,count=200000);assert u.reg_read(UC_X86_REG_ESP)==stack+4
  self.event('return',());return self.events

def scenarios(compose=False):
 for case,(opaque,alpha,pattern,enabled,mutation)in enumerate(itertools.product((0,1,3),(0,1,3),range(8),range(4),range(2))):
  arena=bytearray((i*17+case*13)&255 for i in range(WIDTH));shader=bytearray((i*7+case)&255 for i in range(0x3dc));layout=bytearray(0x14c)
  def put(data,o,v):struct.pack_into('<I',data,o,v&0xffffffff)
  put(arena,0x3a3c,BASE+0x4000);put(arena,0x4058,BASE+0x8500);put(arena,0x3cf0,BASE+0x8500);put(arena,0x3cb8,0)
  put(arena,0x4008,BASE);put(arena,0x8860,BASE+0x4000)
  put(arena,0x2808,0);put(arena,0x280c,0);put(arena,0x2814,1<<((case%5)+1))
  for at in range(0x2818,0x3a24,20):
   put(arena,at,0);put(arena,at+4,case%3);put(arena,at+8,0);arena[at+16]=0
  for offset,start,count,size in ((0x3e58,0x5000,opaque*3,32),(0x3e64,0x4400,opaque,60),(0x3e70,0x7000,alpha*3,32),(0x3e7c,0x4800,alpha,60)):
   put(arena,offset,BASE+start);put(arena,offset+4,BASE+start+count*size);put(arena,offset+8,BASE+start+0x400)
  for start,count in ((0x4400,opaque),(0x4800,alpha)):
   for i in range(count):
    at=start+i*60;arena[at]=(arena[at]&0xfc)|((i%2)if pattern&1 else 0)
    for k,value in enumerate((0,0,0,0)if pattern==0 else (i if pattern&2 else 2,0,320,240)):
     put(arena,at+4+k*4,struct.unpack('<I',struct.pack('<f',value))[0])
    put(arena,at+20,0);put(arena,at+24,BASE+0x8400+(16*(i%2)) if pattern&4 else 0)
    blend=(i+pattern)%7 if pattern&2 else 1
    if compose:blend=max(1,min(5,blend))
    put(arena,at+32,blend)
  for at in (0x8400,0x8410):put(arena,at,BASE+0xa000)
  for at in (0x8500,0x8520):put(arena,at,BASE+0xa100)
  for i in range(7):put(arena,0x8600+i*16,BASE+0xa400)
  put(shader,4,LAYOUT);put(shader,8,7);put(shader,0x3d8,0x20487 if pattern&1 else 0);shader[0x390]=enabled&1;shader[0x391]=(enabled>>1)&1;shader[0x1e8]=(pattern>>1)&1
  put(layout,8,12);put(layout,0x140,BASE+0x8200);put(layout,0x144,BASE+0x8200+(32 if pattern&4 else 0));put(layout,0x148,BASE+0x8240)
  yield case,mutation,bytes(arena),bytes(shader),bytes(layout)
