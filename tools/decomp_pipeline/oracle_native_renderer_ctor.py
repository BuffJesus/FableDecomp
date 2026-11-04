"""Renderer constructor retail replay; engine and ownership callbacks remain boundaries."""
import itertools,struct
from unicorn import Uc,UC_ARCH_X86,UC_MODE_32,UC_HOOK_CODE
from unicorn.x86_const import *
from pe_oracle import EXE,pe_sections
BASE=0x24000000;WIDTH=0x6000;BANK=0x13ca79c;SHADER=0x13bc470

def collect():
 u=Uc(UC_ARCH_X86,UC_MODE_32);u.mem_map(0x400000,0x1200000);u.mem_map(BASE,0x10000)
 raw=EXE.read_bytes()
 for _,va,_,off,n in pe_sections(raw):
  if n:u.mem_write(0x400000+va,raw[off:off+n])
 inputs=bytearray();expected=bytearray();cases=snapshots=0
 def get(a):return struct.unpack('<I',u.mem_read(a,4))[0]
 def put(a,v):u.mem_write(a,struct.pack('<I',v&0xffffffff))
 def event(kind,a=0,b=0,c=0):
  nonlocal snapshots
  state=bytearray(u.mem_read(BASE,WIDTH))
  for off in [0,0x3d20,0x3e3c,0x3e50,0x5404]:state[off:off+4]=b'\0'*4
  expected.extend(struct.pack('<4I',kind,a,b,c));expected.extend(state)
  expected.extend(u.mem_read(BANK,0x38));expected.extend(u.mem_read(SHADER,0x3dc));snapshots+=1
 addresses={0xa0afe0:1,0xa62020:2,0xa62fb0:3,0x99b6b0:4,0x99be70:5,0x99bf30:6,0x9a8840:7,0x99b510:8,0x99ebf0:9,0x9a80b0:10,BASE+0x9100:11,0x98b860:12,0x9de7c0:13,BASE+0x9140:14,0xbfe9bc:15,0xa5d7c0:16,0xa5d4e0:17,0xa5d4f0:18,0xa5d5f0:19,BASE+0x9180:20,0x99eae0:21}
 def hook(u,a,n,_):
  if a not in addresses:return
  kind=addresses[a];sp=u.reg_read(UC_X86_REG_ESP);this=u.reg_read(UC_X86_REG_ECX);cleanup=0;result=0
  if kind==1:
   assert this==BASE and get(sp+4)==BASE+0x4000 and get(sp+8)==flags
   event(1,this,get(sp+4),flags);u.mem_write(BASE,b'\x5a'*0x3d20);result=this;cleanup=8
  elif kind==2:
   assert this==BASE+0x3d20;event(2,this);u.mem_write(this,b'\x6b'*0x128);put(this,BASE+0x9000);result=this
  elif kind==3:
   assert this==BASE+0x3e50;event(3,this);put(this,0x129b3d0);put(this+4,0);result=this
  elif kind==4:
   assert bytes(u.mem_read(get(sp+4),8))=='pc\\\0'.encode('utf-16le');event(4,0x10);put(this,0x10);result=this;cleanup=4
  elif kind==5:
   left=u.reg_read(UC_X86_REG_EDX);right=get(sp+4);assert left==BASE+0x5500 and get(right)==0x10
   event(5,get(left),get(right));put(this,0x20);result=this;cleanup=4
  elif kind==6:
   left=u.reg_read(UC_X86_REG_EDX);assert get(left)==0x20 and bytes(u.mem_read(get(sp+4),24))=='shaders.big\0'.encode('utf-16le')
   event(6,0x20);put(this,0x30);result=this;cleanup=4
  elif kind==7:
   assert this==BANK and get(get(sp+4))==0x30 and get(sp+8)==0
   event(7,this,0x30);cleanup=8
   if mode:put(BASE+0x5500,0x77777777)
  elif kind==8:event(8,get(this));put(this,0)
  elif kind==9:
   text=get(sp+4);token=0x40 if bytes(u.mem_read(text,len(b'SHADERS_BBBLIB\0')))==b'SHADERS_BBBLIB\0' else 0x50
   if token==0x50:assert bytes(u.mem_read(text,len(b'VSHADER_BBBLIB_2D\0')))==b'VSHADER_BBBLIB_2D\0'
   assert get(sp+8)==0xffffffff;event(9,token);put(this,token);cleanup=8;result=this
   if token==0x50 and mode==2:put(BASE+0x3e48,BASE+0x5300)
  elif kind==10:
   assert this==BANK and get(get(sp+4))==0x40;event(10,this,0x40);cleanup=4;result=exists
  elif kind==11:
   assert this==BASE+0x3d20 and get(get(sp+4))==0x40 and get(sp+8)==0x48
   event(11,this,0x40,0x48);cleanup=8;result=0
  elif kind==12:
   out=get(sp+4);assert this==SHADER;event(12,this);put(out,BASE+0x5200);put(out+4,BASE+0x5400 if ownership else 0);result=out;cleanup=4
  elif kind==13:
   other=get(sp+4);assert this==BASE+0x3e48;data=get(other);info=get(other+4)
   event(13,this,data,info);put(this,data);put(this+4,info)
   if info and mode!=1:put(info,get(info)+1)
   cleanup=4;result=this
  elif kind==14:
   assert this==BASE+0x5200;event(14,this)
   if mode:put(BASE+0x3e48,BASE+0x5300)
  elif kind==15:
   value=get(sp+4);assert value==BASE+0x5400;event(15,value)
  elif kind==16:
   assert this in [BASE+0x5200,BASE+0x5300] and get(sp+4)==BASE+0x3d20;event(16,this,get(sp+4));cleanup=4
   if mode==1:put(BASE+0x3e48,BASE+0x5300)
  elif kind==17:
   assert this in [BASE+0x5200,BASE+0x5300] and get(sp+4)==0x129d708;event(17,this,get(sp+4));cleanup=4
   if mode==2:put(BASE+0x3e48,BASE+0x5200)
  elif kind==18:
   assert this in [BASE+0x5200,BASE+0x5300] and get(sp+4)==0x1c4;event(18,this,0x1c4);cleanup=4
  elif kind==19:
   out=get(sp+4);assert this in [BASE+0x5200,BASE+0x5300] and get(get(sp+8))==0x50
   event(19,this,0x50);put(out,0x129b3d0);put(out+4,BASE+0x5040);put(BASE+0x3e54,BASE+0x5000 if old else 0);result=out;cleanup=8
  elif kind==20:
   assert this in [BASE+0x5000,BASE+0x5040] and get(sp+4)==1;event(20,this,1);cleanup=4;result=this
   if this==BASE+0x5000 and mode==2:put(BASE+0x5050,get(BASE+0x5050)^0x100)
  else:event(21,get(this));put(this,0)
  u.reg_write(UC_X86_REG_EAX,result);u.reg_write(UC_X86_REG_EIP,get(sp));u.reg_write(UC_X86_REG_ESP,sp+4+cleanup)
 u.hook_add(UC_HOOK_CODE,hook);put(BASE+0x9004,BASE+0x9100);put(BASE+0x9200,BASE+0x9180)
 for flags,retail,exists,ownership,old,mode,refs in itertools.product([0,1,2,0xffffffff],[0,1],[0,1],[0,1,2],[0,1,2],range(3),[0,2,0xffffffff]):
  arena=bytearray((i*17+cases*13)&255 for i in range(WIDTH))
  def word(o,v):struct.pack_into('<I',arena,o,v&0xffffffff)
  word(0x5000,BASE+0x9200);word(0x5010,old);word(0x5040,BASE+0x9200);word(0x5050,refs)
  word(0x5400,ownership);word(0x5404,BASE+0x9140);word(0x5408,BASE+0x5200);word(0x5500,0xabcdef01)
  bank=bytearray(b'\x4a'*0x38);bank[0x14]=retail;shader=b'\x7b'*0x3dc
  inputs.extend(struct.pack('<7I',flags,retail,exists,ownership,old,mode,refs));inputs.extend(arena);inputs.extend(bank);inputs.extend(shader)
  u.mem_write(BASE,bytes(arena));u.mem_write(BANK,bytes(bank));u.mem_write(SHADER,shader)
  sp=BASE+0xe000;stop=BASE+0xf000;u.mem_write(sp,struct.pack('<4I',stop,BASE+0x4000,BASE+0x5500,flags));u.reg_write(UC_X86_REG_ESP,sp);u.reg_write(UC_X86_REG_ECX,BASE)
  u.emu_start(0x9d9a30,stop,count=10000);assert u.reg_read(UC_X86_REG_ESP)==sp+16 and u.reg_read(UC_X86_REG_EAX)==BASE
  assert bytes(u.mem_read(BASE+0x3ea0,1))==bytes([flags&1]);event(22);cases+=1
 return inputs,expected,dict(cases=cases,snapshots=snapshots)
