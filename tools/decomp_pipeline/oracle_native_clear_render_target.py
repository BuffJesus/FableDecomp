"""Retail clear-path oracle with explicit engine dependency boundaries."""
import itertools,struct
from unicorn import Uc,UC_ARCH_X86,UC_MODE_32,UC_HOOK_CODE
from unicorn.x86_const import UC_X86_REG_ECX,UC_X86_REG_EAX,UC_X86_REG_ESP,UC_X86_REG_EIP
from pe_oracle import EXE,pe_sections
BASE=0x24000000;WIDTH=0x6000;SHADER=0x13bc470;LAYOUT=0x13ca8a8;ENTRY=0x9d8cf0
targets={'___security_cookie': 20564136, '@__security_check_cookie@4': 12577273, '?FableShaderRenderManager_013bc470@@3VCShaderRenderManager@@A': 20694128, '?FableCurrentRenderStateManager_013cb508@@3PAVCRenderStateManager@@A': 20755720, '?Fable2DShaderConstantLayout_013ca8a8@@3VCVertexShaderConstantLayoutEmpty@@A': 20752552, '?ClearRenderTarget@CDisplayManager@@QAEXABVCRGBColour@@K@Z': 10216480, '?EnableVertexShaders@CShaderRenderManager@@QAEXXZ': 9994208, '?DisablePixelShaders@CShaderRenderManager@@QAEXXZ': 9994640, '?FableRenderDefaultValue_01396e14@@3KA': 20540948, '?FableRenderDefaultValue_01396e34@@3KA': 20540980, '?FableRenderDefaultValue_01396f40@@3KA': 20541248, '?GetRenderTargetDimensions@CDisplayManager@@QAE?BVC2DExtentsI@@XZ': 10218944, '?SetIntegerViewportEndpoint@CDisplayManager@@QAEXABVC2DBoxI@@@Z': 10219392, '?RegisterVertexShaderConstantLayout@CShaderRenderManager@@QAEXJPBVCVertexShaderConstantLayout@@@Z': 10007088, '?SetVertexShaderConstantLayout@CShaderRenderManager@@QAEXJ@Z': 10008032, '?ApplyVertexShader@CShaderRenderManager@@QAEXABVCVertexShader@@_N@Z': 9994272, '?SetVSConstants@CShaderRenderManager@@QAEXJPBXK@Z': 10008880, '?UpdatePixelShader@CShaderRenderManager@@QAEXXZ': 9996832, '?UpdatePixelShaderConstants@CShaderRenderManager@@QAEXXZ': 10001392, '?UpdateAmbient@CShaderRenderManager@@QAEXXZ': 10000224, '?UpdateFogColour@CShaderRenderManager@@QAEXXZ': 10000320, '?UpdateLightGlobals@CShaderRenderManager@@QAEXXZ': 10004320, '?ResetWorldTransform@CShaderRenderManager@@QAEXXZ': 9994896, '?UpdateCombinedProjectionTransform@CShaderRenderManager@@QAEXXZ': 9996880, '?FableRenderDefaultValue_01396e74@@3KA': 20541044, '?FableRenderDefaultValue_01396e70@@3KA': 20541040, '?FableRenderDefaultValue_01396eec@@3KA': 20541164, '?DrawAsIndexedTris@CRenderManagerCore@@QAEXPBXJJPAXJ@Z': 10530624, '?RestoreCaptureBlock@CRenderStateManager@@QAEXXZ': 10508352, '?DisableVertexShaders@CShaderRenderManager@@QAEXXZ': 9994464, '?EnablePixelShaders@CShaderRenderManager@@QAEXXZ': 9994512}
raw=EXE.read_bytes()
names={value:name.split('@')[0].lstrip('?') for name,value in targets.items() if name.startswith('?') and '@@QAE' in name}
argc={'ClearRenderTarget':2,'SetIntegerViewportEndpoint':1,'RegisterVertexShaderConstantLayout':2,'SetVertexShaderConstantLayout':1,'ApplyVertexShader':2,'SetVSConstants':3,'DrawAsIndexedTris':5,'GetRenderTargetDimensions':1}
class Runner:
 def __init__(self):
  self.u=Uc(UC_ARCH_X86,UC_MODE_32);self.u.mem_map(0x400000,0x1200000);self.u.mem_map(BASE,0x20000)
  for _,va,_,at,size in pe_sections(raw):
   if size:self.u.mem_write(0x400000+va,raw[at:at+size])
  self.u.hook_add(UC_HOOK_CODE,self.hook)
 def read(self,a):return struct.unpack('<I',self.u.mem_read(a,4))[0]
 def word(self,a,v):self.u.mem_write(a,struct.pack('<I',v&0xffffffff))
 def event(self,name,args):
  self.events.append((name,args,bytes(self.u.mem_read(BASE,WIDTH)),bytes(self.u.mem_read(SHADER,0x3dc))))
 def hook(self,u,a,n,_):
  if a==targets['@__security_check_cookie@4']:
   assert u.reg_read(UC_X86_REG_ECX)==self.read(targets['___security_cookie'])
   count=0
  elif a in names:
   name=names[a];count=argc.get(name,0);sp=u.reg_read(UC_X86_REG_ESP);args=tuple(self.read(sp+4+i*4)for i in range(count));receiver=u.reg_read(UC_X86_REG_ECX)
   if name=='GetRenderTargetDimensions':
    self.event(name,(receiver,));u.mem_write(args[0],bytes(u.mem_read(receiver+0x194,8)));u.reg_write(UC_X86_REG_EAX,args[0])
   elif name=='SetIntegerViewportEndpoint':
    self.event(name,(receiver,bytes(u.mem_read(args[0],16))));u.mem_write(receiver+0x1f4,bytes(u.mem_read(args[0],16)))
   elif name=='DrawAsIndexedTris':
    vertices=bytearray(u.mem_read(args[0],args[1]*args[2]));indices=bytes(u.mem_read(args[3],args[4]*6))
    for i in range(args[2]):vertices[i*32+20:i*32+24]=b'\0'*4
    self.event(name,(receiver,args[1],args[2],args[4],bytes(vertices),indices))
   else:
    self.event(name,(receiver,)+args)
    if name=='EnableVertexShaders':u.mem_write(SHADER+0x390,b'\1')
    elif name=='DisableVertexShaders':u.mem_write(SHADER+0x390,b'\0')
    elif name=='EnablePixelShaders':u.mem_write(SHADER+0x391,b'\1')
    elif name=='DisablePixelShaders':u.mem_write(SHADER+0x391,b'\0')
    elif name=='RegisterVertexShaderConstantLayout' and self.mutation:self.word(SHADER+8,19)
    elif name=='SetVertexShaderConstantLayout':self.word(SHADER+8,args[0]);self.word(SHADER+4,LAYOUT)
    elif name=='ResetWorldTransform':u.mem_write(SHADER+0x1e8,b'\1')
    elif name=='UpdatePixelShader' and self.mutation:self.word(SHADER+0x3d8,self.read(SHADER+0x3d8)^0x20486)
  else:return
  sp=u.reg_read(UC_X86_REG_ESP);u.reg_write(UC_X86_REG_EIP,self.read(sp));u.reg_write(UC_X86_REG_ESP,sp+4+count*4)
 def run(self,arena,shader,layout,flags,mutation):
  u=self.u;self.mutation=mutation;self.events=[]
  u.mem_write(BASE,arena);u.mem_write(SHADER,shader);u.mem_write(LAYOUT,layout);self.word(0x13cb508,BASE)
  u.mem_write(BASE+0x10000,bytes([0xa5])*0x10000);stack=BASE+0x1e000
  u.mem_write(stack,struct.pack('<3I',BASE+0x1f000,BASE+0x5000,flags));u.reg_write(UC_X86_REG_ESP,stack);u.reg_write(UC_X86_REG_ECX,BASE)
  u.emu_start(ENTRY,BASE+0x1f000,count=100000);assert u.reg_read(UC_X86_REG_ESP)==stack+12
  self.event('return',());return self.events

def scenarios():
 cases=0
 for flags,enabled,alternate,dirty,mutation,updates,identity,presets in itertools.product(range(16),range(4),range(2),range(2),range(2),(0,0x20487),range(2),range(2)):
  arena=bytearray((i*17+cases*13)&255 for i in range(WIDTH));shader=bytearray((i*7+cases)&255 for i in range(0x3dc));layout=bytearray(0x14c)
  def put(data,o,v):struct.pack_into('<I',data,o,v&0xffffffff)
  put(arena,0x3a3c,BASE+0x4000);arena[0x3ea0]=alternate
  put(arena,0x2808,0);put(arena,0x280c,0);put(arena,0x2814,1<<((cases%5)+1))
  for at in range(0x2818,0x3a24,20):
   put(arena,at,0);put(arena,at+4,cases%3);put(arena,at+8,0);arena[at+16]=dirty
   if dirty:
    count=struct.unpack_from('<I',arena,0x280c)[0];put(arena,0x2008+count*4,BASE+at);put(arena,0x280c,count+1)
  put(arena,0x4194,640);put(arena,0x4198,480)
  put(shader,4,LAYOUT);put(shader,8,7);put(shader,0x3d8,updates);shader[0x390]=enabled&1;shader[0x391]=(enabled>>1)&1;shader[0x1e8]=identity
  put(layout,8,12);put(layout,0x140,BASE+0x5100);put(layout,0x144,BASE+0x5100+(32 if presets else 0));put(layout,0x148,BASE+0x5140)
  yield flags,mutation,bytes(arena),bytes(shader),bytes(layout)
  cases+=1
