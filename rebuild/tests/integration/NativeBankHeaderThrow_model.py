"""Header throw-boundary oracle; stops at CxxThrowException, no unwinding proof."""
from pathlib import Path
import sys,struct,itertools,json,hashlib
R=Path(__file__).resolve().parents[3];sys.path.insert(0,str(R/'tools/decomp_pipeline'))
from pe_oracle import EXE,read_virtual_data
from unicorn import Uc,UC_ARCH_X86,UC_MODE_32,UC_HOOK_CODE
from unicorn.x86_const import *
ENTRY=0x9d0880;A=0x2000000;BANK=A+0x100;FILE=A+0x400;VT=A+0x800;DATA=A+0x1000;STACK=0x3000000;STOP=0x4000000;API=0x5000000
image=EXE.read_bytes();code=read_virtual_data(image,ENTRY,1635)
helpers={0x99b2c0,0x994700,0x993bc0,0x993ca0,0x994780,0x99eae0,0xbfe9f9,0x9cfbc0,0x9d0380,0x99ebf0,API,API+16,API+32}

helpers.update({0x99f600,0x99f570,0x99f830,0x99e4c0,0x9d1e70,0xbfeb84})

def execute(args):
 readonly,state,alignment,chunk,result,callback=args[:6]
 validmagic,version,format,streamsize=args[6:10] if len(args)>6 else (True,7,19,32)
 throwflag=args[10] if len(args)>10 else 1
 u=Uc(UC_ARCH_X86,UC_MODE_32)
 pages={a&~4095 for a in helpers}|{0x9d0000,0x129b000,0x139c000,0x122e000,0x125c000,0x1369000,STOP,API}
 for p in pages:u.mem_map(p,4096)
 for p,n in ((A,0x4000),(STACK,0x10000)):u.mem_map(p,n)
 u.mem_write(ENTRY,code)
 for page in (0x129b000,0x122e000,0x125c000,0x1369000):u.mem_write(page,read_virtual_data(image,page,4096))
 def put(a,v):u.mem_write(a,struct.pack('<I',v&0xffffffff))
 def get(a):return struct.unpack('<I',u.mem_read(a,4))[0]
 put(BANK,VT);put(BANK+0x7c,FILE);put(BANK+0x98,16);put(BANK+0x9c,7);u.mem_write(BANK+0x8d,bytes([readonly]));put(FILE,VT+0x100)
 put(VT+0x20,API);put(VT+0x124,API+16);put(BANK+0xec,A+0x600 if callback else 0);put(A+0x600,VT+0x200);put(VT+0x208,API+32)
 magic=get(0x129b5a4) if validmagic else 0;u.mem_write(DATA,struct.pack('<8I',magic,version,format,state,alignment,123,456,128))
 sp=STACK+0x8000;put(sp,STOP);put(sp+4,throwflag);trace=[];stream=None;initial=bytes(u.mem_read(BANK,0x110));pool={0:''};exception=[];messages=[]
 def store(a,text):
  key=len(pool);pool[key]=text;put(a,key);return a
 def string(a):return pool[get(a)]
 def literal(a):
  out=bytearray()
  while u.mem_read(a+len(out),1)!=b'\0':out.extend(u.mem_read(a+len(out),1))
  return out.decode('ascii')
 def hook(uc,a,size,_):
  nonlocal stream
  if a not in helpers:
   assert ENTRY<=a<ENTRY+len(code),('unexpected code',hex(a))
   return
  esp=uc.reg_read(UC_X86_REG_ESP);cx=uc.reg_read(UC_X86_REG_ECX);pop=0;value=0xdecafbad
  if a==0x99b2c0:
   assert cx==BANK+0x84;value=get(esp+4);store(value,'fixture');pop=4;trace.append('filename')
  elif a==0x994700:
   assert get(esp+4)==FILE and get(esp+8)==0x4000;stream=cx;u.mem_write(cx,b'\0'*36);put(cx+8,streamsize);value=cx;pop=8;trace.append('stream-ctor')
  elif a==0x993bc0:
   assert cx==stream;p=get(esp+4);put(cx+4,p);put(cx+12,DATA+p);put(cx+20,chunk if p==0 else 0);pop=4;trace.append(('seek',p))
  elif a==0x993ca0:
   assert cx==stream and get(esp+8)==4;p=get(cx+4);u.mem_write(get(esp+4),bytes(u.mem_read(DATA+p,4)));put(cx+4,p+4);put(cx+12,DATA+p+4);put(cx+20,0);pop=8;trace.append(('overflow',p))
  elif a==API:assert cx==BANK;value=19;trace.append('format')
  elif a==API+16:assert cx==FILE;value=0x9000;trace.append('length')
  elif a==0x9cfbc0:
   assert cx==BANK and (get(esp+4),get(esp+8),get(esp+12))==(stream,123,456);assert get(stream+4)==128;pop=12;value=result;trace.append('entries')
  elif a==0x9d0380:
   assert cx==BANK and get(esp+4)==state;pop=4;value=result;trace.append('recovery')
  elif a==0x99ebf0:
   assert get(esp+8)==0xffffffff;store(cx,literal(get(esp+4)));pop=8;value=cx;trace.append(('message',get(esp+4)))
  elif a in (0x99f570,0x99f600):
   left=string(uc.reg_read(UC_X86_REG_EDX));right=get(esp+4);value=store(cx,left+(string(right) if a==0x99f570 else literal(right)));pop=4
  elif a==0x99f830:
   n=uc.reg_read(UC_X86_REG_EDX);value=store(cx,str(n if n<0x80000000 else n-0x100000000))
  elif a==0x99e4c0:messages.append(string(cx));value=0
  elif a==0x9d1e70:
   messages.append(string(esp+4));put(cx,get(esp+8));trace.append('char-dtor');pop=8;value=cx
  elif a==0xbfeb84:
   assert get(esp+8)==0x1369470;exception.append(get(get(esp+4)));uc.emu_stop();return
  elif a==API+32:assert cx==A+0x600;pop=4;trace.append('callback')
  elif a==0x994780:assert cx==stream;trace.append('stream-dtor')
  elif a==0x99eae0:trace.append('char-dtor')
  elif a==0xbfe9f9:
   assert cx==0;value=uc.reg_read(UC_X86_REG_EAX);trace.append('cookie')
  else:raise AssertionError(hex(a))
  uc.reg_write(UC_X86_REG_EAX,value);uc.reg_write(UC_X86_REG_ECX,0xdeadbeef);uc.reg_write(UC_X86_REG_EDX,0x87654321);uc.reg_write(UC_X86_REG_EIP,get(esp));uc.reg_write(UC_X86_REG_ESP,esp+4+pop)
 u.hook_add(UC_HOOK_CODE,hook)
 saved=(UC_X86_REG_EBX,UC_X86_REG_ESI,UC_X86_REG_EDI,UC_X86_REG_EBP)
 for i,reg in enumerate(saved):u.reg_write(reg,0x55660000+i)
 u.reg_write(UC_X86_REG_ESP,sp);u.reg_write(UC_X86_REG_ECX,BANK);u.emu_start(ENTRY,STOP,count=2000)
 assert exception,('expected throw',args,trace)
 expected_type=0 if not streamsize or not validmagic or (version==7 and format==19) else 1
 assert exception==[expected_type],(args,exception)
 expected=bytearray(initial)
 if validmagic and streamsize and version==7 and format==19:
  struct.pack_into('<I',expected,0x98,alignment);struct.pack_into('<I',expected,0xa0,32);struct.pack_into('<I',expected,0xa8,0x9000)
 assert bytes(u.mem_read(BANK,0x110))==expected
 expected_message=('fixture is not a valid resource file!' if not streamsize or not validmagic else 'fixture has the wrong bank format! (should be 7 not %d)'%version if version!=7 else 'fixture has the wrong version number! (should be 19 not %d)'%format if format!=19 else None)
 assert messages==([] if expected_message is None else [expected_message]),(args,messages,expected_message)
 assert 'stream-dtor' not in trace
 return dict(args=args,type=exception[0],messages=messages,bank=bytes(expected).hex(),trace=trace)
