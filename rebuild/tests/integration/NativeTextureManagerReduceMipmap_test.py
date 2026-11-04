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

# Link-only omission check: the fixture has no substitute for the tested root.
link_source=BUILD/'required_root.cpp';link_obj=BUILD/'required_root.obj'
aliases=['#pragma comment(linker,"/alternatename:_MustHaveRoot='+ROOT_SYMBOL+'")']
aliases+=['#pragma comment(linker,"/alternatename:'+symbol+'=_UnusedBoundary")' for symbol in TARGETS if not symbol.startswith('??_7')]
link_source.write_text('#include <stdlib.h>\nextern "C" void UnusedBoundary(){abort();}\nextern "C" void MustHaveRoot();\nint main(){MustHaveRoot();return 0;}\n'+'\n'.join(aliases)+'\n')
res=V.cl(['/nologo','/c','/O2','/MT','/Fo'+str(link_obj),str(link_source)],env,BUILD)
assert not res.returncode,res.stdout+res.stderr
link=[str(V.VC/'bin/link.exe'),'/nologo','/out:'+str(BUILD/'required_root.exe'),str(link_obj)]
res=subprocess.run(link,env=env,capture_output=True,text=True)
assert res.returncode and ('LNK2001' in res.stdout or 'LNK2019' in res.stdout),res.stdout+res.stderr
res=subprocess.run(link+[str(OBJ)],env=env,capture_output=True,text=True)
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
  self.profile=profile;self.success=success;self.reduction=reduction;self.levels=reduction+extra
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
   m.put(fd,count);m.put(fd+8,self.tex[i]);m.put(fd+12,self.cp[i]);m.write(fd+20,struct.pack('<HH',widths[dims],heights[dims]));m.byte(fd+26,[0,1,2,3][(dims+i)%4]|0x10)
  for i,md in enumerate(self.md):
   m.put(md,self.vt);m.put(md+4,count);m.put(md+0x20,self.tex[i]);m.put(md+0x24,self.cp[i]);m.put(md+0x30,b+0x1100+i*4);m.byte(md+0x49,0x80)
  for i,p in enumerate(self.cp):m.put(p,count);m.put(p+4,DELETER);m.put(p+8,self.tex[i])
  for i,t in enumerate(self.tex):m.put(t,self.com[i%2]);m.put(t+4,0x12345678+i)
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
  if address==0x99a2d0:m.put(selfp,0x1231710);return selfp
  if address==0x99a2e0:m.put(selfp,0x1231710);return 0
  if address==0x9f9d80:
   assert selfp==self.tex[0];out=args[0];m.put(out,7)
   if p==2:m.put(self.managed+4,self.md[1]);m.byte(self.fd[0]+26,3)
   self.event(2,selfp,7);return out
  if address==0xa69b70:
   assert selfp==self.manager;dest,w,h,levels,pf,owner=args
   assert m.get(pf)==7;self.temp=dest;m.put(dest+4,self.md[2])
   if p==3:m.put(self.managed+4,self.md[1]);m.put(self.frame,self.fd[1])
   self.event(3,w,h,levels,m.get(pf),owner,self.success);return self.success
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
   m.put(self.frame,self.fd[2]);self.event(7,m.get(texture),w,h,alpha,balpha);return 0
  if address==0xa69a80:
   assert selfp in self.md
   m.byte(selfp+0x48,0)
   if p==5 and selfp==self.md[0]:m.put(self.managed+4,self.md[1])
   self.event(8,selfp);return 0
  if address==0x9fcba0:
   assert selfp in self.fd;m.byte(selfp+26,m.byte(selfp+26)&~16)
   self.event(9,selfp);return 0
  if address==DELETER:
   if p==5:
    for fd in self.fd:
     if m.get(fd+8)==selfp:m.put(fd+12,self.cp[1])
   self.event(10,selfp);return 0
  if address==VDELETE:
   if p==5 and selfp==self.md[0] and self.temp:m.put(self.temp+4,self.md[1])
   self.event(11,selfp);return 0
  if address==0xbfe9bc:self.event(12,args[0]);return 0
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

CALL_ARGS={0x9f9df0:0,0x99a2d0:0,0x99a2e0:0,0x9f9d80:1,0xa69b70:6,0xa69a80:0,0x9f9e00:2,0x9f37e0:4,0x9f2e20:0,0x9fcb20:5,0x9fcba0:0,0xbfe9bc:1,DELETER:0,VDELETE:0}
def make_uc(code):
 u=Uc(UC_ARCH_X86,UC_MODE_32)
 for base,size in [(0x900000,0x400000),(0x1200000,0x100000),(ARENA,0x4000),(STACK,0x10000),(STOP,0x1000)]:u.mem_map(base,size)
 u.mem_write(ENTRY,bytes(code));return u
native=make_uc(original);built=make_uc(patched)
current={}
def hook(u,address,size,user):
 if address==STOP:u.emu_stop();return
 if address not in CALL_ARGS:return
 case=current[id(u)];sp=u.reg_read(UC_X86_REG_ESP);count=CALL_ARGS[address]
 args=[case.m.get(sp+4+i*4) for i in range(count)]
 value=case.callback(address,u.reg_read(UC_X86_REG_ECX),args)
 target=case.m.get(sp);cleanup=0 if address==0xbfe9bc else count*4
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
headers=[R/'rebuild/include'/name for name in ('engine/CTextureManager.h','fable_surface_types.h','fable_graphic_frame.h','fable_graphic_frame_data.h','fable_managed_texture.h','fable_managed_texture_data.h','fable_counted_pointer.h','fable_iv_counted_pointer.h')]
report=dict(cases=cases,snapshots=snapshots,arena_bytes=SIZE,built_bytes=len(compiled),retail_bytes=955,relocations=len(rel),parity=score.to_dict(),omission_checks=1,source_sha256={str(p.relative_to(R)):hashlib.sha256(p.read_bytes()).hexdigest() for p in [SOURCE]+headers},fixture_sha256=hashlib.sha256(Path(__file__).read_bytes()).hexdigest(),original_sha256=hashlib.sha256(original).hexdigest(),boundary='Root-only actual native/compiler execution under bounded x86 emulator; independent model and full16KiB event/final snapshots. All child methods, C++ allocation deletion and COM operations are explicit functional boundaries; no realheap/GPU claim.',direct_call_targets=sorted(set(calls)))
(R/'rebuild/compile-gate/native-texture-manager-reduce-mipmap.json').write_text(json.dumps(report,indent=2)+'\n')
print('NATIVE_TEXTURE_MANAGER_REDUCE_MIPMAP_GATE_PASS',cases,snapshots)
