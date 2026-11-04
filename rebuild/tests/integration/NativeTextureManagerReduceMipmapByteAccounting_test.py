"""Bounded root differential proof. Child methods are explicit callback interfaces."""
import hashlib,json,struct,sys,subprocess
from pathlib import Path
from unicorn import Uc,UC_ARCH_X86,UC_MODE_32,UC_HOOK_CODE
from unicorn.x86_const import UC_X86_REG_EAX,UC_X86_REG_ECX,UC_X86_REG_EDX,UC_X86_REG_ESP,UC_X86_REG_EIP,UC_X86_REG_EBX,UC_X86_REG_EBP,UC_X86_REG_ESI,UC_X86_REG_EDI
from capstone import Cs,CS_ARCH_X86,CS_MODE_32
R=Path(__file__).resolve().parents[3];sys.path.insert(0,str(R/'tools/decomp_pipeline'))
import check_one as V
from gate_game_component_abi import Coff
from near_match import compare,retail_relocations
from pe_oracle import EXE,read_virtual_data
SOURCE=R/'rebuild/src/compiled/00/a6/CTextureManager_ReduceMipmapLevel_00a69f70.cpp'
import tempfile
BUILD=Path(tempfile.mkdtemp(prefix='native-texture-manager-reduce-mipmap-'))
OBJ=BUILD/'root.obj';ENTRY=0xa69f70
ROOT_SYMBOL='?ReduceMipmapLevel@CTextureManager@@QAE_NAAVCGraphicFrame@@AAVCManagedTexture@@K@Z'
TARGETS={
'?GetNoLevels@CTexture@@QBEJXZ':0x9f9df0,
'??0CBaseClass@@QAE@XZ':0x99a2d0,'??1CBaseClass@@UAE@XZ':0x99a2e0,
'??_7CManagedTexture@@6B@':0x129c880,'??_7CSurface@@6B@':0x122f84c,
'?GetPixelFormat@CTexture@@QBE?AVCPixelFormat@@XZ':0x9f9d80,
'?AllocateTexture@CTextureManager@@QAE_NAAVCManagedTexture@@KKIABVCPixelFormat@@PAVCManagedTextureOwner@@@Z':0xa69b70,
'?ReleaseTexture@CManagedTextureData@@QAEXXZ':0xa69a80,
'?GetSurfaceLevel@CTexture@@QBE?AVCSurface@@J@Z':0x9f9e00,
'?CopyFromSurface@CSurface@@QAEXPBVC2DBoxI@@ABV1@0K@Z':0x9f37e0,
'?Uninitialise@CSurface@@QAEXXZ':0x9f2e20,
'?Uninitialise@CGraphicFrameData@@QAEXXZ':0x9fcba0,
'??3@YAXPAX@Z':0xbfe9bc,
'?InitialiseAsSurface@CGraphicFrame@@QAE_NABV?$CCountedPointer@VCTexture@@@@JJ_N1@Z':0x9fcb20}
TARGETS.update({'?Initialise@CPixelFormat@@QAEXW4_D3DFORMAT@@@Z':0x9e3830,'?GFGetSystemManager@@YIPAVCSystemManager@@XZ':0x9a4ec0,'?CalcByteLength@CTexture@@AAEXXZ':0x9f9ee0,'?GetNextValidDimensions@CTexture@@SI?AVC2DExtentsI@@ABV2@ABVCPixelFormat@@@Z':0x9fa620,'?Initialise@CTexture@@QAE_NABVC2DExtentsI@@JABVCPixelFormat@@KW4ESurfacePool@@_N3@Z':0x9fa280,'?SetAsMRU@CResource@@QAEXXZ':0x9fc4a0,'?IsBusy@CTexture@@QBE_NXZ':0x9f9d50,'?ReuseBusyTextures@CTextureManager@@0_NA':0x13d28e8,'?GD3DFormats@@3QBVCD3DFormatInfo@@B':0x129ba40,'?GetColourDepth@CPixelFormat@@QBEKXZ':0x9e3820,'?CalculateTextureSize@CTextureManager@@SIKKK_NABVCPixelFormat@@@Z':0xa698f0,'?GetPoolIndex@CTextureManager@@AAEJK@Z':0xa69730,'?Allocate@CPreallocTexturePool@@QAEPAVCManagedTextureData@@XZ':0xa69830,'?CreateTexture@CManagedTextureData@@QAE_NKKIABVCPixelFormat@@@Z':0xa697a0,'?CreateVolumeTexture@CManagedTextureData@@QAE_NKKKIABVCPixelFormat@@@Z':0xa699d0,'?SetAsLRU@CResource@@QAEXXZ':0x9fc450,'??2@YAPAXI@Z':0xbfea1a,'?InitialiseAsSurface@CGraphicFrameData@@QAE_NABV?$CCountedPointer@VCTexture@@@@JJ_N1@Z':0x9fc950,'?GetDimensions@CTexture@@QBE?AVC2DExtentsI@@K@Z':0x9f9db0})
ARENA=0x2000000;STACK=0x3000000;STOP=0x4000000;DELETER=STOP+0x100;VDELETE=STOP+0x200
SIZE=16384;MASK=0xffffffff
def pack(v):return struct.pack('<I',v&MASK)
def signed(v):return v if v<0x80000000 else v-0x100000000
def half(v):return (int(signed(v)/2))&MASK
env=V.env();res=V.cl(['/nologo','/c','/O2','/Oy','/GS','/MT','/I'+str(R/'rebuild/include'),'/Fo'+str(OBJ),str(SOURCE)],env,R)
assert res.returncode==0,res.stdout+res.stderr
coff=Coff(OBJ);section,off=coff.symbols[ROOT_SYMBOL];compiled,rel=coff.sections[section];assert off==0
original=read_virtual_data(EXE.read_bytes(),ENTRY,955)
score=compare(compiled,original,tuple(rel),retail_relocations('00a69f70',955));assert score.near_match
patched=bytearray(compiled)
for site,(symbol,kind) in rel.items():
 assert symbol in TARGETS and kind in (6,20),(symbol,kind)
 target=TARGETS[symbol]+struct.unpack_from('<I',compiled,site)[0]
 if kind==20:target-=ENTRY+site+4
 patched[site:site+4]=pack(target)
cs=Cs(CS_ARCH_X86,CS_MODE_32);calls=[]
for ins in cs.disasm(original,ENTRY):
 if ins.mnemonic=='call' and ins.bytes[0]==0xe8:
  target=int(ins.op_str,16);assert target in TARGETS.values(),hex(target);calls.append(target)
assert original[-3:]==bytes.fromhex('c20c00') and len(original)==955

HELPERS=[
 (R/'rebuild/src/compiled/00/9f/CGraphicFrame_InitialiseAsSurface_009fcb20.cpp',0x9fcb20,57,'?InitialiseAsSurface@CGraphicFrame@@QAE_NABV?$CCountedPointer@VCTexture@@@@JJ_N1@Z'),
 (R/'rebuild/src/compiled/00/9f/CGraphicFrameData_InitialiseAsSurface_009fc950.cpp',0x9fc950,230,'?InitialiseAsSurface@CGraphicFrameData@@QAE_NABV?$CCountedPointer@VCTexture@@@@JJ_N1@Z'),
 (R/'rebuild/src/compiled/00/9f/CTexture_GetDimensions_009f9db0.cpp',0x9f9db0,43,'?GetDimensions@CTexture@@QBE?AVC2DExtentsI@@K@Z'),
 (R/'rebuild/src/compiled/00/9f/CGraphicFrameData_Uninitialise_009fcba0.cpp',0x9fcba0,69,'?Uninitialise@CGraphicFrameData@@QAEXXZ'),
 (R/'rebuild/src/compiled/00/a6/CManagedTextureData_ReleaseTexture_00a69a80.cpp',0xa69a80,76,'?ReleaseTexture@CManagedTextureData@@QAEXXZ'),
 (R/'rebuild/src/compiled/00/9f/CResource_SetAsLRU_009fc450.cpp',0x9fc450,78,'?SetAsLRU@CResource@@QAEXXZ'),
 (R/'rebuild/src/compiled/00/a6/CTextureManager_Allocate_00a69b70.cpp',0xa69b70,176,'?AllocateTexture@CTextureManager@@QAE_NAAVCManagedTexture@@KKIABVCPixelFormat@@PAVCManagedTextureOwner@@@Z'),
 (R/'rebuild/src/compiled/00/a6/CTextureManager_CalculateTextureSize_00a698f0.cpp',0xa698f0,136,'?CalculateTextureSize@CTextureManager@@SIKKK_NABVCPixelFormat@@@Z'),
 (R/'rebuild/src/compiled/00/9e/CPixelFormat_GetColourDepth_009e3820.cpp',0x9e3820,13,'?GetColourDepth@CPixelFormat@@QBEKXZ'),
 (R/'rebuild/src/compiled/00/a6/CTextureManager_GetPoolIndex_00a69730.cpp',0xa69730,99,'?GetPoolIndex@CTextureManager@@AAEJK@Z'),
 (R/'rebuild/src/compiled/00/a6/CPreallocTexturePool_Allocate_00a69830.cpp',0xa69830,125,'?Allocate@CPreallocTexturePool@@QAEPAVCManagedTextureData@@XZ'),
 (R/'rebuild/src/compiled/00/9f/CResource_SetAsMRU_009fc4a0.cpp',0x9fc4a0,80,'?SetAsMRU@CResource@@QAEXXZ'),
 (R/'rebuild/src/compiled/00/9f/CTexture_IsBusy_009f9d50.cpp',0x9f9d50,3,'?IsBusy@CTexture@@QBE_NXZ'),
 (R/'rebuild/src/compiled/00/a6/CManagedTextureData_CreateTexture_00a697a0.cpp',0xa697a0,100,'?CreateTexture@CManagedTextureData@@QAE_NKKIABVCPixelFormat@@@Z'),
 (R/'rebuild/src/compiled/00/9f/CTexture_Initialise_009fa280.cpp',0x9fa280,181,'?Initialise@CTexture@@QAE_NABVC2DExtentsI@@JABVCPixelFormat@@KW4ESurfacePool@@_N3@Z'),
 (R/'rebuild/src/compiled/00/9f/CTexture_PreallocatedStorage_009fa230.cpp',0x9f9ee0,143,'?CalcByteLength@CTexture@@AAEXXZ'),
 (R/'rebuild/src/compiled/00/9e/CPixelFormat_Initialise_009e3830.cpp',0x9e3830,50,'?Initialise@CPixelFormat@@QAEXW4_D3DFORMAT@@@Z')]
from variable_allocator_parity import check as parity_check
helper_objects=[];helper_original={};helper_built={};helper_proofs=[]
for source,address,length,symbol in HELPERS:
 obj=BUILD/(source.stem+'.obj')
 res=V.cl(['/nologo','/c','/O2','/Oy','/GS','/MT','/I'+str(R/'rebuild/include'),'/Fo'+str(obj),str(source)],env,R)
 assert not res.returncode,res.stdout+res.stderr
 cf=Coff(obj);sec,off=cf.symbols[symbol];body,fixups=cf.sections[sec];assert off==0
 fixed=bytearray(body)
 for site,(target,kind) in fixups.items():
  assert target in TARGETS and kind in (6,20)
  value=TARGETS[target]+struct.unpack_from('<I',body,site)[0]
  if kind==20:value-=address+site+4
  fixed[site:site+4]=pack(value)
 original_body=read_virtual_data(EXE.read_bytes(),address,length)
 helper_proof=parity_check(obj,symbol,address,length,len(fixups),TARGETS)
 helper_proof.update(source=str(source.relative_to(R)),native_sha256=hashlib.sha256(original_body).hexdigest())
 helper_proofs.append(helper_proof)
 helper_objects.append(obj);helper_original[address]=original_body;helper_built[address]=bytes(fixed)
from gate_render_state_static_data import coff_data
TABLE_SOURCE=R/'rebuild/src/compiled/01/29/CPixelFormat_Table_0129ba40.cpp'
table_obj=BUILD/'table.obj'
res=V.cl(['/nologo','/c','/O2','/MT','/I'+str(R/'rebuild/include'),'/Fo'+str(table_obj),str(TABLE_SOURCE)],env,R)
assert not res.returncode,res.stdout+res.stderr
table=coff_data(Coff(table_obj),'?GD3DFormats@@3QBVCD3DFormatInfo@@B',1692)
assert table==read_virtual_data(EXE.read_bytes(),0x129ba40,1692)
helper_objects.append(table_obj);helper_original[0x129ba40]=table;helper_built[0x129ba40]=table
REUSE_SOURCE=R/'rebuild/src/compiled/01/3d/CTextureManager_ReuseBusyTextures_013d28e8.cpp'
reuse_obj=BUILD/'reuse.obj'
res=V.cl(['/nologo','/c','/O2','/MT','/I'+str(R/'rebuild/include'),'/Fo'+str(reuse_obj),str(REUSE_SOURCE)],env,R)
assert not res.returncode,res.stdout+res.stderr
reuse=coff_data(Coff(reuse_obj),'?ReuseBusyTextures@CTextureManager@@0_NA',1)
assert reuse==read_virtual_data(EXE.read_bytes(),0x13d28e8,1)==bytes(1)
helper_objects.append(reuse_obj);helper_original[0x13d28e8]=reuse;helper_built[0x13d28e8]=reuse
# Link-only omission check: the fixture has no substitute for the tested root.
link_source=BUILD/'required_root.cpp';link_obj=BUILD/'required_root.obj'
aliases=['#pragma comment(linker,"/alternatename:_MustHaveRoot='+ROOT_SYMBOL+'")']
aliases+=['#pragma comment(linker,"/alternatename:'+symbol+'=_UnusedBoundary")' for symbol in TARGETS if not symbol.startswith('??_7') and TARGETS[symbol] not in (0x9fcb20,0x9fc950,0x9f9db0,0x9fcba0,0xa69a80,0x9fc450,0xa69b70,0xa698f0,0xa69730,0x9e3820,0x129ba40,0xa69830,0x9fc4a0,0x9f9d50,0x13d28e8,0xa697a0,0x9fa280,0x9f9ee0,0x9e3830)]
link_source.write_text('#include <stdlib.h>\nextern "C" void UnusedBoundary(){abort();}\nextern "C" void MustHaveRoot();\nint main(){MustHaveRoot();return 0;}\n'+'\n'.join(aliases)+'\n')
res=V.cl(['/nologo','/c','/O2','/MT','/Fo'+str(link_obj),str(link_source)],env,BUILD)
assert not res.returncode,res.stdout+res.stderr
link=[str(V.VC/'bin/link.exe'),'/nologo','/out:'+str(BUILD/'required_root.exe'),str(link_obj)]
res=subprocess.run(link,env=env,capture_output=True,text=True)
assert res.returncode and ('LNK2001' in res.stdout or 'LNK2019' in res.stdout),res.stdout+res.stderr
all_objects=[OBJ]+helper_objects
for missing in range(20):
 res=subprocess.run(link+[str(obj) for i,obj in enumerate(all_objects) if i!=missing],env=env,capture_output=True,text=True)
 assert res.returncode and ('LNK2001' in res.stdout or 'LNK2019' in res.stdout),res.stdout+res.stderr
res=subprocess.run(link+[str(obj) for obj in all_objects],env=env,capture_output=True,text=True)
assert not res.returncode,res.stdout+res.stderr

class Memory:
 def __init__(self,uc=None):self.uc=uc;self.blocks={} if uc is None else None
 def read(self,a,n):
  if self.uc:return bytes(self.uc.mem_read(a,n))
  return bytes(self.blocks.get(a+i,0) for i in range(n))
 def write(self,a,b):
  if self.uc:self.uc.mem_write(a,bytes(b))
  else:self.blocks.update((a+i,v) for i,v in enumerate(b))
 def get(self,a):return struct.unpack('<I',self.read(a,4))[0]
 def put(self,a,v):self.write(a,pack(v))
 def byte(self,a,v=None):
  if v is None:return self.read(a,1)[0]
  self.write(a,bytes([v&255]))

class Case:
 def __init__(self,m,config,expected=None):
  self.m=m;self.cfg=config;self.expected=expected;self.events=0;self.snapshots=[];self.temp=0;self.surface_calls=0;self.copies=0
  dims,reduction,extra,success,count,profile,place,fill=config
  self.format_index=[1,7,31,35][dims];self.profile=profile;self.success=success;self.reduction=reduction;self.levels=reduction+extra
  self.base=ARENA+(0x1800 if place else 0);b=self.base
  self.manager=b+0x100;self.frame=b+0x700;self.managed=b+0x740
  self.fd=[b+0x800,b+0x880,b+0x900];self.md=[b+0xa00,b+0xa80,b+0xb00]
  self.tex=[b+0xc00,b+0xc40,b+0xc80];self.cp=[b+0xd00,b+0xd40,b+0xd80]
  self.vt=b+0xe00;self.com=[b+0xf00,b+0xf40]
  m.write(ARENA,bytes([0xa9 if fill else 0])*SIZE)
  m.put(self.frame,self.fd[0]);m.put(self.managed,0x129c880);m.put(self.managed+4,self.md[0])
  m.put(self.vt,STOP+0x300);m.put(self.vt+4,VDELETE)
  widths=[0,1,257,65535];heights=[65535,129,3,0]
  for i,fd in enumerate(self.fd):
   m.put(fd,count);m.put(fd+8,self.tex[i]);m.put(fd+12,self.cp[i]);m.write(fd+20,struct.pack('<HH',widths[dims],heights[dims]));m.byte(fd+26,[0,1,2,3][(dims+i)%4]|0x10|(4 if (dims+i)%2 else 0))
  for i,md in enumerate(self.md):
   m.put(md,self.vt);m.put(md+4,count);m.put(md+0x20,self.tex[i]);m.put(md+0x24,self.cp[i]);m.put(md+0x30,b+0x1100+i*4);m.byte(md+0x49,0x80)
  for i,p in enumerate(self.cp):m.put(p,count);m.put(p+4,DELETER);m.put(p+8,self.tex[i])
  for i,t in enumerate(self.tex):m.put(t,self.com[i%2]);m.put(t+4,0x12345678+i)
  self.headers=[b+0x1300+i*0x20 for i in range(3)]
  self.volumes=[b+0x1400+i*0x20 for i in range(3)]
  self.volume_info=[b+0x1480+i*0x10 for i in range(3)]
  for i,md in enumerate(self.md):
   listp=b+0x1500+i*0x80;head=listp+4;node=b+0x1700+i*0x40
   m.put(md+8,0 if dims==0 else listp)
   m.put(md+12,node);m.put(md+16,0 if dims==2 else head)
   m.put(head+12,md);m.put(head+16,node);m.put(node+12,head);m.put(node+16,md if dims!=3 else head)
   m.put(md+0x28,self.volumes[i]);m.put(md+0x2c,self.volume_info[i] if dims%2 else 0);m.put(md+0x38,self.headers[i] if dims else 0)
   p=self.volume_info[i];m.put(p,count);m.put(p+4,DELETER);m.put(p+8,self.volumes[i])
  m.put(self.com[0],b+0x1200);m.put(b+0x1200+68,STOP+0x400)
  for i in range(16):m.put(self.manager+0x58c+i*4,count)
  m.put(self.manager+8,0 if not success and dims==0 else 4)
  for i,value in enumerate((0,256,65536,0xffffffff)):m.put(self.manager+12+i*4,value)
  m.write(0x129ba40,table)
  w,h=(5,9) if profile==1 else (widths[dims],heights[dims])
  for _ in range(reduction):w//=2;h//=2
  pf=STACK+0x300;m.put(pf,self.format_index)
  size=self.callback(0xa698f0,w,[h,1,pf]);index=self.callback(0xa69730,self.manager,[size])
  pool=self.manager+0x4c+(0 if index==MASK else index)*0x54;head=pool+4
  self.selected_pool=pool;self.owner_calls=0
  for i in range(4):
   p=self.manager+0x4c+i*0x54;sentinel=p+4
   m.put(p+0x3c,0 if not success and dims==1 else 1+extra%2)
   m.put(sentinel+12,sentinel);m.put(sentinel+16,sentinel)
  m.put(head+12,self.md[2]);m.put(head+16,self.md[2])
  m.put(self.md[2]+8,pool);m.put(self.md[2]+12,head);m.put(self.md[2]+16,head)
  m.byte(self.md[2]+0x48,1 if profile in (3,5) else 0)
  m.byte(self.md[2]+0x49,1 if profile==4 else 0);m.byte(self.md[2]+0x4a,1 if count%2 else 0)
  self.owners=[b+0x1000+i*0x40 for i in range(3)]
  for i,owner in enumerate(self.owners):m.put(owner,b+0x1100);m.put(self.md[i]+0x30,owner)
  m.put(b+0x1100+40,STOP+0x500)
  self.system=b+0x1280;self.display=b+0x1180;self.device=b+0x1288
  m.put(self.system+0x60,self.display);m.put(self.display+0x58,self.device);m.put(self.device,b+0x1200)
  m.put(b+0x1200+52,STOP+0x800);m.put(b+0x1200+8,STOP+0x600);m.put(b+0x1200+92,STOP+0x700)
  self.initial=m.read(ARENA,SIZE)
 def event(self,kind,*values):
  m=self.m;log=ARENA+0x3000+self.events*0x50
  assert self.events<40
  m.put(log,kind)
  for i,v in enumerate(values):m.put(log+4+i*4,v)
  snapshot=m.read(ARENA,SIZE)
  if self.expected is None:self.snapshots.append(snapshot)
  else:
   assert self.events<len(self.expected),(self.cfg,kind,'extra event')
   want=self.expected[self.events]
   if snapshot!=want:
    at=next(i for i,(a,b) in enumerate(zip(snapshot,want)) if a!=b)
    raise AssertionError((self.cfg,kind,self.events,hex(at),snapshot[at:at+16].hex(),want[at:at+16].hex()))
  self.events+=1
 def callback(self,address,selfp,args):
  m=self.m;p=self.profile
  if address==0x9f9df0:
   assert selfp==self.tex[0]
   if p==1:m.put(self.frame,self.fd[1]);m.write(self.fd[1]+20,struct.pack('<HH',5,9))
   self.event(1,selfp,self.levels);return self.levels
  if address==0x99a2d0:self.temp=selfp;m.put(selfp,0x1231710);return selfp
  if address==0x99a2e0:m.put(selfp,0x1231710);return 0
  if address==0x9f9d80:
   assert selfp==self.tex[0];out=args[0];m.put(out,self.format_index)
   if p==2:m.put(self.managed+4,self.md[1]);m.byte(self.fd[0]+26,3)
   self.event(2,selfp,self.format_index);return out
  if address==0xa69b70:
   assert selfp==self.manager;dest,w,h,levels,pf,owner=args
   assert m.get(pf)==self.format_index;self.temp=dest
   size=self.callback(0xa698f0,w,[h,1,pf]);index=self.callback(0xa69730,self.manager,[size])
   if index==0xffffffff:return 0
   selected=self.callback(0xa69830,self.manager+0x4c+index*0x54,[])
   if selected and self.callback(0xa697a0,selected,[w,h,levels,pf])&255:
    m.put(selected+0x30,owner)
    if m.get(dest+4):self.drop_intrusive(dest+4);m.put(dest+4,0)
    m.put(dest+4,selected);m.put(selected+4,m.get(selected+4)+1);return 1
   counter=self.manager+0x58c+index*4;m.put(counter,m.get(counter)+1);return 0
  if address==0xa698f0:
   h,mips,pf=args;assert mips==1
   row=m.get(pf);format,kind,bits=struct.unpack('<III',m.read(0x129ba40+row*36,12))
   minimum=4 if format in (0x31545844,0x33545844,0x35545844) else 1
   pixels=0;w=selfp
   while w or h:
    pixels=(pixels+max(w,minimum)*max(h,minimum))&MASK;w>>=1;h>>=1
   return ((pixels*bits)&MASK)>>3
  if address==0xa69730:
   assert selfp==self.manager;size=args[0]
   # Independent linear ceiling oracle for the strictly increasing case capacities.
   capacities=[m.get(self.manager+12+i*4) for i in range(m.get(self.manager+8))]
   return next((i for i,value in enumerate(capacities) if size<=value),MASK)
  if address==0xa69830:
   assert selfp==self.selected_pool
   i=0
   while i<m.get(selfp+0x3c):
    i+=1;selected=m.get(selfp+20)
    if not selected or selected==selfp+4:continue
    listp=m.get(selected+8)
    if listp:
     nxt=m.get(selected+16)
     if nxt:
      prev=m.get(selected+12)
      if m.get(prev+16)==selected and m.get(nxt+12)==selected:
       m.put(prev+16,nxt);m.put(nxt+12,prev)
      m.put(selected+12,0);m.put(selected+16,0)
     head=listp+4;prev=m.get(head+12)
     m.put(selected+16,head);m.put(selected+12,prev);m.put(prev+16,selected);m.put(head+12,selected)
    if m.byte(selected+0x4a):m.byte(selected+0x4a,0) # Actual PC IsBusy returns false.
    if m.byte(selected+0x49):continue
    m.byte(selected+0x49,1)
    if m.byte(selected+0x48):self.callback(STOP+0x500,m.get(selected+0x30),[])
    m.byte(selected+0x49,0)
    if not m.byte(selected+0x48):return selected
   return 0
  if address==STOP+0x500:
   assert selfp==self.owners[2]
   self.owner_calls+=1
   self.event(20,selfp,m.byte(self.md[2]+0x49),m.byte(self.md[2]+0x48))
   if p!=3:m.byte(self.md[2]+0x48,0)
   if p==5:m.put(self.managed+4,self.md[1]);m.byte(self.md[2]+0x49,1)
   return 0
  if address==0xa697a0:
   w,h,levels,pf=args;assert selfp==self.md[2] and m.get(pf)==self.format_index
   input=STACK+0x400;output=STACK+0x420;m.write(input,pack(w)+pack(h))
   self.callback(0x9fa620,output,[input,pf])
   x,y=struct.unpack('<II',m.read(output,8))
   m.write(selfp+0x1c,struct.pack('<HH',x&65535,y&65535))
   dimensions=STACK+0x440;m.write(dimensions,pack(x)+pack(y))
   result=self.callback(0x9fa280,m.get(selfp+0x20),[dimensions,MASK,pf,0,1,0,0])
   m.byte(selfp+0x48,result&255);return result&255
  if address==0x9fa620:
   input,pf=args;assert m.get(pf)==self.format_index
   w,h=struct.unpack('<II',m.read(input,8))
   # Rounding remains explicitly supplied by the unrecovered helper boundary.
   x,y=(w+1)&MASK,(h+2)&MASK
   m.write(selfp,pack(x)+pack(y));self.event(21,w,h,m.get(pf),x,y);return selfp
  if address==0x9fa280:
   dimensions,levels,pf,flags,pool,dynamic,automatic=args
   x,y=struct.unpack('<II',m.read(dimensions,8))
   assert selfp==self.tex[2] and (levels,flags,pool,dynamic,automatic)==(MASK,0,1,0,0)
   self.init_dims=dimensions;self.init_pf=pf
   if m.get(selfp):
    self.callback(STOP+0x600,0,[m.get(selfp)])
    m.put(selfp,0);m.put(selfp+4,m.get(selfp+4)&0x0fffffff)
   mip_count=1;w,h=x,y
   while w>=8 and h>=8:w>>=1;h>>=1;mip_count+=1
   system=self.callback(0x9a4ec0,0,[])
   display=m.get(system+0x60);device=m.get(display+0x58)
   x,y=struct.unpack('<II',m.read(dimensions,8));d3d_format=m.get(0x129ba40+m.get(pf)*36)
   status=self.callback(STOP+0x700,0,[device,x,y,mip_count,0,d3d_format,1,selfp,0])
   if status&0x80000000:return 0
   m.put(selfp+4,(m.get(selfp+4)&0x0fffffff)|0x10000000)
   self.callback(0x9f9ee0,selfp,[]);return 1
  if address==STOP+0x600:
   assert args==[self.com[0]];self.event(23,args[0]);return 0
  if address==0x9a4ec0:
   x,y=struct.unpack('<II',m.read(self.init_dims,8))
   self.event(24,x,y,m.get(self.init_pf),self.system)
   if p==2:m.write(self.init_dims,pack(8)+pack(16));m.put(self.init_pf,1)
   return self.system
  if address==STOP+0x700:
   device,x,y,levels,flags,format,pool,output,shared=args
   assert device==self.device and (flags,pool,output,shared)==(0,1,self.tex[2],0)
   self.created_format=format;m.put(output,self.com[0])
   if p==5:m.put(output+4,0x87654321)
   self.event(25,device,x,y,levels,format,self.success)
   return 0 if self.success else 0x80004005
  if address==0x9f9ee0:
   assert selfp==self.tex[2]
   m.put(selfp+4,m.get(selfp+4)&0xf0000000)
   if not m.get(selfp):return 0
   count=self.callback(STOP+0x800,0,[m.get(selfp)])
   for level in range(max(0,signed(count))):
    descriptor=STACK+0x500;base=STACK+0x540
    self.callback(STOP+0x400,0,[m.get(selfp),level,descriptor])
    self.callback(STOP+0x400,0,[m.get(selfp),0,base])
    format=m.get(base);row=next(i for i in range(46) if m.get(0x129ba40+i*36)==format)
    bits=m.get(0x129ba40+row*36+8)
    byte_count=((bits*m.get(descriptor+24)*m.get(descriptor+28))&MASK)>>3
    previous=m.get(selfp+4);m.put(selfp+4,(previous&0xf0000000)|((previous+byte_count)&0x0fffffff))
   return 0
  if address==STOP+0x800:
   assert args==[self.com[0]]
   count=MASK if p==5 else self.cfg[2]
   self.event(27,args[0],count);return count
  if address==0x9f9e00:
   out,level=args;is_dest=self.surface_calls%2==0
   assert selfp==(self.tex[2] if is_dest else self.tex[0]),(self.cfg,hex(selfp),is_dest)
   assert level==(self.copies if is_dest else self.copies+self.reduction)
   m.write(out,pack(0x122f84c)+pack(self.com[0 if is_dest else 1])+pack(0)+pack(0))
   self.surface_calls+=1;self.event(4,selfp,level,is_dest);return out
  if address==0x9f37e0:
   destbox,source,srcbox,filter=args
   assert destbox==srcbox and filter==1
   assert m.get(selfp+4)==self.com[0] and m.get(source+4)==self.com[1]
   coordinates=struct.unpack('<4I',m.read(destbox,16))
   if p==4:m.put(self.managed+4,self.md[1]);m.byte(self.fd[0]+26,2);m.put(self.frame,self.fd[1]);m.put(self.temp+4,self.md[1])
   self.event(5,*coordinates,filter);self.copies+=1;return 0
  if address==0x9f2e20:
   com=m.get(selfp+4);m.put(selfp+4,0);self.event(6,com);return 0
  if address==0x9fcb20:
   texture,w,h,alpha,balpha=args
   assert selfp==self.frame and m.get(texture)==self.tex[2]
   assert alpha in (0,1) and balpha in (0,1)
   data=self.callback(0xbfea1a,0,[27])
   m.put(data,1);m.put(data+8,0);m.put(data+12,0);m.byte(data+26,m.byte(data+26)&~16)
   m.put(self.frame,data)
   m.byte(data+26,m.byte(data+26)|4)
   incoming=m.get(texture);info=m.get(texture+4)
   if info:
    m.put(data+8,incoming);m.put(data+12,info);m.put(info,m.get(info)+1)
   m.write(data+20,struct.pack('<HHH',w&65535,h&65535,w&65535))
   m.byte(data+26,(m.byte(data+26)&0xf4)|(alpha&1)|((balpha&1)<<1)|16)
   m.write(data+16,bytes(4))
   output=STACK+0x200
   self.callback(0x9f9db0,m.get(texture),[output,0])
   different=struct.unpack('<H',m.read(data+20,2))[0]!=m.get(output)
   if not different:
    self.callback(0x9f9db0,m.get(texture),[output,0])
    different=struct.unpack('<H',m.read(data+22,2))[0]!=m.get(output)
   m.byte(data+26,(m.byte(data+26)&~32)|(32 if different else 0))
   return 1
  if address==0xbfea1a:
   assert args==[27];self.event(14,27,self.fd[2]);return self.fd[2]
  if address==STOP+0x400:
   interface,level,descriptor=args;assert interface==self.com[0] and level in range(3)
   x,y=257>>level,129>>level
   if p==2:
    m.write(self.fd[2]+20,struct.pack('<HH',257,257));m.byte(self.fd[2]+26,0xd7)
   m.write(descriptor,pack(self.created_format)+bytes(20)+pack(x)+pack(y))
   self.event(28,interface,level,self.created_format,x,y);return 0x80004005 if p==2 else 0
  if address==0x9f9db0:
   output,level=args;assert level==0 and selfp==self.tex[2]
   descriptor=STACK+0x600
   self.callback(STOP+0x400,0,[m.get(selfp),level,descriptor])
   m.write(output,m.read(descriptor+24,8));return output
  if address==0xa69a80:
   assert selfp in self.md
   m.byte(selfp+0x48,0);m.put(selfp+0x30,0)
   self.drop_shared(selfp+0x28)
   header=m.get(selfp+0x38);m.put(selfp+0x38,0);self.callback(0xbfe9bc,0,[header])
   listp=m.get(selfp+8)
   if listp:
    nxt=m.get(selfp+16)
    if nxt:
     prev=m.get(selfp+12)
     if m.get(prev+16)==selfp and m.get(nxt+12)==selfp:
      m.put(prev+16,nxt);m.put(nxt+12,prev)
     m.put(selfp+12,0);m.put(selfp+16,0)
    head=listp+4;nxt=m.get(head+16)
    m.put(selfp+12,head);m.put(selfp+16,nxt);m.put(head+16,selfp);m.put(nxt+12,selfp)
   return 0
  if address==0x9fcba0:
   assert selfp in self.fd;m.byte(selfp+26,m.byte(selfp+26)&~16)
   if m.byte(selfp+26)&4:self.drop_shared(selfp+8)
   return 0
  if address==DELETER:
   if p==5:
    for fd in self.fd:
     if m.get(fd+8)==selfp:m.put(fd+12,self.cp[1])
   if p==5 and selfp==self.volumes[0]:
    m.put(self.md[0]+0x2c,self.volume_info[1])
   self.event(10,selfp);return 0
  if address==VDELETE:
   if p==5 and selfp==self.md[0] and self.temp:m.put(self.temp+4,self.md[1])
   self.event(11,selfp);return 0
  if address==0xbfe9bc:
   if p==5 and args[0]==self.headers[0]:m.put(self.managed+4,self.md[1])
   self.event(12,args[0]);return 0
  raise AssertionError(hex(address))
 def drop_intrusive(self,slot):
  m=self.m;data=m.get(slot)
  if data:
   n=(m.get(data+4)-1)&MASK;m.put(data+4,n)
   if n==0:self.callback(VDELETE,data,[])
   m.put(slot,0)
 def release_managed(self,receiver):
  m=self.m;data=m.get(receiver+4)
  if not data:return
  if m.get(data+4)==2:self.callback(0xa69a80,data,[])
  self.drop_intrusive(receiver+4);m.put(receiver+4,0)
 def destroy_managed(self,receiver):
  m=self.m;m.put(receiver,0x129c880);data=m.get(receiver+4)
  if data and m.get(data+4)==2:self.release_managed(receiver)
  self.drop_intrusive(receiver+4);self.callback(0x99a2e0,receiver,[])
 def destroy_frame_data(self,data):
  m=self.m
  if m.byte(data+26)&16:self.callback(0x9fcba0,data,[])
  self.drop_shared(data+8);self.callback(0xbfe9bc,0,[data])
 def drop_shared(self,receiver):
  m=self.m;info=m.get(receiver+4)
  if info:
   m.put(info,m.get(info)-1)
   if m.get(m.get(receiver+4))==0:
    info=m.get(receiver+4);self.callback(DELETER,m.get(info+8),[]);self.callback(0xbfe9bc,0,[m.get(receiver+4)])
  m.put(receiver,0);m.put(receiver+4,0)
 def model(self):
  m=self.m;scratch=STACK+0x100
  oldtexture=m.get(m.get(self.frame)+8);levels=self.callback(0x9f9df0,oldtexture,[])-self.reduction
  fd=m.get(self.frame);w,h=struct.unpack('<HH',m.read(fd+20,4))
  for _ in range(self.reduction):w//=2;h//=2
  m.byte(m.get(self.managed+4)+0x49,1)
  temp=scratch;self.callback(0x99a2d0,temp,[]);m.put(temp,0x129c880);m.put(temp+4,0)
  owner=m.get(m.get(self.managed+4)+0x30)
  pf=scratch+0x20;self.callback(0x9f9d80,oldtexture,[pf])
  ok=self.callback(0xa69b70,self.manager,[temp,w,h,levels,pf,owner])
  if not ok:
   m.byte(m.get(self.managed+4)+0x49,0);self.destroy_managed(temp);return 0
  shared=scratch+0x30;md=m.get(temp+4);m.put(shared,m.get(md+0x20));m.put(shared+4,m.get(md+0x24))
  info=m.get(shared+4)
  if info:m.put(info,m.get(info)+1)
  box=scratch+0x40;m.write(box,struct.pack('<4I',0,0,w,h))
  dest=scratch+0x60;src=scratch+0x80
  for level in range(levels):
   self.callback(0x9f9e00,m.get(shared),[dest,level]);self.callback(0x9f9e00,oldtexture,[src,level+self.reduction])
   self.callback(0x9f37e0,dest,[box,src,box,1]);m.put(box+8,half(m.get(box+8)));m.put(box+12,half(m.get(box+12)))
   self.callback(0x9f2e20,src,[]);self.callback(0x9f2e20,dest,[])
  previous=m.get(self.frame)
  if previous:m.put(previous,m.get(previous)+1)
  live=m.get(self.frame)
  if live:
   m.put(live,m.get(live)-1)
   if m.get(live)==0:self.destroy_frame_data(live)
   m.put(self.frame,0)
  flags=m.byte(previous+26)
  self.callback(0x9fcb20,self.frame,[shared,w,h,flags&1,(flags>>1)&1])
  m.put(m.get(self.managed+4)+0x30,0);m.byte(m.get(self.managed+4)+0x49,0)
  if m.get(self.managed+4)!=m.get(temp+4):
   self.release_managed(self.managed)
   if m.get(self.managed+4)!=m.get(temp+4):
    self.drop_intrusive(self.managed+4);m.put(self.managed+4,m.get(temp+4))
    data=m.get(self.managed+4)
    if data:m.put(data+4,m.get(data+4)+1)
  m.put(previous,m.get(previous)-1)
  if m.get(previous)==0:self.destroy_frame_data(previous)
  self.drop_shared(shared);self.destroy_managed(temp);return 1

CALL_ARGS={0x9f9df0:0,0x99a2d0:0,0x99a2e0:0,0x9f9d80:1,0xa69b70:6,0xa69a80:0,0x9f9e00:2,0x9f37e0:4,0x9f2e20:0,0x9fcb20:5,0x9fcba0:0,0xbfe9bc:1,DELETER:0,VDELETE:0,0xbfea1a:1,0x9f9db0:2}
CALL_ARGS.pop(0x9fcb20)
# Size/index now execute their complete recovered bodies.
CALL_ARGS.pop(0xa69b70)
CALL_ARGS.update({0xa698f0:2,0xa69730:1,0xa69830:0,0xa697a0:4})
CALL_ARGS.pop(0xa69a80)
CALL_ARGS.pop(0x9fcba0)
CALL_ARGS.pop(0x9f9db0)
CALL_ARGS[STOP+0x400]=3
CALL_ARGS.pop(0xa698f0)
CALL_ARGS.pop(0xa69730)
CALL_ARGS.pop(0xa69830)
CALL_ARGS.pop(0xa697a0)
CALL_ARGS.update({0x9fa620:1,0x9fa280:7})
CALL_ARGS[STOP+0x500]=0
CALL_ARGS.pop(0x9fa280)
CALL_ARGS.update({STOP+0x600:1,STOP+0x700:9,0x9a4ec0:0,STOP+0x800:1})
def make_uc(code):
 u=Uc(UC_ARCH_X86,UC_MODE_32)
 for base,size in [(0x900000,0x400000),(0x1200000,0x100000),(0x1300000,0x100000),(ARENA,0x4000),(STACK,0x10000),(STOP,0x1000)]:u.mem_map(base,size)
 u.mem_write(ENTRY,bytes(code))
 for address,body in (helper_original if bytes(code)==original else helper_built).items():u.mem_write(address,body)
 return u
native=make_uc(original);built=make_uc(patched)
current={}
def hook(u,address,size,user):
 if address==0x9fa280:
  case=current[id(u)];sp=u.reg_read(UC_X86_REG_ESP)
  case.init_dims=case.m.get(sp+4);case.init_pf=case.m.get(sp+12)
  return
 if address==STOP:u.emu_stop();return
 if address not in CALL_ARGS:return
 case=current[id(u)];sp=u.reg_read(UC_X86_REG_ESP);count=CALL_ARGS[address]
 args=[case.m.get(sp+4+i*4) for i in range(count)]
 if address in (0xa698f0,0x9fa620):args=[u.reg_read(UC_X86_REG_EDX)]+args
 value=case.callback(address,u.reg_read(UC_X86_REG_ECX),args)
 target=case.m.get(sp);cleanup=0 if address in (0xbfe9bc,0xbfea1a) else (8 if address==0xa698f0 else 4 if address==0x9fa620 else count*4)
 u.reg_write(UC_X86_REG_EAX,value&MASK);u.reg_write(UC_X86_REG_ESP,sp+4+cleanup);u.reg_write(UC_X86_REG_EIP,target)
for u in (native,built):u.hook_add(UC_HOOK_CODE,hook)
cases=snapshots=0
from itertools import product
for cfg in product(range(4),range(3),range(3),range(2),(0,1,2,0xffffffff),range(6),range(2),range(2)):
 model=Case(Memory(),cfg);expected=model.model();model.event(13,expected)
 for u in (native,built):
  case=Case(Memory(u),cfg,model.snapshots);current[id(u)]=case
  sp=STACK+0xff00;case.m.write(sp,b''.join(pack(v) for v in (STOP,case.frame,case.managed,case.reduction)))
  u.reg_write(UC_X86_REG_ESP,sp);u.reg_write(UC_X86_REG_ECX,case.manager);u.reg_write(UC_X86_REG_EDX,0)
  saved={reg:0x12345678+reg for reg in (UC_X86_REG_EBX,UC_X86_REG_EBP,UC_X86_REG_ESI,UC_X86_REG_EDI)}
  for reg,value in saved.items():u.reg_write(reg,value)
  u.emu_start(ENTRY,STOP,count=20000)
  assert u.reg_read(UC_X86_REG_EIP)==STOP,('instruction limit',cfg)
  result=u.reg_read(UC_X86_REG_EAX)&255;assert result==expected,(cfg,result,expected)
  case.event(13,result);assert case.events==len(model.snapshots),(cfg,case.events,len(model.snapshots))
  assert u.reg_read(UC_X86_REG_ESP)==sp+16
  assert all(u.reg_read(reg)==value for reg,value in saved.items())
  snapshots+=case.events
 cases+=1
 if cases%1000==0:print('progress',cases,flush=True)
import re
sources=[SOURCE,TABLE_SOURCE,REUSE_SOURCE]+[row[0] for row in HELPERS]
pending=list(sources);seen=set(sources);headers=[]
while pending:
 path=pending.pop()
 for match in re.finditer(r'^\s*#include\s*"([^"]+)"',path.read_text(encoding='utf-8'),re.M):
  relative=match.group(1);candidates=(path.parent/relative,R/'rebuild/include'/relative)
  dependency=next((p.resolve() for p in candidates if p.is_file()),None)
  if dependency is not None and dependency not in seen:
   seen.add(dependency);headers.append(dependency);pending.append(dependency)
report=dict(cases=cases,snapshots=snapshots,arena_bytes=SIZE,built_bytes=len(compiled),retail_bytes=955,relocations=len(rel),parity=score.to_dict(),omission_checks=20,source_sha256={str(p.relative_to(R)):hashlib.sha256(p.read_bytes()).hexdigest() for p in [SOURCE,TABLE_SOURCE,REUSE_SOURCE]+[row[0] for row in HELPERS]+headers},fixture_sha256=hashlib.sha256(Path(__file__).read_bytes()).hexdigest(),original_sha256=hashlib.sha256(original).hexdigest(),boundary='Root-only actual native/compiler execution under bounded x86 emulator; independent model and full16KiB event/final snapshots. Twenty mandatory sources execute mip root, frame/data initialization and cleanup, texture dimensions, managed release, LRU, allocation, sizing, colour depth/table, pool lookup/selection, MRU, PCbusy/reuse and managed creation. Actual texture initialization, byte-length accounting and pixel-format lookup add mip defaults, packed allocation state, signed level count, two descriptor queries per mip and COM creation. Owner release, deferred next-valid-dimensions, system accessor and surface operations, base lifetime, stored/virtual/heap deletion and COM descriptor remain explicit boundaries; no realheap/GPU/new coverage. Pool cases are bounded singleton lists, count0/1/2, busy clearing and locked/release-leave/release-clear ownership; standalone selection gate retains wider list coverage.',direct_call_targets=sorted(set(calls)),helper_proofs=helper_proofs,data_proofs=[dict(address='0129ba40',bytes=1692,sha256=hashlib.sha256(table).hexdigest()),dict(address='013d28e8',bytes=1,sha256=hashlib.sha256(reuse).hexdigest())])
(R/'rebuild/compile-gate/native-texture-manager-reduce-mipmap-byte-accounting.json').write_text(json.dumps(report,indent=2)+'\n')
print('NATIVE_TEXTURE_MANAGER_REDUCE_MIPMAP_BYTE_ACCOUNTING_GATE_PASS',cases,snapshots)
