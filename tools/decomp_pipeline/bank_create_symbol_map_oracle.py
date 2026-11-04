"""Compare bank enumeration and string lifetime at explicit dependency boundaries."""
import struct
from unicorn import Uc,UC_ARCH_X86,UC_MODE_32,UC_HOOK_CODE
from unicorn.x86_const import *
BANK=0x2000000;MAP=BANK+0x200;VT=BANK+0x400;SYMBOLS=BANK+0x500;RUNTIME=BANK+0x600;TEXT=BANK+0x1000;STOP=0x2200000;ENUM=STOP+16;STACK=0x3000000
KEYS=[b'zero',b'one',b'[two]',b'three[',b'',b'five',b'six',b'seven']
def execute(code,n,packed,missing,append,mode):
 u=Uc(UC_ARCH_X86,UC_MODE_32)
 for a,z in [(0x990000,0x60000),(0xa60000,0x10000),(0x129a000,0x1000),(BANK,0x10000),(STOP,0x1000),(STACK,0x10000)]:u.mem_map(a,z)
 u.mem_write(0x9cc530,bytes(code));u.mem_write(0x129a03c,b'(Unknown)\0')
 def put(a,v):u.mem_write(a,struct.pack('<I',v&0xffffffff))
 def get(a):return int.from_bytes(u.mem_read(a,4),'little')
 def text(a):
  out=b''
  while u.mem_read(a,1)!=b'\0':out+=bytes(u.mem_read(a,1));a+=1
  return out
 put(BANK,VT);put(VT+76,ENUM);put(BANK+4,n);put(BANK+8,SYMBOLS);put(BANK+12,SYMBOLS+(0 if missing else n*4));put(BANK+32,RUNTIME);put(BANK+0x6c,packed&1);put(BANK+0x5c,packed&2)
 for i,t in enumerate(KEYS):
  put(SYMBOLS+4*i,TEXT+64*i);u.mem_write(TEXT+64*i,t+b'\0');u.mem_write(RUNTIME+12*i+8,bytes([i*13,int(i%3!=0)]))
 live=set();events=[];added=[]
 def hook(u,a,z,data):
  if a not in (0x9e5250,0xa629f0,0x99ebf0,0x99ec30,0x99e5a0,ENUM,0x9e54e0,0x99eae0,0x9e5c30):return
  sp=u.reg_read(UC_X86_REG_ESP);this=u.reg_read(UC_X86_REG_ECX);arg=lambda i:get(sp+4+4*i);ret=0;pop=0
  if a==0x9e5250:
   assert this==MAP;events.append(('clear',))
   if mode==1:put(BANK+4,0)
  elif a==0xa629f0:
   i=arg(0)+1;assert 1<=i<n;pop=4
   if this==BANK+0x68:ret=int(i%4!=0);events.append(('valid',i,ret))
   else:assert this==BANK+0x58;ret=0xffffff00+i;events.append(('type',i,ret))
  elif a in (0x99ebf0,0x99ec30):
   assert this not in live;live.add(this);pop=8 if a==0x99ebf0 else 4
   value=arg(0) if a==0x99ebf0 else get(arg(0));put(this,value);events.append(('string',text(value)));ret=this
   if a==0x99ebf0:assert arg(1)==0xffffffff
  elif a==0x99e5a0:
   assert this in live and arg(0)==91;pop=4;ret=text(get(this)).find(b'[')
  elif a==ENUM:
   assert this==BANK;out=arg(0);ty=arg(1);assert out not in live;live.add(out);pop=8;ret=out
   value=TEXT+0x400+(ty&1)*16;u.mem_write(value,(b'odd' if ty&1 else b'even')+b'\0');put(out,value);events.append(('enum',ty))
   if mode==2:put(BANK+4,0)
  elif a==0x9e54e0:
   assert this==MAP and arg(0) in live and arg(1) in live;pop=12;row=(arg(2),text(get(arg(1))),text(get(arg(0))));added.append(row);events.append(('add',)+row)
   if mode==3:put(BANK+4,arg(2)+1)
  elif a==0x99eae0:
   assert this in live;live.remove(this);events.append(('destroy',text(get(this))))
  else:assert this==MAP and not live;events.append(('update',))
  u.reg_write(UC_X86_REG_EAX,ret&0xffffffff);u.reg_write(UC_X86_REG_ESP,sp+4+pop);u.reg_write(UC_X86_REG_EIP,get(sp))
 u.hook_add(UC_HOOK_CODE,hook);sp=STACK+0x8000;put(sp,STOP);put(sp+4,MAP);put(sp+8,append);u.reg_write(UC_X86_REG_ECX,BANK);u.reg_write(UC_X86_REG_ESP,sp)
 regs=[UC_X86_REG_EBX,UC_X86_REG_ESI,UC_X86_REG_EDI,UC_X86_REG_EBP]
 for i,r in enumerate(regs):u.reg_write(r,0x887700+i)
 u.emu_start(0x9cc530,STOP,count=5000)
 assert u.reg_read(UC_X86_REG_EIP)==STOP and u.reg_read(UC_X86_REG_ESP)==sp+12 and not live
 assert all(u.reg_read(r)==0x887700+i for i,r in enumerate(regs))
 expected=[]
 if not(mode==1 and not append):
  for i in range(1,n):
   name=b'(Unknown)' if missing else KEYS[i]
   if (i%4!=0 if packed&1 else i%3!=0) and b'[' not in name:
    ty=0xffffff00+i if packed&2 else i*13;expected.append((i,name,b'odd' if ty&1 else b'even'))
    if mode in (2,3):break
 assert added==expected and events[-1]==('update',)
 assert sum(e==('clear',) for e in events)==int(not append)
 return events,get(BANK+4)
def compare(candidate,native):
 cases=0
 for n in range(9):
  for packed in range(4):
   for missing in range(2):
    for append in range(2):
     for mode in range(4):
      assert execute(candidate,n,packed,missing,append,mode)==execute(native,n,packed,missing,append,mode);cases+=1
 return cases
