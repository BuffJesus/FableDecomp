import struct
from unicorn import Uc,UC_ARCH_X86,UC_MODE_32,UC_HOOK_CODE
from unicorn.x86_const import *
BASE=0x2000000;HEAP=0x2100000;STACK=0x3000000;STOP=0x2200000
BOUNDARIES=(0xbfed4c,0xbfe9f9,0xbfea14,0xbfe9bc,0xbfea0e,0xbfea1a,0xbfeae6,0xbfeaf2,0xbfeaec,0xbfeb1c,0xbfeb22)
def execute(codes,path,null=False):
 u=Uc(UC_ARCH_X86,UC_MODE_32)
 for a,n in ((0x990000,0x50000),(0xbf0000,0x10000),(0x1240000,0x10000),(0x1290000,0x10000),(0x1390000,0x10000),(0x13b0000,0x10000),(BASE,0x10000),(HEAP,0x80000),(STACK,0x10000),(STOP,0x1000)):u.mem_map(a,n)
 def get(a):return int.from_bytes(u.mem_read(a,4),'little')
 def put(a,v):u.mem_write(a,struct.pack('<I',v&0xffffffff))
 for a,body in codes.items():u.mem_write(a,body)
 for a in BOUNDARIES:u.mem_write(a,b'\xc3')
 u.mem_write(HEAP,b'\x7d'*0x80000);put(0x139c8a8,0x5a5a1234);put(0x13bd800,1)
 put(BASE,0 if null else BASE+0x100);put(BASE+0x100,BASE+0x200);put(BASE+0x104,len(path));put(BASE+0x108,len(path)+1);u.mem_write(BASE+0x10c,b'\x01');put(BASE+0x10d,1);u.mem_write(BASE+0x200,path+b'\0')
 allocations={};events=[];frees=set()
 def read_string(a,wide=False):
  step=2 if wide else 1;out=bytearray()
  for i in range(1024):
   v=bytes(u.mem_read(a+i*step,step))
   if v==b'\0'*step:return bytes(out)
   out.extend(v)
  raise AssertionError('unterminated')
 def hook(u,a,size,data):
  if a not in BOUNDARIES:return
  sp=u.reg_read(UC_X86_REG_ESP);arg=get(sp+4);result=0
  if a in (0xbfea0e,0xbfea1a,0xbfeb22):
   assert 0<arg<992;result=HEAP+len(allocations)*1024+16;assert result+arg<HEAP+0x80000;allocations[result]=(arg,a);events.append(('allocate',a,arg,result))
  elif a in (0xbfea14,0xbfe9bc,0xbfeb1c):
   if arg:
    if arg in (BASE+0x100,BASE+0x200):assert arg not in frees
    else:
     assert arg in allocations and arg not in frees
     n,kind=allocations[arg];assert (kind,a) in ((0xbfea0e,0xbfea14),(0xbfea1a,0xbfe9bc),(0xbfeb22,0xbfeb1c))
     assert bytes(u.mem_read(arg-16,16))==b'\x7d'*16 and bytes(u.mem_read(arg+n,16))==b'\x7d'*16
    frees.add(arg);events.append(('free',a,arg))
  elif a==0xbfeae6:
   source=get(sp+8);length=get(sp+12);assert length<1024;u.mem_write(arg,bytes(u.mem_read(source,length)));result=arg;events.append(('memmove',arg,source,length))
  elif a==0xbfe9f9:assert u.reg_read(UC_X86_REG_ECX)==0x5a5a1234
  elif a==0xbfed4c:
   raw=read_string(arg,True);text=raw.decode('utf-16le');drive=text[:2] if len(text)>1 and text[1]==':' else '';rest=text[len(drive):];split=max(rest.rfind('/'),rest.rfind('\\'));directory=rest[:split+1];leaf=rest[split+1:];dot=leaf.rfind('.');name=leaf[:dot] if dot>=0 else leaf;ext=leaf[dot:] if dot>=0 else ''
   for i,value in enumerate((drive,directory,name,ext)):
    dest=get(sp+8+i*4)
    if dest:u.mem_write(dest,value.encode('utf-16le')+b'\0\0')
   events.append(('split',raw,drive,directory,name,ext))
  else:raise AssertionError(('unexpected failure boundary',hex(a)))
  u.reg_write(UC_X86_REG_EAX,result);u.reg_write(UC_X86_REG_ESP,sp+4);u.reg_write(UC_X86_REG_EIP,get(sp))
 u.hook_add(UC_HOOK_CODE,hook)
 def invoke(entry,this,args):
  sp=STACK+0xf000;put(sp,STOP)
  for i,v in enumerate(args):put(sp+4+i*4,v)
  u.reg_write(UC_X86_REG_ESP,sp);u.reg_write(UC_X86_REG_ECX,this);u.emu_start(entry,STOP,count=100000)
  assert u.reg_read(UC_X86_REG_EIP)==STOP and u.reg_read(UC_X86_REG_ESP)==sp+4+4*len(args)
 invoke(0x9cbe10,BASE+0x800,(BASE+4,BASE));assert u.reg_read(UC_X86_REG_EAX)==BASE+4
 ref=get(BASE+4);actual=read_string(get(ref)) if ref else b''
 expected=(b'' if null else path).replace(b'\\',b'/').rsplit(b'/',1)[-1]
 if len(expected)>1 and expected[1:2]==b':':expected=expected[2:]
 assert actual==expected,(path,actual,expected)
 assert get(BASE)==(0 if null else BASE+0x100) and bytes(u.mem_read(BASE+0x200,len(path)+1))==path+b'\0'
 assert get(0x13bca20)==0 and get(0x13bd800)==2
 invoke(0x99eae0,BASE+4,());invoke(0x99eae0,BASE,())
 assert not get(BASE) and not get(BASE+4) and get(0x13bd800)==0 and set(allocations).issubset(frees)
 return actual,events,bytes(u.mem_read(HEAP,0x80000))
