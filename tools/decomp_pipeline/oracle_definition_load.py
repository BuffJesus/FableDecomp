"""Retail definition load orchestration, with resource/stream boundaries."""
import struct
from unicorn import Uc,UC_ARCH_X86,UC_MODE_32,UC_HOOK_CODE
from unicorn.x86_const import UC_X86_REG_EAX,UC_X86_REG_ECX,UC_X86_REG_EDX,UC_X86_REG_ESP,UC_X86_REG_EIP
from pe_oracle import EXE,pe_sections
KINDS=['loaded-unlisted','loaded-current','loaded-stale','unknown-class','new','replacement','no-free-space']
def collect():
 raw=EXE.read_bytes();rows=[]
 for kind in KINDS:
  u=Uc(UC_ARCH_X86,UC_MODE_32);u.mem_map(0x400000,0x1200000)
  for _,va,vs,off,size in pe_sections(raw):
   if size:u.mem_write(va+0x400000,raw[off:off+size])
  u.mem_map(0x2000000,0x100000);manager=0x2001000;entry=0x2002000;cls=0x2003000;definition=0x2004000;old=0x2005000;table=0x2006000;stack=0x2010000;stop=0x200f000;heap=[0x2020000];events=[]
  def r(p):return struct.unpack('<I',u.mem_read(p,4))[0]
  def w(p,v):u.mem_write(p,struct.pack('<I',v&0xffffffff))
  def cs(p):
   value=bytearray()
   while u.mem_read(p+len(value),1)[0]:value+=u.mem_read(p+len(value),1)
   return bytes(value)
  def string(p):return cs(r(r(p))) if r(p) else b''
  def assign(p,value):
   block=heap[0];heap[0]+=32+len(value);u.mem_write(block,bytes(20));w(block,block+20);w(block+4,len(value));u.mem_write(block+20,value+b'\0');w(p,block)
  def ret(v=0,pop=0):
   sp=u.reg_read(UC_X86_REG_ESP);dest=r(sp);u.reg_write(UC_X86_REG_EAX,v&0xffffffff);u.reg_write(UC_X86_REG_ESP,sp+4+pop);u.reg_write(UC_X86_REG_EIP,dest)
  def hook(uc,a,size,user):
   sp=uc.reg_read(UC_X86_REG_ESP);self=uc.reg_read(UC_X86_REG_ECX)
   if a==stop:uc.emu_stop()
   elif a==0x99ebf0:
    ptr=r(sp+4);length=r(sp+8);value=cs(ptr);assign(self,value if length==0xffffffff else value[:length]);ret(self,8)
   elif a==0x99eae0:ret()
   elif a==0x9e9f40:assert r(sp+4)==0xbf800000 and r(sp+8)==0 and not(uc.reg_read(UC_X86_REG_EDX)&255);events.append(('progress',string(self).decode()));ret(0,8)
   elif a==0x993b20:u.mem_write(self,bytes(28));ret(self)
   elif a==0x993b10:events.append('stream-destroy');ret()
   elif a==0x9ad5f0:
    assert r(sp+8)==1;w(r(sp+4)+8,100);events.append('decompress')
    if kind=='replacement':w(entry+12,old)
    ret(0,8)
   elif a==0x9d49b0:assert r(sp+8)==100;assign(r(sp+4),b'CLASS');ret(r(sp+4),8)
   elif a==0x9ad2e0:assert string(r(sp+4))==b'CLASS';events.append('class-lookup');ret(0 if kind=='unknown-class' else cls,4)
   elif a==0x9fc4f0:assert r(sp+4)==137;events.append('make-space');ret(kind!='no-free-space',4)
   elif a==0x9fc150:w(manager+44,r(manager+44)+1);events.append('frame-end');ret()
   elif a==0x200f100:events.append('allocate');ret(definition)
   elif a==0x200f200:assert r(sp+4)==17;events.append('name');ret(0,4)
   elif a==0x200f300:
    assert r(self+28)==manager and r(self+32)==1 and r(r(sp+4)+8)==100;events.append('internal-load');ret(0,4)
   elif a==0x200f400:
    context=r(sp+4);assert r(context+24)==2 and r(r(context+36)+8)==100 and r(context)==0 and u.mem_read(context+21,1)[0]==0;events.append('transfer');ret(0,4)
   elif a==0x9fbf20:assert r(sp+4)==137;w(self+20,137);events.append('size');ret(0,4)
   elif a==0x9fbfe0:
    resource=r(sp+4);w(resource+8,manager);w(resource+24,r(manager+44));events.append('add-resource');ret(0,4)
   elif a==0x9fbfb0:events.append('list-remove');ret()
   elif a==0x9fbf90:assert r(sp+4)==0x2007000 and r(sp+8)==manager+4;events.append('list-insert');ret(0,8)
  u.mem_write(manager,bytes(0xd0));w(manager+0xa4,0x2008000);w(0x2008004,entry);w(manager+16,0x2007000);w(manager+44,7);u.mem_write(entry,struct.pack('<IIII',100,17,0,definition if kind.startswith('loaded-') else 0));u.mem_write(cls,bytes(20));w(cls+4,0x200f100)
  for obj in (definition,old):u.mem_write(obj,bytes(40));w(obj,table);w(obj+4,1 if obj==definition else 2)
  if kind in ('loaded-current','loaded-stale'):w(definition+8,manager);w(definition+24,7 if kind=='loaded-current' else 3)
  u.mem_write(table,bytes(100));w(table+44,0x200f200);w(table+88,0x200f300);w(table+72,0x200f400);u.mem_write(stack,struct.pack('<II',stop,1));u.reg_write(UC_X86_REG_ESP,stack);u.reg_write(UC_X86_REG_ECX,manager);u.hook_add(UC_HOOK_CODE,hook);u.emu_start(0x9ad6e0,stop,count=500000)
  if kind.startswith('loaded-'):assert events==(['list-remove','list-insert'] if kind=='loaded-stale' else [])
  elif kind=='unknown-class':assert events==[('progress','CDefinitionManager::LoadDef 1'),'decompress','class-lookup','stream-destroy'] and r(entry+12)==0
  else:
   assert events==[('progress','CDefinitionManager::LoadDef 1'),'decompress','class-lookup','make-space','frame-end','allocate','name','internal-load',('progress','CDefinitionManager::LoadDef 2'),'transfer','size','add-resource',('progress','CDefinitionManager::LoadDef 3'),'stream-destroy'],events
   assert r(entry+12)==definition and r(definition+4)==1 and r(definition+20)==137 and r(definition+24)==8 and r(old+4)==(1 if kind=='replacement' else 2)
  rows.append(dict(kind=kind,events=events,frame=r(manager+44),last_used=r(definition+24)))
 return rows
if __name__=='__main__':print('RETAIL_LOAD_DEFINITION_PASS cases='+str(len(collect())))
