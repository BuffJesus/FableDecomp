"""Mixed text/geometry parent replay, including the 2001-primitive boundary."""
import struct
import oracle_native_draw_buffered as draw
from unicorn.x86_const import UC_X86_REG_ECX,UC_X86_REG_EAX,UC_X86_REG_ESP,UC_X86_REG_EIP
BASE=draw.BASE;SHADER=draw.SHADER;LAYOUT=draw.LAYOUT;ENTRY=0x9da9f0
targets=dict(draw.targets)
targets.update({'?InitialiseFromPreallocatedTexture@CTexture@@QAE_NPAUIDirect3DTexture9@@@Z':0x9fa230,
 '?erase@?$vector@VCTVertexRHWColSpecTex1@@V?$allocator@VCTVertexRHWColSpecTex1@@@_STL@@@_STL@@QAEPAVCTVertexRHWColSpecTex1@@PAV3@0@Z':0x9e1440})
for a in (0x1396de8,0x1396f78,0x1396f7c,0x1396f6c,0x1396f68,0x1396ef0,0x1396e7c,0x1396e5c):targets[f'?FableRenderDefaultValue_{a:08x}@@3KA']=a
names=dict(draw.names);names.update({BASE+0xc500:'FontPrepare',BASE+0xc600:'FontRender'})
class Runner(draw.Runner):
 def __init__(self):
  super().__init__(True);self.u.mem_map(BASE+0x20000,0xe0000)
 def event(self,name,args):
  compact=self.width>0x9000 and name=='StringDestroy'
  self.events.append((name,args,b''if compact else bytes(self.u.mem_read(BASE,self.width)),b''if compact else bytes(self.u.mem_read(SHADER,0x3dc))))
 def hook(self,u,a,n,_):
  if a==0xa058c0:
   self.event('Realise',(u.reg_read(UC_X86_REG_ECX),))
   if self.mutation:
    self.word(BASE+0x4058,BASE+(0x8520 if self.read(BASE+0x4058)==BASE+0x8500 else 0x8500))
    start=self.read(BASE+0x3e88);length=self.read(BASE+0x3e8c)-start;new=BASE+(0x6000 if start==BASE+0x5000 else 0x5000)
    self.word(BASE+0x3e88,new);self.word(BASE+0x3e8c,new+length);self.word(BASE+0x3e90,new+0x1000)
   count=0
  elif a in (BASE+0xc500,BASE+0xc600):
   sp=u.reg_read(UC_X86_REG_ESP);receiver=u.reg_read(UC_X86_REG_ECX);count=1 if a==BASE+0xc500 else 6
   args=tuple(self.read(sp+4+i*4)for i in range(count))
   if count==1:
    self.event('FontPrepare',(receiver,)+args)
    if self.mutation:
     u.mem_write(SHADER+0x390,b'\1\1')
     for at in range(self.read(BASE+0x3e94),self.read(BASE+0x3e98),60):
      if u.mem_read(at,1)[0]&2:
       self.word(at+0x2c,self.read(at+0x2c)^0x80000000)
       self.word(at+0x38,BASE+(0x8720 if self.read(at+0x38)==BASE+0x8700 else 0x8700))
   else:
    self.event('FontRender',(receiver,args[0],args[2],args[3],args[4],args[5],bytes(u.mem_read(args[1],12))))
    if self.mutation:u.mem_write(SHADER+0x390,b'\0\0')
  else:return super().hook(u,a,n,_)
  sp=u.reg_read(UC_X86_REG_ESP);u.reg_write(UC_X86_REG_EAX,7);u.reg_write(UC_X86_REG_EIP,self.read(sp));u.reg_write(UC_X86_REG_ESP,sp+4+count*4)
 def run(self,arena,shader,layout,depth,mutation):
  u=self.u;self.width=len(arena);self.mutation=mutation;self.events=[]
  u.mem_write(BASE,arena);u.mem_write(SHADER,shader);u.mem_write(LAYOUT,layout);self.word(0x13cb508,BASE)
  for offset,address in ((4,0xc100),(8,0xc000),(0x100+65*4,0xc200),(0x100+83*4,0xc300),(0x600+3*4,0xc500),(0x600+5*4,0xc600)):
   self.word(BASE+0xd0000+offset,BASE+address)
  stack=BASE+0xf0000;stop=BASE+0xff000;u.mem_write(stack,struct.pack('<2I',stop,depth));u.reg_write(UC_X86_REG_ESP,stack);u.reg_write(UC_X86_REG_ECX,BASE)
  u.emu_start(ENTRY,stop,count=2000000);assert u.reg_read(UC_X86_REG_ESP)==stack+8
  self.event('return',());return self.events

def make_case(arena,shader,layout,count,pattern,depth,mutation,large=False):
 arena=bytearray(arena);width=0xc0000 if large else 0x9000
 if large:arena.extend(bytes((i*17+pattern*13)&255 for i in range(len(arena),width)))
 def put(o,v):struct.pack_into('<I',arena,o,v&0xffffffff)
 def floating(o,v):struct.pack_into('<f',arena,o,v)
 info=0x10000 if large else 0x4400;vertices=0x50000 if large else 0x5000;vertex_count=0
 for i in range(count):
  at=info+i*60
  text=False if large else (pattern==1 or (pattern==2 and i%2==1)or(pattern==3 and i==0)or(pattern==4 and i==count-1)or(pattern==5 and i<2)or(pattern==7 and i%3!=1))
  line=bool(pattern&1 and i%2 and not large);arena[at]=(arena[at]&0xfc)|line|(2 if text else 0)
  for k,v in enumerate((float(i%3)if pattern&2 else 0.0,-0.0 if pattern&4 else 0.0,320.0,240.0)):floating(at+4+k*4,v)
  put(at+20,BASE+0x8300);put(at+24,BASE+0x8400+16*(i%2)if pattern&4 else 0)
  put(at+32,1+(i%5)if pattern&2 else 1)
  if text:
   floating(at+32,i*.25);floating(at+36,-i*.5);floating(at+40,.75);floating(at+44,1.25)
  else:vertex_count+=2 if line else 3
  put(at+56,BASE+0x8700+32*(i%2))
 put(0x3e94,BASE+info);put(0x3e98,BASE+info+count*60);put(0x3e9c,BASE+info+count*60)
 put(0x3e88,BASE+vertices);put(0x3e8c,BASE+vertices+vertex_count*32);put(0x3e90,BASE+vertices+max(0x1000,vertex_count*32))
 for at in (0x8400,0x8410):put(at,BASE+0xd0000)
 for at in (0x8500,0x8520):put(at,BASE+0xd0100)
 for at in (0x8700,0x8720):put(at,BASE+0xd0600)
 return depth,mutation,bytes(arena),shader,layout
def scenarios():
 for case,mutation,arena,shader,layout in draw.scenarios(True):
  alpha=(struct.unpack_from('<I',arena,0x3e80)[0]-struct.unpack_from('<I',arena,0x3e7c)[0])//60
  if alpha:continue
  count=(struct.unpack_from('<I',arena,0x3e68)[0]-struct.unpack_from('<I',arena,0x3e64)[0])//60;pattern=(case//8)%8
  for depth in range(2):
   yield make_case(arena,shader,layout,count,pattern,depth,mutation)
   if count==3:yield make_case(arena,shader,layout,6,pattern,depth,mutation)
 seed=next(draw.scenarios(True));_,_,arena,shader,layout=seed
 for count in (2000,2001,2002,4003):
  for depth in range(2):yield make_case(arena,shader,layout,count,0,depth,0,True)
