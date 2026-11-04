"""Execute CDataInputStream's ReadLine, ReadUntilCharacter, ReadUntilWhiteSpace and ReadWhiteSpace against retail."""
import itertools,struct
from unicorn import Uc,UC_ARCH_X86,UC_MODE_32,UC_HOOK_CODE
from unicorn.x86_const import *
STREAM=0x2000000;SRC=0x2001000;RET=0x2002000;FOUND=0x2002100;VT=0x2003000;STACK=0x3000000;STOP=0x4000000
SETPOS,GETPOS=0x5000000,0x5000010
READLINE,UNTILCHAR,UNTILWS,WS=0x994b50,0x994dc0,0x994e90,0x994f60
CTOR,COPY,DTOR,APPEND,OVERFLOW=0x99e4b0,0x99ec30,0x99eae0,0x99f180,0x993ca0
STUBS=(CTOR,COPY,DTOR,APPEND,OVERFLOW,SETPOS,GETPOS)
def run(codes,entry,text,position,chunk,args):
 u=Uc(UC_ARCH_X86,UC_MODE_32)
 for a,z in [(0x993000,0x1000),(0x994000,0x2000),(0x99e000,0x2000),(STREAM,0x4000),(STACK,0x10000),(STOP,0x1000),(SETPOS,0x1000)]:u.mem_map(a,z)
 def put(a,v):u.mem_write(a,struct.pack('<I',v&0xffffffff))
 def get(a):return struct.unpack('<I',u.mem_read(a,4))[0]
 for a,b in codes.items():u.mem_write(a,bytes(b))
 for a in STUBS:u.mem_write(a,b'\xc3')
 put(VT+4,SETPOS);put(VT+8,GETPOS)
 u.mem_write(STREAM,b'\xa5'*0x40);u.mem_write(SRC,text+b'\0');put(STREAM,VT)
 put(STREAM+4,position);put(STREAM+8,len(text));put(STREAM+0xc,SRC+position);put(STREAM+0x10,position);put(STREAM+0x14,min(chunk,len(text)-position))
 u.mem_write(FOUND,b'\x77')
 trace=[];strings={}
 def hook(uc,a,s,user):
  if a not in STUBS:return
  sp=uc.reg_read(UC_X86_REG_ESP);ecx=uc.reg_read(UC_X86_REG_ECX);pop=0;eax=None
  if a==CTOR:strings[ecx]=b'';trace.append(('ctor',));eax=ecx
  elif a==COPY:strings[ecx]=strings.get(get(sp+4),b'?');trace.append(('copy',ecx));eax=ecx;pop=4
  elif a==DTOR:trace.append(('dtor',))
  elif a==APPEND:strings[ecx]=strings.get(ecx,b'?')+bytes([get(sp+4)&0xff]);eax=ecx;pop=4
  elif a==OVERFLOW:
   out,n=get(sp+4),get(sp+8);pos=get(ecx+4);trace.append(('overflow',n,pos))
   u.mem_write(out,text[pos:pos+n]);put(ecx+4,pos+n);put(ecx+0xc,SRC+pos+n);put(ecx+0x14,0);pop=8
  elif a==SETPOS:
   p=get(sp+4);trace.append(('setpos',p));put(ecx+4,p);put(ecx+0xc,SRC+p);put(ecx+0x14,len(text)-p);pop=4
  else:trace.append(('getpos',));eax=get(ecx+4)
  if eax is not None:uc.reg_write(UC_X86_REG_EAX,eax)
  uc.reg_write(UC_X86_REG_EIP,get(sp));uc.reg_write(UC_X86_REG_ESP,sp+4+pop)
 u.hook_add(UC_HOOK_CODE,hook);sp=STACK+0x8000;put(sp,STOP);put(sp+4,RET)
 for i,v in enumerate(args):put(sp+8+4*i,v)
 regs=[UC_X86_REG_EBX,UC_X86_REG_ESI,UC_X86_REG_EDI,UC_X86_REG_EBP]
 for i,r in enumerate(regs):u.reg_write(r,0x778800+i)
 u.reg_write(UC_X86_REG_ECX,STREAM);u.reg_write(UC_X86_REG_ESP,sp)
 u.emu_start(entry,STOP,count=20000)
 assert u.reg_read(UC_X86_REG_ESP)==sp+8+4*len(args) and all(u.reg_read(r)==0x778800+i for i,r in enumerate(regs))
 return u.reg_read(UC_X86_REG_EAX),trace,strings.get(RET),get(STREAM+4),bytes(u.mem_read(FOUND,1))
WSET=b' \t\r\n'
def model(kind,text,pos,ch=None):
 out=b''
 if kind=='line':
  while pos<len(text):
   c=text[pos:pos+1];pos+=1
   if c==b'\r':
    if pos<len(text):
     n=text[pos:pos+1];pos+=1
     if n!=b'\n':pos-=1
    break
   if c in(b'\n',b'\0'):break
   out+=c
  return out,pos,None
 if kind=='char':
  while pos<len(text):
   c=text[pos:pos+1];pos+=1
   if c==ch:return out,pos-1,1
   out+=c
  return out,pos,0
 while pos<len(text):
  c=text[pos:pos+1];pos+=1
  if (c in WSET)==(kind=='until_ws'):return out,pos-1,None
  out+=c
 return out,pos,None
def compare(candidate,native):
 cases=0
 texts=[b'abc\r\ndef',b'abc\rdef',b'abc\ndef',b'abc\0def',b'abc',b'ab\r',b'\r\n',b'',b'key:value',b'word \tnext',b'\r\n\n x',b'::',b' a b ']
 for text,chunk in itertools.product(texts,(0,1,3,16)):
  for start in sorted({0,min(2,len(text))}):
   for entry,kind,args in ((READLINE,'line',[]),(UNTILCHAR,'char',[ord(':'),FOUND]),(UNTILWS,'until_ws',[]),(WS,'ws',[])):
    c=run(candidate,entry,text,start,chunk,args);assert c==run(native,entry,text,start,chunk,args),(hex(entry),text,start,chunk)
    eax,trace,result,pos,found=c;want,wpos,wfound=model(kind,text,start,b':')
    assert eax==RET and result==want and pos==wpos,(hex(entry),text,start,result,want,pos,wpos)
    assert trace[0]==('ctor',) and trace[-2]==('copy',RET) and trace[-1]==('dtor',)
    if kind=='char':assert found==bytes([wfound])
    cases+=1
 return cases
